#ifndef MEMORY_H
#define MEMORY_H

#include <int.h>
#include "boot.h"

u64 memory_get_size(Memory_Map_Descriptor *mMap, u64 mMapSize, u64 mMapDescSize);

#endif // MEMORY_H
