#include <pthread.h>
#include <atomic>
#include <string>
#include <iostream>
#include <cstring>
#include "lru_node.h"

// Implement the LruNode class
void LruNode::SetKey(long long key) {
    this->key = key;
}

long long LruNode::GetKey() {
    return this->key;
}

void LruNode::SetPageContent(char* pageContent) {
    memcpy(this->pageContent, pageContent, 8192);
}

void LruNode::SetPrevNode(LruNode* prevNode) {
    this->prevNode = prevNode;
}

void LruNode::SetNextNode(LruNode* nextNode) {
    this->nextNode = nextNode;
}


char* LruNode::GetPageContent() {
    char* pageContent = this->pageContent;
    return pageContent;
}

LruNode* LruNode::GetPrevNode() {
    LruNode* prevNode = this->prevNode;
    return prevNode;
}

LruNode* LruNode::GetNextNode() {
    LruNode* nextNode = this->nextNode;
    return nextNode;
}

void LruNode::SetDirtyFlag() {
    this->dirtyFlag = true;
}

void LruNode::ClearDirtyFlag() {
    this->dirtyFlag = false;
}

bool LruNode::GetDirtyFlag() {
    return this->dirtyFlag;
}
