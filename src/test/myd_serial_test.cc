#include <assert.h>
#include <stdio.h>

#include <string>

#include <myd_serial.h>

using std::string;

const string kPort = "/dev/ttyACM0";
const string kTestCmd = "?F\r";
const string kTestResponse = "F=0:0\r";
const char kRoboteqDelim = '\r';

int main(int argc, char **argv)
{
  MydSerial test_port;
  string read_buffer;
  
  assert(test_port.Open(kPort, 9600, 8, 'N', 1));
  assert(test_port.Write(kTestCmd));
  assert(test_port.ReadUntilChar(kRoboteqDelim, &read_buffer));
  assert(read_buffer == kTestResponse);
  assert(test_port.Close()); 
  printf("SUCCESS\n");

  return 0;
}
