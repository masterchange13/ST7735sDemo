//
// Created by mao on 2024/8/1.
//

#ifndef ST7735SDEMO_COMMON_H
#define ST7735SDEMO_COMMON_H

typedef struct {
    String location;
    String weather;
    String temperature;
    String wind;
} WeatherData;

typedef struct {
    int hours;
    int minutes;
    int seconds;
    int month;
    int day;
} TimeData;

#endif //ST7735SDEMO_COMMON_H
