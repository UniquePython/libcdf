#include "cdftypes.h"

cdf_bool cdfdt_new(cdfdtk kind, cdfdt *out)
{
    if (out == NULL || kind >= NCDFT)
        return cdf_false;

    out->kind = kind;

    switch (kind)
    {
    case CDFT_I8:
        out->size = sizeof(cdf_i8);
        break;

    case CDFT_I16:
        out->size = sizeof(cdf_i16);
        break;

    case CDFT_I32:
        out->size = sizeof(cdf_i32);
        break;

    case CDFT_I64:
        out->size = sizeof(cdf_i64);
        break;

    case CDFT_U8:
        out->size = sizeof(cdf_u8);
        break;

    case CDFT_U16:
        out->size = sizeof(cdf_u16);
        break;

    case CDFT_U32:
        out->size = sizeof(cdf_u32);
        break;

    case CDFT_U64:
        out->size = sizeof(cdf_u64);
        break;

    case CDFT_FLT:
        out->size = sizeof(cdf_flt);
        break;

    case CDFT_DBL:
        out->size = sizeof(cdf_dbl);
        break;

    case CDFT_BOOL:
        out->size = sizeof(cdf_bool);
        break;

    default:
        return cdf_false;
    }

    return cdf_true;
}
