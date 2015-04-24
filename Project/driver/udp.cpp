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
#include <pthread.h>

std::string myIP;
static int recvsocketfd;
static int sendsocketfd;

static pthread_mutex_t udpSendMutex;

static struct sockaddr_in target;

bool udp_initialize(int targetPort)
{
	pthread_mutex_init(&udpSendMutex, NULL);
    sendsocketfd = socket(AF_INET, SOCK_DGRAM, 0);
	recvsocketfd = socket(AF_INET, SOCK_DGRAM, 0);
    int yes = 1;
    setsockopt(sendsocketfd, SOL_SOCKET, SO_BROADCAST, &yes, sizeof yes);
   	setsockopt(recvsocketfd, SOL_SOCKET, SO_BROADCAST, &yes, sizeof yes);


  	memset(&target, 0, sizeof target);
	target.sin_family = AF_INET;
	target.sin_addr.s_addr = htonl(-1);  // broadcast address
	target.sin_port = htons(targetPort);

  	bind(recvsocketfd, (struct sockaddr*)&target, sizeof target);


	return true; // DO SOME ERROR CHECKING
}

void udp_close ()
{
 	close(sendsocketfd);
 	close(recvsocketfd);
}

bool udp_send(const char *data, size_t dataLength)
{
	pthread_mutex_lock(&udpSendMutex);
	ssize_t bytes_sent;
  	
	bytes_sent  = sendto(sendsocketfd, data, dataLength, 0, (struct sockaddr *)&target, sizeof target);
	pthread_mutex_unlock(&udpSendMutex);
	if (bytes_sent == dataLength)	
		return true;
	else
	{
		std::cout << "Did not send enough bytes!" << std::endl; 
		return false;
	}
}

bool udp_receive(char *data, size_t dataLength)
{
 
	ssize_t bytes_received;

	/*struct timeval tv;
	tv.tv_sec = 0;
	tv.tv_usec = 100000;
	if (setsockopt(socketfd, SOL_SOCKET, SO_RCVTIMEO,&tv,sizeof(tv)) < 0) {
	perror("Error");
	}*/

	bytes_received = recv(recvsocketfd, data, dataLength, 0);
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
