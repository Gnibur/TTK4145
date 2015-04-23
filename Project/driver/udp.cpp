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
#include <iostream>
#include <fstream>
#include <string>

#include <unistd.h>

std::string myIP;
/*int socketfd;

void udp_init ()
{
    socketfd = socket(AF_INET, SOCK_DGRAM, 0);
    int yes = 1;
    setsockopt(socketfd, SOL_SOCKET, SO_BROADCAST, &yes, sizeof yes);
    
}

void udp_close ()
{
}*/

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
	{
        return true;
	}
    else
	{
		std::cout << "Not sent!" << std::endl; 
        return false;
	}
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

  /*struct timeval tv;
  tv.tv_sec = 0;
  tv.tv_usec = 100000;
  if (setsockopt(socketfd, SOL_SOCKET, SO_RCVTIMEO,&tv,sizeof(tv)) < 0) {
    perror("Error");
  }*/
  bind(socketfd, (struct sockaddr*)&target, sizeof target);
  bytes_received = recv(socketfd, data, dataLength, 0);

  close(socketfd);
	if (bytes_received != -1)
    return true;
  else 
    return false;
}

void findMyIP()
{
	std::string line;
	std::ifstream IPFile;
	int offset; 
	char* search0 = "inet addr:";      // search pattern
        
	system("ifconfig > ip.txt");

	IPFile.open ("ip.txt"); 
	if(IPFile.is_open())
	{
   		while(!IPFile.eof())
   		{
   			getline(IPFile,line);
   			if ((offset = line.find(search0, 0)) != std::string::npos)
   			{   
   				line.erase(0,20);
				line.erase(15,58);
   				std::cout << line << std::endl;
				myIP = line;
   				IPFile.close();
   			}
		}
	}
}

std::string getMyIP ()
{
	return myIP;
}
