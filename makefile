OBJS = 	build/diseaseAggregator.o \
		build/parentNamedPipesFifo.o \
		build/list.o \
		build/parentFunctions.o \


OBJS2 = build/worker.o \
		build/workerFunctions.o \
		build/workerNamedPipesFifo.o


CC = gcc
FLAGS = -Wall -Wextra -g -c

TARGET = diseaseAggregator
TARGET2 = worker

all: $(TARGET) $(TARGET2)

clean:
	$(RM) -r $(TARGET) $(TARGET2) build/*


build/diseaseAggregator.o: src/diseaseAggregator.c
	$(CC) $(FLAGS) $< -o $@

build/parentNamedPipesFifo.o: src/parentNamedPipesFifo.c
	$(CC) $(FLAGS) $< -o $@

build/list.o: src/list.c
	$(CC) $(FLAGS) $< -o $@

build/parentFunctions.o: src/parentFunctions.c
	$(CC) $(FLAGS) $< -o $@



build/worker.o: src/worker.c
	$(CC) $(FLAGS) $< -o $@

build/workerFunctions.o: src/workerFunctions.c
	$(CC) $(FLAGS) $< -o $@

build/workerNamedPipesFifo.o: src/workerNamedPipesFifo.c
	$(CC) $(FLAGS) $< -o $@

$(TARGET) : $(OBJS)
	$(CC) $(CFLAGS) $^ -o $@ -lm

$(TARGET2) : $(OBJS2)
	$(CC) $(CFLAGS) $^ -o $@ -lm

rebuild: clean all
