CC := cc
COREOBJ := build/parse.o build/cell.o build/head.o \
	build/closure.o build/debug.o build/stack.o

TYPESOBJ := build/string.o build/regex.o build/tree.o \
	build/value.o

OPOBJ := build/arithmetic_op.o

OBJECTS = $(COREOBJ) $(TYPESOBJ) $(OPOBJ)

all: ssimple

ssimple: dir $(OBJECTS)
	$(CC) -o bin/ssimple src/main.c $(OBJECTS)

build/%.o: src/core/%.c
	$(CC) -c  $<  -o $@

build/%.o: src/types/%.c
	$(CC) -c  $<  -o $@

build/%.o: src/operators/%.c
	$(CC) -c  $<  -o $@

dir:
	mkdir -p  build

clean:
	rm -Rf ./build

debug:
	gdb --args bin/ssimple script/example.sl 
