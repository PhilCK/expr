#include <expr/ast.h>
#include <expr/lexer.h>
#include <stdio.h>
#include <string.h>


static const char *token_file = 0;
static const char *src_file = 0;
static struct expr_token *tokens = 0;
static const char *src = 0;
static int output_stream = 0;
static const char *output_file = 0;


/* ---------------------------------------------------------------- Config -- */


#define PRINT_ARG_PARSER 1
#define PRINT_DESER_TOKENS 1


/* ------------------------------------------------------------------ Args -- */


void
print_help()
{
        printf("usage: expr_ast <token_file> <src_file> <options>\n\n");
        printf("options\n");
        printf("-o <filename> -outputs the ast stream to a file\n");
        printf("-os           -outputs the ast stream to stdout\n");
        printf("-h            -outputs this dialog\n");
}


int
process_args(
        int argc,
        char **argv)
{
        int i;

        for(i = 1; i < argc; ++i) {
                
                if(PRINT_ARG_PARSER) {
                        printf("arg %s\n", argv[i]);
                }

                /* token file */
                if(strcmp(argv[i], "-o") == 0) {
                        if(argc < i + 1) {
                                return 0;
                        }

                        output_file = argv[i + 1];
                        ++i;
                }
                else if(strcmp(argv[i], "-os") == 0) {
                        output_stream = 1;
                }
                else if(strcmp(argv[i], "-h") == 0) {
                        print_help();
                }
                else if(!token_file) {
                        token_file = argv[i];
                }
                else if(token_file && !src_file) {
                        src_file = argv[i];
                }
        }

        if(!token_file || !src_file) {
                return 0;
        }

        if(PRINT_ARG_PARSER) {
                printf("token file %s\n", token_file);
                printf("src file %s\n", src_file);
                printf("print stream %d\n", output_stream);
                printf("out file stream %s\n", output_file);
        }
      
        return 1;
}


/* -------------------------------------------------------------- Internal -- */


struct expr_token*
load_tokens(const char *filename)
{
        printf("load\n");
        struct expr_lexer_deserialize_desc desc = {0};
        desc.type_id = EX_LEX_TYPEID_DESERIALIZE;
        desc.serialized_filename = filename;

        struct expr_token *toks = expr_lexer_deserialize(&desc);
        printf("loaded\n");
        return toks;
}




/*------------------------------------------------------------ Application -- */


int
main(int argc, char **argv) {

        /* args */
        int args_ok = process_args(argc, argv);

        if(!args_ok) {
                printf("Failed processing args\n");
                return 0;
        }

        /* rebuld node array */
        tokens = load_tokens(token_file);

        if(!tokens) {
                printf("Failed to get tokens\n");
                return 0;
        }

        /* todo - load src file */

        if(PRINT_DESER_TOKENS) {
                expr_lexer_print(tokens, src);
        }

        /* request ast with nodes */

        /* stream output */

        return 0;
}
