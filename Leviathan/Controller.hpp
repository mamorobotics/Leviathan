#pragma once

#include <string>
#include <vector>

#include "ControllerValues.hpp"

#include "glad/glad.h"
#include "GLFW/glfw3.h"

/**
 * @class Controller
 * @brief Handles the representation of a controller.
 */
class Controller {
private:
    ControllerValues controllerValues;
    int id;

    int joyCount, buttonCount, hatCount;

public:
	Controller(int id);
    /**
     * @brief Scans for all connected controllers. MUST BE CALLED BEFORE CONTROLLER ASSIGNMENT FOR CONTROLLER TO WORK.
     */
    static void ScanControllers();
    /**
     * @brief Returns the currently selected controller ID.
     * @return int according to above.
     */
    int getId() { return id; }
    /**
     * @brief Sets the currently selected controller ID.
     * @param id currently used controller.
     */
    void setId(int id) { this->id = id; }
    /**
     * @brief Returns the ControllerValues representation of the values of the currently selected controller.
     */
    ControllerValues* GetControllerValues();
    /**
     * @brief Simulates a controller deadzone.
     * @param val the current controller value to simulate on.
     * @param dead the zone in which to simulate a deadzone.
     * @return the simulated deadzone value.
     */
    double deadzone(double val, double dead);
    /**
     * @brief Returns the current number of connected controllers. Only works if ScanControllers has previously been called.
     * @return int according to above definition.
     */
    static int GetNumControllers();
	~Controller();
};