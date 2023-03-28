#include "threaddata.h"

thread_data *new_thread_data (int count_threads)
{
	/* выделяем +1 структуру (последняя для общих данных) для объединения*/
	thread_data *thread_data_array =
			(thread_data *) malloc ((count_threads + 1) * sizeof (thread_data));

	/*инициализация данных для потоков */
	for (int i = 0; i <= count_threads; i++) {
		thread_data_array[i].file_path = NULL;
		thread_data_array[i].byte_count = 0;
		thread_data_array[i].num = i;

		if (i != count_threads) {
			thread_data_array[i].url_counter =
					g_hash_table_new_full (g_str_hash, g_str_equal, g_free, g_free);
			thread_data_array[i].ref_counter =
					g_hash_table_new_full (g_str_hash, g_str_equal, g_free, g_free);
		} else {
			thread_data_array[i].url_counter =
					g_hash_table_new (g_str_hash, g_str_equal);
			thread_data_array[i].ref_counter =
					g_hash_table_new (g_str_hash, g_str_equal);
		}
	}

	return thread_data_array;
}

void free_thread_data (thread_data * thread_data_array, int count_threads)
{
	/*очистка данных по потокам*/
	entry_link temp, next;
	for (int i = 0; i <= count_threads; i++) {
		for (temp = thread_data_array[i].file_path, next = NULL; temp != NULL; temp = next) {
			next = temp->next;
			free (temp);
		}

	  g_hash_table_destroy (thread_data_array[i].url_counter);
	  g_hash_table_destroy (thread_data_array[i].ref_counter);
	}

	free (thread_data_array);
}

entry *new_entry (char *str)
{
	entry *e = (entry *) malloc (sizeof (entry) + strlen (str) + 1);

	if (e == NULL)
		print_error
				("%s\n",
				 "Не удалось выделить память под элемент таблицы");

	e->path = (char *) (e + 1);
	e->next = NULL;
	strcpy (e->path, str);

	return e;
}

void add_file_path_thread_data (thread_data * thread_data_array, int count,
			   int count_threads, char *file_path_name)
{
	int index = count % count_threads;
	entry *file_path_old = thread_data_array[index].file_path;
	thread_data_array[index].file_path = new_entry (file_path_name);
	thread_data_array[index].file_path->next = file_path_old;
}
