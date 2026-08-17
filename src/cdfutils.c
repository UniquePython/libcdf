#include "cdfutils.h"

#include "cdfmem.h"

#include <string.h>

cdf_bool cdf_strdup(const char *str, char **out, cdferr *err)
{
    cdf_usize len;
    char *copy;

    handle_null_2(str, out);

    len = strlen(str);

    handle_fail(len > SIZE_MAX - 1, CDFEC_SIZE_OVERFLOW, "string length overflows allocation size");
    handle_fail(!cdf_alloc(len + 1, &copy), CDFEC_ALLOC_FAILED, "failed to allocate memory for string copy");

    memcpy(copy, str, len + 1);

    *out = copy;
    success;
}
