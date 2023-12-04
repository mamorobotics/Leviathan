#pragma once

#include <string>
#include <vector>

#include "glad/glad.h"
#include "GLFW/glfw3.h"

class Controller {
private:
	static std::vector<int> controllerIds;
    int id;

public:
	Controller(int id);
    static void ScanControllers();
    ControllerValues GetControllerValues();
    static int GetNumControllers();
	~Controller();
};