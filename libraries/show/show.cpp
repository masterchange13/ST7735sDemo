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

#include "../net/net.h"
#include "../common/common.h"

#include "../img/temperature/temperature_image.h"
#include "../img/CZ/CZ.h"

// 定义引脚
#define TFT_CS     15
#define TFT_RST    4
#define TFT_DC     2

// 创建 ST7735S 对象
Adafruit_ST7735 tft = Adafruit_ST7735(TFT_CS, TFT_DC, TFT_RST);
// 创建网络对象
//Network net = Network();

uint16_t height = 160;
uint16_t width = 128;

//typedef struct {
//    String location;
//    String weather;
//    String temperature;
//    String wind;
//} WeatherData;
//
//typedef struct {
//    int hours;
//    int minutes;
//    int seconds;
//    int month;
//    int day;
//} TimeData;
// 创建TimeData并初始化
TimeData timeData = {18, 17, 0, 0 , 0};

WeatherData weatherData = {"CZ", "sun", "30", "n 3"};

void initTft();
void clearTft();
void drawWeatherPage();
void displayImageZyq();
void reflashTFT();
void drawDateWeek();
void updateTimeDisplay();
void updateClock();

void displayImage0();
void displayImage1();
void displayImage2();
void displayImage3();
void displayImageTest();
void displayInitTest();

// 初始化屏幕
void initTft(){
    // 初始化 ST7735 屏幕
    tft.initR(INITR_BLACKTAB);
    tft.fillScreen(ST77XX_BLACK);
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

void initWeatherData(){
    weatherData.location = "CZ";
    weatherData.weather = "Clod";
    weatherData.temperature = "28℃";
    weatherData.wind = "earth-south 3";
}

void drawTitle(){
    tft.setTextColor(ST7735_WHITE);
    tft.setTextSize(2);
    tft.setCursor(0, 0);
    tft.print("  welcome");

    tft.fillRect(0, 20, tft.width(), 5, ST7735_WHITE);
}

void updateTimeDisplay() {
    tft.fillRect(0, 100, tft.width(), 30, ST7735_BLACK); // 清除之前的时间显示
    tft.setTextSize(3);
    tft.setCursor(0, 100);
    tft.setTextColor(ST7735_WHITE);
    if (timeData.hours < 10) tft.print('0'); tft.print(timeData.hours);
    tft.setTextColor(ST7735_YELLOW);
    tft.print(':');
    if (timeData.minutes < 10) tft.print('0'); tft.print(timeData.minutes);

    // 更新秒数显示 需要改变位置
    tft.setTextColor(ST7735_RED);
    tft.setTextSize(2);
    tft.setCursor(100, 115);
    if (timeData.seconds < 10) tft.print('0'); tft.print(timeData.seconds);
}

void drawDateWeek(){
    tft.setTextColor(ST7735_YELLOW);
    tft.setTextSize(2);
//    tft.setCursor(10, 30); // 改变 y 坐标以避免重叠
//    tft.print(weatherData.location);
    pushImage(10, 30, 30, 30, image_CZ);

    // 画出阴天图像
    pushImage(70, 30, 50, 50, lei_black);

    tft.setTextSize(2);
//    tft.setCursor(10, 70); // 改变 y 坐标以避免重叠
//    tft.print(weatherData.wind);
    tft.setCursor(10, 70); // 改变 y 坐标以避免重叠
    tft.print(weatherData.temperature);
    // 绘制摄氏度图表
    pushImage(32, 65, 20, 20, image_temperature);

    updateTimeDisplay();

    tft.setTextColor(ST7735_WHITE);
    tft.setCursor(0, 130); // 改变 y 坐标以避免重叠
    tft.setTextSize(2);
    tft.print("8/1  Thu");
}


void updateClock() {
    timeData.seconds++;
    if (timeData.seconds >= 60) {
        timeData.seconds = 0;
        timeData.minutes++;
        if (timeData.minutes >= 60) {
            timeData.minutes = 0;
            timeData.hours++;
            if (timeData.hours >= 24) {
                timeData.hours = 0;
            }
        }
    }

    // 更新显示
    updateTimeDisplay();
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

void displayInitTest(){
//    net.test();
//    networkSetup();
//    timeData = networkGetTime();
//    Serial.println(net.test());

//    initWeatherData();
    drawTitle();
    drawDateWeek();
}

// 绘制实况天气页面
void drawWeatherPage(){
//    //清空屏幕
//    reflashTFT();
//    //绘制标题
//    drawTitle();
//    //绘制时间、日期、星期
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