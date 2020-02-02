#include "main.h"

#include "motor-group.hpp"

pros::Controller master(pros::E_CONTROLLER_MASTER);

// left drive is normal direction
pros::Motor left_drive(1, false);
// right drive is reversed direction
pros::Motor right_drive(2, true);

// motor group based off of ports [1, 2]
MotorGroup drive({ &left_drive, &right_drive });

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
	drive.set_brake(BRAKE);
}

void disabled()
{
	/*
	   Executes while the program is currently disabled.

	   Useful when encoders or sensor values must be set
	*/
}

void competition_initialize()
{
	/*
	   Just like initialize but runs only in competition.

	   Useful for picking autonomous programs on the lcd.
	*/
}

void autonomous()
{
	/*
	   Autonomous program.

	   Runs in the competition for 15 seconds.
	*/
}

void opcontrol()
{
	while(true)
	{
		drive.run(
			{ master.get_analog(L_ANALOG_Y), master.get_analog(R_ANALOG_Y) });
		pros::delay(10);
	}
}
