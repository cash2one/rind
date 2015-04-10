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
#include "helper.h"

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
            obj.require_size(fields[0].size() + 1);
            memcpy(obj.buffer(), fields[0].c_str(), fields[0].size()+1);
            obj.set_size(fields[0].size() + 1);
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
    LOG_NOTICE("Dict load over. dict_size=%u", table->size());
    table->update_index();
    table->memory_size();
    return table;
}

void show_command_help() {
    printf("Usage:\n");
    printf("\tdoc <doc_id> : search doc of doc_id.\n");
    printf("\tindex <index_1> <index_2>, ...  : search for entity of index_1, index_2, ...\n");
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

    char line[1024];
    show_command_help();
    printf("Query >");
    while ( fgets(line, sizeof(line), stdin) ) {
        line[strlen(line)-1] = 0;
        std::vector<std::string> token_list;
        split(line, " ", token_list);

        // process command.
        if (token_list.size()<=0) {
            show_command_help();
        } else if (token_list[0] == "doc") {
            Buffer_t buffer;
            Rind_ID_t doc_id = (Rind_ID_t)atoi(token_list[1].c_str());
            if (table->get(doc_id, &buffer)) {
                string s((const char*)buffer.buffer());
                printf("Found\n[%u] %s\n", (unsigned)buffer.size(), s.c_str());
            } else {
                printf("NotFound doc_id=[%llu]\n", doc_id);
            }

        } else if (token_list[0] == "index") {
            Buffer_t temp;
            for (size_t i=1; i<token_list.size(); ++i) {
                const char* index = token_list[i].c_str();
                const Rindex_t* out;
                if (table->get_index(index, &out)) {
                    printf("index of [%s], num=%d(unordered=%d), top_5:\n", index, out->_ordered_num, out->_rlist.size()-out->_ordered_num);
                    for (int j=0; j<out->_ordered_num && j<5; ++j) {
                        Rind_ID_t doc_id = out->_rlist[j];
                        table->get(doc_id, &temp);
                        printf("[%llu]: %s\n", doc_id, temp.buffer());
                    }
                } else {
                    printf("index[%s] not found.\n", index);
                }
            }
        } else {
            show_command_help();
        }
        printf("Query >");
    }

    delete table;
    return 0;
}

/* vim: set expandtab ts=4 sw=4 sts=4 tw=100: */
