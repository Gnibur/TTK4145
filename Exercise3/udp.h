#ifndef UDP_H
#define UDP_H

// Sends UDP broadcast.
int sendUdpBroadcast(int targetPort, char buf[], int buflen);

// Receives UDP broadcast.
int recvUdpBroadcast(int targetPort, char buf[], int buflen);

// Sends UDP to specific address.
int sendUdpToAddr(char addr[], char targetPort[], char buf[], int buflen);

#endif
