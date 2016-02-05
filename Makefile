INCLUDE_DIR=-I./include
SRCS=src/myd_serial.cc

CC=g++

myd_serial_test: 
	$(CC) $(SRCS) src/test/myd_serial_test.cc $(INCLUDE_DIR) -o bin/myd_serial_test

clean:
	rm bin/*
