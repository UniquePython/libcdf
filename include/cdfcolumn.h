#ifndef CDF_COLUMN_H_
#define CDF_COLUMN_H_

#include "cdftypes.h"

/*
 * Represents a single data frame column
 */
typedef struct cdfcolumn_t cdfc;

/**
 * Creates an empty column with the specified name and data type kind.
 *
 * Success:
 *   Allocates and initializes a new column, duplicates `name`, sets its data
 *   type kind to `kind`, and stores it in `*out`.
 *
 * Failure:
 *   Returns `cdf_false` if `name` or `out` is NULL, if memory allocation
 *   fails, or if the column name cannot be duplicated. In all failure cases,
 *   `*out` is left untouched.
 */
cdf_bool cdfc_create(const char *name, cdfdtk kind, cdfc **out);

/**
 * Destroys a column and releases all memory owned by it.
 */
void cdfc_destroy(cdfc **column);

/**
 * Appends an element to the end of the column.
 *
 * Success:
 *   Copies `element` into the column, marks it as non-empty, and increments
 *   the number of elements.
 *
 * Failure:
 *   Returns `cdf_false` if `column` or `element` is NULL, or if memory
 *   allocation fails. The column remains unchanged on failure.
 */
cdf_bool cdfc_append(cdfc *column, const void *element);

/**
 * Copies the element at the specified index into the output buffer.
 *
 * Success:
 *   Copies the element into `out` and returns `cdf_true`.
 *
 * Failure:
 *   Returns `cdf_false` if `column` or `out` is NULL, if `index` is out of
 *   bounds, or if the element at `index` is empty. `out` is left untouched.
 */
cdf_bool cdfc_get(const cdfc *column, cdf_usize index, void *out);

/**
 * Sets the element at the specified index to the given value.
 *
 * Success:
 *   Copies `value` into the element at `index`, marks the element as
 *   non-empty, and returns `cdf_true`.
 *
 * Failure:
 *   Returns `cdf_false` if `column` or `value` is NULL, or if `index` is out
 *   of bounds. The column remains unchanged on failure.
 */
cdf_bool cdfc_set(cdfc *column, cdf_usize index, const void *value);

#endif /* CDF_COLUMN_H_ */
