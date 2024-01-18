#ifndef MULTI_THREADS_HASHTABLE_KVSTORE_CONCURRENT_HASHMAP_H
#define MULTI_THREADS_HASHTABLE_KVSTORE_CONCURRENT_HASHMAP_H

#include <pthread.h>
#include <atomic>
#include <string>
#include <iostream>
#include "concurrent_hashmap_node.h"
#include "concurrent_hashmap_bucket.h"

template <typename Key, typename Value>
class ConcurrentHashMap {
public:
    HashBucket<Key, Value>* bucketList;
    int bucketNum;
    ConcurrentHashMap() {
        bucketList = NULL;
        bucketNum = 0;
    }
    ConcurrentHashMap(int bucketNum) {
        this->bucketNum = bucketNum;
        bucketList = new HashBucket<Key, Value>[bucketNum];
    }
    ~ConcurrentHashMap() {
        delete[] bucketList;
    }
    bool GetValue(Key key, Value& value);
    void InsertOrUpdate(Key key, Value value);
    void DeleteValue(Key key);
private:
    // Hash function for string
    int HashKey(std::string key);
};

// Implement the CurrentHashMap class

// Implement HashKey function for string
template <typename Key, typename Value>
int ConcurrentHashMap<Key, Value>::HashKey(std::string key) {
    int hashValue = 0;
    for(int i = 0; i < key.length(); i++) {
        hashValue = hashValue * 31 + key[i]*7;
        hashValue %= 100000;
    }
    return hashValue;
}

template <typename Key, typename Value>
bool ConcurrentHashMap<Key, Value>::GetValue(Key key, Value& value) {
    int hashValue = HashKey(key) % this->bucketNum;
    return this->bucketList[hashValue].GetValue(key, value);
}

template <typename Key, typename Value>
void ConcurrentHashMap<Key, Value>::InsertOrUpdate(Key key, Value value) {
    int hashValue = HashKey(key) % this->bucketNum;
    this->bucketList[hashValue].InsertOrUpdate(key, value);
}

template <typename Key, typename Value>
void ConcurrentHashMap<Key, Value>::DeleteValue(Key key) {
    int hashValue = HashKey(key) % this->bucketNum;
    this->bucketList[hashValue].DeleteNode(key);
}

#endif //MULTI_THREADS_HASHTABLE_KVSTORE_CONCURRENT_HASHMAP_H

