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

void kvstore_run_all_testcases() {
    unit_test_kvstore_case0();
}