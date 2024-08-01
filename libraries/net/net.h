//
// Created by mao on 2024/8/1.
//

#ifndef NET_H
#define NET_H

//#include "../show/show.h"
#include "../common/common.h"

void networkSetup();
TimeData networkGetTime();
int networkTest();

// 注释掉,因为现在用有错误
class Network{
public:
    Network();
    void setup();
    TimeData getTime();
    void test();
};

#endif //ST7735SDEMO_NET_H
