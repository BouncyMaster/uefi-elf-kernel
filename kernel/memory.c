#include <int.h>
#include "boot.h"

u64
memory_get_size(Memory_Map_Descriptor *mMap, u64 mMapSize, u64 mMapDescSize)
{
	Memory_Map_Descriptor *desc;
	u64 memSize = 0;

	for (u64 i = 0; i < mMapSize; i+=mMapDescSize){
		desc = mMap + i;
		memSize += desc->count * 4096;
	}

	return memSize;
}
