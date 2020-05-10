OBJS = 	build/diseaseAggregator.o \
		build/namedPipesFifo.o \
		build/list.o \


CC = gcc
FLAGS = -Wall -Wextra -g -c

TARGET = diseaseAggregator

all: $(TARGET)

clean:
	$(RM) -r $(TARGET) build/*


build/diseaseAggregator.o: src/diseaseAggregator.c
	$(CC) $(FLAGS) $< -o $@

build/namedPipesFifo.o: src/namedPipesFifo.c
	$(CC) $(FLAGS) $< -o $@

build/list.o: src/list.c
	$(CC) $(FLAGS) $< -o $@

$(TARGET) : $(OBJS)
	$(CC) $(CFLAGS) $^ -o $@ -lm

rebuild: clean all
