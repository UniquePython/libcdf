BUILD_DIR := build
BUILD_TYPE ?= Debug
GENERATOR ?= "Unix Makefiles"

.PHONY: all configure build test clean rebuild

all: build

configure:
	cmake -S . -B $(BUILD_DIR) -G $(GENERATOR) -DCMAKE_BUILD_TYPE=$(BUILD_TYPE)

build: configure
	cmake --build $(BUILD_DIR) -j

test: build
	ctest --test-dir $(BUILD_DIR) --output-on-failure

clean:
	rm -rf $(BUILD_DIR)

rebuild: clean build
