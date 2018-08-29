#include <expr/csv.h>
#include <expr/ast.h>
#include <expr/ast_node_csv.h>
#include <expr/lexer.h>
#include <expr/fundamental.h>
#include <expr/file.h>
#include <assert.h>
#include <stdlib.h>


#define DEBUG_PRINT_TOKENS 1



/* -------------------------------------------------------------- Internal -- */


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
        int is_src_file = -1;
        int is_src_str = -1;
        
        assert(desc);
        assert(desc->type_id == EXPR_CSV_STRUCT_IMPORT);
        assert(desc->data);

        if(!desc || !csv) {
                return EXPR_CSV_INVALID_PARAM;
        }

        if(desc->type_id != EXPR_CSV_STRUCT_IMPORT || !desc->data) {
                return EXPR_CSV_INVALID_DESC;
        }

        is_src_file = desc->source == EXPR_CSV_SOURCE_FILE;
        is_src_str = desc->source == EXPR_CSV_SOURCE_STRING;
        
        if((is_src_file + is_src_str) <= 0) {
               return EXPR_CSV_INVALID_DESC;
        }

        const char *src = 0;

        if(is_src_file) {
                int bytes = 0;
                int load = 0;
                load = expr_file_load(desc->data, 0, &bytes);

                if(!load) {
                        return EXPR_CSV_FAIL;
                }
                
                src = calloc(bytes, 1);
                load = expr_file_load(desc->data, &src, 0);

                if(!load) {
                        free((void*)src);
                        src = 0;
                        return EXPR_CSV_FAIL;
                }

        } else if(is_src_str) {
                src = desc->data;
        }

        /* load tokens */
        struct expr_lexer_create_desc lex_desc = {0};
        lex_desc.type_id = EX_LEX_TYPEID_CREATE;
        lex_desc.src = src;
        lex_desc.skip_whitespace = 0;

        struct expr_token *toks = expr_lexer_create(&lex_desc);

        if(!toks) {
                expr_lexer_destroy(toks);
                free((void*)src);
                return EXPR_CSV_FAIL;
        }

        if(DEBUG_PRINT_TOKENS) {
                expr_lexer_print(toks, src);
        }

        /* load csv ast */
        struct expr_ast_create_desc ast_desc = {0};
        ast_desc.type_id = EX_AST_TYPEID_CREATE;
        ast_desc.token_streams = &toks;
        ast_desc.src_streams = &src;
        ast_desc.stream_count = 1;

        struct expr_ast_node *ast = expr_ast_create(&ast_desc);

        if(!ast || ast->id != EX_AST_CSV_DOC) {
                expr_ast_destroy(ast);
                expr_lexer_destroy(toks);
                free((void*)src);
                return EXPR_CSV_FAIL;
        }

        /* create data ctx */
        struct expr_csv_data *data = malloc(sizeof(*data));

        if(!data) {
                expr_ast_destroy(ast);
                expr_lexer_destroy(toks);
                free((void*)src);

                return EXPR_CSV_FAIL;
        }

        data->src = src;
        data->ast = ast;

        *csv = data;
        expr_lexer_destroy(toks);

        return EXPR_CSV_OK;
}

int
expr_csv_destroy(
        struct expr_csv_data **csv)
{
        assert(csv);
        assert(*csv);

        if (!csv || !*csv) {
                return EXPR_CSV_INVALID_PARAM;
        }

        return EXPR_CSV_FAIL;
}


/* ---------------------------------------------------------- Data Helpers -- */


