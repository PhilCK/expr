#ifndef AST_INCLUDED_
#define AST_INCLUDED_


struct ast_node {
        struct id;
        struct sub_id;

        struct ast_node *parent;
        struct ast_node *next;

        struct ast_node *l_param;
        struct ast_node *r_param;
};


/* inc guard */
#endif
