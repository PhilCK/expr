#include <expr/ast.h>
#include <expr/lexer.h>
#include <transforms/transforms.h>
#include <assert.h>


/* -------------------------------------------------------------- Lifetime -- */


struct expr_ast_node*
expr_ast_create(
        struct expr_ast_create_desc *desc)
{
        return 0;
}


int
expr_ast_destroy(
        struct expr_ast_node *root_node)
{
        return 0;
}


/* ----------------------------------------------------------------- Print -- */


int
expr_ast_print(
        struct expr_ast_node *root,
        const char *src)
{
        return 0;
}
