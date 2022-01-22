#include <ESP8266WiFi.h>
#include <ESP8266WebServer.h>
#include "EEPROM.h"
#define  BLYNK_PRINT Serial
#include <BlynkSimpleEsp8266.h>
#include <SoftwareSerial.h>
SoftwareSerial mySerial(13, 15);
ESP8266WebServer server(80);

long last = 0;
int TB1 = 0, TB2 = 0, TB3 = 0, TB4 = 0, TB5 = 0, TB6 = 0 ;
String Data = "";
bool stringComplete = false ;
//char auth[] = "LNpFb1KBhj5WCE79MCLtDv9XyF7gJLsX";
WidgetLED led1(V0);
BlynkTimer timer;

const char*     ssid           = "Tu Quan";
const char*     passphrase     = "12341234";
String          blynk          = "";
String          st;
String          content;
int             statusCode;

void setup()
{
  Serial.begin(9600);
  mySerial.begin(9600);
  EEPROM.begin(512);
  timer.setInterval(1000L, blinkLedWidget);
  setupWifi ();
  last = millis();
}
void loop()
{
  server.handleClient();
  Blynk.run();
  timer.run();
  if (millis () - last >= 300)
  {
    Read_UART();
    checkButton();
    last = millis();
  }
}
void setupWifi ()
{
  Serial.println("Startup");
  Serial.println("Reading EEPROM ssid");
  String esid;
  for (int i = 0; i < 32; ++i)
  {
    esid += char(EEPROM.read(i));
  }
  Serial.print("SSID: ");
  Serial.println(esid.c_str());
  esid.trim();

  Serial.println("Reading EEPROM pass");
  String epass = "";
  for (int i = 32; i < 96; ++i)
  {
    epass += char(EEPROM.read(i));
  }
  Serial.print("PASS: ");
  Serial.println(epass.c_str());
  epass.trim();

  Serial.println("Reading EEPROM blynk");
  String eblynk = "";
  for (int i = 96; i < 128; ++i)
  {
    eblynk += char(EEPROM.read(i));
  }
  Serial.print("BLYNK: ");
  Serial.println(eblynk.c_str());
  eblynk.trim();

  if ( esid.length() > 1 )
  {
    WiFi.begin(esid.c_str(), epass.c_str());
    if (testWifi())
    {
      launchWeb(0);
      WiFi.disconnect();

      char * auth_ = new char[eblynk.length() + 1];
      eblynk.toCharArray(auth_, eblynk.length() + 1);
      Blynk.begin(auth_, esid.c_str(), epass.c_str());
      EEPROM.end();
      return;
    }
  }
  setupAP();
  EEPROM.end();
  last = millis();
}
void Read_UART()
{
  while (mySerial.available ())
  {
    char inChar = (char)mySerial.read();
    Data += inChar;
    if (inChar == '\n')
    {
      stringComplete = true ;
    }
    if (stringComplete)
    {
      Serial.print("data nhan");
      Serial.println(Data);
      xuly();
      tachchuoi();
      Data = "";
      stringComplete = false ;
    }
  }
  last = millis();
}

void xuly()
{
  if (Data.indexOf ("C0D") >= 0)
  {
    TB1 = 0;
  }
  else if (Data.indexOf ("C1D") >= 0)
  {
    TB1 = 1;
  }//======================
  if (Data.indexOf ("D0E") >= 0)
  {
    TB2 = 0;
  }
  else if (Data.indexOf ("D1E") >= 0)
  {
    TB2 = 1;
  }//======================
  if (Data.indexOf ("E0F") >= 0)
  {
    TB3 = 0;
  }
  else if (Data.indexOf ("E1F") >= 0)
  {
    TB3 = 1;
  }//======================
  if (Data.indexOf ("F0G") >= 0)
  {
    TB4 = 0;
  }
  else if (Data.indexOf ("F1G") >= 0)
  {
    TB4 = 1;
  }//======================
  if (Data.indexOf ("G0H") >= 0)
  {
    TB5 = 0;
  }
  else if (Data.indexOf ("G1H") >= 0)
  {
    TB5 = 1;
  }//======================
  if (Data.indexOf ("H0J") >= 0)
  {
    TB6 = 0;
  }
  else if (Data.indexOf ("H1J") >= 0)
  {
    TB6 = 1;
  }//======================
  uppp();
  last = millis();
}
void uppp()
{
  Blynk.virtualWrite (V1, TB1);
  Blynk.virtualWrite (V2, TB2);
  Blynk.virtualWrite (V3, TB3);
  Blynk.virtualWrite (V4, TB4);
  Blynk.virtualWrite (V5, TB5);
  Blynk.virtualWrite (V6, TB6);
}
void tachchuoi()
{
  String temp = "" ;
  String gas = "" ;
  int x = 0, y = 0 ;
  for (int i = 0; i <= Data.length(); i++)
  {
    if (Data[i] == 'A')
    {
      x = 1 ;
    }
    else if (Data[i] == 'B')
    {
      y = 1; x = 0 ;
    }
    else if (Data[i] == 'C' || Data[i] == 'D' || Data[i] == 'E' || Data[i] == 'F' || Data[i] == 'G' || Data[i] == 'H' || Data[i] == 'J')
    {
      y = 0; x = 0 ;
    }
    else
    {
      if ( x == 1)
      {
        temp += Data[i];
      }
      if (y == 1)
      {
        gas += Data[i];
      }
    }
  }
  Blynk.virtualWrite (V7, temp);
  Blynk.virtualWrite (V8, gas);
  last = millis();
}



