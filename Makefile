CC := cc
OBJECTS := build/parse.o build/cell.o build/head.o \
	build/closure.o build/debug.o build/stack.o \
	build/string.o build/regex.o build/tree.o \
	build/value.o \

all: ssimple

ssimple: dir $(OBJECTS)
	$(CC) -o bin/ssimple src/main.c $(OBJECTS)

build/%.o: src/core/%.c
	$(CC) -c  $<  -o $@

build/%.o: src/types/%.c
	$(CC) -c  $<  -o $@

dir:
	mkdir build

clean:
	rm -Rf ./build

debug:
	gdb --args bin/ssimple script/example.sl 
