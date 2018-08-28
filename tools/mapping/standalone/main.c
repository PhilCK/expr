#include <expr/mapping.h>
#include <expr/lexer.h>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>


int
main(int argc, char **argv)
{
        (void)argc;
        (void)argv;

        int count = 0;
        int i;
        struct expr_sub_punctuation *map = 0;

        expr_mapping_get_patterns(EX_MAPPING_C, 0, &count);

        map = malloc(sizeof(*map) * count);

        expr_mapping_get_patterns(EX_MAPPING_C, map, 0);

        FILE *file = fopen("c_mapping", "w");

        if(!file) {
                printf("Failed to open file");
                return 0;
        }

        for(i = 0; i < count; ++i) {
                fwrite(map[i].pattern, strlen(map[i].pattern), 1, file);
                fwrite("\n", 1, 1, file);
        }

        fclose(file);

        return 0;
}
