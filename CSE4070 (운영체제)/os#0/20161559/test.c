#include <stdio.h>

int main(void) {


	int x;
	int* address = &x;

	unsigned address2 = (unsigned)(unsigned long)address;

	printf("%u\n", address2);

	return 0;
}
