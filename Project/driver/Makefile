# Which compiler to use
CC = clang
CCC = gcc

# Compiler flags go here.
CFLAGS = -g -Wall -std=c++11
CCFLAGS = -g -Wall -std=c99

# Linker flags go here.
LDFLAGS =  -lm -lcomedi -lstdc++ -lpthread -lrt

SOURCES = $(wildcard *.cpp)
#CSOURCES = $(wildcard *.c)

OBJECTS := $(SOURCES:%.cpp=%.o)
#COBJECTS := $(CSOURCES:%.c=%.o)

#$(COBJECTS)
all: heis BackupProcess  
		

heis: $(OBJECTS)
	$(CC) $(LDFLAGS) -o $@ $^ 

BackupProcess: BackupProcess.c
	gcc BackupProcess.c -o BackupProcess

%.o: %.cpp %.h
	$(CC) $(CFLAGS) -c $<

%.o: %.cpp 
	$(CC) $(CFLAGS) -c $< 

#%.o: %.c %.h
#	$(CCC) $(CCFLAGS) -c $<

#%.o: %.cc
#	$(CCC) $(CCFLAGS) -c $< 

clean:
	rm -rf heis BackupProcess *.o *.cpp~ *.c~ *.h~

rebuild:	clean all

.PHONY: all rebuild clean 

