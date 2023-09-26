# binary
BIN = epicCompiler3000

# command line args
ARGS=

# directories
SRC_DIR := src
TESTS_SRC_DIR := tests

BUILD_DIR := build
BUILD_O_DIR := $(BUILD_DIR)/obj
BUILD_BIN_DIR := $(BUILD_DIR)/bin
BUILD_DEPS_DIR := $(BUILD_DIR)/.dep

DEBUG_BUILD_DIR := debug_build
DEBUG_O_DIR := $(DEBUG_BUILD_DIR)/obj
DEBUG_BIN_DIR := $(DEBUG_BUILD_DIR)/bin
DEBUG_DEPS_DIR := $(DEBUG_BUILD_DIR)/.dep

TESTS_BUILD_DIR := tests_build
TESTS_O_DIR := $(TESTS_BUILD_DIR)/obj
TESTS_BIN_DIR := $(TESTS_BUILD_DIR)/bin
TESTS_DEPS_DIR := $(TESTS_BUILD_DIR)/.dep

# compiler
CC := gcc
CFLAGS := -std=gnu99 -Wextra -Werror -pedantic -pthread -Wall
debug_build: CFLAGS += -D DEBUG
test_build: CFLAGS += -lcriterion
DEPFLAGS = -MT $@ -MMD -MP -MF
BUILD_DEPFLAGS = $(DEPFLAGS) $(BUILD_DEPS_DIR)/$*.d
DEBUG_DEPFLAGS = $(DEPFLAGS) $(DEBUG_DEPS_DIR)/$*.d
TESTS_DEPFLAGS = $(DEPFLAGS) $(TESTS_DEPS_DIR)/$*.d

# files
C := $(wildcard $(SRC_DIR)/*.c)
TESTS_C := $(wildcard $(TESTS_SRC_DIR)/*.c)

BUILD_O := $(patsubst $(SRC_DIR)/%.c,$(BUILD_O_DIR)/%.o,$(C))
BUILD_O_WITHOUT_MAIN := $(filter-out $(BUILD_O_DIR)/main.o, $(BUILD_O))
BUILD_DEPS := $(C:$(SRC_DIR)/%.c=$(BUILD_DEPS_DIR)/%.d)

DEBUG_O := $(patsubst $(SRC_DIR)/%.c,$(DEBUG_O_DIR)/%.o,$(C))
DEBUG_DEPS := $(C:$(SRC_DIR)/%.c=$(DEBUG_DEPS_DIR)/%.d)

TESTS_O := $(patsubst $(TESTS_SRC_DIR)/%.c,$(TESTS_O_DIR)/%.o,$(TESTS_C))
TESTS_BIN := $(patsubst $(TESTS_SRC_DIR)/%.c,$(TESTS_BIN_DIR)/%,$(TESTS_C))
TESTS_DEPS := $(TESTS_C:$(TESTS_SRC_DIR)/%.c=$(TESTS_DEPS_DIR)/%.d)

# special
.PHONY: all build run debug debug_build debug_run test test_build test_run clean
.SECONDARY:

# ----------------------------------------------------------------

all: build run
build: $(BUILD_BIN_DIR)/$(BIN)
run:
	@ bash cRunner.sh $(BUILD_BIN_DIR)/$(BIN) "$(ARGS)"

debug: debug_build debug_run
debug_build: $(DEBUG_BIN_DIR)/$(BIN)
debug_run:
	@ bash cRunner.sh $(DEBUG_BIN_DIR)/$(BIN) "$(ARGS)"

test: test_build test_run
test_build: build $(TESTS_BIN)
test_run:
	@ for test in $(TESTS_BIN) ; do ./$$test ; done

clean:
	@ rm -rf $(BUILD_DIR) $(DEBUG_BIN_DIR) $(TESTS_BUILD_DIR)
	@ echo Clean done

# ----------------------------------------------------------------

$(BUILD_BIN_DIR)/$(BIN): $(BUILD_O) | $(BUILD_BIN_DIR)
	$(CC) $(CFLAGS) $(BUILD_O) -o $@

$(DEBUG_BIN_DIR)/$(BIN): $(DEBUG_O) | $(DEBUG_BIN_DIR)
	$(CC) $(CFLAGS) $(DEBUG_O) -o $@

$(TESTS_BIN_DIR)/%: $(TESTS_O) | $(TESTS_BIN_DIR)
	$(CC) $(CFLAGS) $(TESTS_O) $(BUILD_O_WITHOUT_MAIN) -o $@

$(BUILD_O_DIR)/%.o: $(SRC_DIR)/%.c
$(BUILD_O_DIR)/%.o: $(SRC_DIR)/%.c $(BUILD_DEPS_DIR)/%.d | $(BUILD_O_DIR) $(BUILD_DEPS_DIR)
	$(CC) $(CFLAGS) $(BUILD_DEPFLAGS) -c $< -o $@

$(DEBUG_O_DIR)/%.o: $(SRC_DIR)/%.c
$(DEBUG_O_DIR)/%.o: $(SRC_DIR)/%.c $(DEBUG_DEPS_DIR)/%.d | $(DEBUG_O_DIR) $(DEBUG_DEPS_DIR)
	$(CC) $(CFLAGS) $(DEBUG_DEPFLAGS) -c $< -o $@

$(TESTS_O_DIR)/%.o: $(TESTS_SRC_DIR)/%.c
$(TESTS_O_DIR)/%.o: $(TESTS_SRC_DIR)/%.c $(TESTS_DEPS_DIR)/%.d | $(TESTS_O_DIR) $(TESTS_DEPS_DIR)
	$(CC) $(CFLAGS) $(TESTS_DEPFLAGS) -c $< -o $@

$(BUILD_O_DIR): ; @mkdir -p $@
$(BUILD_BIN_DIR): ; @mkdir -p $@
$(BUILD_DEPS_DIR): ; @mkdir -p $@
$(DEBUG_O_DIR): ; @mkdir -p $@
$(DEBUG_BIN_DIR): ; @mkdir -p $@
$(DEBUG_DEPS_DIR): ; @mkdir -p $@
$(TESTS_O_DIR): ; @mkdir -p $@
$(TESTS_BIN_DIR): ; @mkdir -p $@
$(TESTS_DEPS_DIR): ; @mkdir -p $@

$(BUILD_DEPS):
$(DEBUG_DEPS):
$(TESTS_DEPS):
include $(wildcard $(BUILD_DEPS))
include $(wildcard $(DEBUG_DEPS))
include $(wildcard $(TESTS_DEPS))
