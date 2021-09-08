.PHONY: default
default: compile ;
run:compile $(args); ./prog1 $(args);
compile: prog1.c project.h; gcc prog1.c -o prog1;
clean:;rm -f prog1;