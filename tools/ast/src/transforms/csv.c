#include <transforms/transforms.h>
#include <expr/ast.h>
#include <expr/file.h>
#include <expr/ast_node_csv.h>
#include <expr/v_array.h>
#include <expr/lexer.h>
#include <stdlib.h>
#include <string.h>


/* -------------------------------------------------------------- Helpers  -- */


static int
parse_cell(
        struct expr_ast_node *root,
        struct expr_ast_node *parent,
        struct expr_ast_node **last_cell,
        struct expr_token **t,
        const char *src,
        const char *delim,
        int delim_len)
{
        struct expr_ast_node *curr_cell = 0;
        ex_varr_push(root, curr_cell);

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

        if(last_cell && *last_cell) {
                (*last_cell)->next = curr_cell;
        }
        else {
                parent->l_param = curr_cell;
        }

        *last_cell = curr_cell;

        curr_cell->id         = EX_AST_CSV_CELL;
        curr_cell->sub_id     = EX_AST_CSV_CELL_STR;
        curr_cell->parent     = parent;
        curr_cell->l_param    = 0;
        curr_cell->r_param    = 0;
        curr_cell->src_offset = (*t)->src_offset;
        curr_cell->src_len    = len;
        curr_cell->next       = 0;

        int consumed = (ct - *t);
        *t += (ct - *t);

        return consumed;
}


static int
parse_rows(
        struct expr_ast_node *root_node,
        struct expr_token *t,
        const char *src,
        const char *delim)
{
        struct expr_ast_node *last_row = 0;

        struct expr_token *ct = t;

        int delim_len = strlen(delim);

        struct expr_ast_node *curr_row = 0;
        

        /* each row */
        while(ct->id != EX_TOKID_NULL) {
                struct expr_ast_node *last_cell = 0;

                ex_varr_push(root_node, curr_row);

                printf("parse row %d %d %d - %.*s\n", ct->id, ct->src_len, ct->src_offset, ct->src_len, &src[ct->src_offset]);

                /* add new row and cell */
                curr_row->id          = EX_AST_CSV_ROW;
                curr_row->sub_id      = EX_AST_CSV_ROW_CONTENT;
                curr_row->parent      = root_node;
                curr_row->l_param     = 0;
                curr_row->r_param     = 0;
                curr_row->next        = 0;
                curr_row->src_offset  = 0;
                curr_row->src_len     = 0;

                if(last_row) {
                        last_row->next = curr_row;
                }                

                last_row = curr_row;

                struct expr_ast_node *lc = 0;

                /* cells */
                while(ct->sub_id != EX_TOKID_WS_NEWLINE && ct->id != EX_TOKID_NULL) {
                        
                        int consume = parse_cell(root_node, last_row, &last_cell, &ct, src, delim, delim_len);

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
        struct expr_ast_node *root_node = 0;
        ex_varr_create(root_node, 1 << 24);

        struct expr_ast_node *node = root_node;
        ex_varr_push(root_node, node);
        
        node->id = EX_AST_CSV_DOC;
        node->sub_id = 0;
        node->parent = 0;
        node->next = 0;
        node->l_param = &root_node[1];
        node->r_param = 0;
        node->src_offset = 0;
        node->src_len = strlen(desc->src);

        parse_rows(root_node, desc->tokens, desc->src, desc->delim);

        expr_ast_csv_print(root_node, desc->src, 0);

        return root_node;
}

