#ifndef CDF_MEM_H_
#define CDF_MEM_H_

#include "cdftypes.h"

cdf_bool cdf_alloc_impl(cdf_usize size, void **out);
void cdf_free_impl(void **ptr);
cdf_bool cdf_realloc_impl(cdf_usize size, void **ptr);

/**
 * Allocates `size` bytes of uninitialized memory.
 *
 * Success:
 *   Allocates `size` bytes and stores the resulting pointer in `*out`.
 *
 * Failure:
 *   Returns `cdf_false` if `size` is zero, `out` is NULL, or allocation
 *   fails. In all failure cases, `*out` is left untouched.
 */
#define cdf_alloc(size, out) cdf_alloc_impl(size, (void **)out)

/**
 * Allocates enough uninitialized memory for one object of the type pointed to
 * by `out`.
 *
 * Success:
 *   Allocates `sizeof(**out)` bytes and stores the resulting pointer in
 *   `*out`.
 *
 * Failure:
 *   Returns `cdf_false` if `out` is NULL or allocation fails. In all failure
 *   cases, `*out` is left untouched.
 *
 * `out` must be a pointer to an object pointer (e.g. `foo **`).
 */
#define cdf_alloc_one(out) cdf_alloc(sizeof(**(out)), (out))

/**
 * Frees memory previously allocated by the CDF memory allocator.
 *
 * Safely accepts NULL and sets `*ptr` to NULL after freeing.
 */
#define cdf_free(ptr) cdf_free_impl((void **)ptr)

/**
 * Resizes an existing memory allocation.
 *
 * Success:
 *   Resizes the allocation to `size` bytes and updates `*ptr` to point to
 *   the new allocation.
 *
 * Failure:
 *   Returns `cdf_false` if `size` is zero, `ptr` is NULL, or reallocation
 *   fails. In all failure cases, the original allocation is left untouched.
 */
#define cdf_realloc(size, ptr) cdf_realloc_impl(size, (void **)ptr)

#endif /* CDF_MEM_H_ */
