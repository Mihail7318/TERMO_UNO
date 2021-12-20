#include <EEPROM.h>
#include <ArduinoJson.h>
#include <ESP8266WiFi.h>
#include <ESP8266HTTPClient.h>
#include <GyverButton.h>
#include <Wire.h>  // This library is already built in to the Arduino IDE
#include <LiquidCrystal_I2C.h> //This library you can add via Include Library > Manage Library >
#include <OneWire.h>
#include <DallasTemperature.h>
#include <NTPClient.h>
#include <WiFiUdp.h>
#include "RTClib.h"
#define ONE_WIRE_BUS 13
#define RELE 16
#define COUNT_CONNECT 250





const long utcOffsetInSeconds = 14400;
char ssid[] = "Honor 8X";
char pass[] = "00000000";


byte fire[] = {
  B00001,
  B00010,
  B00000,
  B00100,
  B01110,
  B11111,
  B01110,
  B00100
};
byte porch[] = {
  B00000,
  B00000,
  B00000,
  B00001,
  B00011,
  B00111,
  B01111,
  B11111
};
byte clocc[] = {
  B00000,
  B01110,
  B10101,
  B10111,
  B10001,
  B10001,
  B01110,
  B00000
};

StaticJsonDocument<1000> jsonDocument;
//ESP8266WebServer server(80);
OneWire oneWire(ONE_WIRE_BUS);
DallasTemperature tmp(&oneWire);
LiquidCrystal_I2C lcd(0x27, 16, 2);
DeviceAddress ADDR;
GButton menu(10, HIGH_PULL, NORM_OPEN);
GButton left(1, HIGH_PULL, NORM_OPEN);
GButton right(3, HIGH_PULL, NORM_OPEN);
WiFiUDP ntpUDP;
NTPClient timeClient(ntpUDP, "pool.ntp.org", utcOffsetInSeconds);
RTC_DS3231 rtc;

float TEMP1 = 0;
float TEMP2 = 0;
float TEMP3 = 0;
float TEMP4 = 0;
unsigned long tmr;
unsigned long tmr1;
unsigned long mil;
char buffer[1000];
int deviceCount = 0;
int i = 0;
int j =0;
bool set_temp = false;

uint8_t sensor1[8] = { 0x28, 0xEE, 0xD5, 0x64, 0x1A, 0x16, 0x02, 0xEC };
uint8_t sensor2[8] = { 0x28, 0x61, 0x64, 0x12, 0x3C, 0x7C, 0x2F, 0x27 };
uint8_t sensor3[8] = { 0x28, 0xD3, 0xE2, 0x07, 0xD6, 0x01, 0x3C, 0x4D };
uint8_t sensor4[8] = { 0x28, 0xEF, 0x91, 0x07, 0xD6, 0x01, 0x3C, 0xFE };
bool stat = true;

int AM_H = 8;
int PM_H = 20;
int AM_M = 00;
int PM_M = 30;
int ust = 25;
bool flag = true;
bool WIFIerr = false;

float TEMP_AM = 26;
float TEMP_PM = 18;

int h = 0;
int m = 0;

#include "LCD.h"
#include "debug.h"
#include "computed.h"
#include "updateTime.h"

