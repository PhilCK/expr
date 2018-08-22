#include <expr/v_array.h>
#define _GNU_SOURCE
#include <sys/mman.h>


struct varray_header {
        void *addr;
};


void *
expr_varray_create(int bytes_of_item, int count)
{
        int length = bytes_of_item * count;
        int prot = PROT_READ | PROT_WRITE;
        int flags = MAP_SHARED | MAP_ANONYMOUS;

        void *addr = mmap(0, length, prot, flags, 0, 0);

        struct varray_header *header = (struct varray_header*)addr;
        header->addr = addr;

        return &header[1];
}
