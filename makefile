# Compiler
CC = clang

# Directories
SRC_DIR = src
INCLUDE_DIR = include
LIB_DIR = lib
BUILD_DIR = bin
ASSET_DIRS = assets

CFLAGS = -I$(INCLUDE_DIR)/

# Platform detection
ifeq ($(OS), Windows_NT)
    EXE_EXT = .exe
    LDFLAGS = -L$(LIB_DIR)/GLFW/Win -Wl,-rpath,$(LIB_DIR)/GLFW/Win -lglfw3 -lgdi32
else
    EXE_EXT =
    LDFLAGS = -L$(LIB_DIR)/GLFW/Linux -L$(LIB_DIR)/cglm/Linux -Wl,-rpath, $(LIB_DIR)/GLFW/Linux -lm -ldl -lpthread -lglfw3
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

# Rule to generate compile_commands.json
compile_commands.json:
	@echo "Generating compile_commands.json..."
	rm -f compile_commands.json
	$(foreach src, $(SRC), \
	    echo '{ "directory": "$(shell pwd)", "command": "$(CC) $(CFLAGS) -c $(src) -o $(BUILD_DIR)/$(notdir $(src:.c=.o))", "file": "$(src)" },' >> compile_commands.json;)
	@sed -i '$$ s/,$$//' compile_commands.json  # Remove trailing comma
	@echo "[" > tmp.json && cat compile_commands.json >> tmp.json && echo "]" >> tmp.json && mv tmp.json compile_commands.json

.PHONY: all clean copy_assets compile_commands.json
