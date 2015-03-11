#pragma once

#include <cstddef>

class UdpBroadcast {
  
 public:
  UdpBroadcast();
  ~UdpBroadcast();

  bool send(int targetPort, char *data, size_t dataLength);

  bool receive(int port, char *data, size_t dataLength);
  
 private:

  int socketfd;


};
