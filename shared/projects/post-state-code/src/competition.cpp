#include "main.h"

/*
   This file contains all match specific code

   Options are set for autonomous and this file should
   be the only difference between projects
*/

void autonomous()
{
	/*
	   Autonomous program.

	   Runs in the competition for 15 seconds.
	*/

	scooper.run(127);
	drive.move_pid(2000);
	drive.move_pid(-1100);
	scooper.stop();
	drive.turn_pid(600);
	drive.move_pid(-1800);
	drive.turn_pid(-600);
	scooper.run(127);
	drive.move_pid(2500,80);
	scooper.stop();
	drive.move_pid(-1000);
	drive.turn_pid(-1300);
	drive.move_pid(1000);
	while(ramp.get_average_position() < 3000)
	{
		ramp.run(true, false);
	}
	ramp.stop();
}

