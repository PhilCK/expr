#ifndef FILE_INCLUDED_
#define FILE_INCLUDED_


/*
 *  filename of the file you wish to load.
 *  memory to write the file, can be NULL,
 *  bytes the size in bytes of the file.
 *  returns 1 for success 0 for fail
 */
int
expr_file_load(
        const char *filename,
        char *memory,
        int *bytes);


/* inc guard */
#endif

