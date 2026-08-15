#ifndef CDF_DF_H_
#define CDF_DF_H_

/*
 * Represents an entire data frame
 */
typedef struct cdfdf_t cdfdf;

/**
 * Creates an empty data frame.
 *
 * Success:
 *   Allocates and initializes a new data frame and stores it in `*out`.
 *
 * Failure:
 *   Returns `cdf_false` if `out` is NULL or memory allocation fails. `*out`
 *   is left untouched on failure.
 */
cdf_bool cdfdf_create(cdfdf **out);

/**
 * Destroys a data frame and releases all memory owned by it.
 */
void cdfdf_destroy(cdfdf **df);

/**
 * Adds a column to the data frame and takes ownership of it.
 *
 * Success:
 *   Adds `*column` to the data frame, updates the column and row counts, and
 *   sets `*column` to NULL.
 *
 * Failure:
 *   Returns `cdf_false` if the data frame or column is NULL, the column has
 *   an incompatible number of elements, a column with the same name already
 *   exists, an allocation size overflows, or memory allocation fails. The
 *   data frame and `*column` remain unchanged on failure.
 */
cdf_bool cdfdf_add_column(cdfdf *df, cdfc **column);

#endif /* CDF_DF_H_ */
