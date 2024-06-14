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
	double dead = 0.05;
	const float* joys = glfwGetJoystickAxes(id, &joyCount);
	const unsigned char* buttons = glfwGetJoystickButtons(id, &buttonCount);
	const unsigned char* hats = glfwGetJoystickHats(id, &hatCount);

	controllerValues.ljoyx = deadzone(joys[0],dead);
	controllerValues.ljoyy = deadzone(-joys[1],dead);
	controllerValues.rjoyx = deadzone(joys[2],dead);
	controllerValues.rjoyy = deadzone(-joys[3],dead);

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

double Controller::deadzone(double val, double dead){
	if(val>-dead && val<dead){return 0.0;}
	return val;
}

int Controller::GetNumControllers()
{
    return numControllers;
}

Controller::~Controller()
{

}