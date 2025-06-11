# Compiler and flags
CC = gcc
CFLAGS = -Wall -Wextra -std=c99

# Targets
TARGETS = shm_write shm_read

# Default target
.PHONY: all
all: $(TARGETS)

# Build shm_write
shm_write: shm_write.c
    $(CC) $(CFLAGS) -o shm_write shm_write.c

# Build shm_read
shm_read: shm_read.c
    $(CC) $(CFLAGS) -o shm_read shm_read.c

# Run tests
.PHONY: test
test: all
    ./test.sh

# Clean up generated files
.PHONY: clean
clean:
    rm -f $(TARGETS)

# Phony target declaration
.PHONY: phony
phony: all clean test