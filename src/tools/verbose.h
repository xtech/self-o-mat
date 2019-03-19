//
// Created by clemens on 13.03.19.
//

#ifndef SELF_O_MAT_VERBOSE_H
#define SELF_O_MAT_VERBOSE_H

#include <iostream>


#ifdef VERBOSE
#define LOG_D(TAG, MSG) std::cout << "[" << TAG  << "] " << MSG << std::endl
#else
#define LOG_D(TAG, MSG) //std::cout << TAG << MSG << std::endl
#endif

#define LOG_E(TAG, MSG) std::cerr << "[" << TAG  << "] " << MSG << std::endl


#endif //SELF_O_MAT_VERBOSE_H
