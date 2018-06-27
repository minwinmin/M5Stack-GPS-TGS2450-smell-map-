#include <M5Stack.h>
#include <TinyGPS++.h>
#include <math.h>
#include <Ambient.h>

WiFiClient client;
Ambient ambient;

int val = 0;

//デザリングパスワード/////////////////
const char* ssid = "MINMIN";
const char* password = "takudooon1444";
///////////////////////////////////////

//自宅/////////////////
//const char* ssid = "HG8045-7E6B-bg";
//const char* password = "nm3wddce";
///////////////////////////////////////

unsigned int channelId = 2157; // AmbientのチャネルID
const char* writeKey = "2a1161252f78c000"; // ライトキー


HardwareSerial GPS_s(2);
TinyGPSPlus gps;

void setup(){
  //WiFi(while文があるとwifiがつながるまで延々とした処理につながらないｓ)////////////////////////////////////////////////////////
  WiFi.begin(ssid, password);  //  Wi-Fi APに接続
  while (WiFi.status() != WL_CONNECTED) {  //  Wi-Fi AP接続待ち
      delay(100);
  }
  //////////////////////////////////////////////////////////////

  //臭気センサ//////////////////////////////////////////////////
  pinMode(21,OUTPUT);
  pinMode(22,OUTPUT);
  //////////////////////////////////////////////////////////////
  Serial.begin(115200);
  M5.begin();
  GPS_s.begin(9600);

  M5.Lcd.printf("Hello GPS!!");

  ambient.begin(channelId, writeKey, &client); 
}

void loop(){
  char buf[16];
  
  M5.Lcd.setCursor(0, 70);
  M5.Lcd.setTextColor(WHITE, BLACK);

  while(!gps.location.isUpdated()){
    while(GPS_s.available()>0){
      if(gps.encode(GPS_s.read())){
         break;
      }
     }
   }
  
   M5.Lcd.printf("GPS.OK");
   
   Serial.printf("lat: %f, lng: %f\r\n", gps.location.lat(), gps.location.lng());
   M5.Lcd.printf("lat: %f, lng: %f\r\n", gps.location.lat(), gps.location.lng());

  //GPS測定&Ambientへ送信
  if(M5.BtnA.wasPressed()){
    for (int i = 0; i <= 3; i++){
        delay(242);
        digitalWrite(22,HIGH);
        delay(8);
        digitalWrite(22,LOW); 
    }     
    delay(237);
    digitalWrite(21,HIGH);
    delay(3);
    val = analogRead(36);
    delay(2);
    digitalWrite(21,LOW);
    digitalWrite(22,HIGH);
    delay(8);
    digitalWrite(22,LOW); 
    val = 1023 - val;
    val = val * 5;
    Serial.println(val);
    // 3msec loop
    delay(3000);

    //臭気をAmbientに送信///////
    //計測データの平均値を送信//
    ambient.set(1,val);
    ///////////////////////////
    
    dtostrf(gps.location.lat(),12,8,buf);
    ambient.set(9, buf);
    dtostrf(gps.location.lng(), 12, 8, buf);
    ambient.set(10, buf);
    ambient.send();
  }

  //臭気測定&Ambientに送信
  if(M5.BtnB.wasPressed()){

  }

  if(M5.BtnC.wasPressed()){
    for (int i = 0; i <= 3; i++){
        delay(242);
        digitalWrite(22,HIGH);
        delay(8);
        digitalWrite(22,LOW); 
    }     
    delay(237);
    digitalWrite(21,HIGH);
    delay(3);
    val = analogRead(36);
    delay(2);
    digitalWrite(21,LOW);
    digitalWrite(22,HIGH);
    delay(8);
    digitalWrite(22,LOW); 
    val = 1023 - val;
    val = val * 5;
    M5.Lcd.printf("%f", val);
    // 3msec loop
    delay(3000);

  }

    m5.update();
   
}

