#include "Serial.hpp"

void Serial::SendController() 
{
    Controller* controller = new Controller(1);    
	while(true){
		ControllerValues* controllerValues = controller->GetControllerValues();
		std::string msgStr = controllerValues->toString();

		unsigned char* msg = new unsigned char[msgStr.size() + 1];
    	std::memcpy(msg, msgStr.c_str(), msgStr.size() + 1);

		int err = RS232_SendBuf(cport_nr, msg, msgStr.size() + 1); // sends string on serial
		if(err==-1){
			std::cout<<"send failed"<<std::endl;
		}

		delete[] msg;
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
