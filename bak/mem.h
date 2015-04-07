/***************************************************************************
 * 
 * Copyright (c) 2014 Baidu.com, Inc. All Rights Reserved
 * 
 **************************************************************************/
 
 
 
/**
 * @file mem.h
 * @author gusimiu(com@baidu.com)
 * @date 2014/01/28 15:19:38
 * @brief 
 *  
 **/

#ifndef  __MEM_H_
#define  __MEM_H_

class Buffer_t {
    public:
        Buffer_t();
        Buffer_t(size_t size);
        ~Buffer_t();

        void alloc(size_t size);
        void realloc(size_t new_size);
        void free();

        void* buffer() const  {return _buffer;}
        size_t size() const {return _size;}
        void set_size(size_t s) {_size = s;}

        size_t buffer_size() const {return _buffer_size;}
        void attach_buffer(void* buffer, size_t buffer_size);
        void attach_data(void* data, size_t data_size);

    private:
        size_t  _size; // data size.
        size_t  _buffer_size;  // buffer size.
        void*   _buffer;
        bool    _need_release;

        static size_t  _get_malloc_size(size_t query_size);
};

#endif  //__MEM_H_

/* vim: set expandtab ts=4 sw=4 sts=4 tw=100: */
