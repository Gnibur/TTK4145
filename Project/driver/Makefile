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


createTempDirectory :
	@mkdir -p $(TEMP_FOLDER)




$(OBJECTS): *.cpp
	$(CC) $(CFLAGS) -c -o $@ $<

$(TARGET): $(OBJECTS)
	$(CC) $(LDFLAGS) $^ -o $@




# rule for cleaning re-compilable files.

clean:
	rm -rf $(TEMP_FOLDER)/* $(TARGET)

rebuild:	clean all

.PHONY: all rebuild clean simulator



