# binary
BIN = epicCompiler3000

# command line args
ARGS=

# directories
SRC_BASE_DIR := src
TESTS_SRC_BASE_DIR := tests

BUILD_BASE_DIR := _build
BUILD_O_BASE_DIR := $(BUILD_BASE_DIR)/obj
BUILD_BIN_BASE_DIR := $(BUILD_BASE_DIR)/bin
BUILD_DEPS_BASE_DIR := $(BUILD_BASE_DIR)/.dep

DEBUG_BUILD_BASE_DIR := _debug_build
DEBUG_O_BASE_DIR := $(DEBUG_BUILD_BASE_DIR)/obj
DEBUG_BIN_BASE_DIR := $(DEBUG_BUILD_BASE_DIR)/bin
DEBUG_DEPS_BASE_DIR := $(DEBUG_BUILD_BASE_DIR)/.dep

TESTS_BUILD_BASE_DIR := _tests_build
TESTS_O_BASE_DIR := $(TESTS_BUILD_BASE_DIR)/obj
TESTS_BIN_BASE_DIR := $(TESTS_BUILD_BASE_DIR)/bin
TESTS_DEPS_BASE_DIR := $(TESTS_BUILD_BASE_DIR)/.dep

DEPLOY_BASE_DIR := _deploy

# compiler
CC := gcc
CFLAGS := -std=gnu99 -Wextra -Werror -pedantic -pthread -Wall
build: CFLAGS += -D NDEBUG
debug_build: CFLAGS += -g -D DEBUG
test_build: CFLAGS += -lcriterion
DEPFLAGS = -MT $@ -MMD -MP -MF
BUILD_DEPFLAGS = $(DEPFLAGS) $(BUILD_DEPS_BASE_DIR)/$*.d
DEBUG_DEPFLAGS = $(DEPFLAGS) $(DEBUG_DEPS_BASE_DIR)/$*.d
TESTS_DEPFLAGS = $(DEPFLAGS) $(TESTS_DEPS_BASE_DIR)/$*.d

# custom functions
uniq = $(if $1,$(firstword $1) $(call uniq,$(filter-out $(firstword $1),$1)))

# files
C := $(shell find "$(SRC_BASE_DIR)" -name "*.c")
H := $(shell find "$(SRC_BASE_DIR)" -name "*.h")
TESTS_C := $(shell find "$(TESTS_SRC_BASE_DIR)" -name "*.c")

BUILD_O := $(patsubst $(SRC_BASE_DIR)/%.c,$(BUILD_O_BASE_DIR)/%.o,$(C))
BUILD_O_DIRS := $(call uniq,$(dir $(BUILD_O)))
BUILD_O_WITHOUT_MAIN := $(filter-out $(BUILD_O_BASE_DIR)/main.o, $(BUILD_O))
BUILD_DEPS := $(C:$(SRC_BASE_DIR)/%.c=$(BUILD_DEPS_BASE_DIR)/%.d)
BUILD_DEPS_DIRS := $(call uniq,$(dir $(BUILD_DEPS)))

DEBUG_O := $(patsubst $(SRC_BASE_DIR)/%.c,$(DEBUG_O_BASE_DIR)/%.o,$(C))
DEBUG_O_DIRS := $(call uniq,$(dir $(DEBUG_O)))
DEBUG_DEPS := $(C:$(SRC_BASE_DIR)/%.c=$(DEBUG_DEPS_BASE_DIR)/%.d)
DEBUG_DEPS_DIRS := $(call uniq,$(dir $(DEBUG_DEPS)))

TESTS_O := $(patsubst $(TESTS_SRC_BASE_DIR)/%.c,$(TESTS_O_BASE_DIR)/%.o,$(TESTS_C))
TESTS_O_DIRS := $(call uniq,$(dir $(TESTS_O)))
TESTS_BIN := $(patsubst $(TESTS_SRC_BASE_DIR)/%.c,$(TESTS_BIN_BASE_DIR)/%,$(TESTS_C))
TESTS_BIN_DIRS := $(call uniq,$(dir $(TESTS_BIN)))
TESTS_DEPS := $(TESTS_C:$(TESTS_SRC_BASE_DIR)/%.c=$(TESTS_DEPS_BASE_DIR)/%.d)
TESTS_DEPS_DIRS := $(call uniq,$(dir $(TESTS_DEPS)))

# clean
MKFILE_PATH := $(abspath $(lastword $(MAKEFILE_LIST)))
MKFILE_DIR := $(MKFILE_PATH:/Makefile=)

#	all files and directories ever created (to remove empty folders)
CLEAN_FS_NODES := $(BUILD_BASE_DIR) $(DEBUG_BUILD_BASE_DIR) $(TESTS_BUILD_BASE_DIR) $(DEPLOY_BASE_DIR) \
				  $(BUILD_O_BASE_DIR) $(BUILD_BIN_BASE_DIR) $(BUILD_DEPS_BASE_DIR) \
				  $(DEBUG_O_BASE_DIR) $(DEBUG_BIN_BASE_DIR) $(DEBUG_DEPS_BASE_DIR) \
				  $(TESTS_O_BASE_DIR) $(TESTS_BIN_BASE_DIR) $(TESTS_DEPS_BASE_DIR)
CLEAN_FS_NODES := $(realpath $(CLEAN_FS_NODES))

#	filters out the Makefiles directory and its parent directories (to preserve Makefiles directory)
CLEAN_FS_NODES := $(filter $(MKFILE_DIR)%,$(CLEAN_FS_NODES))

