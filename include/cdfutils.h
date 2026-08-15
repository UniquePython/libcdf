#ifndef CDF_UTILS_H_
#define CDF_UTILS_H_

#include "cdftypes.h"

/**
 * Duplicates a null-terminated string into newly allocated memory.
 *
 * Success:
 *   Allocates a copy of `str`, including its `\0`, and stores it in `*out`.
 *
 * Failure:
 *   Returns `cdf_false` if `str` or `out` is NULL, if the required allocation
 *   size overflows, or if memory allocation fails. In all failure cases,
 *   `*out` is left untouched.
 */
cdf_bool cdf_strdup(const char *str, char **out);

#endif /* CDF_UTILS_H_ */
