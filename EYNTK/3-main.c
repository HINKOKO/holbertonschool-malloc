#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

void *naive_malloc_header(size_t size)
{
	void *brk, *addr;
	size_t block_size;

	/* initial breakpoint */
	brk = sbrk(0);
	/* get proper size desired and then set new breakpoint will sbrk() call */
	block_size = size + sizeof(size_t);
	/* let's move forward by 'block_size' bytes */
	addr = sbrk(block_size);
	if (addr == (void *)-1)
		return (NULL);
	/* write the size of the block at the beginning of allocated memory */
	*(size_t *)brk = block_size;
	/* point addr immediately after the size_t field */
	addr = (char *)brk + sizeof(size_t);
	/* skip the header of 8 bytes */

	return (addr);
}

/**
 * main -entry point of naiveness
 *
 */

int main(void)
{
	char *str;
	int i;

	printf("Starting breakpoint is %p\n", sbrk(0));

	for (i = 0; i < 10; i++)
	{
		void *chunk;

		str = naive_malloc_header(10);
		strcpy(str, "Holberton");
		printf("%p: %s, ", (void *)str, str);
		chunk = (void *)(str - sizeof(size_t));
		printf("Chunk starting at address %p, ", (void *)chunk);
		printf("chunk size is %lu\n", *((size_t *)chunk));
	}
	printf("Final break is %p\n", sbrk(0));
	return (EXIT_SUCCESS);
}
