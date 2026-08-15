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

#endif /* CDF_DF_H_ */
