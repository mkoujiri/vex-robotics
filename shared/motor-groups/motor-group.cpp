#include "main.h"

#include "motor-group.hpp"

MotorGroup::MotorGroup(std::vector<pros::Motor*> motors)
{
	/*
	   Constructor for motor group.  Takes a list
	   of motors as input.

	   Simple sets motors member variable to be used
	   in all of it's own functions.
	*/
	this->motors = motors;
}

MotorGroup::~MotorGroup()
{
	/*
	   Destructor for motor group.

	   Is always run at the end of the program
	   when clearing programs memory
	*/
}

void MotorGroup::set_brake(BRAKE_MODE mode)
{
	/*
	   A function that takes a brake mode and sets
	   all the motors in the motor-group to use.

	   Most useful if you need motors to be held in
	   place for most accurate controls.  Also useful
	   when using PID controls to maintain accurate
	   stopping
	*/
	for(pros::Motor* motor : motors)
	{
		motor->set_brake_mode(mode);
	}
}

void MotorGroup::run(std::vector<int> speed)
{
	/*
	   A function that takes a list of the speeds at
	   which all the motors in the motor grop will run.

	   Most useful when writing autonomous code and
	   you must have the rates seperated.  Can also be
	   useful when using joysticks to control motors
	   seperately.
	*/
	for(int i = 0; i < motors.size(); i++)
	{
		motors[i]->move(speed[i]);
	}
}

void MotorGroup::run(int speed)
{
	/*
	   A function that takes the parameter for the speed
	   at which all the motors in the motor grop will run.

	   Most useful when using buttons and driving motors
	   the same direction at the same rate.
	*/
	for(int i = 0; i < motors.size(); i++)
	{
		motors[i]->move(speed);
	}
}
