CC = gcc
CFLAGS = -Wall -Wextra -std=c11 -g -fopenmp
LDFLAGS = -lpthread -lm
TARGET = mandelbrot
SRC = $(wildcard *.c)
OBJ = $(SRC:.c=.o)

all: $(TARGET)

$(TARGET): $(OBJ)
	$(CC) $(CFLAGS) -o $@ $^ $(LDFLAGS)

%.o: %.c
	$(CC) $(CFLAGS) -c $< -o $@

clean:
	rm -f $(OBJ) $(TARGET)

.PHONY: all clean
