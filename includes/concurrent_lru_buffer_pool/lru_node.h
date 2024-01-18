#ifndef MULTI_THREADS_HASHTABLE_KVSTORE_LRU_NODE_H
#define MULTI_THREADS_HASHTABLE_KVSTORE_LRU_NODE_H

#include <pthread.h>
#include <atomic>
#include <string>
#include <iostream>
#include <cstring>

class LruNode {
public:
    LruNode() {
        prevNode = NULL;
        nextNode = NULL;
        key = 0;
        pageContent = new char[8192];
        dirtyFlag = false;
    }
    ~LruNode() {
        delete[] pageContent;
    }
    void SetDirtyFlag();
    void ClearDirtyFlag();
    bool GetDirtyFlag();
    void SetPageContent(char* pageContent);
    void SetPrevNode(LruNode* prevNode);
    void SetNextNode(LruNode* nextNode);
    void SetKey(long long key);
    long long GetKey();
    char* GetPageContent();
    LruNode* GetPrevNode();
    LruNode* GetNextNode();

private:
    LruNode* prevNode;
    LruNode* nextNode;
    long long key; // key is the offset of the page in disk
    // Value is a 8192 bytes array
    char* pageContent;
    bool dirtyFlag;
};


#endif //MULTI_THREADS_HASHTABLE_KVSTORE_LRU_NODE_H
