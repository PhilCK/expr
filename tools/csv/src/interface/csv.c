#include <expr/csv.h>
#include <expr/ast.h>
#include <expr/ast_node_csv.h>
#include <expr/lexer.h>
#include <assert.h>


struct expr_csv_data {
        const char *src;
        struct expr_ast *ast;
};


/* -------------------------------------------------------------- Lifetime -- */


int
expr_csv_create(
        struct expr_csv_import_desc *desc,
        struct expr_csv_data **csv)
{
        assert(desc);
        assert(desc->type_id == EXPR_CSV_STRUCT_IMPORT);
        assert(desc->source);

        if (!desc || desc->type_id != EXPR_CSV_STRUCT_IMPORT || !desc->source) {
                return 0;
        }

        /* load tokens */
        struct expr_token *toks = 0;

        /* load csv ast */
        struct expr_ast *ast = 0;

        return 0;
}

int
expr_csv_destroy(
        struct expr_csv_data **csv)
{
        assert(csv);
        assert(*csv);

        if (!csv || !*csv) {
                return 0;
        }

        return 0;
}


/* ------------------------------------------------------------------ Data -- */


int
expr_csv_check(
        struct expr_csv_check_desc *desc,
        struct expr_csv_integrity *out_desc)
{
        assert(desc);
        assert(desc->type_id == EXPR_CSV_STRUCT_CHECK);
        assert(desc->csv);

        if (!desc || desc->type_id != EXPR_CSV_STRUCT_CHECK || !desc->csv) {
                return 0;
        }

        return 0;
}

int
expr_csv_fetch_data(
        struct expr_csv_fetch_data_desc *desc,
        void **data,
        int *types,
        int *count)
{
        assert(desc);
        assert(desc->type_id == EXPR_CSV_STRUCT_FETCH);
        assert(desc->csv);
        
        if (!desc || desc->type_id != EXPR_CSV_STRUCT_FETCH || !desc->csv) {
                return 0;
        }

        return 0;
}