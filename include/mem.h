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

/**
 *  used as a stack variable.
 */
class Buffer_t {
    public:
        // create a new memory buffer.
        Buffer_t();
        // create a buffer with a pre-alloc buffer.
        Buffer_t(size_t size);
        // free buffer.
        ~Buffer_t();

        Buffer_t(const Buffer_t& o);
        void deepcopy(const Buffer_t& o);

        // getter: get raw buffer.
        void* buffer() const { return _buffer; }
        // getter: get used size.
        size_t size() const { return _size; }
        // setter: set size used.
        void set_size(size_t s);
        // require size to use.
        void require_size(size_t size);

    private:
        void operator = (const Buffer_t& o) {};

        size_t  _buffer_size;  // buffer size.
        size_t  _size;
        int  _refer_count;  // refer counter, free itself when counter is zero.
        void*   _buffer;

        void _free();
        static size_t  _get_malloc_size(size_t query_size);
};

#endif  //__MEM_H_

/* vim: set expandtab ts=4 sw=4 sts=4 tw=100: */
