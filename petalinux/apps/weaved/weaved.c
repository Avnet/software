/*
 * Placeholder PetaLinux user application.
 *
 * Replace this with your application code
 */
#include <stdio.h>

int main(int argc, char *argv[])
{
        printf("Hello, Weaved Installation!\n");
	printf("cmdline args:\n");
	while(argc--)
		printf("%s\n",*argv++);

	return 0;
}


