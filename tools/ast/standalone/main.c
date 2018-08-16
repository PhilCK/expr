#include <expr/ast.h>
#include <expr/lexer.h>
#include <stdio.h>
#include <string.h>


const char *token_file = 0;
const char *src_file = 0;
static struct expr_token *tokens = 0;
int output_stream = 0;
const char *output_file = 0;


/* ---------------------------------------------------------------- Config -- */


#define PRINT_ARG_PARSER 1


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
process_args(int argc, char **argv)
{
        int i;

        for(i = 1; i < argc; ++i) {
                
                if(PRINT_ARG_PARSER) {
                        printf("arg %s\n", argv[i]);
                }

                /* token file */
                if(strcmp(argv[i], "-o") == 0) {
                        if(argc < i + 1) {
                                printf("bad args\n");
                                return 0;
                        }
                }
                else if(strcmp(argv[i], "-os") == 0) {

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
                printf("bad args\n");
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
        desc.serialized_filename = "foo";

        struct expr_token *toks = expr_lexer_deserialize(&desc);
        printf("loaded\n");
        return toks;
}




/*------------------------------------------------------------ Application -- */


int
main(int argc, char **argv) {

        /* args */
        process_args(argc, argv);

        /* rebuld node array */
        /*tokens = load_tokens("foo");

        printf("go toks\n");

        expr_lexer_print(tokens, 0);*/

        /* request ast with nodes */

        /* stream output */

        return 0;
}
