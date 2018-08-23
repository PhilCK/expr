#ifndef CSV_INCLUDED
#define CSV_INCLUDED


struct expr_csv * expr_csv_t;


/* -------------------------------------------------------------- Lifetime -- */


struct expr_csv_import_desc {
        int type_id;
        void *ext;

        int source;

        const char *data;
};


int
expr_csv_create(
        struct expr_csv_import_desc *desc,
        expr_csv_t *csv)


/* ------------------------------------------------------------------ Data -- */


struct expr_csv_fetch_data_desc {
        int type_id;
        void *ext;

        expr_csv_t csv;
};


int
expr_csv_fetch_data(
        struct expr_csv_fetch_data *desc)


/* inc guard */
#endif
