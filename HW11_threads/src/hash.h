#ifndef HASH_H
#define HASH_H

#include "utils.h"
#include "threaddata.h"

#define as_hs(A) ((key_value_struct *) user_data)->A
#define get_val(A) *((size_t *)((key_value *) A)->value)

typedef struct key_value
{
	gpointer key;
	gpointer value;
} key_value;

typedef struct key_value_struct
{
	int count;
	key_value *array;
} key_value_struct;

void hash_to_array (gpointer key, gpointer value, gpointer user_data);
void union_hash_func (gpointer key, gpointer value, gpointer user_data);
int compare (const void *i, const void *j);
key_value_struct * malloc_key_value_struct (GHashTable * hash_table);
void print_top_by_value (GHashTable * hash_table, int count);
void union_hash (thread_data * thread_data_array, int count_threads);

#endif // HASH_H
