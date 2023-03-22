#include "utils.h"
#include "globals.h"
#include <stdlib.h>
#include <errno.h>
#include <sys/stat.h>
#include <glib.h>
#include <syslog.h>
#include <sys/resource.h>
#include <fcntl.h>

void print_error(const char *format, ...)
{
	va_list argptr;
	va_start (argptr, format);
	vprintf (format, argptr);
	va_end (argptr);
	exit (EXIT_FAILURE);
}

void read_conf(void)
{
	g_autoptr (GError) error = NULL;
	g_autoptr (GKeyFile) key_file = g_key_file_new ();

	if (!g_key_file_load_from_file(key_file, ini_file, G_KEY_FILE_NONE, &error)) {
		if (!g_error_matches(error, G_FILE_ERROR, G_FILE_ERROR_NOENT)) {
			g_warning ("Error loading key file: %s", error->message);
		}
		return;
	}

	g_autofree gchar *g_file_name =
			g_key_file_get_string(key_file, "First Group", "FileName", &error);
	if (g_file_name == NULL && !g_error_matches(error, G_KEY_FILE_ERROR,
				 G_KEY_FILE_ERROR_KEY_NOT_FOUND)) {
		g_warning ("Error finding key in key file: %s", error->message);
		return;
	} else if (g_file_name == NULL) {
		// Fall back to a default value.
		g_file_name = g_strdup ("default-value");
	}

	strcpy (file_name, (char *) g_file_name);

	g_autofree gchar *g_socket_name =
			g_key_file_get_string (key_file, "First Group", "SocketName", &error);
	if (g_socket_name == NULL && !g_error_matches (error, G_KEY_FILE_ERROR,
				 G_KEY_FILE_ERROR_KEY_NOT_FOUND)) {
		g_warning ("Error finding key in key file: %s", error->message);
		return;
	} else if (g_socket_name == NULL) {
		// Fall back to a default value.
		g_socket_name = g_strdup ("default-value");
	}

	strcpy (socket_name, (char *) g_socket_name);

	g_autofree gchar *g_lock_file =
			g_key_file_get_string (key_file, "First Group", "LockFile", &error);
	if (g_lock_file == NULL && !g_error_matches (error, G_KEY_FILE_ERROR,
				 G_KEY_FILE_ERROR_KEY_NOT_FOUND)) {
		g_warning ("Error finding key in key file: %s", error->message);
		return;
	} else if (g_lock_file == NULL) {
		// Fall back to a default value.
		g_lock_file = g_strdup ("default-value");
	}

	strcpy (lock_file, (char *) g_lock_file);
}

int lockfile(int fd) {
	struct flock fl;
	fl.l_type = F_WRLCK;
	fl.l_start = 0;
	fl.l_whence = SEEK_SET;
	fl.l_len = 0;
	return (fcntl (fd, F_SETLK, &fl));
}

int already_running(void) {
	//проверка на дубль демона

	int fd;
	char buf[16];
	fd = open (lock_file, O_RDWR | O_CREAT, LOCKMODE);
	if (fd < 0)
	{
		syslog (LOG_ERR, "невозможно открыть %s: %s",
			lock_file, strerror (errno));
		exit (EXIT_FAILURE);
	}
	if (lockfile (fd) < 0)
	{
		if (errno == EACCES || errno == EAGAIN)
	{
		close (fd);
		return 1;
	}
		syslog (LOG_ERR,
			"невозможно установить блокировку на %s: %s",
			lock_file, strerror (errno));
		exit (EXIT_FAILURE);
	}
	ftruncate (fd, 0);
	sprintf (buf, "%ld", (long) getpid ());
	write (fd, buf, strlen (buf) + 1);
	return 0;
}
