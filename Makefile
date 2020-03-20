#Flag O0 en debug para poder ver todas las variables y no encontrar el error <optimized out>.

CC = g++
CFLAGS = -Wall -std=gnu++11 -O3
CFLAGS-DEBUG = -g -Wall -O0
SOURCES = main.h greedy.cpp savings.cpp sweep.cpp kmeans.cpp annealing.cpp main.cpp

.PHONY: main main-debug clean
all: main
all-debug : main-debug

main:			$(SOURCES)
	$(CC) $(CFLAGS) $^ -o $@

main-debug:		$(SOURCES)
	$(CC) $(CFLAGS-DEBUG) $^ -o $@

clean:
	rm -rf main
	rm -rf main-debug