#ifndef MULTI_THREADS_HASHTABLE_KVSTORE_CONCURRENT_HASHMAP_NODE_H
#define MULTI_THREADS_HASHTABLE_KVSTORE_CONCURRENT_HASHMAP_NODE_H

#include <pthread.h>
#include <atomic>
#include <string>
#include <iostream>


class HashNodeEle {
public:
    HashNodeEle() {
        nextEle = NULL;
        prevEle = NULL;
    }
    HashNodeEle(std::string key, long long value, HashNodeEle* prevEle, HashNodeEle* nextEle) {
        this->key = key;
        this->value = value;
        this->prevEle = prevEle;
        this->nextEle = nextEle;
        // Initialize the rwlock
        pthread_rwlock_init(&rwlock, NULL);
    }
    ~HashNodeEle() {
        pthread_rwlock_destroy(&rwlock);
    }
    std::string GetKey();
    long long GetValue();
    void SetValue(long long value);
    void SetValueAndPointer(long long value, HashNodeEle* prevEle, HashNodeEle* nextEle);
    HashNodeEle* GetNextEle();
    HashNodeEle* GetPrevEle();
    void SetPrevEle(HashNodeEle* prevEle);
    void SetNextEle(HashNodeEle* nextEle);
private:
    std::string key;
    long long value;
    pthread_rwlock_t rwlock;
    // When entryNum reaches HASH_ELEM_NUM, malloc a new
    // element node linked with this node
    HashNodeEle *nextEle;
    HashNodeEle *prevEle;
};


#endif //MULTI_THREADS_HASHTABLE_KVSTORE_CONCURRENT_HASHMAP_NODE_H
