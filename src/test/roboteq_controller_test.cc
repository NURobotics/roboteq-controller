#include <assert.h>
#include <stdio.h>
#include <unistd.h>

#include <string>

#include <roboteq_controller.h>

using std::string;

const string kTestPort = "/dev/ttyACM0";
const int kTestBaudrate = 9600;
const int kTestMotorChannel = 1;
const int kTestEncoderChannel = 1;
const string kEchofResponse = "+";

int main(int argc, char **argv)
{
  setbuf(stdout, NULL);
  RoboteqController test_controller(kTestPort, kTestBaudrate);
  
  assert(test_controller.set_echo(true)); 
  assert(test_controller.set_encoder_usage(kTestEncoderChannel,
                                           kTestMotorChannel,
                                           RoboteqController::FEEDBACK));
  assert(test_controller.set_encoder_ppr(kTestEncoderChannel, 1250));
  assert(test_controller.set_encoder_home());
  assert(test_controller.set_amp_limit(kTestMotorChannel, 50));
  assert(test_controller.set_proportional_gain(kTestMotorChannel, 100));
  assert(test_controller.set_integral_gain(kTestMotorChannel, 100));
  assert(test_controller.set_differential_gain(kTestMotorChannel, 100));
  assert(test_controller.set_motor_acceleration(kTestMotorChannel, 100));
  assert(test_controller.set_motor_decceleration(kTestMotorChannel, 100));
  assert(test_controller.set_operating_mode(
    kTestMotorChannel,
    RoboteqController::CLOSED_LOOP_POSITION_TRACKING));
  assert(test_controller.set_default_position_velocity(kTestMotorChannel, 10));
  assert(test_controller.set_max_rpm(kTestMotorChannel, 3000));

  string value_buffer;
  assert(test_controller.get_configuration("ECHOF", &value_buffer));
  assert(value_buffer.substr(0, value_buffer.size()-1) == kEchofResponse);
  printf("SUCCESS\n");

  return 0;
}
