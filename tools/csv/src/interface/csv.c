#include <expr/csv.h>
#include <expr/ast.h>
#include <expr/ast_node_csv.h>
#include <expr/lexer.h>
#include <expr/fundamental.h>
#include <assert.h>
#include <stdlib.h>


struct expr_csv_data {
        const char *src;
        struct expr_ast_node *ast;
};


/* -------------------------------------------------------------- Lifetime -- */


int
expr_csv_create(
        struct expr_csv_import_desc *desc,
        struct expr_csv_data **csv)
{
        assert(desc);
        assert(desc->type_id == EXPR_CSV_STRUCT_IMPORT);
        assert(desc->data);

        if (!desc || desc->type_id != EXPR_CSV_STRUCT_IMPORT || !desc->data) {
                return 0;
        }

        const char *src = 0;

        if(desc->source == EXPR_CSV_SOURCE_FILE) {
                int bytes = 0;
                int load = 0;
                load = expr_file_load(desc->data, 0, &bytes);

                if(!load) {
                        return 0;
                }
                
                src = calloc(bytes, 1);
                load = expr_file_load(desc->data, &src, 0);

                if(!load) {
                        free((void*)src);
                        src = 0;
                        return 0;
                }

        } else if(desc->source == EXPR_CSV_SOURCE_STRING) {
                src = desc->data;
        }
        else {
                return 0;
        }

        /* load tokens */
        struct expr_lexer_create_desc lex_desc = {0};
        lex_desc.type_id = EX_LEX_TYPEID_CREATE;
        lex_desc.src = src;
        lex_desc.skip_whitespace = 0;

        struct expr_token *toks = expr_lexer_create(&lex_desc);

        if(!toks) {
                return 0;
        }

        /* load csv ast */
        struct expr_ast_create_desc ast_desc = {0};
        ast_desc.type_id = EX_AST_TYPEID_CREATE;
        ast_desc.token_streams = &toks;
        ast_desc.src_streams = &src;
        ast_desc.stream_count = 1;

        struct expr_ast_node *ast = expr_ast_create(&ast_desc);

        expr_ast_print(ast, src);

        if(!ast) {
                return 0;
        }

        /* create data ctx */


        return 1;
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
