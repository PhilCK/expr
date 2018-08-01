#ifndef FUNDAMENTAL_INCLUDED_
#define FUNDAMENTAL_INCLUDED_


/* ------------------------------------------------------------- [ Array ] -- */


#define EXPR_ARR_COUNT(arr) (sizeof(arr) / sizeof(arr[0]))


/* ----------------------------------------------------------- [ Min Max ] -- */


#define EXPR_MIN(a, b) (a < b ? a : b)
#define EXPR_MAX(a, b) (a < b ? b : a)


/* ------------------------------------------------------ [ Scratch Code ] -- */ 


#define EXPR_VARR_CREATE(count) /* do nothing */
#define EXPR_VARR_PUSH(arr) /* do nothing */
#define EXPR_VARR_DESTROY(arr) /* do nothing */


/* inc guard */
#endif

