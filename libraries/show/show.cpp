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

// 天气
#include "../img/wea/xue.h"
#include "../img/wea/lei.h"
#include "../img/wea/shachen.h"
#include "../img/wea/wu.h"
#include "../img/wea/bingbao.h"
#include "../img/wea/yun.h"
#include "../img/wea/yu.h"
#include "../img/wea/yin.h"
#include "../img/wea/qing.h"
#include "../img/wea/xue_black.h"
#include "../img/wea/lei_black.h"
#include "../img/wea/shachen_black.h"
#include "../img/wea/wu_black.h"
#include "../img/wea/bingbao_black.h"
#include "../img/wea/yun_black.h"
#include "../img/wea/yu_black.h"
#include "../img/wea/yin_black.h"
#include "../img/wea/qing_black.h"
#include "../img/temperature.h"
#include "../img/humidity.h"
#include "../img/temperature_black.h"
#include "../img/humidity_black.h"
#include "../img/QRcode.h"

//#include "../net/net.h"
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
TimeData timeData = {6, 0, 0, 0 , 0};

WeatherData weatherData = {"CZ", "晴", "30", "n 3"};


// 仅用于实验，实验结束以后需要删除
#include <WiFi.h>
#include <NTPClient.h>
#include <WiFiUdp.h>
#include <TimeLib.h>
#include <HTTPClient.h>
#include <ArduinoJson.h>
// OTA
#include <HTTPUpdate.h>


const char *ssid     = "ChinaNet-tQ9gxh";
const char *password = "88888888";

// 初始化WiFiUDP实例
WiFiUDP ntpUDP;

// 使用中国的NTP服务器，设置时区偏移量为 UTC+8（28800秒），更新间隔为60000毫秒
NTPClient timeClient(ntpUDP, "cn.pool.ntp.org", 60 * 60 * 8, 60000);  // 使用中国的NTP服务器
HTTPClient http;

void networkSetup() {
    WiFi.begin(ssid, password);
    unsigned long startAttemptTime = millis();  // 记录开始时间
    while (WiFi.status() != WL_CONNECTED) {
        Serial.print(".");
        if (millis() - startAttemptTime > 50000) {  // 如果连接超过50秒
            Serial.println("WiFi连接超时");
            break;
        }
        delay(500);  // 延迟检查间隔
    }
    if (WiFi.status() == WL_CONNECTED) {
        Serial.println("");
        Serial.println("WiFi connected");
        Serial.println("IP address: ");
        Serial.println(WiFi.localIP());
    }
    timeClient.begin();  // 开始NTP客户端
}


TimeData networkGetTime() {
    // 确保NTP客户端更新
    timeClient.update();

    unsigned long epochTime = timeClient.getEpochTime();

    int currentHour = timeClient.getHours();
    int currentMinute = timeClient.getMinutes();
    int currentSecond = timeClient.getSeconds();

    int currentDay = day(epochTime);
    int currentMonth = month(epochTime);

    return {currentHour, currentMinute, currentSecond, currentMonth, currentDay};
}

// 从自己的服务器获取时间
TimeData networkGetTimeByServer(){
    HTTPClient http;
    http.begin("http://192.168.2.8:8080/time");
    int httpCode = http.GET();
    if (httpCode == HTTP_CODE_OK) {

        String payload = http.getString();
        DynamicJsonDocument doc(1024);

        // 解析JSON
        deserializeJson(doc, payload);

        timeData.hours = doc["hours"].as<int>();
        timeData.minutes = doc["minutes"].as<int >();
        timeData.seconds = doc["seconds"].as<int>();
        timeData.month = doc["month"].as<int>();
        timeData.day = doc["day"].as<int>();
    } else {
        Serial.println("HTTP request failed");
        timeData.hours = 6;
        timeData.minutes = 0;
        timeData.seconds = 0;
        timeData.month = 0;
        timeData.day = 0;
    }
    http.end();
    return timeData;
}

