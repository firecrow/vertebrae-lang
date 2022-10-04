CC := gcc -Wall
INSTALL_DIR := /usr/local/bin
COREOBJ := build/cell.o build/head.o \
	build/closure.o build/debug.o build/stack.o \
	build/state.o build/process_keys.o  build/utils.o

TYPESOBJ := build/string.o build/regex.o build/tree.o \
	build/value.o build/data.o

OPOBJ := build/arithmetic_op.o build/io_op.o build/condition_op.o \
	build/equal_op.o build/default_op.o build/function_op.o\
	build/set_op.o build/save_head_op.o

PARSEOBJ := build/run_parser.o

LIBRARYOBJ := build/default_library.o

OBJECTS = $(COREOBJ) $(PARSEOBJ) $(TYPESOBJ) $(OPOBJ) $(LIBRARYOBJ) 

all: gekkota

gekkota: dir $(OBJECTS)
	$(CC) -g -o bin/gekkota src/main.c $(OBJECTS)

build/%.o: src/core/%.c
	$(CC) -g -c $<  -o $@

build/run_parser.o: src/parse/run_parser.c
	$(CC) -g -c src/parse/run_parser.c -o build/run_parser.o 

build/%.o: src/run/%.c
	$(CC) -g -c $<  -o $@

build/%.o: src/types/%.c
	$(CC) -g -c $<  -o $@

build/%.o: src/operators/%.c
	$(CC) -g -c $<  -o $@

build/%.o: src/library/%.c
	$(CC) -g -c $<  -o $@

dir:
	mkdir -p  build

clean:
	rm -Rf ./build

debug:
	gdb --args bin/ssimple script/example.sl 


test: bin/test
	./bin/test
	rm ./bin/test

bin/test: test/unit.c dir $(OBJECTS)
	$(CC) -g -o bin/test test/unit.c $(OBJECTS)

run:
	./bin/gekkota script/example.sl

install:
	cp ./bin/gekkota $(INSTALL_DIR)
