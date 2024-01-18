// Implement the HashNodeEle class

#include "concurrent_hashmap_node.h"

std::string HashNodeEle::GetKey() {
    pthread_rwlock_rdlock(&rwlock);
    std::string key = this->key;
    pthread_rwlock_unlock(&rwlock);
    return key;
}


long long HashNodeEle::GetValue() {
    pthread_rwlock_rdlock(&rwlock);
    long long value = this->value;
    pthread_rwlock_unlock(&rwlock);
    return value;
}


void HashNodeEle::SetValue(long long value) {
    pthread_rwlock_wrlock(&rwlock);
    this->value = value;
    pthread_rwlock_unlock(&rwlock);
}


void HashNodeEle::SetValueAndPointer(long long value, HashNodeEle* prevEle, HashNodeEle* nextEle) {
    pthread_rwlock_wrlock(&rwlock);
    this->value = value;
    this->prevEle = prevEle;
    this->nextEle = nextEle;
    pthread_rwlock_unlock(&rwlock);
}


HashNodeEle* HashNodeEle::GetNextEle() {
    pthread_rwlock_rdlock(&rwlock);
    HashNodeEle* nextEle = this->nextEle;
    pthread_rwlock_unlock(&rwlock);
    return nextEle;
}


HashNodeEle* HashNodeEle::GetPrevEle() {
    pthread_rwlock_rdlock(&rwlock);
    HashNodeEle* prevEle = this->prevEle;
    pthread_rwlock_unlock(&rwlock);
    return prevEle;
}


void HashNodeEle::SetPrevEle(HashNodeEle* prevEle) {
    pthread_rwlock_wrlock(&rwlock);
    this->prevEle = prevEle;
    pthread_rwlock_unlock(&rwlock);
}


void HashNodeEle::SetNextEle(HashNodeEle* nextEle) {
    pthread_rwlock_wrlock(&rwlock);
    this->nextEle = nextEle;
    pthread_rwlock_unlock(&rwlock);
}
