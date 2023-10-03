#pragma once

#include <string>

#include "imgui.h"

class Management
{
public:
	enum LEV_CODE { CLEAR, CONN_ERROR };

	static std::string CodeDef(LEV_CODE code ) {
		if (code == LEV_CODE::CLEAR)
			return "Clear";
		if (code == LEV_CODE::CONN_ERROR)
			return "Connection Error";
	}

	static ImVec4 CodeColor(LEV_CODE code) {
		if (code == LEV_CODE::CLEAR)
			return ImVec4(1.0, 1.0, 1.0, 1.0);
		else
			return ImVec4(1.0, 0.5, 0.5, 1.0);
	}
};

