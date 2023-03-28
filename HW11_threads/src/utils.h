#ifndef UTILS_H
#define UTILS_H

#define PWD_LENGTH 200
#define SIZE_STR_BUF 8192
#define SIZE_BYTES_BUF 30

#include <stdlib.h>
#include <stdio.h>
#include <stdnoreturn.h>

noreturn void print_error (const char *format, ...);
noreturn void *thread_print_error (const char *format, ...);
void *parse_logs (void *arg);

#endif // UTILS_H
