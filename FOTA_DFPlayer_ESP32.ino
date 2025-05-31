#include <WiFi.h>
#include <HTTPClient.h>
#include <DFRobotDFPlayerMini.h>

const char* ssid = "Tarek ";
const char* password = "shtzr@shtzr1";
const char* serverURL = "http://fota-portal.atwebpages.com/";
const char* versionPath = "version.txt";
const char* hexPath = "firmware/firmware.hex";

const char setupSignal = 's';
const unsigned long timeoutMillis = 5000;

String lastVersion = "1.0.0";
DFRobotDFPlayerMini myDFPlayer;

// Serial1 = DFPlayer (TX=GPIO10, RX=GPIO9)
// Serial2 = STM32 (TX=GPIO17, RX=GPIO16)

void setup() {
  Serial.begin(9600);  // Debug output
//  Serial1.begin(9600, SERIAL_8N1, 9, 10);   // STM32
  Serial2.begin(9600, SERIAL_8N1, 16, 17);  // DFPlayer

  WiFi.begin(ssid, password);
  Serial.print("Connecting to WiFi");
  unsigned long startAttempt = millis();
  while (WiFi.status() != WL_CONNECTED && millis() - startAttempt < timeoutMillis) {
    delay(500);
    Serial.print(".");
  }
  Serial.println(WiFi.status() == WL_CONNECTED ? "\nConnected" : "\nFailed");

  // Init DFPlayer
  if (!myDFPlayer.begin(Serial2)) {
    Serial.println("DFPlayer initialization failed!");
    while (1);
  }
  Serial.println("DFPlayer initialized.");
  myDFPlayer.volume(20); // Set initial volume  
}

void loop() {
  if (Serial.available()) {
    int cmd = Serial.parseInt();
    if (cmd == setupSignal) {
      checkAndUpdateFirmware(); // Your existing firmware update logic
    } else {
      handleDFPlayerCommand(cmd);
    }
  }
}

void handleDFPlayerCommand(int cmd) {
  switch (cmd) {
    case 0x01: myDFPlayer.next(); break;
    case 0x02: myDFPlayer.previous(); break;
    case 0x03: myDFPlayer.play(Serial.parseInt()); break;
    case 0x04: myDFPlayer.volumeUp(); break;
    case 0x05: myDFPlayer.volumeDown(); break;
    case 0x06: myDFPlayer.volume(Serial.parseInt()); break;
    case 0x07: myDFPlayer.EQ(Serial.parseInt()); break;
    case 0x0E: myDFPlayer.pause(); break;
    case 0x0D: myDFPlayer.start(); break;
    case 0x43: Serial.print(myDFPlayer.readVolume()); break;
    case 0x4C: Serial.print(myDFPlayer.readCurrentFileNumber()); break;
    case 0x44: Serial.print(myDFPlayer.readEQ()); break;
    default: break;
  }
}


// -------- VERSION CHECK + UPDATE --------
void checkAndUpdateFirmware() {
  bool error = false;
  bool updated = false;
  String currentVersion;

  // --- Version Check ---
  if (WiFi.status() != WL_CONNECTED) {
    Serial.println("WiFi not connected. Trying to reconnect...");
    WiFi.begin(ssid, password);
    unsigned long startAttempt = millis();
    while (WiFi.status() != WL_CONNECTED && millis() - startAttempt < timeoutMillis) {
      delay(500);
      Serial.print(".");
    }

    if (WiFi.status() != WL_CONNECTED) {
      Serial.println("\nReconnection failed.");
      Serial2.println("0xEE"); // error signal
      return;
    }
    Serial.println("\nWiFi reconnected.");
  }

  HTTPClient http;
  http.setTimeout(timeoutMillis);
  http.begin(String(serverURL) + versionPath);
  int httpCode = http.GET();

  if (httpCode == 200) {
    currentVersion = http.getString();
    currentVersion.trim();
    Serial.println("Fetched version: " + currentVersion);

    if (currentVersion != lastVersion) {
      updated = true;
      lastVersion = currentVersion;
    }
  } else {
    Serial.println("Failed to fetch version.txt");
    Serial2.println("0xEE"); // error signal
    http.end();
    return;
  }
  http.end();

  // --- Handle Result ---
  if (updated) {
    Serial.println("New version found. Downloading hex...");
    http.begin(String(serverURL) + hexPath);
    httpCode = http.GET();

    if (httpCode == 200) {
      WiFiClient* stream = http.getStreamPtr();
      while (http.connected() && stream->available()) {
        String line = stream->readStringUntil('\n');
        line.trim();
        if (!line.isEmpty()) {
          Serial.println("Sending: " + line);  // Debug print to Serial Monitor
          Serial2.println(line);               // Send to STM32
          delay(10);                           // Allow STM32 to process
        }
      }
    } else {
      Serial.println("Failed to fetch hex file.");
      Serial2.println("0xEE"); // error signal
    }
    http.end();
  } else {
    Serial.println("No update available.");
    Serial2.println("0x00"); // no update signal
  }
}
