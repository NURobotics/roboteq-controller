INCLUDE_DIR=-I./include
SRCS=src/myd_serial.cc src/roboteq_controller.cc

CC=g++

myd_serial_test: 
	$(CC) $(SRCS) src/test/myd_serial_test.cc $(INCLUDE_DIR) -o bin/myd_serial_test
	$(CC) $(SRCS) src/test/roboteq_controller_test.cc $(INCLUDE_DIR) -o bin/roboteq_controller_test

clean:
	rm bin/*
