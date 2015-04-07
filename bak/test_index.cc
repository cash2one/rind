/***************************************************************************
 * 
 * Copyright (c) 2014 Baidu.com, Inc. All Rights Reserved
 * 
 **************************************************************************/
 
 
 
/**
 * @file test_index.cc
 * @author gusimiu(com@baidu.com)
 * @date 2014/01/26 15:18:28
 * @brief 
 *  
 **/

#include "tester.h"
#include "index.h"

void test_performance() {
    int doc_num = 5000000;
    int index_range = 10000000;
    int index_each_doc = 30; // final output²î²»¶à 25avps per entity.

    fprintf(stderr, "doc_num        : %d\n", doc_num);
    fprintf(stderr, "index_range    : %d\n", index_range);
    fprintf(stderr, "index_each_doc : %d\n", index_each_doc);

    Timer tmr;
    Index_t index;
    // generate data.
    tmr.begin();
    for (int doc_id=1; doc_id<=doc_num; ++doc_id) {
        for (int j=0; j<index_each_doc; ++j) {
            unsigned index_key = rand();
            index_key %= index_range;
            index.add_delay(index_key, doc_id);
        }

        if (doc_id % 100000 == 0) {
            fprintf( stderr, "add %d doc(s)\n", doc_id);
        }
    }
    index.force_update();
    tmr.end();
    float tm_diff = tmr.cost_time();
    fprintf(stderr, "Cost time       : %.2f (s)\n", tm_diff);
    fprintf(stderr, "Doc per second  : %.2f QPS\n", doc_num * 1.0f / tm_diff);
    fprintf(stderr, "Pair per second : %.2f QPS\n", (doc_num * index_each_doc) * 1.0f/ tm_diff);

    // test.

    return ;
}

int main() {
    srand((int)time(NULL));
    test_performance();
    return 0;
}

/* vim: set expandtab ts=4 sw=4 sts=4 tw=100: */
