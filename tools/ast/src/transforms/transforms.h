#ifndef TRANSFORMS_INCLUDED_
#define TRANSFORMS_INCLUDED_


enum expr_ast_desc_id {
        EX_AST_TYPEID_CSV_CREATE_DESC,
};


/* ------------------------------------------------------------------- CSV -- */


struct expr_ast_csv_create_desc {
        int type_id;
        void *ext;

        const char *src;
        struct expr_token *tokens;
        const char *delim;
};


struct expr_ast_node*
expr_tokens_to_csv_ast(
        struct expr_ast_csv_create_desc *desc);


/* inc guard */
#endif

