##########################
# Makefile for test      #
##########################

#Gather all the .cpp files
SOURCE_FILES =$(wildcard ./*.cpp) \
	      $(wildcard ../source/history/*.cpp) \
	      $(wildcard ../source/global/*.cpp) \
	      $(wildcard ../source/communicator/*.cpp) \
	      $(wildcard ../source/random/*.cpp) \
	      $(wildcard ../source/engine/*.cpp)
	
#Each .cpp file becomes a .o file    
OBJECT_FILES=$(SOURCE_FILES:.cpp=.o)

#Define flags for g++, in this case just a bunch of -I[include_directory] plus the C++11 specification
CXXFLAGS= -I../source/communicator -I../source/global -I../source/history -I../source/random -I../source/engine -std=c++11 -O2

#Main rule: create holdem
test: $(OBJECT_FILES)
	$(CXX) -o $@ $^

#Cleanup
.PHONY:clean
clean:
	rm -f $(OBJECT_FILES) test
