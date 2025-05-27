CC = gcc
CFLAGS = -Wall -Iinclude

SRC = src/aes.c src/utils.c src/keygen.c test/test_crypto.c
OBJ = $(SRC:.c=.o)

TARGET = test_crypto

all: $(TARGET)

$(TARGET): $(OBJ)
	$(CC) $(CFLAGS) -o $@ $^

clean:
	rm -f $(TARGET) src/*.o test/*.o
