#include "malloc.h"

/**
 * main - Program entry point
 *
 * ==> we request some 'size_t size' of allocation in our
 * naive_malloc_align => sive passed as parameters must be rounded up
 * to the next multiple of 8qq
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

		str = naive_malloc(512);
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
