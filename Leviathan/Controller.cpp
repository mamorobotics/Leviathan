#include "Controller.hpp"

static int numControllers;

Controller::Controller(int id)
{
    this->id = id;
}

void Controller::ScanControllers()
{
    for (int i = 0; i < 16; i++)
	{
		if (glfwJoystickPresent(i) != NULL)
		{
			numControllers += 1;
		}
	}
}

ControllerValues* Controller::GetControllerValues()
{
	const float* joys = glfwGetJoystickAxes(id, &joyCount);
	const unsigned char* buttons = glfwGetJoystickButtons(id, &buttonCount);

	controllerValues.ljoyx = joys[0];
	controllerValues.ljoyy = joys[1];
	controllerValues.rjoyx = joys[2];
	controllerValues.rjoyy = joys[3];

	controllerValues.ltrigger = joys[4];
	controllerValues.rtrigger = joys[5];

	controllerValues.a = buttons[0];
	controllerValues.b = buttons[1];
	controllerValues.x = buttons[2];
	controllerValues.y = buttons[3];

	controllerValues.rbumper = buttons[4];
	controllerValues.lbumper = buttons[5];

	return &controllerValues;
}

int Controller::GetNumControllers()
{
    return numControllers;
}

Controller::~Controller()
{

}