void blinkLedWidget()
{
  if (led1.getValue()) {
    led1.off();
  } else {
    led1.on();
  }
}
BLYNK_WRITE(V1) {
  TB1 = param.asInt();
  Blynk.virtualWrite (V1, TB1);
  mySerial.print("TB1=");
  mySerial.println(TB1);
}
BLYNK_WRITE(V2) {
  TB2 = param.asInt();
  Blynk.virtualWrite (V2, TB2);
  mySerial.print("TB2=");
  mySerial.println(TB2);
}
BLYNK_WRITE(V3) {
  TB3 = param.asInt();
  Blynk.virtualWrite (V3, TB3);
  mySerial.print("TB3=");
  mySerial.println(TB3);
}
BLYNK_WRITE(V4) {
  TB4 = param.asInt();
  Blynk.virtualWrite (V4, TB4);
  mySerial.print("TB4=");
  mySerial.println(TB4);
}
BLYNK_WRITE(V5) {
  TB5 = param.asInt();
  Blynk.virtualWrite (V5, TB5);
  mySerial.print("TB5=");
  mySerial.println(TB5);
}
BLYNK_WRITE(V6) {
  TB6 = param.asInt();
  Blynk.virtualWrite (V6, TB6);
  mySerial.print("TB6=");
  mySerial.println(TB6);
}
bool testWifi(void)
{
  int c = 0;
  Serial.println("Xin vui long doi ket noi WIFI");
  while ( c < 20 )
  {
    if (WiFi.status() == WL_CONNECTED)
    {
      return true;
    }
    delay(1000);
    Serial.print(WiFi.status());
    c++;
  }
  Serial.println("");
  Serial.println("Thoi gian ket noi cham, Mo AP");
  return false;
}

void launchWeb(int webtype)
{
  Serial.println("");
  Serial.println("WiFi ket noi");
  Serial.print("Dia chi IP: ");
  Serial.println(WiFi.localIP());
  Serial.print("SoftAP IP: ");
  Serial.println(WiFi.softAPIP());
  createWebServer(webtype);
  // Start the server
  server.begin();
  Serial.println("May chu bat dau");
}

void setupAP(void)
{
  WiFi.mode(WIFI_STA);
  WiFi.disconnect();
  delay(100);
  int n = WiFi.scanNetworks();
  Serial.println("Tim hoan tat");
  if (n == 0)
  {
    Serial.println("no networks found");
  }
  else
  {
    Serial.print(n);
    Serial.println(" networks found");
    for (int i = 0; i < n; ++i)
    {
      // Print SSID and RSSI for each network found
      Serial.print(i + 1);
      Serial.print(": ");
      Serial.print(WiFi.SSID(i));
      Serial.print(" (");
      Serial.print(WiFi.RSSI(i));
      Serial.print(")");
      Serial.println((WiFi.encryptionType(i) == ENC_TYPE_NONE) ? " " : "*");
      delay(10);
    }
  }
  Serial.println("");
  st = "<ol>";
  for (int i = 0; i < n; ++i)
  {
    // Print SSID and RSSI for each network found
    st += "<li>";
    st += WiFi.SSID(i);
    st += " (";
    st += WiFi.RSSI(i);
    st += ")";
    st += (WiFi.encryptionType(i) == ENC_TYPE_NONE) ? " " : "*";
    st += "</li>";
  }
  st += "</ol>";
  delay(100);
  Serial.println("softap");
  Serial.println(ssid);
  Serial.println(passphrase);
  WiFi.softAP(ssid, passphrase, 6);

  launchWeb(1);
  Serial.println("over");
}

