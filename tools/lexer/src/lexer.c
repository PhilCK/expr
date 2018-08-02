#include <expr/lexer.h>
#include <expr/fundamental.h>
#include <expr/v_array.h>
#include <string.h>

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
contains(char needle, char *heystack)
{
        while (heystack) {
                if (needle == *heystack) {
                        return 1;
                }

                heystack += 1;
        }

        return 0;
}

static int is_whitespace(char c)    { return contains(c, whitespace); }
static int is_alpha(char c)         { return contains(c, alpha); }
static int is_numeric(char c)       { return contains(c, numeric); }
static int is_alpha_numeric(char c) { return is_alpha(c) || is_numeric(c); }
static int is_str_literal(char c)   { return contains(c, str_literal); }


/* ----------------------------------------------------------- [ Parsers ] -- */


static int
parse_num_literal(
        struct expr_token *next,
        const char *src)
{
        const char *start = src;

        if (!is_numeric(*src)) {
                return 0;
        }
        
        while (!is_whitespace(*src)) {
                if (is_numeric(*src)) {
                        src += 1;
                }
                else {
                        return 0;
                }
        }

        return src - start;
}


static int
parse_hex_literal(
        struct expr_token *next,
        const char *src)
{
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
        return 0;
}


static int
parse_ident(
        struct expr_token *next,
        const char *src)
{
        return 0;
}


static int
parse_unknown(
        struct expr_token *next,
        const char *src)
{
        return 0;
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
        struct expr_token *token = 0;

        parser_fn parsers[] = {
                parse_hex_literal,
                parse_flt_literal,
                parse_num_literal,
                parse_str_literal,
                parse_ident,
                0 /* terminator */
        };

        /* check good state */
        assert(src);
        assert(strlen(src));
        
        /* param fail */
        if(!src || !strlen(src)) {
                return 0;
        }

        /* loop through characters */
        while(src) {
                int i;
                int consume = 0;

                parser_fn *par_fn = parsers;

                while(par_fn) {
                        consume = (*par_fn)(token, src);
                        par_fn += 1;
                }

                if(consume == 0) {
                        consume = parse_punct(token, src, puntuation, puntuation_len);
                }
                else {
                        parse_unknown(token, src);
                }

                src += EXPR_MAX(1, consume);
        }

        return token;
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
