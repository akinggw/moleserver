#ifndef _NEDALLOCATOR_IMPL_H_
#define _NEDALLOCATOR_IMPL_H_

#include "../common.h"

#if defined(__cplusplus)
extern "C" {
#endif

void* allocBytes(size_t count);

void deallocBytes(void* ptr);

void* allocBytesAligned(size_t align, size_t count);

void deallocBytesAligned(size_t align, void* ptr);

void* MemoryMalloc(size_t sz);

void MemoryFree( void* ptr);

void *Memorycalloc( size_t num,size_t size );

void *Memoryrealloc( void* memblock, size_t size );

void DestroyMemory();

#if defined(__cplusplus)
}
#endif

#endif //_NEDALLOCATOR_IMPL_H_
