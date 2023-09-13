#ifndef __MALLOC__
#define __MALLOC__

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>

/* print color for debugging and */
/* protect eyes from iiiiiiiiiiiniyaaaa */
#define RED "\033[31m"
#define COLOR "\033[35m"
#define COLORDIFF "\033[32m"
#define COLORALIGN "\033[36m"
#define RESET "\033[0m"

#define HEADER 0x10 /* 16 bytes of header for each chunk */
#define ALIGN(size) ((size + (HEADER - 1)) & ~(HEADER - 1))

void *naive_malloc(size_t size);
void *_malloc(size_t size);

#endif /* __MALLOC__ */
