#pragma once

#include <cstddef>

class Udp {
  
 public:
  Udp();
  ~Udp();

  bool send(int targetPort, char *data, size_t dataLength);

  bool receive(int port, char *data, size_t dataLength);
 
  bool sendtoIP(char *ip, int port, char *data, size_t dataLength); 
 
  bool receiveFrom(char *ip, int port, char *buffer, size_t bufferLength);
 private:

  int socketfd;

};
