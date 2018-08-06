#include <expr/lexer.h>
#include <expr/fundamental.h>
#include <expr/v_array.h>
#include <string.h>
#include <assert.h>
#include <stdlib.h>
#include <stdio.h>


/* ------------------------------------------------------------ [ Tables ] -- */


static const char  whitespace[] = {
        ' ', '\t', '\n', '\r',
        '\0'
};


static const char numeric[] = {
        '0', '1', '2', '3', '4', '5', '6', '7', '8', '9',
        '\0'
};


static const char hex[] = {
        '0', '1', '2', '3', '4', '5', '6', '7', '8', '9',
        'a', 'b', 'c', 'd', 'e', 'f',
        'A', 'B', 'C', 'D', 'E', 'F',
        '\0'
};


static const char flt[] = {
        '.',
        '\0'
};


static const char alpha[] = {
        'a', 'A', 'b', 'B', 'c', 'C', 'd', 'D', 'e', 'E', 'f', 'F', 'g', 'G',
        'h', 'H', 'i', 'I', 'j', 'J', 'k', 'K', 'l', 'L', 'm', 'M', 'n', 'N',
        'o', 'O', 'p', 'P', 'q', 'Q', 'r', 'R', 's', 'S', 't', 'T', 'u', 'U',
        'v', 'V', 'w', 'W', 'x', 'X', 'y', 'Y', 'z', 'Z',
        '\0'
};


static const char str_literal[] = {
        '\'', '\"',
        '\0'
};


/* ----------------------------------------------------------- [ Helpers ] -- */


static int
contains(
        char needle,
        const char *heystack)
{
        while (*heystack) {
                if (needle == *heystack) {
                        return 1;
                }

                heystack += 1;
        }

        return 0;
}


static int is_whitespace(char c)    { return contains(c, whitespace);       }
static int is_alpha(char c)         { return contains(c, alpha);            }
static int is_numeric(char c)       { return contains(c, numeric);          }
static int is_alpha_numeric(char c) { return is_alpha(c) || is_numeric(c);  }
static int is_str_literal(char c)   { return contains(c, str_literal);      }
static int is_hex(char c)           { return contains(c, hex);              }


void
set_token(struct expr_token *tok, int id, int sub_id, int offset, int len) {
        tok->id = id;
        tok->sub_id = sub_id;
        tok->src_offset = offset;
        tok->src_len = len;
}


/* ----------------------------------------------------------- [ Parsers ] -- */


static int
parse_num_literal(
        struct expr_token *next,
        const char *src)
{
        const char *end = src;
        int len;

        assert(next);
        assert(src);

        if (!is_numeric(*src)) {
                return 0;
        }

        end += 1;
        
        while(is_numeric(*end)) {
                end += 1;
        }
        
        len = end - src;

        if(len) {
                set_token(next, TOKID_NUM_LIT, 0, 0, len);
        }

        return len;
}


static int
parse_hex_literal(
        struct expr_token *next,
        const char *src)
{
        const char *end = src;
        int len;

        assert(next);
        assert(src);

        /* check starts with 0x or 0X */
        if (*end != '0') {
                return 0;
        }

        end += 1;
        
        if(!(*end == 'x' || *end == 'X')) {
                return 0;
        }

        end += 1;

        /* find all hex values */
        while(is_hex(*end)) {
                end += 1;
        }

        len = end - src;

        if(len) {
                set_token(next, TOKID_HEX_LIT, 0, 0, len);
        }

        return len;
}


static int
parse_flt_literal(
        struct expr_token *next,
        const char *src)
{
        const char *end = src;
        int len;
        int decimal = 0;        

        assert(next);
        assert(src);

        if (!is_numeric(*src)) {
                return 0;
        }

        end += 1;
        
        while(is_numeric(*end) || contains(*end, flt)) {
                if(contains(*end, flt)) {
                        decimal += 1;

                        if(decimal > 1) {
                                return 0;
                        }
                }

                end += 1;
        }
        
        len = end - src;

        if(len) {
                set_token(next, TOKID_NUM_LIT, 0, 0, len);
        }

        return len;


        return len;
}


