#ifndef MULTI_THREADS_HASHTABLE_KVSTORE_CONCURRENT_HASHMAP_NODE_H
#define MULTI_THREADS_HASHTABLE_KVSTORE_CONCURRENT_HASHMAP_NODE_H

#include <pthread.h>
#include <atomic>
#include <string>
#include <iostream>

template <typename Key, typename Value>
class HashNodeEle {
public:
    HashNodeEle() {
        nextEle = NULL;
        prevEle = NULL;
    }
    HashNodeEle(Key key, Value value, HashNodeEle* prevEle, HashNodeEle* nextEle) {
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
    Key GetKey();
    Value GetValue();
    void SetValue(Value value);
    void SetValueAndPointer(Value value, HashNodeEle* prevEle, HashNodeEle* nextEle);
    HashNodeEle* GetNextEle();
    HashNodeEle* GetPrevEle();
    void SetPrevEle(HashNodeEle* prevEle);
    void SetNextEle(HashNodeEle* nextEle);
private:
    Key key;
    Value value;
    pthread_rwlock_t rwlock;
    // When entryNum reaches HASH_ELEM_NUM, malloc a new
    // element node linked with this node
    HashNodeEle *nextEle;
    HashNodeEle *prevEle;
};

// Implement the HashNodeEle class
template <typename Key, typename Value>
Key HashNodeEle<Key, Value>::GetKey() {
    pthread_rwlock_rdlock(&rwlock);
    Key key = this->key;
    pthread_rwlock_unlock(&rwlock);
    return key;
}

template <typename Key, typename Value>
Value HashNodeEle<Key, Value>::GetValue() {
    pthread_rwlock_rdlock(&rwlock);
    Value value = this->value;
    pthread_rwlock_unlock(&rwlock);
    return value;
}

template <typename Key, typename Value>
void HashNodeEle<Key, Value>::SetValue(Value value) {
    pthread_rwlock_wrlock(&rwlock);
    this->value = value;
    pthread_rwlock_unlock(&rwlock);
}

template <typename Key, typename Value>
void HashNodeEle<Key, Value>::SetValueAndPointer(Value value, HashNodeEle* prevEle, HashNodeEle* nextEle) {
    pthread_rwlock_wrlock(&rwlock);
    this->value = value;
    this->prevEle = prevEle;
    this->nextEle = nextEle;
    pthread_rwlock_unlock(&rwlock);
}

template <typename Key, typename Value>
HashNodeEle<Key, Value>* HashNodeEle<Key, Value>::GetNextEle() {
    pthread_rwlock_rdlock(&rwlock);
    HashNodeEle* nextEle = this->nextEle;
    pthread_rwlock_unlock(&rwlock);
    return nextEle;
}

template <typename Key, typename Value>
HashNodeEle<Key, Value>* HashNodeEle<Key, Value>::GetPrevEle() {
    pthread_rwlock_rdlock(&rwlock);
    HashNodeEle* prevEle = this->prevEle;
    pthread_rwlock_unlock(&rwlock);
    return prevEle;
}

template <typename Key, typename Value>
void HashNodeEle<Key, Value>::SetPrevEle(HashNodeEle* prevEle) {
    pthread_rwlock_wrlock(&rwlock);
    this->prevEle = prevEle;
    pthread_rwlock_unlock(&rwlock);
}

template <typename Key, typename Value>
void HashNodeEle<Key, Value>::SetNextEle(HashNodeEle* nextEle) {
    pthread_rwlock_wrlock(&rwlock);
    this->nextEle = nextEle;
    pthread_rwlock_unlock(&rwlock);
}


#endif //MULTI_THREADS_HASHTABLE_KVSTORE_CONCURRENT_HASHMAP_NODE_H
