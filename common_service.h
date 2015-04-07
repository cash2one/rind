/***************************************************************************
 * 
 * Copyright (c) 2013 Baidu.com, Inc. All Rights Reserved
 * 
 **************************************************************************/
 
 
 
/**
 * @file common_service.h
 * @author gusimiu(com@baidu.com)
 * @date 2013/12/04 00:22:55
 * @brief 
 *  
 **/

#ifndef  __COMMON_SERVICE_H_
#define  __COMMON_SERVICE_H_

#include <pthread.h>

namespace common_service {
    class CommonService_t {
        public:
            /**
             *  make a service.
             */
            CommonService_t(int thread_num = 8);
            virtual ~CommonService_t();

            /**
             *  run the service.
             */
            void run();

            /**
             *  init the whole context 
             *  when service start up.
             */
            virtual int init() = 0;

            /**
             *  handle the user request.
             */
            virtual int handle() = 0;

        private:
            int _thread_num;
            pthread_t* _thread_list;
    };
};

#endif  //__COMMON_SERVICE_H_

/* vim: set expandtab ts=4 sw=4 sts=4 tw=100: */