int
type_from_ast(struct expr_ast_node *node) {
        switch(node->sub_id) {
        case(EX_AST_CSV_CELL_STR): return EXPR_CSV_CELL_TYPE_STR;
        case(EX_AST_CSV_CELL_INT): return EXPR_CSV_CELL_TYPE_INT;
        case(EX_AST_CSV_CELL_INT2): return EXPR_CSV_CELL_TYPE_INT2;
        case(EX_AST_CSV_CELL_INT3): return EXPR_CSV_CELL_TYPE_INT3;
        case(EX_AST_CSV_CELL_INT4): return EXPR_CSV_CELL_TYPE_INT3;
        case(EX_AST_CSV_CELL_FLOAT): return EXPR_CSV_CELL_TYPE_FLOAT;
        case(EX_AST_CSV_CELL_FLOAT2): return EXPR_CSV_CELL_TYPE_FLOAT2;
        case(EX_AST_CSV_CELL_FLOAT3): return EXPR_CSV_CELL_TYPE_FLOAT3;
        case(EX_AST_CSV_CELL_FLOAT4): return EXPR_CSV_CELL_TYPE_FLOAT4;
        case(EX_AST_CSV_CELL_BOOL): return EXPR_CSV_CELL_TYPE_BOOL;
        case(EX_AST_CSV_CELL_BOOL2): return EXPR_CSV_CELL_TYPE_BOOL2;
        case(EX_AST_CSV_CELL_BOOL3): return EXPR_CSV_CELL_TYPE_BOOL3;
        case(EX_AST_CSV_CELL_BOOL4): return EXPR_CSV_CELL_TYPE_BOOL4;
        }

        return EXPR_CSV_CELL_TYPE_STR;
}


void
cell_from_ast(
        struct expr_ast_node *node,
        struct expr_csv_data_cell *cell,
        struct expr_csv_data *csv)
{
        cell->data_type = type_from_ast(node);
        cell->src = &csv->src[node->src_offset];
        cell->src_len = node->src_len;
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
        assert(out_desc);

        if (!desc || desc->type_id != EXPR_CSV_STRUCT_CHECK || !desc->csv) {
                return 0;
        }

        return 0;
}


int
expr_csv_fetch_data(
        struct expr_csv_fetch_data_desc *desc,
        struct expr_csv_data_cell *cells,
        int *count)
{
        int is_row_search = -1;
        int is_col_search = -1;

        struct expr_csv_data *csv = 0;
        struct expr_ast_node *ast = 0;

        assert(desc);
        assert(desc->type_id == EXPR_CSV_STRUCT_FETCH);
        assert(desc->csv);

        if(!desc) {
                return EXPR_CSV_INVALID_PARAM;
        }
        
        if (desc->type_id != EXPR_CSV_STRUCT_FETCH || !desc->csv) {
                return EXPR_CSV_INVALID_DESC;
        }

        is_row_search = desc->fetch_type == EXPR_CSV_FETCH_ROW;
        is_col_search = desc->fetch_type == EXPR_CSV_FETCH_COLUMN;

        if((is_row_search + is_col_search) <= 0) {
                return EXPR_CSV_INVALID_DESC;
        }

        csv = desc->csv;
        ast = csv->ast;

        if(ast->id != EX_AST_CSV_DOC) {
                assert(0);
                return EXPR_CSV_FAIL;
        }

        if(is_row_search) { 
                int i;
                struct expr_ast_node *row = ast->l_param;
                struct expr_ast_node *cell = 0;

                i = 0;

                while(row && i != desc->selection) {
                        i += 1;
                        row = row->next;
                }

                if(count) {
                        i = 0;
                        cell = row->l_param;

                        while(cell) {
                                cell = cell->next;
                                i += 1;
                        }

                        *count = i;
                }

                if(cells) {
                        i = 0;
                        cell = row->l_param;

                        while(cell) {
                                cell_from_ast(cell, &cells[i], csv);

                                cell = cell->next;
                                i += 1;
                        }
                }

        }
        else if(is_col_search) {
                int i, j;
                struct expr_ast_node *row = ast->l_param;
                struct expr_ast_node *cell = 0;

                j = 0;

                while(row) {
                        i = 0;
                        cell = row->l_param;

                        while(cell) {
                                if(i < desc->selection) {
                                        i += 1;
                                        cell = cell->next;
                                        continue;
                                }

                                if(cells) {
                                        cell_from_ast(cell, &cells[j], csv);
                                }
                                
                                j += 1;
                                cell = 0;
                        };

                        row = row->next;
                }

                if(count) {
                        *count = j;
                }
        }

        return EXPR_CSV_OK;
}