void setup()
{
  pinMode(0, OUTPUT);
  pinMode(2, OUTPUT);
  digitalWrite(0,HIGH);
  digitalWrite(2,HIGH);
  Wire.begin(4,5);
  Serial.begin(115200);
  WiFi.begin(ssid, pass);;
  delay(100);
  rtc.begin();
  lcd.init();            
  lcd.backlight();
  lcd.setCursor(0,0);
  lcd.print("WIFI connected..");
  while (WiFi.status() != WL_CONNECTED) 
  {
    if(i>16){ i=0; 
    lcd.setCursor(0,1);
    for(int i=0; i<16; i++){
      lcd.setCursor(i,1);
      lcd.print(" ");
      }
    };
    lcd.setCursor(i,1);
    delay(80);
    Serial.print(".");
    lcd.print(char(255));
    i++;
    j++;
    if(j>COUNT_CONNECT){WIFIerr = true; break;}
  }
  lcd.clear();
  lcd.setCursor(0, 0);
  Serial.println("");
  if(WIFIerr){
  Serial.println("NO connect..!");
  lcd.print("NO connect!");
  delay(1500);
    }else{
   //time_update();  
  Serial.println("WiFi connected..!");
  lcd.print("WiFi connect!");
  delay(1500);
  lcd.clear();
  lcd.setCursor(7, 0);
  lcd.print("IP");
  lcd.setCursor(1, 1);
  lcd.print(WiFi.localIP());
  Serial.print("Got IP: ");  Serial.println(WiFi.localIP());
  delay(1000);
      }
  tmp.begin();
  lcd.clear();
  lcd.setCursor(0,0);
  lcd.print("Locating devices");
  lcd.setCursor(0,1);
  for(int i=0;i < 16; i++){
    lcd.setCursor(i,1);
    delay(300);
    lcd.print(char(255));
    }
  lcd.clear();
  Serial.println("Locating devices...");
  lcd.setCursor(0,0);
  lcd.print("Found");
  Serial.print("Found ");
  deviceCount = tmp.getDeviceCount();
  Serial.print(deviceCount, DEC);
  lcd.setCursor(0,1);
  lcd.print(deviceCount, DEC);
  lcd.print(" devices");
  
  delay(1000);
  lcd.clear();
  lcd.setCursor(0,0);
  lcd.print("  Start system");
  delay(1000);
  lcd.clear();
  shedule_window(AM_H,AM_M,PM_H,PM_M,TEMP_AM,TEMP_PM);
  delay(1000);
  lcd.clear();
  Serial.println(" devices.");
  Serial.println("");
  Serial.println("Printing addresses...");
  for (int i = 0;  i < deviceCount;  i++)
  {
    Serial.print("Sensor ");
    Serial.print(i+1);
    Serial.print(" : ");
    tmp.getAddress(ADDR, i);
    printAddress(ADDR);
  }
EEPROM.begin(512);
  lcd.createChar(0, fire);
  lcd.createChar(1, porch);
  lcd.createChar(2, clocc);

}
  




void loop() {
  DateTime now = rtc.now();
  h = now.hour();
  m = now.minute();
  menu.tick();
  left.tick();
  right.tick();
  if (menu.isClick()) { 
    Serial.println("menu");
    }
  if (left.isClick()) {
    if (ust >10){ 
    ust = ust - 1;}
    set_temp_w(ust, set_temp);
    set_temp = true;
    mil = millis();
    }
  if (right.isClick()) {
    if(ust < 32){
    ust = ust + 1;}
    set_temp_w(ust, set_temp);
    set_temp = true;
    mil = millis();
  }
  
if (millis() - tmr >= 2000) {
    tmr = millis();
    if(flag){
      digitalWrite(2, LOW);
      }
      else{digitalWrite(2, HIGH);}
computed();
if(!set_temp){
    main_window(TEMP3, TEMP4);
    }
  }

if(set_temp){
  if (millis() - 5000 >= mil) {
    //mil = millis();
    set_temp = false;
    lcd.clear();
    main_window(TEMP3, TEMP4);
    }
}





if(!WIFIerr){

}
}


void handle_OnConnect() 
{
  Serial.print("TEMP3A ");
  Serial.print(TEMP3);
  Serial.println("");
  jsonDocument["data"]["Temperature"]["room1"]["name"] = "Кабинет 1";
  jsonDocument["data"]["Temperature"]["room1"]["value"] =  TEMP1;
  jsonDocument["data"]["Temperature"]["room2"]["name"] = "Кабинет 2";
  jsonDocument["data"]["Temperature"]["room2"]["value"] =  TEMP2;
  jsonDocument["data"]["Temperature"]["room3"]["name"] = "Кабинет 3";
  jsonDocument["data"]["Temperature"]["room3"]["value"] =  TEMP3;
  jsonDocument["data"]["Temperature"]["room4"]["name"] = "Кабинет 4";
  jsonDocument["data"]["Temperature"]["room4"]["value"] =  TEMP4;
  serializeJsonPretty(jsonDocument, buffer);
  //server.send(200, "text/json", buffer);
}
