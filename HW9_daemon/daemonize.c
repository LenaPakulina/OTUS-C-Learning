#include "daemonize.h"
#include "globals.h"
#include <stdlib.h>
#include <errno.h>
#include <sys/stat.h>
#include <glib.h>
#include <syslog.h>
#include <sys/resource.h>
#include <fcntl.h>

bool daemonize(const char *cmd)
{
	// Сбросить маску режима создания файла.
	umask(0);

	// перестаем быть лидером группы, reparanting to init
	pid_t pid;
	if ((pid = fork()) < 0) {
		perror("ошибка вызова функции fork");
		return false;
	}
	if (pid != 0) { // родительский процесс
		_exit(0);  // use _exit() to avoid triggering atexit() processing
	}

	/* cтановимся лидером сессии: so ^C doesn't whack us. */
	if (setsid() == (pid_t)-1) {
		perror("ошибка вызова функции setsid");
		return false;
	}

	// инициализировать файл журнала, для дальнейшего вывода ошибок
	openlog(cmd, LOG_CONS, LOG_DAEMON);
	//закрыть все открытые файловые дескрипторы.
	struct rlimit rl; // Получить максимально возможный номер дескриптора файла.
	if (getrlimit(RLIMIT_NOFILE, &rl) < 0) {
		perror("невозможно получить максимальный номер дескриптора");
		return false;
	}
	if (rl.rlim_max == RLIM_INFINITY) {
		rl.rlim_max = 1024;
	}
	for (size_t i = 0; i < rl.rlim_max; i++) {
		close(i);
	}

	// Присоединить файловые дескрипторы 0, 1 и 2 к /dev/null.
	int fd0 = open("/dev/null", O_RDWR);
	int fd1 = dup(0);
	int fd2 = dup(0);
	if (fd0 != 0 || fd1 != 1 || fd2 != 2) {
		syslog(LOG_CRIT, "ошибочные файловые дескрипторы %d %d %d", fd0, fd1, fd2);
		return false;
	}

	// Обеспечить невозможность обретения управляющего терминала в будущем.
	struct sigaction sa;
	sa.sa_handler = SIG_IGN;
	sigemptyset(&sa.sa_mask);
	sa.sa_flags = 0;
	if (sigaction(SIGHUP, &sa, NULL) < 0) {
		syslog(LOG_CRIT, "невозможно игнорировать сигнал SIGHUP");
		return false;
	}

	// Перестаем быть лидером сессии
	if ((pid = fork()) < 0) {
		syslog(LOG_CRIT, "ошибка вызова функции fork");
		return false;
	} else if (pid != 0) { /* родительский процесс */
		_exit(0);
	}
	return true;
}
