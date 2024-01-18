#ifndef MULTI_THREADS_HASHTABLE_KVSTORE_LIGHT_WEIGHTED_KVSTORE_H
#define MULTI_THREADS_HASHTABLE_KVSTORE_LIGHT_WEIGHTED_KVSTORE_H

#include <pthread.h>
#include <atomic>
#include <string>
#include <iostream>
#include <unistd.h>
#include <sys/stat.h>
#include <cassert>
#include "../concurrent_hashmap/concurrent_hashmap.h"
#include "../concurrent_lru_buffer_pool/concurrent_lru_buffer_pool.h"

class LightedWeightedKVStore {
public:
    LightedWeightedKVStore() {
    }
    ~LightedWeightedKVStore() {
    }
    bool GetValue(std::string key, char* pageContent);
    void InsertKVPair(std::string key, char* pageContent);
    void DeleteValue(std::string key);

private:
    ConcurrentHashMap concurrentHashMap;
    ConcurrentLruBufferPool concurrentLruBufferPool;

};


#endif //MULTI_THREADS_HASHTABLE_KVSTORE_LIGHT_WEIGHTED_KVSTORE_H
