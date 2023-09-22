#ifndef __MALLOC__
#define __MALLOC__

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>

/* printing in color for debugging */
#define RED "\033[31m"
#define COLOR "\033[35m"
#define COLORDIFF "\033[32m"
#define COLORALIGN "\033[36m"
#define RESET "\033[0m"

#define HEADER 0x10 /* 16 bytes of header for each chunk */
#define DATA sizeof(size_t)
#define ALIGN(size) ((size + (DATA - 1)) & ~(DATA - 1))
#define PAGE (sysconf(_SC_PAGESIZE))

/**
 * struct block_s - struct to register blocks of memory
 * @block_size: size
 */

typedef struct block_s
{
	size_t block_size;
} block_t;

void *naive_malloc(size_t size);
void *_malloc(size_t size);
void _free(void *ptr);

#endif /* __MALLOC__ */
