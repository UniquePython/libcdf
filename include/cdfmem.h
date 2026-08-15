#ifndef CDF_MEM_H_
#define CDF_MEM_H_

#include "cdftypes.h"

cdf_bool cdf_alloc_impl(cdf_usize size, void **out);
void cdf_free_impl(void **ptr);

#define cdf_alloc(size, out) cdf_alloc_impl(size, (void **)out)
#define cdf_free(ptr) cdf_free_impl((void **)ptr)

#endif /* CDF_MEM_H_ */
