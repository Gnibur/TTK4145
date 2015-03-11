# Which compiler to use
CC = g++

# Compiler flags go here.
CFLAGS = -g -Wall

# Linker flags go here.
LDFLAGS =  -lm -lcomedi

# list of sources
SRC = *.cpp

# program executable file name.
TARGET = lift

# top-level rule, to compile everything.
all: $(TARGET)

# Define all object files.
OBJ = $(SRC:.cpp=.o)

# rule to link the program
$(TARGET): $(OBJ)
	$(CC) $(OBJ) $(LDFLAGS) $^ -o $@

# Compile: create object files from C source files.
%.o : %.cpp
	$(CC) $(CFLAGS) -c $< -o $@ 



# rule for cleaning re-compilable files.
clean:
	rm -f $(TARGET) $(ELEVOBJ)

rebuild:	clean all

.PHONY: all rebuild clean



