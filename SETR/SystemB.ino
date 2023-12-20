float hum, temp; // Global variables to store humidity and temperature

void TaskSystemB(void *pvParameters) {
  // Avoid compiler warning about unused parameter
  (void)pvParameters;

  // Flag to track whether cooling is active
  bool isCooling = false;

  // Infinite loop for continuous task execution
  while (1) { 
    // Read humidity and temperature from the DHT sensor
    hum = dht.readHumidity();
    temp = dht.readTemperature();

    // Update LCD with temperature and humidity values
    lcd.setCursor(5, 1);
    lcd.print(int(temp));
    lcd.setCursor(13, 1);
    lcd.print(int(hum));
    
    // Cooling control based on temperature
    if (!isCooling && temp > 24) {
      digitalWrite(Room1R, HIGH);
      digitalWrite(Room1G, LOW);
      digitalWrite(AC, HIGH);
      lcd.setCursor(0, 0);
      lcd.print("Fan ON ");
      isCooling = true;
    } else if (isCooling && temp < 20) {
      digitalWrite(Room1R, LOW);
      digitalWrite(Room1G, HIGH);
      digitalWrite(AC, LOW);
      lcd.setCursor(0, 0);
      lcd.print("Fan OFF");
      isCooling = false;
    }

    // Delay to control the update rate of the task
    vTaskDelay(pdMS_TO_TICKS(100));
  }
}

void requestEvent() {
  // Respond to I2C master request with temperature and humidity values
  Wire.write((uint8_t*)&temp, sizeof(float));
  Wire.write((uint8_t*)&hum, sizeof(float));
}
