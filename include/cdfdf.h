#ifndef CDF_DF_H_
#define CDF_DF_H_

#include "cdftypes.h"
#include "cdferr.h"
#include "cdfcolumn.h"

/*
 * Represents an entire data frame
 */
typedef struct cdfdf_t cdfdf;

/**
 * Creates an empty data frame.
 *
 * Success:
 *   Allocates and initializes a new data frame and stores it in `*out`.
 *   `err`, if non-NULL, is cleared.
 *
 * Failure:
 *   Returns `cdf_false` if `out` is NULL or memory allocation fails. `*out`
 *   is left untouched on failure, and `err`, if non-NULL, is populated with
 *   a code and message describing the specific failure.
 */
cdf_bool cdfdf_create(cdfdf **out, cdferr *err);

/**
 * Destroys a data frame and releases all memory owned by it.
 */
void cdfdf_destroy(cdfdf **df);

/**
 * Adds a column to the data frame and takes ownership of it.
 *
 * Success:
 *   Adds `*column` to the data frame, updates the column and row counts, and
 *   sets `*column` to NULL. `err`, if non-NULL, is cleared.
 *
 * Failure:
 *   Returns `cdf_false` if the data frame or column is NULL, the column has
 *   an incompatible number of elements, a column with the same name already
 *   exists, an allocation size overflows, or memory allocation fails. The
 *   data frame and `*column` remain unchanged on failure, and `err`, if
 *   non-NULL, is populated with a code and message describing the specific
 *   failure.
 */
cdf_bool cdfdf_add_column(cdfdf *df, cdfc **column, cdferr *err);

/**
 * Retrieves a non-owning, read-only pointer to a column at the specified index.
 *
 * Success:
 *   Stores the requested column in `*out`. The caller does not own the
 *   returned column and must not destroy it. `err`, if non-NULL, is cleared.
 *
 * Failure:
 *   Returns `cdf_false` if `df` or `out` is NULL, or if `index` is out of
 *   bounds. `*out` is left untouched on failure, and `err`, if non-NULL, is
 *   populated with a code and message describing the specific failure.
 */
cdf_bool cdfdf_column(const cdfdf *df, cdf_usize index, const cdfc **out, cdferr *err);

/**
 * Retrieves a non-owning, mutable pointer to a column at the specified index.
 *
 * Success:
 *   Stores the requested column in `*out`. The caller does not own the
 *   returned column and must not destroy it. `err`, if non-NULL, is cleared.
 *
 * Failure:
 *   Returns `cdf_false` if `df` or `out` is NULL, or if `index` is out of
 *   bounds. `*out` is left untouched on failure, and `err`, if non-NULL, is
 *   populated with a code and message describing the specific failure.
 */
cdf_bool cdfdf_column_mut(cdfdf *df, cdf_usize index, cdfc **out, cdferr *err);

/**
 * Retrieves a non-owning, read-only pointer to a column with the specified name.
 *
 * Success:
 *   Stores the matching column in `*out`. The caller does not own the
 *   returned column and must not destroy it. `err`, if non-NULL, is cleared.
 *
 * Failure:
 *   Returns `cdf_false` if `df`, `name`, or `out` is NULL, or if no column
 *   with the specified name exists. `*out` is left untouched on failure,
 *   and `err`, if non-NULL, is populated with a code and message describing
 *   the specific failure.
 */
cdf_bool cdfdf_column_by_name(const cdfdf *df, const char *name, const cdfc **out, cdferr *err);

/**
 * Retrieves a non-owning, mutable pointer to a column with the specified name.
 *
 * Success:
 *   Stores the matching column in `*out`. The caller does not own the
 *   returned column and must not destroy it. `err`, if non-NULL, is cleared.
 *
 * Failure:
 *   Returns `cdf_false` if `df`, `name`, or `out` is NULL, or if no column
 *   with the specified name exists. `*out` is left untouched on failure,
 *   and `err`, if non-NULL, is populated with a code and message describing
 *   the specific failure.
 */
cdf_bool cdfdf_column_by_name_mut(cdfdf *df, const char *name, cdfc **out, cdferr *err);

#endif /* CDF_DF_H_ */
