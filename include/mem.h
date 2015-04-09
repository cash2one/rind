/**
 * @file mem.h
 * @author nickgu
 * @date 2014/01/28 15:19:38
 * @brief 
 *  
 **/

#ifndef  __MEM_H_
#define  __MEM_H_

#include <cstdlib>

class Buffer_t {
    public:
        // create a empty buffer.
        Buffer_t();
        // create a buffer with a pre-alloc buffer.
        Buffer_t(size_t size);
        // copy a buffer from other buffer.
        // which means that it doesn't have it's own buffer.
        Buffer_t(const Buffer_t& o);

        // free buffer if it own a buffer.
        ~Buffer_t();

        // make a reference of a existing buffer. 
        void operator = (const Buffer_t& o);
        // copy from other buffer.
        void deepcopy(const Buffer_t& o);

        // getter: get raw buffer.
        void* buffer() const  {return _buffer;}
        // getter: get used size.
        size_t size() const {return _size;}

        // require size to use.
        void require_size(size_t size);

    private:
        size_t  _size; // data size.
        size_t  _buffer_size;  // buffer size.
        void*   _buffer;
        bool    _need_release;

        void _free();
        static size_t  _get_malloc_size(size_t query_size);
};

#endif  //__MEM_H_

/* vim: set expandtab ts=4 sw=4 sts=4 tw=100: */
