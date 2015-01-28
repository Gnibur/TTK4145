#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <sys/un.h>
#include <arpa/inet.h>
#include <netdb.h>
#include <stdio.h>
#include <errno.h>
#include <pthread.h>
#include <stdio.h>

void main () {
	struct sockaddr_storage their_addr;
	struct addrinfo hints, *res, chints, *cres;
	int sockfd, csockfd, new_fd;
	int connecterr;
	int byte_count;
	int status;
	int senderr;
	socklen_t addr_size;
	char msg[1024] = "Are you not supposed to copy me?\0";
	char connectmsg[1024] = "Connect to: 129.241.187.154:20007";
	char buf[1024];
	//char buff[1024];
	//int byte_count;

	memset(&hints, 0, sizeof hints);
	hints.ai_family = AF_UNSPEC;
	hints.ai_socktype = SOCK_STREAM;

	if ((status = getaddrinfo("129.241.187.136", "33546", &hints, &res)) != 0) {
    		fprintf(stderr, "getaddrinfo error: %s\n", gai_strerror(status));
	}

	// make a socket:

	sockfd = socket(res->ai_family, res->ai_socktype, res->ai_protocol);
	// connect!

	if ((connecterr = connect(sockfd, res->ai_addr, res->ai_addrlen)) == -1)
	{
		perror("Error from connect");
	}
	byte_count = recv(sockfd, buf, sizeof buf, 0);
	printf("recv()'d %d bytes of data in buf\n", byte_count);
	printf("Message was: %s\n", buf);

	if ((senderr = (send(sockfd, connectmsg, 1024, 0))) == -1)
	{
		perror("Error from send");
	}

	byte_count = recv(sockfd, buf, sizeof buf, 0);
	printf("recv()'d %d bytes of data in buf\n", byte_count);
	printf("Message was: %s\n", buf);

	memset(&chints, 0, sizeof chints);
	chints.ai_family = AF_UNSPEC;
	chints.ai_socktype = SOCK_STREAM;
	chints.ai_flags = AI_PASSIVE;

	if ((status = getaddrinfo(NULL, "20007", &chints, &cres)) != 0) {
    		fprintf(stderr, "getaddrinfo error: %s\n", gai_strerror(status));
	}


	csockfd = socket(cres->ai_family, cres->ai_socktype, cres->ai_protocol);
	bind(csockfd, cres->ai_addr, cres->ai_addrlen);
	listen(csockfd, 2);

	addr_size = sizeof their_addr;
	/*if ((new_fd = accept(csockfd, (struct sockaddr *)&their_addr, &addr_size)) == -1)
	{
		perror("Error from accept");
	}*/

	close(sockfd);
	close(csockfd);
	//close(new_fd);
}

//129.241.187.144
