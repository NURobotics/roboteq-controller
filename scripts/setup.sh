#!/usr/bin/env bash

ROBOTEQ_SERIAL_PORT=/dev/ttyACM0

# Turn Echo Off
python -c 'print "^ECHOF 1\r"' > ${ROBOTEQ_SERIAL_PORT}
