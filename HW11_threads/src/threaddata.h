#ifndef THREADDATA_H
#define THREADDATA_H

#include <glib.h>
#include <glib/gprintf.h>

typedef struct entry *entry_link;

typedef struct entry
{
	char *path;
	entry_link next;
} entry;

typedef struct thread_data
{
	entry_link file_path;
	size_t byte_count;
	pthread_t thread_id;
	int num;
	GHashTable *url_counter;
	GHashTable *ref_counter;
} thread_data;

thread_data *new_thread_data (int count_threads);
void free_thread_data (thread_data * thread_data_array, int count_threads);
entry *new_entry (char *str);
void add_file_path_thread_data (thread_data * thread_data_array, int count,
			   int count_threads, char *file_path_name);

#endif // THREADDATA_H
