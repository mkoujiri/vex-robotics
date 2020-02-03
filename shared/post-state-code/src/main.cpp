#include "main.h"

#include "motor-group.hpp"

pros::Controller master(pros::E_CONTROLLER_MASTER);

// left drive is normal direction
pros::Motor left_drive(1, false);
// right drive is reversed direction
pros::Motor right_drive(2, true);

// motor group based off of ports [1, 2]
MotorGroup drive({ &left_drive, &right_drive }, { 127, -127 });

// left ramp motor is normal direction
pros::Motor left_ramp(3, false);
// right ramp motor is reversed direction
pros::Motor right_ramp(4, true);

// motor group based off of ports [3, 4]
MotorGroup ramp({ &left_ramp, &right_ramp }, { 40, -60 });

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
	ramp.set_brake(BRAKE);
	ramp.set_threshhold(1500, 1600, { 30, -60 });
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
		// run drive train with joysticks
		drive.run(
			{ master.get_analog(L_ANALOG_Y), master.get_analog(R_ANALOG_Y) });

		ramp.run(master.get_digital(X), master.get_digital(B));
		pros::delay(10);
	}
}
