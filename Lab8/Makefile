# Define the compiler
CC = gcc
# Define compiler flags
CFLAGS = -Wall -g
# Define linker flags
LFLAGS	 = -lpng -lm
# Define the target executable
TARGET = main
# Define source files and object files
SRCS = main.c
OBJS = $(SRCS:.c=.o)

# Default target
all: $(TARGET)

# Rule to link object files into the target executable
$(TARGET): $(OBJS)
	$(CC) $(CFLAGS) -o $(TARGET) $(OBJS) $(LFLAGS)

# Rule to compile source files into object files
%.o: %.c
	$(CC) $(CFLAGS) -c $< -o $@

# Run target to compile and execute the program with arguments
run: $(TARGET)
	./$(TARGET) $(args)
	

# Clean up build files
clean:
	rm -f $(OBJS) $(TARGET)