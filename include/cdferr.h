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

/**
 * Sets `err` to the given code and message (via `cdferr_set`) and returns
 * `cdf_false` from the current function.
 *
 * Intended for the common case where a failure should immediately abort the
 * calling function. Requires a local variable named `err` of type `cdferr *`
 * in scope; use `cdf_fail_explicit` if a different name is in scope.
 */
#define cdf_fail(code, msg) cdf_fail_explicit(code, msg, err)

/**
 * Like `cdf_fail`, but takes `err` explicitly instead of assuming a local
 * variable of that name is in scope.
 */
#define cdf_fail_explicit(code, msg, err) \
    do                                    \
    {                                     \
        cdferr_set((err), (code), (msg)); \
        return cdf_false;                 \
    } while (0)

/**
 * Clears `err` (via `cdferr_clear`) and returns `cdf_true` from the current
 * function.
 *
 * Intended to be the final statement of a successful fallible function, so
 * that `err` never holds stale information from a previous call. Requires a
 * local variable named `err` of type `cdferr *` in scope; use
 * `cdf_success_explicit` if a different name is in scope.
 */
#define cdf_success cdf_success_explicit(err)

/**
 * Like `cdf_success`, but takes `err` explicitly instead of assuming a local
 * variable of that name is in scope.
 */
#define cdf_success_explicit(err) \
    do                            \
    {                             \
        cdferr_clear((err));      \
        return cdf_true;          \
    } while (0)

/**
 * If `cond` is true, fails with the given code and message (see `cdf_fail`).
 * Otherwise, does nothing and execution continues normally.
 *
 * Requires a local variable named `err` of type `cdferr *` in scope; use
 * `cdf_handle_fail_explicit` if a different name is in scope.
 */
#define cdf_handle_fail(cond, code, msg) cdf_handle_fail_explicit(cond, code, msg, err)

/**
 * Like `cdf_handle_fail`, but takes `err` explicitly instead of assuming a
 * local variable of that name is in scope.
 */
#define cdf_handle_fail_explicit(cond, code, msg, err) \
    do                                                 \
    {                                                  \
        if ((cond))                                    \
            cdf_fail_explicit((code), (msg), (err));   \
    } while (0)

/**
 * If `var` is NULL, fails with `CDFEC_INVALID_ARG` and a message of the form
 * "`var` is NULL" (see `cdf_fail`). Otherwise, does nothing.
 *
 * Requires a local variable named `err` of type `cdferr *` in scope; use
 * `cdf_handle_null_explicit` if a different name is in scope. For a custom
 * message, use `cdf_handle_null_custom` instead.
 */
#define cdf_handle_null(var) cdf_handle_null_explicit(var, err)

/**
 * Like `cdf_handle_null`, but takes `err` explicitly instead of assuming a
 * local variable of that name is in scope.
 */
#define cdf_handle_null_explicit(var, err)                         \
    do                                                             \
    {                                                              \
        if ((var) == NULL)                                         \
        {                                                          \
            cdferr_set((err), CDFEC_INVALID_ARG, #var " is NULL"); \
            return cdf_false;                                      \
        }                                                          \
    } while (0)

/**
 * Applies `cdf_handle_null` to each of `var1` and `var2`, in order.
 */
#define cdf_handle_null_2(var1, var2) \
    do                                \
    {                                 \
        cdf_handle_null(var1);        \
        cdf_handle_null(var2);        \
    } while (0)

/**
 * Like `cdf_handle_null_2`, but takes `err` explicitly instead of assuming a
 * local variable of that name is in scope.
 */
#define cdf_handle_null_explicit_2(var1, var2, err) \
    do                                              \
    {                                               \
        cdf_handle_null_explicit(var1, err);        \
        cdf_handle_null_explicit(var2, err);        \
    } while (0)

/**
 * Applies `cdf_handle_null` to each of `var1`, `var2`, and `var3`, in order.
 */
#define cdf_handle_null_3(var1, var2, var3) cdf_handle_null_explicit_3(var1, var2, var3, err)

/**
 * Like `cdf_handle_null_3`, but takes `err` explicitly instead of assuming a
 * local variable of that name is in scope.
 */
#define cdf_handle_null_explicit_3(var1, var2, var3, err) \
    do                                                    \
    {                                                     \
        cdf_handle_null_explicit(var1, err);              \
        cdf_handle_null_explicit(var2, err);              \
        cdf_handle_null_explicit(var3, err);              \
    } while (0)

/**
 * Like `cdf_handle_null`, but fails with `msg` instead of a canned
 * "`var` is NULL" message.
 *
 * Requires a local variable named `err` of type `cdferr *` in scope; use
 * `cdf_handle_null_custom_explicit` if a different name is in scope.
 */
#define cdf_handle_null_custom(var, msg) cdf_handle_null_custom_explicit(var, msg, err)

/**
 * Like `cdf_handle_null_custom`, but takes `err` explicitly instead of
 * assuming a local variable of that name is in scope.
 */
#define cdf_handle_null_custom_explicit(var, msg, err) \
    do                                                 \
    {                                                  \
        if ((var) == NULL)                             \
        {                                              \
            cdferr_set((err), CDFEC_INVALID_ARG, msg); \
            return cdf_false;                          \
        }                                              \
    } while (0)

/**
 * Applies `cdf_handle_null_custom` to `(var1, msg1)` and then `(var2, msg2)`,
 * in order.
 */
#define cdf_handle_null_custom_2(var1, msg1, var2, msg2) cdf_handle_null_custom_explicit_2(var1, msg1, var2, msg2, err)

/**
 * Like `cdf_handle_null_custom_2`, but takes `err` explicitly instead of
 * assuming a local variable of that name is in scope.
 */
#define cdf_handle_null_custom_explicit_2(var1, msg1, var2, msg2, err) \
    do                                                                 \
    {                                                                  \
        cdf_handle_null_custom_explicit(var1, msg1, err);              \
        cdf_handle_null_custom_explicit(var2, msg2, err);              \
    } while (0)

/**
 * Applies `cdf_handle_null_custom` to `(var1, msg1)`, `(var2, msg2)`, and
 * `(var3, msg3)`, in order.
 */
#define cdf_handle_null_custom_3(var1, msg1, var2, msg2, var3, msg3) \
    cdf_handle_null_custom_explicit_3(var1, msg1, var2, msg2, var3, msg3, err)

/**
 * Like `cdf_handle_null_custom_3`, but takes `err` explicitly instead of
 * assuming a local variable of that name is in scope.
 */
#define cdf_handle_null_custom_explicit_3(var1, msg1, var2, msg2, var3, msg3, err) \
    do                                                                             \
    {                                                                              \
        cdf_handle_null_custom_explicit(var1, msg1, err);                          \
        cdf_handle_null_custom_explicit(var2, msg2, err);                          \
        cdf_handle_null_custom_explicit(var3, msg3, err);                          \
    } while (0)

#endif /* CDF_ERR_H_ */
