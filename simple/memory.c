#include <stdio.h>
#include <stdlib.h>

typedef long long address_t;
typedef struct mempool_s
{
    struct mempool_s **owner;
    address_t free_offset;
    address_t element_size;
    address_t element_count;
    void *data;
} mempool_t;

// address_t fixed_pool_alloc(mempool_t *pool)
// {
//     address_t offset = pool->free_offset;
//     address_t *element = (address_t *)((int)&pool->data + offset);
//     pool->free_offset = *element;
//     return offset;
// }

void fixed_pool_free(mempool_t *pool, address_t offset)
{
    address_t *element = (address_t *)((address_t)&pool->data + offset);
    *element = pool->free_offset;
    pool->free_offset = offset;
}

void *fixed_pool_get(mempool_t *pool, address_t offset)
{
    address_t *address = (address_t *)((address_t)&pool->data + offset);
    return (void *)address;
}

void fixed_pool_init(mempool_t **owner, address_t element_size, address_t initial_element_count)
{
    address_t actualElementSize = (element_size * initial_element_count);
    address_t actualSize = actualElementSize + sizeof(mempool_t);
    mempool_t *data = (mempool_t *)malloc(actualSize);
    data->free_offset = 0;
    data->element_size = element_size;
    data->element_count = initial_element_count;
    data->owner = owner;
    address_t address = (address_t)&data->data;
    address_t offset = 0;
    //setup free list
    for (address_t i = 0; i < initial_element_count - 1; i++)
    {
        offset += element_size;
        *(address_t *)address = offset;
        address += element_size;
    }
    *(address_t *)address = -1;
    //assign owner to the new address
    *owner = data;
}

address_t fixed_pool_alloc(mempool_t *pool)
{
    address_t offset = pool->free_offset;
    if (offset == -1)
    {
        address_t oldCount = pool->element_count;
        address_t newCount = oldCount * 2;
        address_t newElementSize = (newCount * pool->element_size);
        address_t newSize = newElementSize + sizeof(mempool_t);
        printf("out of memory, reallocating as size %i\n", newSize);
        pool = (mempool_t *)realloc((void *)pool, newSize);
        pool->element_count = newCount;
        offset = pool->free_offset = (oldCount + 1) * pool->element_size;
        address_t address = (address_t)&pool->data + offset;
        //setup linked list in the new block
        for (address_t i = oldCount; i < newCount - 1; i++)
        {
            offset += pool->element_size;
            *(address_t *)address = offset;
            address += pool->element_size;
        }
        *(address_t *)address = -1;
        offset = pool->free_offset;

        //rewrite owner's reference address
        *pool->owner = pool;
    }

    address_t *element = (address_t *)((address_t)&pool->data + offset);
    pool->free_offset = *element;
    return offset;
}
