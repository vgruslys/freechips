##########################
# Makefile for freechips #
##########################

#Gather all the .cpp files
SOURCE_FILES = $(wildcard source/*.cpp) \
	      $(wildcard source/history/*.cpp) \
	      $(wildcard source/global/*.cpp) \
	      $(wildcard source/communicator/*.cpp) \
	      $(wildcard source/random/*.cpp) \
	      $(wildcard source/engine/*.cpp) \
	      $(wildcard source/engine/qeval/*.cpp) \
#Each .cpp file becomes a .o file    
OBJECT_FILES=$(SOURCE_FILES:.cpp=.o)

#Define flags for g++, in this case just a bunch of -I[include_directory] plus the C++11 specification
CXXFLAGS=-Isource -Isource/communicator -Isource/global -Isource/history -Isource/random -Isource/engine -Isource/engine/qeval -Isource/engine/vectorclass -std=c++11 -mavx2 -mbmi -O3 -flto 

#Main rule: create holdem
holdem: $(OBJECT_FILES)
	$(CXX)  -pg -flto -o $@ $^

#Cleanup
.PHONY:clean
clean:
	rm -f $(OBJECT_FILES) holdem


	  
	

	      


