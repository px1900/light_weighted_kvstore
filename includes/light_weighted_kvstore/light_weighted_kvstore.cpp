// Implement the LightedWeightedKVStore class

#include "light_weighted_kvstore.h"

// InsertKVPair
void LightedWeightedKVStore::InsertKVPair(std::string key, char* pageContent) {
    // Allocate a page space in disk
    long long offset = concurrentLruBufferPool.AllocatePageInDisk();
    // Insert the string-offset pair into the concurrent hashmap
    concurrentHashMap.InsertOrUpdate(key, offset);
    // Put the page content into the concurrent lru buffer pool
    concurrentLruBufferPool.PutPageContent(offset, pageContent);
}

// GetValue
bool LightedWeightedKVStore::GetValue(std::string key, char* pageContent) {
    // Get the offset from the concurrent hashmap
    long long offset;
    bool result = concurrentHashMap.GetValue(key, offset);
    if(result == false) {
        return false;
    }
    // Get the page content from the concurrent lru buffer pool
    concurrentLruBufferPool.GetPageContent(offset, pageContent);
    return true;
}

// DeleteValue
void LightedWeightedKVStore::DeleteValue(std::string key) {
    // Get the offset from the concurrent hashmap
    long long offset;
    bool result = concurrentHashMap.GetValue(key, offset);
    if(result == false) {
        return;
    }
    // Delete the string-offset pair from the concurrent hashmap
    concurrentHashMap.DeleteValue(key);
    // Delete the page content from the concurrent lru buffer pool
    concurrentLruBufferPool.DeletePageContent(offset);
}