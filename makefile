CXX = g++
CXXFLAGS = -std=c++17 -O3 -Wall -DNDEBUG
BIN = bin
SRC = $(shell find ./src -name "*.cpp")
LDLIBS = -lsdsl -ldivsufsort -ldivsufsort64
INC_DIR = ~/include
LIB_DIR = ~/lib
EXEC = test

all: $(BIN)/$(EXEC)

run: clean all
	@echo "Executing..."
	./$(BIN)/$(EXEC)

valgrind:
	valgrind --leak-check=full --show-leak-kinds=all --track-origins=yes --verbose --log-file=valgrind-out.txt ./$(BIN)/$(EXEC)

$(BIN)/$(EXEC): $(SRC)
	clear
	@echo "Building..."
	$(CXX) $(CXXFLAGS) -I $(INC_DIR) -L $(LIB_DIR) $^ -o $@ $(LDLIBS)

clean:
	@echo "Clearing..."
	-rm $(BIN)/*