# makefile to build the project

INCLUDES = -I./includes -I. -L. -I/usr/local/include -I/usr/include -L/usr/local/lib
LINKLIB = -lstdc++ -lpthread
CFLAG = -std=c++17
COMPILER = g++

HEADFILEs = $(wildcard ./includes/*.h)

all:
	g++ -c includes/concurrent_hashmap/concurrent_hashmap.cpp -o concurrent_hashmap.o
	g++ -c includes/concurrent_hashmap/concurrent_hashmap_bucket.cpp -o concurrent_hashmap_bucket.o
	g++ -c includes/concurrent_hashmap/concurrent_hashmap_node.cpp -o concurrent_hashmap_node.o
	g++ -c includes/concurrent_lru_buffer_pool/concurrent_lru_buffer_pool.cpp -o concurrent_lru_buffer_pool.o
	g++ -c includes/concurrent_lru_buffer_pool/lru_node.cpp -o lru_node.o
	g++ -c includes/disk_manager/disk_manager.cpp -o disk_manager.o
	g++ -c includes/light_weighted_kvstore/light_weighted_kvstore.cpp -o light_weighted_kvstore.o
	g++ -c c_compatible_api/light_weighted_kvstore_api.cpp -o light_weighted_kvstore_api.o

unit_test:
	g++ -c unit_test_concurrent_hashmap.cpp -o unit_test_concurrent_hashmap.o
	g++ -c unit_test_concurrent_lru_buffer_pool.cpp -o unit_test_concurrent_lru_buffer_pool.o
	g++ -c unit_test_light_weighted_kvstore.cpp -o unit_test_light_weighted_kvstore.o
	g++ -c unit_test_disk_manager.cpp -o unit_test_disk_manager.o
	g++ -c main.cpp -o main.o
	g++ -o unit_test_check main.o concurrent_hashmap.o concurrent_hashmap_bucket.o concurrent_hashmap_node.o concurrent_lru_buffer_pool.o lru_node.o disk_manager.o light_weighted_kvstore.o light_weighted_kvstore_api.o unit_test_concurrent_hashmap.o unit_test_concurrent_lru_buffer_pool.o unit_test_light_weighted_kvstore.o unit_test_disk_manager.o


clean:
	rm -rf *.o main; rm -rf *.a
static:
	ar -r libkvstore.a *.o
