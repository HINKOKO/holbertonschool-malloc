#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <unistd.h>

#define RED "\033[31m"
#define COLOR "\033[35m"
#define COLORDIFF "\033[32m"
#define RESET "\033[0m"

/* block zero => beginning of the heap allocated */
static void *block_zero = NULL;
/* var to keep track of chunks used */
static size_t chunk_count = 0;

void *naive_malloc_page(size_t size)
{
	void *block_ptr, *next_block, *payload_addr = NULL;
	size_t i, std_block_size, block_size, unused_block_size = 0;
	long pagesize;

	std_block_size = size + sizeof(size_t);
	(void)unused_block_size;
	(void)block_size;

	if (chunk_count == 0)
	{
		pagesize = sysconf(_SC_PAGE_SIZE);
		if (pagesize == -1)
		{
			perror("sysconf");
			return (NULL);
		}
		/* extend the program break by the virtual memory page size */
		block_zero = sbrk(pagesize);
		if (block_zero == (void *)-1)
			return (NULL);
		printf("%sNew program break is %p%s\n", RED, sbrk(0), RESET);
		printf("%Hey there Block Zero point to (beginning of heap)=> %p%s?\n", COLOR, block_zero, RESET);
		printf("%s Yeah cool I notice a diff of 0x1000 bytes == 4096 == pagesize between block_zero and the new program break %s\n", COLORDIFF, RESET);
	}
	/* loop through that mega chunk and chunky chunk */
	for (i = 0, block_ptr = block_zero; i < chunk_count; i++)
	{
		block_size = *(size_t *)block_ptr;
		block_ptr = (char *)block_ptr + block_size;
	}
	unused_block_size = chunk_count ? *(size_t *)block_ptr : (size_t)pagesize;

	next_block = (char *)block_ptr + std_block_size;

	*(size_t *)next_block = unused_block_size - std_block_size;
	*(size_t *)block_ptr = std_block_size;

	chunk_count++;

	payload_addr = (char *)block_ptr - sizeof(size_t);

	return (payload_addr);
}

/**
 * main - Program entry point
 *
 * Return: EXIT_SUCCESS or EXIT_FAILURE
 */
int main(void)
{
	char *str;
	int i;

	printf("Starting break is %p\n", sbrk(0));

	for (i = 0; i < 10; i++)
	{
		void *chunk;

		str = naive_malloc_page(1016);
		strcpy(str, "Holberton");
		str[9] = '\0';
		printf("%p: %s, ", (void *)str, str);
		chunk = (void *)(str - sizeof(size_t));
		printf("chunk addr: %p, ", (void *)chunk);
		printf("size: %lu, ", *((size_t *)chunk));
		printf("%sbreak: %p%s\n", RED, sbrk(0), RESET);
	}

	printf("%sFinal break is %p%s\n", RED, sbrk(0), RESET);
	return (EXIT_SUCCESS);
}
