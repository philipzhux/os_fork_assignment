.PHONY: default
default: compile ;
run:compile $(args); ./myfork $(args);
compile: abort.c myfork.c project.h; gcc abort.c myfork.c -o myfork;
clean:;rm -f myfork;