#include "Connection.hpp"

void Connection::Connect()
{
    std::cout << "connecting..." << std::endl;
    asio::error_code error;
    //socket.receive_from(asio::buffer(recv_buffer), sender_endpoint, 0, error);
	UI::Get()->setConnectionDetails(connDetails);
    // std::string test = "hello";
    // test.insert(0, 32-test.size(), ' ');
    // auto tupSent = socket.send_to(asio::buffer(test, 32), remote_endpoint, 0);
    while(true){
        socket.receive_from(asio::buffer(recv_buffer), remote_endpoint, 0, error);
        if(recv_buffer.data() != NULL){
            if(std::string(recv_buffer.data()) != "0110"){
                std::cout << "[WARNING] Handshake with client failed" << std::endl;
            }else{
                connDetails.connectedIP = remote_endpoint.address().to_string();
                connDetails.connectedPort = remote_endpoint.port();
                connDetails.connectionStatus = "Connected";
                UI::Get()->setConnectionDetails(connDetails);
                socket.async_receive_from(asio::buffer(recv_buffer), remote_endpoint, std::bind(&Connection::HandleHandshake, this, std::placeholders::_1, std::placeholders::_2));
                break;
            }
        }else{
            std::cout << "Error Code for receiving: " << error.message() << std::endl;
        }
    }
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
        int length;
        std::cout << "Number Received = " << (numMessages+1) << std::endl;
        if(numMessages==0){
            length = stoi(std::string(recv_buffer.data(), bytes_received));
            numMessages++;
        }
        else if(numMessages == 2){
            std::cout << "Received all three messages" << std::endl;

            auto tup = std::make_tuple(recv_buffer.data());
            char * headerP = std::get<0>(tup);
            // int header = stoi(std::string(headerP, &headerP.size()));
            // std::string message;

            // std::cout << "Length:" << length << std::endl;
            // std::cout << "Header:" << header << std::endl;
            // std::cout << "Message:" << message << std::endl;
            // numMessages=0;
        }
    }else{
        std::cout << "Error Code for receiving: " << error.message() << std::endl;
    }

    socket.async_receive(asio::buffer(recv_buffer), std::bind(&Connection::HandleReceive, this, std::placeholders::_1, std::placeholders::_2));
}

void Connection::HandleHandshake(){
    // std::cout << "smth was received" << std::endl; 
    // if(!error){
    //     std::cout << "Received a message, whose size is: " << bytes_transferred << "and whose data is"  << std::string(recv_buffer.data(), bytes_transferred) << std::endl;
    //     if(std::string(recv_buffer.data(), bytes_transferred) != "0110"){
    //         std::cout << "[WARNING] Handshake with client failed" << std::endl;
    //     }else{
    //         connDetails.connectedIP = sender_endpoint.address().to_string();
    //         connDetails.connectedPort = sender_endpoint.port();
    //         connDetails.connectionStatus = "Connected";
    //         UI::Get()->setConnectionDetails(connDetails);
    //     }
    // }else{
    //     std::cout << "Error Code for receiving: " << error.message() << std::endl;
    // }
    // socket.async_receive(asio::buffer(recv_buffer), std::bind(&Connection::HandleReceive, this, std::placeholders::_1, std::placeholders::_2));
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
