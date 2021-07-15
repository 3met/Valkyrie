
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

EXE_NAME		:= EB-chess

FILE_TYPES	:= *.c *.cpp *.cc *.cxx *.c++ *.C *.cp


# ---------------------------
# DO NOT EDIT BELOW THIS LINE
# ---------------------------

# Where to store .o and .d files
MAIN_DIR	:= main
DEBUG_DIR	:= debug

# Source Files
rwildcard	= $(foreach d,$(wildcard $(1:=/*)),$(call rwildcard,$d,$2) $(filter $(subst *,%,$2),$d))
SRC_FILES	:= $(call rwildcard, $(SRC_DIR), $(FILE_TYPES))
# Object Files
MAIN_OBJ	:= $(patsubst $(SRC_DIR)/%.cpp, $(BUILD)/$(MAIN_DIR)/%.o, $(SRC_FILES))
DEBUG_OBJ	:= $(patsubst $(SRC_DIR)/%.cpp, $(BUILD)/$(DEBUG_DIR)/%.o, $(SRC_FILES))
# Dependancy Files
MAIN_DEPS	:= $(patsubst $(SRC_DIR)/%.cpp, $(BUILD)/$(MAIN_DIR)/%.d, $(SRC_FILES))
DEBUG_DEPS	:= $(patsubst $(SRC_DIR)/%.cpp, $(BUILD)/$(DEBUG_DIR)/%.d, $(SRC_FILES))

-include $(MAIN_DEPS)
-include $(DEBUG_DEPS)

# ----- OS-SPECIFIC DEFINITIONS ------
EXE =
DEBUG_EXE =
copy_data =
rm =
ifeq ($(OS), Windows_NT)
	EXE += $(EXE_NAME).exe
	DEBUG_EXE = $(EXE_NAME)-debug.exe
	copy_data := xcopy "$(DATA)" "$(RELEASE_DIR)\\$(RELEASE_NAME)\\$(DATA)" /s /q /y /i /c
	rm := rmdir /s
else
	EXE += $(EXE_NAME)
	DEBUG_EXE = $(EXE_NAME)-debug
	copy_data := rsync -rupE "$(DATA)" "$(RELEASE_DIR)\\$(RELEASE_NAME)"
	rm := -r
endif


# ----- COMPILE AND RUN NORMALLY -----
.PHONY: compile
compile: $(BIN)/$(EXE)

# Compile object files into executable
$(BIN)/$(EXE): $(MAIN_OBJ)
	if not exist "$(BIN)" mkdir "$(BIN)"
	$(CXX) $(CXX_FLAGS) $^ -o "$@" -I $(INCLUDE) -L $(LIBRARIES)

# Compile object and dependancy files for each C/C++ file
$(BUILD)/$(MAIN_DIR)/%.o: $(SRC_DIR)/%.cpp
	if not exist "$(dir $@)" mkdir "$(dir $@)"
	$(CXX) $(CXX_FLAGS) -c "$<" -o "$@" -I $(INCLUDE)
	$(CXX) $(CXX_FLAGS) -MM $< -MT "$@" -MF "$(patsubst $(SRC_DIR)/%.cpp,$(BUILD)/$(MAIN_DIR)/%.d,$<)" -I $(INCLUDE)

# Build & Run Project
.PHONY: run
run:
	cd "$(BIN)" && "$(EXE)"

# Build & Run Project
.PHONY: clean
clean:
	if exist "$(BIN)" $(rm) "$(BIN)"
	if exist "$(BUILD)" $(rm) "$(BUILD)"


# ----- COMPILE WITH DEBUG -----
.PHONY: debug
debug: $(BIN)/$(DEBUG_EXE)
	cd "$(BIN)" && gdb $(DEBUG_EXE)

# Compile debug exe
$(BIN)/$(DEBUG_EXE): $(DEBUG_OBJ)
	if not exist "$(BIN)" mkdir "$(BIN)"
	$(CXX) $(CXX_FLAGS) $(DEBUG_FLAGS) $^ -o "$@" -I $(INCLUDE) -L $(LIBRARIES)

# Compile object file with debug flags
$(BUILD)/$(DEBUG_DIR)/%.o: $(SRC_DIR)/%.cpp
	if not exist "$(dir $@)" mkdir "$(dir $@)"
	$(CXX) $(CXX_FLAGS) $(DEBUG_FLAGS) -c "$<" -o "$@" -I $(INCLUDE)
	$(CXX) $(CXX_FLAGS) -MM $< -MT "$@" -MF "$(patsubst $(SRC_DIR)/%.cpp,$(BUILD)/$(DEBUG_DIR)/%.d,$<)" -I $(INCLUDE)


# ----- CREATE RELEASE -----
.PHONY: release
release: $(RELEASE_DIR)/$(RELEASE_NAME)/$(BIN)/$(RELEASE_NAME).exe

# 
$(RELEASE_DIR)/$(RELEASE_NAME)/$(BIN)/$(RELEASE_NAME).exe: $(SRC_FILES)
	if not exist "$(dir $@)" mkdir "$(dir $@)"
	$(copy_data)
	$(CXX) $(CXX_FLAGS) $(RELEASE_FLAGS) $^ -o "$@" -I $(INCLUDE) -L $(LIBRARIES)
