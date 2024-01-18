#include "includes/concurrent_hashmap/concurrent_hashmap.h"
#include "includes/concurrent_hashmap/unit_test_concurrent_hashmap.h"
#include "includes/disk_manager/disk_manager.h"
#include "includes/disk_manager/unit_test_disk_manager.h"
#include "includes/concurrent_lru_buffer_pool/concurrent_lru_buffer_pool.h"
#include "includes/concurrent_lru_buffer_pool/unit_test_lru_buffer_pool.h"
#include "includes/light_weighted_kvstore/light_weighted_kvstore.h"
#include "includes/light_weighted_kvstore/unit_test_light_weighted_kvstore.h"
#include<iostream>
#include<map>
#include <cassert>


int main() {
    run_all_testcases();
    disk_manager_run_all_testcases();
    lru_buffer_pool_run_all_testcases();
    kvstore_run_all_testcases();
}