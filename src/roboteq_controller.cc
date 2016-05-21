#include <iostream>
#include <string>
#include <sstream>

#include <myd_serial.h>
#include <roboteq_controller.h>

using std::stringstream;
using std::string;
using std::cout;
using std::cerr;
using std::endl;

const string RoboteqController::kCmdHeader = "!";
const string RoboteqController::kQueryHeader = "?";
const string RoboteqController::kSetConfigHeader = "^";
const string RoboteqController::kGetConfigHeader = "~";
const string RoboteqController::kFlashSettingHeader = "%";
const string RoboteqController::kTrailer = "\r\n";
const string RoboteqController::kDelimiters = "\r\n";
const int RoboteqController::kSafetyKey = 321654987;

RoboteqController::RoboteqController(const string& port, const int baud_rate)
{
  ok_ = myd_serial_.Open(port, baud_rate, 8, 'N', 1);    
  if (!ok_) {
    cerr << "RoboteqController::RoboteqController: Failed to open myd_serial." << endl;
    return;
  }
}

bool RoboteqController::get_echo(bool *echo)
{
  string echof_setting;
  if (!get_configuration("ECHOF", &echof_setting)) {
    cerr << "RoboteqController::get_echo: Failed to get echo" << endl;
    return false;
  }

  if (echof_setting.substr(0, echof_setting.size()-1) == "ECHOF=0") {
    *echo = true;
  } else if (echof_setting.substr(0, echof_setting.size()-1) == "ECHOF=1") {
    *echo = false;
  } else if (echof_setting.substr(0, echof_setting.size()-1) == "~ECHOF") {
    if (!myd_serial_.ReadUntilChar(kDelimiters, &echof_setting)) {
      return false;
    }

    if (echof_setting.substr(0, echof_setting.size()-1) == "ECHOF=0") {
      *echo = true;
    } else if (echof_setting.substr(0, echof_setting.size()-1) == "ECHOF=1") {
      *echo = false;
    } else {
      return false;
    }
  }
  return true;
}

bool RoboteqController::set_echo(const bool on)
{
  bool echo = true;
  if (!get_echo(&echo)) {
    cerr << "RoboteqController::set_echo: Failed to get echo" << endl;
    return false;
  }

  if (echo) {
    stringstream ss;
    ss << kSetConfigHeader << "ECHOF 1";
    const string echo_config = ss.str();
    ss << kTrailer;
    if (!myd_serial_.Write(ss.str())) {
      return false;
    }

    string echof_response;
    if (!myd_serial_.ReadUntilChar(kDelimiters, &echof_response)) {
      return false;
    }

    if (echof_response.substr(0, echof_response.size()-1) != echo_config) {
      return false;
    }
  }
  return true;
}

bool RoboteqController::set_encoder_usage(const int encoder_channel,
                                          const int motor_channel,
                                          const EncoderMode usage_mode)
{
  stringstream ss;
  const int value = ((motor_channel == 1) ? 16 : 32) + ((int)usage_mode);
  ss << "EMOD " << encoder_channel << " " << value;
  return set_configuration(ss.str());
}

bool RoboteqController::set_encoder_ppr(const int encoder_channel, const int ppr)
{
  stringstream ss;
  ss << "EPPR " << encoder_channel << " " << ppr;
  return set_configuration(ss.str());
}

bool RoboteqController::set_amp_limit(const int motor_channel, const int amp_limit)
{
  stringstream ss;
  ss << "ALIM " << motor_channel << " " << amp_limit;
  return set_configuration(ss.str());
}

bool RoboteqController::set_amp_trigger_level(const int motor_channel, const int amp_trigger_level)
{
  stringstream ss;
  ss << "ATRIG " << motor_channel << " " << amp_trigger_level;
  return set_configuration(ss.str());
}

bool RoboteqController::set_amp_trigger_delay(const int motor_channel, const int amp_trigger_delay)
{
  stringstream ss;
  ss << "ATGD " << motor_channel << " " << amp_trigger_delay;
  return set_configuration(ss.str());
}

bool RoboteqController::set_amp_trigger_action(
  const int motor_channel,
  const DigitalInputAction digital_input_action)
{
  stringstream ss;
  const int trigger_action = ((motor_channel == 1) ? 16 : 32) + ((int)digital_input_action);
  ss << "ATGA " << motor_channel << " " << trigger_action;
  return set_configuration(ss.str());
}

