#include "Connection.hpp"

Connection::Connection()
{
}

void Connection::Connect()
{
	asio::error_code ec;
	asio::io_context context;
	asio::ip::tcp::endpoint endpoint = asio::ip::tcp::endpoint(asio::ip::make_address(UI.connectedIP, ec), 1974);
	asio::ip::tcp::socket socket = asio::ip::tcp::socket(context);
	socket.connect(endpoint, ec);
	if (!ec) { UI::Get()->PublishOutput("Connected!"); }
	else { UI::Get()->PublishOutput("Failed to connect to address: \n" + ec.message()); }
	
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
