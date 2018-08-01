#ifndef LEXER_INCLUDED
#define LEXER_INCLUDED


enum expr_token_id {
        TOKID_NULL,
        TOKID_NUM_LIT,
        TOKID_HEX_LIT,
        TOKID_BIN_LIT,
        TOKID_FLT_LIT,
        TOKID_STR_LIT,
        TOKID_IDENT,
        TOKID_PUNT,
};


struct expr_token {
        int token_id;
        int token_sub_id;

        const char *src;
        int len;

        struct expr_token *next;
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

