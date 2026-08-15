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
 * Represents the possible data types for a data frame column
 */
typedef enum cdftype_t
{
    CDFT_I8,   // 8 bit signed integer data frame column data type
    CDFT_I16,  // 16 bit signed integer data frame column data type
    CDFT_I32,  // 32 bit signed integer data frame column data type
    CDFT_I64,  // 64 bit signed integer data frame column data type
    CDFT_U8,   // 8 bit unsigned integer data frame column data type
    CDFT_U16,  // 16 bit unsigned integer data frame column data type
    CDFT_U32,  // 32 bit unsigned integer data frame column data type
    CDFT_U64,  // 64 bit unsigned integer data frame column data type
    CDFT_FLT,  // Single-precision floating-point number data frame column data type
    CDFT_DBL,  // Double-precision floating-point number data frame column data type
    CDFT_BOOL, // Boolean value data frame column data type
    NCDFT,     // Number of data frame column data types

} cdftype;

#endif /* CDF_TYPES_H_ */
