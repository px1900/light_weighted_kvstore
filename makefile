# makefile to build the project

INCLUDES = -I./includes -I. -L. -I/usr/local/include -I/usr/include -L/usr/local/lib
LINKLIB = -lstdc++ -lpthread
CFLAG = -std=c++17
COMPILER = g++

HEADFILEs = $(wildcard ./includes/*.h)

all: main

main: main.o unit_test_concurrent_hashmap.o unit_test_disk_manager.o disk_manager.o unit_test_concurrent_lru_buffer_pool.o concurrent_lru_buffer_pool.o lru_node.o concurrent_hashmap.o concurrent_hashmap_bucket.o concurrent_hashmap_node.o light_weighted_kvstore.o unit_test_light_weighted_kvstore.o $(HEADFILEs)
	$(COMPILER) $(CFLAG) -o $@ $^ $(INCLUDES) $(LINKLIB)

main.o: main.cpp $(HEADFILEs)

disk_manager.o: includes/disk_manager/disk_manager.cpp $(HEADFILEs)
	$(COMPILER) $(CFLAG) -c $< $(INCLUDES) $(LINKLIB)

lru_node.o: includes/concurrent_lru_buffer_pool/lru_node.cpp $(HEADFILEs)
	$(COMPILER) $(CFLAG) -c $< $(INCLUDES) $(LINKLIB)

concurrent_lru_buffer_pool.o: includes/concurrent_lru_buffer_pool/concurrent_lru_buffer_pool.cpp $(HEADFILEs)
	$(COMPILER) $(CFLAG) -c $< $(INCLUDES) $(LINKLIB)

unit_test_concurrent_hashmap.o: unit_test_concurrent_hashmap.cpp $(HEADFILEs)
	$(COMPILER) $(CFLAG) -c $< $(INCLUDES) $(LINKLIB)

unit_test_disk_manager.o: unit_test_disk_manager.cpp $(HEADFILEs)
	$(COMPILER) $(CFLAG) -c $< $(INCLUDES) $(LINKLIB)

unit_test_concurrent_lru_buffer_pool.o: unit_test_concurrent_lru_buffer_pool.cpp $(HEADFILEs)
	$(COMPILER) $(CFLAG) -c $< $(INCLUDES) $(LINKLIB)

concurrent_hashmap.o: includes/concurrent_hashmap/concurrent_hashmap.cpp $(HEADFILEs)
	$(COMPILER) $(CFLAG) -c $< $(INCLUDES) $(LINKLIB)

concurrent_hashmap_bucket.o: includes/concurrent_hashmap/concurrent_hashmap_bucket.cpp $(HEADFILEs)
	$(COMPILER) $(CFLAG) -c $< $(INCLUDES) $(LINKLIB)

concurrent_hashmap_node.o: includes/concurrent_hashmap/concurrent_hashmap_node.cpp $(HEADFILEs)
	$(COMPILER) $(CFLAG) -c $< $(INCLUDES) $(LINKLIB)

light_weighted_kvstore.o: includes/light_weighted_kvstore/light_weighted_kvstore.cpp $(HEADFILEs)
	$(COMPILER) $(CFLAG) -c $< $(INCLUDES) $(LINKLIB)

unit_test_light_weighted_kvstore.o: unit_test_light_weighted_kvstore.cpp $(HEADFILEs)
	$(COMPILER) $(CFLAG) -c $< $(INCLUDES) $(LINKLIB)

clean:
	rm -rf *.o main

