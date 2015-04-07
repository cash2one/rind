/***************************************************************************
 * 
 * Copyright (c) 2013 Baidu.com, Inc. All Rights Reserved
 * 
 **************************************************************************/
 
 
 
/**
 * @file common_service.cpp
 * @author gusimiu(com@baidu.com)
 * @date 2013/12/04 00:36:00
 * @brief 
 *  
 **/

#include "common_service.h"
#include "helper.h"

common_service::CommonService_t
::CommonService_t(int thread_num):
    _thread_num(thread_num)
{
    LOG("service construction..");
}

common_service::CommonService_t
::~CommonService_t() {
    LOG("service destroy..");
}

void* __inner_worker_wrapper(void* p) {
    LOG("I am a worker.");
    ((common_service::CommonService_t*)p)->handle();
    return NULL;
}

void 
common_service::CommonService_t::run()
{
    LOG("service running..");
    if (_thread_num>0) {
        _thread_list = new pthread_t[_thread_num];
        for (int i=0; i<_thread_num; ++i) {
            int ret = pthread_create(
                    _thread_list+i, NULL, __inner_worker_wrapper, (void*)(this));
            if (ret) {
                ERROR("Thread start failed : ret=%d", ret);
            }
        }

        // join.
        for (int i=0; i<_thread_num; ++i) {
            pthread_join(_thread_list[i], NULL);
        }
    }
    return ;
}

/* vim: set expandtab ts=4 sw=4 sts=4 tw=100: */
