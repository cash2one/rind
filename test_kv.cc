/***************************************************************************
 * 
 * Copyright (c) 2014 Baidu.com, Inc. All Rights Reserved
 * 
 **************************************************************************/
 
 
 
/**
 * @file test_kv.cc
 * @author gusimiu(com@baidu.com)
 * @date 2014/01/26 23:59:52
 * @brief 
 *  
 **/

#include <string>
#include <vector>
#include <algorithm>
using namespace std;


#include "tester.h"
#include "file_kv.h"

void test_performance() {
    int doc_num = 600000;
    int max_doc_length = 32 * 1024; // each entity 27k per zhixin json.
    int query_num = 100000;
    srand((int)time(NULL));

    fprintf(stderr, "doc_num    : %d\n", doc_num);
    fprintf(stderr, "doc_length : %d\n", max_doc_length);
    fprintf(stderr, "query_num  : %d\n", query_num);

    Timer tmr;
    FileKV_t fkv;

    // generate data.
    vector< pair<size_t, string> > doc_list;
    char* doc_buffer = new char [max_doc_length+1];
    for (int ch=0; ch<max_doc_length; ++ch) {
        doc_buffer[ch] = rand() % 26 + 'a';
    }
    doc_buffer[max_doc_length] = 0;

    // test add.
    tmr.begin();
    fkv.open("db");
    fprintf(stderr, "Begin to test adding..\n");
    for (int doc_id=0; doc_id<doc_num; ++doc_id) {
        // randomly change one char
        doc_buffer[ rand() % max_doc_length ] = rand() % 26 + 'a';

        Buffer_t data;
        data.attach_data(doc_buffer, max_doc_length + 1);
        fkv.add( doc_id, data );

        if ( (doc_id + 1) % 100000 == 0) {
            fprintf( stderr, "add %d doc(s)\n", doc_id + 1);
        }
    }
    tmr.end();
    float tm_diff = tmr.cost_time();

    fprintf(stderr, "Cost time       : %.2f (s)\n", tm_diff);
    fprintf(stderr, "Doc per second  : %.2f QPS\n", doc_num * 1.0f / tm_diff);

    // test search.
    fprintf(stderr, "Preparing query..\n");
    vector<size_t> query_list;
    for (int i=0; i<query_num; ++i) {
        size_t query = rand() % doc_num;
        query_list.push_back(query);
    }

    //sort(query_list.begin(), query_list.end());
    fprintf(stderr, "Begin query..\n");
    tmr.begin();
    Buffer_t data(max_doc_length + 1);
    for (size_t i=0; i<query_list.size(); ++i) {
        size_t query = query_list[i];
        if (fkv.get(query, data)) {
            //fprintf(stderr, "search %d : DIFF.\n", query);
            //fprintf(stderr, "get : [%s]\n", doc_buffer);
            //fprintf(stderr, "ans : [%s]\n", doc_list[query].second.c_str());
        }

        if ( (i + 1) % 10000 == 0) {
            fprintf( stderr, "query %d doc(s)\n", i + 1);
            tmr.end();
            tm_diff = tmr.cost_time();
            fprintf(stderr, "Query cost time   : %.2f (s)\n", tm_diff);
            fprintf(stderr, "query per second  : %.2f QPS\n", (i+1) * 1.0f / tm_diff);
        }
    }
    data.free();
    tmr.end();
    fkv.close();
    tm_diff = tmr.cost_time();

    fprintf(stderr, "Query cost time   : %.2f (s)\n", tm_diff);
    fprintf(stderr, "query per second  : %.2f QPS\n", query_num * 1.0f / tm_diff);


    delete [] doc_buffer;
    return ;
}

int main() {
    test_performance();

    return 0;
}

/* vim: set expandtab ts=4 sw=4 sts=4 tw=100: */
