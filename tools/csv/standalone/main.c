
#include <expr/csv.h>
#include <stdio.h>
#include <stdlib.h>


/* ------------------------------------------------------ Application Vars -- */


static const char *file = 0;


/* -------------------------------------------------------------- Cmd Args -- */


/* todo */


/* ----------------------------------------------------------- Application -- */


int
main()
{
        #ifdef _WIN32
        file = "C:/Users/SimStim/Developer/expr/tools/lexer/test_data/csv/no_header.csv";
        #else
        file = "/media/phil/Dev/dev-scratch/expr/tools/lexer/test_data/csv/no_header.csv";
        #endif

        struct expr_csv_data *data = 0;

        int err = 0;

        /* get csv */
        struct expr_csv_import_desc import_desc = {0};
        import_desc.type_id = EXPR_CSV_STRUCT_IMPORT;
        import_desc.source = EXPR_CSV_SOURCE_FILE;
        import_desc.data = file;
        import_desc.delimiter = ",";
        
        err = expr_csv_create(&import_desc, &data);

        if(err != EXPR_CSV_OK) {
                printf("Failed to import CSV\n");
                return 0;
        }
        else {
                printf("Loaded CSV\n");
        }

        /* check integrity */
        /*
        struct expr_csv_check_desc integ_desc = {0};
        integ_desc.type_id = EXPR_CSV_STRUCT_CHECK;
        integ_desc.csv = data;

        struct expr_csv_integrity integ = {0};

        success = expr_csv_check(&integ_desc, &integ);

        if(!success) {
                printf("CSV Failed to check integrity\n");
        }

        if(!(integ.integrity & EXPR_CSV_HAS_UNIFORM_COLUMN_COUNT)) {
                printf("CSV Columns aren't consistant\n");
        }*/

        /* get data */
        struct expr_csv_fetch_data_desc data_desc = {0};
        data_desc.type_id = EXPR_CSV_STRUCT_FETCH;
        data_desc.fetch_type = EXPR_CSV_FETCH_COLUMN;
        data_desc.selection = 2;
        data_desc.csv = data;

        int count = 0;

        err = expr_csv_fetch_data(&data_desc, 0, &count);

        if(err!= EXPR_CSV_OK) {
                printf("Failed to get cell count\n");
                return 0;
        } else {
                printf("%d cell count\n", count);
        }

        struct expr_csv_data_cell *cells = malloc(sizeof(cells[0]) * count);

        err = expr_csv_fetch_data(&data_desc, cells, 0);

        if(err != EXPR_CSV_OK) {
                printf("Failed to get cell data\n");
        }

        int i;
        for(i = 0; i < count; ++i) {
                printf("%d: %.*s\n", i + 1, cells[i].src_len, cells[i].src);
        }

        return 0;
}
