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

cdf_bool cdfc_create(const char *name, cdfdtk kind, cdfc **out)
{
    cdfc *column;

    if (name == NULL || out == NULL)
        return cdf_false;

    if (!cdf_alloc(sizeof(*column), &column))
        return cdf_false;

    column->name = NULL;
    column->nelements = 0;
    column->capacity = 0;
    column->elements = NULL;
    column->is_empty = NULL;

    if (!cdf_strdup(name, &column->name))
    {
        cdfc_destroy(&column);
        return cdf_false;
    }

    cdfdt type;
    if (!cdfdt_new(kind, &type))
    {
        cdfc_destroy(&column);
        return cdf_false;
    }
    column->type = type;

    *out = column;
    return cdf_true;
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

cdf_bool cdfc_reserve(cdfc *column, cdf_usize capacity)
{
    void *elements;
    cdf_bool *is_empty;
    cdf_usize element_bytes;
    cdf_usize empty_bytes;

    if (column == NULL)
        return cdf_false;

    if (capacity <= column->capacity)
        return cdf_true;

    if (column->type.size != 0 && capacity > SIZE_MAX / column->type.size)
        return cdf_false;

    if (capacity > SIZE_MAX / sizeof(*is_empty))
        return cdf_false;

    element_bytes = capacity * column->type.size;
    empty_bytes = capacity * sizeof(*is_empty);

    if (!cdf_alloc(element_bytes, &elements))
        return cdf_false;

    if (!cdf_alloc(empty_bytes, &is_empty))
    {
        cdf_free(&elements);
        return cdf_false;
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

    return cdf_true;
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

cdf_bool cdfc_append(cdfc *column, const void *element)
{
    cdf_usize index;
    cdf_usize offset;

    if (column == NULL || element == NULL)
        return cdf_false;

    if (column->nelements == column->capacity)
    {
        cdf_usize new_capacity;

        if (!cdfc_grow_capacity(column->capacity, &new_capacity))
            return cdf_false;

        if (!cdfc_reserve(column, new_capacity))
            return cdf_false;
    }

    index = column->nelements;
    offset = index * column->type.size;

    memcpy((char *)column->elements + offset, element, column->type.size);

    column->is_empty[index] = cdf_false;
    column->nelements++;

    return cdf_true;
}

cdf_bool cdfc_get(const cdfc *column, cdf_usize index, void *out)
{
    if (column == NULL || out == NULL)
        return cdf_false;

    if (index >= column->nelements)
        return cdf_false;

    if (column->is_empty[index])
        return cdf_false;

    memcpy(out, (const char *)column->elements + index * column->type.size, column->type.size);

    return cdf_true;
}

cdf_bool cdfc_set(cdfc *column, cdf_usize index, const void *value)
{
    if (column == NULL || value == NULL)
        return cdf_false;

    if (index >= column->nelements)
        return cdf_false;

    memcpy((char *)column->elements + index * column->type.size, value, column->type.size);
    column->is_empty[index] = cdf_false;

    return cdf_true;
}
