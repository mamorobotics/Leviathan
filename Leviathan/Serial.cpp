#include "Serial.hpp"

void Serial::SendController() 
{
    Controller* controller = new Controller(1);    
	while(true){
		ControllerValues* controllerValues = controller->GetControllerValues();
		//std::cout<<controllerValues->toString().c_str()<<std::endl;
		std::string msgSerialStr = controllerValues->toString();
		const char* msgSerial = msgSerialStr.c_str();

		RS232_cputs(cport_nr, msgSerial); // sends string on serial

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
