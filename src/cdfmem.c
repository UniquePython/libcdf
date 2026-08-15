#include "cdfmem.h"

#include <stdlib.h>

cdf_bool cdf_alloc_impl(cdf_usize size, void **out)
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

void cdf_free_impl(void **ptr)
{
    if (ptr == NULL)
        return;

    free(*ptr);
    *ptr = NULL;
}

cdf_bool cdf_realloc_impl(cdf_usize size, void **ptr)
{
    void *new_ptr;

    if (size == 0 || ptr == NULL)
        return cdf_false;

    new_ptr = realloc(*ptr, size);

    if (new_ptr == NULL)
        return cdf_false;

    *ptr = new_ptr;
    return cdf_true;
}
