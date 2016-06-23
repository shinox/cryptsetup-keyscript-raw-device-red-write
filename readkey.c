#include <stdio.h>
#include <stdlib.h>
#include <string.h>

void main(int argc, char *argv[])
{
	int blockSize = 512;
	int keySize = 2048;

	if (  argc == 2 
           && (sizeof(argv[1]) / sizeof(char)) > 1
	   && (atoi(argv[1]) % 512) == 0
	   )
		keySize = atoi(argv[1]);

	int count;

	char *block;

	FILE *f;

	/* Verify if key is multiple of blocks */
	int numBlocks = 0;
	if (keySize % 512 != 0) {
	   printf("\nSory but key size is not multiple of block size, try again. TA.\n");
	   exit(1);
	}
	

	f = fopen("/dev/sdb", "r");
	if(f == NULL) exit(EXIT_FAILURE);

	/* Seek till the end to get disk size and position to start */
	fseek(f, 0, SEEK_END);

	/* Determine where is the end */
	long endOfDisk = ftell(f);

	/* Make sure we start again */
	rewind(f); // Do I need it ???

	/* Get the required amount minus block size */
	long startFrom = endOfDisk - blockSize - keySize;

	/* Allocate space for bloc */
	block = calloc(keySize, sizeof(char));

	/* Start reading from specified block */
	fseek(f, startFrom, SEEK_SET);
	fread(block, 1, keySize, f);

	/* Do something with the data */
	for(count = 0; count < keySize/*sizeof(block)*/; count++){
		printf("%c", block[count]);
	}

	/* Close file */
	fclose(f);

	/* Make sure freed array is zeroed */
	memset(block, 0, keySize);
	free(block);
}

