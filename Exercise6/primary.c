/* socket header files */
#include <sys/types.h>
#include <sys/socket.h>
#include <netdb.h>

/* Gets error status variable */
#include <errno.h>
#include <string.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

int sendUdpToAddr(char addr[], char targetPort[], char buf[], int buflen)
{
	int sockfd;
	struct addrinfo hints, *servinfo;
	
	memset(&hints, 0, sizeof hints);
	hints.ai_family = AF_UNSPEC;
	hints.ai_socktype = SOCK_DGRAM;

	if (getaddrinfo(addr, targetPort, &hints, &servinfo) != 0)
	{
		perror("talker: gettaddr");
		return 0;
	}
	if ((sockfd = socket(servinfo->ai_family, servinfo->ai_socktype, servinfo->ai_protocol)) == -1)
	{
		perror("talker: socket");
		return 0;
	}
	if (sendto(sockfd, buf, buflen, 0, servinfo->ai_addr, servinfo->ai_addrlen) == -1)
	{
		perror("talker: send");
		return 0;
	}
	freeaddrinfo(servinfo);
	return 1;
}

main () {
	int x = 0;
	char msg[512];
	while (1)
	{
		sprintf(msg, "%d", x);
		sendUdpToAddr("127.0.0.1", "2000", msg, 512);
		printf("Counting: %d\n", x);
		x = x + 1;
		sleep(2);
	}
}
