/***************************************************************************
 * 
 * Copyright (c) 2014 Baidu.com, Inc. All Rights Reserved
 * 
 **************************************************************************/
 
 
 
/**
 * @file file_kv.h
 * @author gusimiu(com@baidu.com)
 * @date 2014/01/24 15:44:07
 * @brief 
 *   一种key-value结构
 *      sign : 64bit
 *      file_pos : file_pos_t
 *   支持对内存key-value的操作&持久化
 *   目前内部使用std::map（红黑树），原因是
 *   这样可以确保数据量
 * 
 *   FileKV_t对象每次open一个文件后，即可确保内存操作和磁盘是同步的。 
 **/

#ifndef  __FILE_KV_H_
#define  __FILE_KV_H_

#include <map>
#include <vector>
using namespace std;

//#include <ext/hash_map>
//using namespace __gnu_cxx;

#include "file_db.h"
#include "mem.h"

typedef size_t key_sign_t;

class FileKV_t {
    public:
        FileKV_t();
        ~FileKV_t();

        void open(const char *file_name);
        void close();

        void  add(key_sign_t key, const Buffer_t& data);
        bool  get(key_sign_t key, Buffer_t& out, bool auto_realloc=false); 
        vector<Buffer_t> get_multi(vector<key_sign_t> key_list);
        
    private:
        map<key_sign_t, file_pos_t> _kvd;
        FileDB_t                    _file_db;
};

#endif  //__FILE_KV_H_

/* vim: set expandtab ts=4 sw=4 sts=4 tw=100: */
