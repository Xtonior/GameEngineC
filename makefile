# Compiler
CC = clang

# Directories
SRC_DIR = src
INCLUDE_DIR = include
LIB_DIR = lib
BUILD_DIR = bin
ASSET_DIRS = assets

# Flags
CFLAGS = -I$(INCLUDE_DIR)/
LDFLAGS = -L$(LIB_DIR)/GLFW -lglfw3 -lgdi32 -luser32 -lkernel32 -lshell32 -lwinmm

# Files
SRC = $(wildcard $(SRC_DIR)/*.c)
OBJS = $(SRC:.c=.o)

# Output
TARGET = $(BUILD_DIR)/main.exe

# Rules
all: $(TARGET) copy_assets

$(TARGET): $(OBJS)
	$(CC) -o $@ $^ $(LDFLAGS)

%.o: %.c
	$(CC) $(CFLAGS) -c $< -o $@

copy_assets:
	@$(foreach dir, $(ASSET_DIRS), \
		mkdir -p $(BUILD_DIR)/$(dir); \
		cp -r $(dir)/* $(BUILD_DIR)/$(dir);)

clean:
	rm -rf $(OBJS) $(TARGET) $(foreach dir, $(ASSET_DIRS), $(BUILD_DIR)/$(dir))

.PHONY: all clean copy_assets
