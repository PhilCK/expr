#include <transforms/transforms.h>
#include <expr/ast.h>
#include <expr/file.h>
#include <expr/ast_node_csv.h>
#include <expr/lexer.h>
#include <stdlib.h>
#include <string.h>


/* -------------------------------------------------------------- Helpers  -- */


static int
parse_cell(
        struct expr_ast_node *n,
        struct expr_token *t,
        const char *src,
        const char *delim,
        int delim_len)
{
        struct expr_token *ct = t;
        int len = 0;
        
        while(ct->id != EX_TOKID_NULL) {
                
                /* check if its the delim */
                int is_len = (delim_len == ct->src_len) ? 1 : 0;
                int is_str = strncmp(&src[ct->src_offset], delim, delim_len);
                
                if(is_len == 1 && is_str == 0) {
                        ct += 1;
                        break;
                }

                /* check if its a new line */
                if(ct->sub_id == EX_TOKID_WS_NEWLINE) {
                        break;
                }

                len += ct->src_len;
                ct += 1;

        }

        printf(
                "Cell:%d %d %.*s\n",
                (int)t->src_offset,
                len,
                len,
                &src[t->src_offset]);

        return ct - t;
}


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

        int delim_len = strlen(delim);

        cn += 1;

        /* each row */
        while(ct->id != EX_TOKID_NULL) {
                printf("parse row %d %d %d - %.*s\n", ct->id, ct->src_len, ct->src_offset, ct->src_len, &src[ct->src_offset]);

                /* add new row and cell */
                cn[0].id          = EX_AST_CSV_ROW;
                cn[0].sub_id      = 0;
                cn[0].parent      = n;
                cn[0].l_param     = 0;
                cn[0].r_param     = 0;
                cn[0].next        = 0;
                cn[0].src_offset  = 0;
                cn[0].src_len     = 0;

                if(cr) {
                        cr->next = cn;
                }

                cr = cn;
                cn += 1;

                /* cells */
                while(ct->sub_id != EX_TOKID_WS_NEWLINE && ct->id != EX_TOKID_NULL) {
                        int consume = parse_cell(cn, ct, src, delim, delim_len);

                        if(!consume) {
                                break;
                        }

                        ct += consume;
                        cn += 1;
                }

                ct += 1;
        }

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
        root_node[0].src_len = strlen(desc->src);

        parse_rows(root_node, desc->tokens, desc->src, desc->delim);

        return root_node;
}

