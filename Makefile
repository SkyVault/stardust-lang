CC=gcc
CFLAGS=-W -Wall -ggdb -Iinclude/ -Wno-unused-parameter -Wno-unused-variable
TARGET=bin/stardust.exe

SRCS := $(wildcard src/*.c)
HDRS := $(wildcard src/*.h)
OBJS := $(patsubst src/%.c,bin/%.o,$(SRCS))

$(TARGET): $(OBJS) $(HDRS) Makefile
	@mkdir -p bin
	$(CC) $(CFLAGS) $(OBJS) -o $(TARGET)

bin/%.o: src/%.c $(HDRS) Makefile
	@mkdir -p bin
	$(CC) $(CFLAGS) -c $< -o $@

clean:
	rm -f $(TARGET) $(OBJS)
