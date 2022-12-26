#include <int.h>
#include <bool.h>

void
bitmap_set(u8 *bitmap, u64 index, bool value)
{
	u64 byteIndex = index / 8;
	u8 bitIndex = 0b10000000 >> (index % 8);

	if (value)
		bitmap[byteIndex] |= bitIndex;
	else
		bitmap[byteIndex] &= ~bitIndex;
}

bool
bitmap_get(u8 *bitmap, u64 index)
{
	u64 byteIndex = index / 8;
	u8 bitIndex = 0b10000000 >> (index % 8);

	if (bitmap[byteIndex] & bitIndex)
		return true;
	return false;
}
