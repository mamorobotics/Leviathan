#include "Connection.hpp"

void Connection::Connect()
{
	UI::Get()->setConnectionDetails(connDetails);
}

void Connection::SendError(std::string message)
{
	Send(2, &message);
}

void Connection::SendWarning(std::string message)
{
	Send(1, &message);
}

void Connection::SendTelemetry(std::string key, std::string value)
{
    std::string message = key + "!" + value;
	Send(3, &message);
}

void Connection::Send(int header, void * message)
{
    auto msgTup = std::make_tuple(header, message);

    std::string msgLength = std::to_string(sizeof(msgTup));
    msgLength.insert(0, 32-msgLength.size(), ' ');
    auto lenSent = socket.send_to(asio::buffer(msgLength, 32), remote_endpoint, 0);
    std::cout << "Sent length --- " << lenSent << "\n";

    auto tupSent = socket.send_to(asio::buffer(&msgTup, sizeof(msgTup)), remote_endpoint, 0);
    std::cout << "Sent header --- " << tupSent << "\n";

    // std::string msgHeader = std::to_string(header);
    // msgHeader.insert(0, 32-msgHeader.size(), ' ');
    // auto headerSent = socket.send_to(asio::buffer(msgHeader, 32), remote_endpoint, 0);
    // std::cout << "Sent header --- " << headerSent << "\n";

    // auto msgSent = socket.send_to(asio::buffer(message, sizeof(message)), remote_endpoint, 0);
    // std::cout << "Sent message --- " << msgSent << "\n";
}

void Connection::HandleReceive(const asio::error_code& error, std::size_t bytes_received){
    if(!error){

    }else{
        std::cout << "Error Code for receiving: " << error.message() << std::endl;
    }

    socket.async_receive(asio::buffer(recv_buffer), std::bind(&Connection::HandleReceive, this, std::placeholders::_1, std::placeholders::_2));
}

void Connection::HandleHandshake(const asio::error_code& error, std::size_t bytes_transferred){
    if(!error){
        if(bytes_transferred != '0110'){
            std::cout << "[WARNING] Handshake with client failed" << std::endl;
        }else{
            connDetails.connectedIP = sender_endpoint.address().to_string();
            connDetails.connectedPort = sender_endpoint.port();
            connDetails.connectionStatus = "Connected";
            UI::Get()->setConnectionDetails(connDetails);
        }
    }else{
        std::cout << "Error Code for receiving: " << error.message() << std::endl;
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
