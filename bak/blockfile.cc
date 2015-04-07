/***************************************************************************
 * 
 * Copyright (c) 2014 Baidu.com, Inc. All Rights Reserved
 * 
 **************************************************************************/
 
 
 
/**
 * @file blockfile.cc
 * @author gusimiu(com@baidu.com)
 * @date 2014/01/28 12:33:27
 * @brief 
 *  
 **/
#include <stdexcept>
#include <string>
using namespace std;

#include <errno.h>
#include <fcntl.h>

#include "blockfile.h"

BlockFile_t::BlockFile_t(): 
    _fd(-1) 
{}

BlockFile_t::~BlockFile_t() {
    close();
}

void 
BlockFile_t::open(const char* file_name) {
    close();
    _fd = ::open(file_name, O_RDWR|O_CREAT|O_APPEND, S_IREAD|S_IWRITE);
    if (_fd == -1) {
        char buf[256];
        snprintf(buf, sizeof(buf), 
                "Cannot open file %s to read. error=[%s]", 
                file_name,
                strerror(errno));
        throw runtime_error(string(buf));
    }
    return ;
}

void 
BlockFile_t::close() {
    if (_fd != -1) {
        ::close(_fd);
    }
}

size_t
BlockFile_t::read(ssize_t pos, 
        void* buffer, 
        size_t char_count) 
{
    if (pos != -1) {
        if (lseek(_fd, pos, SEEK_SET) < 0) {
            throw runtime_error("lseek error.");
        }
    }
    ssize_t readbyte = ::read(_fd, buffer, char_count);
    if (readbyte < 0) {
        throw runtime_error("read error.");
    }
    return readbyte;
}

void 
BlockFile_t::append(const void* buffer, size_t char_count) {
    if (write(_fd, buffer, char_count) < 0) {
        throw runtime_error("write error.");
    }
}

size_t BlockFile_t::tell() {
    struct stat buf;
    fstat(_fd, &buf);
    return buf.st_size;
}

/* vim: set expandtab ts=4 sw=4 sts=4 tw=100: */
