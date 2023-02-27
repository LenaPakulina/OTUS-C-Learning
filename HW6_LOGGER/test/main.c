#include <stdlib.h>
#include <string.h>
#include <threads.h>
#include <errno.h>
#include "pthread.h"
#include "../library/sLogger.h"

#define _unused_ __attribute__((unused))

pthread_barrier_t bar;

int print_msg(void *msg) {
	pthread_barrier_wait(&bar);
	sl_info_sync("%s", (const char *)msg);
	return 0;
}

int main() {
	const char *arrayMsgs[] = {"Msg1!",	"Msg2!", "Msg3!",	 "Msg4!"};
	int sz = sizeof(arrayMsgs) / sizeof(arrayMsgs[0]);
	thrd_t *threads = (thrd_t *)calloc(sz, sizeof(thrd_t));
	if (!threads) {
		sl_error_sync("Ошибка выделения памяти");
		return EXIT_FAILURE;
	}

	init_fstream(stdout);
	on_exit(clear_src, 0);

	if (pthread_barrier_init(&bar, 0, sz) != 0) {
		sl_error_sync("%s", strerror(errno));
		return EXIT_FAILURE;
	}

	set_verbosility(SFL_ALL & ~SFL_DEBUG);
	sl_debug_sync("Проверка на отключение сообщения типа Дебаг");
	sl_info_sync("Информационное сообщение");
	for (int i = 0; i < sz; i++) {
		thrd_create(&threads[i], print_msg, (void *)arrayMsgs[i]);
	}
	for (int i = 0; i < sz; i++) {
		thrd_join(threads[i], 0);
	}

	free(threads);
	pthread_barrier_destroy(&bar);
	sl_warn_sync("Предупреждающее сообщение");

	if (!init_file_log("./test_log.txt", 0)) {
		sl_error_sync("Не удалось записать в текстовый файл");
	} else {
		sl_info_sync("%s", "Запись в текстовый файл.");
	}

	sl_error_sync("Конец.");

	return 0;
}
