bool isActive = false, found = false;

// Function to play a tone on the buzzer
void playTone(int frequency) {
  int period = 1000000 / frequency;
  int halfPeriod = period / 2;
  int cycles = frequency * 125 / 1000;

  // Generate the specified tone
  for (int i = 0; i < cycles; ++i) {
    digitalWrite(Buzzer, HIGH);
    delayMicroseconds(halfPeriod);
    digitalWrite(Buzzer, LOW);
    delayMicroseconds(halfPeriod);
  }
}

void TaskSystemC(void *pvParameters) {
  // Avoid compiler warning about unused parameter
  (void)pvParameters;

  unsigned long time;
  bool isToAwait = false;
  byte ledState = HIGH;

  // Infinite loop for continuous task execution
  while (1) {
    // Update LCD with isActive state
    lcd.setCursor(8, 0);
    lcd.print(isActive);

    // If not active, delay and continue to the next iteration
    if (!isActive) {
      vTaskDelay(pdMS_TO_TICKS(100));
      continue;
    }

    // PIR sensor detected motion
    if (digitalRead(PIR) == HIGH && !found) {
      time = millis();
      found = true;
      isToAwait = false;
    }

    // Execute actions when motion is detected
    if (found && !isToAwait) {
      playTone(1000);
      digitalWrite(Room2, ledState);
      ledState = !ledState;

      // Check if the elapsed time is greater than 10 seconds
      if (millis() - time > 10000) {
        digitalWrite(Buzzer, LOW);
        ledState = LOW;
        digitalWrite(Room2, LOW);
        isToAwait = true;
        time = millis();
      }
    }

    // Wait for 5 seconds after the action and reset
    if (isToAwait && millis() - time > 5000) {
      isToAwait = false;
      time = millis();
    }

    // Delay to control the update rate of the task
    vTaskDelay(pdMS_TO_TICKS(100));
  }
}

void interruptHandler() {
  // Toggle the isActive state and reset variables
  isActive = !isActive;
  found = false;
  delay(10); // Small delay to debounce the button
  digitalWrite(Buzzer, LOW);
  digitalWrite(Room2, LOW);
}
