/***************************************************************************
 * 
 * Copyright (c) 2014 Baidu.com, Inc. All Rights Reserved
 * 
 **************************************************************************/
 
 
 
/**
 * @file table.h
 * @author gusimiu(com@baidu.com)
 * @date 2014/01/28 12:40:39
 * @brief 
 *      完整的table接口，定义对一个表的完整操作。
 *      对象定义：
 *          Object:
 *              uri             : 对象的唯一标示符 [必须]
 *              object_data     : 对象的数据 [非必须]
 *              index_list      : 对象的所有索引信息 [非必须]
 **/

#ifndef  __TABLE_H_
#define  __TABLE_H_

#include <string>
#include <vector>

#include "mem.h"
#include "file_kv.h"
#include "index.h"

struct Object_t {
    string          _uri;
    vector<string>  _index;
    Buffer_t        _data;
};

class IndexQuery_t {
    public:
        void add(const char* index);
        size_t size() const {return _index_list.size();}
        size_t index(size_t ind) const {return _index_list.at(ind);}

    private:
        std::vector<size_t> _index_list;
};

class Table_t {
    public:
        Table_t();
        virtual ~Table_t();

        void open(const char* table_path);
        void close();

        /**
         * 新一个实体
         */
        void add(const Object_t& o);

        /**
         * 获取一个实体
         */
        bool get(const char* uri, 
                Buffer_t& out, bool auto_realloc=false); 

        /**
         * 获取一个实体by sign.
         */
        bool get_by_sign(size_t uri_sign, 
                Buffer_t& out, bool auto_realloc=false); 

        /**
         * 批量查询一组实体
         * NOTICE: 用户端需要release object
         */
        vector<Buffer_t> get_multi(vector<const char*> uri_list);

        void force_update();

        /**
         *  search for uri-list by index-query.
         */
        void indquery_uri(std::vector<size_t>& uri_list, 
                            const IndexQuery_t& query);
        //void query(const IndexQuery_t& query);

    private:
        string      _table_path;
        string      _table_uri_obj_filename;
        string      _table_index_filename;

        FileKV_t    _uri_obj_db;
        Index_t     _index;
};

#endif  //__TABLE_H_

/* vim: set expandtab ts=4 sw=4 sts=4 tw=100: */
