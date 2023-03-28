#include "utils.h"
#include "threaddata.h"
#include "globals.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdarg.h>
#include <dirent.h>
#include <glib.h>
#include <glib/gprintf.h>
#include <pcre.h>
#include <sys/mman.h>
#include <errno.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <unistd.h>
#include <pthread.h>
#include <assert.h>

noreturn void print_error (const char *format, ...)
{
	va_list argptr;
	va_start (argptr, format);
	vprintf (format, argptr);
	va_end (argptr);
	exit (EXIT_FAILURE);
}

noreturn void *thread_print_error (const char *format, ...)
{
	va_list argptr;
	va_start (argptr, format);
	vprintf (format, argptr);
	va_end (argptr);
	return ((void *) EXIT_FAILURE);
}

void ht_insert_dupkey_with_increment(GHashTable *ht, char *value,
									 size_t vol, size_t *malloc_bytes) {
	gpointer val = NULL;
	val = g_hash_table_lookup(ht, value);
	if (val)
		*((size_t *) val) += vol;
	else
	{
		malloc_bytes = malloc (sizeof (size_t));
		*malloc_bytes = vol;
		g_hash_table_insert (ht, g_strdup (value), malloc_bytes);
	}
}

long int parse_log_string(char *str, char *url, char *refer) {
	// 127.0.0.1 - frank [10/Oct/2000:13:55:36 -0700] "GET /apache_pb.gif HTTP/1.0" 200 2326
	// "http://www.example.com/start.html" "Mozilla/4.08 [en] (Win98; I ;Nav)"
	char *save_ptr, *stmp;
	strtok_r(str, "\"", &save_ptr);                // пропустили до "
	strtok_r(NULL, " ", &save_ptr);                // пропустили GET

	unsigned long i = 0;
	for (; i < strlen(save_ptr); i++) {
		if (save_ptr[i] == ' ') {
			break;
		}
	}
	if (i > 1) {
		strncpy(url, save_ptr, i);
	} else {
		return -1;
	}
	strtok_r(NULL, " ", &save_ptr);         // взяли /apache_pb.gif
	if (url == NULL)
		return -1;
	strtok_r(NULL, "\" ", &save_ptr);              // пропустили до "
	strtok_r(NULL, " ", &save_ptr);                // пропустили 200
	long int ddsadd = atol(save_ptr);
	strtok_r(NULL, " ", &save_ptr);              // взяли 2326

	i = 1;
	for (; i < strlen(save_ptr); i++) {
		if (save_ptr[i] == '\"') {
			break;
		}
	}

	if (i > 1) {
		strncpy(refer, save_ptr, i+1);
	} else {
		return -1;

	}
	return ddsadd;
}

void *parse_logs(void *arg)
{
	int num_t = *((int *) arg);
	int fd, i, j, options = 0, length, erroffset;

	char url[SIZE_STR_BUF] = { 0 };
	char referer[SIZE_STR_BUF] = { 0 };
	char bytes_string[SIZE_BYTES_BUF] = { 0 };

	size_t *malloc_bytes = NULL;
	char *str, *end_str, *path;
	void *src;
	struct stat statbuf;
	const char *error;
	printf ("Номер потока %d\n", num_t);

	entry_link file_path = thread_data_array[num_t].file_path;

	for (path = NULL; file_path; file_path = file_path->next) {
		path = file_path->path;
		if ((fd = open (path, O_RDONLY)) < 0) {
			thread_print_error ("невозможно открыть %s для чтения", path);
		}

		errno = 0;

		if (fstat (fd, &statbuf) < 0)
			thread_print_error ("Ошибка вызова функции fstat у файла %s\n", path);

		if ((src = mmap (0, statbuf.st_size, PROT_READ, MAP_SHARED, fd, 0)) == MAP_FAILED)
			thread_print_error ("Ошибка вызова функции mmap для входного файла\n");

		str = src;
		end_str = str + statbuf.st_size;

		for (length = 0; str < end_str; str += (length + 1), length = 0) {
			while (str + length < end_str && *(str + length) != '\n') {
				length++;
			}

			char newStr[length+1];
			strncpy(newStr, str, length);
			newStr[length] = '\0';
			memset(url, NULL, SIZE_STR_BUF);
			memset(referer, NULL, SIZE_STR_BUF);
			int ans = parse_log_string(newStr, url, referer);
			if (ans < 1) {
				continue;
			}

			thread_data_array[num_t].byte_count += ans;
			ht_insert_dupkey_with_increment(thread_data_array[num_t].url_counter,
											url,
											ans,
											malloc_bytes);
			ht_insert_dupkey_with_increment(thread_data_array[num_t].ref_counter,
											referer,
											1,
											malloc_bytes);

			//printf("%s", url);

			continue;
		}

		munmap (src, statbuf.st_size);

		if (close (fd) < 0) {
			thread_print_error("Ошибка вызова close(чтение) для файла %s\n", path);
		}
	}

	return ((void *) EXIT_SUCCESS);

}
