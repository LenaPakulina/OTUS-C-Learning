#include "supported_fonts.h"

#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <netdb.h>

#define PORT 23
#define BUF_SIZE 4096
#define HOSTNAME "telehack.com"
#define SERVICE "telnet"

static char buffer[BUF_SIZE] = { 0 };
char query_string[1024] = { 0 };

int main (int argc, char **argv)
{
	char *font = NULL;
	char *text = NULL;

	if (argc != 3) {
		printf("Use: app <font> <text>");
		return EXIT_FAILURE;
	}

	font = argv[1];
	text = argv[2];

	int isFontExst = 0;
	for (uint32_t i = 0; i < supported_fonts_count; i++) {
		if (strcmp(font, supported_fonts[i]) == 0) {
			isFontExst = 1;
			break;
		}
	}

	if (!isFontExst) {
		printf("Unsupported font %s.", font);
		return EXIT_FAILURE;
	}

	snprintf (query_string, sizeof(query_string), "figlet /%s %s\r\n", font, text);

	struct addrinfo *result, *rp;
	struct addrinfo hints = {0};
	int sfd;

	hints.ai_family = AF_INET;	/* Allow IPv4 or IPv6 */
	hints.ai_socktype = SOCK_STREAM;	/* Datagram socket */
	hints.ai_flags = 0;		/* For wildcard IP address */
	hints.ai_protocol = IPPROTO_TCP;

	int s = getaddrinfo (HOSTNAME, SERVICE, &hints, &result);
	if (s != 0) {
		printf("getaddrinfo: %s\n", gai_strerror (s));
		return EXIT_FAILURE;
	}

	/* getaddrinfo() returns a list of address structures.
	 Try each address until we successfully connect(2).
	 If socket(2) (or connect(2)) fails, we (close the socket
	 and) try the next address. */

	for (rp = result; rp != NULL; rp = rp->ai_next) {
		sfd = socket (rp->ai_family, rp->ai_socktype, rp->ai_protocol);
		if (sfd == -1)
			continue;

		if (connect (sfd, rp->ai_addr, rp->ai_addrlen) != -1)
			break;			/* Success */

		close (sfd);
	}

	if (rp == NULL) {
		printf("No address succeeded\n");
		return EXIT_FAILURE;
	}

	freeaddrinfo(result);

	int len = 0, r = 0;
	while ((r = recv (sfd, &buffer[len], BUF_SIZE - len, 0)) > 0){
		len += r;
		if (buffer[len - 1] == '.' && buffer[len - 2] == '\n')
			break;
	}

	if (send (sfd, query_string, strlen (query_string), 0) < 0){
		close (sfd);
		printf("Send error.\n");
		return EXIT_FAILURE;
	}

	len = 0, r = 0;
	while ((r = recv (sfd, &buffer[len], BUF_SIZE - len, 0)) > 0){
		len += r;
		if (buffer[len - 1] == '.' && buffer[len - 2] == '\n')
			break;
	}

	if (r < 0){
		close (sfd);
		return EXIT_FAILURE;
	}

	buffer[len] = '\0';

	puts (buffer);
	shutdown (sfd, SHUT_RDWR);
	close (sfd);

	return EXIT_SUCCESS;

}
