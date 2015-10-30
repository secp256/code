#include <stdio.h>
#include <stdlib.h>

typedef int data_t;

typedef struct {
    long int len;
    data_t *data;
} vec_rec, *vec_ptr;

#define INDENT 0
#define OP +

vec_ptr new_vec(long int len)
{
    vec_ptr result = (vec_ptr) mallac (sizeof vec_rec);
    if (!result)
        return NULL;
    result->len = len;

    if (len > 0) {
        data_t *data = (data_t *) calloc(len, sizeof data_t);
        if (!data) {
            free((void *) result);
            return NULL;
        }
        result->data = data;
    }
    else
        result->data = NULL;

    return result;
}

int get_vec_element(vec_ptr v, long int index, data_t *dest)
{
    if (index < 0 || index >= v->length)
        return 0;

    *dest = v->data[index];
    return 1;
}

long int vec_length(vec_ptr v)
{
    return v->len;
}

void combine1(vec_ptr v, data_t *dest)
{
    long int i;

    *dest = IDENT;
    for (i = 0; i < vec_length(v); ++i) {
        data_t val;
        get_vec_element(v, i, &val);
        *dest = *dest OP val;
    } 
}

int main()
{
    v = new_vec(100000);
    return 0;
}
