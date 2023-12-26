// Return -1, if supplied code does not map to a key.
int mapCodeToButton(unsigned long code) {
  // Check for codes from this specific remote
  if ((code & 0x0000FFFF) == 0x0000FF00) {

    // No longer need the lower 16 bits. Shift the code by 16
    // to make the rest easier.
    code >>= 16;
    // Check that the value and inverse bytes are complementary.
    if (((code >> 8) ^ (code & 0x00FF)) == 0x00FF) {
      return code & 0xFF; // Extract the lower 8 bits as the button code
    }
  }
  return -1; // Return -1 if the code doesn't match the expected format
}

int readInfrared() {
  int result = -1;
  // Check if we've received a new code
  if (IrReceiver.decode()) {
    // Get the infrared code
    unsigned long code = IrReceiver.decodedIRData.decodedRawData;
    // Map it to a specific button on the remote
    result = mapCodeToButton(code);
    // Enable receiving of the next value
    IrReceiver.resume();
  }
  return result;
}

void openGate() {
  for (int i = 5; i < 170; i++) {
    myservo.write(i);
    delay(30);
  }
}

void closeGate() {
  for (int i = 170; i > 5; i--) {
    myservo.write(i);
    delay(30);
  }
}

void TaskSystemD(void *pvParameters)
{
  (void)pvParameters;
  unsigned long time;
  int button = -1;
  bool isOpen = false;

  while (1) 
  {
    button = readInfrared();

    switch (button) {
      case 12: // Button 1
        // Toggle LED state for Room3
        statesLed[0] = !statesLed[0];
        digitalWrite(Room3, LOW);
        break;
      case 24: // Button 2
        // Toggle LED state for Room4
        statesLed[1] = !statesLed[1];
        digitalWrite(Room4, LOW);
        break;
      case 94: // Button 3
        // Toggle gate state (open/close)
        if (isOpen) closeGate();
        else openGate();
        isOpen = !isOpen;
        break;
      default:
        // No action for other buttons
        break;
    }
    
    vTaskDelay(pdMS_TO_TICKS(100));
  }
}