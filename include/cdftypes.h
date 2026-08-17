#ifndef CDF_TYPES_H_
#define CDF_TYPES_H_

#include <stdint.h>
#include <stdbool.h>
#include <stddef.h>

// libcdf 8 bit signed integer
typedef int8_t cdf_i8;
// libcdf 16 bit signed integer
typedef int16_t cdf_i16;
// libcdf 32 bit signed integer
typedef int32_t cdf_i32;
// libcdf 64 bit signed integer
typedef int64_t cdf_i64;
// libcdf 8 bit unsigned integer
typedef uint8_t cdf_u8;
// libcdf 16 bit unsigned integer
typedef uint16_t cdf_u16;
// libcdf 32 bit unsigned integer
typedef uint32_t cdf_u32;
// libcdf 64 bit unsigned integer
typedef uint64_t cdf_u64;
// libcdf Unsigned integer type for object sizes
typedef size_t cdf_usize;
// libcdf Single-precision floating-point number
typedef float cdf_flt;
// libcdf Double-precision floating-point number
typedef double cdf_dbl;
// libcdf Boolean value
typedef bool cdf_bool;

// libcdf true (=1)
#define cdf_true ((cdf_bool) true)
// libcdf false (=0)
#define cdf_false ((cdf_bool) false)

/*
 * Represents the possible data type kinds
 */
typedef enum cdf_data_type_kind_t
{
    CDFDTK_I8,    // 8 bit signed integer data type kind
    CDFDTK_I16,   // 16 bit signed integer data type kind
    CDFDTK_I32,   // 32 bit signed integer data type kind
    CDFDTK_I64,   // 64 bit signed integer data type kind
    CDFDTK_U8,    // 8 bit unsigned integer data type kind
    CDFDTK_U16,   // 16 bit unsigned integer data type kind
    CDFDTK_U32,   // 32 bit unsigned integer data type kind
    CDFDTK_U64,   // 64 bit unsigned integer data type kind
    CDFDTK_FLT,   // Single-precision floating-point number data type kind
    CDFDTK_DBL,   // Double-precision floating-point number data type kind
    CDFDTK_BOOL,  // Boolean value data type kind
    CDFDTK_COUNT, // Number of data type kinds

} cdfdtk;

/*
 * Represents a data type
 */
typedef struct cdf_data_type_t
{
    cdfdtk kind; // Data type kind
    cdf_u8 size; // Cached size of data type in bytes
} cdfdt;

/*
 * Forward declaration of `cdferr`, defined in cdferr.h.
 *
 * cdftypes.h cannot include cdferr.h directly: cdferr.h itself depends on
 * cdf_usize/cdf_bool from this header, which would create a circular
 * include. A forward declaration is sufficient here since cdferr is only
 * ever used behind a pointer in this header's function declarations.
 */
typedef struct cdferror_t cdferr;

/**
 * Initializes a data type descriptor for the specified data type kind.
 *
 * Success:
 *   Populates `*out` with the given `kind` and its corresponding size.
 *   `err`, if non-NULL, is cleared.
 *
 * Failure:
 *   Returns `cdf_false` if `out` is NULL or if `kind` is not a valid
 *   `cdfdtk` value. In all failure cases, `*out` is left untouched, and
 *   `err`, if non-NULL, is populated with a code and message describing
 *   the specific failure.
 */
cdf_bool cdfdt_new(cdfdtk kind, cdfdt *out, cdferr *err);

#endif /* CDF_TYPES_H_ */
