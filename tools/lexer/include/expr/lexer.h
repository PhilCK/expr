#ifndef LEXER_INCLUDED_
#define LEXER_INCLUDED_


enum expr_token_id {
        TOKID_NULL,
        TOKID_NUM_LIT,
        TOKID_HEX_LIT,
        TOKID_BIN_LIT,
        TOKID_FLT_LIT,
        TOKID_STR_LIT,
        TOKID_WHITESPACE,
        TOKID_IDENT,
        TOKID_PUNT,
};


struct expr_token {
        int id;
        int sub_id;

        int src_offset;
        int src_len;
};


struct expr_sub_puntuation {
        const char *pattern;
        int token_sub_id;
};


struct expr_token*
expr_lexer_create(
        const char *src,
        struct expr_sub_puntuation *puntuation,
        int puntuation_len);


void
expr_lexer_destroy(
        struct expr_token *destroy);


/* inc guard */
#endif

