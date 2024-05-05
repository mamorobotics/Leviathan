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
	const unsigned char* hats = glfwGetJoystickHats(id, &hatCount);

	controllerValues.ljoyx = joys[0];
	controllerValues.ljoyy = -joys[1];
	controllerValues.rjoyx = joys[2];
	controllerValues.rjoyy = -joys[3];

	controllerValues.ltrigger = buttons[6];
	controllerValues.rtrigger = buttons[7];

	controllerValues.a = buttons[1];
	controllerValues.b = buttons[2];
	controllerValues.x = buttons[0];
	controllerValues.y = buttons[3];

	controllerValues.rbumper = buttons[5];
	controllerValues.lbumper = buttons[4];

	controllerValues.up = buttons[12];
	controllerValues.right = buttons[13];
	controllerValues.down = buttons[14];
	controllerValues.left = buttons[15];

	return &controllerValues;
}

int Controller::GetNumControllers()
{
    return numControllers;
}

Controller::~Controller()
{

}