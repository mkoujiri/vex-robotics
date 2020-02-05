#ifndef MOTOR_GROUP_HPP
#define MOTOR_GROUP_HPP

struct PositionalSpeed
{
	int pos_start;
	int pos_end;
	std::vector<int> speed;

	bool in_pos(int pos)
	{
		return pos > pos_start && pos < pos_end ||
			   pos < pos_start && pos > pos_end;
	}
};

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

	// pid execution
	void set_pid_constants(double kP, double kI, double kD);
	void move_pid(int position_delta);

	// movement speeds
	void set_threshhold(int pos_start, int pos_end, std::vector<int> speed);
	void set_brake(BRAKE_MODE mode);

	// encoders
	unsigned int get_average_position();
	void clear_encoders();

	private:
	std::vector<pros::Motor*> motors;
	std::vector<int> directional_speeds;
	PositionalSpeed threshhold = { 0 };

	double kP, kI, kD;
};

#endif
