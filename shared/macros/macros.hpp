/*
	The macros file makes code more simple to read
	by abstracting from the definitions that the
	pros api supplies.
*/

#define BRAKE_MODE pros::motor_brake_mode_e_t
#define COAST	   pros::E_MOTOR_BRAKE_COAST
#define HOLD	   pros::E_MOTOR_BRAKE_HOLD
#define BRAKE	   pros::E_MOTOR_BRAKE_BRAKE

#define UP	  pros::E_CONTROLLER_DIGITAL_UP
#define DOWN  pros::E_CONTROLLER_DIGITAL_DOWN
#define RIGHT pros::E_CONTROLLER_DIGITAL_RIGHT
#define LEFT  pros::E_CONTROLLER_DIGITAL_LEFT

#define X pros::E_CONTROLLER_DIGITAL_X
#define B pros::E_CONTROLLER_DIGITAL_B
#define Y pros::E_CONTROLLER_DIGITAL_Y
#define A pros::E_CONTROLLER_DIGITAL_A

#define L_BUMPER  pros::E_CONTROLLER_DIGITAL_L1
#define L_TRIGGER pros::E_CONTROLLER_DIGITAL_L2
#define R_BUMPER  pros::E_CONTROLLER_DIGITAL_R1
#define R_TRIGGER pros::E_CONTROLLER_DIGITAL_R2

#define L_ANALOG_X pros::E_CONTROLLER_ANALOG_LEFT_X
#define L_ANALOG_Y pros::E_CONTROLLER_ANALOG_LEFT_Y
#define R_ANALOG_X pros::E_CONTROLLER_ANALOG_RIGHT_X
#define R_ANALOG_Y pros::E_CONTROLLER_ANALOG_RIGHT_Y
