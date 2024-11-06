#pragma once
#include <iostream>
#include <tuple>
#include <sstream>
#include <thread>

#include <asio.hpp>
#include "UI.hpp"
#include <unistd.h>
#include <atomic>
#include <chrono>

#include "Management.hpp"

#include <RF24/RF24.h>

using namespace std;


class Radio
{
private:
	int index = 0;

	uint8_t pipeNumber = 0;
	uint8_t payloadSize = 0;

	std::array<std::array<std::atomic<float>, 128>, 2> depthProfileOne;
	std::array<std::array<std::atomic<float>, 128>, 2> depthProfileTwo;
	std::vector<std::string> floatOutputs;
	
	static Radio* radio;

public:
	Radio();
	void startRadioReceive();
	//std::array<std::array<std::atomic<float>, 128>, 2> getDepthVals(int i){return i==1? depthProfileOne : depthProfileTwo;}
	std::vector<std::string> getFloatOutputs(){return floatOutputs;}
	Radio(const Radio& obj) = delete;
	~Radio();
	static Radio* Get();
};
