#include "Serial.hpp"

void Serial::SendController() 
{
    Controller* controller = new Controller(1);    
	while(true){
		ControllerValues* controllerValues = controller->GetControllerValues();
		//std::cout<<controllerValues->toString().c_str()<<std::endl;
		std::string msgStr = "<" + controllerValues->toString() + ">";

		// unsigned char* msg = new unsigned char[msgStr.size() + 1];
		// std::memcpy(msg, msgStr.c_str(), msgStr.size()+1);

		// std::cout<<msg<<std::endl;

		// int err = RS232_SendBuf(cport_nr, msg, msgStr.size() + 1); // sends string on serial
		// if(err == -1){
		// 	std::cout<<"failed send" << std::endl;
		// }

		RS232_cputs(cport_nr, msgStr.c_str());

		std::this_thread::sleep_for(std::chrono::milliseconds(100));

		//printf("Sent to Arduino: '%s'\n", msgSerial);
	}
}

Serial::~Serial()
{
}

Serial* Serial::Get()
{
	if (serial == nullptr)
		serial = new Serial();

	return serial;
}
