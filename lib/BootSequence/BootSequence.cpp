#include <BootSequence.h>

/**
 * @brief Indicates whether the Wi-Fi connection has been established.
 */
bool wifiConnected = false;
/**
 * @brief Indicates whether the touch button interrupts have been configured
 * successfully.
 */
bool touchButtonsAreLoaded = false;
/**
 * @brief Indicates whether the mode LED hardware has been initialized
 * successfully.
 */
bool ledIsLoaded = false;
/**
 * @brief Indicates whether the buzzer hardware has been initialized
 * successfully.
 */
bool buzzerIsLoaded = false;
/**
 * @brief Indicates whether the IMU hardware has been initialized successfully.
 */
bool imuIsLoaded = false;
/**
 * @brief Indicates whether the BME environmental sensor has been initialized
 * successfully.
 */
bool bmeIsLoaded = false;
/**
 * @brief Indicates whether the MQTT connection has been established
 * successfully.
 */
bool mqttConnected = false;
/**
 * @brief Indicates whether the ADC hardware has been initialized successfully.
 */
bool adcIsLoaded = false;
/**
 * @brief Indicates whether the DAC hardware has been initialized successfully.
 */
bool dacIsLoaded = false;
/**
 * @brief Indicates whether the SD card has been mounted successfully.
 */
bool sdMounted = false;
/**
 * @brief Indicates whether the screen has been initialized successfully.
 */
bool screenMounted = false;
/**
 * @brief Indicates whether the battery is sufficiently charged for operation.
 */
bool batteryCharged = false;
/**
 * @brief Maximum time to wait for a component to become available, in
 * milliseconds.
 */
const int TIMEOUT_MS = 11000;
/**
 * @brief Interval for animating boot screen dots, in milliseconds.
 */
const int DOT_ANIMATION_INTERVAL_MS = 300;

/**
 * @brief Initializes the screen hardware.
 *
 * @return True if the screen was initialized successfully, otherwise false.
 */
bool screenLoaded() { return screen.begin(SSD1306_SWITCHCAPVCC, SSD1306_I2C); }

/**
 * @brief Configures SD card pins and mounts the SD card.
 *
 * @return True if the SD card was mounted successfully, otherwise false.
 */
bool sdLoaded() {
  SD_MMC.setPins(SD_CLOCK, SD_COMMAND, SD_D0, SD_D1, SD_D2, SD_D3);

  return SD_MMC.begin("/sdcard", true);
}

/**
 * @brief Configures the I/O direction and initial output states for the port
 * extender.
 *
 * This preloads output latches high so that LEDs remain off initially, then
 * configures all pins on both GPIO banks as outputs.
 */
void configurePinsForPortExtender() {
  // Output latches HIGH so LEDs stay off
  mcpWriteRegister(GPIOA, 0xFF);
  mcpWriteRegister(GPIOB, 0xFF);
  gpioAState = 0xFF;
  gpioBState = 0xFF;

  // Set all pins as outputs.
  mcpWriteRegister(IODIRA, 0x00);
  mcpWriteRegister(IODIRB, 0x00);
}

/**
 * @brief Initializes the basic hardware required before the full boot sequence.
 *
 * This includes serial communication, I2C, the port extender, screen, and SD
 * card. If the SD card or screen cannot be initialized, an error message is
 * printed and the function returns early.
 */
void setupBasicHardware() {
  Serial.begin(115200);
  delay(200);

  Wire.begin(SDA_PIN, SCL_PIN);

  configurePinsForPortExtender();

  screenMounted = screenLoaded();
  sdMounted = sdLoaded();

  if (!sdMounted) {
    Serial.println("SD_MMC mount failed");
    if (SD_MMC.cardType() == CARD_NONE) {
      Serial.println("No SD card attached");
      sdMounted = false;
    }
    return;
  }

  if (!screenMounted) {
    Serial.println("Screen mount failed");
    return;
  }
}

/**
 * @brief Configures touch button interrupt handlers.
 *
 * This function attaches touch interrupts for mode selection and touch action
 * buttons, then reports successful initialization.
 *
 * @return ComponentStatus Status result for the touch button initialization.
 */
