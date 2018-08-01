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
        /* arguments */ 
        int i = process_args(argc, argv);
        if(i == 0) {
                printf("Failed processing args\n");
                return 1;
        }

        printf("%d %s %s\n", argc, mapping_file, input_file);

        printf("Expr Lexer\n");
        return 0;
}
