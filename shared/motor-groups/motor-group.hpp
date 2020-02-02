#ifndef MOTOR_GROUP_HPP
#define MOTOR_GROUP_HPP

class MotorGroup
{
	public:
	MotorGroup(std::vector<pros::Motor*> motors);
	~MotorGroup();

	void set_brake(BRAKE_MODE mode);

	void run(std::vector<int> speed);
	void run(int speed);

	private:
	std::vector<pros::Motor*> motors;
};

#endif
