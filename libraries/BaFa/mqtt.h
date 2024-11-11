//
// Created by mao on 2024/11/9.
//

#ifndef MQTT_H
#define MQTT_H

//void callback(char* topic, byte* payload, unsigned int length);
//void reconnect();
void MQTT_reconnect();
void mqttSetup();
void mqttPrint();

#endif //MQTT_H
