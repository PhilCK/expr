#ifndef LEXER_INCLUDED_
#define LEXER_INCLUDED_


enum expr_token_id {
        TOKID_NULL,
        TOKID_UNKNOWN,
        TOKID_LITERAL,
        TOKID_WHITESPACE,
        TOKID_IDENT,
        TOKID_PUNCT,

        /* whitespace sub ids */
        TOKID_WS_SPACE,
        TOKID_WS_TAB,
        TOKID_IS_NEWLINE,

        /* literal sub ids */
        TOKID_LIT_NUM,
        TOKID_LIT_HEX,
        TOKID_LIT_BIN,
        TOKID_LIT_FLT,
        TOKID_LIT_STR,
};


enum expr_type_id {
        EX_LEX_TYPEID_NULL,
        EX_LEX_TYPEID_CREATE,
};


struct expr_token {
        int id;
        int sub_id;

        int src_offset;
        int src_len;
};


struct expr_sub_punctuation {
        const char *pattern;
        int token_sub_id;
};


struct expr_lexer_create_desc {
        int type_id;
        void *ext;

        const char *src;
        struct expr_sub_punctuation *punctuation;
        int punctuation_count;

        int skip_whitespace;
};


struct expr_token*
expr_lexer_create(
        struct expr_lexer_create_desc *desc);


void
expr_lexer_destroy(
        struct expr_token *destroy);


/* inc guard */
#endif

