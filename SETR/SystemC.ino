bool isActive = false, found = false;

// Function to play a tone on the buzzer
void playTone(int frequency) {
  unsigned int period = 1000000 / frequency;
  unsigned int halfPeriod = period / 2;
  unsigned int cycles = frequency * 125 / 1000;
  unsigned int i;

  // Generate the specified tone
  for (i = 0; i < cycles; ++i) {
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
  unsigned long startTime, endTime;

  // Infinite loop for continuous task execution
  while (1) {
    startTime = millis();

    // Update LCD with isActive state
    lcd.setCursor(8, 0);
    lcd.print(isActive);

    // If not active, delay and continue to the next iteration
    if (!isActive) {
      digitalWrite(Buzzer, LOW);
      digitalWrite(Room2, LOW);
      vTaskDelay(pdMS_TO_TICKS(100));
      continue;
    }

    // PIR sensor detected motion
    if (digitalRead(PIR) && !found) {
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

    endTime = millis();

    // Imprimir os tempos de execução
    Serial.println("Time: " + String(endTime - startTime) + " ms");

    // Delay to control the update rate of the task
    vTaskDelay(pdMS_TO_TICKS(100));
  }
}

void TaskSystemCWorst(void *pvParameters) {
  // Avoid compiler warning about unused parameter
  (void)pvParameters;

  long time;
  bool isToAwait = false;
  byte ledState = HIGH;
  long startTime, endTime;

  // Infinite loop for continuous task execution
  while (1) {
    startTime = millis();

    // Update LCD with isActive state
    lcd.setCursor(8, 0);
    lcd.print(isActive);

    if (isActive) {
      // PIR sensor detected motion
      if (digitalRead(PIR) == HIGH) {
        if (!found) {
          time = millis();
          found = true;
          isToAwait = false;
        }
      }
      // Execute actions when motion is detected
      if (found) {
        if (!isToAwait) {
          playTone(1000);
          digitalWrite(Room2, ledState);
          ledState = !ledState;

          // Check if the elapsed time is greater than 10 seconds
          if (millis() - time >= 10000) {
            digitalWrite(Buzzer, LOW);
            ledState = LOW;
            digitalWrite(Room2, LOW);
            isToAwait = true;
            time = millis();
          }
        }
      }
      
      // Wait for 5 seconds after the action and reset
      if (isToAwait) {
        if (millis() - time >= 5000) {
          isToAwait = false;
          time = millis();
        }
      }
    }
    else {
      digitalWrite(Buzzer, LOW);
      digitalWrite(Room2, LOW);
    }

    endTime = millis();

    // Imprimir os tempos de execução
    Serial.println("Time: " + String(endTime - startTime) + " ms");

    // Delay to control the update rate of the task
    vTaskDelay(pdMS_TO_TICKS(100));
  }
}

void interruptHandler() {
  // Toggle the isActive state and reset variables
  isActive = !isActive;
  found = false;
}
