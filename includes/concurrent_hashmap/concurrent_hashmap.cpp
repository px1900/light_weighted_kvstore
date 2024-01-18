// Implement the CurrentHashMap class
#include "concurrent_hashmap.h"

// Implement Hashstd::string function for string
int ConcurrentHashMap::HashKey(std::string key) {
    int hashValue = 0;
    for(int i = 0; i < key.length(); i++) {
        hashValue = hashValue * 31 + key[i]*7;
        hashValue %= 100000;
    }
    return hashValue;
}

bool ConcurrentHashMap::GetValue(std::string key, long long& value) {
    int hashValue = HashKey(key) % this->bucketNum;
    return this->bucketList[hashValue].GetValue(key, value);
}

void ConcurrentHashMap::InsertOrUpdate(std::string key, long long value) {
    int hashValue = HashKey(key) % this->bucketNum;
    this->bucketList[hashValue].InsertOrUpdate(key, value);
}

void ConcurrentHashMap::DeleteValue(std::string key) {
    int hashValue = HashKey(key) % this->bucketNum;
    this->bucketList[hashValue].DeleteNode(key);
}
