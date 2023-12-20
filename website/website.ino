#include <Wire.h>
#include <ESP8266WiFi.h>
#include <ESP8266WebServer.h>
#include <NTPClient.h>
#include <WiFiUdp.h>

// WiFi credentials
const char *ssid = "Tino's iPhone";
const char *password = "55c8tbg8x12cj";

// WebServer
ESP8266WebServer server(80);

// Define NTP Client to get time
WiFiUDP ntpUDP;
NTPClient timeClient(ntpUDP, "pool.ntp.org");

// Temperature and Humidity
float temperature = 0.0;
float humidity = 0.0;

void setup() {
  Serial.begin(9600);

  // Connect to WiFi
  WiFi.begin(ssid, password);
  while (WiFi.status() != WL_CONNECTED) {
    delay(1000);
    Serial.println("Connecting to WiFi...");
  }
  Serial.println("Connected to WiFi");

  // Initialize the web server
  server.on("/", HTTP_GET, handleRoot);
  server.on("/data", HTTP_GET, handleData);
  server.begin();

  Serial.print("Web server started: ");
  Serial.println(WiFi.localIP());

  // Initialize the NTP client
  timeClient.begin();

  // Initialize I2C communication
  Wire.begin(D1, D2);
}

void loop() {
  server.handleClient();

  // Request data from the I2C slave
  Wire.requestFrom(8, 2 * sizeof(float));
  while(Wire.available() >= 2 * sizeof(float)){
    Wire.readBytes((uint8_t*)&temperature, sizeof(float));
    Wire.readBytes((uint8_t*)&humidity, sizeof(float));
  }
}

void handleRoot() {
  // HTML page provided by the server
  String html = String(
  "<!DOCTYPE html>"
  "<html lang='en'>"
    "<head>"
      "<meta charset='UTF-8'>"
      "<meta name='viewport' content='width=device-width, initial-scale=1.0'>"
      "<title>ESP8266 Data Visualization</title>"
      "<script src='https://cdn.jsdelivr.net/npm/chart.js'></script>"
      "<script src='https://cdn.jsdelivr.net/npm/chartjs-plugin-zoom'></script>"
    "</head>"
    "<body>"
      "<h1>ESP8266 Data Visualization</h1>"
      "<div style='width: 80%; overflow-x: auto;'>"
        "<canvas id='myChart'></canvas>"
      "</div>"
      "<script>"
        "var ctx = document.getElementById('myChart').getContext('2d');"
        "var labels = [];"
        "var dataTemp = [];"
        "var dataHum = [];"
        "var myChart = null;"
        "function updateChart() {"
          "var xhr = new XMLHttpRequest();"
          "xhr.open('GET', '/data', true);"
          "xhr.onload = function () {"
            "if (xhr.status === 200) {"
              "var responseData = xhr.responseText.split(',');"
              "labels = labels.concat(responseData[0]);"
              "dataTemp = dataTemp.concat(responseData[1]);"
              "dataHum = dataHum.concat(responseData[2]);"
              "if (labels.length > 30) {"
                "labels = labels.slice(-30);"
                "dataTemp = dataTemp.slice(-30);"
                "dataHum = dataHum.slice(-30);"
              "}"
              "updateChartWithData();"
            "}"
          "};"
          "xhr.send();"
        "}"
        "function updateChartWithData() {"
          "if (!myChart) {"
            "myChart = new Chart(ctx, {"
              "type: 'line',"
              "data: {"
                "labels: labels,"
                "datasets: ["
                  "{"
                    "label: 'Temperature',"
                    "data: dataTemp,"
                    "backgroundColor: 'rgba(255, 186, 128, 0.2)',"
                    "borderColor: 'rgba(255, 186, 128, 1)',"
                    "borderWidth: 1"
                  "},"
                  "{"
                    "label: 'Humidity',"
                    "data: dataHum,"
                    "backgroundColor: 'rgba(187, 209, 255, 0.2)',"
                    "borderColor: 'rgba(187, 209, 255, 1)',"
                    "borderWidth: 1"
                  "}"
                "]"
              "},"
              "options: {"
                "scales: {"
                  "y: {"
                    "beginAtZero: true"
                  "}"
                "},"
              "}"
            "});"
          "} else {"
            "myChart.data.labels = labels;"
            "myChart.data.datasets[0].data = dataTemp;"
            "myChart.data.datasets[1].data = dataHum;"
            "myChart.update();"
          "}"
        "}"
        "setInterval(updateChart, 1000);"
      "</script>"
    "</body>"
  "</html>");

  server.send(200, "text/html", html);
}

void handleData() {  
  // Update the NTP client and get the current time
  timeClient.update();

  // Format data
  String response = String(timeClient.getFormattedTime()) + "," + String(temperature, 2) + "," + String(humidity, 2);

  // Send data as text
  server.send(200, "text/plain", response);
}