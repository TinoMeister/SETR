// Return -1, if supplied code does not map to a key.
int mapCodeToButton(unsigned long code) {
  // Check for codes from this specific remote
  if ((code & 0x0000FFFF) == 0x0000FF00) {

    // No longer need the lower 16 bits. Shift the code by 16
    // to make the rest easier.
    code >>= 16;
    // Check that the value and inverse bytes are complementary.
    if (((code >> 8) ^ (code & 0x00FF)) == 0x00FF) {
      return code & 0xFF;
    }
  }
  return -1;
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

void TaskSystemD(void *pvParameters)
{
  (void)pvParameters;
  unsigned long time;
  int button = -1;

  while (1) 
  {
    //Serial.println(F("System D"));
    button = readInfrared();

    switch (button) {
      case 12: // 1
        // statements
        statesLed[0] = !statesLed[0];
        digitalWrite(Room3, LOW);
        break;
      case 24: // 2
        // statements
        statesLed[1] = !statesLed[1];
        digitalWrite(Room4, LOW);
        break;
      case 94: // 3
        // statements
        break;
      case 8: // 4
        // statements
        break;
      case 28: // 5
        // statements
        break;
      case 90: // 6
        // statements
        break;
      default:
        // statements
        break;
    }
    
    vTaskDelay(pdMS_TO_TICKS(100));
  }
}