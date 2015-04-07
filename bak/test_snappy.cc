/***************************************************************************
 * 
 * Copyright (c) 2014 Baidu.com, Inc. All Rights Reserved
 * 
 **************************************************************************/
 
 
 
/**
 * @file test_snappy.cc
 * @author gusimiu(com@baidu.com)
 * @date 2014/01/29 09:50:16
 * @brief 
 *  
 **/

#include <cstdio>
#include <cstdlib>

#include <string>
using namespace std;

#include "snappy.h"

char big_buffer[512 * 1024];

int main() {
    size_t all_input_size = 0;
    size_t all_output_size = 0;
    while (fgets(big_buffer, sizeof(big_buffer), stdin)) {
        size_t length = strlen(big_buffer);
        length += 1;
        string compressed_data;
        snappy::Compress(big_buffer, length, &compressed_data);
        float compressed_ratio = compressed_data.size() * 1.0f / length;
        printf("Compress info: size:%u -> size:%u [ ratio:%.3f ]\n", 
                (unsigned)length, 
                (unsigned)compressed_data.size(),
                compressed_ratio);
        all_input_size += length;
        all_output_size += compressed_data.size();
    }
    float ratio = all_output_size * 1.0f / all_input_size;
    fprintf(stderr, "All input data size    : %u\n", all_input_size);
    fprintf(stderr, "All output data size   : %u\n", all_output_size);
    fprintf(stderr, "Compreseed ratio       : %.1f%%\n", ratio * 100.0f);
    return 0;
}

/* vim: set expandtab ts=4 sw=4 sts=4 tw=100: */
