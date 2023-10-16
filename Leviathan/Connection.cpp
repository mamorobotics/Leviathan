#include "Connection.hpp"

Connection::Connection()
{
}

void Connection::Connect()
{
	asio::error_code ec;
	asio::io_context context;
	asio::ip::tcp::endpoint endpoint = asio::ip::tcp::endpoint(asio::ip::make_address("192.168.1.1", ec), 1974);
	asio::ip::tcp::socket socket = asio::ip::tcp::socket(context);
	socket.connect(endpoint, ec);

	if (!ec) { connDetails.connectionStatus = "Connected"; }
	else 
	{ 
		connDetails.connectionStatus = "Failed to connect";
		UI::Get()->PublishOutput(("Failed to connect to address: \n\t" + ec.message()), LEV_CODE::CONN_ERROR);
	}
	UI::Get()->setConnectionDetails(connDetails);
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