bool RoboteqController::set_closed_loop_error_detection(const int motor_channel, const ClosedLoopErrorDetection clerd)
{
  stringstream ss;
  ss << "CLERD " << motor_channel << " " << clerd;
  return set_configuration(ss.str());
}

bool RoboteqController::set_proportional_gain(const int motor_channel, const int gain)
{
  stringstream ss;
  ss << "KP " << motor_channel << " " << gain;
  return set_configuration(ss.str());
}

bool RoboteqController::set_integral_gain(const int motor_channel, const int gain)
{
  stringstream ss;
  ss << "KI " << motor_channel << " " << gain;
  return set_configuration(ss.str());
}

bool RoboteqController::set_differential_gain(const int motor_channel, const int gain)
{
  stringstream ss;
  ss << "KD " << motor_channel << " " << gain;
  return set_configuration(ss.str());
}

bool RoboteqController::set_motor_acceleration(const int motor_channel, const int accel)
{
  stringstream ss;
  ss << "MAC " << motor_channel << " " << accel;
  return set_configuration(ss.str());
}

bool RoboteqController::set_motor_decceleration(const int motor_channel, const int deccel)
{
  stringstream ss;
  ss << "MDEC " << motor_channel << " " << deccel;
  return set_configuration(ss.str());
}

bool RoboteqController::set_encoder_high_count_limit(const int encoder_channel, int encoder_count_limit)
{
  stringstream ss;
  ss << "EHL " << encoder_channel << " " << encoder_count_limit;
  return set_configuration(ss.str());
}

bool RoboteqController::set_encoder_low_count_limit(const int encoder_channel, int encoder_count_limit)
{
  stringstream ss;
  ss << "ELL " << encoder_channel << " " << encoder_count_limit;
  return set_configuration(ss.str());
}

bool RoboteqController::set_operating_mode(const int motor_channel,
                                           const MotorMode operating_mode)
{
  stringstream ss;
  ss << "MMOD " << motor_channel << " " << operating_mode;
  return set_configuration(ss.str());
}

bool RoboteqController::set_default_position_velocity(const int motor_channel,
                                                      const int vel)
{
  stringstream ss;
  ss << "MVEL " << motor_channel << " " << vel;
  return set_configuration(ss.str());
}

bool RoboteqController::set_max_rpm(const int motor_channel, const int max_rpm)
{
  stringstream ss;
  ss << "MXRPM " << motor_channel << " " << max_rpm;
  return set_configuration(ss.str());
}

bool RoboteqController::get_configuration(const string &name, std::string *value)
{
  if (!myd_serial_.Write(kGetConfigHeader + name + kTrailer)) {
    cerr << "RoboteqController::get_configuration: Failed to write" << endl;
    return false;
  }

  value->clear();
  if (!myd_serial_.ReadUntilChar(kDelimiters, value)) {
    cerr << "RoboteqController::get_configuration: Couldn't read until delimiters" << endl;
    return false;
  }

  return true;
}

bool RoboteqController::set_encoder_count(const int encoder_channel, const int encoder_count)
{
  stringstream ss;
  ss << "C " << encoder_channel << " " << encoder_count;
  return send_command(ss.str());
}

bool RoboteqController::motor_position_mode_velocity_command(const int motor_channel, const int rpm)
{
  stringstream ss;
  ss << "S " << motor_channel << " " << rpm;
  return send_command(ss.str());
}

bool RoboteqController::motor_absolute_position_command(const int motor_channel, const int count)
{
  stringstream ss;
  ss << "P " << motor_channel << " " << count;
  return send_command(ss.str());
}

bool RoboteqController::send_query(const std::string &query, std::string *value)
{
  if (!myd_serial_.Write(kQueryHeader + query + kTrailer)) {
    cerr << "RoboteqController::send_query: Failed to write" << endl;
    return false;
  }

  value->clear();
  if (!myd_serial_.ReadUntilChar(kDelimiters, value)) {
    cerr << "RoboteqController::get_configuration: Couldn't read until delimiters" << endl;
    return false;
  }

  return true;
}
