#include "Connection.hpp"

Connection::Connection()
{
	ui = UI::Get();
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
