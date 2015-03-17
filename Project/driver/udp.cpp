#include "udp.h"


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


/* TODO:
   - check last parameter in socket(...)
 */
Udp::Udp() 
{
    socketfd = socket(AF_INET, SOCK_DGRAM, 0);

    int yes = 1;
    setsockopt(socketfd, SOL_SOCKET, SO_BROADCAST, &yes, sizeof yes);
}
/* TODO:
   - Check if socket is initialized before calling close

 */
Udp::~Udp()
{
  close(socketfd);
}

bool Udp::send(int targetPort, char *data, size_t dataLength){
  
  struct sockaddr_in target;
  ssize_t bytes_sent;
  memset(&target, 0, sizeof target);
  
  target.sin_family = AF_INET;
  target.sin_addr.s_addr = htonl(-1);  // broadcast address
  target.sin_port = htons(targetPort);
    
  bytes_sent  = sendto(socketfd, data, dataLength, 0, (struct sockaddr *)&target, sizeof target);

  if (bytes_sent == dataLength)
    return true;
  else 
    return false;
}

bool Udp::receive(int port, char *data, size_t dataLength){
  struct sockaddr_in target;
  ssize_t bytes_received;

  memset(&target, 0, sizeof target);
  
  target.sin_family = AF_INET;
  target.sin_addr.s_addr = htonl(INADDR_ANY); 
  target.sin_port = htons(port);

  struct timeval tv;
  tv.tv_sec = 0;
  tv.tv_usec = 100000;
  if (setsockopt(socketfd, SOL_SOCKET, SO_RCVTIMEO,&tv,sizeof(tv)) < 0) {
    perror("Error");
  }
    
  bind(socketfd, (struct sockaddr*)&target, sizeof target);

  bytes_received = recv(socketfd, data, dataLength, 0);
  if (bytes_received != -1)
    return true;
  else 
    return false;
}


bool Udp::sendtoIP(char *destinationIP, int port, char *data, size_t dataLength){
  /*  struct addrinfo hints, *servinfo;
	
  memset(&hints, 0, sizeof hints);
  hints.ai_family = AF_UNSPEC;
  hints.ai_socktype = SOCK_DGRAM;

  if (getaddrinfo(destinationIP, targetPort, &hints, &servinfo) != 0)
    {
      perror("talker: gettaddr");
      return 0;
    }
  if ((sockfd = socket(servinfo->ai_family, servinfo->ai_socktype, servinfo->ai_protocol)) == -1)
    {
      perror("talker: socket");
      return 0;
    }
  if (sendto(sockfd, data, dataLength, 0, servinfo->ai_addr, servinfo->ai_addrlen) == -1)
    {
      perror("talker: send");
      return 0;
    }
  freeaddrinfo(servinfo);
  */
}
