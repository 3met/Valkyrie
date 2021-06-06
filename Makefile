
CXX			:= g++

CXX_FLAGS	:= -std=c++17 -ggdb
DEBUG_FLAGS	:= -std=c++17 -ggdb -g

BIN			:= bin
SRC			:= src
INCLUDE		:= include
DATA		:= data
RELEASES	:= ..\\releases

LIBRARIES	:= lib
EXE			:= main
DEBUG_EXE	:= debug_main

# Run exe
run:
	cd BIN && cls && $(EXE)

compile: $(BIN)/$(EXE)

# Compile Normally
$(BIN)/$(EXE): $(SRC)/*.cpp $(SRC)/*/*.cpp
	cls && $(CXX) $(CXX_FLAGS) -I $(INCLUDE) $^ -o $@ -L $(LIBRARIES)

debug: $(BIN)/$(DEBUG_EXE)
	cd BIN && cls && gdb $(DEBUG_EXE)

# Compile with Debug
$(BIN)/$(DEBUG_EXE): $(SRC)/*.cpp $(SRC)/*/*.cpp
	cls && $(CXX) $(DEBUG_FLAGS) -I $(INCLUDE) $^ -o $@ -L $(LIBRARIES)

release: $(RELEASES)/$(EXE)/$(BIN)/$(EXE)

$(RELEASES)/$(EXE)/$(BIN)/$(EXE): $(SRC)/*.cpp $(SRC)/*/*.cpp
	cls
	if not exist "$(RELEASES)/$(EXE)/$(BIN)" md "$(RELEASES)/$(EXE)/$(BIN)"
	$(CXX) $(CXX_FLAGS) -I $(INCLUDE) $^ -o $@ -s -static-libgcc -static-libstdc++ -static -lpthread
	xcopy "$(DATA)" "$(RELEASES)\\$(EXE)\\$(DATA)" /s /q /y /i /c
	cls
