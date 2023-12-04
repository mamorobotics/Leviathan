#include "Connection.hpp"

void Connection::Connect()
{
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
    std::stringstream stream;

    std::string msgLength = std::to_string(length);
    msgLength.insert(0, 32-msgLength.size(), ' ');
    auto lenSent = socket.send_to(asio::buffer(msgLength, 32), remote_endpoint, 0);
    std::cout << "Sent length --- " << lenSent << "\n";

    auto msgTup = std::make_tuple(header, message);

    std::string msgHeader = std::to_string(header);
    msgHeader.insert(0, 32-msgHeader.size(), ' ');
    auto headerSent = socket.send_to(asio::buffer(msgHeader, 32), remote_endpoint, 0);
    std::cout << "Sent header --- " << headerSent << "\n";

    auto msgSent = socket.send_to(asio::buffer(message, sizeof(message)), remote_endpoint, 0);
    std::cout << "Sent message --- " << msgSent << "\n";
}

void Connection::HandleReceive(const asio::error_code& error, std::size_t bytes_received){
    if(!error){

    }else{
        std::cout << error.message();
    }

    socket.async_receive(asio::buffer(recv_buffer), std::bind(&Connection::HandleReceive, this, std::placeholders::_1, std::placeholders::_2));
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
