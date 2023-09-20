#include "malloc.h"

static void *first_block;
static size_t chunks;

/**
 * _malloc - mimics malloc, Ramdeck ! simply
 *
 * @size: size requested to be malloced
 *
 * Return: pointer to allocated memory (for user) that is
 * " suitably aligned for any kind of variable " (man malloc)
 */

void *_malloc(size_t size)
{
	void *ptr, *prev_break, *payload_addr;
	size_t i, requested = 0, freed = 0, pad = 0;

	/* enters here when starting */
	if (!chunks)
	{
		prev_break = sbrk(PAGE);
		if (!prev_break)
			return (NULL);
		first_block = prev_break;
		memset(prev_break, 0, PAGE);
		/* do we need to write the size at first block ? */
	}
	for (i = 0, ptr = first_block; i < chunks; i++)
	{
		/* loop to traverse linked list of blocks */
		/* advance ptr by size of the blocks */
		ptr = ((char *)ptr) + *((size_t *)((char *)ptr));
	}
	/* value in freed */
	freed = *(size_t *)ptr;
	requested = DATA + size; /* add the 'header' to the block requested by user */
	pad = ALIGN(requested) - requested;
	requested += pad;

	while (((int)freed) - ((int)requested) < 0)
	{
		prev_break = sbrk(PAGE);
		memset(prev_break, 0, PAGE);
		freed += PAGE;
	}
	*(size_t *)ptr = requested;
	payload_addr = (char *)ptr + DATA;
	*((size_t *)(char *)ptr + requested) = freed - requested;
	chunks++;
	return (payload_addr);
}
