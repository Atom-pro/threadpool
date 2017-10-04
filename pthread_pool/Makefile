.PHONY:clean


TARGET=main
OBJECTS=condition.o\
		threadpool.o\
		main.o

CC=gcc
flags=-lpthread -g -Wall -lrt


all:$(TARGET)
$(TARGET):$(OBJECTS)
	$(CC) $^ -o $@ $(flags)

%.o:%.c
	$(CC) -c $^ -o $@
clean:
	rm -rf $(OBJECTS) $(TARGET)
