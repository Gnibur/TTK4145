#include <time.h>
#include <stdlib.h>
#include <stdio.h>


/* socket header files */
#include <sys/types.h>
#include <sys/socket.h>
#include <netdb.h>

/* Gets error status variable */
#include <errno.h>
#include <string.h>
#include <stdio.h>

#include <unistd.h>

#define BROADCAST_PORT 20007
#define TIMEOUT_LIMIT 1
#define BUFFER_LENGTH 1024

enum state { BACKUP, PRIMARY };

enum state currentState = BACKUP;
int currentNumber = 0;


void runAsBackup(void);
int readImAliveMessage(char buf[], int buflen);
void spawnNewBackup();

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


int main(int argc, char *argv[])
{
  runAsBackup();
  printf("Now it runs as primary\n");
  spawnNewBackup();
  //runAsPrimary();
}


void runAsBackup(void)
{
  char buf[BUFFER_LENGTH];
  
  int nextNumber = 0;

  time_t lastReceiveTime = time(0);

  while (1){
    
    if (lastReceiveTime + TIMEOUT_LIMIT < time(0))
      break;

    printf("Receiving...\n");
    if (recvUdpBroadcast(BROADCAST_PORT, buf, BUFFER_LENGTH)) {

      printf("Received broadcast\n");
      int temp = readImAliveMessage(buf, BUFFER_LENGTH);

      if (temp != -1){
	currentNumber = temp;
	lastReceiveTime = time(0);
      }
    }
  }
}

int readImAliveMessage(char buf[], int buflen)
{
  // include error checking
  return atoi(buf);  
}

void spawnNewBackup()
{

}
