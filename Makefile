
CXX			:= g++

CXX_FLAGS		:= -std=c++17 -O3 -Wshadow -Wdouble-promotion -Wformat-overflow -Wformat-truncation -fno-common
DEBUG_FLAGS		:= -ggdb -g -g3 -Og -Wall
RELEASE_FLAGS	:= -static-libgcc -static-libstdc++ -static -lpthread

BIN			:= bin
SRC_DIR		:= src
INCLUDE		:= include
LIBRARIES	:= lib
BUILD		:= build
DATA		:= data

RELEASE_DIR		:= ..\\releases
RELEASE_NAME	:= release

EXE			:= EB-chess.exe
DEBUG_EXE	:= debug.exe

FILE_TYPES	:= *.c *.cpp *.cc *.cxx *.c++ *.C *.cp


# ---------------------------
# DO NOT EDIT BELOW THIS LINE
# ---------------------------


rwildcard	= $(foreach d,$(wildcard $(1:=/*)),$(call rwildcard,$d,$2) $(filter $(subst *,%,$2),$d))
SRC_FILES	:= $(call rwildcard, $(SRC_DIR), $(FILE_TYPES))
OBJECTS		:= $(patsubst $(SRC_DIR)/%.cpp, $(BUILD)/%.o, $(SRC_FILES))


# ----- COMPILE AND RUN NORMALLY -----
.PHONY: compile
compile: $(BIN)/$(EXE)

# Compile object files into executable
$(BIN)/$(EXE): $(OBJECTS)
	if not exist "$(BIN)" mkdir "$(BIN)"
	$(CXX) $(CXX_FLAGS) $^ -o "$@" -I $(INCLUDE) -L $(LIBRARIES)

# Compile object file for each cpp file
$(BUILD)/%.o: $(SRC_DIR)/%.cpp
	if not exist "$(dir $@)" mkdir "$(dir $@)"
	$(CXX) $(CXX_FLAGS) -c "$^" -o "$@" -I $(INCLUDE)

# Build & Run Project
.PHONY: run
run:
	cd "$(BIN)" && "$(EXE)"


# ----- COMPILE WITH DEBUG -----
.PHONY: debug
debug: $(BIN)/$(DEBUG_EXE)
	cd "$(BIN)" && gdb $(DEBUG_EXE)

$(BIN)/$(DEBUG_EXE): $(SRC_FILES)
	if not exist "$(BIN)" mkdir "$(BIN)"
	$(CXX) $(CXX_FLAGS) $^ -o $@ -I $(INCLUDE) -L $(LIBRARIES)


# ----- CREATE RELEASE -----
.PHONY: release
release: $(RELEASE_DIR)/$(RELEASE_NAME)/$(BIN)/$(RELEASE_NAME).exe

# OS specific copy commands
copy_data =
ifeq ($(OS), Windows_NT)
	copy_data := xcopy "$(DATA)" "$(RELEASE_DIR)\\$(RELEASE_NAME)\\$(DATA)" /s /q /y /i /c
else
	copy_data := rsync -rupE "$(DATA)" "$(RELEASE_DIR)\\$(RELEASE_NAME)"
endif

$(RELEASE_DIR)/$(RELEASE_NAME)/$(BIN)/$(RELEASE_NAME).exe: $(SRC_FILES)
	if not exist "$(dir $@)" mkdir "$(dir $@)"
	$(copy_data)
	$(CXX) $(CXX_FLAGS) $(RELEASE_FLAGS) $^ -o "$@" -I $(INCLUDE) -L $(LIBRARIES)
