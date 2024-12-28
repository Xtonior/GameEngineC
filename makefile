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
    MKDIR = if not exist $(subst /,\,$(1)) mkdir $(subst /,\,$(1))
    COPY = xcopy /E /I /Q $(subst /,\,$(1)) $(subst /,\,$(2))
    RM = rmdir /S /Q $(subst /,\,$(1))
    LDFLAGS = -L$(LIB_DIR)/GLFW/Win -Wl,$(LIB_DIR)/GLFW/Win -lglfw3 -lgdi32
else
    EXE_EXT =
    MKDIR = mkdir -p $(1)
    COPY = cp -r $(1)/* $(2)/
    RM = rm -rf $(1)
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
	$(call MKDIR,$(BUILD_DIR))
	$(CC) $(CFLAGS) -c $< -o $@

copy_assets:
	$(call MKDIR,$(BUILD_DIR)/assets)
	$(call COPY,assets,$(BUILD_DIR)/assets)

clean:
	$(call RM,$(BUILD_DIR))

# Rule to generate compile_commands.json
compile_commands.json:
	@echo "Generating compile_commands.json..."
	rm -f compile_commands.json
	$(foreach src, $(SRC), \
	    echo '{ "directory": "$(shell pwd)", "command": "$(CC) $(CFLAGS) -c $(src) -o $(BUILD_DIR)/$(notdir $(src:.c=.o))", "file": "$(src)" },' >> compile_commands.json;)
	@sed -i '$$ s/,$$//' compile_commands.json  # Remove trailing comma
	@echo "[" > tmp.json && cat compile_commands.json >> tmp.json && echo "]" >> tmp.json && mv tmp.json compile_commands.json

.PHONY: all clean copy_assets compile_commands.json