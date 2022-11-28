#include <stdio.h>
#include <stdlib.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <unistd.h>
#include <string.h>
#include <stdbool.h>
#include <errno.h>
#include <ctype.h>
#include "customhash.h"

#define BUFFSIZE 4096
#define SIZE_WORD 32

int main(int argc, char *argv[])
{
	if (argc != 2){
		printf("Use arg: app <inpfile>\n");
		return EXIT_FAILURE;
	}

	int fd = open(argv[1], O_RDONLY);
	if (fd < 0){
		printf("Error: Cant open file %s\n", argv[1]);
		return EXIT_FAILURE;
	}

	int currIndPartFile = 0;						// current position of a part of the file
	int currIndWord = 0;							// current position of a char of the word
	int N = -1;										// total count of characters read

	char bufFile[BUFFSIZE * sizeof(char)];			// array of text characters
	char bufWord[SIZE_WORD * sizeof(char)];			// array word

	// Inition CustomHash
	CustomHash myHash = {.table = NULL, .size = 10};
	myHash.table = (CellHash**)calloc(sizeof(CellHash*), myHash.size);
	if (myHash.table == NULL) {
		printf("Error: some word is too long\n");
		close(fd);
		return EXIT_FAILURE;
	}

	while ((N = read(fd, bufFile, BUFFSIZE)) > 0){
		currIndPartFile = 0;

		while (currIndPartFile < N){
			char currChar = bufFile[currIndPartFile];

			if (bufFile[currIndPartFile] == '\n') {
				++currIndPartFile;
				continue;
			}

			if (isspace(currChar)){
				bufWord[currIndWord] = '\0';
				insertKey(bufWord, &myHash);
				currIndWord = 0;
				bufWord[0] = '\0';
			} else {
				bufWord[currIndWord] = bufFile[currIndPartFile];
				if (++currIndWord == SIZE_WORD) {
					printf("Error: some word is too long\n");
					close(fd);
					return EXIT_FAILURE;
				}
			}

			++currIndPartFile;
		}
	}

	close(fd);
	if (N < 0) {
		printf("Error: cant read input file\n");
		return EXIT_FAILURE;
	}

	if (strlen(bufWord)) {
		bufWord[currIndWord] = '\0';
		insertKey(bufWord, &myHash);
	}

	printHash(&myHash);

	// Destroy CustomHash
	for (int i = 0; i < myHash.size; i++){
		free(myHash.table[i]);
	}
	free(myHash.table);

	return 0;
}
