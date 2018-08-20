#include <transforms/transforms.h>
#include <expr/ast.h>
#include <expr/ast_node_csv.h>
#include <expr/lexer.h>
#include <stdlib.h>


/* -------------------------------------------------------------- Helpers  -- */


static int
parse_rows(
        struct expr_ast_node *n,
        struct expr_token *t,
        const char *src,
        const char *delim)
{
        struct expr_ast_node *cn = n;
        struct expr_ast_node *cr = 0;
        struct expr_token *ct = t;



        while(*ct->id != EX_TOKID_NULL) {
                /* add new row and cell */
                cn += 1;
                cn[0].id = EX_AST_CSV_ROW;
                cn[0].sub_id = 0;
                cn[0].parent = n;
                cn[0].l_param = 0;
                cn[0].r_param = 0;
                cn[0].next = 0;
                cn[0].src_offset = 0;
                cn[0].src_len = 0;
       
                if(cr) {
                        cr->next = cn;
                }

                cr = cn;
                cn += 1;

                while(*ct->sub_id != EX_TOKID_WS_NEWLINE) {
                          /* new cell */
                          

                          unsigned src_offset = cn[0].src_offset;
                          unsigned 

                          unsigned delim_len = strlen(delmim);

                          /* length of cell */
                          while(1) {
                                  /* is this a delim */

                                  /* add
                          } 
                }
        }

        /* per line */
        /* con-cat all toks until delim */
        /* check if con-cat fits a type */
        /* save cell and delim nodes */

        return 0;
}


/* ------------------------------------------------------------- Interface -- */


struct expr_ast_node*
expr_tokens_to_csv_ast(
        struct expr_ast_csv_create_desc *desc)
{
        struct expr_ast_node *root_node = malloc(sizeof(root_node[0]) * 1000);
        struct expr_ast_node *node = &root_node[0];
        
        root_node[0].id = EX_AST_CSV_DOC; 
        root_node[0].sub_id = 0;
        root_node[0].parent = 0;
        root_node[0].next = 0;
        root_node[0].l_param = &root_node[1];
        root_node[0].r_param = 0;
        root_node[0].src_offset = 0;
        root_node[0].src_len = sizeof(desc->src);

        parse_rows(root_node, desc->tokens, desc->src, desc->delim);

        return root_node;
}

