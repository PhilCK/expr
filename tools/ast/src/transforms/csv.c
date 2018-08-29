#include <transforms/transforms.h>
#include <expr/ast.h>
#include <expr/file.h>
#include <expr/ast_node_csv.h>
#include <expr/fundamental.h>
#include <expr/v_array.h>
#include <expr/lexer.h>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>


/* --------------------------------------------------------- AST to String -- */


const char *expr_ast_node_csv_names[EX_AST_CSV_COUNT] = {
        "EX_AST_CSV_NULL"
          ,
        "EX_AST_CSV_DOC",

        "EX_AST_CSV_ROW",
        "EX_AST_CSV_ROW_HEADER",
        "EX_AST_CSV_ROW_CONTENT",

        "EX_AST_CSV_CELL",
        "EX_AST_CSV_CELL_STR",
        "EX_AST_CSV_CELL_INT",
        "EX_AST_CSV_CELL_INT2",
        "EX_AST_CSV_CELL_INT3",
        "EX_AST_CSV_CELL_INT4",
        "EX_AST_CSV_CELL_FLOAT",
        "EX_AST_CSV_CELL_FLOAT2",
        "EX_AST_CSV_CELL_FLOAT3",
        "EX_AST_CSV_CELL_FLOAT4",
        "EX_AST_CSV_CELL_BOOL",
        "EX_AST_CSV_CELL_BOOL2",
        "EX_AST_CSV_CELL_BOOL3",
        "EX_AST_CSV_CELL_BOOL4",
};


/* -------------------------------------------------------------- Helpers  -- */


static int
parse_bool(const char *src, int src_len) {
        const char *bools[] = {
                "true", "false",
                "TRUE", "FALSE",
                "True", "False",

                "yes", "no",
                "YES", "NO",
                "Yes", "No",
        };

        unsigned i;

        for(i = 0; i < EX_ARR_COUNT(bools); ++i) {
                if(strncmp(bools[i], src, src_len) == 0) {
                        return 1;
                }
        }

        return 0;
}


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

        struct type_info {
                int strs;
                int ints;
                int floats;
                int bools;
        } type_info = {0};
        

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

                /* check type info */
                if(ct->sub_id == EX_TOKID_LIT_NUM) {
                        type_info.ints += 1;
                } else if(ct->sub_id == EX_TOKID_LIT_FLT) {
                        type_info.floats += 1;
                } else if(ct->id != EX_TOKID_WHITESPACE) {
                        type_info.strs += 1;

                        if(parse_bool(&src[ct->src_offset], ct->src_len)) {
                                type_info.strs -= 1;
                                type_info.bools += 1;
                        }
                }

                len += ct->src_len;
                ct += 1;

        }

        /* this is an error */
        if(len == 0) {
                return ct - *t;
        }

        /*
        printf(
                "Cell:%d %d %.*s\n",
                (int)ct->src_offset,
                len,
                len,
                &src[(*t)->src_offset]);
        */

        if(last_cell && *last_cell) {
                (*last_cell)->next = curr_cell;
        }
        else {
                parent->l_param = curr_cell;
        }

        /* cell type */
        int cell_type = EX_AST_CSV_CELL_STR;

        if(type_info.strs == 0) {
                int has_bools = type_info.bools > 0;
                int has_ints = type_info.ints > 0;
                int has_floats = type_info.floats > 0;

                if(has_bools && (!has_ints || !has_floats)) {
                        int count = type_info.bools;

                        if(count == 1) {
                                cell_type = EX_AST_CSV_CELL_BOOL;
                        } else if (count == 2) {
                                cell_type = EX_AST_CSV_CELL_BOOL2;
                        } else if (count == 3) {
                                cell_type = EX_AST_CSV_CELL_BOOL3;
                        } else if (count == 4) {
                                cell_type = EX_AST_CSV_CELL_BOOL4;
                        }
                }
                else if(has_ints && (!has_floats || !has_bools)) {
                        int count = type_info.ints;

                        if(count == 1) {
                                cell_type = EX_AST_CSV_CELL_INT;
                        } else if (count == 2) {
                                cell_type = EX_AST_CSV_CELL_INT2;
                        } else if (count == 3) {
                                cell_type = EX_AST_CSV_CELL_INT3;
                        } else if (count == 4) {
                                cell_type = EX_AST_CSV_CELL_INT4;
                        }
                }
                else if(has_floats && (!has_bools)) {
                        int count = type_info.ints + type_info.floats;

                        if(count == 1) {
                                cell_type = EX_AST_CSV_CELL_FLOAT;
                        } else if (count == 2) {
                                cell_type = EX_AST_CSV_CELL_FLOAT2;
                        } else if (count == 3) {
                                cell_type = EX_AST_CSV_CELL_FLOAT3;
                        } else if (count == 4) {
                                cell_type = EX_AST_CSV_CELL_FLOAT4;
                        }
                }
        }

        *last_cell = curr_cell;

        curr_cell->id         = EX_AST_CSV_CELL;
        curr_cell->sub_id     = cell_type;
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

                /*
                printf("parse row %d %d %d - %.*s\n", ct->id, ct->src_len, ct->src_offset, ct->src_len, &src[ct->src_offset]);
                */

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

        return root_node;
}

