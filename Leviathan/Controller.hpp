#pragma once

#include <string>
#include <vector>

#include "ControllerValues.hpp"

#include "glad/glad.h"
#include "GLFW/glfw3.h"

class Controller {
private:
    ControllerValues controllerValues;
    int id;

    int joyCount, buttonCount;

public:
	Controller(int id);
    static void ScanControllers();
    int getId() { return id; }
    void setId(int id) { this->id = id; }
    ControllerValues* GetControllerValues();
    static int GetNumControllers();
	~Controller();
};