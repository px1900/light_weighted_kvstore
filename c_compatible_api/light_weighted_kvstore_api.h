#ifndef SRC_LIGHT_WEIGHTED_KVSTORE_API_H
#define SRC_LIGHT_WEIGHTED_KVSTORE_API_H


#ifdef __cplusplus
extern "C" {
#endif
void InitLightWeightedKVStore();
int KvStoreGetValue(char* key, int keylen, char* pageContent);
void KvStoreInsertKVPair(char* key, int keylen, char* pageContent);
void KvStoreDeleteValue(char* key, int keylen);
void DestroyLightWeightedKVStore();
#ifdef __cplusplus
}
#endif

#endif //SRC_LIGHT_WEIGHTED_KVSTORE_API_H
