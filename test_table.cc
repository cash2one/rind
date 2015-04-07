/***************************************************************************
 * 
 * Copyright (c) 2014 Baidu.com, Inc. All Rights Reserved
 * 
 **************************************************************************/
 
 
 
/**
 * @file test_table.cc
 * @author gusimiu(com@baidu.com)
 * @date 2014/01/29 10:36:36
 * @brief 
 *  
 **/

#include <cstdio>

#include <exception>
#include <string>
#include <vector>

#include "table.h"

char big_line[512 * 1024];

Table_t* load_file(const char* file_name, const char* table_name) {
    /*
     * input file format:
     *  uri, data, index_1, index_2, ..., index_N 
     * tab-seperated.
     */
    FILE* fp = fopen(file_name, "r");
    if (!fp) {
        fprintf(stderr, "Cannot open file [%s] to read.\n", file_name);
        return NULL;
    }
    Table_t* table = new Table_t();
    try {
        table->open(table_name);
        size_t line_count = 0;
        fprintf(stderr, "Begin to build table.\n");
        while (fgets(big_line, sizeof(big_line), fp)) {
            big_line[strlen(big_line) - 1] = 0;
            vector<const char*> fields;
            fields.push_back(big_line);
            for (size_t i=0; big_line[i]!=0; ++i) {
                if (big_line[i] == '\t') {
                    big_line[i] = 0;
                    fields.push_back( big_line + i + 1 );
                }
            }

            Object_t obj;
            obj._uri = fields[0];

            size_t data_size = strlen(fields[1]);
            obj._data.alloc(data_size);
            memcpy(obj._data.buffer(), fields[1], data_size);
            obj._data.set_size(data_size);

            for (size_t i=2; i<fields.size(); ++i) {
                obj._index.push_back( fields[i] );
            }

            table->add(obj);
            line_count += 1;
            if (line_count % 100000 == 0) {
                fprintf(stderr, "Build %d lines.\n", line_count);
            }
        }
        fprintf(stderr, "Build table successfully.\n");

    } catch (exception& e) {
        fprintf(stderr, "Catch exception on loading file : [%s]\n", e.what());
        delete table;
        table = NULL;
    } 
    fclose(fp);
    table->force_update();
    return table;
}

int main(int argc, const char** argv) {
    /* usage:
     *  test_table <input_file> <table_path>
     */
    if (argc!=3) {
        fprintf(stderr, "Usage : test_table <input_file> <table_path>\n");
        return -1;
    }
    Table_t* table = load_file(argv[1], argv[2]);

    printf("Usage:\n");
    printf("uri <uri>                       : search for entity of uri=<uri>.\n");
    printf("uri_sign <uri_sign>             : search for entity of uri_sign=<uri_sign>.\n");
    printf("index <index_1> <index_2>, ...  : search for entity of index_1, index_2, ...\n");
    printf("set <key>=<value>\n");
    printf("  key_list:\n");
    printf("    output_entity : 0|1 [default 0]\n");
    char line[1024];
    printf("Query >");
    while ( fgets(line, sizeof(line), stdin) ) {
        line[strlen(line)-1] = 0;
        char* temp;
        char* token = strtok_r(line, " ", &temp);
        std::vector<std::string> token_list;
        while (1) {
            token_list.push_back(token);
            token = strtok_r(NULL, " ", &temp);
            if (token == NULL) {
                break;
            }
        }

        // process command.
        if (token_list[0] == "uri") {
            Buffer_t buffer;
            if (table->get(token_list[1].c_str(), buffer, true)) {
                string s((const char*)buffer.buffer());
                if (s.length() > buffer.size()) {
                    s = s.substr(0, buffer.size());
                }
                printf("[%u] %s\n", (unsigned)buffer.size(), s.c_str());
            } else {
                printf("NotFound uri=[%s]\n", token_list[1].c_str());
            }

        } else if (token_list[0] == "uri_sign") {
            Buffer_t buffer;
            size_t sign;
            sscanf(token_list[1].c_str(), "%llu", &sign);
            if (table->get_by_sign(sign, buffer, true)) {
                string s((const char*)buffer.buffer());
                if (s.length() > buffer.size()) {
                    s = s.substr(0, buffer.size());
                }
                printf("[%u] %s\n", (unsigned)buffer.size(), s.c_str());
            } else {
                printf("NotFound uri=[%s]\n", token_list[1].c_str());
            }

        } else if (token_list[0] == "index") {
            IndexQuery_t indquery;
            for (size_t i=1; i<token_list.size(); ++i) {
                indquery.add(token_list[i].c_str());
            }
            vector<size_t> uri_list;
            table->indquery_uri(uri_list, indquery);
            for (size_t i=0; i<uri_list.size() && i<20; ++i) {
                printf("uri_sign : %llu\n", (long long unsigned)uri_list[i]);
            }

        } else if (token_list[0] == "set") {
        }

        printf("Query >");
    }
    return 0;
}

/* vim: set expandtab ts=4 sw=4 sts=4 tw=100: */
