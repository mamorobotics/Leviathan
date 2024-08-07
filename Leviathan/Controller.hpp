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

    int joyCount, buttonCount, hatCount;

public:
	Controller(int id);
    static void ScanControllers();
    int getId() { return id; }
    void setId(int id) { this->id = id; }
    ControllerValues* GetControllerValues();
    double deadzone(double val, double dead);
    static int GetNumControllers();
	~Controller();
};