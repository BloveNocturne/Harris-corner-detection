CC = g++ -std=c++11

TARGET = test

SRCS = harris.cpp

$(TARGET):$(SRCS)
	$(CC) $^ `pkg-config opencv --cflags --libs` -o $@ 
