#include "Connection.hpp"

void Connection::Connect()
{
    //maybe stuffs
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
    std::string msgLength = std::to_string(sizeof(message));
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

void Connection::HandleReceive(std::size_t bytes_received){
    std::string length;
    std::string header;
    std::cout << "Number Received = " << (numMessages+1) << std::endl;
    if(numMessages==0){
        recvLength = std::string(recv_buffer.data(), bytes_received);
        numMessages++;
        StartReceiving();
    }
    else if(numMessages==1){
        recvHeader = std::string(recv_buffer.data(), bytes_received);
        numMessages++;
        ResizeBuffer(stoi(recvLength));
        StartReceiving();
    }
    else if(numMessages == 2){
        std::cout << "Received all three messages" << std::endl;

        std::string message = std::string(recv_buffer.data());

        std::cout << "Length:" << recvLength << std::endl;
        std::cout << "Header:" << recvHeader << std::endl;
        std::cout << "Message:" << message << std::endl;

        numMessages=0;
        StartReceiving();
        ResizeBuffer(32);
    }
}

void Connection::ResizeBuffer(int newSize){
    recv_buffer.resize(newSize);
}

void Connection::HandleHandshake(){
    ResizeBuffer(64);
    asio::error_code error;
    socket.receive_from(asio::buffer(recv_buffer), remote_endpoint, 0, error);
    if(recv_buffer.data() != NULL){
        if(std::string(recv_buffer.data()) != "0110"){
            std::cout << "[WARNING] Handshake with client failed" << std::endl;
        }else{
            std::cout << "got handshake" << std::endl;
            connDetails.connectedIP = remote_endpoint.address().to_string();
            connDetails.connectedPort = "8080";
            connDetails.connectionStatus = "Connected";
            UI::Get()->setConnectionDetails(connDetails);
            std::thread recurs (&Connection::StartReceiving, this);
            io_context.run();
        }
    }else{
        std::cout << "Error Code for receiving: " << error.message() << std::endl;
    }
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
