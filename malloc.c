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
	void *payload_addr = NULL, *prev_break = NULL, *ptr = NULL;
	size_t unused_mem = 0, i = 0, requested = 0, padding = 0;

	if (size == 0)
		return (NULL);

	if (!first_block)
	{
		prev_break = sbrk(PAGE);
		if (prev_break == ((void *)-1))
			return (NULL);
		first_block = prev_break;
		memset(prev_break, 0, PAGE);
		*((size_t *)((char *)first_block)) = PAGE - sizeof(size_t);
	}

	for (i = 0, ptr = first_block; i < chunks; i++)
	{
		/* iterate over the block to find a place to fit */
		ptr = ((char *)ptr) + *((size_t *)((char *)ptr));
	}

	unused_mem = *((size_t *)((char *)ptr));
	requested = sizeof(size_t) + size;
	padding = (sizeof(size_t) - (requested % sizeof(size_t))) % sizeof(size_t);
	requested += padding;

	while (((int)unused_mem) - ((int)requested) < 0)
	{
		/* when request exceed available unused memory */
		/* from the previous call to sbrk(PAGE) */
		prev_break = sbrk(PAGE);
		/* this check because at some point memory might be exhausted*/
		if (prev_break == (void *)-1)
			return (NULL);
		memset(prev_break, 0, PAGE);
		/* update your new memory credit accordingly */
		unused_mem += PAGE;
	}

	*((size_t *)((char *)ptr)) = requested;
	payload_addr = (void *)(((char *)ptr) + sizeof(size_t));
	*((size_t *)(((char *)ptr) + requested)) = unused_mem - requested;
	chunks++;
	return (payload_addr);
}
