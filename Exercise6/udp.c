/* A small program which connects to server using UDP
 */


/* socket header files */
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


#include <unistd.h>


int sendport = 20007;
int receiveport = 30000;

int sendUdpBroadcast(int targetPort, char buf[], int buflen)
{
  int sockfd = socket(AF_INET, SOCK_DGRAM, 0);
  if (sockfd == -1){
    printf("Error creating socket\n");
    return 0;
  }
   
  int yes = 1;
  setsockopt(sockfd, SOL_SOCKET, SO_BROADCAST, &yes, sizeof yes);

  struct sockaddr_in target;
  memset(&target, 0, sizeof target);
  
  target.sin_family = AF_INET;
  target.sin_addr.s_addr = htonl(-1);  // broadcast address
  target.sin_port = htons(targetPort);
    
  int status = sendto(sockfd, buf, buflen, 0, (struct sockaddr *)&target, sizeof target);
  close(sockfd);

  if (status != -1)
    return 1;
  else 
    return 0;
}


int recvUdpBroadcast(int targetPort, char buf[], int buflen)
{
  int sockfd = socket(AF_INET, SOCK_DGRAM, 0);

  if (sockfd == -1){
    printf("Error creating socket\n");
    return 0;
  }

  struct sockaddr_in target;
  memset(&target, 0, sizeof target);
  
  target.sin_family = AF_INET;
  target.sin_addr.s_addr = htonl(INADDR_ANY); 
  target.sin_port = htons(targetPort);

  struct timeval tv;
  tv.tv_sec = 0;
  tv.tv_usec = 100000;
  if (setsockopt(sockfd, SOL_SOCKET, SO_RCVTIMEO,&tv,sizeof(tv)) < 0) {
    perror("Error");
  }

    
  bind(sockfd, (struct sockaddr*)&target, sizeof target);


  int status = recv(sockfd, buf, (size_t)buflen, 0);
  close(sockfd);

  if (status != -1)
    return 1;
  else
    return 0;
}

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

 
/*int main(int argc, char *argv[])
{
  if (argc == 3){
    sendport = atoi(argv[1]);
    receiveport = atoi(argv[2]);
  }

  char buf[1024] = {};
  if (recvUdpBroadcast(receiveport, buf, 1024))
    printf("Received: %s\n", buf);;
  
  sprintf(buf, "Hello world");
  if (sendUdpBroadcast(sendport, buf, 1024))
    printf("Sent message\n");

  return 0;
}*/
