#i am using a makefile that i modified from TODO

# tool macros
CC := gcc
CFLAGS := -Wall
DBGFLAGS := -g
COBJFLAGS := $(CFLAGS) -c

# path macros
BIN_PATH := bin
OBJ_PATH := obj
SRC_PATH := src
DBG_PATH := debug

INCLUDE_PATH := headers
DYNLIB_PATHS := ./lib/x86_64
DYN_LIB := discord_game_sdk

# compile macros
TARGET_NAME := a.out
ifeq ($(OS),Windows_NT)
	TARGET_NAME := $(addsuffix .exe,$(TARGET_NAME))
endif
TARGET := $(BIN_PATH)/$(TARGET_NAME)
TARGET_DEBUG := $(DBG_PATH)/$(TARGET_NAME)

# src files & obj files
SRC := $(foreach x, $(SRC_PATH), $(wildcard $(addprefix $(x)/*,.c)))
OBJ := $(addprefix $(OBJ_PATH)/, $(addsuffix .o, $(notdir $(basename $(SRC)))))
OBJ_DEBUG := $(addprefix $(DBG_PATH)/, $(addsuffix .o, $(notdir $(basename $(SRC)))))

# clean files list
DISTCLEAN_LIST := $(OBJ) \
                  $(OBJ_DEBUG)
CLEAN_LIST := $(TARGET) \
			  $(TARGET_DEBUG) \
			  $(DISTCLEAN_LIST)

# default rule
default: makedir all

# non-phony targets
$(TARGET): $(OBJ)
	$(CC) -o $@ $(OBJ) -I $(INCLUDE_PATH) $(CFLAGS) -L $(DYNLIB_PATHS) $(DYNLIB_PATHS)/$(DYN_LIB).so

$(OBJ_PATH)/%.o: $(SRC_PATH)/%.c
	$(CC) $(COBJFLAGS) -o $@ $< -I $(INCLUDE_PATH) 

$(DBG_PATH)/%.o: $(SRC_PATH)/%.c
	$(CC) $(COBJFLAGS) $(DBGFLAGS) -o $@ $< -I $(INCLUDE_PATH) 

$(TARGET_DEBUG): $(OBJ_DEBUG)
	$(CC) $(CFLAGS) $(DBGFLAGS) $(OBJ_DEBUG) -o $@ -I $(INCLUDE_PATH) 

# phony rules
.PHONY: makedir
makedir:
	@mkdir -p $(BIN_PATH) $(OBJ_PATH) $(DBG_PATH)

.PHONY: all
all: $(TARGET)

.PHONY: debug
debug: $(TARGET_DEBUG)

.PHONY: clean
clean:
	@echo CLEAN $(CLEAN_LIST)
	@rm -f $(CLEAN_LIST)

.PHONY: distclean
distclean:
	@echo CLEAN $(DISTCLEAN_LIST)
	@rm -f $(DISTCLEAN_LIST)