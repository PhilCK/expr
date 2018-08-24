#ifndef V_ARRAY_INCLUDED_
#define V_ARRAY_INCLUDED_


/* ------------------------------------------------------------- Interface -- */

#define ex_varr_create(arr, count) do {                           \
                arr = expr_varray_create(sizeof(arr[0]), count);  \
        } while(0)                                                \

#define ex_varr_push(arr, push) do {                              \
                push = expr_varray_push(arr);                     \
        } while(0)                                                \

/* ---------------------------------------------------- Internal Interface -- */


void *
expr_varray_create(int bytes_of_item, int count);

int
expr_varray_destroy(void *arr);


void *
expr_varray_push(void *arr);


/* inc guard */
#endif


