#include <expr/lexer.h>
#include <stdio.h>
#include <string.h>


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



/* ------------------------------------------------------- [ Applicatoin ] -- */


int
main(int argc, char **argv)
{
        printf("foifo\n");
        /* arguments */ 
        /*
        int i = process_args(argc, argv);
        if(i == 0) {
                printf("Failed processing args\n");
                return 1;
        }

        printf("%d %s %s\n", argc, mapping_file, input_file);
        printf("Expr Lexer\n");
        */

        const char *src = "123 \"456\"\t'789' hello789 world 0xFF00FF, 1.234";
        printf("start\n");

        struct expr_token *toks = expr_lexer_create(src, 0, 0);
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
