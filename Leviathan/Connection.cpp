#include "Connection.hpp"

#define SENDSIZE 1024
#define PORT 8080
#define IP "192.168.1.1"

Connection::Connection()
{
}

void Connection::Connect()
{
	//suff

	UI::Get()->setConnectionDetails(connDetails);
}

void Connection::Send(int header, std::string message)
{
	//stuff
}

Connection::~Connection()
{
}

Connection* Connection::Get()
{
	if (connection == nullptr)
		connection = new Connection();

	return connection;
}
