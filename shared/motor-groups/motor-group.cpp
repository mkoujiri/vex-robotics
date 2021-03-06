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
	   which all the motors in the motor group will run.

	   Most useful when writing autonomous code and
	   you must have the rates seperated.  Can also be
	   useful when using joysticks to control motors
	   seperately.

	   NOTE: This function ignores all thresholds.
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

	   NOTE: This function ignores all thresholds.
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
	   the thresholds set by user.
	*/

	std::vector<int> voltage = directional_speeds;
	if(threshold.in_pos(get_average_position()))
	{
		voltage = threshold.speed;
		std::cout << "in threshold" << std::endl;
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
	else
	{
		stop();
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

void MotorGroup::set_pid_constants(double kP, double kI, double kD)
{
	/*
	   Assigns the constants of motor group that will
	   be used in PID (proportional-integral-derivative)
	   controller.

	   kP is the proportional constants that sets the power
	   according to the distance from the target position.

	   kI is the integral scaling constant, this allows
	   us to fix steady-state error to assure we aren't
	   off by the general 15-30 degrees.

	   kD is the derivative scaling constant that slows
	   the motors prior them reaching their destination.
	*/

	this->kP = kP;
	this->kI = kI;
	this->kD = kD;
}

void MotorGroup::set_pid_turn_constants(double kP2, double kI2, double kD2)
{
	/*
	   Assigns the constants of motor group that will
	   be used in PID (proportional-integral-derivative)
	   controller.

	   kP2 is the proportional constants that sets the power
	   according to the distance from the target position.

	   kI2 is the integral scaling constant, this allows
	   us to fix steady-state error to assure we aren't
	   off by the general 15-30 degrees.

	   kD2 is the derivative scaling constant that slows
	   the motors prior them reaching their destination.
	*/

	this->kP2 = kP2;
	this->kI2 = kI2;
	this->kD2 = kD2;
}

void MotorGroup::move_pid(int position_delta, int max_speed,
						  const int error_threshold)
{
	/*
	   Moves motor group to position based off of PID control.

	   Uses PID (proportional-integral-derivative) controllers
	   to accurately and smoothly move to given position.

	   NOTE: Uses constants kP kI kD.

	   error_threshold is the accuracy goal within n degrees.
	*/

	// reset values of encoders
	clear_encoders();

	// variables used in function
	const int dT = 10;
	int error = 0;
	int prev_error = 0;
	int power;
	int integral = 0;
	int derivative;

	long long timer = 0;

	bool active = false;

	// continue looping while we aren't at end position
	while(true)
	{
		// calculate error
		if(position_delta < 0)
		{
			error = position_delta + get_average_position();
		}
		else
		{
			error = position_delta - get_average_position();
		}

		// calculate integral
		bool passed_setpoint =
			prev_error > 0 && error < 0 || prev_error < 0 && error > 0;
		// limit integral by check in desired range or passed setpoint
		integral += error;

		constexpr double integral_threshold = 500;
		if(passed_setpoint)
		{
			integral = 0;
		}
		else if(integral > integral_threshold)
		{
			integral = integral_threshold;
		}
		else if(integral < -integral_threshold)
		{
			integral = -integral_threshold;
		}

		// this block controls the completion condition
		{
			/*
			   check if delta vel is < error_threshold degrees and
			   less than margin of error away from destination.
			*/
			if(abs(error - prev_error) < error_threshold &&
			   abs(error) < error_threshold)
			{
				// activate timer and set to current time
				if(!active)
				{
					active = true;
					timer = pros::millis();
				}
				else
				{
					// check if has been stopped in time threshold (ms)
					if(pros::millis() - timer > 50)
					{
						break;
					}
				}
			}
			else
			{
				// disable timer if still moving
				active = false;
			}
		}

		// calculate derivative
		derivative = error - prev_error;
		prev_error = error;

		// execute at power
		power = error * kP + integral * kI + derivative * kD;
		if(power > max_speed)
		{
			power = max_speed;
		}
		else if(power < -max_speed)
		{
			power = -max_speed;
		}
		run(power);

		// wait for poll rate of motors
		pros::delay(dT);
	}

	stop();
}

void MotorGroup::move_pid_indices(int position_delta, std::vector<double> mod,
								  std::vector<int> read_idx, int max_speed,
								  const int error_threshold)
{
	/*
	   Moves motor group to position based off of PID control.

	   Uses PID (proportional-integral-derivative) controllers
	   to accurately and smoothly move to given position.

	   NOTE: Uses constants kP kI kD.

	   error_threshold is the accuracy goal within n degrees.

	   This variant of the move_pid function easily allows
	   collectively running part of the entire set of
	   motors (specificed by the mod parameter).  mod
	   will multiply each motor by a position so for
	   example, {1,1,0.5,0.5} would allow for a coasting
	   motion (in a 4wd train).

	   Resulting from this motion might produce error
	   in quadratic-encoder reading.  In order to combat
	   this, read_idx is available.  read_idx is a list
	   of motor indices that the reading will be averaged
	   from.
	*/

	// reset values of encoders
	clear_encoders();

	// variables used in function
	const int dT = 10;
	int error = 0;
	int prev_error = 0;
	int power;
	int integral = 0;
	int derivative;

	long long timer = 0;

	bool active = false;

	auto read_index_position = [read_idx, this]() -> int {
		size_t total = 0;
		for(size_t i = 0; i < motors.size(); i++)
		{
			if(std::find(read_idx.begin(), read_idx.end(), i) != read_idx.end())
			{
				total += abs(motors[i]->get_position());
			}
		}
		return total;
	};

	// continue looping while we aren't at end position
	while(true)
	{
		// calculate error
		if(position_delta < 0)
		{
			error = position_delta + read_index_position();
		}
		else
		{
			error = position_delta - read_index_position();
		}

		// calculate integral
		bool passed_setpoint =
			prev_error > 0 && error < 0 || prev_error < 0 && error > 0;
		// limit integral by check in desired range or passed setpoint
		integral += error;

		constexpr double integral_threshold = 500;
		if(passed_setpoint)
		{
			integral = 0;
		}
		else if(integral > integral_threshold)
		{
			integral = integral_threshold;
		}
		else if(integral < -integral_threshold)
		{
			integral = -integral_threshold;
		}

		// this block controls the completion condition
		{
			/*
			   check if delta vel is < error_threshold degrees and
			   less than margin of error away from destination.
			*/
			if(abs(error - prev_error) < error_threshold &&
			   abs(error) < error_threshold)
			{
				// activate timer and set to current time
				if(!active)
				{
					active = true;
					timer = pros::millis();
				}
				else
				{
					// check if has been stopped in time threshold (ms)
					if(pros::millis() - timer > 50)
					{
						break;
					}
				}
			}
			else
			{
				// disable timer if still moving
				active = false;
			}
		}

		// calculate derivative
		derivative = error - prev_error;
		prev_error = error;

		// execute at power
		power = error * kP + integral * kI + derivative * kD;
		if(power > max_speed)
		{
			power = max_speed;
		}
		else if(power < -max_speed)
		{
			power = -max_speed;
		}
		std::vector<int> powers;
		for(size_t i = 0; i < motors.size(); i++)
		{
			powers.push_back(power * mod[i]);
		}
		run(powers);

		// wait for poll rate of motors
		pros::delay(dT);
	}

	stop();
}

void MotorGroup::turn_pid(int position_delta, int max_speed,
						  const int error_threshold)
{
	/*
	   Moves motor group to position based off of PID control.
	   Each motor is run to turn accordingly.

	   Uses PID (proportional-integral-derivative) controllers
	   to accurately and smoothly move to given position.

	   NOTE: Uses constants kP2 kI2 kD2.

	   error_threshold is the accuracy goal within n degrees.
	*/

	// reset values of encoders
	clear_encoders();

	// variables used in function
	const int dT = 10;
	int error = 0;
	int prev_error = 0;
	int power;
	int integral = 0;
	int derivative;

	long long timer = 0;

	bool active = false;

	// continue looping while we aren't at end position
	while(true)
	{
		// calculate error
		if(position_delta < 0)
		{
			error = position_delta + get_average_position();
		}
		else
		{
			error = position_delta - get_average_position();
		}

		// calculate integral
		bool passed_setpoint =
			prev_error > 0 && error < 0 || prev_error < 0 && error > 0;
		// limit integral by check in desired range or passed setpoint
		integral += error;

		constexpr double threshold = 1000;
		if(passed_setpoint)
		{
			integral = 0;
		}
		else if(integral > threshold)
		{
			integral = threshold;
		}
		else if(integral < -threshold)
		{
			integral = -threshold;
		}

		// this block controls the completion condition
		{
			/*
			   check if delta vel is < 2 degrees and
			   less than margin of error away from destination.
			*/
			if(abs(error - prev_error) < error_threshold &&
			   abs(error) < error_threshold)
			{
				// activate timer and set to current time
				if(!active)
				{
					active = true;
					timer = pros::millis();
				}
				else
				{
					// check if has been stopped in time threshold (ms)
					if(pros::millis() - timer > 50)
					{
						break;
					}
				}
			}
			else
			{
				// disable timer if still moving
				active = false;
			}
		}

		// calculate derivative
		derivative = error - prev_error;
		prev_error = error;

		// execute at power
		power = error * kP2 + integral * kI2 + derivative * kD2;
		if(power > max_speed)
		{
			power = max_speed;
		}
		else if(power < -max_speed)
		{
			power = -max_speed;
		}
		std::vector<int> powers;
		for(size_t i = 0; i < motors.size() / 2; i++)
		{
			powers.push_back(power);
		}
		for(size_t i = motors.size() / 2; i < motors.size(); i++)
		{
			powers.push_back(-power);
		}
		run(powers);

		// wait for poll rate of motors
		pros::delay(dT);
	}

	stop();
}

void MotorGroup::set_threshold(int start_pos, int end_pos,
							   std::vector<int> speeds)
{
	/*
	   Takes the beginning of the threshold, the end of
	   the threshold and the rate that takes place in
	   this threshold.

	   Used to control a segment of the position of the
	   motors' speed. Only used when controlled by via
	   buttons in the run(int, int) method.
	*/

	threshold = PositionalSpeed{ start_pos, end_pos, speeds };
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
	   point b independent of the start position.
	*/

	for(pros::Motor* motor : motors)
	{
		motor->tare_position();
	}
}
