#ifndef LEXER_INCLUDED_
#define LEXER_INCLUDED_


enum expr_token_id {
        EX_TOKID_NULL,
        EX_TOKID_UNKNOWN,
        EX_TOKID_LITERAL,
        EX_TOKID_WHITESPACE,
        EX_TOKID_IDENT,
        EX_TOKID_PUNCT,

        /* whitespace sub ids */
        EX_TOKID_WS_SPACE,
        EX_TOKID_WS_TAB,
        EX_TOKID_WS_NEWLINE,

        /* literal sub ids */
        EX_TOKID_LIT_NUM,
        EX_TOKID_LIT_HEX,
        EX_TOKID_LIT_BIN,
        EX_TOKID_LIT_FLT,
        EX_TOKID_LIT_STR,
};


enum expr_lex_type_id {
        EX_LEX_TYPEID_NULL,
        EX_LEX_TYPEID_CREATE,
        EX_LEX_TYPEID_SERIALIZE,
        EX_LEX_TYPEID_DESERIALIZE,
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


/* ------------------------------------------------------------- Lifetime --- */


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


int
expr_lexer_destroy(
        struct expr_token *destroy);


/* ----------------------------------------------------------------- Print -- */


void
expr_lexer_print(
        struct expr_token *toks,
        const char *src);


/* ------------------------------------------------------------- Serialize -- */


struct expr_lexer_serialize_desc {
        int type_id;
        void *ext;

        const char *serialize_filename;

        struct expr_token *tokens;
};


int
expr_lexer_serialize(
        struct expr_lexer_serialize_desc *desc);


struct expr_lexer_deserialize_desc {
        int type_id;
        void *ext;

        const char *serialized_filename;
};


struct expr_token *
expr_lexer_deserialize(
        struct expr_lexer_deserialize_desc *desc);


/* inc guard */
#endif

