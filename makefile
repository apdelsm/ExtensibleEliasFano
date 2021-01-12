CXX = g++
CXXFLAGS = -std=c++11 -O3
BIN = bin
SRC = $(shell find ./src -name "*.cpp")
LDLIBS = -lsdsl -ldivsufsort -ldivsufsort64
INC_DIR = ~/include
LIB_DIR = ~/lib
EXEC = test

all: $(BIN)/$(EXEC)

run: clean all
	clear
	@echo "Executing..."
	./$(BIN)/$(EXEC)

$(BIN)/$(EXEC): $(SRC)
	clear
	@echo "Building..."
	$(CXX) $(CXXFLAGS) -I $(INC_DIR) -L $(LIB_DIR) $^ -o $@ $(LDLIBS)

clean:
	@echo "Clearing..."
	-rm $(BIN)/*