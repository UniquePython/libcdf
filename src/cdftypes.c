#include "cdftypes.h"

#include "cdferr.h"

cdf_bool cdfdt_new(cdfdtk kind, cdfdt *out, cdferr *err)
{
    cdf_handle_null(out);
    cdf_handle_fail(kind >= CDFDTK_COUNT, CDFEC_INVALID_ARG, "kind is not a valid cdfdtk value");

    out->kind = kind;

    switch (kind)
    {
    case CDFDTK_I8:
        out->size = sizeof(cdf_i8);
        break;

    case CDFDTK_I16:
        out->size = sizeof(cdf_i16);
        break;

    case CDFDTK_I32:
        out->size = sizeof(cdf_i32);
        break;

    case CDFDTK_I64:
        out->size = sizeof(cdf_i64);
        break;

    case CDFDTK_U8:
        out->size = sizeof(cdf_u8);
        break;

    case CDFDTK_U16:
        out->size = sizeof(cdf_u16);
        break;

    case CDFDTK_U32:
        out->size = sizeof(cdf_u32);
        break;

    case CDFDTK_U64:
        out->size = sizeof(cdf_u64);
        break;

    case CDFDTK_FLT:
        out->size = sizeof(cdf_flt);
        break;

    case CDFDTK_DBL:
        out->size = sizeof(cdf_dbl);
        break;

    case CDFDTK_BOOL:
        out->size = sizeof(cdf_bool);
        break;

    case CDFDTK_COUNT:
    default:
        cdf_fail(CDFEC_INVALID_ARG, "kind is not a valid cdfdtk value");
    }

    cdf_success;
}
