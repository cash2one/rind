/***************************************************************************
 * 
 * Copyright (c) 2014 Baidu.com, Inc. All Rights Reserved
 * 
 **************************************************************************/
 
 
 
/**
 * @file file_db.h
 * @author gusimiu(com@baidu.com)
 * @date 2014/01/24 10:22:59
 * @brief 
 *      文件db接口，支持：
 *          1. 打开、关闭一个文件数据库
 *          2. 根据file_pos_t从中读取一条记录。
 *          3. 写入一条记录，并且获取到file_pos_t
 **/

#ifndef  __FILE_DB_H_
#define  __FILE_DB_H_

#include <string>
using namespace std;

#include <fcntl.h>
#include <unistd.h>

#include "mem.h"
#include "blockfile.h"

/** 
 * 封装文件位置的结构体
 */
struct file_pos_t {
    size_t fno:12;
    size_t offset:52;

    bool operator == (const file_pos_t& o) const {
        return fno == o.fno && offset == o.offset;
    }

    bool operator < (const file_pos_t & o) const {
        if (fno == o.fno) {
            return offset < o.offset;
        }
        return fno < o.fno;
    }
};

#define MaxOpenFileNum (4096)

class FileDB_t {
    public:
        FileDB_t();
        virtual ~FileDB_t();

        void open(const char* db);
        void close();
        
        void get(file_pos_t pos, Buffer_t& out, bool auto_realloc=false);

        file_pos_t write(const Buffer_t& buffer);

    private:
        string  _db_index_file_name;
        string  _db_name;

        BlockFile_t   _db_writing_fp;
        BlockFile_t   _db_fp_list[MaxOpenFileNum];

        int         _file_no;
        int         _record_num;
        size_t      _total_size;
};

#endif  //__FILE_DB_H_

/* vim: set expandtab ts=4 sw=4 sts=4 tw=100: */
