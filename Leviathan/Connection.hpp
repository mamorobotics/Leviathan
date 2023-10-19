#pragma once

#include <WinSock2.h>
#include <Ws2tcpip.h>
#pragma comment(lib, "Ws2_32.lib")

#include <asio.hpp>

#include "UI.hpp"
#include "ConnDetails.hpp"
#include "Management.hpp"

class Connection
{
private:
	ConnDetails connDetails;
	SOCKET clientSocket = INVALID_SOCKET;
	struct sockaddr_in clientAddr;

	static Connection* connection;

public:
	Connection();
	void Connect();
	void Send(std::string message);
	Connection(const Connection& obj) = delete;
	~Connection();
	static Connection* Get();
};

