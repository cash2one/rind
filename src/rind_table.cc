/**
 * @file rind_table.cc
 * @author nickgu
 * @date 2015/04/08 15:48:50
 * @brief 
 *  
 **/

#include "rind_table.h"
#include "sign.h"

#include <cstring>

Rind_ID_t _calc_index_sign(const char* index) {
    Rind_ID_t s;
    Sign::create_sign(index, strlen(index), &s);
    return s;
}

RindTable_t::RindTable_t() {}

RindTable_t::~RindTable_t() {
    for (int i=0; i<_data.size(); ++i) {
        if (_data[i]) {
            delete _data[i];
        }
    }
}

Rind_ID_t 
RindTable_t::add(const Buffer_t& obj) {
    Buffer_t *copy_one = new Buffer_t(obj);
    _data.push_back(copy_one);
    return _data.size() - 1;
}

void 
RindTable_t::set(Rind_ID_t doc_id, const Buffer_t& obj) {
    if (doc_id < _data.size()) {
        if (_data[doc_id]) {
            _data[doc_id]->deepcopy(obj);
        } else {
            _data[doc_id] = new Buffer_t(obj);
        }
    }
}

void 
RindTable_t::del(Rind_ID_t doc_id) {
    if (doc_id < _data.size()) {
        if (_data[doc_id]) {
            delete _data[doc_id];
            _data[doc_id] = NULL;
        }
    }
}

bool 
RindTable_t::get(Rind_ID_t doc_id, Buffer_t* out) {
    if (doc_id < _data.size()) {
        out->deepcopy(*_data[doc_id]);
        return true;
    }
    return false;
}

// index->doc_id linking management.
bool 
RindTable_t::add_index(const char* index, Rind_ID_t doc_id) {
    Rind_Sign_t index_sign = _calc_index_sign(index);
    _index[index_sign].add(doc_id);
    //LOG_NOTICE("add_index: [%s]->%llu sign=%llu", index, doc_id, index_sign);
}

bool 
RindTable_t::remove_index(const char* index, Rind_ID_t doc_id) {
    // TODO.
    LOG_ERROR("RindTable_t::remove_index is not realized.");
    return false;
}

void
RindTable_t::update_index() {
    for (IndexDict_t::iterator it = _index.begin();
            it!=_index.end(); ++it )
    {
        it->second.update();
    }
}

// index search.
FArray_t<Rind_ID_t> search(const char** index_list, size_t num) {
    FArray_t<RindTable_t> ret;
}

Rindex_t::Rindex_t() {
    _ordered_num = 0;
}

void
Rindex_t::add(Rind_ID_t doc_id) {
    //LOG_NOTICE("add: %llu", doc_id);
    _rlist.push_back(doc_id);
}

size_t 
Rindex_t::memory_size() const {
    return sizeof(_ordered_num) + _rlist.memory_size();
}

void 
Rindex_t::update() {
    if (_ordered_num < _rlist.size()) {
        sort(_rlist.buffer(), _rlist.buffer()+_rlist.size());
        size_t p = 1;
        for (size_t i=1; i<_rlist.size(); ++i) {
            if (_rlist[i] != _rlist[i-1]) {
                if (i < p) {
                    _rlist[p] =  _rlist[i];
                }
                p++;
            }
        }
        _rlist.set_size(p);
        _ordered_num = _rlist.size();
    } 
}

bool
RindTable_t::get_index(const char* index, const Rindex_t** out) const {
    Rind_Sign_t index_sign = _calc_index_sign(index);
    IndexDict_t::const_iterator it = _index.find(index_sign);
    if (it == _index.end()) {
        *out = NULL;
        return false;
    }
    *out = &(it->second);
    return true;
}

size_t 
RindTable_t::memory_size() const {
    size_t data_size = 0;
    size_t index_size = 0;
    size_t index_count = 0;
    for (int i=0;i<_data.size(); ++i) {
        data_size += _data[i]->buffer_size();
    }
    for (IndexDict_t::const_iterator it = _index.begin();
            it!=_index.end(); ++it )
    {
        index_size += it->second.memory_size();
        index_count += it->second._rlist.size();
    }
    LOG_NOTICE("index_lemma : %llu", _index.size());
    LOG_NOTICE("index_count : %llu", index_count);
    LOG_NOTICE("data_size=%s index_size=%s", 
            size_display(data_size).c_str(), 
            size_display(index_size).c_str());
    return data_size + index_size;
}

/* vim: set expandtab ts=4 sw=4 sts=4 tw=100: */
