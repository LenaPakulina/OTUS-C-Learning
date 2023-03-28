#include "hash.h"

void hash_to_array(gpointer key, gpointer value, gpointer user_data)
{
	as_hs (array[as_hs (count)]).key = key;
	as_hs (array[as_hs (count)]).value = value;
	as_hs (count)++;
}

void union_hash_func(gpointer key, gpointer value, gpointer user_data)
{
	gpointer val = g_hash_table_lookup ((GHashTable *) user_data, key);

	if (val)
		*((size_t *) val) += *((size_t *) value);
	else
		g_hash_table_insert ((GHashTable *) user_data, key, value);
}

int compare(const void *i, const void *j)
{
	return get_val (j) - get_val (i);
}

key_value_struct *malloc_key_value_struct(GHashTable *hash_table)
{
	int hashSize = g_hash_table_size (hash_table) * sizeof (key_value);
	key_value_struct *key_value_struct_buf =
			malloc (sizeof (key_value_struct) + hashSize);
	key_value_struct_buf->count = 0;
	key_value_struct_buf->array = (key_value *) (key_value_struct_buf + 1);
	return key_value_struct_buf;
}

void print_top_by_value(GHashTable *hash_table, int count)
{
	key_value_struct *key_value_struct_buf = malloc_key_value_struct(hash_table);
	g_hash_table_foreach (hash_table, hash_to_array, key_value_struct_buf);
	qsort (key_value_struct_buf->array,
		   key_value_struct_buf->count,
		   sizeof (key_value),
		   compare);

	for (int i = 0; i < key_value_struct_buf->count && count; i++, count--)
		printf ("%s = %zu\n", (char *) key_value_struct_buf->array[i].key,
				*((size_t *) key_value_struct_buf->array[i].value));

	free (key_value_struct_buf);
}

void union_hash(thread_data *thread_data_array, int count_threads)
{
	for (int i = 0; i < count_threads; i++) {
		thread_data_array[count_threads].byte_count +=
				thread_data_array[i].byte_count;

		g_hash_table_foreach (thread_data_array[i].url_counter,
							  union_hash_func,
							  thread_data_array[count_threads].url_counter);
		g_hash_table_foreach (thread_data_array[i].ref_counter,
							  union_hash_func,
							  thread_data_array[count_threads].ref_counter);
	}
}