ComponentStatus touchButtonsLoaded() {
  touchAttachInterrupt(TOUCH_RIGHT_PIN, increaseModeNumber,
                       TOUCH_DETECTION_THRESHOLD);
  touchAttachInterrupt(TOUCH_LEFT_PIN, decreaseModeNumber,
                       TOUCH_DETECTION_THRESHOLD);
  touchAttachInterrupt(TOUCH_UP_PIN, onTouchUp, TOUCH_DETECTION_THRESHOLD);
  touchAttachInterrupt(TOUCH_DOWN_PIN, onTouchDown, TOUCH_DETECTION_THRESHOLD);
  touchAttachInterrupt(TOUCH_X_PIN, onTouchX, TOUCH_DETECTION_THRESHOLD);
  return {"", true};
}

/**
 * @brief Initializes the mode blink LED.
 *
 * @return ComponentStatus Status result for the LED initialization.
 */
ComponentStatus modeBlinkLEDLoaded() {
  pinMode(LED_BOOT_BUTTON_PIN, OUTPUT);
  digitalWrite(LED_BOOT_BUTTON_PIN, HIGH);
  return {"", true};
}

/**
 * @brief Initializes the buzzer output channel.
 *
 * @return ComponentStatus Status result for the buzzer initialization.
 */
ComponentStatus buzzerLoaded() {
  ledcSetup(BUZZER_CHANNEL, 2000, 12);
  ledcAttachPin(BUZZER_PIN, BUZZER_CHANNEL);
  ledcWrite(BUZZER_CHANNEL, 0);
  return {"", true};
}

/**
 * @brief Initializes the IMU sensor.
 *
 * @return ComponentStatus Status result for the IMU initialization.
 */
ComponentStatus imuLoaded() { return {"", imu.begin_SPI(IMU_CS)}; }

/**
 * @brief Initializes the BME environmental sensor.
 *
 * @return ComponentStatus Status result for the BME initialization.
 */
ComponentStatus bmeLoaded() { return {"", bme.begin(BME280_I2C, &Wire)}; }

/**
 * @brief Initializes the ADC and configures its gain.
 *
 * @return ComponentStatus Status result for the ADC initialization.
 */
ComponentStatus adcLoaded() {
  bool loaded = adc.begin(ADS1X15_I2C, &Wire);
  adc.setGain(GAIN_TWO);
  return {"", loaded};
}

/**
 * @brief Initializes the DAC hardware.
 *
 * @return ComponentStatus Status result for the DAC initialization.
 */
ComponentStatus dacLoaded() { return {"", dac.begin(MCP4725_I2C, &Wire)}; }

/**
 * @brief Starts the Wi-Fi connection attempt using the configured credentials.
 */
void startWifiConnection() { WiFi.begin(WIFI_SSID.c_str(), WIFI_PASSWORD); }

/**
 * @brief Checks whether the configured Wi-Fi SSID is visible.
 *
 * @return True if the SSID exists, otherwise false.
 */
bool wifiSSIDExists() { return WiFi.status() != WL_NO_SSID_AVAIL; }

/**
 * @brief Checks whether Wi-Fi is connected and provides a status message if
 * not.
 *
 * @return ComponentStatus Status result for the Wi-Fi connection state.
 */
ComponentStatus wifiIsConnected() {
  String status;
  if (WiFi.status() == WL_NO_SSID_AVAIL) {
    status = "SSID not found";
  }

  return {status, WiFi.status() == WL_CONNECTED};
}

/**
 * @brief Checks whether the battery voltage is above the minimum threshold.
 *
 * @return ComponentStatus Status result for battery availability.
 */
ComponentStatus batteryIsAvailable() {
  return {"", getBatteryVoltage() >= 3.2f};
}

/**
 * @brief Determines whether the full device boot sequence completed
 * successfully.
 *
 * @return True if all required components are loaded and battery is available,
 * otherwise false.
 */
