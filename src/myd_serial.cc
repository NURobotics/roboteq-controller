#include <stdio.h>
#include <termios.h>
#include <unistd.h>
#include <errno.h>
#include <fcntl.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <string.h>
#include <stdlib.h>

#include <iostream>
#include <map>
#include <string>

#define READ_BUFFER_SIZE  50

#include <myd_serial.h>

using std::cerr;
using std::endl;

static std::map<int, int> GenerateBaudRateFlags()
{
  std::map<int, int> BaudRateFlags;
  BaudRateFlags[0] = B0;
  BaudRateFlags[50] = B50;
  BaudRateFlags[75] = B75;
  BaudRateFlags[110] = B110;
  BaudRateFlags[134] = B134;
  BaudRateFlags[150] = B150;
  BaudRateFlags[200] = B200;
  BaudRateFlags[300] = B300;
  BaudRateFlags[600] = B600;
  BaudRateFlags[1200] = B1200;
  BaudRateFlags[1800] = B1800;
  BaudRateFlags[2400] = B2400;
  BaudRateFlags[4800] = B4800;
  BaudRateFlags[9600] = B9600;
  BaudRateFlags[19200] = B19200;
  BaudRateFlags[38400] = B38400;
  BaudRateFlags[57600] = B57600;
  BaudRateFlags[115200] = B115200;
  BaudRateFlags[230400] = B230400;
  return BaudRateFlags;
}

const std::map<int, int> MydSerial::kBaudRateFlags = GenerateBaudRateFlags();

#ifdef MYD_BOARD
bool MydSerial::Open(const int fd,
                     const int baud_rate,
                     const int char_size,
                     const char parity,
                     const int stop_bits)
{
  fd_ = fd;
  
  memset(&prev_tio_, 0, sizeof(prev_tio_));
  if (!tcgetattr(fd_, &prev_tio_)) {
    cerr << "MydSerial::Open: Could not obtain previous serial port" << endl;
    return false;
  }

  memset(&current_tio_, 0, sizeof(current_tio_));

  // ignore modem control lines and enable receiver
  current_tio_.c_cflag |= CLOCAL | CREAD;
  if (char_size <= 8 && char_size >= 5) {
    static const char char_size_flags[] = {
      CS5,
      CS6,
      CS7,
      CS8,
    };

    current_tio_.c_cflag |= char_size_flags[char_size - 5];
  } else {
    current_tio_.c_cflag |= CS8;
  }

  // parity flag
  switch (parity) {
    case 'o':
    case 'O': {
      current_tio_.c_cflag |= PARENB | PARODD | INPCK | ISTRIP;
      break;
    }
    case 'e':
    case 'E': {
      current_tio_.c_cflag |= PARENB | INPCK | ISTRIP; 
      current_tio_.c_cflag &= ~PARODD;
      break;
    }
    case 'n':
    case 'N':
    default: {
      current_tio_.c_cflag &= ~PARENB; 
      break;
    }
  }

  // stop bit flag
  if (parity == 2) {
    current_tio_.c_cflag |= CSTOPB;
  } else {
    current_tio_.c_cflag &= CSTOPB;
  }

  std::map<int, int>::const_iterator baud_iter =
    MydSerial::kBaudRateFlags.find(baud_rate);
  if (kBaudRateFlags.end() != baud_iter) {
    // Found correct flag
    current_tio_.c_cflag |= baud_iter->second;
  } else {
    current_tio_.c_cflag |= B115200;
  }

  /* set timeout in deciseconds for non-canonical read */
  current_tio_.c_cc[VTIME] = 0;
  /* set minimum number of characters for non-canonical read */
  current_tio_.c_cc[VMIN] = 0;
  /* flushes data received but not read */
  tcflush(fd_, TCIFLUSH);

  /* set the parameters associated with the terminal from 
     the termios structure and the change occurs immediately */
  if((tcsetattr(fd_, TCSANOW, &current_tio_)) != 0) {
    tcsetattr(fd_, TCSANOW, &prev_tio_);
    fd_ = -1;
    cerr << "MydSerial::Open: tcsetattr could not set TCSANOW";
    return false;
  }
  
  return true;
}
#endif // MYD_BOARD

bool MydSerial::Open(const std::string& port,
                     const int baud_rate,
                     const int char_size,
                     const char parity,
                     const int stop_bits)
{
  // Open port and obtains file descriptor.
  const int fd = open(const_cast<char*>(port.c_str()), O_RDWR);
  if (fd < 0) {
    perror("MydSerial::Open: Could not open serial port");
    return false;
  }

  fd_ = fd;
  port_ = port;

#ifdef MYD_BOARD
  if (!Open(fd, baud_rate, char_size, parity, stop_bits)) {
    close(fd);
    return false;
  }
#endif // MYD_BOARD

  return true;
}

bool MydSerial::Close()
{
  if (fd_ >= 0) {
    tcsetattr(fd_, TCSANOW, &prev_tio_);
    const int close_code = close(fd_);
    fd_ = -1;
    if (close_code != 0) {
      perror("MydSerial::Close: Failed to close");
      return false;
    }
    return true;
  }
  return false;
}

bool MydSerial::Write(std::string data)
{
  if (fd_ < 0) {
    cerr << "MydSerial::Write: File descriptor not initialized";
    return false;
  }
  
  const int total_bytes = data.size();
  int written_bytes = write(fd_, const_cast<char*>(data.c_str()), total_bytes);
  int total_written = written_bytes;

  while (total_written < total_bytes && 
         (written_bytes = write(fd_,
                                const_cast<char*>(data.c_str() + total_written),
                                total_bytes - total_written)) >= 0) {
    total_written += written_bytes;
  }

  return total_bytes == total_written;
}

int MydSerial::Read(std::string *buffer)
{
  if (fd_ < 0) {
    cerr << "MydSerial::Write: File descriptor not initialized";
    return false;
  }
  
  int read_bytes = 0;
  static char temp_buffer[READ_BUFFER_SIZE+1];
  while ((read_bytes = read(fd_, temp_buffer, READ_BUFFER_SIZE)) > 0) {
    buffer->append(temp_buffer);
  }

  return read_bytes == 0; 
}

bool MydSerial::ReadUntilChar(const char delimiter, std::string *read_buffer)
{
  if (fd_ < 0) {
    cerr << "MydSerial::Write: File descriptor not initialized";
    return false;
  }
  
  static char temp_buffer[READ_BUFFER_SIZE+1];
  int read_bytes = 0;
  while (read_bytes = read(fd_, temp_buffer, 1) > 0) {
    read_buffer->append(temp_buffer);
    if (read_buffer->size() > 0 && 
        read_buffer->at(read_buffer->size()-1) == delimiter) {
      return true;
    }
  }

  return false;
}
