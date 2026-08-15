#include "cdf_test.h"
#include "cdfmem.h"

static void test_alloc(void)
{
    void *p = NULL;

    /* zero size fails, *out left untouched */
    CDF_CHECK(!cdf_alloc(0, &p));
    CDF_CHECK(p == NULL);

    /* NULL out fails */
    CDF_CHECK(!cdf_alloc(16, NULL));

    /* normal alloc succeeds */
    CDF_CHECK(cdf_alloc(16, &p));
    CDF_CHECK(p != NULL);

    cdf_free(&p);
}

static void test_realloc(void)
{
    void *p = NULL;
    void *before;

    CDF_CHECK(cdf_alloc(16, &p));
    CDF_CHECK(p != NULL);

    /* grow succeeds */
    CDF_CHECK(cdf_realloc(64, &p));
    CDF_CHECK(p != NULL);

    /* shrink succeeds */
    CDF_CHECK(cdf_realloc(8, &p));
    CDF_CHECK(p != NULL);

    /* zero size fails, original allocation untouched */
    before = p;
    CDF_CHECK(!cdf_realloc(0, &p));
    CDF_CHECK(p == before);

    /* NULL ptr fails */
    CDF_CHECK(!cdf_realloc(16, NULL));

    cdf_free(&p);
}

static void test_free(void)
{
    void *p = NULL;

    CDF_CHECK(cdf_alloc(16, &p));
    CDF_CHECK(p != NULL);

    /* free sets the pointer to NULL */
    cdf_free(&p);
    CDF_CHECK(p == NULL);

    /* freeing again (pointer already NULL) is a safe no-op */
    cdf_free(&p);
    CDF_CHECK(p == NULL);
}

int main(void)
{
    test_alloc();
    test_realloc();
    test_free();

    CDF_TEST_REPORT();
}
