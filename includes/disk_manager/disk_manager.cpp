// Implement the DiskManager class

#include <pthread.h>
#include <atomic>
#include <string>
#include <iostream>
#include <unistd.h>
#include <sys/stat.h>
#include <cassert>
#include "disk_manager.h"

// Insert a page to a file and return the offset of the page
// If there are vacuumed pages in the vacuumedNodeList, then reuse the offset of the vacuumed page
// Otherwise, append the page to the end of the file
// Then calculate which file the page should be inserted to.
// If file doesn't exist, create a new file, if exists, write page to the file upon offset.
long long DiskManager::InsertPage(char* pageContent) {
    // Get an available offset to fit this page
    long long offset = -1;
    pthread_mutex_lock(&mutex);
    if(vacuumedNodeList != NULL) {
        VacuumedNode* currentNode = vacuumedNodeList;
        vacuumedNodeList = vacuumedNodeList->next;
        offset = currentNode->offset;
        delete currentNode;
    } else {
        offset = appendOffset;
        appendOffset += PAGE_SIZE;
    }
    pthread_mutex_unlock(&mutex);

    // Calculate which file the page should be inserted to
    long long fileIndex = offset / (PAGE_SIZE * MAX_PAGE_PER_FILE);
    long long pageIndex = (offset % (PAGE_SIZE * MAX_PAGE_PER_FILE)) / PAGE_SIZE;
    // The file name format is "kvdata/file_0", "kvdata/file_1", "kvdata/file_2", ...
    std::string fileName = "kvdata/file_" + std::to_string(fileIndex);
    // Open the file, if not exists, create it
    FILE* file = fopen(fileName.c_str(), "a+");
    // Write the page to the file
    fseek(file, pageIndex * PAGE_SIZE, SEEK_SET);
    fwrite(pageContent, PAGE_SIZE, 1, file);
    fclose(file);
    return offset;
}

// Delete a page from a file and add the offset to the vacuumedNodeList
// Don't need to erase the file content, just add it to vacuumedNodeList
// Also check whether this offset exceeds the appendOffset using assert
void DiskManager::DeletePage(long long offset) {
    assert(offset < appendOffset);
    VacuumedNode* newVacuumedNode = new VacuumedNode();
    newVacuumedNode->offset = offset;
    pthread_mutex_lock(&mutex);
    newVacuumedNode->next = vacuumedNodeList;
    vacuumedNodeList = newVacuumedNode;
    pthread_mutex_unlock(&mutex);
}

// Get a page from a file using the given offset
void DiskManager::GetPage(long long offset, char* pageContent) {
    assert(offset < appendOffset);
    // Calculate which file the page should be inserted to
    long long fileIndex = offset / (PAGE_SIZE * MAX_PAGE_PER_FILE);
    long long pageIndex = (offset % (PAGE_SIZE * MAX_PAGE_PER_FILE)) / PAGE_SIZE;
    // The file name format is "kvdata/file_0", "kvdata/file_1", "kvdata/file_2", ...
    std::string fileName = "kvdata/file_" + std::to_string(fileIndex);

    // Open the file, if not exists, create a new file
    FILE* file = fopen(fileName.c_str(), "a+");
    // Chech whether the file exists, if not exist, create a new one
    assert(file != nullptr);

    // Read the page from the file
    fseek(file, pageIndex * PAGE_SIZE, SEEK_SET);
    fread(pageContent, PAGE_SIZE, 1, file);
    fclose(file);
}

// Update a page from a file using the given offset
void DiskManager::UpdatePage(long long offset, char* pageContent) {
    assert(offset < appendOffset);
    // Calculate which file the page should be inserted to
    long long fileIndex = offset / (PAGE_SIZE * MAX_PAGE_PER_FILE);
    long long pageIndex = (offset % (PAGE_SIZE * MAX_PAGE_PER_FILE)) / PAGE_SIZE;
    // The file name format is "kvdata/file_0", "kvdata/file_1", "kvdata/file_2", ...
    std::string fileName = "kvdata/file_" + std::to_string(fileIndex);
    // Open the file, using assert to check whether the file exists
    FILE* file = fopen(fileName.c_str(), "r+");
    assert(file != nullptr);
    // Read the page from the file
    fseek(file, pageIndex * PAGE_SIZE, SEEK_SET);
    fwrite(pageContent, PAGE_SIZE, 1, file);
    fclose(file);
}


// Allocate a empty space for a coming page
// If there are vacuumed pages in the vacuumedNodeList, then reuse the offset of the vacuumed page
// Otherwise, append the page to the end of the file
long long DiskManager::AllocateSpace() {
    long long offset = -1;
    pthread_mutex_lock(&mutex);
    if(vacuumedNodeList != NULL) {
        VacuumedNode* currentNode = vacuumedNodeList;
        vacuumedNodeList = vacuumedNodeList->next;
        offset = currentNode->offset;
        delete currentNode;
    } else {
        offset = appendOffset;
        appendOffset += PAGE_SIZE;
    }
    pthread_mutex_unlock(&mutex);
    return offset;
}