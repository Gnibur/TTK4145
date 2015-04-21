#pragma once

#include <cstddef>

bool udp_send(int targetPort, char *data, size_t dataLength);

bool udp_receive(int port, char *data, size_t dataLength);
 
