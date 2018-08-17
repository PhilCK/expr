#include <transforms/transforms.h>
#include <expr/ast.h>
#include <expr/lexer.h>
#include <stdlib.h>


/* -------------------------------------------------------------- Helpers  -- */


int
parse_row(
        struct expr_ast_node *n,
        struct expr_ast_token *t,
        const char *delim)
{
        
        /* per line */
        /* con-cat all toks until delim */
        /* check if con-cat fits a type */
        /* save cell and delim nodes */
}


/* ------------------------------------------------------------- Interface -- */


struct expr_ast_node*
expr_tokens_to_csv_ast(
        struct expr_ast_csv_create_desc *desc)
{
        struct expr_ast_node *root_node = malloc(sizeof(root_node[0]) * 1000);
        struct expr_ast_node *node = &root_node[0];
        
         

        return 0;
}

