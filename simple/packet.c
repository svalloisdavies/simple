#include <stdio.h>
#include "packet.h"

char *mac_toa_r(const unsigned char *addr, char *str, size_t size)
{
    if (addr == NULL || str == NULL || size < 18)
        return NULL;

    snprintf(str, size, "%02x:%02x:%02x:%02x:%02x:%02x",
             addr[0], addr[1], addr[2], addr[3], addr[4], addr[5]);

    return str;
}