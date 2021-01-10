#include "config.h"

WiFiUDP Udp;

// my lolin with oled
SSD1306 display(0x3c, 5, 4);

unsigned long last_ts;
bool state;

// support cyrillic
char FontUtf8Rus(const byte ch) {
  static uint8_t LASTCHAR;

  if ((LASTCHAR == 0) && (ch < 0xC0)) {
    return ch;
  }

  if (LASTCHAR == 0) {
    LASTCHAR = ch;
    return 0;
  }

  uint8_t last = LASTCHAR;
  LASTCHAR = 0;

  switch (last) {
    case 0xD0:
      if (ch == 0x81) return 0xA8;
      if (ch >= 0x90 && ch <= 0xBF) return ch + 0x30;
      break;
    case 0xD1:
      if (ch == 0x91) return 0xB8;
      if (ch >= 0x80 && ch <= 0x8F) return ch + 0x70;
      break;
  }

  return (uint8_t) 0;
}

void setup() {
  Serial.begin(115200);

  Serial.println("Hello!");

  pinMode(CAT_PIN, OUTPUT);
  digitalWrite(CAT_PIN, LOW);

  display.init();
  display.setFontTableLookupFunction(FontUtf8Rus);
  display.flipScreenVertically();

  WiFi.mode(WIFI_STA);
  WiFi.begin(STASSID, STAPSK);
  while (WiFi.status() != WL_CONNECTED) {
    Serial.print('.');
    delay(500);
  }
  Serial.print("Connected! IP address: ");
  Serial.println(WiFi.localIP());
  Serial.printf("UDP server on port %d\n", LOCAL_PORT);
  Udp.begin(LOCAL_PORT);

  display.clear();
  display.setColor(WHITE);
  display.setTextAlignment(TEXT_ALIGN_CENTER);
  display.setFont(ArialMT_Plain_10);
  display.drawString(64, 0, WiFi.localIP().toString() + ":" + LOCAL_PORT);
  display.setFont(ArialRus_Plain_14);
  display.drawString(64, 15, "Waiting...");
  display.display();
}

void loop() {

  while (WiFi.status() != WL_CONNECTED) {
    Serial.print('.');
    delay(500);
  }

  // if there's data available, read a packet
  int packetSize = Udp.parsePacket();
  if (packetSize) {
    Serial.printf(
      "Received packet of size %d from %s:%d (to: %s:%d, free heap = %d B)\n",
      packetSize,
      Udp.remoteIP().toString().c_str(), Udp.remotePort(),
      WiFi.localIP().toString().c_str(), LOCAL_PORT,
      ESP.getFreeHeap()
    );

    // read the packet into packetBufffer
    String bufin = Udp.readString();

    Serial.println("Contents:");
    Serial.println(bufin);

    // send a reply, to the IP address and port that sent us the packet we received
    Udp.beginPacket(Udp.remoteIP(), Udp.remotePort());
    Udp.println(REPLY);
    Udp.endPacket();

    if (packetSize > 2) {
      last_ts = millis();
      state = true;
      digitalWrite(CAT_PIN, HIGH);
      Serial.println("ON");
    } else {
      state = false;
      digitalWrite(CAT_PIN, LOW);
      Serial.println("OFF (signal)");
    }

    display.clear();
    display.setColor(WHITE);
    display.setTextAlignment(TEXT_ALIGN_CENTER);
    display.setFont(ArialRus_Plain_16);
    display.drawStringMaxWidth(64, 15, display.width(), bufin);
    display.display();
  }

  if (state && millis() > last_ts + LAST_TIMEOUT)
  {
    state = false;
    digitalWrite(CAT_PIN, LOW);
    Serial.println("OFF (time)");
  }
}
