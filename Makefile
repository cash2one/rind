
VERSION = 1.1.0
CPPFLAGS =  -D__VERSION_ID__="\"$(VERSION)\"" -g -Wall -O3 -fPIC  -pipe -D_REENTRANT -DLINUX -Wall
DEBUG_CPPFLAGS =  -D__VERSION_ID__="\"$(VERSION)\"" -g -Wall -O0 -fPIC  -pipe -D_REENTRANT -DLINUX -Wall

TARGET=test_index test_kv test_snappy test_table rind

OBJ=blockfile.o  \
	file_db.o  \
	file_kv.o  \
	index.o  \
	mem.o \
   	table.o

INCLUDES=-I libs/snappy

		  
LIBS = -lcrypto \
	   -lpthread \
	   -Llibs/snappy -lsnappy

all: clean $(TARGET)
	@echo 'MAKE: ALL'
	mkdir output
	mv $(TARGET) output

test_index: $(OBJ) test_index.o 
	@echo 'MAKE [[' $@ ']]'
	g++ $^ -o $@ $(LIBS)

test_kv: $(OBJ) test_kv.o 
	@echo 'MAKE [[' $@ ']]'
	g++ $^ -o $@ $(LIBS)

test_snappy: $(OBJ) test_snappy.o 
	@echo 'MAKE [[' $@ ']]'
	g++ $^ -o $@ $(LIBS)

test_table: $(OBJ) test_table.o 
	@echo 'MAKE [[' $@ ']]'
	g++ $^ -o $@ $(LIBS)

rind: $(OBJ) rind.o 
	@echo 'MAKE [[' $@ ']]'
	g++ $^ -o $@ $(LIBS)



%.o: %.cc
	g++ -c $< -o $@ $(INCLUDES)

clean:
	rm -rf *.o *.so *~ $(TARGET) models/*~
	rm -rf output



