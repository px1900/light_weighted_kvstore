#include "includes/light_weighted_kvstore/light_weighted_kvstore.h"
#include <string>
#include <pthread.h>
#include <cassert>

using namespace std;

void unit_test_kvstore_case0() {
    class LightedWeightedKVStore testStore;
    char* pageContent = new char[8192];
    testStore.InsertKVPair("test1", pageContent);
    char* result = new char[8192];
    assert(testStore.GetValue("test1", result) == true);
    assert(testStore.GetValue("test2", result) == false);
    testStore.DeleteValue("test1");
    assert(testStore.GetValue("test1", result) == false);

    cout << "light_weighted_kvstore test_case0 passed" << endl;
};

void unit_test_kvstore_case1() {
    class LightedWeightedKVStore testStore;
    char* pageContent = new char[8192];
    for(int i = 0; i < 1000; i++) {
        testStore.InsertKVPair("test" + to_string(i), pageContent);
    }
    char* result = new char[8192];
    for(int i = 0; i < 1000; i++) {
        assert(testStore.GetValue("test" + to_string(i), result) == true);
    }
    assert(testStore.GetValue("test1000", result) == false);

    cout << "light_weighted_kvstore test_case1 passed" << endl;
}

// For function WriteAndRead1, it will write 100000 key-value pairs into the testStore, and then read them out.
void* KVWriteAndRead1(void* arg) {
    class LightedWeightedKVStore* testStore = (class LightedWeightedKVStore*) arg;
    char* pageContent = new char[8192];
    for(int i = 0; i < 100000; i++) {
        testStore->InsertKVPair("test" + to_string(i), pageContent);
    }
    char* result = new char[8192];
    for(int i = 0; i < 100000; i++) {
        assert(testStore->GetValue("test" + to_string(i), result) == true);
    }
    return NULL;
}

// For function WriteAndRead2, it will write 100000 key-value pairs which are different with WriteAndRead1 into the testStore, for each kv pair, it will write
// it first and then read it out.
void* KVWriteAndRead2(void* arg) {
    class LightedWeightedKVStore* testStore = (class LightedWeightedKVStore*) arg;
    char* pageContent = new char[8192];
    for(int i = 100000; i < 200000; i++) {
        testStore->InsertKVPair("test" + to_string(i), pageContent);
        char* result = new char[8192];
        assert(testStore->GetValue("test" + to_string(i), result) == true);
    }
    return NULL;
}


// WriteAndRead3 do same thing with WriteAndRead2
void* KVWriteAndRead3(void* arg) {
    class LightedWeightedKVStore* testStore = (class LightedWeightedKVStore*) arg;
    char* pageContent = new char[8192];
    for(int i = 0; i < 100000; i++) {
        testStore->InsertKVPair("test" + to_string(i), pageContent);
        char* result = new char[8192];
        assert(testStore->GetValue("test" + to_string(i), result) == true);
    }
    return NULL;
}

void unit_test_kvstore_case2() {
    class LightedWeightedKVStore testStore;
    // Initialize three threads
    pthread_t thread1, thread2, thread3;
    int ret1, ret2, ret3;
    ret1 = pthread_create(&thread1, NULL, KVWriteAndRead1, &testStore);
    ret2 = pthread_create(&thread2, NULL, KVWriteAndRead2, &testStore);
    ret3 = pthread_create(&thread3, NULL, KVWriteAndRead3, &testStore);
    // Wait until the three threads finish
    pthread_join(thread1, NULL);
    pthread_join(thread2, NULL);
    pthread_join(thread3, NULL);
    cout << "light_weighted_kvstore test_case2 passed" << endl;

}

void kvstore_run_all_testcases() {
    unit_test_kvstore_case0();
    unit_test_kvstore_case1();
    unit_test_kvstore_case2();
}