// Implement the ConcurrentLruBufferPool class

#include "concurrent_lru_buffer_pool.h"

ConcurrentLruBufferPool::ConcurrentLruBufferPool(int capacity) {
    this->capacity = capacity;
    this->size = 0;
    this->lru_head = NULL;
    this->lru_tail = NULL;
    this->bufferPool = new LruNode[capacity];
    pthread_mutex_init(&mutex, NULL);

    // One way link all the bufferPool nodes
    for(int i = 0; i < this->capacity-1; i++) {
        this->bufferPool[i].SetNextNode(&(this->bufferPool[i+1]));
    }
    this->bufferPool[this->capacity-1].SetNextNode(NULL);

    // Initialize the freeNodeList
    this->freeNodeList = &(this->bufferPool[0]);
}

// Insert the targetNode to the head of LRU list, and also insert to index_map
void ConcurrentLruBufferPool::insertNodeLruList(LruNode* targetNode) {
//    printf("insertNodeLruList key = %lld\n", targetNode->GetKey());
    // If the LRU list is empty, set the targetNode as the head and tail of LRU list
    if(this->lru_head == nullptr) {
        this->lru_head = targetNode;
        this->lru_tail = targetNode;
        targetNode->SetPrevNode(nullptr);
        targetNode->SetNextNode(nullptr);
        index_map[targetNode->GetKey()] = targetNode;
        return;
    }

    // Insert the targetNode to the head of LRU list
    targetNode->SetPrevNode(nullptr);
    targetNode->SetNextNode(this->lru_head);
    this->lru_head->SetPrevNode(targetNode);
    this->lru_head = targetNode;
    index_map[targetNode->GetKey()] = targetNode;
}

LruNode* ConcurrentLruBufferPool::getNodeFromFreelist() {
    // If there is no available free node, return null
    if(this->freeNodeList == nullptr) {
        return nullptr;
    }


    // Get the first node from freelist
    LruNode* targetNode = this->freeNodeList;
    this->freeNodeList = this->freeNodeList->GetNextNode();

    return targetNode;
}


// Get the tail node from LRU list
LruNode* ConcurrentLruBufferPool::getNodeFromLruList() {
    // If there is no available free node, return null
    if(this->lru_head == nullptr) {
        return nullptr;
    }

    // Get the tail node from LRU list and remove it from the LRU list
    LruNode* targetNode = this->lru_tail;
    this->lru_tail = this->lru_tail->GetPrevNode();
    if(this->lru_tail != nullptr) {
        this->lru_tail->SetNextNode(nullptr);
    } else {
        this->lru_head = nullptr;
    }

    // Get the targetNode's key
    long long key = targetNode->GetKey();
//    printf("remove key %lld from lru list\n", key);

    // Remove the targetNode from index_map
    this->index_map.erase(key);

    // If this node is dirty, write it to disk
    if(targetNode->GetDirtyFlag()) {
        char* pageContent = targetNode->GetPageContent();
        this->diskManager.UpdatePage(key, pageContent);
        targetNode->ClearDirtyFlag();
//        printf("flush key %lld to disk\n", key);
    }

    return targetNode;
}


// Move a node to the front of LRU list
void ConcurrentLruBufferPool::moveNodeToLruListFront(LruNode* targetNode) {
    // If the targetNode is already the head of LRU list, return
    if(targetNode == this->lru_head) {
        return;
    }

    // If the targetNode is the tail of LRU list, update the tail
    if(targetNode == this->lru_tail) {
        this->lru_tail = targetNode->GetPrevNode();
    }

    // Remove the targetNode from LRU list
    LruNode* prevNode = targetNode->GetPrevNode();
    LruNode* nextNode = targetNode->GetNextNode();
    if(prevNode != nullptr) {
        prevNode->SetNextNode(nextNode);
    }
    if(nextNode != nullptr) {
        nextNode->SetPrevNode(prevNode);
    }

    // Add the targetNode to the head of LRU list
    targetNode->SetPrevNode(nullptr);
    targetNode->SetNextNode(this->lru_head);
    if(this->lru_head != nullptr) {
        this->lru_head->SetPrevNode(targetNode);
    }
    this->lru_head = targetNode;
    if(this->lru_tail == nullptr) {
        this->lru_tail = targetNode;
    }
}

