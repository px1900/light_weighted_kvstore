#include "includes/concurrent_hashmap/concurrent_hashmap.h"
#include <string>
#include <pthread.h>
#include <cassert>

using namespace std;


void unit_test_case0() {
    class ConcurrentHashMap testMap(26);
    testMap.InsertOrUpdate("test1", 1);
    long long result = -1;

    assert(testMap.GetValue("test1", result) == true);
    assert(result == 1);
    cout << "concurrent_hashmap test_case0 passed" << endl;
};

void unit_test_case1() {
    class ConcurrentHashMap testMap(26);
    for(int i = 0; i < 100; i++) {
        testMap.InsertOrUpdate("test" + to_string(i), i);
    }
    long long result = -1;
    for(int i = 0; i < 100; i++) {
        assert(testMap.GetValue("test" + to_string(i), result) == true);
        assert(result == i);
    }

    cout << "concurrent_hashmap test_case1 passed" << endl;
}

void unit_test_case2() {
    class ConcurrentHashMap testMap(26);
    for(int i = 0; i < 100; i++) {
        testMap.InsertOrUpdate("test" + to_string(i), i);
    }
    long long result = -1;
    assert(testMap.GetValue("test100", result) == false);

    cout << "concurrent_hashmap test_case2 passed" << endl;
}


// Test DeleteValue
void unit_test_case3() {
    class ConcurrentHashMap testMap(26);
    for(int i = 0; i < 100; i++) {
        testMap.InsertOrUpdate("test" + to_string(i), i);
    }
    long long result = -1;
    for(int i = 0; i < 100; i++) {
        assert(testMap.GetValue("test" + to_string(i), result) == true);
        assert(result == i);
    }
    testMap.DeleteValue("test50");
    assert(testMap.GetValue("test50", result) == false);
    for(int i = 0; i < 100; i++) {
        if(i == 50) {
            continue;
        }
        assert(testMap.GetValue("test" + to_string(i), result) == true);
        assert(result == i);
    }

    cout << "concurrent_hashmap test_case3 passed" << endl;
}

// Test UpdateValue
void unit_test_case4() {
    class ConcurrentHashMap testMap(26);
    for(int i = 0; i < 100; i++) {
        testMap.InsertOrUpdate("test" + to_string(i), i);
    }
    long long result = -1;
    for(int i = 0; i < 100; i++) {
        assert(testMap.GetValue("test" + to_string(i), result) == true);
        assert(result == i);
    }
    testMap.InsertOrUpdate("test50", 100);
    assert(testMap.GetValue("test50", result) == true);
    assert(result == 100);
    for(int i = 0; i < 100; i++) {
        if(i == 50) {
            continue;
        }
        assert(testMap.GetValue("test" + to_string(i), result) == true);
        assert(result == i);
    }

    cout << "concurrent_hashmap test_case4 passed" << endl;
}


// Insert test0-test99, then read test0-test99
void *WriteAndRead1(void* arg) {
    class ConcurrentHashMap* testMap = (class ConcurrentHashMap*) arg;
    for(int i = 0; i < 100; i++) {
        testMap->InsertOrUpdate("test" + to_string(i), i);
    }
    long long result = -1;
    for(int i = 0; i < 100; i++) {
        assert(testMap->GetValue("test" + to_string(i), result) == true);
        assert(result == i);
    }
    return NULL;
}

// Insert test100-test199, then read test100-test199
void *WriteAndRead2(void* arg) {
    class ConcurrentHashMap* testMap = (class ConcurrentHashMap*) arg;
    for(int i = 100; i < 200; i++) {
        testMap->InsertOrUpdate("test" + to_string(i), i);
    }
    long long result = -1;
    for(int i = 100; i < 200; i++) {
        assert(testMap->GetValue("test" + to_string(i), result) == true);
        assert(result == i);
    }
    return NULL;
}

// Insert test200-test299, then read test200-test299
void *WriteAndRead3(void* arg) {
    class ConcurrentHashMap* testMap = (class ConcurrentHashMap*) arg;
    for(int i = 200; i < 300; i++) {
        testMap->InsertOrUpdate("test" + to_string(i), i);
    }
    long long result = -1;
    for(int i = 200; i < 300; i++) {
        assert(testMap->GetValue("test" + to_string(i), result) == true);
        assert(result == i);
    }
    return NULL;
}

void *OnlyRead1(void *arg) {
    class ConcurrentHashMap* testMap = (class ConcurrentHashMap*) arg;
    long long result = -1;
    for(int i = 0; i < 100; i++) {
        if(testMap->GetValue("test" + to_string(i), result) == true)
            assert(result == i);
    }
    return NULL;
}

// Test three threads conConcurrent read and write
void unit_test_case5() {
    class ConcurrentHashMap testMap(26);
    // Initialize three threads
    pthread_t thread1, thread2, thread3;
    int ret1, ret2, ret3;
    ret1 = pthread_create(&thread1, NULL, WriteAndRead1, &testMap);
    ret2 = pthread_create(&thread2, NULL, WriteAndRead2, &testMap);
    ret3 = pthread_create(&thread3, NULL, WriteAndRead3, &testMap);
    // Wait until the three threads finish
    pthread_join(thread1, NULL);
    pthread_join(thread2, NULL);
    pthread_join(thread3, NULL);

    cout << "concurrent_hashmap test_case5 passed" << endl;
}

// Test two threads, one thread write, one thread read
void unit_test_case6() {
    class ConcurrentHashMap testMap(26);
    // Initialize two threads
    pthread_t thread1, thread2;
    int ret1, ret2;
    ret1 = pthread_create(&thread1, NULL, WriteAndRead1, &testMap);
    ret2 = pthread_create(&thread2, NULL, OnlyRead1, &testMap);
    // Wait until the two threads finish
    pthread_join(thread1, NULL);
    pthread_join(thread2, NULL);

    cout << "concurrent_hashmap test_case6 passed" << endl;
}

void run_all_testcases() {
    cout << "Start running all test cases" << endl;

    unit_test_case0();
    unit_test_case1();
    unit_test_case2();
    unit_test_case3();
    unit_test_case4();
    unit_test_case5();
    unit_test_case6();
}


