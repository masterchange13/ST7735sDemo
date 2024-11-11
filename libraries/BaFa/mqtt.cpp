//
// Created by mao on 2024/11/9.
//

#include "../PubSubClient/src/PubSubClient.h"
#include <WiFi.h>

const char* mqtt_server = "bemfa.com";                //默认，MQTT服务器地址
uint16_t mqtt_server_port = 9501;                    //默认，MQTT服务器端口
#define ID_MQTT  "2a2c6fe8997b489da138b231a8f887e7";  //mqtt客户端ID，修改为你的开发者密钥
const char*  topic = "weatherClockTime";              //Led主题名字，可在巴法云控制台自行创建，名称随意
const char*  updateTopic = "weatherClockTime";              //Led主题名字，可在巴法云控制台自行创建，名称随意

WiFiClient MQTTclient; //WIFI客户端对象
PubSubClient client(MQTTclient);

// MQTT 连接回调函数
void callback(char* topic, byte* payload, unsigned int length)
{
    Serial.print("Message received [");
    Serial.print(topic);
    Serial.print("]: ");
    for (unsigned int i = 0; i < length; i++) {
        Serial.print((char)payload[i]);
    }
    Serial.println();
};

/*重连MQTT函数*/
void MQTT_reconnect()
{

    /*如果没连接成功一直循环*/
    while (!client.connected())
    {
        Serial.print("Attempting MQTT connection...");
        // 尝试去连接
        if (client.connect(ID_MQTT)){
            Serial.println("connected");//连接成功
            client.subscribe("weatherClockTime~");//订阅主题
        }
        else{
            Serial.print("failed, rc=");//连接失败，输出状态，五秒后重试
            Serial.print(client.state());
            Serial.println(" try again in 5 seconds");
            delay(5000);
        }
    }
};

void mqttSetup()
{
    client.setServer(mqtt_server, mqtt_server_port);
    client.setCallback(callback);
};

void mqttPrint()
{
    Serial.println("MQTT connected");
}

