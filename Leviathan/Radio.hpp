#pragma once
#include <iostream>
#include <tuple>
#include <sstream>
#include <thread>
#include <iostream>
#include <tuple>
#include <sstream>
#include <thread>

#include <asio.hpp>
#include <asio.hpp>
#include "UI.hpp"
#include <unistd.h>
#include <atomic>
#include <thread>
#include <chrono>

#include "Management.hpp"

#include "lib/RF24/nRF24L01.h"
//#include "lib/RF24-master/RF24.h"


class Radio
{
private:
	static Radio* radio;

	uint8_t pipeNumber;
	uint8_t payloadSize;

	std::vector<std::array<std::atomic<float>, 2>> depthProfileOne;
	std::vector<std::array<std::atomic<float>, 2>> depthProfileTwo;
	std::vector<std::string> floatOutputs;


public:
	Radio(){
		RF24 radio(PIN_CE, PIN_CSN);

		radio.begin();
		radio.setChannel(5);
		radio.setPALevel(RF24_PA_HIGH);
		radio.setDataRate(RF24_1MBPS);
		radio.enableDynamicPayloads();

		radio.openReadingPipe(0, 0x7878787878LL);

		radio.printDetails();

		radio.startListening(); 
	}
	void startRadioReceive();
	vector getDepthvals(int i){return i==1? depthProfileOne : depthProfileTwo;}
	vector getFloatOutputs(){return floatOutputs;}
	Radio(const Radio& obj) = delete;
	~Radio();
	static Radio* Get();
};
