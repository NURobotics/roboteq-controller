#!/usr/bin/env python

import sys

import roboteq_controller

from optparse import OptionParser

if __name__ == '__main__':
  parser = OptionParser()
  parser.add_option('-p', '--port', action='store', dest='port')
  parser.add_option('-b', '--baudrate', action='store', dest='baudrate')
  parser.add_option('-b', '--baudrate', action='store', dest='baudrate')
  options, args = parser.parse_args(sys.argv)

  RC = roboteq_controller.RoboteqController(options.port, options.baudrate)
