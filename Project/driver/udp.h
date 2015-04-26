#pragma once

#include <cstddef>
#include <string>

#define BROADCAST_PORT 2222

bool isValidIP(std::string ip);

std::string getMyIP();

bool udp_initialize(int targetPort);

bool udp_send(const char *data, size_t dataLength);

bool udp_receive(char *data, size_t dataLength);