bool deviceHasBootedSuccessfully() {
  return wifiConnected && touchButtonsAreLoaded && ledIsLoaded &&
         buzzerIsLoaded && imuIsLoaded && bmeIsLoaded && mqttConnected &&
         adcIsLoaded && dacIsLoaded && batteryCharged;
}

/**
 * @brief Generates an animated dot string for the boot screen.
 *
 * The number of dots cycles from 0 to 3 based on the elapsed time.
 *
 * @param elapsedMs Elapsed time in milliseconds.
 * @return Animated dot string.
 */
String animatedDots(long elapsedMs) {
  int dotCount = (elapsedMs / DOT_ANIMATION_INTERVAL_MS) % 4;
  String dots = "";
  for (int i = 0; i < dotCount; i++) {
    dots += ".";
  }
  return dots;
}

/**
 * @brief Draws the boot screen with up to four lines of text.
 *
 * @param bootTitle Title shown at the top of the boot screen.
 * @param line2 Second line of text.
 * @param line3 Third line of text.
 * @param line4 Fourth line of text.
 */
void drawBootScreen(const String &bootTitle, const String &line2,
                    const String &line3, const String &line4) {
  clearScreen();
  drawText(bootTitle, {10, 10});
  drawText(line2, {10, 35});
  drawText(line3, {10, 45});
  drawText(line4, {10, 55});
  updateScreen();
}

/**
 * @brief Displays a status message on the screen for a fixed duration.
 *
 * @param message Message to display.
 * @param timeToDisplayMs Duration to display the message in milliseconds.
 * @return void
 */
void displayStatus(const String &message, int timeToDisplayMs) {
  clearScreen();
  drawText(message, {1, 10});
  updateScreen();
  delay(timeToDisplayMs);
}

/**
 * @brief Displays the current boot status of a component.
 *
 * If the component is not ready, an animated waiting message is shown.
 *
 * @param bootTitle Title shown at the top of the boot screen.
 * @param componentName Name of the component being initialized.
 * @param status True if the component is ready, otherwise false.
 * @param elapsedMs Elapsed waiting time in milliseconds.
 */
void displayBootStatus(const String &bootTitle, const String &componentName,
                       bool status, long elapsedMs) {
  String statusText = status ? "ready" : "waiting" + animatedDots(elapsedMs);
  drawBootScreen(bootTitle, "Starting:", componentName, statusText);

  delay(status ? 150 : 100);
}

/**
 * @brief Displays a boot error for a component.
 *
 * @param bootTitle Title shown at the top of the boot screen.
 * @param componentName Name of the component that failed.
 * @param error Error message to display.
 */
void displayError(const String &bootTitle, const String &componentName,
                  const String &error) {
  drawBootScreen(bootTitle, "FAILED:", componentName, error);
}

/**
 * @brief Waits for a component to report that it has loaded successfully.
 *
 * This function repeatedly checks the component status until it is loaded,
 * an error is reported, or the timeout is exceeded.
 *
 * @param bootTitle Title shown at the top of the boot screen.
 * @param name Name of the component being initialized.
 * @param isLoadedFunc Function returning the current component status.
 * @return True if the component loaded successfully, otherwise false.
 */
bool waitForComponent(const String &bootTitle, const String &name,
                      ComponentStatus (*isLoadedFunc)()) {
  long startTime = systemClock.getCurrentTimeMilliseconds();

  while (!isLoadedFunc().isLoaded) {
    long now = systemClock.getCurrentTimeMilliseconds();
    long elapsed = now - startTime;

    displayBootStatus(bootTitle, name, false, elapsed);

    if (isLoadedFunc().errorStatus != "") {
      displayError(bootTitle, name, isLoadedFunc().errorStatus);
      return false;
    }

    if (elapsed >= TIMEOUT_MS) {
      displayError(bootTitle, name, "exceeded timeout");
      return false;
    }
  }

  long elapsed = systemClock.getCurrentTimeMilliseconds() - startTime;
  displayBootStatus(bootTitle, name, true, elapsed);
  return true;
}

