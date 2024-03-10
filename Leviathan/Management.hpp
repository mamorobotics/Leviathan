#include <string>

#ifndef FILE_H
#define FILE_H

enum LEV_CODE { CLEAR, CONN_ERROR, TEST, IMAGE_ERROR, GENERAL_ERROR, WARNING };

static std::string CodeDef(LEV_CODE code) {
	if (code == LEV_CODE::CLEAR)
		return "Clear";
	if (code == LEV_CODE::CONN_ERROR)
		return "Connection Error";
	if (code == LEV_CODE::TEST)
		return "Test Error";
	if (code == LEV_CODE::IMAGE_ERROR)
		return "Image Error";
	if (code == LEV_CODE::GENERAL_ERROR)
		return "Error";
	if (code == LEV_CODE::WARNING)
		return "Warning";

	return "NULL CODE";
}

#endif