#include "utils.h"
#include <stdio.h>
#include <string.h>
#include <sys/stat.h>
#include <stdbool.h>
#include "zipinfo.h"

FILE *getFILE(char *filePath) {
	FILE *file;
	file = fopen(filePath, "rb");
	return file;
}

bool isRarjpegFile(FILE *file, _off_t fileSize) {
	for (long i = 0; i <= UINT16_MAX; ++i) {
		if (fseek(file, fileSize - i - (long)sizeof(struct EOCD), SEEK_SET) != 0) {
			continue;
		}

		struct EOCD ecdr;
		if (fread(&ecdr, sizeof(ecdr), 1, file) != 1) {
			continue;
		}

		if (ecdr.signature == CENTRAL_DIRECTORY_EOCD) {
			return true;
		}
	}

	return false;
}

void printCont(FILE *file, _off_t fileSize) {
	for (long i = 0; i < fileSize; ++i) {
		fseek(file, i, SEEK_SET);
		struct CentralDirectoryFileHeader fileHeader;

		if (fread(&fileHeader, sizeof(fileHeader), 1, file) != 1) {
			continue;
		}

		if (fileHeader.signature == CENTRAL_DIRECTORY_ENTRY_SIGNATURE) {
			char buffer[fileHeader.filenameLength + 1];
			fseek(file, i + (long)sizeof(struct CentralDirectoryFileHeader), SEEK_SET);
			fread(buffer, fileHeader.filenameLength, 1, file);
			buffer[fileHeader.filenameLength] = '\0';
			printf("file name: %s\n", buffer);
		}
	}
}
