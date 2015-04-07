/***************************************************************************
 * 
 * Copyright (c) 2014 Baidu.com, Inc. All Rights Reserved
 * 
 **************************************************************************/
 
 
 
/**
 * @file table.cc
 * @author gusimiu(com@baidu.com)
 * @date 2014/01/28 14:38:29
 * @brief 
 *  
 **/
#include "table.h"

size_t calc_sign(const char* s) {
    size_t ret = 0;
    size_t t = 0;
    while (*s) {
        ret ^= (((size_t)*s) << t);
        t += 7;
        t %= 56;
        s++;
    }
    return ret;
}

void 
IndexQuery_t::add(const char* index) {
    size_t index_sign = calc_sign(index);
    this->_index_list.push_back(index_sign);
}

Table_t::Table_t() {}

Table_t::~Table_t() {}

void 
Table_t::open(const char* table_path)
{
    _table_path = table_path;
    _table_index_filename = _table_path + "/index";
    _table_uri_obj_filename = _table_path;

    //_index.open(_table_index_filename);
    _uri_obj_db.open(_table_uri_obj_filename.c_str());
}

void 
Table_t::close() {
    _uri_obj_db.close();
    //_index.close();
}

/**
 * 新一个实体
 */
void 
Table_t::add(const Object_t& o)
{
    size_t uri_sign = calc_sign(o._uri.c_str());
    // insert object.
    _uri_obj_db.add(uri_sign, o._data);
    // insert index-uri.
    for (size_t i=0; i<o._index.size(); ++i) {
        _index.add_delay(calc_sign(o._index[i].c_str()), uri_sign);
    }
}

/**
 * 随机获取一个实体
 */
bool  
Table_t::get(const char* uri, 
        Buffer_t& out, bool auto_realloc)
{
    key_sign_t key_sign = calc_sign(uri);
    return _uri_obj_db.get(key_sign, out, auto_realloc);
}

bool  
Table_t::get_by_sign(size_t key_sign, 
        Buffer_t& out, bool auto_realloc)
{
    return _uri_obj_db.get(key_sign, out, auto_realloc);
}


/**
 * 批量查询一组实体
 */
vector<Buffer_t> 
Table_t::get_multi(vector<const char*> uri_list)
{
    vector<key_sign_t> key_sign_list;
    for (size_t i=0; i<uri_list.size(); ++i) {
        key_sign_list.push_back( calc_sign(uri_list[i]) );
    }
    return _uri_obj_db.get_multi(key_sign_list);
}

void 
Table_t::force_update() {
    _index.force_update();
}


void 
Table_t::indquery_uri(std::vector<size_t>& uri_list, const IndexQuery_t& query) 
{
    // do merge.
    const IndexList_t* indlist = _index.get(query.index(0));
    if (indlist == NULL) {
        return ;
    }
    for (size_t j=0; j<indlist->length(); ++j) {
        uri_list.push_back(indlist->at(j));
    }

    for (size_t i=1; i<query.size(); ++i) {
        //printf("index: %llu\n", (long long unsigned)query.index(i));
        const IndexList_t* indlist = _index.get(query.index(i));
        std::vector<size_t> next_list;
        size_t pi, pj = 0;
        while (pi<uri_list.size() && pj<indlist->length()) {
            if (uri_list[pi] == indlist->at(pj)) {
                next_list.push_back(uri_list[pi]);
                pi ++;
                pj ++;
            } else if (uri_list[pi] < indlist->at(pj)) {
                pi ++;
            } else {
                pj ++;
            }
        }

        uri_list = next_list;
    }
}

/*
void 
Table_t::query(const IndexQuery_t& query);
*/

/* vim: set expandtab ts=4 sw=4 sts=4 tw=100: */