/**
 * @brief Waits for the MQTT connection to be established.
 *
 * While waiting, this function repeatedly attempts to start MQTT and displays
 * the current boot status until the connection is established or the timeout
 * is exceeded.
 *
 * @param bootTitle Title shown at the top of the boot screen.
 * @param name Name of the component being initialized.
 * @param startFunc Function used to initiate MQTT startup.
 * @param mqttCallback MQTT callback function for incoming messages.
 * @param isLoadedFunc Function returning the current MQTT connection status.
 * @return True if MQTT connected successfully, otherwise false.
 */
bool waitForMQTT(const String &bootTitle, const String &name,
                 void (*startFunc)(void (*)(char *topic, byte *payload,
                                            unsigned int length)),
                 void (*mqttCallback)(char *topic, byte *payload,
                                      unsigned int length),
                 ComponentStatus (*isLoadedFunc)()) {
  long startTime = systemClock.getCurrentTimeMilliseconds();

  while (!isLoadedFunc().isLoaded) {
    long now = systemClock.getCurrentTimeMilliseconds();
    long elapsed = now - startTime;

    displayBootStatus(bootTitle, name, false, elapsed);

    if (elapsed >= TIMEOUT_MS) {
      displayError(bootTitle, name, "exceeded timeout");
      return false;
    }

    startFunc(mqttCallback);
  }

  long elapsed = systemClock.getCurrentTimeMilliseconds() - startTime;
  displayBootStatus(bootTitle, name, true, elapsed);
  return true;
}

/**
 * @brief Performs the full device boot sequence.
 *
 * This function initializes the basic hardware, displays boot progress,
 * connects to Wi-Fi and MQTT, initializes all required hardware components,
 * verifies battery level, and updates the RGB indicators accordingly.
 *
 * @param bootTitle Title shown on the boot screen.
 * @param mqttCallBack Callback function used for incoming MQTT messages.
 */
void bootDevice(const String &bootTitle,
                void (*mqttCallBack)(char *topic, byte *payload,
                                     unsigned int length)) {
  setupBasicHardware();
  setRGB(RGB_1, true, false, false);
  setRGB(RGB_2, true, false, false);
  setRGB(RGB_3, true, false, false);

  displayStatus("meow meow is starting", 1000);

  startWifiConnection();

  wifiConnected = waitForComponent(bootTitle, "WiFi", wifiIsConnected);
  if (!wifiConnected)
    return;
  setRGB(RGB_1, false, true, false);
  touchButtonsAreLoaded =
      waitForComponent(bootTitle, "Touch buttons", touchButtonsLoaded);
  if (!touchButtonsAreLoaded)
    return;
  ledIsLoaded = waitForComponent(bootTitle, "Mode LED", modeBlinkLEDLoaded);
  if (!ledIsLoaded)
    return;
  buzzerIsLoaded = waitForComponent(bootTitle, "Buzzer", buzzerLoaded);
  if (!buzzerIsLoaded)
    return;
  imuIsLoaded = waitForComponent(bootTitle, "IMU", imuLoaded);
  if (!imuIsLoaded)
    return;
  setRGB(RGB_2, false, true, false);
  bmeIsLoaded = waitForComponent(bootTitle, "BME", bmeLoaded);
  if (!bmeIsLoaded)
    return;
  adcIsLoaded = waitForComponent(bootTitle, "ADC", adcLoaded);
  if (!adcIsLoaded)
    return;
  dacIsLoaded = waitForComponent(bootTitle, "DAC", dacLoaded);
  if (!dacIsLoaded)
    return;
  mqttConnected = waitForMQTT(bootTitle, "MQTT", startMQTT, mqttCallBack,
                              mqttConnectedToBroker);
  if (!mqttConnected)
    return;
  batteryCharged = waitForComponent(bootTitle, "Battery", batteryIsAvailable);
  if (!batteryCharged)
    return;
  setRGB(RGB_3, false, true, false);

  displayStatus("meow meow is ready", 1000);
  rgbOff(RGB_1);
  rgbOff(RGB_2);
  rgbOff(RGB_3);
}