#include "malloc.h"

/**
 * _free - basic mimic of free function
 * @ptr: pointer to the memory space to be freed
 * (or not to be)
 */

void _free(void *ptr)
{
	char *freeder;
	block_t *block;

	if (ptr)
	{
		freeder = (char *)ptr - sizeof(block_t);
		block = (void *)freeder;
		block->block_size = 0;
	}
}
