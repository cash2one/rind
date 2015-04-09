/**
 * @file rind_table.h
 * @author nickgu
 * @date 2015/04/07 23:36:26
 * @brief 
 *  
 **/

#ifndef  __RIND_TABLE_H_
#define  __RIND_TABLE_H_

#include <map>

#include "helper.h"
#include "mem.h"

typedef size_t Rind_ID_t;
typedef size_t Rind_Sign_t;

struct Rindex_t {
    FArray_t<Rind_ID_t> _rlist;
    size_t _ordered_num;

    void add(Rind_ID_t doc_id);
    void update();
};

class RindTable_t {
    public:
        RindTable_t();
        virtual ~RindTable_t();

        // obj management.
        Rind_ID_t add(Buffer_t obj);
        void set(Rind_ID_t doc_id, Buffer_t obj);
        void del(Rind_ID_t doc_id);
        bool get(Rind_ID_t doc_id, Buffer_t* out);

        // index->uri linking management.
        bool add_index(const char* index, Rind_ID_t doc_id);
        bool remove_index(const char* index, Rind_ID_t doc_id);

        /**
         *  adding index is a latent operation.
         *  if update_index() called, then index will be rebuild.
         */
        void update_index();
        
        // index search.
        FArray_t<Rind_ID_t> search(const char** index_list, size_t num);

    private:
        typedef map<Rind_ID_t, Rindex_t> IndexDict_t;

        // temp code for debug.
        map<Rind_ID_t, Buffer_t> _data;
        IndexDict_t _index;
        Rind_ID_t _cur_id;

        Rind_ID_t _calc_index_sign(const char* index);
};

#endif  //__RIND_TABLE_H_

/* vim: set expandtab ts=4 sw=4 sts=4 tw=100: */
