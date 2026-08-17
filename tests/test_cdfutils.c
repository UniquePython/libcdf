#include "cdf_test.h"
#include "cdfutils.h"
#include "cdfmem.h"
#include "cdferr.h"

#include <string.h>

static void test_strdup_success(void)
{
    char *copy = NULL;
    const char *original = "hello, libcdf";
    cdferr err = {0};

    CDF_CHECK(cdf_strdup(original, &copy, &err));
    CDF_CHECK(copy != NULL);
    CDF_CHECK(copy != original);
    CDF_CHECK(strcmp(copy, original) == 0);
    CDF_CHECK(err.code == CDFEC_NONE);

    cdf_free(&copy);
}

static void test_strdup_empty_string(void)
{
    char *copy = NULL;
    cdferr err = {0};

    CDF_CHECK(cdf_strdup("", &copy, &err));
    CDF_CHECK(copy != NULL);
    CDF_CHECK(copy[0] == '\0');
    CDF_CHECK(err.code == CDFEC_NONE);

    cdf_free(&copy);
}

static void test_strdup_independent_copy(void)
{
    char original[] = "mutable";
    char *copy = NULL;
    cdferr err = {0};

    CDF_CHECK(cdf_strdup(original, &copy, &err));
    CDF_CHECK(copy != NULL);

    /* mutating the source must not affect the copy */
    original[0] = 'M';
    CDF_CHECK(copy[0] == 'm');

    cdf_free(&copy);
}

static void test_strdup_null_args(void)
{
    char *copy = NULL;
    cdferr err = {0};

    CDF_CHECK(!cdf_strdup(NULL, &copy, &err));
    CDF_CHECK(copy == NULL);
    CDF_CHECK(err.code == CDFEC_INVALID_ARG);

    err = (cdferr){0};
    CDF_CHECK(!cdf_strdup("abc", NULL, &err));
    CDF_CHECK(err.code == CDFEC_INVALID_ARG);

    /* NULL err must be safely accepted */
    CDF_CHECK(!cdf_strdup("abc", NULL, NULL));
}

int main(void)
{
    test_strdup_success();
    test_strdup_empty_string();
    test_strdup_independent_copy();
    test_strdup_null_args();

    CDF_TEST_REPORT();
}
