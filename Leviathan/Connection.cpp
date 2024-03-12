#include "Connection.hpp"

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

void Connection::Send(int header, void* message)
{
    std::string initialMsg = std::to_string(sizeof(message)) + "!" + std::to_string(header);
    initialMsg.insert(0, 32-initialMsg.size(), ' ');
    auto lenSent = socket.send_to(asio::buffer(initialMsg, 32), remote_endpoint, 0);

    if(sizeof(message) > 65500){
        while(sizeof(message) > 65500){
            void* chunk = malloc(65500);
            std::memcpy(chunk, message, 65500);
            void* rest = malloc(sizeof(message) - 65500);
            std::memcpy(rest, message + 65500, sizeof(message) - 65500);
            free(message);
            message = rest;
            auto msgSent = socket.send_to(asio::buffer(chunk, 65500), remote_endpoint, 0);
            free(chunk);
            free(rest);
        }
    }
    if(sizeof(message) != 0){
        auto msgSent = socket.send_to(asio::buffer(message, sizeof(message)), remote_endpoint, 0);
    }
}

void Connection::Recieve() 
{
    UI* gui = UI::Get();
    int i = 0;
    int j=0;
    bool failedFrame = false;
    while (true)
    {
        j++;
        failedFrame = false;
        asio::error_code error;
        // size_buffer.resize(32);
        initial_buffer.resize(32);
        // socket.receive_from(asio::buffer(size_buffer), remote_endpoint, 0, error);
        // int size = 0;
        // try{
        //     size = stoi(std::string(size_buffer.data()));
        // }catch(const std::invalid_argument& e){
        //     UI::Get()->PublishOutput("Invalid size message", LEV_CODE::CONN_ERROR);

        //     size = 65500;

        //     failedFrame = true;
        // }

        // if (error.value()) gui->PublishOutput(error.message(), LEV_CODE::CONN_ERROR);
        
        // header_buffer.resize(32);
        // socket.receive_from(asio::buffer(header_buffer), remote_endpoint, 0, error);
        // int header = 0;
        // try{
        //     header = stoi(std::string(header_buffer.data()));
        // }catch(const std::invalid_argument& e){
        //     UI::Get()->PublishOutput("Invalid header message", LEV_CODE::CONN_ERROR);

        //     header = 11;

        //     failedFrame = true;
        // }

        // if (error.value()) gui->PublishOutput(error.message(), LEV_CODE::CONN_ERROR);

        socket.receive_from(asio::buffer(initial_buffer), remote_endpoint, 0, error);

        std::string msg = std::string(initial_buffer.data());
        int index = msg.find("!");
        std::string size = msg.substr(0, index);
        std::string header = msg.substr(index + 1, msg.length() - (id.length() + 1));

        data_buffer.resize(0);
        if(!isDecoding && header == 4){
            image_buffer.resize(size);
        }

        int total_size = 0;
        while (total_size < size - 1)
        {
            std::vector<char> buf;
            buf.resize((size - total_size) > 65500 ? 65500 : (size - total_size));
            socket.receive_from(asio::buffer(buf), remote_endpoint, 0, error);
            total_size += (size - total_size) > 65500 ? 65500 : (size - total_size);
            data_buffer.insert(data_buffer.end(), buf.begin(), buf.end());
        }

        if(!isDecoding && header == 4){
            image_buffer = data_buffer;
        }

        if (error.value()) gui->PublishOutput(error.message(), LEV_CODE::CONN_ERROR);

        if(header == 4 && !failedFrame && !isDecoding && !gui->isCameraPaused()){
            newImage = true;
        }
        
        if(header == 2)
        {
            gui->PublishOutput(std::string(data_buffer.data()), LEV_CODE::GENERAL_ERROR);
        }

        if(header == 1)
        {
            gui->PublishOutput(std::string(data_buffer.data()), LEV_CODE::WARNING);
        }

        if(header == 3)
        {
            std::string msg = std::string(data_buffer.data());
            int index = msg.find("!");
            std::string id = msg.substr(0, index);
            std::string value = msg.substr(index + 1, msg.length() - (id.length() + 1));
            gui->PublishTelemetry(id, value);
        }

        data_buffer.resize(0);
    }
}

void Connection::HandleHandshake(){
    UI* gui = UI::Get();
    data_buffer.resize(32);
    asio::error_code error;
    socket.receive_from(asio::buffer(data_buffer), remote_endpoint, 0, error);
    if(data_buffer.data() != NULL){
        if(std::string(data_buffer.data()) != "0110"){
            gui->PublishOutput("Handshake with client failed", LEV_CODE::CONN_ERROR);
        } else { 
            gui->PublishOutput("Succesful handshake with client", LEV_CODE::CLEAR);
            connDetails.connectedIP = remote_endpoint.address().to_string();
            connDetails.connectedPort = "8080";
            connDetails.connectionStatus = "Connected";
            UI::Get()->setConnectionDetails(connDetails);
            Recieve();
        }
    }else{
        gui->PublishOutput("Error Code for receiving: " + error.message(), LEV_CODE::CONN_ERROR);
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
