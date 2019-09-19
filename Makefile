all: bin/parser

bin/parser: bin build build/main.o
	g++ -std=c++17 -Wall build/main.o build/SimpleConfigParser.o -o3 -o bin/parser

build:
	mkdir build

bin:
	mkdir bin

build/main.o: main.cpp build/SimpleConfigParser.o
	g++ -std=c++17 -o3 -Wall -g -c $< -o $@

build/SimpleConfigParser.o: SimpleConfigParser.cpp SimpleConfigParser.h
	g++ -std=c++17 -o3 -Wall -g -c $< -o $@

clean:
	rm -rf bin build

run: all
	bin/parser