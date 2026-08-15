#ifndef CDF_MEM_H_
#define CDF_MEM_H_

#include "cdftypes.h"

cdf_bool cdf_alloc(cdf_usize size, void **out);
void cdf_free(void **ptr);

#endif /* CDF_MEM_H_ */
