#ifndef UTILS_H
#define UTILS_H

#include <stdio.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <unistd.h>
#include <string.h>
#include <stdint.h>
#include <ctype.h>

#define ASCII_CONSTANT (0x80)

void printUtf8Char(uint32_t symbol, FILE* dst);
int16_t *getCurrCode(char *str);

#endif // UTILS_H
