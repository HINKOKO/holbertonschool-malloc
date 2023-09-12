#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>

int main(void)
{
	long page_size = sysconf(_SC_PAGE_SIZE);

	if (page_size == -1)
	{
		perror("sysconf");
		return (EXIT_FAILURE);
	}
	printf("Page size on your system is %ld bytes\n\n", page_size);

	return (EXIT_SUCCESS);
}
