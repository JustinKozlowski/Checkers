CC := gcc
CFLAGS := -O -g -Wall -Werror -std=c99 -D_DEFAULT_SOURCE -D_GNU_SOURCE
LIBS := -lpthread 

SOURCES := src/Checkers.c
OBJS := $(patsubst %.c,%.o,$(SOURCES))

all: checkers

test: $(TESTS)

checkers: $(OBJS)
	$(CC) -o $@ $^

%.o: %.c
	$(CC) -c $< $(CFLAGS) -o $@

clean:
	rm -f src/*.o tests/*.o $(OBJS)

.PHONY: all clean
