
#include <stdlib.h>
#include <malloc.h>

/*
**  ALLOC_BLOCK_SIZE - adjust this size
*/

#define ALLOC_BLOCK_SIZE 512000 /*(62*1024)*/

typedef struct
{
    long dummy;
} alloc_handle_t;

/*
**  alloc_hdr_t - Header for each block of memory.
*/
typedef struct alloc_hdr_s
{
    struct alloc_hdr_s *next; /* Next Block          */
    char *block,              /* Start of block      */
        *free,                /* Next free in block  */
        *end;                 /* block + block size  */
} alloc_hdr_t;

typedef struct alloc_root_s
{
    alloc_hdr_t *first, /* First header in pool */
        *current;       /* Current header       */
} alloc_root_t;

alloc_handle_t *p_alloc_init(void);
char *p_alloc(long);
alloc_handle_t *p_alloc_set_pool(alloc_handle_t *);
void p_alloc_reset(void);
void p_alloc_free_pool(void);

static alloc_root_t *root;
static alloc_hdr_t *p_allocHdr(void);

static alloc_hdr_t *p_allocHdr()
{
    alloc_hdr_t *hdr;
    char *block;

    block = (char *)malloc(ALLOC_BLOCK_SIZE);
    hdr = (alloc_hdr_t *)malloc(sizeof(alloc_hdr_t));

    if (hdr == NULL || block == NULL)
        return (NULL);
    hdr->block = block;
    hdr->free = block;
    hdr->next = NULL;
    hdr->end = block + ALLOC_BLOCK_SIZE;

    return (hdr);
}

/*
**  p_alloc_init()
**
**  Create a new memory pool with one block.
**  Returns pointer to the new pool.
*/

alloc_handle_t *p_alloc_init()
{
    alloc_handle_t *newpool;

    root = (alloc_root_t *)malloc(sizeof(alloc_root_t));
    if (root == NULL)
        return (NULL);
    if ((root->first = p_allocHdr()) == NULL)
        return (NULL);
    root->current = root->first;
    newpool = (alloc_handle_t *)root;
    return (newpool);
}

/*
**  p_alloc()
**
**  Use as a direct replacement for malloc().  p_allocates
**  memory from the current pool.
*/

char *p_alloc(long size)
{
    alloc_hdr_t *hdr = root->current;
    char *ptr;

    /*
    **  Align to 4 byte boundary - should be ok for most machines.
    **  Change this if your machine has weird alignment requirements.
    */
    size = (size + 3) & 0xfffffffc;

    ptr = hdr->free;
    hdr->free += size;

    /* Check if the current block is exhausted. */

    if (hdr->free >= hdr->end)
    {
        /* Is the next block already allocated? */

        if (hdr->next != NULL)
        {
            /* re-use block */
            hdr->next->free = hdr->next->block;
            root->current = hdr->next;
        }
        else
        {
            /* extend the pool with a new block */
            if ((hdr->next = p_allocHdr()) == NULL)
                return (NULL);
            root->current = hdr->next;
        }

        /* set ptr to the first location in the next block */
        ptr = root->current->free;
        root->current->free += size;
    }

    /* Return pointer to allocated memory. */

    return (ptr);
}

/*
**  p_alloc_set_pool()
**
**  Change the current pool.  Return the old pool.
*/

alloc_handle_t *p_alloc_set_pool(alloc_handle_t *newpool)
{
    alloc_handle_t *old = (alloc_handle_t *)root;
    root = (alloc_root_t *)newpool;
    return (old);
}

/*
**  p_alloc_reset()
**
**  Reset the current pool for re-use.  No memory is freed,
**  so this is very fast.
*/

void p_alloc_reset()
{
    root->current = root->first;
    root->current->free = root->current->block;
}

/*
**  p_alloc_free_pool()
**
**  Free the memory used by the current pool.
**  Don't use where p_alloc_reset() could be used.
*/

void p_alloc_free_pool()
{
    alloc_hdr_t *tmp,
        *hdr = root->first;

    while (hdr != NULL)
    {
        tmp = hdr->next;
        free((char *)hdr->block);
        free((char *)hdr);
        hdr = tmp;
    }
    free((char *)root);
    root = NULL;
}