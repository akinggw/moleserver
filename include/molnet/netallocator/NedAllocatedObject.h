#ifndef _NEDALLOCATED_OBJECT_H_
#define _NEDALLOCATED_OBJECT_H_

#include "../common.h"

class NedAllocatedObject
{
public:
	explicit NedAllocatedObject()
	{

	}
	~NedAllocatedObject()
	{

	}

	void* operator new(size_t sz);

	/// placement operator new
	void* operator new(size_t sz, void* ptr);

	void* operator new[] ( size_t sz );
	void operator delete( void* ptr );

	void operator delete( void* ptr, void* );

	void operator delete[] ( void* ptr );
};


#endif //_NEDALLOCATED_OBJECT_H_
