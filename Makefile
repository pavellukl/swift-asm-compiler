# binary
BIN = epicCompiler3000

# command line args
ARGS=

# directories
SRC_DIR := src
TEST_DIR := tests
BUILD_DIR := build
DEBUG_DIR := debug_$(BUILD_DIR)
DEP_DIR := .deps

# compiler
CC := gcc
CFLAGS := -std=gnu99 -Wextra -Werror -pedantic -pthread -Wall
debug_build: CFLAGS += -D DEBUG
DEPFLAGS = -MT $@ -MMD -MP -MF $(DEP_DIR)/$*.d

# files
C := $(wildcard $(SRC_DIR)/*.c)
H := $(wildcard $(SRC_DIR)/*.h)
BUILD_O := $(patsubst $(SRC_DIR)/%.c,$(BUILD_DIR)/obj/%.o,$(C))
BUILD_O_WITHOUT_MAIN := $(filter-out $(BUILD_DIR)/obj/main.o, $(BUILD_O))
DEBUG_O := $(patsubst $(SRC_DIR)/%.c,$(DEBUG_DIR)/obj/%.o,$(C))
TEST_C := $(wildcard $(TEST_DIR)/*.c)
TEST_BIN := $(patsubst $(TEST_DIR)/%.c,$(TEST_DIR)/bin/%,$(TEST_C))
DEPENDENCIES := $(C:$(SRC_DIR)/%.c=$(DEP_DIR)/%.d)

.PHONY: all build run debug debug_build debug_run test test_build test_run clean
.PRECIOUS: %/obj %/bin

# ----------------------------------------------------------------

all: build run
build: $(BUILD_DIR)/bin/$(BIN)
run:
	@ bash cRunner.sh $(BUILD_DIR)/bin/$(BIN) "$(ARGS)"

debug: debug_build debug_run
debug_build: $(DEBUG_DIR)/bin/$(BIN)
debug_run:
	@ bash cRunner.sh $(DEBUG_DIR)/bin/$(BIN) "$(ARGS)"

test: test_build test_run
test_build: build $(TEST_BIN)
test_run:
	@ for test in $(TEST_BIN) ; do ./$$test ; done

clean:
	rm -rf $(BUILD_DIR) $(DEBUG_DIR) $(TEST_DIR)/bin $(DEP_DIR)

# ----------------------------------------------------------------

$(BUILD_DIR)/bin/$(BIN): $(BUILD_O) | $(BUILD_DIR)/bin
	$(CC) $(CFLAGS) $< -o $@

$(DEBUG_DIR)/bin/$(BIN): $(DEBUG_O) | $(DEBUG_DIR)/bin
	$(CC) $(CFLAGS) $< -o $@

$(BUILD_DIR)/obj/%.o: $(SRC_DIR)/%.c
$(BUILD_DIR)/obj/%.o: $(SRC_DIR)/%.c $(DEP_DIR)/%.d | $(BUILD_DIR)/obj $(DEP_DIR)
	$(CC) $(CFLAGS) $(DEPFLAGS) -c $< -o $@

$(DEBUG_DIR)/obj/%.o: $(SRC_DIR)/%.c
$(DEBUG_DIR)/obj/%.o: $(SRC_DIR)/%.c $(DEP_DIR)/%.d | $(DEBUG_DIR)/obj $(DEP_DIR)
	$(CC) $(CFLAGS) $(DEPFLAGS) -c $< -o $@

$(TEST_DIR)/bin/%: $(TEST_C) $(BUILD_O_WITHOUT_MAIN) | $(TEST_DIR)/bin
	$(CC) $(CFLAGS) $< $(BUILD_O_WITHOUT_MAIN) -o $@ -lcriterion

$(DEP_DIR):
	@mkdir -p $@

%/bin:
	@mkdir -p $@

%/obj:
	@mkdir -p $@

$(DEPENDENCIES):
include $(wildcard $(DEPENDENCIES))
