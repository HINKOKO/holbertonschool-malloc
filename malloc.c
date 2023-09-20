#include "malloc.h"

/**
 * _malloc - reimplementation of malloc
 * @size: bytes requested
 * Return: SUCCESS= pointer to allocated memory; FAIL = NULL
 */
void *_malloc(size_t size)
{
	static void *heap_start;
	static size_t chunks;
	void *valid_unused_memory_ptr = NULL, *previous_break = NULL, *ptr = NULL;
	size_t unused_memory = 0, i = 0, mem_asked = 0, padding = 0;

	if (size == 0)
		return (NULL);

	if (!heap_start)
	{
		previous_break = sbrk(PAGE);
		if (previous_break == ((void *)-1))
			return (NULL);
		heap_start = previous_break;
		memset(previous_break, 0, PAGE);
		*((size_t *)((char *)heap_start)) = PAGE - sizeof(size_t);
	}

	for (i = 0, ptr = heap_start; i < chunks; i++)
		ptr = ((char *)ptr) + *((size_t *)((char *)ptr));

	unused_memory = *((size_t *)((char *)ptr));
	mem_asked = sizeof(size_t) + size;
	padding = (sizeof(size_t) - (mem_asked % sizeof(size_t))) % sizeof(size_t);
	mem_asked += padding;

	while (((int)unused_memory) - ((int)mem_asked) < 0)
	{
		previous_break = sbrk(PAGE);
		if (previous_break == ((void *)-1))
			return (NULL);
		memset(previous_break, 0, PAGE);
		unused_memory += PAGE;
	}

	*((size_t *)((char *)ptr)) = mem_asked;
	valid_unused_memory_ptr = (void *)(((char *)ptr) + sizeof(size_t));
	*((size_t *)(((char *)ptr) + mem_asked)) = unused_memory - mem_asked;
	chunks++;
	return (valid_unused_memory_ptr);
}
