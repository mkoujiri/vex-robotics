#ifndef MOTOR_GROUP_HPP
#define MOTOR_GROUP_HPP

/*
	The PositionalSpeed struct allows for a MotorGroup
	object's speed to be controlled differently within
	certain ranges of encoder readings.
*/

struct PositionalSpeed
{
	int pos_start;
	int pos_end;
	std::vector<int> speed;

	bool in_pos(int pos)
	{
		return pos > pos_start && pos < pos_end;
	}
};

/*
	The MotorGroup class maintains a set of multiple
	pros::Motor objects.  It allows velocity based
	movement and PID movement to complete a task
	while maintaining an even distribution of
	power specificied by the user.

	Examples can include a drive train or a set
	of motors lifting an arm.  The user would want
	them to move synchronously so they can
	be placed in a MotorGroup to assure that they
	move as expected.
*/

class MotorGroup
{
	public:
	MotorGroup(std::vector<pros::Motor*> motors,
			   std::vector<int> directional_speeds);
	~MotorGroup();

	// movement execution
	void run(std::vector<int> speed);
	void run(int speed);
	void run(int button_one, int button_two);
	void stop();

	// PID execution
	void set_pid_constants(double kP, double kI, double kD);
	void set_pid_turn_constants(double kP2, double kI2, double kD2);
	void move_pid(int position_delta);
	void turn_pid(int position_delta);

	// movement speeds
	void set_threshold(int pos_start, int pos_end, std::vector<int> speed);
	void set_brake(BRAKE_MODE mode);

	// encoders
	unsigned int get_average_position();
	void clear_encoders();

	private:
	std::vector<pros::Motor*> motors;
	std::vector<int> directional_speeds;
	PositionalSpeed threshold = { 0 };

	// PID constants
	double kP, kI, kD;
	double kP2, kI2, kD2;
};

#endif
