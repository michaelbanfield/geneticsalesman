TARGET = geneticsalesman
SRC = $(wildcard *.c)
OBJS = $(SRC:.c=.o)



CC = gcc
CFLAGS = -pg

all: $(OBJS) $(TARGET)
	@echo Compilation complete

%.o: %.c
	@echo
	@echo Compiling $<...
	$(CC) $(CFLAGS) -c $< -o $@

$(TARGET): $(OBJS)
	@echo
	@echo Linking...
	$(CC) $(OBJS) -pg -o bin/$(TARGET) -lm

clean:
	@echo
	@echo Cleaning...
	rm -f bin/*