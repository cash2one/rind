/**
 * @file test_table.cc
 * @author nickgu
 * @date 2014/01/29 10:36:36
 * @brief 
 *  
 **/

#include <cstdio>

#include <exception>
#include <string>
#include <vector>

#include "rind_table.h"

char big_line[512 * 1024];

RindTable_t* load_file(const char* file_name) {
    /*
     * input file format:
     *  data, index_1, index_2, ..., index_N 
     * tab-seperated.
     */
    FILE* fp = fopen(file_name, "r");
    if (!fp) {
        fprintf(stderr, "Cannot open file [%s] to read.\n", file_name);
        return NULL;
    }
    RindTable_t* table = new RindTable_t();
    try {
        size_t line_count = 0;
        fprintf(stderr, "Begin to build table.\n");
        while (fgets(big_line, sizeof(big_line), fp)) {
            vector<string> fields;
            big_line[strlen(big_line)-1] = 0;
            split(big_line, "\t", fields);

            Buffer_t obj;
            //obj.set(fields[0].c_str(), fields[0].size(), 0);
            Rind_ID_t doc_id = table->add(obj);

            for (size_t i=1; i<fields.size(); ++i) {
                table->add_index(fields[i].c_str(), doc_id);
            }

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
    //table->force_update();
    return table;
}

int main(int argc, const char** argv) {
    /* usage:
     *  test_table <input_file>
     */
    if (argc!=2) {
        fprintf(stderr, "Usage : test_table <input_file>\n");
        return -1;
    }
    RindTable_t* table = load_file(argv[1]);

    /*
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
    */
    return 0;
}

/* vim: set expandtab ts=4 sw=4 sts=4 tw=100: */