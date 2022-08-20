ssimple: src/main.c
	cc -g -o bin/ssimple src/main.c

debug:
	gdb --args ./bin/ssimple script/example.sl 
