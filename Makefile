INCLUDE_DIR=-I./include
SRCS=src/myd_serial.cc src/roboteq_controller.cc
DEFINES=-DMYD_BOARD

CC=arm-linux-gnueabihf-g++

roboteq_controller_position_main: 
	$(CC) $(SRCS) src/roboteq_controller_position_main.cc $(INCLUDE_DIR) $(DEFINES) -o bin/roboteq_controller_position_main

roboteq_controller_test: 
	$(CC) $(SRCS) src/test/roboteq_controller_test.cc $(INCLUDE_DIR) $(DEFINES) -o bin/roboteq_controller_test

myd_serial_test: 
	$(CC) $(SRCS) src/test/myd_serial_test.cc $(INCLUDE_DIR) -o bin/myd_serial_test

clean:
	rm bin/*
	touch bin/.gitignore
