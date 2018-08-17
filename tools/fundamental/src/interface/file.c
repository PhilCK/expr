#include <expr/file.h>
#include <stdio.h>


int
expr_file_load(
        const char *filename,
        const char **memory,
        unsigned *bytes)
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
                *bytes = (unsigned)length;
        }

        if(!memory) {
                return 0;
        }

        fseek(f, 0, SEEK_SET);
        fread((void*)*memory, 1, length, f);

        fclose(f);

        return 1;
}
