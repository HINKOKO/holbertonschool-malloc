#include "malloc.h"

/**
 * first_call - function to mimic malloc first allocation
 * of a page-sized block when called.
 * => Just a piece of code for 'init' the first memory allocation
 * of size 'page' according to system
 * @page: size of the page
 * Return: pointer to the first chunk of allocated page
 */

void *first_call(ssize_t *page)
{
	void *first_chunk;

	(*page) = sysconf(_SC_PAGESIZE);
	if ((*page) == -1)
	{
		perror("sysconf");
		return (NULL);
	}
	first_chunk = sbrk((*page));
	if (first_chunk == (void *)-1)
	{
		perror("sbrk error");
		return (NULL);
	}
	return (first_chunk);
}

/**
 * _malloc - naive & simple implementation of malloc library call
 * @size: size needed to be allocated by user
 *
 * Return: a pointer to the allocted memory, suitably aligned
 * FOR ANY KIND OF VARIABLE
 */

void *_malloc(size_t size)
{
	static void *block_zero;
	static size_t chunks;
	void *block_ptr, *next_block, *payload_addr = NULL;
	size_t i, std_block_size, block_size, unused_block_size = 0;
	long pagesize = 0;

	std_block_size = ALIGN(size) + sizeof(size_t);

	if (!chunks)
		block_zero = first_call(&pagesize);

	if (!block_zero)
		return (fprintf(stderr, "first_call: failed to allocate page size"), NULL);

	for (i = 0, block_ptr = block_zero; i < chunks; i++)
	{
		block_size = *(size_t *)block_ptr;
		block_ptr = (char *)block_ptr + block_size;
	}

	unused_block_size = chunks ? *(size_t *)block_ptr : (size_t)pagesize;
	next_block = (char *)block_ptr + std_block_size;

	while (unused_block_size < (sizeof(size_t) * 2) + ALIGN(size))
	{
		pagesize = sysconf(_SC_PAGESIZE);
		if (pagesize == -1)
			return (NULL);
		if (sbrk(pagesize) == (void *)-1)
			return (NULL);
		unused_block_size += pagesize;
	}

	*(size_t *)next_block = unused_block_size - std_block_size;
	*(size_t *)block_ptr = std_block_size;
	chunks++;

	payload_addr = (char *)block_ptr + sizeof(size_t);
	return (payload_addr);
}
