#include <stdio.h>
#include <stdlib.h>


int main(int argc, char* argv[])
{
	if(argc == 1)
		return 1;

	int lowerLimit = atoi(argv[1]);
	int upperLimit = atoi(argv[2]);

	for(int i = lowerLimit; i <= upperLimit; i++) {
		printf("0x%02x, ", i);
	}

	printf("\n");

	return 0;
}