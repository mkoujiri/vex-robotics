#include "main.h"

#include "motor-group.hpp"

/*
   The main file is where the user can define
   various pros functions and establish variables.
*/

pros::Controller master(pros::E_CONTROLLER_MASTER);

// left drive is normal direction
pros::Motor left_drive(1, false);
// right drive is reversed direction
pros::Motor right_drive(2, true);

// motor group based off of ports [1, 2]
MotorGroup drive({ &left_drive, &right_drive }, {});

// left ramp motor is normal direction
pros::Motor left_ramp(3, false);
// right ramp motor is reversed direction
pros::Motor right_ramp(4, true);

// motor group based off of ports [3, 4]
MotorGroup ramp({ &left_ramp, &right_ramp }, { 80, -60 });

// left scooper motor is normal direction
pros::Motor left_scooper(12, false);
// right scooper motor is reversed direction
pros::Motor right_scooper(13, true);

// motor group based off of ports [12, 13]
MotorGroup scooper({ &left_scooper, &right_scooper }, { 100, -40 });

// left scooper motor is normal direction
pros::Motor left_arm(5, true);
// right scooper motor is reversed direction
pros::Motor right_arm(6, false);

// motor group based off of ports [5, 6]
MotorGroup arm({ &left_arm, &right_arm }, { 60, -40 });

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
	ramp.set_brake(BRAKE);
	/*
	   slowing down the ramp towards the end,
	   this allows for easier stacking.
	*/
	arm.set_brake(BRAKE);
	ramp.set_threshold(1500, 2500, { 35, -60 });

	drive.set_pid_constants(0.20,0.05,3.0);
	drive.set_pid_turn_constants(0.1,0,0);
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
		drive.run({ master.get_analog(JOY_LY), master.get_analog(JOY_RY) });

		// control ramp based off of x and b button
		ramp.run(master.get_digital(X), master.get_digital(B));
		// control arm based off of left index finger controls
		arm.run(master.get_digital(L_BUMPER), master.get_digital(L_TRIGGER));
		// control scooper based off of right index finger controls
		scooper.run(master.get_digital(R_BUMPER),
					master.get_digital(R_TRIGGER));

		pros::delay(10);
	}
}
