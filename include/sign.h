/**
 * @file include/sign.h
 * @author nickgu
 * @date 2015/04/09 14:46:48
 * @brief 
 *  
 **/

#ifndef  __SIGN_H_
#define  __SIGN_H_

#include <cstdlib>

namespace Sign{
    int create_sign(const char* psrc,int slen, size_t* output_sign);
};

#endif  //__SIGN_H_

/* vim: set expandtab ts=4 sw=4 sts=4 tw=100: */
