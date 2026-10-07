CXX   = g++
FLAGS = -std=c++20 -DNDEBUG -Iinclude
LIBS  = -ltbb12 -pthread
SRC   = source/*.cpp

all:
	$(CXX) $(FLAGS) -O0 $(SRC) -o reduce_O0.exe $(LIBS)
	$(CXX) $(FLAGS) -O3 $(SRC) -o reduce_O3.exe $(LIBS)

bench: all
	mkdir -p results
	./reduce_O0.exe > results/run_O0.txt
	./reduce_O3.exe > results/run_O3.txt

clean:
	del /q /f reduce_O0.exe reduce_O3.exe