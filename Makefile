CC = gcc

CFLAGS = -Wall -pthread

TARGET = restaurant

SRC = main.c functions.c list.c

OBJ = $(SRC:.c=.o)

DEPS = functions.h lsit.h

all : $(TARGET)

$(TARGET): $(OBJ)
	$(CC) $(CFLAGS) -o $@ $^
	
%.o: %.c $(DEPS)
	$(CC) $(CFLAGS) -c $< -o $@

run: $(TARGET)
	./$(TARGET)

clean:
	rm -f $(TARGET)