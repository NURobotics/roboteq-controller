#ifndef ROBOTEQ_CONTROLLER_H
#define ROBOTEQ_CONTROLLER_H

#include <string>

#include <myd_serial.h>

class RoboteqController {
public:
  const static std::string kCmdHeader;
  const static std::string kQueryHeader;
  const static std::string kSetConfigHeader;
  const static std::string kGetConfigHeader;
  const static std::string kTrailer;
  const static std::string kDelimiters;

  enum EncoderMode {
    UNUSED = 0,
    COMMAND = 1,
    FEEDBACK = 2, 
  };

  enum MotorMode {
    OPEN_LOOP_SPEED = 0,
    CLOSED_LOOP_SPEED = 1,
    CLOSED_LOOP_POSITION_RELATIVE = 2,
    CLOSED_LOOP_COUNT_POSITION = 3,
    CLOSED_LOOP_POSITION_TRACKING = 4,
    TORQUE = 5,
  };
  
  RoboteqController(const std::string& port, const int baud_rate);
  ~RoboteqController() { myd_serial_.Close(); }

  bool set_echo(const bool on);

  bool set_encoder_usage(const int encoder_channel,
                         const int motor_channel,
                         const EncoderMode usage_mode);
  bool set_encoder_ppr(const int encoder_channel, const int ppr);
  bool set_encoder_home();

  bool set_amp_limit(const int motor_channel, const int amp_limit);

  bool set_proportional_gain(const int motor_channel, const int gain);
  bool set_integral_gain(const int motor_channel, const int gain);
  bool set_differential_gain(const int motor_channel, const int gain);

  bool set_motor_acceleration(const int motor_channel, const int accel);
  bool set_motor_decceleration(const int motor_channel, const int deccel);

  bool set_operating_mode(const int motor_channel, const MotorMode operating_mode);

  bool set_default_position_velocity(const int motor_channel, const int vel);
  bool set_max_rpm(const int motor_channel, const int max_rpm);

  bool send_command(const std::string &cmd) {
    return (ok_) ? myd_serial_.Write(kCmdHeader + cmd + kTrailer) : false;
  }

  bool send_query(const std::string &query, std::string *value);

  bool set_configuration(const std::string &config) {
    return (ok_) ? myd_serial_.Write(kSetConfigHeader + config + kTrailer) : false;
  }

  bool get_configuration(const std::string &name, std::string *value);

  bool is_ok() const { return ok_; };

private:
  MydSerial myd_serial_;
  std::string port_;
  int baud_rate_;
  bool ok_;

};

#endif // ROBOTEQ_CONTROLLER_H
