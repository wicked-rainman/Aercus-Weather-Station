//-----------------------------------------------------
// Multiple UDP source weather display.
// This code is similar to the M5StackUDPClient code.
// Differences are, the UDP data format has been modified
// to include:
// 1. A Station ID
// 2. A message format version number
// 3. Date time stamping
//
// Pressing button A or B changes which station data
// is displayed.
//-----------------------------------------------------
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
char ssid[4] = {'x', 'x', 'x', '\0'};                 //SSID. Change length and value to suit.
char pass[9] = {'x', 'x', 'x', 'x', 'x', 'x', 'x', '0', '\0'}; //Password. Change length and value to suit.
char packetBuffer[255];
int clientCount = 0;
int sel1 = 1;
int sel2 = 0;
void setup() {
  M5.begin(true, true, true);
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
  char SoftwareVer[10];
  char StationID[10];
  char dtgStr[30];
  char outTemp[10];
  char outHumi[10];
  char RelPress[10];
  char avgwind[10];
  char windDir[10];
  char dailyrainin[10];
  char solarradiation[10];
  char tempCentigrade[10];
  int percentFill;
  float pres;
  float temp;
  int row, go;

  int countPos, pos;
  char mmRain[10];
  int packetSize = 0;
  int payloadSize = 0;
  char buffer[200];
  M5.update();
  if (M5.BtnA.isPressed()) {
    sel1 = 1;
    sel2 = 0;
    M5.Lcd.setTextColor(WHITE);
    M5.Lcd.setTextSize(3);
    M5.Lcd.setCursor(260, 45);
    M5.Lcd.print("***");
  }
  if (M5.BtnB.isPressed()) {
    sel2 = 1;
    sel1 = 0;
    M5.Lcd.setTextColor(WHITE);
    M5.Lcd.setTextSize(3);
    M5.Lcd.setCursor(260, 45);
    M5.Lcd.print("***");
  }
  packetSize = Udp.parsePacket();
  if (packetSize) {
    payloadSize = Udp.read(weatherChars, 255);
    weatherChars[payloadSize] = 0;
    if (payloadSize > 0) {

      //Extract SoftwareVer
      pos = strpos(",", weatherChars);
      strncpy(SoftwareVer, weatherChars, pos);
      SoftwareVer[pos] = 0;
      strcpy(weatherChars, &weatherChars[pos + 1]);

      //Extract StationID
      pos = strpos(",", weatherChars);
      strncpy(StationID, weatherChars, pos);
      StationID[pos] = 0;
      strcpy(weatherChars, &weatherChars[pos + 1]);

      //Extract dtgStr
      pos = strpos(",", weatherChars);
      strncpy(dtgStr, weatherChars, pos);
      dtgStr[pos] = 0;
      strcpy(weatherChars, &weatherChars[pos + 1]);

      //Extract temp
      pos = strpos(",", weatherChars);
      strncpy(outTemp, weatherChars, pos);
      outTemp[pos] = 0;
      strcpy(weatherChars, &weatherChars[pos + 1]);

      //Extract Humidity
      pos = strpos(",", weatherChars);
      strncpy(outHumi, weatherChars, pos);
      outHumi[pos] = 0;
      strcpy(weatherChars, &weatherChars[pos + 1]);

      //Extract Pressure
      pos = strpos(",", weatherChars);
      strncpy(RelPress, weatherChars, pos);
      RelPress[pos] = 0;
      strcpy(weatherChars, &weatherChars[pos + 1]);

      //Extract wind speed
      pos = strpos(",", weatherChars);
      strncpy(avgwind, weatherChars, pos);
      avgwind[pos] = 0;
      strcpy(weatherChars, &weatherChars[pos + 1]);

      //Extract windDir
      pos = strpos(",", weatherChars);
      strncpy(windDir, weatherChars, pos);
      windDir[pos] = 0;
      strcpy(weatherChars, &weatherChars[pos + 1]);

      //Extract rain
      //pos=strpos(',',weatherChars);
      strcpy(dailyrainin, weatherChars);


      //----------------------Finished extracting values

      go = 0;
      if ((strcmp(StationID, "001") == 0) && (sel1 == 1)) go = 1;
      if ((strcmp(StationID, "002") == 0) && (sel2 == 1)) go = 1;
      if (go == 1) {
        M5.Lcd.clear();
        if (clientCount >= 9) clientCount = 1;
        clientCount++;

        //Display humidity as a bargraph
        percentFill = atoi(outHumi) * 3;
        M5.Lcd.fillRect(0, 0, percentFill, 5, RED);
        M5.Lcd.fillRect(percentFill, 0, 320 - percentFill, 5, WHITE);

        //Display stationID
        M5.Lcd.setTextColor(WHITE);
        M5.Lcd.setTextSize(3);
        M5.Lcd.setCursor(260, 45);
        M5.Lcd.print(StationID);

        //Display temperature
        temp = strtof(outTemp, NULL);
        //sprintf(tempCentigrade, "%.1fC",outTemp);
        if (temp > 24) M5.Lcd.setTextColor(RED);
        else if (temp > 20) M5.Lcd.setTextColor(ORANGE);
        else if (temp > 15) M5.Lcd.setTextColor(YELLOW);
        else if (temp > 10) M5.Lcd.setTextColor(GREEN);
        else if (temp > 5) M5.Lcd.setTextColor(WHITE);
        else M5.Lcd.setTextColor(BLUE);
        M5.Lcd.setTextSize(7);
        M5.Lcd.setCursor(10, 30);
        M5.Lcd.print(outTemp);

        //Display pressure
        pres = strtof(RelPress, NULL);
        if (pres > 1000) M5.Lcd.setTextColor(GREEN);
        else M5.Lcd.setTextColor(RED);
        M5.Lcd.setCursor(10, 110);
        M5.Lcd.print(RelPress);

        //Display average wind and rain totals
        M5.Lcd.setTextSize(3);
        M5.Lcd.setTextColor(WHITE);
        M5.Lcd.setCursor(10, 200);
        strcat(avgwind, "mph");
        M5.Lcd.print(avgwind);
        M5.Lcd.setCursor(180, 200);
        dtostrf((strtof(dailyrainin, NULL) * 25.4), 3, 1, mmRain);
        strcat(mmRain, "mm");
        M5.Lcd.print(mmRain);

        //Display Heartbeat of records coming in
        countPos = clientCount * 3;
        M5.Lcd.fillRect(countPos * 10, 233, 10, 8, YELLOW);
      }
      else {}
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
