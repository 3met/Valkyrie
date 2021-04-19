
CXX       := g++
# -static-libstdc++ links standard libraries
# https://stackoverflow.com/questions/6404636/libstdc-6-dll-not-found

CXX_FLAGS := -std=c++17 -ggdb -static-libstdc++
DEBUG_FLAGS	:= -std=c++17 -ggdb -g

BIN     := bin
SRC     := src
INCLUDE := include

LIBRARIES   :=
EXECUTABLE  := main
DEBUG_EXECUTABLE 	:= debug_main

run:
	cd BIN && cls && $(EXECUTABLE)

compile_run: clean compile
	cd BIN && cls && $(EXECUTABLE)

debug: clean compile_debug
	cd BIN && cls && $(DEBUG_EXECUTABLE) && gdb a.out

clean:
	-del $(BIN)/*

compile: $(BIN)/$(EXECUTABLE)

# Compile Normally
$(BIN)/$(EXECUTABLE): $(SRC)/*.cpp $(SRC)/*/*.cpp
	cls && $(CXX) $(CXX_FLAGS) -I$(INCLUDE) $^ -o $@ $(LIBRARIES)

compile_debug: $(BIN)/$(DEBUG_EXECUTABLE)

# Compile with Debug
$(BIN)/$(DEBUG_EXECUTABLE): $(SRC)/*.cpp
	$(CXX) $(DEBUG_FLAGS) -I$(INCLUDE) $^ -o $@ $(LIBRARIES)