#	adds all files ever created to make sure even those files which are in the Makefiles directory
#   or parent directories are deleted (to delete all files)
CLEAN_FS_NODES := $(BUILD_O) $(DEBUG_O) $(TESTS_O) \
			  	  $(BUILD_DEPS) $(DEBUG_DEPS) $(TESTS_DEPS) \
			  	  $(TESTS_BIN) $(BUILD_BIN_BASE_DIR)/$(BIN) $(DEBUG_BIN_BASE_DIR)/$(BIN) \
				  xluklp00.zip \
				  $(CLEAN_FS_NODES)

# special
.PHONY: build run debug debug_build debug_run test test_build test_run clean deploy
.SECONDARY:
.SECONDEXPANSION:

# ----------------------------------------------------------------

build: $(BUILD_BIN_BASE_DIR)/$(BIN)
run:
	@ bash binRunner.sh $(BUILD_BIN_BASE_DIR)/$(BIN) "$(ARGS)"

debug: debug_build debug_run
debug_build: $(DEBUG_BIN_BASE_DIR)/$(BIN)
debug_run:
	@ bash binRunner.sh $(DEBUG_BIN_BASE_DIR)/$(BIN) "$(ARGS)"

test: test_build test_run
test_build: build $(TESTS_BIN)
test_run:
	@ for test in $(TESTS_BIN) ; do ./$$test --verbose=1; echo ""; done

deploy: | $(DEPLOY_BASE_DIR).dir
	@ rm -rf $(DEPLOY_BASE_DIR)/*
	@ for file in $(C) ; do basename $$file | ( read filename; cat $$file | sed -E 's,#include *"([^\/"]+\/)*([^"]+)",#include "\2",g' > $(DEPLOY_BASE_DIR)/$$filename ) ; done
	@ for file in $(H) ; do basename $$file | ( read filename; cat $$file | sed -E 's,#include *"([^\/"]+\/)*([^"]+)",#include "\2",g' > $(DEPLOY_BASE_DIR)/$$filename ) ; done
	@ cp deployMakefile $(DEPLOY_BASE_DIR)/Makefile
	@ cp rozdeleni $(DEPLOY_BASE_DIR)
	@ cp dokumentace.pdf $(DEPLOY_BASE_DIR)
	@ zip -j xluklp00.zip $(DEPLOY_BASE_DIR)/*
	@ bash is_it_ok.sh xluklp00.zip $(DEPLOY_BASE_DIR)
	@ rm -rf xluklp00.zip

clean:
	@ rm -rf $(CLEAN_FS_NODES)
	@ echo Clean done
# ----------------------------------------------------------------

$(BUILD_BIN_BASE_DIR)/$(BIN): $(BUILD_O) | $(BUILD_BIN_BASE_DIR)
	$(CC) $(CFLAGS) $(BUILD_O) -o $@

$(DEBUG_BIN_BASE_DIR)/$(BIN): $(DEBUG_O) | $(DEBUG_BIN_BASE_DIR)
	$(CC) $(CFLAGS) $(DEBUG_O) -o $@

$(TESTS_BIN_BASE_DIR)/%: $(TESTS_O_BASE_DIR)/%.o $(BUILD_O_WITHOUT_MAIN) | $$(dir $(TESTS_BIN_BASE_DIR)/%)
	$(CC) $(CFLAGS) $< $(BUILD_O_WITHOUT_MAIN) -o $@

$(BUILD_O_BASE_DIR)/%.o: $(SRC_BASE_DIR)/%.c
$(BUILD_O_BASE_DIR)/%.o: $(SRC_BASE_DIR)/%.c $(BUILD_DEPS_BASE_DIR)/%.d | $$(dir $(BUILD_O_BASE_DIR)/%.o) $$(dir $(BUILD_DEPS_BASE_DIR)/%.d)
	$(CC) $(CFLAGS) $(BUILD_DEPFLAGS) -c $< -o $@

$(DEBUG_O_BASE_DIR)/%.o: $(SRC_BASE_DIR)/%.c
$(DEBUG_O_BASE_DIR)/%.o: $(SRC_BASE_DIR)/%.c $(DEBUG_DEPS_BASE_DIR)/%.d | $$(dir $(DEBUG_O_BASE_DIR)/%.o) $$(dir $(DEBUG_DEPS_BASE_DIR)/%.d)
	$(CC) $(CFLAGS) $(DEBUG_DEPFLAGS) -c $< -o $@

$(TESTS_O_BASE_DIR)/%.o: $(TESTS_SRC_BASE_DIR)/%.c
$(TESTS_O_BASE_DIR)/%.o: $(TESTS_SRC_BASE_DIR)/%.c $(TESTS_DEPS_BASE_DIR)/%.d | $$(dir $(TESTS_O_BASE_DIR)/%.o) $$(dir $(TESTS_DEPS_BASE_DIR)/%.d)
	$(CC) $(CFLAGS) $(TESTS_DEPFLAGS) -c $< -o $@

$(BUILD_BIN_BASE_DIR): ; @mkdir -p $@
$(DEBUG_BIN_BASE_DIR): ; @mkdir -p $@
$(DEPLOY_BASE_DIR).dir: ; @mkdir -p $(DEPLOY_BASE_DIR)
$(TESTS_BIN_DIRS): ; @mkdir -p $@
$(BUILD_O_DIRS): ; @mkdir -p $@
$(DEBUG_O_DIRS): ; @mkdir -p $@
$(TESTS_O_DIRS): ; @mkdir -p $@
$(BUILD_DEPS_DIRS): ; @mkdir -p $@
$(DEBUG_DEPS_DIRS): ; @mkdir -p $@
$(TESTS_DEPS_DIRS): ; @mkdir -p $@

$(BUILD_DEPS):
$(DEBUG_DEPS):
$(TESTS_DEPS):
include $(wildcard $(BUILD_DEPS))
include $(wildcard $(DEBUG_DEPS))
include $(wildcard $(TESTS_DEPS))
