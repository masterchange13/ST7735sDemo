// net.cpp

#include "net.h"
#include <WiFi.h>
#include <NTPClient.h>
#include <WiFiUdp.h>
#include <TimeLib.h>

const char *ssid     = "DESKTOP-IJOD4LC 3771";
const char *password = "r3Z153@1";

WiFiUDP ntpUDP;
NTPClient timeClient(ntpUDP, "pool.ntp.org", 3600 * 8, 60000);

Network::Network() {

};

void Network::setup() {
    WiFi.begin(ssid, password);
    while (WiFi.status() != WL_CONNECTED) {
        delay(500);
        Serial.print(".");
    }
    Serial.println("");
    Serial.println("WiFi connected");
    Serial.println("IP address: ");
    Serial.println(WiFi.localIP());
    timeClient.begin();
}

TimeData Network::getTime() {
    unsigned long epochTime = timeClient.getEpochTime();

    int currentHour = hour(epochTime);
    int currentMinute = minute(epochTime);
    int currentSecond = second(epochTime);

    int currentDay = day(epochTime);
    int currentMonth = month(epochTime);

    return {currentHour, currentMinute, currentSecond, currentDay, currentMonth};
}

void Network::test() {

}


// 下面是没有类的代码
void networkSetup() {
    WiFi.begin(ssid, password);
    while (WiFi.status() != WL_CONNECTED) {
        delay(500);
        Serial.print(".");
    }
    Serial.println("");
    Serial.println("WiFi connected");
    Serial.println("IP address: ");
    Serial.println(WiFi.localIP());
    timeClient.begin();
}

TimeData networkGetTime() {
    unsigned long epochTime = timeClient.getEpochTime();

    int currentHour = hour(epochTime);
    int currentMinute = minute(epochTime);
    int currentSecond = second(epochTime);

    int currentDay = day(epochTime);
    int currentMonth = month(epochTime);

    return {currentHour, currentMinute, currentSecond, currentDay, currentMonth};
}

int networkTest() {
    return 10;
}