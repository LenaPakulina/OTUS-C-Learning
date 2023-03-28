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

void *parse_logs(void *arg)
{
	int num_t = *((int *) arg);
	int fd, i, j, options = 0, length, erroffset;

	char url[SIZE_STR_BUF] = { 0 };
	char referer[SIZE_STR_BUF] = { 0 };
	char bytes_string[SIZE_BYTES_BUF] = { 0 };

	size_t bytes = 0;
	size_t *malloc_bytes = NULL;
	char *str, *end_str, *path;
	void *src;
	struct stat statbuf;
	int ovector[30];
	pcre *re;
	const char *error;
	printf ("Номер потока %d\n", num_t);

	char *pattern = "^[^\"]+[^\\s]+\\s([^\\s]+)[^\"]+\"\\s[\\d]+\\s([\\d]+)\\s\"([^\"]+).*$";
	re = pcre_compile (pattern, options, &error, &erroffset, NULL);

	if (!re) {
		thread_print_error ("Failed pcre_compile in thread %d\n", num_t);
	}

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

			if (pcre_exec (re, NULL, str, length, 0, 0, ovector, 30) == -1) {
				continue;
			}

			for (i = 0, j = ovector[2]; j < ovector[3]; i++, j++)
				url[i] = *(str + j);
			url[i] = '\0';

			assert (i <= SIZE_STR_BUF);

			for (i = 0, j = ovector[4]; j < ovector[5]; i++, j++)
				bytes_string[i] = *(str + j);
			bytes_string[i] = '\0';

			assert (i <= SIZE_BYTES_BUF);

			for (i = 0, j = ovector[6]; j < ovector[7]; i++, j++)
				referer[i] = *(str + j);
			referer[i] = '\0';

			assert (i <= SIZE_STR_BUF);

			bytes = atol(bytes_string);
			thread_data_array[num_t].byte_count += bytes;
			ht_insert_dupkey_with_increment(thread_data_array[num_t].url_counter, url,
											bytes, malloc_bytes);
			ht_insert_dupkey_with_increment(thread_data_array[num_t].ref_counter, referer,
											1, malloc_bytes);
		}

		munmap (src, statbuf.st_size);

		if (close (fd) < 0) {
			thread_print_error("Ошибка вызова close(чтение) для файла %s\n", path);
		}
	}

	pcre_free (re);

	return ((void *) EXIT_SUCCESS);

}
