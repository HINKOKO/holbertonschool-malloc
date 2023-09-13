#include <stdio.h>

/**
 * test/training file to show that order of declaration and constraints
 * due to memory alignement can lead to a bigger size than expected !
 * compile an run to see that more bytes in expected for struct no_align
 */

struct no_align
{
	char c;
	double d;
	int i;
	char c2no[3];
};

struct align
{
	double d;
	int i;
	char c2[3];
	char c;
};

int main(void)
{
	printf("size of no_align => %zu\tsize of align => %zu\n", sizeof(struct no_align), sizeof(struct align));
	return (0);
}

/** after compilation the struct no_align more looks like this in memory
 *
 * struct no_align_compiled
 * {
 * 		char c;
 * 		char __pad1 [7]
 * 		double d;
 * 		int i;
 * 		char c2 [3]
 * 		char __pad2;
 * }
 *
 * __pad1 [7] permits double d to start at address modulo 8
 * __pad2 "fills the struct" to reach a modulo 8 address
 *
 * => gcc trick => compile with -Wpadded to determine if your code declaration requires padding
 */
