CC := cc
COREOBJ := build/parse.o build/cell.o build/head.o \
	build/closure.o build/debug.o build/stack.o \
	build/state.o 

TYPESOBJ := build/string.o build/regex.o build/tree.o \
	build/value.o

OPOBJ := build/arithmetic_op.o build/io_op.o \
	build/definition_op.o build/closure_op.o

LIBRARYOBJ := build/default_library.o

OBJECTS = $(COREOBJ) $(TYPESOBJ) $(OPOBJ) $(LIBRARYOBJ)

TESTOBJECTS = $(TYPESOBJ) build/head.o build/cell.o build/closure.o build/arithmetic_op.o build/state.o build/stack.o build/parse.o build/debug.o

all: crow-lisp

crow-lisp: dir $(OBJECTS)
	$(CC) -g -o bin/crow-lisp src/main.c $(OBJECTS)

build/%.o: src/core/%.c
	$(CC) -c  $<  -o $@

build/%.o: src/run/%.c
	$(CC) -c  $<  -o $@

build/%.o: src/types/%.c
	$(CC) -c  $<  -o $@

build/%.o: src/operators/%.c
	$(CC) -c  $<  -o $@

build/%.o: src/library/%.c
	$(CC) -c  $<  -o $@

dir:
	mkdir -p  build

clean:
	rm -Rf ./build

debug:
	gdb --args bin/ssimple script/example.sl 


test: bin/test
	./bin/test
	rm ./bin/test

bin/test: test/unit.c dir $(TESTOBJECTS)
	$(CC) -g -o bin/test test/unit.c $(TESTOBJECTS)

run:
	./bin/crow-lisp script/example.sl
