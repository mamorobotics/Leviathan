#include "Serial.hpp"

void Serial::SendController() 
{
    Controller* controller = new Controller(1);    
	while(true){
		ControllerValues* controllerValues = controller->GetControllerValues();
		std::string msgStr = "<" + controllerValues->toString() + ">";

		RS232_cputs(cport_nr, msgStr); // sends string on serial
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
