#ifndef MULTI_THREADS_HASHTABLE_KVSTORE_CONCURRENT_HASHMAP_BUCKET_H
#define MULTI_THREADS_HASHTABLE_KVSTORE_CONCURRENT_HASHMAP_BUCKET_H

#include <pthread.h>
#include <atomic>
#include <string>
#include <iostream>
#include "concurrent_hashmap_node.h"

template <typename Key, typename Value>
class HashBucket {
public:
    HashBucket() {
        pthread_rwlock_init(&bucketLock, NULL);
        nodeList = NULL;
    }
    ~HashBucket() {
        pthread_rwlock_wrlock(&bucketLock);
        HashNodeEle<Key, Value>* currentNode = this->nodeList;
        while(currentNode != NULL) {
            HashNodeEle<Key, Value>* nextNode = currentNode->GetNextEle();
            delete currentNode;
            currentNode = nextNode;
        }
        pthread_rwlock_unlock(&bucketLock);
    }
    bool GetValue(Key key, Value& value);
    void InsertOrUpdate(Key key, Value value);
    void DeleteNode(Key key);

private:
    pthread_rwlock_t bucketLock;
    HashNodeEle<Key, Value>* nodeList;
};

// Implement the HashBucket class
// Delete the node with the given key if exists
template <typename Key, typename Value>
void HashBucket<Key, Value>::DeleteNode(Key key) {
    pthread_rwlock_wrlock(&bucketLock);
    HashNodeEle<Key, Value>* currentNode = this->nodeList;
    while(currentNode != NULL) {
        if(currentNode->GetKey() == key) {
            HashNodeEle<Key, Value>* prevNode = currentNode->GetPrevEle();
            HashNodeEle<Key, Value>* nextNode = currentNode->GetNextEle();
            if(prevNode != NULL) {
                prevNode->SetNextEle(nextNode);
            } else {
                this->nodeList = nextNode;
            }
            if(nextNode != NULL) {
                nextNode->SetPrevEle(prevNode);
            }
            delete currentNode;
            pthread_rwlock_unlock(&bucketLock);
            return;
        }
        currentNode = currentNode->GetNextEle();
    }
    pthread_rwlock_unlock(&bucketLock);
    return;
}

// Get the value of the given key
template <typename Key, typename Value>
bool HashBucket<Key, Value>::GetValue(Key key, Value& value) {
    pthread_rwlock_rdlock(&bucketLock);
    HashNodeEle<Key, Value>* currentNode = this->nodeList;
    while(currentNode != NULL) {
        if(currentNode->GetKey() == key) {
            value = currentNode->GetValue();
            pthread_rwlock_unlock(&bucketLock);
            return true;
        }
        currentNode = currentNode->GetNextEle();
    }
    pthread_rwlock_unlock(&bucketLock);
    return false;
}

// Insert or update the value of the given key
// Firstly, grab the read lock, and check whether this key exists.
// If exists, update the value and release the read lock.
// Else, release the read lock and grab the write lock, and check whether this key exists again.
// If exists, update the value and release the write lock.
// Else, insert this node to the head of the list, and release the write lock.
template <typename Key, typename Value>
void HashBucket<Key, Value>::InsertOrUpdate(Key key, Value value) {
    // We assume this key exists in the HashBucket, and grab the read lock to check
    pthread_rwlock_rdlock(&bucketLock);
    HashNodeEle<Key, Value>* currentNode = this->nodeList;
    while(currentNode != NULL) {
        if(currentNode->GetKey() == key) {
            currentNode->SetValue(value);
            pthread_rwlock_unlock(&bucketLock);
            return;
        }
        currentNode = currentNode->GetNextEle();
    }
    pthread_rwlock_unlock(&bucketLock);

    // Now we find that this key does not exist in the HashBucket, we need to grab the write lock
    // Before insert the new node, we need to check whether this key exists again
    pthread_rwlock_wrlock(&bucketLock);
    currentNode = this->nodeList;
    while(currentNode != NULL) {
        // If key exists now, just update the value and return
        if(currentNode->GetKey() == key) {
            currentNode->SetValue(value);
            pthread_rwlock_unlock(&bucketLock);
            return;
        }
        currentNode = currentNode->GetNextEle();
    }
    // Key not exist. Insert this node to the head of the list
    HashNodeEle<Key, Value>* newNode = new HashNodeEle<Key, Value>(key, value, NULL, this->nodeList);
    if(this->nodeList != NULL) {
        this->nodeList->SetPrevEle(newNode);
    }
    this->nodeList = newNode;
    pthread_rwlock_unlock(&bucketLock);
    return;
}




#endif //MULTI_THREADS_HASHTABLE_KVSTORE_CONCURRENT_HASHMAP_BUCKET_H
