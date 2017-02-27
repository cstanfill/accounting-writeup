CXXFLAGS=-fstack-protector-all -Wno-deprecated-declarations -fomit-frame-pointer -O0 -g
CC=clang++
# CC=/usr/local/bin/afl-g++
# CXX=/usr/local/bin/afl-g++
CXX=clang++

all: bleh input

bleh: bleh.o

input: gen_input.py bleh
	./gen_input.py > input

clean:
	rm -f bleh bleh.o input
