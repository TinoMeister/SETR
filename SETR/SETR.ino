#include <Wire.h>
#include <Arduino_FreeRTOS.h>
#include <LiquidCrystal.h>
#include <IRremote.h>
#include <DHT.h>

/* GENERAL VARIABLES*/
#define Light A0
#define Room1G 8
#define Room1R 10
#define Room2 9
#define Room3 11
#define Room4 12
#define AC 53

#define Command 5

#define Buzzer 39
#define PIR 7
#define Button 18

#define DHTPIN 45
#define DHTTYPE DHT22
DHT dht(DHTPIN, DHTTYPE);

LiquidCrystal lcd(51, 2, 3, 4, 49, 47);

//Function Declaration
void TaskSystemA(void *pvParameters);
void TaskSystemB(void *pvParameters);
void TaskSystemC(void *pvParameters);
void TaskSystemD(void *pvParameters);
 
// R3, R4
bool statesLed[2] = {0, 0};

void setup() {
  // put your setup code here, to run once:
  Serial.begin(9600);

  // Initialize I2C as Slave
  Wire.begin(8);
  Wire.onRequest(requestEvent);

  // Initialize pin modes
  pinMode(Room1G, OUTPUT);
  pinMode(Room1R, OUTPUT);
  pinMode(Room2, OUTPUT);
  pinMode(Room3, OUTPUT);
  pinMode(Room4, OUTPUT);

  pinMode(Light, INPUT); 
  pinMode(AC, OUTPUT);

  pinMode(Buzzer, OUTPUT);
  noTone(Buzzer);
  
  pinMode(Button, INPUT_PULLUP);
  pinMode(PIR, INPUT);

  // Initialize IR Receiver
  IrReceiver.begin(Command);

  // Initialize DHT sensor
  dht.begin();

  // Initialize LCD
  lcd.begin(16, 2);

  // Set initial state for Room1 LEDs and LCD
  digitalWrite(Room1R, LOW);
  digitalWrite(Room1G, HIGH);
  lcd.setCursor(0, 0);
  lcd.print("Fan OFF");
  lcd.setCursor(0, 1);
  lcd.print("Temp:");
  lcd.setCursor(9, 1);
  lcd.print("Hum:");

  // Attach interrupt for the button
  attachInterrupt(digitalPinToInterrupt(Button), interruptHandler, FALLING);

  // Create FreeRTOS tasks
  xTaskCreate(TaskSystemA, "System A", 128, NULL, 1, NULL);
  xTaskCreate(TaskSystemB, "System B", 500, NULL, 1, NULL);
  xTaskCreate(TaskSystemC, "System C", 1000, NULL, 1, NULL);
  xTaskCreate(TaskSystemD, "System D", 255, NULL, 1, NULL);

  // Start FreeRTOS scheduler
  vTaskStartScheduler();
}

void loop() {
  // No code in the loop function when using FreeRTOS
}
