#include "customhash.h"

void printHash(CustomHash *hash)
{
	for (int i = 0; i < hash->size; i++) {
		if (hash->table[i]) {
			printf("Word: \"%s\", Count: %u\n", hash->table[i]->key, hash->table[i]->value);
		}
	}
}

void closeApp(char *str)
{
	printf("Error: %s\n", str);
	exit(EXIT_FAILURE);
}

CellHash *createCellHash(char *str)
{
	CellHash *e = (CellHash*)malloc(sizeof(CellHash) + strlen(str) + 1);
	if (e == NULL) {
		closeApp("Error: cant malloc memory for the CellHash");
	}

	e->key = (char *) e + sizeof(CellHash);
	e->value = 1;
	strcpy(e->key, str);

	return e;
}

int hashCode(char *str, CustomHash *hash)
{
	// https://learnc.info/adt/hashmap.html
	unsigned long long thash = 5381;
	int c;

	if (hash->size == 1) {
		closeApp("Error: size of the hash is more that 1");
	}

	while ((c = (unsigned char)*str++)) {
		thash = ((thash << 5) + thash) ^ c;
	}
	return thash % hash->size;
}

void insertKey(char *str, CustomHash *hash)
{
	if (strlen(str) == 0) {
		return;
	}

	int index = hashCode(str, hash);

	int i = index;
	bool success = false;
	int red_line = hash->size;

	while (i < red_line) {
		if (hash->table[i]) {
			if (strcmp(hash->table[i]->key, str) == 0){
				hash->table[i]->value++;
				success = true;
				break;
			}
		} else {
			hash->table[i] = createCellHash(str);
			success = true;
			break;
		}

		if (++i == hash->size) {
			i = 0;
			red_line = index;
		}
	}

	if (!success) {
		rebaseKey(str, hash);
	}
}

void rebaseKey(char *str, CustomHash *hash)
{
	// let's increase the size of the hash table by 2 unit
	int new_size_hash_table = hash->size << 1;

	CellHash **new_hash_table = (CellHash**) calloc(sizeof(CellHash*), new_size_hash_table);

	if (new_hash_table == NULL) {
		closeApp("Error: cant malloc memory for the CellHash");
	}

	int	i=0, j=0, index=0, red_line=0, n = hash->size;
	hash->size = new_size_hash_table;

	while (i < n){

		index = hashCode(hash->table[i]->key, hash);
		red_line = new_size_hash_table;
		j = index;

		while (j < red_line){

			if (!new_hash_table[j]){
				new_hash_table[j] = hash->table[i];
				break;
			}

			if (++j == new_size_hash_table){
				j = 0;
				red_line = index;
			}
		}
		i++;
	}

	free(hash->table[i]);
	hash->table = new_hash_table;
	insertKey(str, hash);
}
