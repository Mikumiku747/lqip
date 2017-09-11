# Project makefile


# Tollchain options
CFLAGS := -Wall -Wextra -ansi -g
LDFLAGS := -lm

# File list generation
SRCFILES := $(shell find . -name "*.c")
OBJFILES := $(SRCFILES:*.c=*.o)

# Recipies
all: lqip

lqip: $(OBJFILES)
	gcc $(LDFLAGS) $(OBJFILES) -o lqip

%.o: %.c
	gcc $(CFLAGS) $< -o $@ -MMD


# Include the header-generated dependencies
include $(wildcard *.d)

