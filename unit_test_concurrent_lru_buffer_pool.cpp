#include "includes/concurrent_lru_buffer_pool/concurrent_lru_buffer_pool.h"
#include <string>
#include <pthread.h>
#include <cassert>

using namespace std;

void lru_test_case0() {
    class ConcurrentLruBufferPool testPool(4);

    cout << "concurrent_lru_buffer_pool test_case0 passed" << endl;

    char* pageContent = new char[8192];
    long long offset, offset2, offset3, offset4, offset5, offset6;
    offset = testPool.AllocatePageInDisk();
    assert(offset == 0);
    testPool.PutPageContent(offset, pageContent);
    testPool.PutPageContent(offset, pageContent);

    offset2 = testPool.AllocatePageInDisk();
    assert(offset2 == 8192);
    testPool.PutPageContent(offset2, pageContent);

    offset3 = testPool.AllocatePageInDisk();
    assert(offset3 == 16384);
    testPool.PutPageContent(offset3, pageContent);

    offset4 = testPool.AllocatePageInDisk();
    assert(offset4 == 24576);
    testPool.PutPageContent(offset4, pageContent);

    testPool.PutPageContent(offset, pageContent);
    testPool.PutPageContent(offset2, pageContent);
    testPool.PutPageContent(offset3, pageContent);
    testPool.PutPageContent(offset4, pageContent);

    offset5 = testPool.AllocatePageInDisk();
    testPool.PutPageContent(offset5, pageContent);

    testPool.GetPageContent(0, pageContent);
    testPool.GetPageContent(16384, pageContent);
    testPool.GetPageContent(8192, pageContent);

    testPool.DeletePageContent(0);

    offset6 = testPool.AllocatePageInDisk();
    testPool.PutPageContent(offset6, pageContent);

};

void lru_buffer_pool_run_all_testcases() {
    lru_test_case0();
}