#ifndef MYD_SERIAL_H
#define MYD_SERIAL_H

#include <termios.h>

#include <map>
#include <string>

class MydSerial {
public:
  static const std::map<int, tcflag_t> kBaudRateFlags;

  MydSerial() : fd_(-1) {}

#ifdef MYD_BOARD
  bool Open(const int fd,
            const int baud_rate,
            const int char_size,
            const char parity,
            const int stop_bits);
#endif // MYD_BOARD
  
  bool Open(const std::string& port,
            const int baud_rate,
            const int char_size,
            const char parity,
            const int stop_bits);
  
  bool Close();

  bool Write(const std::string &data);
  int Read(std::string *buffer);
  bool ReadUntilChar(const std::string &delimiters, std::string *buffer);

private:
  std::string port_;
  int fd_;
  int char_size_;
  int parity_;
  int stop_bits_;

  struct termios prev_tio_;
  struct termios current_tio_;
};

#endif // MYD_SERIAL_H
