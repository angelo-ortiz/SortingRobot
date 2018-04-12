MAKE_DIR = $(CURDIR)
C_FLAGS := -g -Wall
SDL := -lSDL -lm

INCLUDE_SRC_PATH := $(MAKE_DIR)/include/src
INCLUDE_LIB_PATH := $(MAKE_DIR)/include/lib
INC_SRC := -I$(INCLUDE_SRC_PATH) 
INC_LIB := -I$(INCLUDE_LIB_PATH) 

OBJ_SRC_PATH := $(MAKE_DIR)/obj/src
OBJ_LIB_PATH := $(MAKE_DIR)/obj/lib
OBJ_BIN_PATH := $(MAKE_DIR)/obj/bin

SRC_PATH := $(MAKE_DIR)/src
LIB_PATH := $(MAKE_DIR)/lib
BIN_PATH := $(MAKE_DIR)/bin
TESTS_PATH := $(MAKE_DIR)/tests

INCLUDE_SRC := $(wildcard $(INCLUDE_SRC_PATH)/*.h)
INCLUDE_LIB := $(wildcard $(INCLUDE_LIB_PATH)/*.h)

OBJ_SRC := $(patsubst $(SRC_PATH)/%.c, $(OBJ_SRC_PATH)/%.o, $(wildcard $(SRC_PATH)/*.c))
OBJ_LIB := $(patsubst $(LIB_PATH)/%.c, $(OBJ_LIB_PATH)/%.o, $(wildcard $(LIB_PATH)/*.c))
OBJ_BIN := $(patsubst $(TESTS_PATH)/%.c, $(OBJ_BIN_PATH)/%.o, $(wildcard $(TESTS_PATH)/*.c))

BIN_EXE := $(patsubst $(TESTS_PATH)/%.c, $(BIN_PATH)/%.exe, $(wildcard $(TESTS_PATH)/*.c))

.PHONY: all clean

.PRECIOUS: $(OBJ_SRC) $(OBJ_LIB) $(OBJ_BIN)

all: $(BIN_EXE) 

$(OBJ_SRC_PATH)/%.o: $(SRC_PATH)/%.c $(INCLUDE_SRC) $(INCLUDE_LIB)
	@echo "Compilation de " $<
	gcc -c -o $@ $< $(C_FLAGS) $(INC_SRC) $(INC_LIB)

$(OBJ_LIB_PATH)/%.o: $(LIB_PATH)/%.c $(INCLUDE_LIB)
	@echo "Compilation de " $<
	gcc -c -o $@ $< $(C_FLAGS) $(INC_LIB) 

$(OBJ_BIN_PATH)/%.o: $(TESTS_PATH)/%.c $(INCLUDE_SRC) $(INCLUDE_LIB)
	@echo "Compilation de " $<
	gcc -c -o $@ $< -g $(INC_SRC) $(INC_LIB)

$(BIN_PATH)/%.exe: $(OBJ_BIN_PATH)/%.o $(OBJ_SRC) $(OBJ_LIB)
	@echo "Compilation de " $<
	gcc -o $@ $^ $(C_FLAGS) $(SDL)

clean:
	rm -f $(OBJ_SRC_PATH)/*.o $(OBJ_LIB_PATH)/*.o $(OBJ_BIN_PATH)/*.o $(BIN_PATH)/*