// Get the page content using the given key
// Firstly, lock the mutex
// 1. If the key exists in index_map, move the node to the front of LRU list and return the page content, then unlock the mutex
// 2. If the key doesn't exist in index_map, get a node from free list or from LRU list.
//     Then get the page content from disk and set the node's key, diskOffset and pageContent.
//     Then add the node to index_map and LRU list. Finally, unlock the mutex and return the page content.
bool ConcurrentLruBufferPool::GetPageContent(long long key, char* pageContent) {
    pthread_mutex_lock(&mutex);
    // 1. If the key exists in index_map, move the node to the front of LRU list and return the page content, then unlock the mutex
    if(this->index_map.find(key) != this->index_map.end()) {
        LruNode* targetNode = this->index_map[key];
        moveNodeToLruListFront(targetNode);
        memcpy(pageContent, targetNode->GetPageContent(), 8192);
        pthread_mutex_unlock(&mutex);
        return true;
    }

    // 2. If the key doesn't exist in index_map, get a node from free list or from LRU list.
    //     Then get the page content from disk and set the node's key, diskOffset and pageContent.
    //     Then add the node to index_map and LRU list. Finally, unlock the mutex and return the page content.
    LruNode* targetNode = getNodeFromFreelist();
    if(targetNode == nullptr) {
        targetNode = getNodeFromLruList();
    }
    if(targetNode == nullptr) {
        pthread_mutex_unlock(&mutex);
        return false;
    }
//    printf("read key %lld from disk\n", key);
    this->diskManager.GetPage(key, pageContent);
    targetNode->SetKey(key);
    targetNode->SetPageContent(pageContent);
    insertNodeLruList(targetNode);
    pthread_mutex_unlock(&mutex);
    return true;
}


// Put page content to the buffer pool
// Firstly, lock the mutex
// 1. If the key exists in index_map, move the node to the front of LRU list and update the page content, then unlock the mutex
// 2. If the key doesn't exist in index_map, get a node from free list or from LRU list. Then update the page content.
//     Then add the node to index_map and LRU list. Finally, unlock the mutex.
void ConcurrentLruBufferPool::PutPageContent(long long key, char* pageContent) {
    pthread_mutex_lock(&mutex);
    // 1. If the key exists in index_map, move the node to the front of LRU list and update the page content, then unlock the mutex
    if(this->index_map.find(key) != this->index_map.end()) {
//        printf("When putting key %lld, it already exists in index_map\n", key);
        LruNode* targetNode = this->index_map[key];
        moveNodeToLruListFront(targetNode);
        targetNode->SetPageContent(pageContent);
        targetNode->SetDirtyFlag();
        pthread_mutex_unlock(&mutex);
        return;
    }

    // 2. If the key doesn't exist in index_map, get a node from free list or from LRU list. Then update the page content.
    //     Then add the node to index_map and LRU list. Finally, unlock the mutex.
    LruNode* targetNode = getNodeFromFreelist();
    if(targetNode == nullptr) {
        targetNode = getNodeFromLruList();
    }
    if(targetNode == nullptr) {
        pthread_mutex_unlock(&mutex);
        return;
    }
//    printf("LINE %d, key = %lld\n", __LINE__, key);
    targetNode->SetKey(key);
    targetNode->SetPageContent(pageContent);
    targetNode->SetDirtyFlag();
//    printf("LINE %d, targetNode key = %lld\n", __LINE__, targetNode->GetKey());
    insertNodeLruList(targetNode);
    pthread_mutex_unlock(&mutex);
}


// Delete the page content using the given key
// Firstly, lock the mutex
// 1. If the key exists in index_map, remove it from Lru list and index_map,
//     then add it to the free list
// 2. Call diskManager.DeletePage(key) to delete the page from disk
// 3. Unlock the mutex
void ConcurrentLruBufferPool::DeletePageContent(long long key) {
    pthread_mutex_lock(&mutex);
    // 1. If the key exists in index_map, remove it from Lru list and index_map,
    //     then add it to the free list
    if(this->index_map.find(key) != this->index_map.end()) {
        LruNode* targetNode = this->index_map[key];
        LruNode* prevNode = targetNode->GetPrevNode();
        LruNode* nextNode = targetNode->GetNextNode();
        if(prevNode != nullptr) {
            prevNode->SetNextNode(nextNode);
        }
        if(nextNode != nullptr) {
            nextNode->SetPrevNode(prevNode);
        }
        if(targetNode == this->lru_head) {
            this->lru_head = nextNode;
        }
        if(targetNode == this->lru_tail) {
            this->lru_tail = prevNode;
        }
        this->index_map.erase(key);
        targetNode->SetNextNode(this->freeNodeList);
        this->freeNodeList = targetNode;
    }

    // 2. Call diskManager.DeletePage(key) to delete the page from disk
    this->diskManager.DeletePage(key);

    // 3. Unlock the mutex
    pthread_mutex_unlock(&mutex);
}


// Call diskManager to allocate a page in disk
long long ConcurrentLruBufferPool::AllocatePageInDisk() {
    return this->diskManager.AllocateSpace();
}
