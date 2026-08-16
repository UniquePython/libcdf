#include "cdferr.h"

#include <stdio.h>

void cdferr_set_impl(cdferr *err, cdfec code, const char *msg, const char *file, cdf_usize line, const char *func)
{
    if (err == NULL || code == CDFEC_NONE)
        return;

    err->code = code;
    err->msg = msg;
    err->file = file;
    err->line = line;
    err->func = func;
}

void cdferr_print(const cdferr *err)
{
    if (err == NULL)
        return;

    fprintf(stderr, CDFERR_FMT_LN, cdferr_argp(err));
}
