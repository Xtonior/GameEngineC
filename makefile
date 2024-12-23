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

# Platform detection
ifeq ($(OS), Windows_NT)
    EXE_EXT = .exe
    LDFLAGS = -L$(LIB_DIR)/GLFW -lglfw3 -lgdi32
else
    EXE_EXT =
    LDFLAGS = -lglfw -lm -ldl -lpthread
endif

# Files
SRC = $(wildcard $(SRC_DIR)/*.c)
OBJS = $(patsubst $(SRC_DIR)/%.c, $(BUILD_DIR)/%.o, $(SRC))

# Output
TARGET = $(BUILD_DIR)/main$(EXE_EXT)

# Rules
all: $(TARGET) copy_assets

$(TARGET): $(OBJS)
	$(CC) -o $@ $^ $(LDFLAGS)

$(BUILD_DIR)/%.o: $(SRC_DIR)/%.c
	mkdir -p $(BUILD_DIR)
	$(CC) $(CFLAGS) -c $< -o $@

copy_assets:
	@$(foreach dir, $(ASSET_DIRS), \
		mkdir -p $(BUILD_DIR)/$(dir); \
		cp -r $(dir)/* $(BUILD_DIR)/$(dir);)

clean:
	rm -rf $(BUILD_DIR)

.PHONY: all clean copy_assets
