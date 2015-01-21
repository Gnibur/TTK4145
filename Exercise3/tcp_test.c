#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <sys/un.h>
#include <arpa/inet.h>
#include <netdb.h>
#include <stdio.h>
#include <errno.h>

void main () {
	struct addrinfo hints, *res;
	int sockfd;
	int connecterr;
	int status;
	int senderr;
	char msg[512];
	//char buff[1024];
	//int byte_count;

	memset(&hints, 0, sizeof hints);
	hints.ai_family = AF_UNSPEC;
	hints.ai_socktype = SOCK_STREAM;

	if ((status = getaddrinfo("129.241.187.144", "20007", &hints, &res)) != 0) {
    		fprintf(stderr, "getaddrinfo error: %s\n", gai_strerror(status));
	}

	// make a socket:

	sockfd = socket(res->ai_family, res->ai_socktype, res->ai_protocol);

	// connect!

	if ((connecterr = connect(sockfd, res->ai_addr, res->ai_addrlen)) == -1)
	{
		perror("Error from connect");
	}
	
	if ((senderr = (send(sockfd, msg, 512, 0))) == -1)
	{
		perror("Error from send");
	} 


	close(sockfd);
}

//129.241.187.144
