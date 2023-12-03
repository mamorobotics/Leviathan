#include "Connection.hpp"

void Connection::Connect()
{
	//suff

	UI::Get()->setConnectionDetails(connDetails);
}

void Connection::SendError(std::string message)
{
	Send(2, &message, message.size());
}

void Connection::SendWarning(std::string message)
{
	Send(1, &message, message.size());
}

void Connection::SendTelemetry(std::string key, std::string value)
{
    std::string message = key + "!" + value;
	Send(3, &message, message.size());
}

void Connection::Send(int header, void * message, int length)
{
//     auto sent = socket.send_to(asio::buffer(message, length), remote_endpoint, 0);
//     std::cout << "Sent Payload --- " << sent << "\n";

    std::string msgLength = std::to_string(length);
    msgLength.insert(0, 32-msgLength.size(), ' ');
    auto lenSent = socket.send_to(asio::buffer(msgLength, 32), remote_endpoint, 0);
    std::cout << "Sent length --- " << lenSent << "\n";

    std::string msgHeader = std::to_string(header);
    msgHeader.insert(0, 32-msgHeader.size(), ' ');
    auto headerSent = socket.send_to(asio::buffer(msgHeader, 32), remote_endpoint, 0);
    std::cout << "Sent header --- " << headerSent << "\n";

    auto msgSent = socket.send_to(asio::buffer(message, sizeof(message)), remote_endpoint, 0);
    std::cout << "Sent message --- " << msgSent << "\n";
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
