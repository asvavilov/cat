#define LAST_TIMEOUT 3000

#define CAT_PIN 2

// local port to listen on
#define LOCAL_PORT 8888

// buffers for receiving and sending data
// a string to send back
#define REPLY "ok"

#include "secure.h"
#include <Arduino.h>
#include <WiFi.h>
#include "SSD1306.h" // alias for `#include "SSD1306Wire.h"'
#include "fontsRus.h"
