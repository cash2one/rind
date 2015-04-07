/***************************************************************************
 * 
 * Copyright (c) 2015 Baidu.com, Inc. All Rights Reserved
 * 
 **************************************************************************/
 
 
 
/**
 * @file rind_table.h
 * @author nickgu
 * @date 2015/04/07 23:36:26
 * @brief 
 *  
 **/

#ifndef  __RIND_TABLE_H_
#define  __RIND_TABLE_H_

class RindTable_t {
    public:
        RindTable_t();
        virtual ~RindTable_t();

        // uri->obj management.
        /**
         * set a uri->obj mapping.
         * can be used to insert or update.
         */
        bool set(const char* uri, Buffer_t obj);
        
        /**
         * remove data of uri.
         */
        void del(const char* uri);

        bool get(const char* uri, Buffer_t* out);

        // index->uri linking management.
        bool add_index(const char* index, const char* uri);
        bool remove_index(const char* index, const char* uri);
        
        // index search.
};

#endif  //__RIND_TABLE_H_

/* vim: set expandtab ts=4 sw=4 sts=4 tw=100: */
