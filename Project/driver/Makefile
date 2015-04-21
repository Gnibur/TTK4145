# Which compiler to use
CC = g++

# Compiler flags go here.
CFLAGS = -g -Wall -std=c++11

# Linker flags go here.
LDFLAGS =  -lm  

SOURCES = $(wildcard *.cpp)

OBJECTS := $(SOURCES:%.cpp=%.o)


all: $(OBJECTS)
	$(CC) $(LDFLAGS) -o $@ $^ 

%.o: %.cpp %.h
	$(CC) $(CFLAGS) -c $<

%.o: %.cpp 
	$(CC) $(CFLAGS) -c $< 

clean:
	rm -r *.o $(TARGET)

rebuild:	clean all

.PHONY: all rebuild clean 


