#ifndef PRINT_H
#define PRINT_H

#include "boot.h"

void print_init(Video_Mode_Info *video);
const char *to_str(u64 val);
void print(const char *s);

#endif // PRINT_H
