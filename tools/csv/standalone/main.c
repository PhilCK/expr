
#include <expr/csv.h>
#include <stdio.h>



static const char *file = 0;


/* ----------------------------------------------------------- Application -- */


int
main()
{
        const char *file = "C:/Users/SimStim/Developer/expr/tools/lexer/test_data/csv/no_header.csv";

        struct expr_csv_data *data = 0;

        int success = 0;

        /* get csv */
        struct expr_csv_import_desc import_desc = {0};
        import_desc.type_id = EXPR_CSV_STRUCT_IMPORT;
        import_desc.source = EXPR_CSV_SOURCE_FILE;
        import_desc.data = file;
        import_desc.delimiter = ",";
        
        success = expr_csv_create(&import_desc, &data);

        if(!success) {
                printf("Failed to import CSV\n");
                return 0;
        }

        /* check integrity */
        //struct expr_csv_check_desc integ_desc = {0};
        //integ_desc.type_id = EXPR_CSV_STRUCT_CHECK;
        //integ_desc.csv = data;

        //struct expr_csv_integrity integ = {0};

        //success = expr_csv_check(&integ_desc, &integ);

        //if(!success) {
        //        printf("CSV Failed to check integrity\n");
        //}

        //if(!(integ.integrity & EXPR_CSV_HAS_UNIFORM_COLUMN_COUNT)) {
        //        printf("CSV Columns aren't consistant\n");
        //}

        /* get data */
        struct expr_csv_fetch_data_desc data_desc = {0};
        data_desc.type_id = EXPR_CSV_STRUCT_FETCH;
        data_desc.fetch_type = EXPR_CSV_FETCH_ROW;
        data_desc.selection = 0;
        data_desc.csv = data;

        int count = 0;
        success = expr_csv_fetch_data(&data_desc, 0, 0, &count);

        int *types = malloc(sizeof(int) * count);
        expr_csv_fetch_data(&data_desc, 0, types, 0);

        return 0;
}
