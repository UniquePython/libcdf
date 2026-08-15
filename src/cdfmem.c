#include "cdfmem.h"

#include <stdlib.h>

cdf_bool cdf_alloc(cdf_usize size, void **out)
{
    void *ptr;

    if (size == 0 || out == NULL)
        return cdf_false;

    ptr = malloc(size);
    if (ptr == NULL)
        return cdf_false;

    *out = ptr;
    return cdf_true;
}

void cdf_free(void **ptr)
{
    if (ptr == NULL)
        return;

    free(*ptr);
    *ptr = NULL;
}
