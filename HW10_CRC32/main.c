#include <stdio.h>
#include <sys/stat.h>
#include <stdlib.h>
#include <fcntl.h>
#include <errno.h>
#include <sys/mman.h>
#include "crc32_utils.h"

int main (int argc, char **argv)
{
	if (argc != 2) {
		printf("Arg Errors. Use: <app> <file>");
		return EXIT_FAILURE;
	}

	struct stat statbuf;
	int fd;
	void *src;

	if ((fd = open(argv[1], O_RDONLY)) < 0) {
		printf("Can't read %s", argv[1]);
		return EXIT_FAILURE;
	}

	if (fstat(fd, &statbuf) < 0) {
		printf("Execution error fstat() %m");
		return EXIT_FAILURE;
	}

	src = mmap(0, statbuf.st_size, PROT_READ, MAP_SHARED, fd, 0);
	if (src== MAP_FAILED) {
		printf("Execution error mmap() %m");
		return EXIT_FAILURE;
	}

	uint32_t num = getCrc32((const void *) src, statbuf.st_size);
	munmap (src, statbuf.st_size);
	printf ("CRC32 = %x\n", num);

	return EXIT_SUCCESS;
}
