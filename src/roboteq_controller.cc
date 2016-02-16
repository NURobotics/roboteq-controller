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
const string RoboteqController::kTrailer = "\r";
const string RoboteqController::kDelimiters = "\r\n";

RoboteqController::RoboteqController(const string& port, const int baud_rate)
{
  ok_ = myd_serial_.Open(port, baud_rate, 8, 'N', 1);    
  if (!ok_) {
    cerr << "RoboteqController::RoboteqController: Failed to open myd_serial." << endl;
    return;
  }
}

bool RoboteqController::set_echo(const bool on)
{
  string echof = "ECHOF ";
  return set_configuration(echof + ((on) ? "0" : "1"));
}

bool RoboteqController::set_encoder_usage(const int encoder_channel,
                                          const int motor_channel,
                                          const EncoderMode usage_mode)
{
  stringstream ss;
  ss << "EMOD " << encoder_channel << " " << (usage_mode + motor_channel);
  return set_configuration(ss.str());
}

bool RoboteqController::set_encoder_ppr(const int encoder_channel, const int ppr)
{
  stringstream ss;
  ss << "EPPR " << encoder_channel << " " << ppr;
  return set_configuration(ss.str());
}

bool RoboteqController::set_encoder_home()
{
  return set_configuration("EHOME");
}

bool RoboteqController::set_amp_limit(const int motor_channel, const int amp_limit)
{
  stringstream ss;
  ss << "ALIM " << motor_channel << " " << amp_limit;
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
