#pragma once
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
	void Send(int header, std::string message);
	Connection(const Connection& obj) = delete;
	~Connection();
	static Connection* Get();
};

