#include <expr/v_array.h>

#ifdef __linux__
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


#endif


#ifdef _WIN32
#include <Windows.h>


struct varray_header {
        char *addr;
        char *curr;
        char *end;
        DWORD page_size;
        int stride;
};


void *
expr_varray_create(int bytes_of_item, int count)
{
        int bytes = bytes_of_item * count;
        char *addr = (char*)VirtualAlloc(
                NULL,
                bytes,
                MEM_RESERVE,
                PAGE_NOACCESS);

        /* we need the page size */
        SYSTEM_INFO sys_info;
        GetSystemInfo(&sys_info);

        DWORD page_size = sys_info.dwPageSize;

        addr = (char*)VirtualAlloc(
                addr,
                page_size,
                MEM_COMMIT,
                PAGE_EXECUTE_READWRITE);

        struct varray_header *header = (struct varray_header*)addr;
        header->addr = (char*)addr;
        header->end = addr + page_size;
        header->curr = addr + sizeof(*header);
        header->page_size = page_size;
        header->stride = bytes_of_item;

        return header->curr;
}


void *
expr_varray_push(void *arr)
{
        /* get header */
        struct varray_header *header = (struct varray_header*)arr;
        header -= 1;

        /* if a new push spills then add a new item */
        if (header->curr + header->stride > header->end) {
                header->addr = (char*)VirtualAlloc(
                        header->addr,
                        header->page_size,
                        MEM_COMMIT,
                        PAGE_EXECUTE_READWRITE);
        }

        header->curr += header->stride;

        return (void*)header->curr;
}

#endif
