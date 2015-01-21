/* A small program which connects to server using UDP

 */


/* socket header files */
#include <sys/types.h>
#include <sys/socket.h>
#include <netdb.h>

/* Gets error status variable */
#include <errno.h>

/* close file */
#include <unistd.h>

#include <string.h>
#include <stdio.h>


 
int main(void)
{
  struct sockaddr_in si_other, si_me;
  int s, i, slen=sizeof(si_other);
  char buf[1024];

  memset(&si_me, 0, sizeof si_me);
  si_me.sin_family = AF_INET;
  si_me.sin_port = htons(30000);
  si_me.sin_addr.s_addr = htonl(INADDR_ANY);

  if ((s=socket(AF_INET, SOCK_DGRAM, IPPROTO_UDP))==-1)
    perror("socket");

  /*memset(&si_other, 0, sizeof(si_other));
  si_other.sin_family = AF_INET;
  si_other.sin_port = htons(30000);
  */
  if (inet_aton("129.241.187.255", &si_other.sin_addr)==0) {
    fprintf(stderr, "inet_aton() failed\n");
  }

  if (bind(s, (struct sockaddr*)&si_me, sizeof si_me) == -1)
    perror("binding:");
  
  size_t len = 1024;
  i = recv(s, buf, len, 0);
  printf("Received %i, %s", i, buf);
  /*  for (i=0; i<NPACK; i++) {
    printf("Sending packet %d\n", i);
    sprintf(buf, "This is packet %d\n", i);
    if (sendto(s, buf, BUFLEN, 0, &si_other, slen)==-1)
      diep("sendto()");
      }*/
  
 
  close(s);
  return 0;
}
