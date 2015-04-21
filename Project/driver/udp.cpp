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

bool udp_send(int targetPort, const char *data, size_t dataLength)
{
  int socketfd = socket(AF_INET, SOCK_DGRAM, 0);

  int yes = 1;
  setsockopt(socketfd, SOL_SOCKET, SO_BROADCAST, &yes, sizeof yes);
  
  struct sockaddr_in target;
  ssize_t bytes_sent;
  memset(&target, 0, sizeof target);
  
  target.sin_family = AF_INET;
  target.sin_addr.s_addr = htonl(-1);  // broadcast address
  target.sin_port = htons(targetPort);
    
  bytes_sent  = sendto(socketfd, data, dataLength, 0, (struct sockaddr *)&target, sizeof target);

  close(socketfd);

  if (bytes_sent == dataLength)
    return true;
  else 
    return false;
}

bool udp_receive(int port, char *data, size_t dataLength)
{
  int socketfd = socket(AF_INET, SOCK_DGRAM, 0);

  int yes = 1;
  setsockopt(socketfd, SOL_SOCKET, SO_BROADCAST, &yes, sizeof yes);

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

