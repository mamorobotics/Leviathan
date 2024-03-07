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

    std::string msgHeader = std::to_string(header);
    msgHeader.insert(0, 32-msgHeader.size(), ' ');
    auto headerSent = socket.send_to(asio::buffer(msgHeader, 32), remote_endpoint, 0);

    auto msgSent = socket.send_to(asio::buffer(message, sizeof(message)), remote_endpoint, 0);
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
        std::cout<<"s"<<std::endl;
        size_buffer.resize(32);
        socket.receive_from(asio::buffer(size_buffer), remote_endpoint, 0, error);
        int size = 0;
        try{
            size = stoi(std::string(size_buffer.data()));
        }catch(const std::invalid_argument& e){
            std::cerr << "Invalid argument(size): " << e.what() << std::endl;
            std::cout << "size is(size): " << size_buffer.size() << std::endl;
            std::cout << "data is(size): " << size_buffer.data() << std::endl;

            size = 65500;

            failedFrame = true;
        }

        if (error.value()) gui->PublishOutput(error.message(), LEV_CODE::CONN_ERROR);
        
        std::cout<<"h"<<std::endl;
        header_buffer.resize(32);
        socket.receive_from(asio::buffer(header_buffer), remote_endpoint, 0, error);
        int header = 0;
        try{
            header = stoi(std::string(header_buffer.data()));
        }catch(const std::invalid_argument& e){
            std::cerr << "Invalid argument(header): " << e.what() << std::endl;
            std::cout << "size is(header): " << header_buffer.size() << std::endl;
            std::cout << "data is(header): " << std::string(header_buffer.data()) << std::endl;
            std::cout << "data is(size): " << size << std::endl;

            header = 11;

            failedFrame = true;
        }

        if (error.value()) gui->PublishOutput(error.message(), LEV_CODE::CONN_ERROR);

        temp_buffer.resize(0);
        data_buffer.resize(0);

        std::cout<<"d"<<std::endl;
        if(isDecoding){
            socket.receive_from(asio::buffer(temp_buffer), remote_endpoint, 0, error);
        }else{
            socket.receive_from(asio::buffer(data_buffer), remote_endpoint, 0, error);
        }

        if (error.value()) gui->PublishOutput(error.message(), LEV_CODE::CONN_ERROR);

        std::cout<<std::endl;

        // std::cout << "data is(header): " << std::string(recv_buffer.data()) << std::endl;
        // std::cout << "data is(size): " << size << std::endl;        
        
        unsigned char imageData[512*512*3];

        if(!imageData){
            fprintf(stderr, "Error allocating memory for image data\n");
        }

        if(header==4 && !failedFrame && !isDecoding){
            std::cout << "image" << std::endl;
            // tjhandle decompressor = tjInitDecompress();

            // int width, height, subSamp;

            // tjDecompressHeader2(decompressor, message, size, &width, &height, &subSamp);
            // int result = tjDecompress2(decompressor, message, size, imageData, 512, 0, 512, TJPF_RGB, TJFLAG_FASTDCT);

            // if(result!=0){
            //     fprintf(stderr, "Error decompressing JPEG image: %s\n", tjGetErrorStr());
            //     delete[] imageData;
            // }

            // tjDestroy(decompressor);

            // //std::cout<<"finished decomp"<<std::endl;
            // if(i==0){
            //     printf("%s", imageData);
            //     //std::cout<<imageData<<std::endl;
            //     i++;
            // }
            //printf("%s", imageData);

            std::cout << data_buffer.empty() << std::endl;

            std::thread decodeThread([&](){
                LoadTexture(&data_buffer, 512, 512, gui->getCameraTexture());
            });
	        decodeThread.detach();

            //LoadTexture(data_buffer, 512, 512, gui->getCameraTexture());
        //     //std::cout<<"loaded texture"<<std::endl;   
        }
        temp_buffer.resize(0);
    }
}

void Connection::HandleHandshake(){
    UI* gui = UI::Get();
    ResizeBuffer(32);
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
