#pragma once

#include <cstddef>
#include <string>

#define BROADCAST_PORT 2222

bool udp_send(int targetPort, const char *data, size_t dataLength);

bool udp_receive(int port, char *data, size_t dataLength);

std::string getMyIP();
void findMyIP();
