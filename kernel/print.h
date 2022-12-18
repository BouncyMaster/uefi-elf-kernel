#ifndef PRINT_H
#define PRINT_H

#include "boot.h"

void print_init(Video_Mode_Info *video);
const char *u_to_str(u64 val);
void print(const char *s);
void print_lf(void);

#endif // PRINT_H
