#include <stdio.h>
#include <stdlib.h>
#include <string.h>

int main(int argc, char *argv[])
{
	int blockSize = 512;
	int keySize = 2048;

	int count;

	unsigned char *block;
        
	/*
	    Thing to always remember that argv starts from 0 - the name of the program, and argc starts from 1 i.e. 1 is the name of the program.
	*/
	if ( argc == 3 
	   && strcmp(argv[1], "genwrite") != 0
	   && (sizeof(argv[2]) / sizeof(char)) > 2
	   ) {
		char ch;
		FILE *keyF;
		keyF = fopen(argv[1], "r");
		if (keyF == NULL) exit(EXIT_FAILURE);

		/* Tell key Size */
		fseek(keyF, 0, SEEK_END);
		keySize = ftell(keyF);
		rewind(keyF);
		printf("\nKey Size: %d\n", keySize);
		
		block = calloc(keySize, sizeof(char));
		printf("\n-- Start Key --:\n");
                for(count = 0; count < keySize/*sizeof(block)*/; count++){
			char ch = fgetc(keyF);
                        block[count] = ch;
			/*
			  Uncomment below to see your key on screen
			*/
			// printf("%c",ch);
			
                }
		printf("\n-- End Key --:\n");
		fclose(keyF);
	}
	else if (  argc == 3 
		&& strcmp(argv[1], "genwrite") == 0 
		&& (sizeof(argv[2]) / sizeof(char)) > 2
		) 
		{
		printf("\n-- Attempting to create random key(ish --) of size: %d\n", keySize);
		block = calloc(keySize, sizeof(char));
		int count;
		for(count = 0; count < keySize/*sizeof(block)*/; count++){
	 		block[count] = (char) rand();
		}
		FILE *tmpfile;
		tmpfile = fopen(".tmpckey", "w");
		if(tmpfile == NULL) exit(EXIT_FAILURE);
		fwrite(block, 1, keySize, tmpfile);
		fclose(tmpfile);
		chmod(".tmpckey", 0600);
	}
	else if (  argc == 4 
		&& strcmp(argv[1], "genwrite") == 0
		&& (sizeof(argv[2]) / sizeof(char)) > 2
		&& ((atoi(argv[3]) % 512) == 0)
		) 
		{
		keySize = atoi(argv[3]);
		printf("\n-- Attempting to create random key(ish --) of size: %d\n", keySize);
		block = calloc(keySize, sizeof(char));
		int count;
		for(count = 0; count < keySize/*sizeof(block)*/; count++){
			block[count] = (char) rand();
		}
		FILE *tmpfile;
		tmpfile = fopen(".tmpckey", "w");
		if(tmpfile == NULL) exit(EXIT_FAILURE);
		fwrite(block, 1, keySize, tmpfile);
		fclose(tmpfile);
		chmod(".tmpckey", 0600);
	}	
	else {
		printf("\n");
		printf("################################################################################\n");
		printf("#                                                                              #\n");
		printf("#                              Usage:                                          #\n");
		printf("#                                                                              #\n");
		printf("################################################################################\n");
		printf("#> To write existing key to device:                                            #\n");
		printf("#                                                                              #\n");
		printf("#     writekey </path/to/keyfile> </path/to/removable/sd*>                     #\n");
		printf("#                                                                              #\n");
		printf("#> To generate and write pseudo random key,                                    #\n");
		printf("#> key will be saved to temporary file .tmpckey                                #\n");
		printf("#                                                                              #\n");
		printf("#     writekey genwrite </path/to/removable/sd*> <keysize in multiples of 512> #\n");
		printf("#                                                                              #\n");
		printf("#> When keysize is not provided default size is set to %d.                     #\n", keySize);
		printf("#                                                                              #\n");
		printf("################################################################################\n");
		exit(1);
	}
	
	/*
	    Some printf debugging below, uncomment when needed to see what is going on.
	*/
	/*
	printf("\nNumber of Args: %d\n", argc);
	printf("\nCurrently block array contains: \n");
	for(count = 0; count < keySize; count++){
		printf("%c", block[count]);
	}
	printf("\n-- End block -- \n");
	*/
	/* Open Device itp... */
	FILE *device = fopen(argv[2], "w");
	if(device == NULL) exit(EXIT_FAILURE);

	printf("\nDevice to write: %s\n", argv[2]);

	fseek(device, 0, SEEK_END);
	
	/* Determine where is the end */
	long endOfDisk = ftell(device);
	printf("\nDevice Size: %ld\n", endOfDisk);

	/* Verify if key is multiple of blocks */
	int numBlocks = 0;
	if (keySize % 512 != 0 || endOfDisk < (blockSize + keySize) ) {
	        printf("\nSorry but key size is not multiple of block size or device you trying to write to is too small, try again. TA.\n");
		fclose(device);
	        exit(1);
	}



	/* Make sure we start again */
	rewind(device);

	/* Get the required amount sunbstracting block size */
	long startFrom = endOfDisk - blockSize - keySize;

	/* Write some data to the disk */
	printf("\nWriting data starting from: %ld\n", startFrom);
	fseek(device, startFrom, SEEK_SET);
	fwrite(block, 1, keySize, device);
	printf("\nBlock Position after data write procedure : %ld\n", ftell(device));

	/*
		Below is just for convenience, to read what was written,
		can aid in debugging hence left commented for later.
	*/
	/*
	printf("\nAmount of Data written : %ld\n", ftell(device) - startFrom);

	// Start reading from specified block 
	printf("\n>>>>>>>> DEBUGGING SECTION <<<<<<<<<\n");
	rewind(device); //
	fseek(device, startFrom, SEEK_SET);
	printf("\nBlock Position before read attempted: %d\n", ftell(device));
	printf("\nKey size: %d\n", keySize);
	fread(block, 1, keySize, device);

	// Do something with the data
	printf("\nBlock Position startFrom: %ld\n", startFrom);
	printf("\nBlock Position after read: %d\n", ftell(device));
	printf("\n-- Buffer Read: --\n");
	for(count = 0; count < keySize; count++){
		printf("%c", block[count]);
	}
	printf("\n-- End block -- \n");
	printf("\n--  -- \n");
	printf("\n--  -- \n");
	*/

	/* Close file */
	fclose(device);

	/* Make sure freed array is zeroed */
	memset(block, 0, keySize);
	free(block);

/* Return success, might change it to be useful return not place holder */
return 0;
}
