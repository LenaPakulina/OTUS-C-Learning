#include "sLogger.h"
#include <execinfo.h>
#include <threads.h>
#include <time.h>

static FILE *fs = 0;
static int verbose = SFL_ALL;
static bool need_to_close_fs = false; // закрываем файл, только если сами открыли в init
static mtx_t mutex;

bool init_file_log(const char *path, bool need_create_log) {
	if (fs && need_to_close_fs) {
		fclose(fs);
		need_to_close_fs = false;
		return fs;
	}
	if (!fs) {
		mtx_init(&mutex, mtx_plain);
	}
	fs = fopen(path, need_create_log ? "w" : "a");
	need_to_close_fs = true;
	return fs;
}

void init_fstream(FILE *fstream) {
	if (!fstream) {
		return;
	}
	if (fs && need_to_close_fs) {
		fclose(fs);
	}
	if (!fs) {
		mtx_init(&mutex, mtx_plain);
	}
	fs = fstream;
	need_to_close_fs = false;
}

void clear_src() {
	if (!fs) {
		return;
	}
	mtx_destroy(&mutex);
	if (need_to_close_fs) {
		fclose(fs);
	} else {
		fflush(fs);
	}
	fs = 0;
}

void set_verbosility(int level_flags) {
	verbose = level_flags;
}

void print_backtrace() {
	void *traces[SFLOG_BACKTRACE_SIZE];
	int sz = backtrace(traces, SFLOG_BACKTRACE_SIZE);
	if (sz > 1) { // не выводим текущую функцию, но весь стек выше
		fprintf(fs, "Stack backtrace:\n");
		fflush(fs);
		backtrace_symbols_fd(&traces[1], sz - 1, fileno(fs));
	}
}

static void print_prefix(const char *level_name) {
	struct tm tmbuf;
	time_t timer = time(0);
	struct tm *ptm = gmtime_r(&timer, &tmbuf);
	struct timespec sp;
	timespec_get(&sp, TIME_UTC);
	fprintf(fs, "[%s - %02d:%02d:%d - %2ld:%02ld:%02ld.%03ld] ", level_name, ptm->tm_mday,
					ptm->tm_mon + 1, (ptm->tm_year + 1900) % 100, (sp.tv_sec / 3600) % 24,
					(sp.tv_sec / 60) % 60, sp.tv_sec % 60, sp.tv_nsec / 1000000);
}

void print_level_msg(int level, const char *level_name, const char *format, ...)
{
	if (!((verbose & level) && fs)) {
		return;
	}
	va_list args;
	va_start(args, format);
	mtx_lock(&mutex);
	print_prefix(level_name);
	vfprintf(fs, format, args);
	fprintf(fs, "\n");
	mtx_unlock(&mutex);
	fflush(fs);
	va_end(args);

	if (level == SFL_ERROR) {
		print_backtrace();
	}
}
