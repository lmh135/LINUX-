CXX = g++
CC = gcc
client1: client1.o MySocket.o spidev_test.o uart.o mylib.o gps.o dataprocess.o VLP16.o
	$(CXX) -o client1 client1.o MySocket.o spidev_test.o uart.o mylib.o gps.o dataprocess.o VLP16.o -lpthread

client1.o: client1.cpp MySocket.h imu.h uart.h Data.h gps.h mylib.h VLP16.h
	$(CXX) -c client1.cpp 

MySocket.o: MySocket.cpp MySocket.h 
	$(CXX) -c MySocket.cpp
spidev_test.o: spidev_test.c imu.h
	$(CC) -c spidev_test.c 
uart.o: uart.cpp uart.h
	$(CXX) -c uart.cpp  
mylib.o:mylib.cpp mylib.h
	$(CXX) -c mylib.cpp
gps.o:gps.c gps.h conversion.h
	$(CXX) -c gps.c	
dataprocess.o:dataprocess.cpp dataprocess.h
	$(CXX) -c dataprocess.cpp
VLP16.o:VLP16.cpp VLP16.h
	$(CXX) -c VLP16.cpp
clean:
	rm -rf *.o
	rm client1	
	
