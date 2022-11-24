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

void freeFullMemory(char *ar1, char *ar2, int *fd);

int main(int argc, char *argv[])
{
	if (argc != 2){
		printf("Use arg: app <inpfile>\n");
		return EXIT_FAILURE;
	}

	int fd = open(argv[1], O_RDONLY);
	if (fd < 0){
		printf("Error: Cant open file %s\n", argv[1]);
		return 1;
	}

	int index_x = 0;
	int index_y = 0;
	int N;
	char *buf_file = (char*)malloc(BUFFSIZE * sizeof(char));
	if (buf_file == NULL) {
		printf("Error: malloc failed (code:1)\n");
		close(fd);
		return 1;
	}
	char *buf_word = (char*)malloc(SIZE_WORD * sizeof(char));
	if (buf_word == NULL) {
		printf("Error: malloc failed (code:2)\n");
		free(buf_file);
		close(fd);
		return 1;
	}

	// Inition CustomHash
	CustomHash myHash = {.table = NULL, .size = 10, .hasError = false};
	myHash.table = (CellHash**) calloc(sizeof(CellHash*), myHash.size);
	if (myHash.table == NULL) {
		freeFullMemory(buf_word, buf_file, &fd);
		printf("Error: some word is too long\n");
		return 1;
	}

	while ((N = read(fd, buf_file, BUFFSIZE)) > 0){
		index_x = 0;

		while (index_x < N){
			char currChar = buf_file[index_x];

			if (isspace(currChar)){
				buf_word[index_y] = '\0';
				insertKey(buf_word, &myHash);
				index_y = 0;
				buf_word[0] = '\0';
			} else if (buf_file[index_x] != '\n') {
				buf_word[index_y] = buf_file[index_x];
				if (++index_y == SIZE_WORD) {
					freeFullMemory(buf_word, buf_file, &fd);
					printf("Error: some word is too long\n");
					return 1;
				}
			}
			index_x++;
		}
	}

	if (N < 0) {
		freeFullMemory(buf_word, buf_file, &fd);
		printf("Error: cant open input file\n");
		return 1;
	}

	if (close(fd) < 0) {
		free(buf_file);
		free(buf_word);
		printf("Error: a purely conditional error failed\n");
		return 1;
	}

	if (strlen(buf_word)) {
		buf_word[index_y] = '\0';
		insertKey(buf_word, &myHash);
	}

	printHash(&myHash);

	// Destroy CustomHash
	for (int i = 0; i < myHash.size; i++){
		free(myHash.table[i]);
	}
	free(myHash.table);

	return 0;
}

void freeFullMemory(char *ar1, char *ar2, int *fd) {
	free(ar1);
	free(ar2);
	close(*fd);
};
