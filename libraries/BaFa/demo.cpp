////
//// Created by mao on 2024/11/9.
////
//#include <WiFi.h>               //ESP32下的WIFI库
//#include "PubSubClient.h"       //MQTT库
//
///*WIFI配置*/
//#define WIFISSID "****"          //WIFI名称
//#define WIFIPSW "********"    //WIFI密码
//
///*MQTT服务器设置*/
//#define MQTT_SERVER "bemfa.com" //MQTT服务器
//#define MQTT_SERVER_PORT 9501   //MQTT服务器端口
//#define MQTT_ID "bc5b7*************c510335"  //巴法云密钥
//
//WiFiClient MQTTclient; //WIFI客户端对象
//PubSubClient client(MQTTclient); //MQTT客户端对象
//
//long lastMsg = 0;//上次消息发布时间
//char msg[50];//消息
//
///*订阅的主题有消息发布时的回调函数*/
//void callback(char* topic, byte* payload, unsigned int length) {
//    Serial.print("Message arrived [");
//    Serial.print(topic);   // 打印主题信息
//    Serial.print("] ");
//    for (int i = 0; i < length; i++) {
//        Serial.print((char)payload[i]); // 打印主题内容
//    }
//    Serial.println();
//}
//
///*重连MQTT函数*/
//void MQTT_reconnect()
//{
//
//    /*如果没连接成功一直循环*/
//    while (!client.connected())
//    {
//        Serial.print("Attempting MQTT connection...");
//        // 尝试去连接
//        if (client.connect(MQTT_ID)){
//            Serial.println("connected");//连接成功
//            client.subscribe("TOPIC");//订阅主题
//        }
//        else{
//            Serial.print("failed, rc=");//连接失败，输出状态，五秒后重试
//            Serial.print(client.state());
//            Serial.println(" try again in 5 seconds");
//            delay(5000);
//        }
//    }
//}
//
///*WIFI重连（等待）函数，WIFI.begin后需要一段时间才能连接成功*/
//void WIFI_reconnect(){
//    while (WiFi.status() != WL_CONNECTED) {
//        Serial.println("Connecting to WiFi..");
//        delay(500);
//    }
//
//    /*连接成功输出IP地址*/
//    Serial.print("\r\nGet IP Address: ");
//    Serial.println(WiFi.localIP());
//}
//
//
//void setup()
//{
//    Serial.begin(115200);
//    client.setServer(MQTT_SERVER, MQTT_SERVER_PORT);//连接MQTT服务器
//    client.setCallback(callback);//绑定回调函数
//    WiFi.begin(WIFISSID, WIFIPSW);//连接WIFI
//
//}
//
//void loop()
//{
//    /*检查WIFI连接*/
//    if (WiFi.status() != WL_CONNECTED)WIFI_reconnect();
//        /*若WIFI已经连接，则检查MQTT连接*/
//    else if (!client.connected())MQTT_reconnect();
//
//
//
//    client.loop();
//    long now = millis();
//
//    /*消息间隔*/
//    if (now - lastMsg > 2000)
//    {
//        lastMsg = now;
//        Serial.println(now);
//        dtostrf(now, 2, 2, msg);
//        client.publish("TOPIC", msg); //向指定主题发布消息
//
//    }
//}
