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
  const static std::string kFlashSettingHeader;
  const static std::string kTrailer;
  const static std::string kDelimiters;
  const static int kSafetyKey;

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

  enum DigitalInputAction {
     NO_ACTION = 0,
     SAFETY_STOP = 1,
     EMERGENCY_STOP = 2,
     MOTOR_STOP = 3,
     FORWARD_LIMIT = 4,
     REVERSE_LIMIT = 5,
     INVERT_DIRECTION = 6,
     RUN_MICROBASIC_SCRIPT = 7,
     LOAD_COUNTER_WITH_HOME_VALUE = 8,
  };

  enum ClosedLoopErrorDetection {
    DETECTION_DISABLED = 0,
    ERROR_AT_250MS = 1,
    ERROR_AT_500MS = 2,
    ERROR_AT_1000MS = 3,
  };
  
  RoboteqController(const std::string& port, const int baud_rate);
  ~RoboteqController() { myd_serial_.Close(); }

  bool set_echo(const bool on);

  bool set_encoder_usage(const int encoder_channel,
                         const int motor_channel,
                         const EncoderMode usage_mode);
  bool set_encoder_ppr(const int encoder_channel, const int ppr);

  bool set_amp_limit(const int motor_channel, const int amp_limit);
  bool set_amp_trigger_level(const int motor_channel, const int amp_trigger_level);
  bool set_amp_trigger_delay(const int motor_channel, const int amp_trigger_delay);
  bool set_amp_trigger_action(const int motor_channel,
                              DigitalInputAction digital_input_action);
  bool set_closed_loop_error_detection(const int motor_channel, const ClosedLoopErrorDetection clerd);

  bool set_proportional_gain(const int motor_channel, const int gain);
  bool set_integral_gain(const int motor_channel, const int gain);
  bool set_differential_gain(const int motor_channel, const int gain);

  bool set_motor_acceleration(const int motor_channel, const int accel);
  bool set_motor_decceleration(const int motor_channel, const int deccel);

  bool set_encoder_high_count_limit(const int encoder_channel, int encoder_count_limit);
  bool set_encoder_low_count_limit(const int encoder_channel, int encoder_count_limit);

  bool set_operating_mode(const int motor_channel, const MotorMode operating_mode);

  bool set_default_position_velocity(const int motor_channel, const int vel);
  bool set_max_rpm(const int motor_channel, const int max_rpm);
  bool set_motor_position_mode_velocity(const int motor_channel, const int rpm);
  bool motor_position_mode_velocity_command(const int motor_channel, const int rpm);
  bool motor_absolute_position_command(const int motor_channel, const int count);

  bool set_encoder_count(const int encoder_channel, const int encoder_count);

  bool check_valid_response() {
    if (ok_) {
      std::string response;
      if (!myd_serial_.ReadUntilChar(kDelimiters, &response)) {
        std::cerr << "RoboteqController::send_command: Failed to read" << std::endl;
        return false;
      }

      if (response.size() > 1) {
        if (response.substr(0, response.size()-1) == "+") {
          std::cerr << "RoboteqController::send_command: Gee willikers" << std::endl;
          return true;
        }
      }
    }
    return false;
  }

  bool send_command(const std::string &cmd) {
    if (ok_) {
      if (!myd_serial_.Write(kCmdHeader + cmd + kTrailer)) {
        return false;
      }

      return check_valid_response();
    }
    return false;
  }

  bool reset_factory_conditions() {
    if (ok_) {
      std::stringstream ss;
      ss << kFlashSettingHeader << "EERST " << kSafetyKey << kTrailer;
      if (!myd_serial_.Write(ss.str())) {
        return false;
      }

      return check_valid_response();
    }
  }

  bool get_echo(bool *echo);

  bool send_query(const std::string &query, std::string *value);

  bool set_configuration(const std::string &config) {
    if (ok_) {
      if (!myd_serial_.Write(kSetConfigHeader + config + kTrailer)) {
        return false;
      }

      return check_valid_response();
    }
    return false;
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
