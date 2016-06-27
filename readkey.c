#include <stdio.h>
#include <stdlib.h>
#include <string.h>


void main(int argc, char *argv[])
{
	int blockSize = 512;
	int keySize = 2048;	

	FILE *device;	    

	if (  argc == 3 
           && (sizeof(argv[1]) / sizeof(char)) > 1
           && (sizeof(argv[2]) / sizeof(char)) > 1
	   && (atoi(argv[2]) % 512) == 0
	   ) {
	   	device = fopen(argv[1], "r");
		if(device == NULL) { 
			printf("\nI got trouble opening the device %s\n", argv[1]);
			exit(EXIT_FAILURE);
		}
		keySize = atoi(argv[2]);		
	}
	else if (  argc == 2 
	        && (sizeof(argv[1]) / sizeof(char)) > 1
		) {
	  	device = fopen(argv[1], "r");
		if(device == NULL) { 
			printf("\nI got trouble opening the device %s\n", argv[1]);
			exit(EXIT_FAILURE);
		}

	}
	else {
		
		printf("\nUsage: \n");
		printf("\nKey Size Provided: \n");
		printf("\n\t\treadkey </path/to/device> <keysize> \n");
		printf("\nDefault key size: %d\n", keySize);
		printf("\n\t\treadkey </path/to/device>\n");
		exit(1);
	}

	int count;

	char *block;

	/* Verify if key is multiple of blocks */
	int numBlocks = 0;
	if (keySize % 512 != 0) {
	   printf("\nSory but key size is not multiple of block size, try again. TA.\n");
	   exit(1);
	}

	/* Seek till the end to get disk size and position to start */
	fseek(device, 0, SEEK_END);

	/* Determine where is the end */
	long endOfDisk = ftell(device);

	/* Make sure we start again */
	rewind(device); // Do I need it ???

	/* Get the required amount minus block size */
	long startFrom = endOfDisk - blockSize - keySize;

	/* Allocate space for bloc */
	block = calloc(keySize, sizeof(char));

	/* Start reading from specified block */
	fseek(device, startFrom, SEEK_SET);
	fread(block, 1, keySize, device);

	/* Do something with the data */
	for(count = 0; count < keySize/*sizeof(block)*/; count++){
		printf("%c", block[count]);
	}

	/* Close file */
	fclose(device);

	/* Make sure freed array is zeroed */
	memset(block, 0, keySize);
	free(block);
}

