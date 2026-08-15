#include "cdfdf.h"

#include "cdftypes.h"
#include "cdfcolumn.h"

struct cdfdf_t
{
    cdfc *cols;      // All the columns present in the data frame
    cdf_usize ncols; // Number of columns present in the dataframe
    cdf_usize nrows; // Cached number of rows present in the dataframe; `nrows = cols[i].nelements (0 <= i < ncols)`
};
