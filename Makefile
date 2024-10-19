# Reference: https://makefiletutorial.com/#the-all-target

CXX 		:= g++
CXXFLAGS	:= -std=c++2a -g

TARGET_EXEC := eucleia 
SRC_DIR 	:= src

# Find all source files.
SRCS	:= $(shell find $(SRC_DIR) -name '*.cpp')

# Find all object files.
OBJS	:= $(patsubst %.cpp, %.o, $(SRCS))

# Find all directories so compiler can find headers.
INC_DIRS := $(shell find $(SRC_DIR) -type d)

# Prepend directories with '-I'.
INC_FLAGS := $(addprefix -I, $(INC_DIRS))

# Linker flags.
LDFLAGS := $(INC_FLAGS)

# Final build step. Run on all object files.
$(TARGET_EXEC): $(OBJS)
	$(CXX) $(OBJS) -o $@ $(LDFLAGS)

# Build step for cpp source files. Run on all source files.
$(OBJS): %.o: %.cpp 
	$(CXX) $(CXXFLAGS) -c $< -o $@

all: $(TARGET)

debug:
	@echo "srcs: $(SRCS)"
	@echo "objs: $(OBJS)"

.PHONY: clean 
clean: 
	rm -r $(OBJS) $(TARGET_EXEC)