/***************************************************************************
 * 
 * Copyright (c) 2013 Baidu.com, Inc. All Rights Reserved
 * 
 **************************************************************************/
 
 
 
/**
 * @file helper.h
 * @author gusimiu(com@baidu.com)
 * @date 2013/12/04 00:38:33
 * @brief 
 *  
 **/

#ifndef  __HELPER_H_
#define  __HELPER_H_

#include <cstdio>

#define ERROR(fmt, arg...) {fprintf(stderr, "ERROR " fmt "\n", ##arg);}
#define LOG(fmt, arg...) {fprintf(stderr, "LOG " fmt "\n", ##arg);}

#endif  //__HELPER_H_

/* vim: set expandtab ts=4 sw=4 sts=4 tw=100: */
