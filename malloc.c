#include "malloc.h"

/**
 * first_call - like an 'init_heap' function so to speak
 * called first when our _malloc is invoked to allocate
 * memory on the heap, aligned on the page_size of system
 *
 * @page: long variable to save the page size
 *
 * Return: pointer to the starting big heap block of size page
 */

void *first_call(long *page)
{
	void *block_zero;

	(*page) = sysconf(_SC_PAGESIZE);
	if ((*page) == -1)
	{
		perror("sysconf");
		return (NULL);
	}
	block_zero = sbrk((*page));
	if (block_zero == (void *)-1)
	{
		perror("First call to heap alloc: error\n");
		return (NULL);
	}
	return (block_zero);
}

/**
 * new_page - alloc new page on the heap
 * when it's not sufficient anymore
 * @ptr: pointer to alloce'd page
 * @chunks_len: Number of chunks
 * @block: size of block that has been asked for to alloc
 * @page: Value of the page of running system
 *
 * Return: Pointer to new alloce'd page
 */

void *new_page(void *ptr, size_t *chunks_len, size_t block, long page)
{
	void *next_page;
	size_t tmp = 0;

	tmp = *chunks_len ? *(size_t *)ptr : (size_t)page;
	next_page = (char *)ptr + block;

	while (tmp < block)
	{
		page = sysconf(_SC_PAGESIZE);
		if (page == -1)
			return (NULL);
		if (sbrk(page) == (void *)-1)
			return (NULL);
		tmp += page;
	}

	*(size_t *)next_page = tmp - block;
	*(size_t *)((char *)ptr + sizeof(block_t)) = block;
	return (ptr);
}

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
	static void *first_block;
	static size_t chunks_len;
	block_t *temp;
	void *ptr, *payload_addr;
	size_t block = ALIGN(size) + sizeof(block_t) + sizeof(size);
	size_t i, block_size, freed = 0;
	long page = 0;

	/* enters here when starting */
	if (chunks_len == 0)
	{
		first_block = first_call(&page);
		if (!first_block)
			return (NULL);
	}
	for (i = 0, ptr = first_block; i < chunks_len; ++i)
	{
		block_size = *(size_t *)((char *)ptr + sizeof(block_t));
		temp = (block_t *)ptr;
		if (temp->used == 0 && block_size <= block)
		{
			freed = 1;
			break;
		}
		ptr = (char *)ptr + block_size;
	}

	if (!freed)
		ptr = new_page(ptr, &chunks_len, block, page);
	temp = (block_t *)ptr;
	temp->start = first_block;
	temp->used = 1;
	chunks_len++;

	payload_addr = (char *)ptr + sizeof(block_t) + sizeof(size);

	return (payload_addr);
}
