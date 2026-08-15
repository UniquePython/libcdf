#include "cdfcolumn.h"

#include "cdftypes.h"

struct cdfcolumn_t
{
    char *name;         // Name of column
    cdfdt type;         // Data type of column
    cdf_usize nentries; // Number of entries in column
    void *entries;      // Data present in the column. `Total size = nentries * type.size`
    cdf_bool *is_empty; // `cdf_true` if data missing else `cdf_false`. `Length = nentries`
};
