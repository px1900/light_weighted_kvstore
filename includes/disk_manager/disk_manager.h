#ifndef MULTI_THREADS_HASHTABLE_KVSTORE_DISK_MANAGER_H
#define MULTI_THREADS_HASHTABLE_KVSTORE_DISK_MANAGER_H

#include <pthread.h>
#include <atomic>
#include <string>
#include <iostream>
#include <unistd.h>
#include <sys/stat.h>
#include <cassert>

// Each page's size is 8KB
#define PAGE_SIZE 8192

// Each file's size is 64MB
#define MAX_PAGE_PER_FILE 8192

struct VacuumedNode {
    long long offset;
    VacuumedNode* next;
};

class DiskManager {
public:
    DiskManager() {
        appendOffset = 0;
        vacuumedNodeList = NULL;
        pthread_mutex_init(&mutex, NULL);
        // Check whether the kvdata directory exists, if not, create it
        if(access("kvdata", 0) == -1) {
            mkdir("kvdata", 0777);
        }
    }
    ~DiskManager() {
        VacuumedNode* currentNode = vacuumedNodeList;
        while(currentNode != NULL) {
            VacuumedNode* nextNode = currentNode->next;
            delete currentNode;
            currentNode = nextNode;
        }
        pthread_mutex_destroy(&mutex);
    }
    long long InsertPage(char* pageContent);
    void DeletePage(long long offset);
    void GetPage(long long offset, char* pageContent);
    void UpdatePage(long long offset, char* pageContent);
    long long AllocateSpace();

private:
    // Protect the appendOffset and vacuumedNodeList
    pthread_mutex_t mutex;

    long long appendOffset;
    VacuumedNode* vacuumedNodeList;
};

#endif //MULTI_THREADS_HASHTABLE_KVSTORE_DISK_MANAGER_H
