CC := gcc
CFLAGS := -Werror -std=gnu17

SRC := $(wildcard *.c)
OBJ := $(SRC:.c=.o)

TARGET := $(CTF_TARGET)

all: $(TARGET)

$(TARGET): $(OBJ)
	$(CC) -o $@ $^

%.o: %.c
	$(CC) -o $@ $(CFLAGS) -c $<

clean:
	rm -rf $(OBJ) $(TARGET)