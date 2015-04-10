
VERSION = 1.1.0
CPPFLAGS =  -D__VERSION_ID__="\"$(VERSION)\"" -g -Wall -O3 -fPIC  -pipe -D_REENTRANT -DLINUX -Wall
DEBUG_CPPFLAGS =  -D__VERSION_ID__="\"$(VERSION)\"" -g -Wall -O0 -fPIC  -pipe -D_REENTRANT -DLINUX -Wall

TARGET=test_rind

OBJ=rind_table.o \
	mem.o \
	sign.o

INCLUDES=-I include

		  
LIBS = -lcrypto \
	   -lpthread \

all: clean $(TARGET)
	@echo 'MAKE: ALL'
	mkdir output
	mv $(TARGET) output

test_rind: $(OBJ) test_rind.o 
	@echo 'MAKE [[' $@ ']]'
	g++ $^ -o $@ $(LIBS)

%.o: src/%.cc
	g++ -c $< -o $@ $(INCLUDES)

clean:
	find . -name '*~' | xargs rm -rf
	rm -rf *.o *.so *~ $(TARGET) models/*~
	rm -rf output



