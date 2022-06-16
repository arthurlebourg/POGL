CXX      := -c++
CXXFLAGS := -pedantic-errors -Wall -Wextra -Werror -O3
CXX_FLAGS += -m64 -march=native
CXX_FLAGS += -fopt-info-vec-optimized #-fopt-info-vec-missed -ftree-vectorize
LDFLAGS  := -L/usr/lib -lstdc++ -lm
LDXX_FLAGS = -lGL  -lGLEW -lglut -lpthread

OBJ_DIR  := ./build
BIN_DIR  := ./bin
TARGET   := portal
INCLUDE  := -Iinclude/
SRC      := $(wildcard src/*.cc)

OBJECTS  := $(SRC:%.cc=$(OBJ_DIR)/%.o)
DEPENDENCIES \
         := $(OBJECTS:.o=.d)

all: build $(BIN_DIR)/$(TARGET)

$(OBJ_DIR)/%.o: %.cc
	@mkdir -p $(@D)
	$(CXX) $(CXXFLAGS) $(INCLUDE) -c $< -MMD -o $@

$(BIN_DIR)/$(TARGET): $(OBJECTS)
	@mkdir -p $(@D)
	$(CXX) $(CXXFLAGS) -o $(BIN_DIR)/$(TARGET) $(LDXX_FLAGS) $^ $(LDFLAGS)

-include $(DEPENDENCIES)

.PHONY: all build clean debug release info

build:
	@mkdir -p $(BIN_DIR)
	@mkdir -p $(OBJ_DIR)

debug: CXXFLAGS += -DDEBUG -g
debug: all

exe: all
	./$(BIN_DIR)/$(TARGET)
	@feh scene.ppm

clean:
	-@rm -rvf $(OBJ_DIR)/*
	-@rm -rvf $(BIN_DIR)/*
