#include "Controller.hpp"

Controller::Controller(int id)
{
    this.id = id;
}

void Controller::ScanControllers()
{
    for (int i = 0; i < 16; i++)
	{
		if (glfwJoystickPresent(i) != NULL)
		{
			controllerIds.push_back(i);
		}
	}
}

ControllerValues GetControllerValues()
{
	//count = # of values in returned array of values
	//glfwGetJoystickAxes(id, &count)[0]
	//glfwGetJoystickButtons(id, &count)[0]
}

int Controller::GetControllerIds()
{
    return controllerIds.size();
}

Controller::~Controller()
{

}