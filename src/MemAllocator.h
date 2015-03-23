#ifndef MemAllocator_h_
#define MemAllocator_h_

#include <cstdlib>

namespace MemAllocator
{
    // allocates aligned memory block
    void* allocate(const size_t size);

    // deallocates aligned memory block
    void deallocate(void* pointer);
}

#endif /* MemAllocator_h_ */
