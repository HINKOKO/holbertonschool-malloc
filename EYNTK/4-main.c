#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <unistd.h>

#define RED "\033[31m"
#define COLOR "\033[33m"
#define RESET "\033[0m"

static void *block_zero;
static size_t chunks = 0;

void *naive_malloc_page(size_t size)
{
	void *block_ptr = NULL, *next_block = NULL, *addr = NULL;
	size_t std_block_size = 0, i, block_size, unused_block_space = 0;
	long pagesize = 0;

	std_block_size = size + sizeof(size_t);

	if (chunks == 0)
	{
		/* first call no chunks allocated to boom a PAGE_SIZE right away */
		pagesize = sysconf(_SC_PAGE_SIZE);
		if (pagesize == -1)
		{
			fprintf(stderr, "naive_malloc_page: sysconf error");
			return (NULL);
		}
		block_zero = sbrk(pagesize);
		if (block_zero == (void *)-1)
		{
			perror("naive_malloc_page: sbrk error");
			return (NULL);
		}
		printf("%sblock zero points to %p%s\n", COLOR, block_zero, RESET);
		printf("%sProgram break after page alloc is %p%s\n", RED, sbrk(0), RESET);
	}

	/**
	 * analysis on sbrk(0) and first_chunk pointer reveals a difference of
	 * 0x1000 bytes => 4096 bytes == page size on the system
	 * let's go to iterate through that
	 */
	/* random brk offset, not contiguous with program I think */

	for (i = 0, block_ptr = block_zero; i < chunks; i++)
	{
		// if (i == 0)
		// 	printf("blocksize when i is 0 is %lu\n", block_size);
		block_size = *(size_t *)block_ptr;
		block_ptr = (char *)block_ptr + block_size;
	}
	unused_block_space = chunks ? *(size_t *)block_ptr : (size_t)pagesize;
	// printf("%sunused at first call%lu%s\n", COLOR, tmp, RESET);
	next_block = (char *)block_ptr + std_block_size;
	*(size_t *)next_block = unused_block_space - std_block_size;
	*(size_t *)block_ptr = std_block_size;
	chunks++;

	addr = (char *)block_ptr + sizeof(size_t);
	return (addr);
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

		str = naive_malloc_page(10);
		strcpy(str, "Holberton");
		str[9] = '\0';
		printf("%p: %s, ", (void *)str, str);
		chunk = (void *)(str - sizeof(size_t));
		printf("chunk addr: %p, ", (void *)chunk);
		printf("size: %lu, ", *((size_t *)chunk));
		printf("%sbreak: %p%s\n", RED, sbrk(0), RESET);
	}

	printf("Final break is %p\n", sbrk(0));
	return (EXIT_SUCCESS);
}