WeatherData networkGetWeather(){
    // 指定请求的URL
    http.begin("http://100.122.175.207:8080/weather");
    // 发起GET请求
    int httpCode = http.GET();
    if (httpCode == HTTP_CODE_OK) {
        String payload = http.getString();
        DynamicJsonDocument doc(1024);

        // 解析JSON
        deserializeJson(doc, payload);

        // 填充结构体
        weatherData.location = doc["location"].as<String>();
        weatherData.weather = doc["weather"].as<String>();
        weatherData.temperature = doc["temperature"].as<String>();
        weatherData.wind = doc["wind"].as<String>();

        // 打印结构体内容
        Serial.println("Weather Data:");
        Serial.print("Location: ");
        Serial.println(weatherData.location);
        Serial.print("Weather: ");
        Serial.println(weatherData.weather);
        Serial.print("Temperature: ");
        Serial.println(weatherData.temperature);
        Serial.print("Wind Speed: ");
        Serial.println(weatherData.wind);
    } else {
        Serial.println("HTTP request failed");
    }
    http.end();
    return weatherData;
}

int networkTest() {
    return 10;
}





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

int calculateDayOfWeek(int day, int month, int year) {
    if (month < 3) {
        month += 12;
        year -= 1;
    }
    int k = year % 100;
    int j = year / 100;
    int dayOfWeek = (day + 13 * (month + 1) / 5 + k + k / 4 + j / 4 + 5 * j) % 7;
    return ((dayOfWeek + 5) % 7) + 1; // 将结果转换为1-7，表示星期一到星期日
}

