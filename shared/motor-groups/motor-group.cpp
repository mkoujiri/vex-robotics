#include "main.h"

#include "motor-group.hpp"

MotorGroup::MotorGroup(std::vector<pros::Motor*> motors,
					   std::vector<int> directional_speeds)
{
	/*
	   Constructor for motor group.  Takes a list
	   of motors as input.

	   Simple sets motors member variable to be used
	   in all of it's own functions.
	*/

	this->motors = motors;
	this->directional_speeds = directional_speeds;
}

MotorGroup::~MotorGroup()
{
	/*
	   Destructor for motor group.

	   Is always run at the end of the program
	   when clearing programs memory.
	*/
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

	   NOTE: This function ignores all threshholds.
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

	   NOTE: This function ignores all threshholds.
	*/

	for(int i = 0; i < motors.size(); i++)
	{
		motors[i]->move(speed);
	}
}

void MotorGroup::run(int button_one, int button_two)
{
	/*
	   A function that takes the values of two buttons and
	   controls the motors based off of the two buttons.

	   Used to control a grouping of motors bidirectionally
	   based off of buttons and modifiers. Takes into account
	   the threshholds set by user.
	*/

	std::vector<int> voltage = directional_speeds;
	if(threshhold.in_pos(get_average_position()))
	{
		voltage = threshhold.speed;
	}

	if(button_one)
	{
		for(size_t i = 0; i < motors.size(); i++)
		{
			motors[i]->move(voltage[0]);
		}
	}
	else if(button_two)
	{
		for(size_t i = 0; i < motors.size(); i++)
		{
			motors[i]->move(voltage[1]);
		}
	}
}

void MotorGroup::stop()
{
	/*
	   Stops all motors in motor group.

	   Equivalent to callings run(0) but much
	   easier to read when reading autonomous.
	*/

	for(pros::Motor* motor : motors)
	{
		motor->move(0);
	}
}

void MotorGroup::set_threshhold(int start_pos, int end_pos,
								std::vector<int> speeds)
{
	/*
	   Takes the beginning of the threshold, the end of
	   the threshhold and the rate that takes place in
	   this threshhold.

	   Used to control a segment of the position of the
	   motors' speed. Only used when controlled by via
	   buttons in the run(int, int) method.
	*/

	threshhold = PositionalSpeed{ start_pos, end_pos, speeds };
}

void MotorGroup::set_brake(BRAKE_MODE mode)
{
	/*
	   A function that takes a brake mode and sets
	   all the motors in the motor-group to use.

	   Most useful if you need motors to be held in
	   place for most accurate controls.  Also useful
	   when using PID controls to maintain accurate
	   stopping.
	*/

	for(pros::Motor* motor : motors)
	{
		motor->set_brake_mode(mode);
	}
}

unsigned int MotorGroup::get_average_position()
{
	/*
	   Returns the average encoder value of
	   all motors.

	   Useful to describe how far the motors have
	   traveled.
	*/

	unsigned int total = 0;

	for(pros::Motor* motor : motors)
	{
		total += abs(motor->get_position());
	}
	return total / motors.size();
}

void MotorGroup::clear_encoders()
{
	/*
	   A function that tares the position of
	   the motors encoders.  Allows positions to
	   be zeroed.

	   Used mostly when traveling from point a to
	   point b independently of the start position.
	*/

	for(pros::Motor* motor : motors)
	{
		motor->tare_position();
	}
}
