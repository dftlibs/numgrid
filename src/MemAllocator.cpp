

#include "MemAllocator.h"

namespace MemAllocator
{
    void* allocate(const size_t size)
    {
#ifdef VAR_INTEL_CXX
        return _mm_malloc((int)size, CPU_MEM_ALIGN);
#endif
#ifdef VAR_USEGCD
        // apple malloc intrinsically aligned
        return malloc(size);
#else
        void* pointer = NULL;
        int state = posix_memalign(&pointer, CPU_MEM_ALIGN, size);
        if (state != 0) return NULL;
        return pointer;
#endif
    }


    void deallocate(void* pointer)
    {
#ifdef VAR_INTEL_CXX
        _mm_free(pointer);
#else
        free(pointer);
#endif
        pointer = NULL;
    }
}
