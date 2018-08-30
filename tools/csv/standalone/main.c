
#include <expr/csv.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>


/* ---------------------------------------------------------------- Config -- */


#define DEBUG_PRINT 0


/* ------------------------------------------------------ Application Vars -- */


const char *file = 0;
int fetch_type = -1;
int selector = -1;


/* -------------------------------------------------------------- Cmd Args -- */


void
print_help()
{
        printf("usage: ex-csv <csv_file> <options>\n\n");
        printf("options:\n");
        printf("-fetch <row|col> <selector>   returns the row or column\n");
        printf("-help                         this screen\n");
}


int
process_args(int argc, const char **argv)
{
        int i;

        for (i = 1; i < argc; ++i) {
                if(DEBUG_PRINT) {
                        printf("arg %s\n", argv[i]);
                }

                /* help */
                if (strcmp(argv[i], "-help") == 0) {
                        print_help();
                }
                /* fetch */
                else if (strcmp(argv[i], "-fetch") == 0) {
                        ++i;

                        if (i > argc) {
                                return 0;
                        }

                        if (strcmp(argv[i], "row") == 0) {
                                fetch_type = EXPR_CSV_FETCH_ROW;
                        }
                        else if (strcmp(argv[i], "col") == 0) {
                                fetch_type = EXPR_CSV_FETCH_COLUMN;
                        }
                        else {
                                return 0;
                        }

                        ++i;

                        if (i > argc) {
                                return 0;
                        }

                        selector = atoi(argv[i]);
                }
                /* input file */
                else {
                        file = argv[i];
                }
        }

        /* do we have everything we need */
        if((file == 0) || (fetch_type == -1) || (selector == -1)) {
                return 0;
        }

        return 1;
}


/* ----------------------------------------------------------- Application -- */


int
main(int argc, const char **argv)
{
        if (!process_args(argc, argv)) {
                printf("Unknown args\n");
                return 0;
        }

        struct expr_csv_data *data = 0;

        int err = 0;

        /* get csv */
        struct expr_csv_import_desc import_desc = {0};
        import_desc.type_id = EXPR_CSV_STRUCT_IMPORT;
        import_desc.source = EXPR_CSV_SOURCE_FILE;
        import_desc.data = file;
        import_desc.delimiter = ",";
        
        err = expr_csv_create(&import_desc, &data);

        struct expr_csv_check_desc integ_desc = {0};
        integ_desc.type_id = EXPR_CSV_STRUCT_CHECK;
        integ_desc.csv = data;

        struct expr_csv_integrity integ = {0};

        err = expr_csv_check(&integ_desc, &integ);

        if(err != EXPR_CSV_OK) {
                printf("CSV Failed to check integrity err: %d\n", err);
        }

        if(!integ.uniform_row_cell_count) {
                printf("CSV Columns aren't consistant\n");
        }

        /* get data */
        struct expr_csv_fetch_data_desc data_desc = {0};
        data_desc.type_id = EXPR_CSV_STRUCT_FETCH;
        data_desc.fetch_type = fetch_type;
        data_desc.selection = selector;
        data_desc.csv = data;

        int count = 0;

        err = expr_csv_fetch_data(&data_desc, 0, &count);

        if(err!= EXPR_CSV_OK) {
                printf("Failed to get cell count\n");
                return 0;
        } else if (DEBUG_PRINT) {
                printf("%d cell count\n", count);
        }

        struct expr_csv_data_cell *cells = malloc(sizeof(cells[0]) * count);

        err = expr_csv_fetch_data(&data_desc, cells, 0);

        if(err != EXPR_CSV_OK) {
                printf("Failed to get cell data\n");
        }

        int i;

        const char *type_names[] = {
                "STR", "INT", "INT2", "INT3", "INT4",
                "FLT", "FLT2", "FLT3", "FLT4", "BOOL",
                "BOOL2", "BOOL3", "BOOL4",
        };

        for(i = 0; i < count; ++i) {
                int index = cells[i].data_type - (int)EXPR_CSV_CELL_TYPE_STR;
                const char *type = type_names[index];

                printf(
                        "%d: %d %s, %.*s\n",
                        i + 1,
                        index,
                        type,
                        cells[i].src_len,
                        cells[i].src);
        }

        return 0;
}
