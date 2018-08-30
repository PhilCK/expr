#ifndef CSV_INCLUDED
#define CSV_INCLUDED


struct expr_csv_data;


enum expr_csv_identifiers {
        /* errors */
        EXPR_CSV_OK,
        EXPR_CSV_FAIL,
        EXPR_CSV_INVALID_PARAM,
        EXPR_CSV_INVALID_DESC,
        
        /* struct ids */
        EXPR_CSV_STRUCT_IMPORT,
        EXPR_CSV_STRUCT_CHECK,
        EXPR_CSV_STRUCT_FETCH,

        /* csv source */
        EXPR_CSV_SOURCE_FILE,
        EXPR_CSV_SOURCE_STRING,

        /* fetch selector */
        EXPR_CSV_FETCH_ROW,
        EXPR_CSV_FETCH_COLUMN,

        /* cell data types */
        EXPR_CSV_CELL_TYPE_STR,
        EXPR_CSV_CELL_TYPE_INT,
        EXPR_CSV_CELL_TYPE_INT2,
        EXPR_CSV_CELL_TYPE_INT3,
        EXPR_CSV_CELL_TYPE_INT4,
        EXPR_CSV_CELL_TYPE_FLOAT,
        EXPR_CSV_CELL_TYPE_FLOAT2,
        EXPR_CSV_CELL_TYPE_FLOAT3,
        EXPR_CSV_CELL_TYPE_FLOAT4,
        EXPR_CSV_CELL_TYPE_BOOL,
        EXPR_CSV_CELL_TYPE_BOOL2,
        EXPR_CSV_CELL_TYPE_BOOL3,
        EXPR_CSV_CELL_TYPE_BOOL4,
};


/* -------------------------------------------------------------- Lifetime -- */


struct expr_csv_import_desc {
        int type_id;
        void *ext;

        int source;
        const char *data;
        const char *delimiter;
};


/* load a csv file or source
 *  returns EXPR_CSV_INVALID_PARAM if desc or csv are null
 *  returns EXPR_CSV_INVALID_DESC if desc type id is not EXPR_CSV_STRUCT_IMPORT
 *  returns EXPR_CSV_INVALID_DESC if desc data is null;
 *  returns EXPR_CSV_INVALID_DESC if desc source is not a expr_csv_source
 *  returns EXPR_CSV_FAIL if file was not loaded
 *  returns EXPR_CSV_FAIL if unable to allocate memory.
 *  returns EXPR_CSV_FAIL if AST or Tokens couldn't be generated.
 *  returns EXPR_CSV_OK on success.
 */
int
expr_csv_create(
        struct expr_csv_import_desc *desc,
        struct expr_csv_data **csv);


/* free's a csv file or source 
 *  returns EXPR_CSV_INVALID_PARAM if csv is null
 *  returns EXPR_CSV_OK on success
 */
int
expr_csv_destroy(
        struct expr_csv_data **csv);


/* ------------------------------------------------------------- Integrity -- */


struct expr_csv_check_desc {
        int type_id;
        void *ext;

        struct expr_csv_data *csv;
};


struct expr_csv_integrity {
        int has_header_row;
        int uniform_column_types;
        int uniform_row_cell_count;
};


/* returns EXPR_CSV_INVALID_PARAM if desc or out_desc is null
 * returns EXPR_CSV_INVALID_DESC if desc type id is not EXPR_CSV_STRUCT_CHECK
 * returns EXPR_CSV_INVALID_DESC if desc csv is invalid
 * returns EXPR_CSV_OK on success
 */
int
expr_csv_check(
        struct expr_csv_check_desc *desc,
        struct expr_csv_integrity *out_desc);


/* ------------------------------------------------------------------ Data -- */


/*
 *  type_id should be EXPR_CSV_STRUCT_FETCH
 *  ext unused
 *  fetch_type a 'fetch selector' identifier
 *  selection an integer representing the row or column to fetch
 *  csv a valid struct expr_csv_data * object
 *
 */
struct expr_csv_fetch_data_desc {
        int type_id;
        void *ext;

        int fetch_type;
        int selection;

        struct expr_csv_data *csv;
};


/*
 *  date_type contains a 'cell data type' identifier
 *  src the string of the data, not null term.
 *  src_len the length of the string
 */
struct expr_csv_data_cell {
        int data_type;
        const char *src;
        int src_len;
};


/* get cell data and count, cells or count maybe null
 *  returns EXPR_CSV_INVALID_PARAM if desc is null
 *  returns EXPR_CSV_INVALID_DESC if type id is not EXPR_CSV_STRUCT_FETCH
 *  returns EXPR_CSV_INVALID_DESC if csv is invalid 
 *  returns EXPR_CSV_INVALID_DESC if fetch_type is not a expr_csv_fetch_selector
 *  returns EXPR_CSV_INVALID_DESC if selection is less than 0
 *  returns EXPR_CSV_FAIL if csv was corrupt
 *  returns EXPR_CSV_OK on success.
 */
int
expr_csv_fetch_data(
        struct expr_csv_fetch_data_desc *desc,
        struct expr_csv_data_cell *cells,
        int *count);


/* inc guard */
#endif
