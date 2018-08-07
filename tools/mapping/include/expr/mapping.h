#ifndef MAPPING_INCLUDED_
#define MAPPING_INCLUDED_




enum mapping_c_id {
        EX_TOKID_AND,
        EX_TOKID_BIT_AND_ASSIGN,
        EX_TOKID_BIT_AND,
        EX_TOKID_OR,
        EX_TOKID_BIT_OR_ASSIGN,
        EX_TOKID_BIT_OR,
        EX_TOKID_BIT_NOT_ASSIGN,
        EX_TOKID_BIT_NOT,
        EX_TOKID_ADD_ASSIGN,
        EX_TOKID_ADD,
        EX_TOKID_SUB_ASSIGN,
        EX_TOKID_SUB,
        EX_TOKID_DIV_ASSIGN,
        EX_TOKID_DIV,
        EX_TOKID_MUL_ASSIGN,
        EX_TOKID_MUL,
        EX_TOKID_NOT_ASSIGN,
        EX_TOKID_EQ,
        EX_TOKID_ASSIGN,
        EX_TOKID_GE,
        EX_TOKID_LE,
        EX_TOKID_DOT,
        EX_TOKID_COMMA,
        EX_TOKID_SEMICOLON,
        EX_TOKID_PAREN_OPEN,
        EX_TOKID_PAREN_CLOSE,
        EX_TOKID_SQ_PAREN_OPEN,
        EX_TOKID_SQ_PAREN_CLOSE,
        EX_TOKID_ANG_PAREN_OPEN,
        EX_TOKID_ANG_PAREN_CLOSE,
        EX_TOKID_CURLY_PAREN_OPEN,
        EX_TOKID_CURLY_PAREN_CLOSE,
        EX_TOKID_COMMENT,
        EX_TOKID_COMMENT_OPEN,
        EX_TOKID_COMMENT_CLOSE
};


enum ex_mapping_type {
        EX_MAPPING_C,
};


struct expr_sub_punctuation;

void
expr_mapping_get_patterns(
        int mapping_type,
        struct expr_sub_punctuation *map,
        int *map_count);


/* inc guard */
#endif

