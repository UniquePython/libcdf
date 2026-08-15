#ifndef CDF_TEST_H_
#define CDF_TEST_H_

#include <stdio.h>

static int cdf_test_failures = 0;

#define CDF_CHECK(cond)                                                     \
    do                                                                      \
    {                                                                       \
        if (!(cond))                                                        \
        {                                                                   \
            fprintf(stderr, "FAIL %s:%d: %s\n", __FILE__, __LINE__, #cond); \
            cdf_test_failures++;                                            \
        }                                                                   \
    } while (0)

#define CDF_TEST_REPORT()                                               \
    do                                                                  \
    {                                                                   \
        if (cdf_test_failures == 0)                                     \
        {                                                               \
            printf("all checks passed\n");                              \
        }                                                               \
        else                                                            \
        {                                                               \
            fprintf(stderr, "%d check(s) failed\n", cdf_test_failures); \
        }                                                               \
        return cdf_test_failures == 0 ? 0 : 1;                          \
    } while (0)

#endif /* CDF_TEST_H_ */
