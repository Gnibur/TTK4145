#include <time.h>
#include <stdlib.h>
#include <stdio.h>

#include <unistd.h>


#include "udp.h"

#define COMMUNICATION_PORT 20007
#define TIMEOUT_LIMIT 5
#define BUFFER_LENGTH 1024
#define LOCALHOST "127.0.0.1"


void runAsBackup(void);
void runAsPrimary(void);

int nextNumber = 0;

int main(int argc, char *argv[])
{
  
  while (1) {
    runAsBackup();

    // if id != 0, then we know that a child was spawned.
    pid_t id = fork();
    if (id != 0){
      printf("Spawned new backup, with id %d\n",id);
      runAsPrimary();
    } else {
      printf("I am the child, hello.\n");
    }
  }
}


void runAsBackup(void)
{
  char buf[BUFFER_LENGTH];
  
  time_t lastReceiveTime = time(0);

  while (1){
    
    if (lastReceiveTime + TIMEOUT_LIMIT < time(0))
      break;
    if (recvUdpBroadcast(COMMUNICATION_PORT, buf, BUFFER_LENGTH)) {

      printf("Received broadcast\n");
      int temp = atoi(buf);
      nextNumber = temp;
      lastReceiveTime = time(0);
    }
  }
}

void runAsPrimary(void)
{
  char msg[BUFFER_LENGTH];
  char temp[BUFFER_LENGTH];
  while (1){
    printf("Counting: %d\n", nextNumber);
    nextNumber++;
    sprintf(msg, "%d", nextNumber);
    sprintf(temp, "%d", COMMUNICATION_PORT);
    sendUdpToAddr(LOCALHOST, temp, msg, BUFFER_LENGTH);
    sleep(2);
  }
}
