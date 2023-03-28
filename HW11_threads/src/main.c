#include <stdio.h>
#include <stdlib.h>
#include <string.h>
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
#include "hash.h"
#include "utils.h"
#include "threaddata.h"

char save_pwd[PWD_LENGTH] = {0};
thread_data *thread_data_array = NULL;

int main (int argc, char **argv)
{
	if (argc != 3) {
		print_error ("Use: app log_dir count_threads.\n");
	}

	/* сохраним текущий каталог для возврата */
	if (getcwd (save_pwd, PWD_LENGTH) == NULL) {
		print_error ("ошибка вызова getcwd");
	}

	unsigned int count_threads = atoi(argv[2]);
	if (count_threads < 1 || count_threads > 50) {
		count_threads = 1;
	}

	thread_data_array = new_thread_data(count_threads);

	DIR *dp;
	if ((dp = opendir(argv[1])) == NULL) {
		print_error("невозможно открыть %s для чтения", argv[1]);
	}

	/*распределение файлов по потокам */
	int threadNum = 0;
	struct dirent *dirp;
	while ((dirp = readdir (dp)) != NULL) {
		if (strcmp (dirp->d_name, ".") == 0 ||
				strcmp (dirp->d_name, "..") == 0) {
			continue;
		}

		add_file_path_thread_data(thread_data_array,
								   threadNum++,
								   count_threads,
								   dirp->d_name);
	}

	closedir(dp);

	/*work threads */
	/* сменим каталог на каталог с логами, чтобы потоки нашли свои файлы */
	if (chdir (argv[1]) < 0) {
		print_error("ошибка вызова функцииsave_pwd для каталога %s\n", argv[1]);
	}

	for (unsigned int i = 0; i < count_threads; i++) {
		int ans = pthread_create (&thread_data_array[i].thread_id,
								NULL,
								parse_logs,
								(void *) &thread_data_array[i].num);
		if (ans != 0) {
			print_error ("невозможно создать поток %d\n",  i);
		}
	}


	void *tret;
	for (unsigned int i = 0; i < count_threads; i++) {
		if (pthread_join (thread_data_array[i].thread_id, &tret) != 0)  {
			print_error("невозможно присоединить поток %d", i);
		}
	}

	/* вернем каталог в прежнее положение */
	if (chdir (save_pwd) < 0) {
		print_error("ошибка вызова функции chdir для каталога %s\n", save_pwd);
	}

	/* объединяем все данные в последнуюю структуру выводим данные и очищаем все */
	union_hash(thread_data_array, count_threads);

	printf("общее количество отданных байт = %zu\n", thread_data_array[count_threads].byte_count);

	printf("10 самых “тяжёлых” по суммарному трафику URL’ов:\n");
	print_top_by_value(thread_data_array[count_threads].url_counter, 10);

	printf("10 наиболее часто встречающихся Referer’ов:\n");
	print_top_by_value (thread_data_array[count_threads].ref_counter, 10);

	free_thread_data (thread_data_array, count_threads);

	return EXIT_SUCCESS;

}

