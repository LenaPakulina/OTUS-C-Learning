#ifndef GLOBALS_H
#define GLOBALS_H

#include <stdio.h>

#define BUFFSIZE 1024
#define NAMESIZE 200
#define LOCKMODE (S_IRUSR|S_IWUSR|S_IRGRP|S_IROTH)

extern char file_name[NAMESIZE];
extern char socket_name[NAMESIZE];
extern char lock_file[NAMESIZE];
extern char ini_file[NAMESIZE];
extern int sock;

#endif // GLOBALS_H
