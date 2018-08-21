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
        struct expr_ast_node *parent,
        struct expr_ast_node *last_cell,
        struct expr_ast_node **n,
        struct expr_token **t,
        const char *src,
        const char *delim,
        int delim_len)
{
        struct expr_ast_node *cn = *n;
        struct expr_token *ct = *t;
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

        /* this is an error */
        if(len == 0) {
                return ct - *t;
        }

        printf(
                "Cell:%d %d %.*s\n",
                (int)ct->src_offset,
                len,
                len,
                &src[(*t)->src_offset]);

        if(last_cell) {
                last_cell->next = &cn[0];
        }
        else {
                parent->l_param = &cn[0];
        }

        cn[0].id = EX_AST_CSV_CELL;
        cn[0].sub_id = EX_AST_CSV_CELL_STR; 
        cn[0].parent = parent;
        cn[0].l_param = 0;
        cn[0].r_param = 0;
        cn[0].src_offset = (*t)->src_offset;
        cn[0].src_len = len;
        cn[0].next = 0;

        int consumed = (ct - *t);
        *t += (ct - *t);

        return consumed;
}


static int
parse_rows(
        struct expr_ast_node *n,
        struct expr_token *t,
        const char *src,
        const char *delim)
{
        struct expr_ast_node *cn = n;
        struct expr_ast_node *lr = 0;
        struct expr_token *ct = t;

        int delim_len = strlen(delim);

        cn += 1;

        /* each row */
        while(ct->id != EX_TOKID_NULL) {
                printf("parse row %d %d %d - %.*s\n", ct->id, ct->src_len, ct->src_offset, ct->src_len, &src[ct->src_offset]);

                /* add new row and cell */
                cn[0].id          = EX_AST_CSV_ROW;
                cn[0].sub_id      = EX_AST_CSV_ROW_CONTENT;
                cn[0].parent      = n;
                cn[0].l_param     = 0;
                cn[0].r_param     = 0;
                cn[0].next        = 0;
                cn[0].src_offset  = 0;
                cn[0].src_len     = 0;

                if(lr) {
                        lr->next = &cn[0];
                }                

                lr = cn;

                cn += 1;


                struct expr_ast_node *lc = 0;

                /* cells */
                while(ct->sub_id != EX_TOKID_WS_NEWLINE && ct->id != EX_TOKID_NULL) {
                        int consume = parse_cell(lr, lc, &cn, &ct, src, delim, delim_len);
                        lc = cn;
                        cn += 1;

                        if(!consume) {
                                break;
                        }
                }

                ct += 1;
        }

        return 0;
}


/* ------------------------------------------------------------- Interface -- */


void
expr_ast_csv_print(struct expr_ast_node *n, const char *src, int tab_offset)
{
        struct expr_ast_node *this_n = n;

        while(this_n) {
                int tab_spacing = tab_offset * 2;

                while(tab_spacing) {
                        printf(" ");
                        tab_spacing -= 1;
                }

                const char *id_str = expr_ast_node_csv_names[this_n->id];
                const char *sid_str = expr_ast_node_csv_names[this_n->sub_id];

                if(this_n->sub_id > 0) {
                        printf("%s|%s\n", id_str, sid_str);
                }
                else {
                        printf("%s\n", id_str);
                }

                if(this_n->id == EX_AST_CSV_CELL) {
                        int pretty_tabs = tab_offset * 2;

                        while(pretty_tabs) {
                                printf(" ");
                                pretty_tabs -= 1;
                        }

                        const char *content = &src[this_n->src_offset];
                        int len = this_n->src_len;
                        printf("content(%.*s) ", len, content);

                        printf("\n");
                }

                if(n->l_param) {
                        expr_ast_csv_print(this_n->l_param, src, tab_offset + 1);
                }

                this_n = this_n->next;

        }
}


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

        expr_ast_csv_print(root_node, desc->src, 0);

        return root_node;
}

