#ifndef UTILS_H
#define UTILS_H

void print_error(const char *format, ...);
void read_conf(void);

int lockfile(int fd);
int already_running(void);

#endif // UTILS_H
