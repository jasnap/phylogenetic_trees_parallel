CXX = g++
CXXFLAGS = -Wall -std=c++0x -pthread

main.o: main.cpp main.h
	@$(CXX) $(CXXFLAGS) main.cpp
	@echo 'INFO: Run with: ./a.out example_filename.txt'
