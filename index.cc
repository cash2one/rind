/***************************************************************************
 * 
 * Copyright (c) 2014 Baidu.com, Inc. All Rights Reserved
 * 
 **************************************************************************/
 
 
 
/**
 * @file index.cc
 * @author gusimiu(com@baidu.com)
 * @date 2014/01/26 09:51:25
 * @brief 
 *  
 **/

#include "index.h"

const int InitBlockNum = 8;
const int BlockNum = 128;
const doc_sign_t DelDoc_Flag = doc_sign_t(-1);
const size_t ForceUpdateCount = 100000000;

IndexList_t::IndexList_t() 
{
    _num = 0;
    _size = InitBlockNum;
    _list = (doc_sign_t*)malloc( _size * sizeof(doc_sign_t) );
}

IndexList_t::~IndexList_t() 
{
    if (_list) {
        free(_list);
    }
    _size = 0;
    _num = 0;
}

int 
IndexList_t::search(doc_sign_t doc_no) const 
{
    int left = 0;
    int right = _num;

    while (left < right) {
        int mid = (left + right ) / 2;
        if (_list[mid] == doc_no) {
            return mid;
        } else if (_list[mid] < doc_no) {
            left = mid+1;
        } else {
            right = mid;
        }
    }
    return -1;
}

void 
IndexList_t::add_single(doc_sign_t doc_no)
{
    int idx = search(doc_no);
    if ( idx >= 0 ) {
        return ;
    }

    // 实时加入一个docid
    //  1. 分配内存
    //  2. 加入
    //  3. 重排序
    if (_size == _num) {
        _size = _size + BlockNum;
        _list = (doc_sign_t*)realloc( _list, _size * sizeof(doc_sign_t) );
    }

    if (_list) {
        _list[_num ++] = doc_no;
        sort(_list, _list + _num);
    }
}

void 
IndexList_t::add_batch(const vector<doc_sign_t>& v)
{
    for (size_t i=0; i<v.size(); ++i) {
        doc_sign_t doc_no = v[i];
        int idx = search(doc_no);
        if ( idx >= 0 ) {
            continue;
        }

        // 实时加入一个docid
        //  1. 分配内存
        //  2. 加入
        //  3. 重排序
        if (_size == _num) {
            _size = _size + BlockNum;
            _list = (doc_sign_t*)realloc( _list, _size * sizeof(doc_sign_t) );
        }

        if (!_list) {
            throw runtime_error("realloc failed!");
        }
        _list[_num ++] = doc_no;
    }
    sort(_list, _list + _num);
}

/*
void 
IndexList_t::del(doc_sign_t doc_no)
{
    int idx = search(doc_no);
    if ( idx < 0 ) {
        return ;
    }

    // 设置为删除
    _list[idx] = DelDoc_Flag;
}
*/


Index_t::Index_t():
    _delay_pair_count(0)
{}

Index_t::~Index_t()
{}

void 
Index_t::add_realtime(size_t index_sign, doc_sign_t doc_no)
{
    if (_index.find(index_sign) == _index.end()) {
        // add new index sign.
        _index[index_sign] = new IndexList_t();
    }
    _index[index_sign]->add_single(doc_no);
    return ;
}

void 
Index_t::add_delay(size_t index_sign, doc_sign_t doc_no)
{
    _delay_index[index_sign].push_back(doc_no);
    _delay_pair_count ++;
    if (_delay_pair_count % ForceUpdateCount == 0) {
        force_update();
    }
    return ;
}

void 
Index_t::force_update() {
    fprintf(stderr, "Force update..\n");
    for (hash_map<size_t, vector<doc_sign_t> >::iterator it = _delay_index.begin();
            it!=_delay_index.end(); ++it) 
    {
        size_t key = it->first;
        vector<doc_sign_t> &v = it->second;

        if (_index.find(key) == _index.end() ) {
            _index[key] = new IndexList_t();
        }
        _index[key]->add_batch(v);
    }
    _delay_index.clear();
    _delay_pair_count = 0;
    fprintf(stderr, "Force update over.\n");
}


/*
void 
Index_t::del(size_t index_sign, doc_sign_t key_sign)
{
    if (_index.find(index_sign) != _index.end()) {
        _index[index_sign].del(key_sign);
    }
}
*/

const IndexList_t* 
Index_t::get(size_t index_sign)
{
    if (_index.find(index_sign) == _index.end()) {
        return NULL;
    }
    return _index[ index_sign ];
}

/* vim: set expandtab ts=4 sw=4 sts=4 tw=100: */
