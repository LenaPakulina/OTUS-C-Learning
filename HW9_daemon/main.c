#include <sys/socket.h>
#include <sys/un.h>
#include <stdlib.h>
#include <stdio.h>
#include <errno.h>
#include <string.h>
#include <sys/stat.h>
#include <glib.h>
#include <glib/gprintf.h>
#include <syslog.h>
#include <fcntl.h>
#include "utils.h"
#include "globals.h"
#include "daemonize.h"

char file_name[NAMESIZE] = { 0 };
char socket_name[NAMESIZE] = { 0 };
char lock_file[NAMESIZE] = { 0 };
char ini_file[NAMESIZE] = { 0 };
int sock = 0;

static void sig_hup() {
	syslog (LOG_INFO, "Чтение конфигурационного файла");
	read_conf ();
}

static void sig_quit() {
	syslog (LOG_INFO, "Удаление сокета");
	close (sock);
	unlink (socket_name);
}

int main (int argc, char **argv)
{
	if (argc < 2) {
		/*_Noreturn */
		print_error("Use: app <path To Config-file>.\n");
	}

	int msgsock;
	struct sockaddr_un server;
	char buf[BUFFSIZE];
	struct stat statbuf;
	struct sigaction sa;

	strcpy(ini_file, argv[1]);
	read_conf();

	//запуск демона
	if (!(argc == 3 && strcmp (argv[2], "-test") == 0)) {
		daemonize("daemon_stat");
		if (already_running ()) {
			syslog(LOG_ERR, "демон уже запущен");
			return EXIT_FAILURE;}

		/*устанавливаем блокировку всех сигналов кроме SIGHUB
		   с назначением обработчика сигнала
		 */
		sa.sa_handler = sig_hup;
		sigfillset (&sa.sa_mask);
		sigdelset (&sa.sa_mask, SIGHUP);
		sa.sa_flags = 0;
		if (sigaction (SIGHUP, &sa, NULL) < 0) {
			syslog (LOG_ERR,
					"невозможно перехватить сигнал SIGHUP: %s",
					strerror (errno));
			return EXIT_FAILURE;
		}

		/*устанавливаем блокировку всех сигналов кроме SIGQUIT
		   с назначением обработчика сигнала
		 */
		sa.sa_handler = sig_quit;
		sigfillset (&sa.sa_mask);
		sigdelset (&sa.sa_mask, SIGQUIT);
		sa.sa_flags = 0;
		if (sigaction (SIGQUIT, &sa, NULL) < 0) {
			syslog (LOG_ERR,
					"невозможно перехватить сигнал SIGQUIT: %s",
					strerror (errno));
			return EXIT_FAILURE;
		}
	}

	/*Далее функционаяльная часть программы */
	sock = socket (AF_UNIX, SOCK_STREAM, 0);

	if (sock < 0)
		print_error ("%s\n", "opening stream socket");

	server.sun_family = AF_UNIX;
	strcpy (server.sun_path, socket_name);

	if (bind (sock, (struct sockaddr *) &server, sizeof (struct sockaddr_un))) {
		print_error ("%s\n", "binding stream socket");
	}

	printf("Socket has name %s\n", server.sun_path);
	listen(sock, 5);
	while (1) {
		msgsock = accept (sock, 0, 0);

		if (stat (file_name, &statbuf) < 0)
			snprintf (buf, BUFFSIZE,
					  "Ошибка вызова функции stat:%s у файла %s\n",
					  strerror (errno), file_name);
		else
			snprintf (buf, BUFFSIZE,
					  "Размер файла %s в байтах:%ld\n",
					  file_name, statbuf.st_size);

		send (msgsock, buf, strlen (buf), 0);
		close (msgsock);
	}

	//  return EXIT_SUCCESS;
}
