/*
    Description:
    Use UNIT CAT-M to connect to the MQTT server, and implement subscription and publishing messages.
    Check the status through Serial and Screen. When the MQTT connection is successful, Click BtnA Public Topic
    Please install library before compiling:
    M5Stack: https://github.com/m5stack/M5Stack
    Use HEROKU MQTT Server
*/
#include "M5StickCPlus.h"
#include "M5_SIM7080G.h"

M5_SIM7080G device;

String readstr;


void log(String str) {
  Serial.print(str);
}

void setup()
{
  M5.begin();

  M5.Lcd.setTextColor(WHITE);
  M5.Lcd.println(F("M5StickCPlus+CAT-M MQTT HEROKU"));
  Serial2.begin(115200, SERIAL_8N1, 33, 32);// Grove
  //SIM7080
  device.Init(&Serial2, 33, 32);
  //Reboot
  device.sendMsg("AT+CREBOOT\r\n");
  delay(1000);
  //Signal Quality Report
  device.sendMsg("AT+CSQ\r\n");
  delay(1000);
  device.sendMsg("AT+COPS=1,2,44010 \r\n");
  readstr = device.waitMsg(5000);
  log(readstr);
  delay(1000);

  device.sendMsg("AT+CGNSPWR=1 \r\n");
  readstr = device.waitMsg(5000);
  log(readstr);
  delay(1000);

  device.sendMsg("AT+CMNB=1\r\n");
  readstr = device.waitMsg(5000);
  log(readstr);
  delay(1000);
  
  device.sendMsg("AT+CGNAPN\r\n");
  readstr = device.waitMsg(5000);
  log(readstr);
  delay(1000);

  device.sendMsg("AT+CGNSINF\r\n");
  readstr = device.waitMsg(5000);
  log(readstr);
  delay(1000);

}

void loop()
{

  while (1) {

    device.sendMsg("AT+SECMAUTH?\r\n");
    readstr = device.waitMsg(200);
    log(readstr);
    
    // APP network Active
    device.sendMsg("AT+CNACT=0,1\r\n");
    readstr = device.waitMsg(200);
    log(readstr);
    device.sendMsg("AT+CNACT?\r\n");
    readstr = device.waitMsg(200);
    log(readstr);

    //Set MQTT Parameter
    device.sendMsg("AT+SMCONF=\"URL\",\"driver.cloudmqtt.com\",\"18829\"\r\n");
    readstr = device.waitMsg(1000);
    log(readstr);
    //Please change username & password 
    device.sendMsg("AT+SMCONF=\"USERNAME\",\"username\"\r\n");
    readstr = device.waitMsg(1000);
    log(readstr);
    device.sendMsg("AT+SMCONF=\"PASSWORD\",\"password\"\r\n");
    readstr = device.waitMsg(1000);
    log(readstr);
    device.sendMsg("AT+SMCONF=\"KEEPTIME\",60\r\n");
    readstr = device.waitMsg(1000);
    log(readstr);

    device.sendMsg("AT+SMCONF=\"CLEANSS\",1\r\n");
    readstr = device.waitMsg(1000);
    log(readstr);

    device.sendMsg("AT+SMCONF=\"CLIENTID\",\"simmqtt\"\r\n");
    readstr = device.waitMsg(1000);
    log(readstr);

    //MQTT Connection
    device.sendMsg("AT+SMCONN\r\n");
    readstr = device.waitMsg(5000);
    log(readstr);

    if (readstr.indexOf("ERROR") == -1) {
      break;
    }
  }
  //Subscribe Packet
  device.sendMsg("AT+SMSUB=\"sub_topic\",1\r\n");
  readstr = device.waitMsg(1000);
  log(readstr);

  while (1) {
    M5.update();
    if (M5.BtnA.wasPressed()) {
      //Send Packet
      device.sendMsg("AT+SMPUB=\"pub_topic\",5,1,1\r\n");
      delay(100);
      device.sendMsg("hello\r\n");
    }
    readstr = device.waitMsg(0);
    Serial.print(readstr);
    log(readstr);
  }
}
