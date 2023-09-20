#include "malloc.h"

/**
 * _free - basic mimic of free function
 * @ptr: pointer to the memory space to be freed
 * (or not to be)
 */

void _free(void *ptr)
{
	if (ptr)
		((block_t *)((char *)ptr - sizeof(size_t) - sizeof(block_t)))->used = 0;
}
