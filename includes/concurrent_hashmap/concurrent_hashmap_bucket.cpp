// Implement the HashBucket class

#include "concurrent_hashmap_bucket.h"

// Delete the node with the given key if exists
void HashBucket::DeleteNode(std::string key) {
    pthread_rwlock_wrlock(&bucketLock);
    HashNodeEle* currentNode = this->nodeList;
    while(currentNode != NULL) {
        if(currentNode->GetKey() == key) {
            HashNodeEle* prevNode = currentNode->GetPrevEle();
            HashNodeEle* nextNode = currentNode->GetNextEle();
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
bool HashBucket::GetValue(std::string key, long long& value) {
    pthread_rwlock_rdlock(&bucketLock);
    HashNodeEle* currentNode = this->nodeList;
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

void HashBucket::InsertOrUpdate(std::string key, long long value) {
    // We assume this key exists in the HashBucket, and grab the read lock to check
    pthread_rwlock_rdlock(&bucketLock);
    HashNodeEle* currentNode = this->nodeList;
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
    // std::string not exist. Insert this node to the head of the list
    HashNodeEle* newNode = new HashNodeEle(key, value, NULL, this->nodeList);
    if(this->nodeList != NULL) {
        this->nodeList->SetPrevEle(newNode);
    }
    this->nodeList = newNode;
    pthread_rwlock_unlock(&bucketLock);
    return;
}
