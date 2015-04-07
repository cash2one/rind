/***************************************************************************
 * 
 * Copyright (c) 2014 Baidu.com, Inc. All Rights Reserved
 * 
 **************************************************************************/
 
 
 
/**
 * @file file_kv.cc
 * @author gusimiu(com@baidu.com)
 * @date 2014/01/24 16:33:30
 * @brief 
 *  
 **/
#include "file_kv.h"
#include <string>
#include <stdexcept>
#include <algorithm>
using namespace std;

FileKV_t::FileKV_t()
{}

FileKV_t::~FileKV_t()
{}

void 
FileKV_t::open(const char* file_name)
{
    _file_db.open(file_name);
}

void 
FileKV_t::close()
{
    _file_db.close();
}

void 
FileKV_t::add(key_sign_t key, const Buffer_t& data)
{
    /**
     * 加入数据的话:
     *  * 序列化到磁盘
     *  * 修改map 
     */
    file_pos_t info = _file_db.write(data);
    _kvd[key] = info;
}

bool
FileKV_t::get(key_sign_t key, Buffer_t& out, bool auto_realloc)
{
    /**
     * just get from map.
     */
    map<key_sign_t, file_pos_t>::iterator it = _kvd.find(key);
    if ( it == _kvd.end() ) {
        return false;
    }
    file_pos_t pos = it->second;
    _file_db.get(pos, out, auto_realloc);
    return true;   
}

vector<Buffer_t> 
FileKV_t::get_multi(vector<key_sign_t> key_list)
{
    vector<Buffer_t> ret;
    vector<file_pos_t> fpos_list;
    for (size_t i=0; i<key_list.size(); ++i) {
        key_sign_t key = key_list[i];
        map<key_sign_t, file_pos_t>::iterator it = _kvd.find(key);
        if ( it == _kvd.end() ) {
            continue;
        }
        file_pos_t pos = it->second;
        fpos_list.push_back(pos);
    }
    sort(fpos_list.begin(), fpos_list.end());
    for (size_t i=0; i<fpos_list.size(); ++i) {
        if (i>0 && fpos_list[i] == fpos_list[i-1]) {
            continue;
        }
        Buffer_t data;
        _file_db.get(fpos_list[i], data, true);
        ret.push_back(data);
    }
    return ret;
}

/* vim: set expandtab ts=4 sw=4 sts=4 tw=100: */
