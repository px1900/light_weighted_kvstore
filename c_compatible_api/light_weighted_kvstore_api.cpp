// These functions are used to be compatible with C

#include "../includes/light_weighted_kvstore/light_weighted_kvstore.h"
#include "../includes/concurrent_hashmap/concurrent_hashmap.h"

// Global Light weighted KV store instance
LightedWeightedKVStore* lightedWeightedKVStore;
void InitLightWeightedKVStore() {
    lightedWeightedKVStore = new LightedWeightedKVStore();
}

int KvStoreGetValue(char* key, int keylen, char* pageContent) {
    std::string keyStr(key, keylen);
    if(lightedWeightedKVStore->GetValue(keyStr, pageContent) == true) {
        return 0;
    } else {
        return 1;
    }
}

void KvStoreInsertKVPair(char* key, int keylen, char* pageContent) {
    std::string keyStr(key, keylen);
    lightedWeightedKVStore->InsertKVPair(keyStr, pageContent);
}

void KvStoreDeleteValue(char* key, int keylen) {
    std::string keyStr(key, keylen);
    lightedWeightedKVStore->DeleteValue(keyStr);
}

void DestroyLightWeightedKVStore() {
    delete lightedWeightedKVStore;
}
