# Compiler
CC = clang

# Directories
SRC_DIR = src
INCLUDE_DIR = include
LIB_DIR = lib
BUILD_DIR = bin

# Flags
CFLAGS = -I$(INCLUDE_DIR)/
LDFLAGS = -L$(LIB_DIR)/GLFW -lglfw3 -lgdi32 -luser32 -lkernel32 -lshell32 -lwinmm

# Files
SRC = $(wildcard $(SRC_DIR)/*.c)
OBJS = $(SRC:.c=.o)

# Output
TARGET = $(BUILD_DIR)/main.exe

# Rules
all: $(TARGET)

$(TARGET): $(OBJS)
	$(CC) -o $@ $^ $(LDFLAGS)

%.o: %.c
	$(CC) $(CFLAGS) -c $< -o $@

clean:
	rm -rf $(OBJS) $(TARGET)
