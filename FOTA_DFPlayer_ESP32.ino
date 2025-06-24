#include <WiFi.h>
#include <HTTPClient.h>
#include <DFRobotDFPlayerMini.h>

#define ACK_TIMEOUT_MS 3000

const char* ssid = "Tarek ";
const char* password = "shtzr@shtzr1";
const char* serverURL = "http://fota-portal.atwebpages.com/";
const char* versionPath = "version.txt";
const char* hexPath = "firmware/firmware.hex";
int WIFI_State;

const int setupSignal = 255;
const int SendSameLine = 0xBB;
const int Reply_ACK = 0xAA;
const int Cancle = 0xCC;

const unsigned long timeoutMillis = 5000;
int ACK;
String lastVersion = "1.0.0";
String newVersion; 
DFRobotDFPlayerMini myDFPlayer;

// Serial1 = DFPlayer (TX=GPIO10, RX=GPIO9)
// Serial2 = STM32 (TX=GPIO17, RX=GPIO16)

void setup() {
  Serial.begin(9600);
  Serial1.begin(9600, SERIAL_8N1, 18, 5);  // DFplayer
  Serial2.begin(9600, SERIAL_8N1, 16, 17);  // stm32

  WiFi.begin(ssid, password);
  unsigned long startAttempt = millis();
  while (WiFi.status() != WL_CONNECTED && millis() - startAttempt < timeoutMillis) {
    delay(500);
  }
    if (WiFi.status() == WL_CONNECTED) {
    Serial.println("WiFi connected!");
    Serial.print("IP address: ");
    Serial.println(WiFi.localIP());
  } else {
    Serial.println("WiFi connection failed.");
  }
  
  // Init DFPlayer
  if (!myDFPlayer.begin(Serial1)) {
    while (1);
  }

  myDFPlayer.volume(20); // Set initial volume   
}

void loop() {
  // Check if STM32 sent a command
  if (Serial.available()) {
    int DATA = Serial.parseInt();
    if(DATA == 1)
    {
      if (WiFi.status() != WL_CONNECTED) {
    WiFi.begin(ssid, password);
    unsigned long startAttempt = millis();
    while (WiFi.status() != WL_CONNECTED && millis() - startAttempt < timeoutMillis) {
      delay(500);
    }

    if (WiFi.status() != WL_CONNECTED) {
       Serial.println("WiFi connection failed");
    }
    else if(WiFi.status() == WL_CONNECTED){
      Serial.println("WiFi connected");
    }
  }
}
//    Serial2.write(DATA);
  }

  // Check if there's a command from STM32 on Serial2
  if (Serial2.available()) {
    int cmd = Serial2.read();
    Serial.println(cmd); // For debugging on PC

    if (cmd == setupSignal) {
      performFirmwareUpdate();
//        Serial2.println(":1000000000000120C5020208CB020208CF0202084C");
//        Serial.println(":1000000000000120C5020208CB020208CF0202084C");
    } 
    else if (cmd == 254) {  // Special request to send volume, songNum, wifiState
      uint8_t wifiState = (WiFi.status() == WL_CONNECTED) ? 1 : 0;
      
      Serial2.write(wifiState);
      Serial.println(wifiState);
    }
    else if(cmd == 253){
       bool VERSION = checkFirmwareVersion(newVersion);
       if(VERSION == true)
       {
         Serial2.println(newVersion);
         Serial.println(newVersion);
       }else{
          Serial2.println(lastVersion);
          Serial.println("No New Version");
       }
    }
    else {
      handleDFPlayerCommand(cmd);  // Handle normal DFPlayer commands
    }
    cmd=0;
  }
}


void handleDFPlayerCommand(int cmd) {
  switch (cmd) {
    case 0x01: myDFPlayer.next(); break;
    case 0x02: myDFPlayer.previous(); break;
    case 0x03: myDFPlayer.play(Serial2.read()); break;
    case 0x04: myDFPlayer.volumeUp(); break;
    case 0x05: myDFPlayer.volumeDown(); break;
    case 0x06: myDFPlayer.volume(Serial2.read()); break;
    case 0x07: myDFPlayer.EQ(Serial2.read()); break;
    case 0x0E: myDFPlayer.pause(); break;
    case 0x0D: myDFPlayer.start(); break;
    case 0x43: Serial2.write(myDFPlayer.readVolume()); break;
    case 0x4C: Serial2.write(myDFPlayer.readCurrentFileNumber()); break;
    case 0x44: Serial2.write(myDFPlayer.readEQ()); break;
    default: break;
  }
}

void performFirmwareUpdate() {
  HTTPClient http;
  http.setTimeout(timeoutMillis);
  http.begin(String(serverURL) + hexPath);
  int httpCode = http.GET();

  if (httpCode == 200) {
    Serial.println("Connected to firmware server successfully.");

    WiFiClient* stream = http.getStreamPtr();
    while (http.connected() && stream->available()) {
      String line = stream->readStringUntil('\n');
      line.trim();
      if (!line.isEmpty()) {
        Serial2.println(line);
        Serial.println(line);

        ACK = 0;
        unsigned long ackStart = millis(); // 🔸 وقت بداية الانتظار

        while (ACK != Reply_ACK) {
          // 🔹 check timeout
          if (millis() - ackStart > ACK_TIMEOUT_MS) {
            Serial.println("Timeout waiting for ACK. Update aborted.");
            return; // ❌ خروج من الفانكشن لو مفيش رد
          }

          if (Serial2.available()) {
            ACK = Serial2.read();

            if (ACK == SendSameLine) {
              Serial.println("Resending line:");
              Serial.println(line);
              Serial2.println(line);
              ackStart = millis(); // reset timeout
            } else if (ACK == Cancle) {
              Serial.println("Firmware update cancelled by receiver.");
              Serial2.print('F');
              return;
            }
          }

          delay(1); // 🔹 منع استهلاك المعالج 100%
        }

        Serial.print("Received ACK: ");
        Serial.println(ACK);
        delay(10);
      }
    }
  } else {
    Serial.print("HTTP GET failed with code: ");
    Serial.println(httpCode);
  }
}



bool checkFirmwareVersion(String& newVersion) {
  if (WiFi.status() != WL_CONNECTED) {
    WiFi.begin(ssid, password);
    unsigned long startAttempt = millis();
    while (WiFi.status() != WL_CONNECTED && millis() - startAttempt < timeoutMillis) {
      delay(500);
    }

    if (WiFi.status() != WL_CONNECTED) {
      Serial.println("WiFi connection failed");
      return false;
    }
  }

  HTTPClient http;
  http.setTimeout(timeoutMillis);
  http.begin(String(serverURL) + versionPath);
  int httpCode = http.GET();

  if (httpCode == 200) {
    String fetchedVersion = http.getString();
    fetchedVersion.trim();

    if (fetchedVersion != lastVersion) {
      newVersion = fetchedVersion;
      lastVersion = fetchedVersion;  
      Serial.println("New firmware version found: " + newVersion);
      http.end();
      return true;
    } else {
      Serial.println("No new version available.");
    }
  } else {
    Serial.println("Failed to fetch version file. HTTP code: " + String(httpCode));
  }

  http.end();
  return false;
}
