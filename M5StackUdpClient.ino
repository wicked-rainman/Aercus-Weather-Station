#include <SPI.h>
#include <WiFiClient.h>
#include <WiFiScan.h>
#include <WiFiServer.h>
#include <WiFiUdp.h>
#include <WiFiMulti.h>
#include <WiFi.h>
#include <ETH.h>
#include <WiFiAP.h>
#include <WiFiType.h>
#include <WiFiGeneric.h>
#include <WiFiSTA.h>
#include <M5Stack.h>
int status = WL_IDLE_STATUS;
WiFiUDP Udp;
char ssid[4] = {'x','x','x','\0'};                    //SSID. Change length and value to suit.
char pass[9]= {'x','x','x','x','x','x','x','x','\0'}; //Password. Change length and value to suit.
char packetBuffer[255];
int clientCount = 0;
void setup() {
  M5.begin();
  M5.Power.begin();
  M5.Lcd.fillScreen(BLACK);
  M5.Lcd.setCursor(10, 10);
  M5.Lcd.setTextColor(WHITE);
  M5.Lcd.setTextSize(3);
  M5.Lcd.printf("Connecting ");
  WiFi.begin(ssid, pass);
  while (WiFi.status() != WL_CONNECTED) {
    M5.Lcd.printf(".");
    delay(1000);
  }
  M5.Lcd.clear();
  M5.Lcd.setCursor(10, 10);
  M5.Lcd.print(WiFi.localIP());
  Udp.begin(5678);

}

void loop() {
  char weatherChars[1024];
  char outTemp[10];
  char outHumi[10];
  char RelPress[10];
  char avgwind[10];
  char dailyrainin[10];
  int percentFill;
  float pres;
  float baro;
  float temp;
  float mmrain;
  int countPos,pos,nextpos;
  char mmRain[10];
  int packetSize=0;
  int payloadSize=0;
  char tempCentigrade[12];
  char pressure[12];
  packetSize=Udp.parsePacket();
  if (packetSize) {
      clientCount++;
      payloadSize=Udp.read(weatherChars, 255);
      weatherChars[payloadSize]=0;
      if(payloadSize > 0) {
      if(clientCount>=9) clientCount=1;
      //Extract temp
      pos=strpos(",",weatherChars);
      strncpy(outTemp, weatherChars,pos);
      outTemp[pos]=0;
      strcpy(weatherChars, &weatherChars[pos+1]);
       
      //Extract Humidity
      pos=strpos(",",weatherChars);
      strncpy(outHumi, weatherChars,pos);
      outHumi[pos]=0;
      strcpy(weatherChars, &weatherChars[pos+1]);

      //Extract temp
      pos=strpos(",",weatherChars);
      strncpy(RelPress, weatherChars,pos);
      RelPress[pos]=0;
      strcpy(weatherChars, &weatherChars[pos+1]);

      //Extract wind speed
      pos=strpos(",",weatherChars);
      strncpy(avgwind, weatherChars,pos);
      avgwind[pos]=0;
      strcpy(weatherChars, &weatherChars[pos+1]);

      //Extract rain
      //pos=strpos(',',weatherChars);
      strcpy(dailyrainin, weatherChars);




      M5.Lcd.clear();
      
      //Display humidity as a bargraph
      percentFill=atoi(outHumi)*3;
      M5.Lcd.fillRect(0,0,percentFill,5,RED);
      M5.Lcd.fillRect(percentFill,0,320-percentFill,5,WHITE); 

      
    //Display temperature
      temp=strtof(outTemp,NULL);
      //sprintf(tempCentigrade, "%.1fC",outTemp);
      if(temp>24) M5.Lcd.setTextColor(RED);
      else if(temp>20) M5.Lcd.setTextColor(ORANGE);
      else if(temp>15) M5.Lcd.setTextColor(YELLOW);
      else if(temp>10) M5.Lcd.setTextColor(GREEN);
      else if(temp>5) M5.Lcd.setTextColor(WHITE);
      else M5.Lcd.setTextColor(BLUE);
      M5.Lcd.setTextSize(7);
      M5.Lcd.setCursor(10, 30);
      M5.Lcd.print(outTemp);

    //Display pressure
      pres=strtof(RelPress,NULL);
      if(pres > 1000) M5.Lcd.setTextColor(GREEN);
      else M5.Lcd.setTextColor(RED);
      M5.Lcd.setCursor(10, 110);
      M5.Lcd.print(RelPress);

      //Display average wind and rain totals
      M5.Lcd.setTextSize(3);
      M5.Lcd.setTextColor(WHITE);
      M5.Lcd.setCursor(10,200);
      strcat(avgwind,"mph");
      M5.Lcd.print(avgwind);
      M5.Lcd.setCursor(180,200);
      dtostrf((strtof(dailyrainin,NULL)*25.4),3,1,mmRain);
      strcat(mmRain,"mm");
      M5.Lcd.print(mmRain);
;

      //Display Heartbeat of records coming in
      countPos =clientCount*3;
      M5.Lcd.fillRect(countPos*10,233,10,8,YELLOW);

      
    }
   
  }
}
int strpos(char *needle, char *haystack) {
  char *offset;
  offset = strstr(haystack, needle);
  if (offset != NULL) {
    return offset - haystack;
  }
  return -1;
}
