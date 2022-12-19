#ifndef BITMAP_H
#define BITMAP_H

#include <int.h>
#include <bool.h>

void bitmap_set(u8 *bitmap, u64 index, bool value);
bool bitmap_get(u8 *bitmap, u64 index);

#endif // BITMAP_H
