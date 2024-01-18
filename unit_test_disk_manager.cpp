#include "includes/disk_manager/disk_manager.h"

void test_case0() {
    DiskManager* diskManager = new DiskManager();
    char * pageContent = new char[8192];
    long long offset = diskManager->InsertPage(pageContent);
    assert(offset == 0);
    offset = diskManager->InsertPage(pageContent);
    assert(offset == 8192);
    offset = diskManager->InsertPage(pageContent);
    assert(offset == 16384);
    diskManager->DeletePage(8192);
    offset = diskManager->InsertPage(pageContent);
    assert(offset == 8192);
    delete[] pageContent;

    std::cout << "disk_manager test_case0 passed" << std::endl;
}


void test_case1() {
    DiskManager* diskManager = new DiskManager();
    char * pageContent = new char[8192];
    long long offset = diskManager->InsertPage(pageContent);
    assert(offset == 0);
    offset = diskManager->InsertPage(pageContent);
    assert(offset == 8192);
    offset = diskManager->InsertPage(pageContent);
    assert(offset == 16384);
    diskManager->DeletePage(8192);
    offset = diskManager->InsertPage(pageContent);
    assert(offset == 8192);
    diskManager->DeletePage(0);
    offset = diskManager->InsertPage(pageContent);
    assert(offset == 0);
    diskManager->DeletePage(16384);
    offset = diskManager->InsertPage(pageContent);
    assert(offset == 16384);
    delete[] pageContent;

    std::cout << "disk_manager test_case1 passed" << std::endl;
}

void disk_manager_run_all_testcases() {
    test_case0();
    test_case1();
}