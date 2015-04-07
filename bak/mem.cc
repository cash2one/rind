/***************************************************************************
 * 
 * Copyright (c) 2014 Baidu.com, Inc. All Rights Reserved
 * 
 **************************************************************************/
 
 
 
/**
 * @file mem.cc
 * @author gusimiu(com@baidu.com)
 * @date 2014/01/28 16:59:11
 * @brief 
 *  
 **/
#include <cstdlib>

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
    alloc(size);
}

Buffer_t::~Buffer_t() {}

void 
Buffer_t::alloc(size_t size)
{
    if (_size > 0) {
        free();
    }
    _buffer = ::malloc(size);
    _buffer_size = size;
    _need_release = true;
}

void 
Buffer_t::realloc(size_t new_size)
{
    _buffer = ::realloc(_buffer, new_size);
    _buffer_size = new_size;
    _need_release = true;
}

void 
Buffer_t::free()
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
Buffer_t::attach_buffer(void* data, size_t length)
{
    free();
    _buffer = data;
    _buffer_size = length;
    _size = 0;
    _need_release = false;
}

void 
Buffer_t::attach_data(void* data, size_t length)
{
    free();
    _buffer = data;
    _buffer_size = length;
    _size = length;
    _need_release = false;
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
