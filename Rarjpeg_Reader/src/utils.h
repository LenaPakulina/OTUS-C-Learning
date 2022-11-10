#ifndef UTILS_H
#define UTILS_H

#include <stdio.h>
#include <stdbool.h>

FILE*		getFILE(char *filePath);
bool		isRarjpegFile(FILE *file, _off_t fileSize);
void		printCont(FILE *file, _off_t fileSize);

#endif // UTILS_H
