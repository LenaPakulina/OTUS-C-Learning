#include <stdio.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <unistd.h>
#include <string.h>
#include <stdint.h>
#include <ctype.h>
#include "utils.h"

int main(int argc, char* argv[])
{
	if (argc != 4) {
		printf("Use arg: app <inpfile> <strEncoding> <outfile>\n");
		return 1;
	}

	char* src_file = argv[1];
	char* dst_file = argv[3];
	int16_t *currCode = getCurrCode(argv[2]);

	if (currCode == NULL) {
		printf("Encoding options: \"cp1251\", \"koi8\", \"iso8859\"\n");
		return 1;
	}

	FILE* src = fopen(src_file, "rb");
	if (src == NULL) {
		printf("Error: Failed to open file %s\n", src_file);
		return 1;
	}
	FILE* dst = fopen(dst_file, "wb");
	if (dst == NULL) {
		printf("Error: Failed to open file %s\n", dst_file);
		fclose(src);
		return 1;
	}

	while (!feof(src)) {
		int16_t chr = fgetc(src);
		if (chr < ASCII_CONSTANT) {
			fputc((unsigned char) chr, dst);
			continue;
		}
		int16_t symbol = currCode[chr];
		printUtf8Char(symbol, dst);
	}

	fclose(dst);
	fclose(src);

	return 0;
}
