/***************************************************************************
 * 
 * Copyright (c) 2014 Baidu.com, Inc. All Rights Reserved
 * 
 **************************************************************************/
 
 
 
/**
 * @file blockfile.h
 * @author gusimiu(com@baidu.com)
 * @date 2014/01/28 12:33:05
 * @brief 
 *  
 **/

#ifndef  __BLOCKFILE_H_
#define  __BLOCKFILE_H_

class BlockFile_t {
    public:
        BlockFile_t();
        virtual ~BlockFile_t();

        void open(const char* file_name);
        void close();
        /**
         * read data from file.
         * if pos is -1, read data directly from current pos.
         * @return value: bytes read.
         */
        size_t read(ssize_t pos, 
                void* buffer, 
                size_t char_count);
        void append(const void* buffer, size_t char_count);

        size_t tell();

    private:
        int _fd;
};

#endif  //__BLOCKFILE_H_

/* vim: set expandtab ts=4 sw=4 sts=4 tw=100: */