void drawDay(){
    tft.setTextColor(ST7735_WHITE);
    tft.setCursor(0, 130); // 改变 y 坐标以避免重叠
    tft.setTextSize(2);
    if (timeData.day < 10) tft.print('0'); tft.print(timeData.day);
    tft.print('/');
    if (timeData.month < 10) tft.print('0'); tft.print(timeData.month);
    tft.print(' ');
    int day = calculateDayOfWeek(timeData.day, timeData.month, 2024);
    switch (day) {
        case 1:
            tft.print("Mon");
            break;
        case 2:
            tft.print("Tue");
            break;
        case 3:
            tft.print("Wed");
            break;
        case 4:
            tft.print("Thu");
            break;
        case 5:
            tft.print("Fri");
            break;
        case 6:
            tft.print("Sat");
            break;
        case 7:
            tft.print("Sun");
            break;
    }
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

void drawWeather(){
    String s = weatherData.weather;
    Serial.println(s);
    if(s.equals("雪")){
        pushImage(70, 30, 50, 50,xue_black);
    }else if(s.equals("雷")){
        pushImage(70, 30, 50, 50,lei_black);
    }else if(s.equals("沙尘")){
        pushImage(70, 30, 50, 50,shachen_black);
    }else if(s.equals("雾")){
        pushImage(70, 30, 50, 50,wu_black);
    }else if(s.equals("冰雹")){
        pushImage(70, 30, 50, 50,bingbao_black);
    }else if(s.equals("多云")){
                pushImage(70, 30, 50, 50,yun_black);
    }else if(s.equals("雨")){
        pushImage(70, 30, 50, 50,yu_black);
    }else if(s.equals("阴")){
        pushImage(70, 30, 50, 50,yin_black);
    }else if(s.equals("晴")){
        pushImage(70, 30, 50, 50,qing_black);
    }else {
        tft.setCursor(70, 30); // 改变 y 坐标以避免重叠
        tft.setTextSize(2);
        tft.println("error");
    }
}

void drawDateWeek(){
    tft.setTextColor(ST7735_YELLOW);
    tft.setTextSize(2);
    tft.setCursor(10, 30); // 改变 y 坐标以避免重叠
//    tft.print(weatherData.location);
    pushImage(10, 30, 30, 30, image_CZ);

    // 画出阴天图像
//    pushImage(70, 30, 50, 50, lei_black);
    drawWeather(); // 绘制天气图标

    tft.setTextSize(2);
//    tft.setCursor(10, 70); // 改变 y 坐标以避免重叠
//    tft.print(weatherData.wind);
    tft.setCursor(10, 70); // 改变 y 坐标以避免重叠
    tft.print(weatherData.temperature);
    // 绘制摄氏度图表
    pushImage(32, 65, 20, 20, image_temperature);

    updateTimeDisplay();

    drawDay(); // 绘制日期和星期
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

// 当一天过去了，需要重新刷新界面
void updateMain(){
    if(timeData.hours == 0 && timeData.minutes == 0 && timeData.seconds == 0){
        reflashTFT();
        clearTft();
        displayInitTest();
    }
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
    networkSetup();
    timeData = networkGetTimeByServer();
    weatherData = networkGetWeather();
    Serial.println(timeClient.getFormattedTime());

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


// OTA
//固件链接，在巴法云控制台复制、粘贴到这里即可
String upUrl = "http://bin.bemfa.com/b/3BcMmEyYzZmZTg5OTdiNDg5ZGExMzhiMjMxYThmODg3ZTc=weatherClockTime.bin";

//当升级开始时，打印日志
void update_started() {
    Serial.println("CALLBACK:  HTTP update process started");
}

//当升级结束时，打印日志
void update_finished() {
    Serial.println("CALLBACK:  HTTP update process finished");
}

//当升级中，打印日志
void update_progress(int cur, int total) {
    Serial.printf("CALLBACK:  HTTP update process at %d of %d bytes...\n", cur, total);
}

//当升级失败时，打印日志
void update_error(int err) {
    Serial.printf("CALLBACK:  HTTP update fatal error code %d\n", err);
}
/**
 * 固件升级函数
 * 在需要升级的地方，加上这个函数即可，例如setup中加的updateBin();
 * 原理：通过http请求获取远程固件，实现升级
 */
void updateBin(){
    Serial.println("start update");
    WiFiClient UpdateClient;

    httpUpdate.onStart(update_started);//当升级开始时
    httpUpdate.onEnd(update_finished);//当升级结束时
    httpUpdate.onProgress(update_progress);//当升级中
    httpUpdate.onError(update_error);//当升级失败时

    t_httpUpdate_return ret = httpUpdate.update(UpdateClient, upUrl);
    switch(ret) {
        case HTTP_UPDATE_FAILED:      //当升级失败
            Serial.println("[update] Update failed.");
            break;
        case HTTP_UPDATE_NO_UPDATES:  //当无升级
            Serial.println("[update] Update no Update.");
            break;
        case HTTP_UPDATE_OK:         //当升级成功
            Serial.println("[update] Update ok.");
            break;
    }
}

// mqtt
//#include "../PubSubClient/src/PubSubClient.h"
#include <PubSubClient.h>

// 配置 MQTT 信息
const char* mqtt_server = "bemfa.com"; // MQTT 服务器地址
uint16_t mqtt_server_port = 9501;      // MQTT 服务器端口
#define ID_MQTT  "2a2c6fe8997b489da138b231a8f887e7" // MQTT 客户端 ID
const char* topic = "weatherClockTime"; // 主题名称
const char* updateTopic = "weatherClockTime"; // OTA 更新主题

WiFiClient MQTTclient; // WiFi 客户端对象
PubSubClient client(MQTTclient);

// MQTT 消息回调函数
void callback(char* topic, byte* payload, unsigned int length) {
    Serial.print("Message received [");
    Serial.print(topic);
    Serial.print("]: ");

    // 将 payload 转换为字符串
    String message;
    for (unsigned int i = 0; i < length; i++) {
        message += (char)payload[i];
    }
    Serial.println(message);

    // 处理收到的消息
    if (String(topic) == updateTopic) {
        if (message == "start") {
            Serial.println("Starting OTA update...");
            updateBin(); // 调用 OTA 更新函数
        }
    }
}

// MQTT 重连函数
void MQTT_reconnect() {
    // 如果未连接，则尝试重新连接
    while (!client.connected()) {
        Serial.print("Attempting MQTT connection...");
        // 连接 MQTT 服务器
        if (client.connect(ID_MQTT)) {
            Serial.println("connected");
            // 订阅主题
            client.subscribe(topic);
            Serial.print("Subscribed to topic: ");
            Serial.println(topic);
        } else {
            Serial.print("failed, rc=");
            Serial.print(client.state());
            Serial.println(" try again in 5 seconds");
            delay(5000);
        }
    }
}

// 初始化 MQTT
void mqttSetup() {
    client.setServer(mqtt_server, mqtt_server_port);
    client.setCallback(callback);
//    MQTT_reconnect(); // 初次连接
}

void mqttLoop(){
    if (!client.connected()) {
        MQTT_reconnect();
    }
    client.loop();
}
