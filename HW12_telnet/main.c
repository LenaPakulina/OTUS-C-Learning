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
char g_queryStr[1024] = { 0 };

int createConnect() {
	int fd;
	struct addrinfo hints = {0};
	hints.ai_family = AF_INET;
	hints.ai_socktype = SOCK_STREAM;
	hints.ai_flags = 0;
	hints.ai_protocol = IPPROTO_TCP;

	struct addrinfo *result;
	int ans = getaddrinfo(HOSTNAME, SERVICE, &hints, &result);
	if (ans != 0) {
		return -1;
	}

	struct addrinfo *currAddrStr;
	for (currAddrStr = result; currAddrStr != NULL; currAddrStr = currAddrStr->ai_next) {
		fd = socket(currAddrStr->ai_family, currAddrStr->ai_socktype, currAddrStr->ai_protocol);
		if (fd == -1)
			continue;
		if (connect(fd, currAddrStr->ai_addr, currAddrStr->ai_addrlen) != -1) {
			break;
		}
		close (fd);
	}

	if (currAddrStr == NULL) {
		return -1;
	}

	freeaddrinfo(result);
	return fd;
}

int receivedDataSocket (int fd) {
	memset(buffer, NULL, BUF_SIZE);

	int len = 0, r = 0;
	while ((r = recv (fd, &buffer[len], BUF_SIZE - len, 0)) > 0){
		len += r;
		if (buffer[len - 1] == '.' && buffer[len - 2] == '\n')
			break;
	}
	return r;
}

int main (int argc, char **argv)
{
	if (argc != 3) {
		printf("Use: app <font> <text>");
		return EXIT_FAILURE;
	}

	char *font = argv[1];
	char *text = argv[2];

	snprintf(g_queryStr, sizeof(g_queryStr), "figlet /%s %s\r\n", font, text);

	int fd = createConnect();
	if (fd < 0) {
		printf("Can't create connection\n");
		return EXIT_FAILURE;
	}

	if (receivedDataSocket(fd) < 0){
		close (fd);
		return EXIT_FAILURE;
	}

	if (send(fd, g_queryStr, strlen(g_queryStr), 0) < 0){
		close (fd);
		printf("Send error.\n");
		return EXIT_FAILURE;
	}

	if (receivedDataSocket(fd) < 0){
		close (fd);
		return EXIT_FAILURE;
	}
	puts (buffer);
	shutdown (fd, SHUT_RDWR);
	close (fd);

	return 0;

}
