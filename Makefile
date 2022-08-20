CC := cc
OBJECTS := build/parse.o build/cell.o build/head.o \
	build/closure.o build/debug.o build/stack.o \
	build/string.o build/regex.o build/tree.o \
	build/value.o \

all: ssimple
	mkdir -p build

ssimple: $(OBJECTS)
	$(CC) -o bin/ssimple src/main.c $(OBJECTS)

build/%.o: src/core/%.c
	mkdir -p build
	$(CC) -c  $<  -o $@

build/%.o: src/types/%.c
	mkdir -p build
	$(CC) -c  $<  -o $@

clean:
	rm -Rf ./build

debug:
	gdb --args bin/ssimple script/example.sl 
