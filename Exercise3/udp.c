/* A small program which connects to server using UDP
 */


/* socket header files */
#include <sys/types.h>
#include <sys/socket.h>
#include <netdb.h>

/* Gets error status variable */
#include <errno.h>
#include <string.h>
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
    
  bind(sockfd, (struct sockaddr*)&target, sizeof target);


  int status = recv(sockfd, buf, (size_t)buflen, 0);
  close(sockfd);

  if (status != -1)
    return 1;
  else
    return 0;
}

 
int main(int argc, char *argv[])
{
  if (argc == 3){
    sendport = atoi(argv[1]);
    receiveport = atoi(argv[2]);
  }

  char buf[1024] = {};
  if (recvUdpBroadcast(1024, buf, receiveport))
    printf("Received: %s\n", buf);;
  
  sprintf(buf, "Hello world");
  if (sendUdpBroadcast(1024, buf, sendport))
    printf("Sent message\n");

  return 0;
}
