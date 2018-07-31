#include <expr/lexer.h>
#include <expr/fundamental.h>


/* ----------------------------------------------------------- [ Helpers ] -- */


static int
parse_num_literal(
        struct expr_token *next,
        const char *src)
{
        return 0;
}


static int
parse_hex_literal(
        struct expr_token *next,
        const char *src)
{
        return 0;
}


static int
parse_bin_literal(
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
parse_punct(
        struct expr_token *next,
        const char *src,
        struct expr_sub_puntuation *sub_punt,
        int sub_punt_len)
{
        return 0;
}


static int
parse_puntuation


/* --------------------------------------------------------- [ Interface ] -- */


typedef int(*parser_fn)(struct expr_token *, const char *);


struct expr_token*
expr_lexer_create(
        const char *src,
        struct expr_sub_puntuation *puntuation,
        int puntuation_len)
{
        /* variables */
        struct expr_token *token = EXPR_VARR_CREATE(100000);

        parser_fn parsers[] = {
                parse_num_literal,
                parse_hex_literal,
                parse_bin_literal,
                parse_flt_literal,
                parse_str_literal,
                parse_ident,
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
                int parser_count = EXPR_ARR_COUNT(parsers);

                for(i = 0; i < parser_count; ++i) {
                        consume = parsers(token, src);
                };

                if(consume == 0) {
                        parse_punt(token, src);
                }

                src += EXPR_MAX(1, consume);

                /* new token */
                EXPR_ARR_PUSH(token);
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
