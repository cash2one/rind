/***************************************************************************
 * 
 * Copyright (c) 2014 Baidu.com, Inc. All Rights Reserved
 * 
 **************************************************************************/
 
 
 
/**
 * @file index.h
 * @author gusimiu(com@baidu.com)
 * @date 2014/01/26 09:45:14
 * @brief 
 *  
 **/

#ifndef  __INDEX_H_
#define  __INDEX_H_

#include <stdexcept>
using namespace std;

#include <vector>
using namespace std;

#include <ext/hash_map>
using namespace __gnu_cxx;

typedef size_t doc_sign_t;

class IndexList_t {
    public:
        IndexList_t();
        virtual ~IndexList_t();

        size_t length() const {
            return _num;
        }

        doc_sign_t operator[] (const size_t index) const {
            return this->at(index);
        }

        doc_sign_t at(const size_t index) const {
            if (index >= _num) {
                throw out_of_range("Index out of range.");
            }
            return _list[index];
        }

        void add_single(doc_sign_t doc_no);
        void add_batch(const vector<doc_sign_t>& doc_list);
        
        //void del(doc_sign_t doc_no);

        /**
         * 查找是否包含某个doc_no
         * 如果不存在，返回-1，否则返回下标
         */
        int search(doc_sign_t doc_no) const;

    private:
        size_t      _num;
        size_t      _size;
        doc_sign_t*   _list;
};

/**
 * 倒排索引 
 */
class Index_t {
    public:
        Index_t();
        virtual ~Index_t();
        
        void add_realtime(size_t index_sign, doc_sign_t doc_no);
        void add_delay(size_t index_sign, doc_sign_t doc_no);
        void force_update();
       
        //void del(size_t index_sign, doc_sign_t doc_no);
        const IndexList_t* get(size_t index_sign);

    private:
        hash_map<size_t, IndexList_t*> _index;

        hash_map<size_t, vector<doc_sign_t> > _delay_index;
        size_t                              _delay_pair_count;
};

#endif  //__INDEX_H_

/* vim: set expandtab ts=4 sw=4 sts=4 tw=100: */





