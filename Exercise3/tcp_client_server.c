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

void* client_thread(){
	// Declare variables
	struct addrinfo hints, *res;
	int sockfd;
	char connectmsg[1024] = "Connect to: 129.241.187.154:20007";
	char buf[1024];

	// Prepare get addrinfo
	memset(&hints, 0, sizeof hints);
	hints.ai_family = AF_UNSPEC;
	hints.ai_socktype = SOCK_STREAM;

	if (getaddrinfo("129.241.187.136", "33546", &hints, &res) != 0) {
		perror("Addrinfo error");
	}

	// Create new socket
	sockfd = socket(res->ai_family, res->ai_socktype, res->ai_protocol);

	// Connect new socket
	if (connect(sockfd, res->ai_addr, res->ai_addrlen) == -1)
	{
		perror("Error from connect");
	}

	// Recieve welcome message!
	if (recv(sockfd, buf, sizeof buf, 0) == -1)
	{
		perror("Error from receive");
	}
	printf("Received from client-side. Message be: %s\n", buf);

	// Send connect message
	if (send(sockfd, connectmsg, 1024, 0) == -1)
	{
		perror("Error from send");
	}

	close(sockfd);
}

void* server_thread(){
	// Declare variables
	struct sockaddr_storage their_addr;
	struct addrinfo chints, *cres;
	int csockfd, new_fd;
	char buf[1024];
	char testmsg[1024] = "This be me ye be connecting to lad";
	int status;
	socklen_t addr_size;

	// Prepare for getaddrinfo
	memset(&chints, 0, sizeof chints);
	chints.ai_family = AF_UNSPEC;
	chints.ai_socktype = SOCK_STREAM;
	chints.ai_flags = AI_PASSIVE;

	// Get addr
	if (getaddrinfo(NULL, "20007", &chints, &cres) != 0) {
    		perror("Addrinfo error");
	}

	// Bind and listen
	csockfd = socket(cres->ai_family, cres->ai_socktype, cres->ai_protocol);
	bind(csockfd, cres->ai_addr, cres->ai_addrlen);
	listen(csockfd, 2);

	// Accept
	addr_size = sizeof their_addr;
	if ((new_fd = accept(csockfd, (struct sockaddr *)&their_addr, &addr_size)) == -1)
	{
		perror("Error from accept");
	}
	
	// Send message
	if (send(new_fd, testmsg, 1024, 0) == -1)
	{
		perror("Error from serversend");
	}

	// Receive welcome message
	if (recv(new_fd, buf, sizeof buf, 0) == -1)
	{
		perror("Error from receive");
	}
	printf("Received from server-side. Message be: %s\n", buf);

	// Receive sent message
	if (recv(new_fd, buf, sizeof buf, 0) == -1)
	{
		perror("Error from receive");
	}
	printf("Received from server-side. Message be: %s\n", buf);

	// Close it up
	close(csockfd);
	close(new_fd);

}

void main () {
	pthread_t sthread_1;
	pthread_t sthread_2;
	pthread_create(&sthread_1, NULL, client_thread, NULL);
	pthread_create(&sthread_2, NULL, server_thread, NULL);

	pthread_join(sthread_1, NULL);
	pthread_join(sthread_2, NULL);
}

//129.241.187.144
