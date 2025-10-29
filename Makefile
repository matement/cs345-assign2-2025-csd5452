CC = gcc

CFLAGS = -Wall -pthread

TARGET = restaurant

SRC = main.c functions.c

all = $(TARGET)

$(TARGET): $(SRC)
	$(CC) $(CFLAGS) -o $(TARGET) $(SRC)
	

run: $(TARGET)
	./$(TARGET)

clean:
	rm -f $(TARGET)