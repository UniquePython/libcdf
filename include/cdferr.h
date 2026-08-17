#ifndef CDF_ERR_H_
#define CDF_ERR_H_

#include "cdftypes.h"

#include <string.h>

/*
 * Represents the possible error codes
 */
typedef enum cdf_error_code_t
{
    CDFEC_NONE = 0,            // No error occurred
    CDFEC_INVALID_ARG,         // A required argument was NULL or otherwise invalid
    CDFEC_ALLOC_FAILED,        // Underlying memory allocation (malloc/realloc) failed
    CDFEC_SIZE_OVERFLOW,       // A required allocation size would overflow before allocation was attempted
    CDFEC_INDEX_OUT_OF_BOUNDS, // The given index is outside the valid range
    CDFEC_ELEMENT_EMPTY,       // The element at the given index has no value
    CDFEC_ROW_COUNT_MISMATCH,  // Column's element count doesn't match the data frame's row count
    CDFEC_DUPLICATE_NAME,      // A column with the given name already exists
    CDFEC_NOT_FOUND,           // No column with the given name exists

} cdfec;

/*
 * Represents a detailed error, including where it occurred
 */
typedef struct cdferror_t
{
    cdfec code;       // The error code
    const char *msg;  // A static, human-readable description of the error
    const char *file; // The source file where the error occurred (from __FILE__)
    cdf_usize line;   // The source line where the error occurred (from __LINE__)
    const char *func; // The function where the error occurred (from __func__)
} cdferr;

/**
 * Populates an error with the given code, message, and call-site location.
 *
 * `msg` must point to a string with static storage duration (e.g. a string
 * literal); it is stored by reference, not copied.
 *
 * Safely accepts a NULL `err`, in which case this is a no-op.
 *
 * `code` must not be `CDFEC_NONE`; passing `CDFEC_NONE` is treated as a
 * no-op, since `CDFEC_NONE` is reserved to mean "no error" as produced by
 * `cdferr_clear`, and must never be paired with populated location fields.
 * Use `cdferr_clear` to reset an error, not this function.
 *
 * This function should not be called directly; use the `cdferr_set` macro
 * instead, which supplies the call-site location automatically.
 */
void cdferr_set_impl(cdferr *err, cdfec code, const char *msg,
                     const char *file, cdf_usize line, const char *func);

/**
 * Populates `err` with the given code, message, and the location of the
 * call site (file, line, and function). Safely accepts a NULL `err`.
 *
 * `code` must not be `CDFEC_NONE`; see `cdferr_set_impl` for details.
 */
#define cdferr_set(err, code, msg) cdferr_set_impl((err), (code), (msg), __FILE__, (cdf_usize)__LINE__, __func__)

/**
 * Resets `err` to a clean, error-free state (`CDFEC_NONE`, all fields zeroed).
 * Safely accepts a NULL `err`, in which case this is a no-op.
 *
 * Intended to be called on every successful return from a fallible function,
 * so that `err` never holds stale information from a previous call.
 */
#define cdferr_clear(err)                                 \
    do                                                    \
    {                                                     \
        cdferr *cdferr_clear_tmp_ = (err);                \
        if (cdferr_clear_tmp_ != NULL)                    \
            memset(cdferr_clear_tmp_, 0, sizeof(cdferr)); \
    } while (0)

/**
 * printf-style format string for displaying a `cdferr` value. Pair with
 * `cdferr_arg` (for a `cdferr` value) or `cdferr_argp` (for a `cdferr *`).
 *
 * Example:
 *   cdferr err = {0};
 *   if (!cdfc_create("x", CDFDTK_I32, &col, &err))
 *       fprintf(stderr, CDFERR_FMT_LN, cdferr_arg(err));
 */
#define CDFERR_FMT "%s:%zu:%s: %s (code=%d)"
#define CDFERR_FMT_LN CDFERR_FMT "\n"
#define cdferr_arg(err) err.file, err.line, err.func, err.msg, err.code
#define cdferr_argp(errp) cdferr_arg((*errp))

/**
 * Prints a `cdferr` to stderr..
 *
 * Safely accepts a NULL `err`, in which case this is a no-op.
 */
void cdferr_print(const cdferr *err);

#define cdf_handle_null_explicit(var, err)                         \
    do                                                             \
    {                                                              \
        if ((var) == NULL)                                         \
        {                                                          \
            cdferr_set((err), CDFEC_INVALID_ARG, #var " is NULL"); \
            return cdf_false;                                      \
        }                                                          \
    } while (0)

#define cdf_handle_null_explicit_2(var1, var2, err) \
    do                                              \
    {                                               \
        cdf_handle_null_explicit(var1, err);        \
        cdf_handle_null_explicit(var2, err);        \
    } while (0)

#define cdf_handle_null(var) cdf_handle_null_explicit(var, err)

#define cdf_handle_null_2(var1, var2) \
    do                                \
    {                                 \
        cdf_handle_null(var1);        \
        cdf_handle_null(var2);        \
    } while (0)

#define cdf_handle_null_custom_explicit(var, msg, err) \
    do                                                 \
    {                                                  \
        if ((var) == NULL)                             \
        {                                              \
            cdferr_set((err), CDFEC_INVALID_ARG, msg); \
            return cdf_false;                          \
        }                                              \
    } while (0)

#define cdf_handle_null_custom_explicit_2(var1, msg1, var2, msg2, err) \
    do                                                                 \
    {                                                                  \
        cdf_handle_null_custom_explicit(var1, msg1, err);              \
        cdf_handle_null_custom_explicit(var2, msg2, err);              \
    } while (0)

#define cdf_handle_null_custom(var, msg) cdf_handle_null_custom_explicit(var, msg, err)

#define cdf_handle_null_custom_2(var1, msg1, var2, msg2) cdf_handle_null_custom_explicit_2(var1, msg1, var2, msg2, err)

#define cdf_fail_explicit(code, msg, err) \
    do                                    \
    {                                     \
        cdferr_set((err), (code), (msg)); \
        return cdf_false;                 \
    } while (0)

#define cdf_fail(code, msg) cdf_fail_explicit(code, msg, err)

#define cdf_handle_fail_explicit(cond, code, msg, err) \
    do                                                 \
    {                                                  \
        if ((cond))                                    \
            cdf_fail_explicit((code), (msg), (err));   \
    } while (0)

#define cdf_handle_fail(cond, code, msg) cdf_handle_fail_explicit(cond, code, msg, err)

#define cdf_success_explicit(err) \
    do                            \
    {                             \
        cdferr_clear((err));      \
        return cdf_true;          \
    } while (0)

#define cdf_success cdf_success_explicit(err)

#endif /* CDF_ERR_H_ */
