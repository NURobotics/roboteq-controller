#include <assert.h>
#include <stdio.h>
#include <unistd.h>

#include <string>
#include <sstream>
#include <iostream>

#include <roboteq_controller.h>

using namespace std;

const string kTestPort = "/dev/ttyO2";
const int kTestBaudrate = 115200;
const int kTestMotorChannel = 1;
const int kTestEncoderChannel = 1;

int main(int argc, char **argv){
	RoboteqController test_controller = RoboteqController(kTestPort, kTestBaudrate);
  assert(test_controller.set_echo(false));
  assert(test_controller.reset_factory_conditions());
  assert(test_controller.set_echo(false));
	assert(test_controller.set_encoder_high_count_limit(kTestEncoderChannel, 10000));
	assert(test_controller.set_encoder_low_count_limit(kTestEncoderChannel, -10000));
	assert(test_controller.set_encoder_usage(kTestEncoderChannel, kTestMotorChannel, RoboteqController::FEEDBACK));
	assert(test_controller.set_encoder_ppr(kTestEncoderChannel, 1250));
	assert(test_controller.set_amp_limit(kTestMotorChannel, 180));
	assert(test_controller.set_amp_trigger_level(kTestMotorChannel, 150));
	assert(test_controller.set_amp_trigger_delay(kTestMotorChannel, 1000));
	assert(test_controller.set_amp_trigger_action(kTestMotorChannel, RoboteqController::SAFETY_STOP));
	assert(test_controller.set_closed_loop_error_detection(kTestMotorChannel, RoboteqController::ERROR_AT_500MS));
	assert(test_controller.set_differential_gain(kTestMotorChannel, 0));
	assert(test_controller.set_integral_gain(kTestMotorChannel, 0));
	assert(test_controller.set_proportional_gain(kTestMotorChannel, 20));
	assert(test_controller.set_motor_acceleration(kTestMotorChannel, 170));
	assert(test_controller.set_motor_decceleration(kTestMotorChannel, 170));
	assert(test_controller.set_default_position_velocity(kTestMotorChannel, 100));
	assert(test_controller.set_max_rpm(kTestMotorChannel, 100));
  assert(test_controller.motor_position_mode_velocity_command(1, 100));
	assert(test_controller.set_operating_mode(kTestMotorChannel, RoboteqController::OPEN_LOOP_SPEED));
  assert(test_controller.set_encoder_count(kTestEncoderChannel, 0));
	assert(test_controller.set_operating_mode(kTestMotorChannel, RoboteqController::CLOSED_LOOP_COUNT_POSITION));
}
