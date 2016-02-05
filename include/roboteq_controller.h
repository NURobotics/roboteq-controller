#ifndef ROBOTEQ_CONTROLLER_H
#define ROBOTEQ_CONTROLLER_H

#include <string>

class RoboteqController {
public:
  RoboteqController(const std::string& port, const int baud_rate);
  ~RoboteqController();

private:
  std::string port_;
  int baud_rate_;
};

#endif // ROBOTEQ_CONTROLLER_H
