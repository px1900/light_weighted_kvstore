#ifndef MULTI_THREADS_HASHTABLE_KVSTORE_CONCURRENT_LRU_BUFFER_POOL_H
#define MULTI_THREADS_HASHTABLE_KVSTORE_CONCURRENT_LRU_BUFFER_POOL_H

#include <pthread.h>
#include <atomic>
#include <string>
#include <iostream>
#include <unordered_map>
#include "lru_node.h"
#include "../disk_manager/disk_manager.h"

// Default buffer pool size is 16MB
#define DEFAULT_BUFFER_POOL_SLOTS (16*1024/8)

class ConcurrentLruBufferPool {

public:
    ConcurrentLruBufferPool(int capacity = DEFAULT_BUFFER_POOL_SLOTS);
    ~ConcurrentLruBufferPool() {
        delete[] bufferPool;
        pthread_mutex_destroy(&mutex);
    }

    // Get the page content using the given
    bool GetPageContent(long long key, char* pageContent);
    void PutPageContent(long long key, char* pageContent);
    void DeletePageContent(long long key);
    long long AllocatePageInDisk();
private:
    LruNode* getNodeFromFreelist();
    LruNode* getNodeFromLruList();
    void moveNodeToLruListFront(LruNode* targetNode);
    void insertNodeLruList(LruNode* targetNode);

    std::unordered_map<long long, LruNode*> index_map;
    LruNode* lru_head;
    LruNode* lru_tail;
    int capacity;
    int size;
    pthread_mutex_t mutex;
    LruNode* bufferPool;
    LruNode* freeNodeList;

    DiskManager diskManager;
};


#endif //MULTI_THREADS_HASHTABLE_KVSTORE_CONCURRENT_LRU_BUFFER_POOL_H
