#pragma once

#include <string>
#include <vector>

#include "ControllerValues.hpp"

#include "glad/glad.h"
#include "GLFW/glfw3.h"

class Controller {
private:
    ControllerValues controllerValues;
	std::vector<int> controllerIds;
    int id;

    int joyCount, buttonCount;

public:
	Controller(int id);
    static void ScanControllers();
    ControllerValues GetControllerValues();
    static int GetNumControllers();
	~Controller();
};