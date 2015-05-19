#include "MemAllocator.h"

namespace MemAllocator
{
    void* allocate(const size_t size)
    {
        return malloc(size);
    }


    void deallocate(void* pointer)
    {
        free(pointer);
        pointer = NULL;
    }
}
