
CXX			:= g++

CXX_FLAGS		:= -std=c++17 -ggdb -O -O2 -O3 -Wshadow -Wdouble-promotion -Wformat-overflow -Wformat-truncation -fno-common -Wpadded
DEBUG_FLAGS		:= -g -g3 -Wall
RELEASE_FLAGS	:= -s -static-libgcc -static-libstdc++ -static -lpthread

BIN			:= bin
SRC_DIR		:= src
BUILD		:= build
OBJECTS		:= $(BUILD)
INCLUDE		:= include
LIBRARIES	:= lib

DATA		:= data
RELEASES	:= ..\\releases

LIBRARIES	:= lib
EXE			:= main
DEBUG_EXE	:= debug_main

TARGET		:= $(BIN)/$(EXE)


# ----- COMPILE AND RUN NORMALLY -----
.PHONY: run
run: $(TARGET)
	cd BIN && cls && $(EXE)

$(TARGET): $(SRC_DIR)/*.cpp $(SRC_DIR)/*/*.cpp
	cls && $(CXX) $(CXX_FLAGS) -I $(INCLUDE) $^ -o $@ -L $(LIBRARIES)


# ----- COMPILE WITH DEBUG -----
.PHONY: debug
debug: $(BIN)/$(DEBUG_EXE)
	cd BIN && gdb $(DEBUG_EXE)

$(BIN)/$(DEBUG_EXE): $(SRC_DIR)/*.cpp $(SRC_DIR)/*/*.cpp
	cls
	$(CXX) $(CXX_FLAGS) $(DEBUG_FLAGS) -I $(INCLUDE) $^ -o $@ -L $(LIBRARIES)


# ----- CREATE RELEASE -----
.PHONY: release
release: $(RELEASES)/$(EXE)/$(BIN)/$(EXE)

$(RELEASES)/$(EXE)/$(BIN)/$(EXE): $(SRC_DIR)/*.cpp $(SRC_DIR)/*/*.cpp
	cls
	if not exist "$(RELEASES)/$(EXE)/$(BIN)" md "$(RELEASES)/$(EXE)/$(BIN)"
	$(CXX) $(CXX_FLAGS) $(RELEASE_FLAGS) -I $(INCLUDE) $^ -o $@ -L $(LIBRARIES)
	xcopy "$(DATA)" "$(RELEASES)\\$(EXE)\\$(DATA)" /s /q /y /i /c
	cls
