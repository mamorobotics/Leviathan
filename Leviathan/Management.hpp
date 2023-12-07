#include <string>

#ifndef FILE_H
#define FILE_H

enum LEV_CODE { CLEAR, CONN_ERROR, TEST };

static std::string CodeDef(LEV_CODE code) {
	if (code == LEV_CODE::CLEAR)
		return "Clear";
	if (code == LEV_CODE::CONN_ERROR)
		return "Connection Error";
	if (code == LEV_CODE::TEST)
		return "Test Error";
	else
		return "NULL CODE";
}

#endif