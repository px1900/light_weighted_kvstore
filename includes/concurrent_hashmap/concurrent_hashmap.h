#ifndef MULTI_THREADS_HASHTABLE_KVSTORE_CONCURRENT_HASHMAP_H
#define MULTI_THREADS_HASHTABLE_KVSTORE_CONCURRENT_HASHMAP_H

#include <pthread.h>
#include <atomic>
#include <string>
#include <iostream>
#include "concurrent_hashmap_node.h"
#include "concurrent_hashmap_bucket.h"
#include "../concurrent_lru_buffer_pool/concurrent_lru_buffer_pool.h"

// Default bucket number is 1024
#define DEFAULT_BUCKET_NUM 1024

class ConcurrentHashMap {
public:
    HashBucket* bucketList;
    int bucketNum;

    ConcurrentHashMap(int bucketNum = DEFAULT_BUCKET_NUM) {
        this->bucketNum = bucketNum;
        bucketList = new HashBucket[bucketNum];
    }
    ~ConcurrentHashMap() {
        delete[] bucketList;
    }
    bool GetValue(std::string key, long long& value);
    void InsertOrUpdate(std::string key, long long value);
    void DeleteValue(std::string key);
private:
    // Hash function for string
    int HashKey(std::string key);
};


#endif //MULTI_THREADS_HASHTABLE_KVSTORE_CONCURRENT_HASHMAP_H

