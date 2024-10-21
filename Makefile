# Reference: https://makefiletutorial.com/#the-all-target

CXX 		:= g++
CXXFLAGS	:= -std=c++2a -g -O0

# TODO: - need a release version.
TARGET_EXEC := eucleia

INSTALL_DIR	:= install
BUILD_DIR	:= build
SRC_DIR 	:= src

# Find all source files.
SRCS	:= $(shell find $(SRC_DIR) -name '*.cpp')

# Find all object files.
_OBJS	:= $(patsubst %.cpp, %.o, $(SRCS))
OBJS	:= $(addprefix $(BUILD_DIR)/, $(_OBJS))

# Find all directories so compiler can find headers.
INC_DIRS := $(shell find $(SRC_DIR) -type d)

# Prepend directories with '-I'.
INC_FLAGS := $(addprefix -I, $(INC_DIRS))

# Linker flags.
LDFLAGS := $(INC_FLAGS)

# Final build step. Run on all object files.
$(INSTALL_DIR)/$(TARGET_EXEC): $(OBJS)
	mkdir -p $(INSTALL_DIR)
	$(CXX) $(OBJS) -o $@

# Build step for cpp source files. Run on all source files.
$(BUILD_DIR)/%.o: %.cpp 
	mkdir -p $(dir $@)
	$(CXX) $(CXXFLAGS) -c $< -o $@ $(LDFLAGS)

all: $(INSTALL_DIR)/$(TARGET_EXEC)

test: $(INSTALL_DIR)/$(TARGET_EXEC)
	$(INSTALL_DIR)/$(TARGET_EXEC) --test

debug:
	@echo "srcs: $(SRCS)"
	@echo "objs: $(OBJS)"

.PHONY: clean 
clean: 
	rm -rf $(BUILD_DIR) $(INSTALL_DIR)