#include <expr/lexer.h>
#include <expr/fundamental.h>
#include <expr/v_array.h>
#include <string.h>
#include <assert.h>
#include <stdlib.h>
#include <stdio.h>


/* ------------------------------------------------------------ [ Tables ] -- */


const char  whitespace[] = {
        ' ', '\t', '\n', '\r',
        '\0'
};


const char numeric[] = {
        '0', '1', '2', '3', '4', '5', '6', '7', '8', '9',
        '\0'
};


const char alpha[] = {
        'a', 'A', 'b', 'B', 'c', 'C', 'd', 'D', 'e', 'E', 'f', 'F', 'g', 'G',
        'h', 'H', 'i', 'I', 'j', 'J', 'k', 'K', 'l', 'L', 'm', 'M', 'n', 'N',
        'o', 'O', 'p', 'P', 'q', 'Q', 'r', 'R', 's', 'S', 't', 'T', 'u', 'U',
        'v', 'V', 'w', 'W', 'x', 'X', 'y', 'Y', 'z', 'Z',
        '\0'
};


const char str_literal[] = {
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
        
        while (!is_whitespace(*end)) {
                if (is_numeric(*end)) {
                        end += 1;
                }
                else {
                        break;
                }
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
        const char *start = src;

        assert(next);
        assert(src);

        return 0;
}


static int
parse_flt_literal(
        struct expr_token *next,
        const char *src)
{
        return 0;
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

        if (!is_str_literal(*src)) {
                return 0;
        }
        
        end += 1;

        while (*end != *src) {
                printf(". %c\n", *end);
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

        if(!is_alpha(*end)) {
                return 0;
        }
       
        end += 1;

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
                printf("whitespace\n");
                next->id         = TOKID_WHITESPACE;
                next->sub_id     = TOKID_NULL;
                next->src_offset = 0;
                next->src_len    = len;
        }

        return len;
}


static int
parse_punct(
        struct expr_token *next,
        const char *src,
        struct expr_sub_puntuation *sub_punt,
        int sub_punt_len)
{
        return 0;
}


/* --------------------------------------------------------- [ Interface ] -- */


typedef int(*parser_fn)(struct expr_token *, const char *);


struct expr_token*
expr_lexer_create(
        const char *src,
        struct expr_sub_puntuation *puntuation,
        int puntuation_len)
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
                                puntuation,
                                puntuation_len);
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
