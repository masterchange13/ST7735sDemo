#include <Adafruit_GFX.h>
#include <Adafruit_ST7735.h>
#include <SPI.h>
//#include "lib/show/show.h"
#include <show.h>
#include "libraries/show/show.h"

// ST7735S 屏幕引脚 | ESP32 引脚
// VCC | 3.3V
// GND | GND
// SCL (SCK) | GPIO18
// SDA (MOSI) | GPIO23
// RES (RST) | GPIO4
// DC (A0) | GPIO2
// CS | GPIO15

// 定义引脚
#define TFT_CS     15
#define TFT_RST    4
#define TFT_DC     2

// 创建 ST7735S 对象
//Adafruit_ST7735 tft = Adafruit_ST7735(TFT_CS, TFT_DC, TFT_RST);

uint8_t currentPage = 0;

void setup() {
    // 初始化串口用于调试
    Serial.begin(11200);
    Serial.println("ST7735S Test");

    // 初始化 ST7735S 屏幕
    initTft();

    // 显示图片
    displayImageZyq();
    delay(100);

}



void loop() {
    // 主循环中可以添加更多的显示内容
    switch(currentPage){
        case 0:
            displayImage0();
            break;
        case 1:
            break;
        default:
            break;
    }
}