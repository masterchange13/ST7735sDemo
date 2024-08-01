//
// Created by mao on 2024/7/28.
//

#include "show.h"
#include <Adafruit_GFX.h>
#include <Adafruit_ST7735.h>
#include <SPI.h>
#include "../img/zyq/img_zyq.h"
#include "../img/page0/llx.h"
#include "../img/page1/page1.h"
#include "../img/page2/page2.h"
#include "../img/page3/page3.h"
#include "../img/page_test/page_test.h"

// 宇航员
#include "../img/yuhangyuan/yuhangyuan0.h"

// 天气
#include "../img/wea/lei_black.h"

// 定义引脚
#define TFT_CS     15
#define TFT_RST    4
#define TFT_DC     2

// 创建 ST7735S 对象
Adafruit_ST7735 tft = Adafruit_ST7735(TFT_CS, TFT_DC, TFT_RST);

// 初始化屏幕
void initTft(){
    // 初始化 ST7735 屏幕
    tft.initR(INITR_BLACKTAB);
    tft.fillScreen(ST77XX_BLACK);
}

// 绘制实况天气页面
void drawWeatherPage(){
////    清空屏幕 tftUtil
//    reflashTFT();
////    绘制标题 task
//    drawTitle();
////    绘制时间、日期、星期
//    drawDateWeek();
//    //绘制温湿度图标
//    if(backColor == BACK_BLACK){
//        tft.pushImage(7,235,40,40,temperature_black);
//        tft.pushImage(5,275,40,40,humidity_black);
//    }else{
//        tft.pushImage(7,235,40,40,temperature);
//        tft.pushImage(5,275,40,40,humidity);
//    }
//    // 绘制天气相关内容
//    drawWeatherContent();
}

// 刷新局部区域函数
void pushImage(uint16_t x, uint16_t y, uint16_t w, uint16_t h, const uint16_t *image) {
    // 设置地址窗口
    tft.setAddrWindow(x, y, x + w - 1, y + h - 1);

    // 开始写入数据
    tft.startWrite();
    for (uint16_t j = 0; j < h; j++) {
        for (uint16_t i = 0; i < w; i++) {
            tft.writePixel(x + i, y + j, image[j * w + i]);
        }
    }
    tft.endWrite();
}

void clearTft(){
    tft.fillScreen(ST77XX_BLACK);
}

uint16_t imageSize = 128 * 160;
void displayImageZyq() {
    // 设置光标到左上角
    tft.setAddrWindow(0, 0, 128, 160);

    // 将图片数据写入屏幕
    for (uint16_t i = 0; i < imageSize; i++) {
        tft.pushColor(img_zyq[i]);
    }
}

void reflashTFT(){
    // 设置光标到左上角
    tft.setAddrWindow(0, 0, 128, 160);
    for (uint16_t i = 0; i < imageSize; i++) {
        tft.pushColor(0x0000); // 设置为黑色
    }
}

void drawDateWeek(){
    tft.setTextColor(ST7735_BLUE);
    tft.setTextSize(2);
    tft.setCursor(12,0);
    tft.print("2024-07-28");
}

void displayImage0(){
    clearTft();
    // 设置光标到左上角
    tft.setAddrWindow(0, 0, 128, 160);

    // 将图片数据写入屏幕
    for (uint16_t i = 0; i < imageSize; i++) {
        tft.pushColor(image_llx[i]);
    }
}

void displayImage1(){
    clearTft();
    // 设置光标到左上角
    tft.setAddrWindow(0, 0, 128, 160);

    // 将图片数据写入屏幕
    for (uint16_t i = 0; i < imageSize; i++) {
        tft.pushColor(image_page1[i]);
    }
}

void displayImage2(){
    clearTft();
    // 设置光标到左上角
    tft.setAddrWindow(0, 0, 128, 160);

    // 将图片数据写入屏幕
    for (uint16_t i = 0; i < imageSize; i++) {
        tft.pushColor(image_page2[i]);
    }
    pushImage(0, 0, 60, 100, image_page_test);
    pushImage(60, 60, 60, 100, image_page_test);
}


void displayImage3(){
    clearTft();
    // 设置光标到左上角
    tft.setAddrWindow(0, 0, 128, 160);

    // 将图片数据写入屏幕
    for (uint16_t i = 0; i < imageSize; i++) {
        tft.pushColor(image_page3[i]);
    }
}

void displayImageTest(){
    tft.fillScreen(ST77XX_BLACK);

    pushImage(30, 30, 60, 100, image_page_test);


}

void displayIimeTest(int time){
    drawDateWeek();
}

