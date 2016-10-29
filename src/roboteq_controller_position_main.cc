#include <assert.h>
#include <stdio.h>
#include <unistd.h>

#include <string>
#include <sstream>
#include <iostream>

#include <roboteq_controller.h>

using namespace std;

const string kPort = "/dev/ttyO2";
const int kBaudrate = 115200;
const int kMotorChannel = 1;
const int kEncoderChannel = 1;

int main(int argc, char **argv){
	RoboteqController roboteq_controller = RoboteqController(kPort, kBaudrate);
  roboteq_controller.set_echo(false);
	roboteq_controller.set_encoder_high_count_limit(kEncoderChannel, 10000);
	roboteq_controller.set_encoder_low_count_limit(kEncoderChannel, -10000);
	roboteq_controller.set_encoder_usage(kEncoderChannel, kMotorChannel, RoboteqController::FEEDBACK);
	roboteq_controller.set_encoder_ppr(kEncoderChannel, 1250);
	roboteq_controller.set_amp_limit(kMotorChannel, 180);
	roboteq_controller.set_amp_trigger_level(kMotorChannel, 150);
	roboteq_controller.set_amp_trigger_delay(kMotorChannel, 1000);
	roboteq_controller.set_amp_trigger_action(kMotorChannel, RoboteqController::SAFETY_STOP);
	roboteq_controller.set_closed_loop_error_detection(kMotorChannel, RoboteqController::ERROR_AT_500MS);
	roboteq_controller.set_differential_gain(kMotorChannel, 0);
	roboteq_controller.set_integral_gain(kMotorChannel, 0);
	roboteq_controller.set_proportional_gain(kMotorChannel, 20);
	roboteq_controller.set_motor_acceleration(kMotorChannel, 170);
	roboteq_controller.set_motor_decceleration(kMotorChannel, 170);
	roboteq_controller.set_default_position_velocity(kMotorChannel, 100);
	roboteq_controller.set_max_rpm(kMotorChannel, 100);
  roboteq_controller.motor_position_mode_velocity_command(kMotorChannel, 100);
	roboteq_controller.set_operating_mode(kMotorChannel, RoboteqController::OPEN_LOOP_SPEED);
  roboteq_controller.encoder_count_command(kEncoderChannel, 0);
	roboteq_controller.set_operating_mode(kMotorChannel, RoboteqController::CLOSED_LOOP_COUNT_POSITION);

  int i;
  string encoder_count;
	while (true) {
  		cout << "Please enter the encoder count to move motor " << kMotorChannel << " to: ";
  		cin >> i;
  		if (i < 0) { break; }
      roboteq_controller.motor_absolute_position_command(1, i);
      roboteq_controller.send_query("C", &encoder_count);
      cout << "Queried the encoder counts: " << encoder_count << endl;
	}
}
