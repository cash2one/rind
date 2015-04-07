/***************************************************************************
 * 
 * Copyright (c) 2014 Baidu.com, Inc. All Rights Reserved
 * 
 **************************************************************************/
 
 
 
/**
 * @file file_db.cc
 * @author gusimiu(com@baidu.com)
 * @date 2014/01/24 10:19:41
 * @brief 
 **/
#include <cstdio>
#include <stdexcept>
#include <string>
using namespace std;

#include <errno.h>
#include <fcntl.h>

#include "file_db.h"

const size_t SizeEachFile = 200000000;

/**
 * 存在文件中的文件记录信息
 */ 
struct __record_info_t {
    size_t size;
    // 保留字段, 用于后续升级
    size_t reserved;
};

FileDB_t::FileDB_t():
    _file_no(0),
    _record_num(0),
    _total_size(0)
{}

FileDB_t::~FileDB_t()
{
    close();
}

void 
FileDB_t::open(const char* db)
{
    _db_index_file_name = (string(db) + "/fno.ind");
    _db_name = string(db);

    FILE* db_index_fp = fopen( _db_index_file_name.c_str(), "r" );
    if (db_index_fp != NULL) {
        fscanf(db_index_fp, "%d%d%llu",  &_file_no, &_record_num, &_total_size);
    }

    char buf[256];
    if (_file_no == 0) {
        /* open one file at least. */
        _file_no = 1;
    }
    for (int i=0; i<_file_no + 1; ++i) {
        snprintf(buf, sizeof(buf), "%s/%05d.data", db, i);
        _db_fp_list[i].open(buf);
    }
}

void 
FileDB_t::close()
{
    // close index.
    if (_db_index_file_name!="") {
        FILE* index_fp = fopen(_db_index_file_name.c_str(), "w");
        fprintf(index_fp, "%d %d %llu\n", _file_no, _record_num, _total_size);
        fclose(index_fp);

        _db_index_file_name = "";
        _db_name = "";
    }

    for (int i=0; i<_file_no; ++i) {
        _db_fp_list[i].close();
    }

    _file_no = 0;
    _total_size = 0;
    _record_num = 0;
}

void
FileDB_t::get(file_pos_t pos, Buffer_t& out, bool auto_realloc)
{
    BlockFile_t& fd = _db_fp_list[ pos.fno ];
    __record_info_t record_info;
    size_t read_byte;
    read_byte = fd.read(pos.offset, (void*)&record_info, sizeof(record_info));

    // exception.
    if (record_info.size <= 0) {
        throw std::runtime_error( "data_size is zero in FileDB_t::get()" );
    }
    if (record_info.size > out.buffer_size()) {
        if (auto_realloc) {
            out.alloc(record_info.size);
            if (record_info.size > out.buffer_size()) {
                throw std::runtime_error( "data_size > buffer size. [after alloc]" );
            }
        } else {
            throw std::runtime_error( "data_size > buffer size." );
        }
    }

    fd.read(-1, out.buffer(), record_info.size);
    out.set_size(record_info.size);
    return;
}

file_pos_t 
FileDB_t::write(const Buffer_t& buffer)
{
    __record_info_t record_info;
    record_info.size = buffer.size();
    record_info.reserved = 0;

    BlockFile_t& writing_fp = _db_fp_list[_file_no - 1];

    file_pos_t fpos;
    fpos.fno = _file_no - 1;
    fpos.offset = writing_fp.tell();

    // write to file.
    writing_fp.append(&record_info, sizeof(record_info));
    writing_fp.append(buffer.buffer(), buffer.size());

    _record_num ++;
    _total_size += record_info.size + sizeof(record_info);
    if (_total_size / SizeEachFile >= (size_t)_file_no) {
        char buf[256];
        _file_no ++;
        snprintf(buf, sizeof(buf), "%s/%05d.data", _db_name.c_str(), _file_no -1);
        fprintf(stderr, "switch to new writing file [%s]\n", buf);
        _db_fp_list[ _file_no - 1 ].open(buf);
    }
    return fpos;
}

/* vim: set expandtab ts=4 sw=4 sts=4 tw=100: */
