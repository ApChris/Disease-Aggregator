OBJS = 	build/diseaseAggregator.o \
		build/namedPipesFifo.o \
		build/list.o \


OBJS2 = build/worker.o \



CC = gcc
FLAGS = -Wall -Wextra -g -c

TARGET = diseaseAggregator
TARGET2 = worker

all: $(TARGET) $(TARGET2)

clean:
	$(RM) -r $(TARGET) $(TARGET2) build/*


build/diseaseAggregator.o: src/diseaseAggregator.c
	$(CC) $(FLAGS) $< -o $@

build/namedPipesFifo.o: src/namedPipesFifo.c
	$(CC) $(FLAGS) $< -o $@

build/list.o: src/list.c
	$(CC) $(FLAGS) $< -o $@

build/worker.o: src/worker.c
	$(CC) $(FLAGS) $< -o $@

$(TARGET) : $(OBJS)
	$(CC) $(CFLAGS) $^ -o $@ -lm

$(TARGET2) : $(OBJS2)
	$(CC) $(CFLAGS) $^ -o $@ -lm

rebuild: clean all
