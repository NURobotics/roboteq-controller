import io
import serial
import time

from roboteq_controller_constants import RUNTIME_COMMANDS, RUNTIME_QUERIES

class RoboteqController(object):
  
  CONTROLLER_TIMEOUT = 1
  
  def __init__(self, port='/dev/ttyACM0', baudrate=115200):
    try:
      self.serial = serial.Serial(timeout=RoboteqController.CONTROLLER_TIMEOUT)
      self.serial.port = port
      self.serial.baudrate = baudrate
      self.serial.open()
      self.serial_wrapper = io.TextIOWrapper(io.BufferedRWPair(self.serial, self.serial), newline='\r', line_buffering=True)
    except serial.SerialException as serial_error:
      print serial_error 

  def __del__(self):
    self.close()

  def close(self):
    if self.serial and self.serial.isOpen():
      self.serial.flushInput()
      self.serial.flushOutput()
      self.serial.close()
  
  def _parse_response(self, reply):
    print 'Reading %s' % str(reply)
    if type(reply) is not str:
      return None

    # This command did not have an output but is considered valid.
    if reply is '':
      return [] 

    # Replies should be in the form VALUE=data0:data1:dataN
    parts = reply.split('=')
    if len(parts) is not 2:
      return None
    
    data = parts[1].strip('\r').split(':')
    return data

  def _read_response(self):
    if self.serial:
      self.serial_wrapper.flush()
      return str(self.serial_wrapper.readline())

  def _write(self, data):
    print 'Writing %s' % data
    self.serial_wrapper.write(unicode(data))
    self.serial_wrapper.flush()

  def _build_arguments(self, mapping, inputs, arg):
    if mapping.has_key(arg):
      args = {}
      for arg_name, arg_value in inputs.iteritems():
        if arg_name in mapping[arg][0]:
          args[arg_name] = arg_value
        else:
          return
      return args

  def _build_string(self, args, arg_list):
    s = ''
    for arg_name in arg_list:
      if arg_name in args:
        s += ' %s' % str(args[arg_name])
      else:
        return
    return s

  def _runtime_handler(self, runtime, mapping, kwargs):
    # Commands must be strings.
    if type(runtime) is not str:
      return False

    runtime = runtime.upper()

    # Bind the arguments.
    args = self._build_arguments(mapping, kwargs, runtime)
    if args is None:
      return False

    # Generate the command string.
    s = self._build_string(args, mapping[runtime][0])
    if s is None:
      return False

    return s

  def runtime_query(self, query, **kwargs):
    s = self._runtime_handler(query, RUNTIME_QUERIES, kwargs)
    if s is False:
      return None

    query = '?' + query + s + '\r'
    
    # Write out the command.
    self._write(query)
    return self._parse_response(self._read_response())

  def runtime_command(self, cmd, **kwargs):
    s = self._runtime_handler(cmd, RUNTIME_COMMANDS, kwargs)
    if s is False:
      return None

    cmd = '!' + cmd + s + '\r'

    # Write out the command.
    self._write(cmd)
    return self._parse_response(self._read_response())

  def set_configuration(self, name, value):
    self._write('^%s %s\r' % (name, str(value)))

  def get_configuration(self, name):
    self._write('^%s\r' % str(name))
    return self._parse_response(self._read_response())
  
  def set_motor_angle(self, angle):
    return self.runtime_command('P', channel_position=angle)
