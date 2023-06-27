CC := gcc
CFLAGS := -Werror -std=gnu17

SRC := $(wildcard *.c)
OBJ := $(SRC:.c=.o)

ifdef CTF_TARGET
	TARGET := $(CTF_TARGET)
else
	TARGET := chal
endif

all: $(TARGET)

$(TARGET): $(OBJ)
	$(CC) -o $@ $^

%.o: %.c
	$(CC) -o $@ $(CFLAGS) -c $<

clean:
	rm -rf $(OBJ) $(TARGET)