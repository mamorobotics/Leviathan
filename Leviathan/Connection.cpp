#include "Connection.hpp"

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

void Connection::SendError(std::string message)
{f
	Send(2, message);
}

void Connection::SendWarning(std::string message)
{
	Send(1, message);
}

void Connection::SendTelemetry(std::string key, std::string value)
{
	Send(3, key + "!!" + value);
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
