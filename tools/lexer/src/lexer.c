#include <expr/lexer.h>
#include <expr/fundamental.h>
#include <expr/v_array.h>
#include <string.h>
#include <assert.h>
#include <stdlib.h>
#include <stdio.h>


/* ------------------------------------------------------------ [ Tables ] -- */


static const char whitespace[] = {
        ' ', '\t', '\n', '\r',
        '\0'
};


/* must match whitespace[] */
static int whitespace_sub_id[] = {
        EX_TOKID_WS_SPACE,
        EX_TOKID_WS_TAB,
        EX_TOKID_WS_NEWLINE,
        EX_TOKID_WS_NEWLINE, 
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
        '-', '_',
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

        if(!len) {
                return 0;
        }
        
        set_token(next, EX_TOKID_LITERAL, EX_TOKID_LIT_NUM, 0, len);

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

        if(!len) {
                return 0;
        }
        
        set_token(next, EX_TOKID_LITERAL, EX_TOKID_LIT_HEX, 0, len);

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

        /* floats have to start with a number */
        if (!is_numeric(*src)) {
                return 0;
        }

        end += 1;
       
        /* must be numbers and a single decimal */ 
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

        if(!len) {
                return 0;
        }

        set_token(next, EX_TOKID_LITERAL, EX_TOKID_LIT_FLT, 0, len);

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

        if(!len) {
                return 0;
        }
        
        set_token(next, EX_TOKID_LITERAL, EX_TOKID_LIT_STR, 0, len);

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

        if(!len) {
                return 0;
        }
        
        set_token(next, EX_TOKID_IDENT, 0, 0, len);

        return len;
}


static int
parse_unknown(
        struct expr_token *next,
        const char *src)
{
        (void)next;
        (void)src;

        set_token(next, EX_TOKID_UNKNOWN, 0, 0, 1);
        return 1;
}


static int
parse_whitespace(
        struct expr_token *next,
        const char *src)
{
        const char *end = src;
        int len, i, sub_id;

        assert(next);
        assert(src);

        if (!is_whitespace(*src)) {
                return 0;
        } 

        while (*end == *src) {
                end += 1;
        }

        len = end - src;

        if(len) {
                sub_id = 0;

                for(i = 0; i < EX_ARR_COUNT(whitespace_sub_id); ++i) {
                        if(*src == whitespace[i]) {
                                sub_id = whitespace_sub_id[i];
                                break;
                        }
                }

                set_token(next, EX_TOKID_WHITESPACE, sub_id, 0, len);      
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
                        set_token(next, EX_TOKID_PUNCT, sub_id, 0, pat_len);

                        return pat_len;
                }
        }


        return 0;
}


/* -------------------------------------------------------------- Lifetime -- */


typedef int(*parser_fn)(struct expr_token *, const char *);


struct expr_token*
expr_lexer_create(
        struct expr_lexer_create_desc *desc)
{
        /* variables */
        struct expr_token *start_token = 0;
        ex_varr_create(start_token, 1 << 24);

        struct expr_token *token = start_token;

        const char *start = 0;
        const char *src = 0;

        parser_fn parsers[] = {
                parse_hex_literal,
                parse_flt_literal,
                parse_num_literal,
                parse_str_literal,
                parse_ident,
                parse_whitespace,
                0 
        };

        /* check good state */
        assert(desc);
        assert(desc->src);
        assert(strlen(desc->src));

        start = desc->src;
        src = start;
        
        /* param fail */
        if(!src || !strlen(src)) {
                return 0;
        }

        /* loop through characters */
        while(*src) {
                set_token(token, EX_TOKID_NULL, 0, 0, 0);      

                int consume = 0;

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
                                desc->punctuation,
                                desc->punctuation_count);
                }

                if(consume == 0) {
                        consume = parse_unknown(token, src);
                }

                assert(consume > 0);

                token->src_offset = src - start;

                if(desc->skip_whitespace && token->id == EX_TOKID_WHITESPACE) {
                        set_token(token, EX_TOKID_NULL, 0, 0, 0);      
                }
                else {
                        ex_varr_push(start_token, token);
                }

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


/* ----------------------------------------------------------------- Print -- */


void
expr_lexer_print(
        struct expr_token *toks,
        const char *src)
{
        struct expr_token *t = &toks[0];

        assert(toks);

        if(!toks) {
                return;
        }

        if(src) {
                while(t->id != EX_TOKID_NULL) {
                        printf("%d tok: (%d|%d) - [%.*s]\n",
                                (int)(t - toks),
                                t->id,
                                t->sub_id,
                                t->src_len,
                                &src[t->src_offset]);

                        t += 1;
                }
        }
        else {
                while(t->id != EX_TOKID_NULL) {
                        printf("%d tok: (%d|%d) - [%d|%d]\n",
                                (int)(t - toks),
                                t->id,
                                t->sub_id,
                                t->src_offset,
                                t->src_len);

                        t += 1;

                }
        }
}


/* ------------------------------------------------------------- Serialize -- */


int
expr_lexer_serialize(
        struct expr_lexer_serialize_desc *desc)
{
        FILE *file = 0;
        file = fopen(desc->serialize_filename, "wb");

        if(!file) {
                printf("Failed to open file to serialize!\n");
                return 0;
        }

        struct expr_token *t = &desc->tokens[0];

        while(t->id != EX_TOKID_NULL) {
                char line[64] = {0};
                sprintf(
                        line,
                        "%d %d %d %d",
                        t->id,
                        t->sub_id,
                        t->src_offset,
                        t->src_len);

                fputs(line, file);
                putc('\n', file);

                t += 1;
        }

        fclose(file);

        return 1;
}


struct expr_token*
expr_lexer_deserialize(
        struct expr_lexer_deserialize_desc *desc)
{
        FILE *file = 0;
        file = fopen(desc->serialized_filename, "r");

        if(!file) {
                printf("Failed to open file to deserialize!\n");
                return 0;
        }

        struct expr_token *start_token = calloc(sizeof(*start_token) * 1000, 1);
        struct expr_token *t = start_token;

        const char *fmt = "%d %d %d %d[^\n]";

        while(fscanf(
                      file,
                      fmt,
                      &t->id,
                      &t->sub_id,
                      &t->src_offset,
                      &t->src_len) == 4)
        {
                t += 1;
        }

        return start_token;
}


