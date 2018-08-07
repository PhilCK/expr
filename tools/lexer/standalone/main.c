#include <expr/lexer.h>
#include <expr/fundamental.h>
#include <stdio.h>
#include <string.h>
#include <stdlib.h>

/* -------------------------------------------------- [ Application Vars ] -- */


const char *mapping_file = 0;
const char *input_file = 0;


/* ---------------------------------------------------------- [ Internal ] -- */


int
process_args(int argc, char **argv)
{
        int i;

        for(i = 1; i < argc; ++i) {
                printf("arg %s\n", argv[i]);

                /* mapping */
                if(strcmp(argv[i], "-m") == 0) {
                        if(argc <  i + 1) {
                                printf("Missing Arg\n");
                                return 0;
                        }
                        
                        mapping_file = argv[i + 1];
                        ++i;
                }

                /* file to read */
                else {
                        if(input_file) {
                                printf("Input File already set\n");
                                return 0;
                        }

                        input_file = argv[i];
                }
        }

       return !!input_file; 
}


unsigned
load_file(const char *filename, const char **out_src)
{
        FILE *file = fopen(filename, "rb");
        unsigned length = 0;
        const char *src = 0;

        if (file) {
                fseek(file, 0, SEEK_END);
                length = ftell(file);
                fseek(file, 0, SEEK_SET);
                src = calloc(1, length + 1);
                
                fread((void *)src, 1, length, file);            
                fclose(file);

                if(out_src) {
                        *out_src = src;
                }
        }

        return length;
}


/* ------------------------------------------------------- [ Applicatoin ] -- */


int
main(int argc, char **argv)
{
        /* arguments */ 
        int i = process_args(argc, argv);
        const char *src = 0;
        const char *mapping_src = 0;

        struct expr_sub_punctuation *expr_sub_punct = 0;
        int expr_sub_punct_count = 0;
        
        if(i == 0) {
                printf("Failed processing args\n");
                return 1;
        }

        printf("%d %s %s\n", argc, mapping_file, input_file);
        printf("Expr Lexer\n");

        unsigned input_len = load_file(input_file, &src);

        if(!input_len) {
                printf("Failed to open input file");
                return 0;
        }

        if(mapping_file) {
                unsigned mapping_len = load_file(mapping_file, &mapping_src);

                if(!mapping_len) {
                        printf("Failed to open mapping file");
                        return 0;
                }

                /* number of lines */
                const char *lines = mapping_src;
                int line_count = 0;

                while(*lines) {
                        if(*lines == '\n') {
                                line_count += 1;
                        }

                        lines += 1;
                }

                /* line count */
                expr_sub_punct_count = line_count;
                expr_sub_punct = malloc(sizeof(*expr_sub_punct) * line_count);
                lines = mapping_src;

                unsigned i;
                for(i = 0; i < line_count; ++i) {
                        expr_sub_punct[i].pattern = mapping_src;
                        expr_sub_punct[i].token_sub_id = i;

                        while(*mapping_src != '\n') {
                                mapping_src += 1;
                        }

                        *((char*)mapping_src) = '\0';
                        mapping_src += 1;

                        printf("%s\n", expr_sub_punct[i].pattern);
                }
        }
         
        /* create tokens setup  */ 
        struct expr_lexer_create_desc lex_desc;
        lex_desc.type_id           = EX_LEX_TYPEID_CREATE;
        lex_desc.ext               = 0;
        lex_desc.src               = src;
        lex_desc.punctuation       = expr_sub_punct;
        lex_desc.punctuation_count = expr_sub_punct_count;
        lex_desc.skip_whitespace   = 1;

        struct expr_token *toks = expr_lexer_create(&lex_desc);
        struct expr_token *tok = &toks[0];

        while(tok->id != TOKID_NULL) {
                printf("tok: (%d|%d) - [%.*s]\n",
                    tok->id,
                    tok->sub_id,
                    tok->src_len,
                    &src[tok->src_offset]);
                tok += 1;
        }

        printf("end\n");

        return 0;
}
