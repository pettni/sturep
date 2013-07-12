UNAME := $(shell uname -s)

ifeq ($(UNAME),Darwin)
CC = g++-4.7
CFLAGS = --std=c++11
endif
ifeq ($(UNAME),Linux)
CC = g++
CFLAGS = --std=c++0x
endif

# Make main
spel: main.cpp help_functions.o base environments actors objects controller.o
	$(CC) $(CFLAGS) -o spel.out *.o main.cpp

controller.o: controller.cpp creators.cpp action_lambdas.cpp old_creators.cpp controller.h main_actor.o
	$(CC) $(CFLAGS) -c controller.cpp

# Make class base
base: environment.o actor.o object.o

object.o: object.cpp object.h
	$(CC) $(CFLAGS) -c object.cpp
environment.o: environment.cpp environment.h
	$(CC) $(CFLAGS) -c environment.cpp
actor.o: actor.cpp actor.h
	$(CC) $(CFLAGS) -c actor.cpp


# Make environment subclasses
environments: indoor.o outdoor.o queue.o taxi_queue.o

indoor.o: environment.o environments/indoor.cpp environments/indoor.h
	$(CC) $(CFLAGS) -c environments/indoor.cpp 
outdoor.o: environment.o environments/outdoor.cpp environments/outdoor.h
	$(CC) $(CFLAGS) -c environments/outdoor.cpp 
queue.o: environment.o environments/queue.cpp environments/queue.h
	$(CC) $(CFLAGS) -c environments/queue.cpp 
taxi_queue.o: environment.o environments/taxi_queue.cpp environments/taxi_queue.h
	$(CC) $(CFLAGS) -c environments/taxi_queue.cpp 


# Make actor subclasses
actors: visitor.o worker.o main_actor.o salesman.o

visitor.o: actor.o actors/visitor.cpp actors/visitor.h
	$(CC) $(CFLAGS) -c actors/visitor.cpp 
main_actor.o: actor.o visitor.o actors/main_actor.cpp actors/main_actor.h
	$(CC) $(CFLAGS) -c actors/main_actor.cpp 
worker.o: actor.o actors/worker.cpp actors/worker.h
	$(CC) $(CFLAGS) -c actors/worker.cpp
salesman.o: worker.o actors/salesman.cpp actors/salesman.h
	$(CC) $(CFLAGS) -c actors/salesman.cpp


# Make object  subclasses
objects: container.o weapon.o usable.o money.o

container.o: object.o objects/container.cpp objects/container.h
	$(CC) $(CFLAGS) -c objects/container.cpp
weapon.o: object.o objects/weapon.cpp objects/weapon.h
	$(CC) $(CFLAGS) -c objects/weapon.cpp
usable.o: object.o objects/usable.cpp objects/usable.h
	$(CC) $(CFLAGS) -c objects/usable.cpp
money.o: object.o objects/money.cpp objects/money.h
	$(CC) $(CFLAGS) -c objects/money.cpp




# Help function
help_functions.o: help_functions.cpp help_functions.h
	$(CC) $(CFLAGS) -c help_functions.cpp

clean:
	rm -f spel.out *.o 

tags: base 
	/usr/local/bin/ctags -e -R -V .
