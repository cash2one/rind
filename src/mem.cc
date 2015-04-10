/**
 * @file mem.cc
 * @author nickgu
 * @date 2014/01/28 16:59:11
 * @brief 
 *  
 **/
#include <cstdlib>
#include <cstring>

#include <stdexcept>
#include "mem.h"

Buffer_t::Buffer_t() :
    _buffer_size(0),
    _size(0),
    _buffer(NULL)
{}

Buffer_t::Buffer_t(size_t size) :
    _buffer_size(0),
    _size(0),
    _buffer(NULL)
{
    require_size(size);
}

Buffer_t::Buffer_t(const Buffer_t& o) {
    _buffer_size = 0;
    _size = 0;
    _buffer = NULL;
    _free();
    require_size(o._buffer_size);
    _size = o._size;
    memcpy(_buffer, o.buffer(), o._buffer_size);
}

void 
Buffer_t::deepcopy(const Buffer_t& o) {
    require_size(o._buffer_size);
    _size = o._size;
    memcpy(_buffer, o._buffer, o._buffer_size);
}

Buffer_t::~Buffer_t() {
    _free();
}

void 
Buffer_t::_free()
{
    if (_buffer_size>0) {
        ::free(_buffer);
    }
    _buffer_size = 0;
    _size = 0;
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

void 
Buffer_t::require_size(size_t size) {
    if (_buffer_size == 0) { // alloc.
        _buffer_size = size;
        _buffer = malloc(size);
    } else {
        _buffer_size = size;
        _buffer = realloc(_buffer, size);
    }
}

void Buffer_t::set_size(size_t new_size) {
    _size = new_size;
}

/* vim: set expandtab ts=4 sw=4 sts=4 tw=100: */
