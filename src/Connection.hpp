#pragma once

#include "UI.hpp"

#define ASIO_STANDALONE
#include "asio.hpp"
#include "asio/ts/buffer.hpp"
#include "asio/ts/internet.hpp"

class Connection
{
public:
	static void Connect();
}

