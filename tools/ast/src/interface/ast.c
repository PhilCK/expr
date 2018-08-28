#include <expr/ast.h>
#include <expr/lexer.h>
#include <expr/v_array.h>
#include <transforms/transforms.h>
#include <assert.h>


/* -------------------------------------------------------------- Lifetime -- */


struct expr_ast_node*
expr_ast_create(
        struct expr_ast_create_desc *desc)
{
        assert(desc);

        struct expr_ast_csv_create_desc csv_desc = {0};
        csv_desc.type_id = EX_AST_TYPEID_CSV_CREATE_DESC;
        csv_desc.tokens  = desc->token_streams[0];
        csv_desc.src     = desc->src_streams[0];
        csv_desc.delim   = ",";
        
        struct expr_ast_node *ast = expr_tokens_to_csv_ast(&csv_desc);

        return  ast;
}


int
expr_ast_destroy(
        struct expr_ast_node *root_node)
{
        assert(root_node);

        if (!root_node) {
                return 0;
        }

        return expr_varray_destroy((void*)root_node);
}


/* ----------------------------------------------------------------- Print -- */


int
expr_ast_print(
        struct expr_ast_node *root,
        const char *src)
{
        (void)root;
        (void)src;

        return 0;
}
