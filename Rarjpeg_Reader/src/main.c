#include <stdio.h>
#include <string.h>
#include <sys/stat.h>
#include <stdbool.h>
#include "zipinfo.h"
#include "utils.h"

int main(int argc, char** argv)
{
	if (argc <= 1) {
		printf("Use arg: app <filePath to Rarjpeg>\n");
		return 1;
	}

	for (int i = 1; i < argc; i++) {

		char *filePath = argv[i];
		printf("File path: %s\n", filePath);

		FILE *file = getFILE(filePath);
		if (file == NULL) {
			printf("Can't open file\n");
			continue;
		}

		struct stat cstat;
		if (stat(filePath, &cstat) != 0) {
			fclose(file);
			printf("Could not get information on the file\n");
			continue;
		}

		_off_t fileSize = (_off_t) cstat.st_size;

		if (!isRarjpegFile(file, fileSize)) {
			printf("File is non-Rarjpeg\n");
			fclose(file);
			continue;
		}

		printCont(file, fileSize);
		fclose(file);

	}

	return 0;
}
