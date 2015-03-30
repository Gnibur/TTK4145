# Which compiler to use
CC = g++

# Compiler flags go here.
CFLAGS = -g -Wall -std=c++11

# Linker flags go here.
LDFLAGS =  -lm -lcomedi 

# list of sources
SOURCES = $(wildcard *.cpp)
INCLUDES = $(wildcard *.h)

TEMP_FOLDER = ./.temp

SIMULATOR_DIR = ./simulator/source


OBJECTS := $(SOURCES:%.cpp=$(TEMP_FOLDER)/%.o)

# program executable file name.
TARGET = lift


# top-level rule, to compile everything.
all: createTempDirectory $(TARGET) 

simulator: 
	dmd $(SIMULATOR_DIR)/simulator_interface.d -ofTheSimulator

$(TEMP_FOLDER)/simulation_elevator.a:
	dmd $(SIMULATOR_DIR)/simulation_elevator.d $(SIMULATOR_DIR)/timer_event.d -lib -of$(TEMP_FOLDER)/simulation_elevator

createTempDirectory :
	@mkdir -p $(TEMP_FOLDER)




$(OBJECTS): $(TEMP_FOLDER)/%.o : %.cpp 
	$(CC) $(CFLAGS) -c $^ -o $@ 

$(TARGET): $(OBJECTS) $(TEMP_FOLDER)/simulation_elevator.a
	$(CC) $(LD_FLAGS) $(TEMP_FOLDER)/simulation_elevator.a -l:libphobos2.a $^ -o $@




# rule for cleaning re-compilable files.

clean:
	rm -rf $(TEMP_FOLDER)/* $(TARGET)

rebuild:	clean all

.PHONY: all rebuild clean simulator



