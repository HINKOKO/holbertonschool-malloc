#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

/**
 * main - entry point
 *
 * Return: SUCCESS || FAILURE haha
 *
 */

int main(void)
{
	char *ptr;

	printf("Starting break of program => %p\n", sbrk(0));

	ptr = sbrk(1); /* incrementing bank account */
	printf("Breakpoint extended, it is now => %p\n", sbrk(0));

	*ptr = 'H';
	printf("Address of our char is %p\n", ptr);
	printf("Value at this address is '%c'\n", *ptr);

	return (EXIT_SUCCESS);
}
