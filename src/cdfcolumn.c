#include "cdfcolumn.h"

#include "cdfmem.h"
#include "cdfutils.h"

#include <string.h>

struct cdfcolumn_t
{
    char *name; // Name of column
    cdfdt type; // Data type of column

    cdf_usize nelements; // Number of elements in column
    cdf_usize capacity;  // Number of elements that can be stored without reallocating

    void *elements;     // Data present in the column. `Total size = nelements * type.size`
    cdf_bool *is_empty; // `cdf_true` if data missing else `cdf_false`. `Length = nelements`
};

cdf_bool cdfc_create(const char *name, cdfdtk kind, cdfc **out, cdferr *err)
{
    cdfc *column;
    cdfdt type;

    cdf_handle_null_2(name, out);
    cdf_handle_fail(!cdf_alloc(sizeof(*column), &column), CDFEC_ALLOC_FAILED, "failed to allocate memory for column");

    column->name = NULL;
    column->nelements = 0;
    column->capacity = 0;
    column->elements = NULL;
    column->is_empty = NULL;

    if (!cdf_strdup(name, &column->name, err) || !cdfdt_new(kind, &type, err))
    {
        cdfc_destroy(&column);
        return cdf_false;
    }

    column->type = type;

    *out = column;
    cdf_success;
}

void cdfc_destroy(cdfc **column)
{
    if (column == NULL || *column == NULL)
        return;

    cdf_free(&(*column)->name);
    cdf_free(&(*column)->elements);
    cdf_free(&(*column)->is_empty);
    cdf_free(column);
}

cdf_bool cdfc_reserve(cdfc *column, cdf_usize capacity, cdferr *err)
{
    void *elements;
    cdf_bool *is_empty;
    cdf_usize element_bytes;
    cdf_usize empty_bytes;

    cdf_handle_null(column);

    if (capacity <= column->capacity)
        cdf_success;

    cdf_handle_fail(column->type.size != 0 && capacity > SIZE_MAX / column->type.size,
                    CDFEC_SIZE_OVERFLOW, "element buffer size overflows");

    cdf_handle_fail(capacity > SIZE_MAX / sizeof(*is_empty),
                    CDFEC_SIZE_OVERFLOW, "is_empty buffer size overflows");

    element_bytes = capacity * column->type.size;
    empty_bytes = capacity * sizeof(*is_empty);

    cdf_handle_fail(!cdf_alloc(element_bytes, &elements), CDFEC_ALLOC_FAILED, "failed to allocate element buffer");

    if (!cdf_alloc(empty_bytes, &is_empty))
    {
        cdf_free(&elements);
        cdf_fail(CDFEC_ALLOC_FAILED, "failed to allocate is_empty buffer");
    }

    /*
     * Preserve existing elements.
     */
    if (column->nelements != 0)
    {
        memcpy(elements, column->elements, column->nelements * column->type.size);
        memcpy(is_empty, column->is_empty, column->nelements * sizeof(*is_empty));
    }

    cdf_free(&column->elements);
    cdf_free(&column->is_empty);

    column->elements = elements;
    column->is_empty = is_empty;
    column->capacity = capacity;

    cdf_success;
}

static cdf_bool cdfc_grow_capacity(cdf_usize current, cdf_usize *out)
{
    if (current == 0)
    {
        *out = 8;
        return cdf_true;
    }

    if (current > SIZE_MAX / 2)
        return cdf_false;

    *out = current * 2;
    return cdf_true;
}

cdf_bool cdfc_append(cdfc *column, const void *element, cdferr *err)
{
    cdf_usize index;
    cdf_usize offset;

    cdf_handle_null_2(column, element);

    if (column->nelements == column->capacity)
    {
        cdf_usize new_capacity;

        cdf_handle_fail(!cdfc_grow_capacity(column->capacity, &new_capacity),
                        CDFEC_SIZE_OVERFLOW, "capacity growth overflows");

        if (!cdfc_reserve(column, new_capacity, err))
            return cdf_false;
    }

    index = column->nelements;
    offset = index * column->type.size;

    memcpy((char *)column->elements + offset, element, column->type.size);

    column->is_empty[index] = cdf_false;
    column->nelements++;

    cdf_success;
}

cdf_bool cdfc_get(const cdfc *column, cdf_usize index, void *out, cdferr *err)
{
    cdf_handle_null_2(column, out);
    cdf_handle_fail(index >= column->nelements, CDFEC_INDEX_OUT_OF_BOUNDS, "index is out of bounds");
    cdf_handle_fail(column->is_empty[index], CDFEC_ELEMENT_EMPTY, "element at index is empty");
    memcpy(out, (const char *)column->elements + index * column->type.size, column->type.size);
    cdf_success;
}

cdf_bool cdfc_set(cdfc *column, cdf_usize index, const void *value, cdferr *err)
{
    cdf_handle_null_2(column, value);
    cdf_handle_fail(index >= column->nelements, CDFEC_INDEX_OUT_OF_BOUNDS, "index is out of bounds");

    memcpy((char *)column->elements + index * column->type.size, value, column->type.size);
    column->is_empty[index] = cdf_false;

    cdf_success;
}

cdf_usize cdfc_nelements(const cdfc *column)
{
    if (column == NULL)
        return 0;

    return column->nelements;
}

const char *cdfc_name(const cdfc *column)
{
    if (column == NULL)
        return NULL;

    return column->name;
}
