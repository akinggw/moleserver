#include "../../../include/molnet/netallocator/NedAllocatorImpl.h"

#if defined(__cplusplus)
extern "C" {
#endif
    #include "nedmalloc.c"
#if defined(__cplusplus)
}
#endif

void* allocBytes(size_t count)
{
	void* ptr = nedalloc::nedmalloc(count);
	return ptr;
}

void deallocBytes(void* ptr)
{
	if(!ptr)
		return;

	nedalloc::nedfree(ptr);
}

void* allocBytesAligned(size_t align, size_t count)
{
	const size_t SIMD_ALIGNMENT = 16;
	void* ptr =  align ? nedalloc::nedmemalign(align, count)
		: nedalloc::nedmemalign(SIMD_ALIGNMENT, count);

	return ptr;
}

void deallocBytesAligned(size_t align, void* ptr)
{
	if (!ptr)
		return;

	nedalloc::nedfree(ptr);
}

void* MemoryMalloc(size_t size)
{
    void* ptr = nedalloc::nedmalloc(size);
    return ptr;
 }

void MemoryFree( void* ptr )
{
    if(!ptr)
        return;

    nedalloc::nedfree(ptr);
}

void *Memorycalloc( size_t num,size_t size )
{
    void* ptr = nedalloc::nedcalloc(num,size);
    return ptr;
}

void *Memoryrealloc( void* memblock, size_t size )
{
    if(!memblock)
        return NULL;
    void* ptr = nedalloc::nedrealloc(memblock, size);
    return ptr;
}

void DestroyMemory()
{
    nedalloc::neddestroysyspool();
}
