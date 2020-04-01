#ifndef _SOCKET_SERVER_H
#define _SOCKET_SERVER_H

#include <netinet/in.h>    // for sockaddr_in
#include <sys/types.h>    // for socket
#include <sys/socket.h>    // for socket
#include <stdlib.h>        // for exit
#include <string.h>        // for bzero
#include <stdio.h>        // for printf
#include <unistd.h>
#include <arpa/inet.h>
#include <iostream>


class DetectorServer
{
public:
	DetectorServer();
	~DetectorServer();

	void StartModel();

	bool StartServer();

	std::string SendMessage2Client(std::string sendstr);

	
private:
	struct sockaddr_in server_addr;
	int server_socket;
	bool ModelLoaded;
	std::string StatusStr;
};

#endif
