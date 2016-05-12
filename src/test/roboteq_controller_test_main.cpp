#include <assert.h>
#include <stdio.h>
#include <unistd.h>

#include <string>
#include <sstream>
#include <iostream>

#include <roboteq_controller.h>

using namespace std;

// set the configurations
// make a loop that take in different positions & moves the arm & checks the position

const string kTestPort = "/dev/ttyACM0";
const int kTestBaudrate = 9600;
const int kTestMotorChannel = 1;
const int kTestEncoderChannel = 1;
const string kEchofResponse = "+";
bool done = false;

int main(int argc, char **argv){

	RoboteqController test_controller = RoboteqController(kTestPort, kTestBaudrate);

	//set the configurations
	test_controller.set_echo(false);
	test_controller.set_encoder_usage(kTestEncoderChannel, kTestMotorChannel, RoboteqController::FEEDBACK);
	test_controller.set_encoder_ppr(kTestEncoderChannel, 1250);
	test_controller.set_encoder_home();
	test_controller.set_amp_limit(kTestMotorChannel, 5);
	test_controller.set_differential_gain(kTestMotorChannel, 0);
	test_controller.set_integral_gain(kTestMotorChannel, 10);
	test_controller.set_proportional_gain(kTestMotorChannel, 10);
	test_controller.set_motor_acceleration(kTestMotorChannel, 0.1);
	test_controller.set_motor_decceleration(kTestMotorChannel, 0.1);
	test_controller.set_operating_mode(kTestMotorChannel, RoboteqController::CLOSED_LOOP_POSITION_TRACKING);
	test_controller.set_default_position_velocity(kTestMotorChannel, 10);
	test_controller.set_max_rpm(kTestMotorChannel, 3000);

	//enter loop which takes in position and sets arm's position there
	while(!done) {
		int i;
  		cout << "Please enter an integer value: ";
  		cin >> i;
  		if (i < 0) {done = true; break;}
  		stringstream ss;
  		ss << i;
  		string cmd = "P 1 " + ss.str();
  		test_controller.send_command(cmd);
  		string buf;
  		test_controller.send_query("C 1", &buf); //change the send_query method so that it actually returns something (instead of a boolean)
  		int pos;
  		ss << buf;
  		ss >> pos;
  		if (pos == i) {cout << "success!" <<endl ;}

	}


}