void createWebServer(int webtype)
{
  if ( webtype == 1 )
  {
    server.on("/", []()
    {
      IPAddress ip = WiFi.softAPIP();
      String ipStr = String(ip[0]) + '.' + String(ip[1]) + '.' + String(ip[2]) + '.' + String(ip[3]);
      content = "<!DOCTYPE HTML>\r\n<html><h2>TuQuan</h2>";
      //content += ipStr;
      //content += "<form method='get' action='setting'><table width='100%' border='1'><tr><td width=\"30%\"><label>Wifi</label></td><td width=\"70%\><input name='ssid' length=32 width='500'></td></tr><tr><td><label>Password</label></td><td><input name='pass' length=64 width='500'></td></tr><tr><td><label>Blynk</label></td><td><input name='blynk' length=32 width='500'></td></tr><tr><td></td><td><input type='submit'></tr></tr></table></form>";
      content += "<form method=\"get\" action=\"setting\">";
      content += "<div>Wifi</div>";
      content += "<div><input name=\"ssid\" size=\"40\"></div>";
      content += "<div>Mat Khau</div>";
      content += "<div><input name=\"pass\" size=\"40\"></div>";
      content += "<div>Blynk</div>";
      content += "<div><input name=\"blynk\" size=\"40\"></div>";
      content += "<div><input type='submit'></div>";

      content += "<p>";
      content += st;
      content += "</p>";
      content += "</html>";
      server.send(200, "text/html", content);
    });
    server.on("/setting", []()
    {
      String qsid = server.arg("ssid");
      String qpass = server.arg("pass");
      String qblynk = server.arg("blynk");
      if (qsid.length() > 0 && qpass.length() > 0)
      {
        EEPROM.begin(512);
        Serial.println("clearing eeprom");
        for (int i = 0; i < 128; ++i)
        {
          EEPROM.write(i, 0);
        }
        EEPROM.commit();
        Serial.println(qsid);
        Serial.println("");
        Serial.println(qpass);
        Serial.println("");
        Serial.println(qblynk);
        Serial.println("");

        Serial.println("writing eeprom ssid:");
        for (int i = 0; i < qsid.length(); ++i)
        {
          EEPROM.write(i, qsid[i]);
          Serial.print("Wrote: ");
          Serial.print(qsid[i]);
          Serial.println("");
        }

        Serial.println("writing eeprom pass:");
        for (int i = 0; i < qpass.length(); ++i)
        {
          EEPROM.write(32 + i, qpass[i]);
          Serial.print("Wrote: ");
          Serial.print(qpass[i]);
          Serial.println("");
        }

        Serial.println("writing eeprom blynk:");
        for (int i = 0; i < qblynk.length(); ++i)
        {
          EEPROM.write(96 + i, qblynk[i]);
          Serial.print("Wrote: ");
          Serial.print(qblynk[i]);
          Serial.println("");
        }
        EEPROM.commit();
        EEPROM.end();
        content = "{\"Success\":\"Luu vao he thong. Khoi dong lai ten wifi moi\"}";
        statusCode = 200;
      }
      else
      {
        content = "{\"Error\":\"404 not found\"}";
        statusCode = 404;
        Serial.println("Sending 404");
      }
      server.send(statusCode, "application/json", content);
    });
  }
  else if (webtype == 0)
  {
    server.on("/", []()
    {
      IPAddress ip = WiFi.localIP();
      String ipStr = String(ip[0]) + '.' + String(ip[1]) + '.' + String(ip[2]) + '.' + String(ip[3]);
      server.send(200, "application/json", "{\"IP\":\"" + ipStr + "\"}");
    });
    server.on("/cleareeprom", []()
    {
      content = "<!DOCTYPE HTML>\r\n<html>";
      content += "<h2>TuQuan</h2><p>Clearing the EEPROM</p></html>";
      server.send(200, "text/html", content);
      Serial.println("clearing eeprom");
      for (int i = 0; i < 128; ++i)
      {
        EEPROM.write(i, 0);
      }
      EEPROM.commit();
    });
  }
  last = millis();
}
void checkButton()
{
  int button = digitalRead(0);
  if (button == 0)
  {
    Serial.println("reset esp and set up wifi");
    for (int i = 0; i < 128; ++i)
    {
      EEPROM.write(i, 0);
    }
    EEPROM.commit();
    ESP.restart();

  }

}
