// Reference
// ESP32 http
// https://qiita.com/zakkied/items/ee68f6a49d7921549bc7
// Slack API
// https://qiita.com/sudo5in5k/items/947f0bf6450ac7435a9c

#include <WiFi.h>
#include <HTTPClient.h>

// Wifi
const char ssid[] = "YOUR_WIFI_SSID";
const char passwd[] = "YOUR_WIFI_PASSWORD";

// Token (Get from https://api.slack.com/custom-integrations/legacy-tokens)
const char slackToken[] = "yourtoken-000000000000-000000000000-000000000000-00000000000000000000000000000000";
// Channel ID (Check the URL of Slack web ver)
const char channel[] = "C00000000";
// Message you will send
const char msg[] = "Hello!";


#define LED_PIN 13

bool postSlack() {
  HTTPClient http;
  http.begin("https://www.slack.com/api/chat.postMessage");
  http.addHeader("Content-Type", "application/x-www-form-urlencoded;charset=UTF-8");

  char authstr[255];
  sprintf(authstr, "Bearer %s", slackToken);
  http.addHeader("Authorization", authstr);

  char params[255];
  sprintf(params, "channel=%s&text=%s&as_user=true", channel, msg);  
  int httpCode = http.POST(params);
  Serial.printf("Response: %d", httpCode);
  Serial.println();

  if (httpCode == HTTP_CODE_OK) {
    String body = http.getString();
    Serial.print("Response Body: ");
    Serial.println(body);
    if (strstr(body.c_str(), "true") != NULL) {
        return true;
    }
  }

  return false;
}

void setup() {
  pinMode(LED_PIN, OUTPUT);

  Serial.begin(115200);
  while (!Serial);

  WiFi.begin(ssid, passwd);
  Serial.print("WiFi connecting");

  while (WiFi.status() != WL_CONNECTED) {
    Serial.print(".");
    delay(100);
  }

  Serial.println(" connected");

  if (postSlack()) { // success
    for (int i=0; i<10; ++i) {
        digitalWrite(LED_PIN, 1);delay(250);
        digitalWrite(LED_PIN, 0);delay(250);
    }
  } else {            // fail
    for (int i=0; i<5; ++i) {
        digitalWrite(LED_PIN, 1);delay(100);
        digitalWrite(LED_PIN, 0);delay(900);
    }
  }

}

void loop() {
  // do nothing
}
