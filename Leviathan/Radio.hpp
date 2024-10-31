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

		cout << "Start listening..." << endl;
	}
	void startRadioReceive();
	Radio(const Radio& obj) = delete;
	~Radio();
	static Radio* Get();
};
