/***************************************************************************
 * 
 * Copyright (c) 2014 Baidu.com, Inc. All Rights Reserved
 * 
 **************************************************************************/
 
 
 
/**
 * @file tester.h
 * @author gusimiu(com@baidu.com)
 * @date 2014/01/27 00:04:57
 * @brief 
 *  
 **/

#ifndef  __TESTER_H_
#define  __TESTER_H_

#include <cstdio>
#include <cstdlib>
#include <ctime>
#include <sys/time.h>

class Timer {
public:
    Timer() :_sum(0.0) {}

    void begin() {
        gettimeofday(&_begin_tv, NULL);
    }

    void end() {
        gettimeofday(&_end_tv, NULL);
        _sum = (_end_tv.tv_sec - _begin_tv.tv_sec) + (_end_tv.tv_usec - _begin_tv.tv_usec) * 0.000001f;
    }

    /* return unit : seconds.*/
    float cost_time() const {
        return _sum;
    }

private:
    float   _sum;

    timeval _begin_tv;
    timeval _end_tv;
};

#endif  //__TESTER_H_

/* vim: set expandtab ts=4 sw=4 sts=4 tw=100: */
