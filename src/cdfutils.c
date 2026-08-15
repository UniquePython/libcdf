#include "cdfutils.h"

#include <string.h>

cdf_bool cdf_strdup(const char *str, char **out)
{
    cdf_usize len;
    char *copy;

    if (str == NULL || out == NULL)
        return cdf_false;

    len = strlen(str);

    if (len > SIZE_MAX - 1)
        return cdf_false;

    if (!cdf_alloc(len + 1, (void **)&copy))
        return cdf_false;

    memcpy(copy, str, len + 1);

    *out = copy;
    return cdf_true;
}
