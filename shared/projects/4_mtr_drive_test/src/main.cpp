#include "main.h"

#include "motor-group.hpp"

/*
   The main file is where the user can define
   various pros functions and establish variables.
*/

pros::Controller master(pros::E_CONTROLLER_MASTER);

pros::Motor left_back_drive(1, true);
pros::Motor left_front_drive(2, true);

pros::Motor right_back_drive(3, false);
pros::Motor right_front_drive(4, false);

// motor group based off of ports [1, 2, 3, 4]
MotorGroup drive({ &left_back_drive, &left_front_drive, 
				   &right_back_drive, &right_front_drive }, {});

void initialize()
{
	/*
	   Initializes when program is started.

	   Useful to set variables and assure that something is
	   called before anything else.
	*/

	// initializes hardware
	pros::lcd::initialize();

	// user initialization
}

void competition_initialize()
{
	/*
	   Just like initialize but runs only in competition.

	   Useful for picking autonomous programs on the lcd.
	*/
}

void disabled()
{
	/*
	   Executes while the program is currently disabled.

	   Useful when encoders or sensor values must be set
	*/
}

void opcontrol()
{
	while(true)
	{
		// control drive train with joysticks
		drive.run({ master.get_analog(JOY_LY), master.get_analog(JOY_LY),
					master.get_analog(JOY_RY), master.get_analog(JOY_RY)});

		pros::delay(10);
	}
}
