/**
 * @file mem.cc
 * @author nickgu
 * @date 2014/01/28 16:59:11
 * @brief 
 *  
 **/
#include <cstdlib>
#include <cstring>

#include "mem.h"

Buffer_t::Buffer_t():
    _size(0), 
    _buffer_size(0),
    _buffer(NULL),
    _need_release(false)
{}

Buffer_t::Buffer_t(size_t size):
    _size(0), 
    _buffer_size(0),
    _buffer(NULL),
    _need_release(false)
{
    require_size(size);
}

Buffer_t::Buffer_t(const Buffer_t& o) {
    _size = o._size;
    _buffer_size = o._buffer_size;
    _buffer = o._buffer;
    _need_release = false;
}

void
Buffer_t::operator = (const Buffer_t& o) {
    _free();
    _size = o._size;
    _buffer_size = o._buffer_size;
    _buffer = o._buffer;
    _need_release = false;
}

void
Buffer_t::deepcopy (const Buffer_t& o) {
    _free();
    require_size(o.size());
    memcpy(_buffer, o.buffer(), o.size());
}


Buffer_t::~Buffer_t() {
    _free();
}

void 
Buffer_t::_free()
{
    if (_need_release) {
        if (_buffer_size>0) {
            ::free(_buffer);
            _buffer_size = 0;
            _size = 0;
        }
        _need_release = false;
    }
}

void 
Buffer_t::require_size(size_t size) {
    if (!_need_release) {
        // this is a reference data.
    }
}

size_t  
Buffer_t::_get_malloc_size(size_t query_size)
{
    // require size as [16, 128, 512]
    if (query_size < 16) {
        return 16;
    } else if (query_size < 128) {
        return 128;
    } else if (query_size < 512) {
        return 1024;
    } else {
        return ((query_size >> 9) + 1) << 9;
    }
}

/* vim: set expandtab ts=4 sw=4 sts=4 tw=100: */
