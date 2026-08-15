#include "cdf_test.h"
#include "cdftypes.h"

static void check_kind(cdfdtk kind, cdf_u8 expected_size)
{
    cdfdt dt;

    CDF_CHECK(cdfdt_new(kind, &dt));
    CDF_CHECK(dt.kind == kind);
    CDF_CHECK(dt.size == expected_size);
}

static void test_all_kinds(void)
{
    check_kind(CDFDTK_I8, (cdf_u8)sizeof(cdf_i8));
    check_kind(CDFDTK_I16, (cdf_u8)sizeof(cdf_i16));
    check_kind(CDFDTK_I32, (cdf_u8)sizeof(cdf_i32));
    check_kind(CDFDTK_I64, (cdf_u8)sizeof(cdf_i64));
    check_kind(CDFDTK_U8, (cdf_u8)sizeof(cdf_u8));
    check_kind(CDFDTK_U16, (cdf_u8)sizeof(cdf_u16));
    check_kind(CDFDTK_U32, (cdf_u8)sizeof(cdf_u32));
    check_kind(CDFDTK_U64, (cdf_u8)sizeof(cdf_u64));
    check_kind(CDFDTK_FLT, (cdf_u8)sizeof(cdf_flt));
    check_kind(CDFDTK_DBL, (cdf_u8)sizeof(cdf_dbl));
    check_kind(CDFDTK_BOOL, (cdf_u8)sizeof(cdf_bool));
}

static void test_invalid_kind(void)
{
    cdfdt dt;

    /* CDFDTK_COUNT is the sentinel, not a real kind: must fail */
    CDF_CHECK(!cdfdt_new(CDFDTK_COUNT, &dt));

    /* anything past the sentinel must also fail */
    CDF_CHECK(!cdfdt_new((cdfdtk)(CDFDTK_COUNT + 1), &dt));
}

static void test_null_out(void)
{
    CDF_CHECK(!cdfdt_new(CDFDTK_I32, NULL));
}

int main(void)
{
    test_all_kinds();
    test_invalid_kind();
    test_null_out();

    CDF_TEST_REPORT();
}
