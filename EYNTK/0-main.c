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
	void *b_1, *b_2;

	b_1 = sbrk(0); /* sbrk with increment 0 used to find current location of program break */
	printf("Starting break is %p\n", b_1);

	b_2 = sbrk(1);
	if (b_2 == (void *)-1)
		return (EXIT_FAILURE);

	b_2 = sbrk(0); /* reading new program break extended by one byte */
	printf("New breakpoint after extension is %p\n", b_2);

	/* assign 'H' to pointer */
	*((char *)b_1) = 'H';
	printf("Address of char 'H' is %p\n", b_1);
	printf("Value at this address is '%c'\n", *((char *)b_1));

	return (EXIT_SUCCESS);
}
