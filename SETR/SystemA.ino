void TaskSystemA(void *pvParameters) {
  // Avoid compiler warning about unused parameter
  (void)pvParameters;

  // Variables to store light sensor and LED values
  int val_light = 0, val = 0;

  // Infinite loop for continuous task execution
  while (1) 
  {
    // Read analog value from the light sensor
    val_light = analogRead(Light);
    
    // Map the light sensor value to LED brightness
    if (val_light >= 800) val = 255;
    else if (val_light >= 600) val = 192;
    else if (val_light >= 400) val = 128;
    else if (val_light >= 200) val = 64;
    else val = 0;

    // Print light sensor and LED values to Serial Monitor
    Serial.print("Light Sensor: ");
    Serial.print(val_light);
    Serial.print(" Light Value: ");
    Serial.println(val);

    // Adjust LED brightness based on light sensor value
    if (statesLed[0] != 0) analogWrite(Room3, val);
    if (statesLed[1] != 0) analogWrite(Room4, val);

    // Delay to control the update rate of the task
    vTaskDelay(pdMS_TO_TICKS(100));
  }
}