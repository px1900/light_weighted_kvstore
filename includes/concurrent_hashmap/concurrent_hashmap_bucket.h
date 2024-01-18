#ifndef MULTI_THREADS_HASHTABLE_KVSTORE_CONCURRENT_HASHMAP_BUCKET_H
#define MULTI_THREADS_HASHTABLE_KVSTORE_CONCURRENT_HASHMAP_BUCKET_H

#include <pthread.h>
#include <atomic>
#include <string>
#include <iostream>
#include "concurrent_hashmap_node.h"


class HashBucket {
public:
    HashBucket() {
        pthread_rwlock_init(&bucketLock, NULL);
        nodeList = NULL;
    }
    ~HashBucket() {
        pthread_rwlock_wrlock(&bucketLock);
        HashNodeEle* currentNode = this->nodeList;
        while(currentNode != NULL) {
            HashNodeEle* nextNode = currentNode->GetNextEle();
            delete currentNode;
            currentNode = nextNode;
        }
        pthread_rwlock_unlock(&bucketLock);
    }
    bool GetValue(std::string key, long long& value);
    void InsertOrUpdate(std::string key, long long value);
    void DeleteNode(std::string key);

private:
    pthread_rwlock_t bucketLock;
    HashNodeEle* nodeList;
};





#endif //MULTI_THREADS_HASHTABLE_KVSTORE_CONCURRENT_HASHMAP_BUCKET_H
