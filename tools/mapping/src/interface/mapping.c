#include <expr/mapping.h>
#include <expr/lexer.h>
#include <expr/fundamental.h>


/* ----------------------------------------------------------- [ Mapping ] -- */




/* --------------------------------------------------------- [ Interface ] -- */


void
expr_mapping_get_patterns(
        int mapping_type,
        struct expr_sub_punctuation *map,
        int *map_count)
{
        switch(mapping_type) {
        case(EX_MAPPING_C): {
                static const char *mapping_c[] =  {
                        "&&", "&=", "&",
                        "||", "|=", "|",
                        "^=", "^",
                        "+=", "+",
                        "-=", "-",
                        "/=", "/",
                        "*=", "*",
                        "!=", "!",
                        "==", "=",
                        ">=",
                        "<=",
                        ".",
                        ",",
                        ";",
                        "(", ")",
                        "[", "]",
                        "<", ">",
                        "{", "}",
                        "//",
                        "/*", "*/",
                };

                if(map_count) {
                        *map_count = EX_ARR_COUNT(mapping_c);
                }

                if(map) {
                        int i;
                        int c = EX_ARR_COUNT(mapping_c);

                        for(i = 0; i < c; ++i) {
                                map[i].pattern = mapping_c[i];
                                map[i].token_sub_id = i;
                        }
                }
                            
        }
        }; /* switch */ 
}

