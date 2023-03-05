#include <inttypes.h>
#include <stdio.h>
#include <stdlib.h>

#define MAIN_TYPE int64_t
#define EMPTY_STR ("")

MAIN_TYPE data[] = {4, 8, 15, 16, 23, 42};
MAIN_TYPE data_length = sizeof(data) / sizeof(data[0]);

void print_int(MAIN_TYPE *v) {
	fprintf(stdout, "%ld ", *v);
	fflush(stdout);
}

MAIN_TYPE p(MAIN_TYPE v) {
	return v & 1;
}

// Можно сделать через указатели
MAIN_TYPE *add_element(MAIN_TYPE *prev, MAIN_TYPE value) {
	MAIN_TYPE *ret = (MAIN_TYPE *)malloc(sizeof(MAIN_TYPE));
	if (!ret) {
		return NULL;
	}
	ret[1] = prev ? prev : NULL;
	ret[0] = value;

	return ret;
}

// Aналог функции std::for_each
void m(MAIN_TYPE *p, void (*func)(MAIN_TYPE *)) {
	if (!p) {
		return;
	}

	MAIN_TYPE *prev = (void *)(p[1]);
	func(p);
	m(prev, func);
}

// Aналог функции std::copy_if
MAIN_TYPE *f(MAIN_TYPE *p, MAIN_TYPE *op, MAIN_TYPE (*func)(MAIN_TYPE)) {
	if (!p) {
		return op;
	}
	if (func(p[0])) {
		op = add_element(op, p[0]);
	}
	return f((MAIN_TYPE*) p[1], op, func);
}

void free_p(MAIN_TYPE *p) {
	free(p);
}

int main() {
	/*
	 * Оригинал, сборка:
	 * nasm -f elf64 file.asm
	 * gcc -no-pie -o file file.o
	 *
	 * Вывод:
	 * 4 8 15 16 23 42
		 23 15
	 */

	MAIN_TYPE *s1 = NULL;
	MAIN_TYPE *s2 = NULL;
	MAIN_TYPE len = data_length;

	// Действия с односвязным списком

	while (len) {
		len = len - 1;
		s1 = add_element(s1, data[len]);
	}

	m(s1, print_int);
	puts(EMPTY_STR);

	s2 = f(s1, s2, p); // S2 от s1 по р(...)

	m(s2, print_int);
	puts(EMPTY_STR);

	m(s1, free_p); // Этого должно быть достаточно
	m(s2, free_p);

	// В ячейкай хранится объект из 2х элементов, но обьектов N кол-во,
	// удаление будет N раз

	// Output copy:
	// 4 8 15 16 23 42
	// 23 15

	return 0;
}
