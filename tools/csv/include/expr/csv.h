#ifndef CSV_INCLUDED
#define CSV_INCLUDED


struct expr_csv_data;


enum expr_csv_identifiers {
        EXPR_CSV_STRUCT_IMPORT,
        EXPR_CSV_STRUCT_CHECK,
        EXPR_CSV_STRUCT_FETCH,
};


/* -------------------------------------------------------------- Lifetime -- */


enum expr_csv_source {
        EXPR_CSV_SOURCE_FILE,
        EXPR_CSV_SOURCE_STRING,
};


struct expr_csv_import_desc {
        int type_id;
        void *ext;

        int source;
        const char *data;
        const char *delimiter;
};


int
expr_csv_create(
        struct expr_csv_import_desc *desc,
        struct expr_csv_data **csv);

int
expr_csv_destroy(
        struct expr_csv_data **csv);


/* ------------------------------------------------------------- Integrity -- */


struct expr_csv_check_desc {
        int type_id;
        void *ext;

        struct expr_csv_data *csv;
};


enum expr_csv_integrity_flags {
        EXPR_CSV_HAS_UNIFORM_COLUMN_TYPES = 1 << 0,
        EXPR_CSV_HAS_UNIFORM_COLUMN_COUNT = 1 << 1,
};


struct expr_csv_integrity {
        int has_header_row;
        int integrity;

        int row_count;
        int col_count;
};


int
expr_csv_check(
        struct expr_csv_check_desc *desc,
        struct expr_csv_integrity *out_desc);


/* ------------------------------------------------------------------ Data -- */


enum expr_csv_fetch_selectors {
        EXPR_CSV_FETCH_ROW,
        EXPR_CSV_FETCH_COLUMN,
};


struct expr_csv_fetch_data_desc {
        int type_id;
        void *ext;

        int fetch_type;
        int selection;

        struct expr_csv_data *csv;
};


struct expr_csv_data_cell {
        int data_type;
        const char *src;
        int src_len;
};


int
expr_csv_fetch_data(
        struct expr_csv_fetch_data_desc *desc,
        struct expr_csv_data_cell *cells,
        int *count);


/* inc guard */
#endif
