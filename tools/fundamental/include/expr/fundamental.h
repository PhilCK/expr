#ifndef FUNDAMENTAL_INCLUDED_
#define FUNDAMENTAL_INCLUDED_


/* ------------------------------------------------------------- [ Array ] -- */


#define EX_ARR_COUNT(arr) (sizeof(arr) / sizeof(arr[0]))
#define EX_ARR_DATA(arr) &arr[0]


/* ----------------------------------------------------------- [ Min Max ] -- */


#define EXPR_MIN(a, b) (a < b ? a : b)
#define EXPR_MAX(a, b) (a < b ? b : a)


/* inc guard */
#endif

