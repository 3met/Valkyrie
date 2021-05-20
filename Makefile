
CXX       := g++

CXX_FLAGS 	:= -std=c++17 -ggdb -static-libgcc -static-libstdc++
DEBUG_FLAGS	:= -std=c++17 -ggdb -g -static-libgcc -static-libstdc++

BIN     := bin
SRC     := src
INCLUDE := include

LIBRARIES   :=
EXECUTABLE  := main
DEBUG_EXECUTABLE 	:= debug_main

# Clear binary folder
clean:
	-del $(BIN)/*

# Run exe
run:
	cd BIN && cls && $(EXECUTABLE)

compile: $(BIN)/$(EXECUTABLE)

# Compile Normally
$(BIN)/$(EXECUTABLE): $(SRC)/*.cpp $(SRC)/*/*.cpp
	cls && $(CXX) $(CXX_FLAGS) -I $(INCLUDE) $^ -o $@ $(LIBRARIES)

debug: clean $(BIN)/$(DEBUG_EXECUTABLE)
	cd BIN && cls && gdb $(DEBUG_EXECUTABLE)

# Compile with Debug
$(BIN)/$(DEBUG_EXECUTABLE): $(SRC)/*.cpp $(SRC)/*/*.cpp
	cls && $(CXX) $(DEBUG_FLAGS) -I $(INCLUDE) $^ -o $@ $(LIBRARIES)
