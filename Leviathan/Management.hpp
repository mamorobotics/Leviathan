#include <string>

#ifndef FILE_H
#define FILE_H

static constexpr float CLEAR_COLOR = 4294967295;
static constexpr float HOME_ERROR_COLOR = 4279506175;
static constexpr float BOT_ERROR_COLOR = 4288028415;

static enum LEV_CODE { CLEAR, CONN_ERROR, TEST };

static std::string CodeDef(LEV_CODE code) {
	if (code == LEV_CODE::CLEAR)
		return "Clear";
	if (code == LEV_CODE::CONN_ERROR)
		return "Connection Error";
	if (code == LEV_CODE::TEST)
		return "Test Error";
}

static float CodeColor(LEV_CODE code) {
	if (code == LEV_CODE::CLEAR) return CLEAR_COLOR;
	if (code == LEV_CODE::CONN_ERROR) return HOME_ERROR_COLOR;
	if (code == LEV_CODE::TEST) return BOT_ERROR_COLOR;
}

#endif