static int
parse_str_literal(
        struct expr_token *next,
        const char *src)
{
        const char *end = src;
        int len;

        assert(next);
        assert(src);

        /* check starts with literal */
        if (!is_str_literal(*src)) {
                return 0;
        }
        
        end += 1;

        /* continue until we find matching literal */
        while (*end != *src) {
                end += 1;
        }

        end += 1;

        len = end - src;

        if(len) {
                set_token(next, TOKID_STR_LIT, 0, 0, len);
        }

        return len;
}


static int
parse_ident(
        struct expr_token *next,
        const char *src)
{
        const char *end = src;
        int len;

        assert(next);
        assert(src);

        /* idents can't start with a number */
        if(!is_alpha(*end)) {
                return 0;
        }
       
        end += 1;

        /* continue with all alpha numeric */
        while (is_alpha_numeric(*end)) {
                end += 1;
        }

        len = end - src;

        if(len) {
                set_token(next, TOKID_IDENT, 0, 0, len);
        }

        return len;
}


static int
parse_unknown(
        struct expr_token *next,
        const char *src)
{
        (void)next;
        (void)src;

        set_token(next, TOKID_UNKNOWN, 0, 0, 1);
        return 1;
}


static int
parse_whitespace(
        struct expr_token *next,
        const char *src)
{
        const char *end = src;
        int len;

        assert(next);
        assert(src);

        if (!is_whitespace(*src)) {
                return 0;
        }
        
        while (is_whitespace(*end)) {
                end += 1;
        }

        len = end - src;

        if(len) {
                set_token(next, TOKID_WHITESPACE, 0, 0, len);      
        }

        return len;
}


static int
parse_punct(
        struct expr_token *next,
        const char *src,
        struct expr_sub_punctuation *sub_punct,
        int sub_punct_len)
{
        int i;

        for(i = 0; i < sub_punct_len; ++i) {
                int pat_len = strlen(sub_punct[i].pattern);
                int cmp = strncmp(src, sub_punct[i].pattern, pat_len);

                if(cmp == 0) {
                        int sub_id = sub_punct[i].token_sub_id;
                        set_token(next, TOKID_PUNCT, sub_id, 0, pat_len);

                        return pat_len;
                }
        }


        return 0;
}


/* --------------------------------------------------------- [ Interface ] -- */


typedef int(*parser_fn)(struct expr_token *, const char *);


struct expr_token*
expr_lexer_create(
        const char *src,
        struct expr_sub_punctuation *punctuation,
        int punctuation_len)
{
        /* variables */
        struct expr_token *start_token = calloc(sizeof(*start_token) * 1000, 1);
        struct expr_token *token = &start_token[0];
        const char *start = src;

        parser_fn parsers[] = {
                parse_hex_literal,
                parse_flt_literal,
                parse_num_literal,
                parse_str_literal,
                parse_whitespace,
                parse_ident,
                0 
        };

        /* check good state */
        assert(src);
        assert(strlen(src));

        /* param fail */
        if(!src || !strlen(src)) {
                return 0;
        }

        /* loop through characters */
        while(*src) {
                int consume = 0;
                printf("new token\n");

                parser_fn *par_fn = &parsers[0];

                while(*par_fn) {
                        consume = (*par_fn)(token, src);

                        if(consume) {
                                break;
                        }

                        par_fn += 1;
                }

                if(consume == 0) {
                        consume = parse_punct(
                                token,
                                src,
                                punctuation,
                                punctuation_len);
                }

                if(consume == 0) {
                        consume = parse_unknown(token, src);
                }

                assert(consume > 0);

                token->src_offset = src - start;

                token += 1;
                src += consume;
        }

        return start_token;
}


void
expr_lexer_destroy(
        struct expr_token *destroy)
{
        assert(destroy);

        if(!destroy) {
                return;
        }

        EXPR_VARR_DESTROY(destroy);
}
