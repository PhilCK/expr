#ifndef V_ARRAY_INCLUDED_
#define V_ARRAY_INCLUDED_


/* ------------------------------------------------------------- Interface -- */



/* ---------------------------------------------------- Internal Interface -- */


void *
expr_varray_create(int bytes_of_item, int count);

int
expr_varray_destroy(void **arr);


/* inc guard */
#endif


