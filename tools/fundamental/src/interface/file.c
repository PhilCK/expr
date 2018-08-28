#include <expr/file.h>
#include <stdio.h>


int
expr_file_load(
        const char *filename,
        const char **memory,
        int *bytes)
{
        long length = 0;

        if(!filename) {
                return 0;
        }

        FILE *f = fopen(filename, "rb");

        if(!f) {
                return 0;
        }

        fseek(f, 0, SEEK_END);
        length = ftell(f);
        
        if(bytes) {
                *bytes = (int)length;
        }

        /* not a fail, just return */
        if(!memory) {
                return 1;
        }

        fseek(f, 0, SEEK_SET);
        fread((void*)*memory, 1, length, f);

        fclose(f);

        return 1;
}
