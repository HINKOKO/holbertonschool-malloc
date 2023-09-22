#include "malloc.h"

static void *first_block;
static size_t chunks;

/**
 * _malloc - attempt to mimics the malloc
 * @size: bytes requested by user
 * Return: pointer to 'payload' address , pointer to allocated memory
 * or NULL on failure
 */

void *_malloc(size_t size)
{
	void *prev_break = NULL, *ptr = NULL;
	void *payload_addr = NULL; /* address returned to user */
	size_t i, block_size, unused = 0, requested = 0, padding = 0;

	if (!first_block)
	{
		prev_break = sbrk(PAGE);
		if (first_block == (void *)-1)
			return (NULL);
		first_block = prev_break;
		memset(prev_break, 0, PAGE);
		*((size_t *)((char *)first_block)) = PAGE - DATA;
	}

	/* iterate to find fitting_block */
	for (i = 0, ptr = first_block; i < chunks; i++)
		ptr = ((char *)ptr) + *((size_t *)((char *)ptr));

	unused = *((size_t *)((char *)ptr));
	requested = size + DATA;
	padding = (DATA - (requested & (DATA - 1))) & (DATA - 1);
	requested += padding;

	/*enters here if unused gets not sufficient */
	while (((int)unused) - ((int)requested) < 0)
	{
		prev_break = sbrk(PAGE);
		if (prev_break == (void *)-1)
			return (NULL);
		memset(prev_break, 0, PAGE);
		unused += PAGE;
	}
	*(size_t *)ptr = requested;
	payload_addr = (void *)(((char *)ptr) + DATA);
	*((size_t *)(((char *)ptr) + requested)) = unused - requested;
	/* update chunks size */
	chunks++;
	return (payload_addr);
}
