#ifndef AST_INCLUDED_
#define AST_INCLUDED_


struct expr_ast_node {
        int id;
        int sub_id;

        struct expr_ast_node *parent;
        struct expr_ast_node *next;

        struct expr_ast_node *l_param;
        struct expr_ast_node *r_param;

        unsigned src_offset;
        unsigned src_len;
};


enum expr_ast_type_id {
        EX_AST_TYPEID_NULL,
        EX_AST_TYPEID_CREATE,
};


enum expr_ast_type {
        EX_AST_TYPE_NONE,
        EX_AST_TYPE_CSV,
};


/* -------------------------------------------------------------- Lifetime -- */


struct expr_ast_create_desc {
        int type_id;
        void *ext;

        int ast_type;

        struct expr_token **token_streams;
        int stream_count;
};


struct expr_ast_node*
expr_ast_create(
        struct expr_ast_create_desc *desc);


int
expr_ast_destroy(
        struct expr_ast_node *root_node);


/* ----------------------------------------------------------------- Print -- */


int
expr_ast_print(
        struct expr_ast_node *root,
        const char *src);


/* inc guard */
#endif
