#include <Utils.h>

/**
 * @brief Timestamp of the last sensor update.
 */
unsigned long lastSensorUpdate = 0;
/**
 * @brief Set by the touch interrupt when the up button is pressed.
 */
volatile bool upTouchRequested = false;
/**
 * @brief Set by the touch interrupt when the down button is pressed.
 */
volatile bool downTouchRequested = false;
/**
 * @brief Set by the touch interrupt when the X button is pressed.
 */
volatile bool xTouchRequested = false;
/**
 * @brief Timestamp of last control switch for debouncing.
 */
unsigned long lastControlSwitchTime = 0;

/**
 * @brief Interrupt service routine for the up touch button.
 *
 * Sets the flag indicating that an up touch input has been requested.
 */
void IRAM_ATTR onTouchUp() { upTouchRequested = true; }

/**
 * @brief Interrupt service routine for the down touch button.
 *
 * Sets the flag indicating that a down touch input has been requested.
 */
void IRAM_ATTR onTouchDown() { downTouchRequested = true; }

/**
 * @brief Interrupt service routine for the X touch button.
 *
 * Sets the flag indicating that an X touch input has been requested.
 */
void IRAM_ATTR onTouchX() { xTouchRequested = true; }

/**
 * @brief Determines whether sensors should be updated based on debounce timing.
 *
 * @return True if sensors may be updated, otherwise false.
 */
bool mayUpdateSensor() {
  return isDebounced(systemClock.getCurrentTimeMilliseconds(), lastSensorUpdate,
                     SENSOR_UPDATE_INTERVAL);
}

/**
 * @brief Writes content to a file on the SD card.
 *
 * If the file already exists, it is removed before writing.
 *
 * @param filePath Path to the file.
 * @param content Content to write.
 */
void writeFile(const char *filePath, const String &content) {
  SD_MMC.remove(filePath);
  File file = SD_MMC.open(filePath, FILE_WRITE);

  if (!file) {
    Serial.println("Failed to open file for writing");
    return;
  }

  file.print(content);
  file.close();
}

/**
 * @brief Reads the contents of a file from the SD card.
 *
 * @param path Path to the file.
 * @return File contents as a string.
 */
String readFile(const char *path) {
  File file = SD_MMC.open(path);

  if (!file) {
    Serial.println("Failed to open file");
    return "";
  }

  /**
   * @brief Buffer holding file content.
   */
  String content = "";

  while (file.available()) {
    content += (char)file.read();
  }

  file.close();
  return content;
}

/**
 * @brief Clamps a value to be within the range [0, maxValue - 1].
 *
 * @param value The value to clamp.
 * @param maxValue The maximum allowable value.
 * @return Clamped value.
 */
int clampValue(int value, int maxValue) {
  if (value < 0)
    return 0;
  if (value >= maxValue)
    return maxValue - 1;
  return value;
}

/**
 * @brief Sets an RGB LED to an orange colour.
 *
 * This is achieved by toggling red and green channels to approximate
 * orange.
 *
 * @param targetLED Target RGB LED.
 */
void setLEDToOrange(RGBLed targetLED) { setRGB(targetLED, true, true, false); }

/**
 * @brief Sets an RGB LED to red.
 *
 * @param targetLED Target RGB LED.
 */
void setLEDToRed(RGBLed targetLED) { setRGB(targetLED, true, false, false); }

/**
 * @brief Debounce helper function for timing-based events.
 *
 * Ensures that an event only triggers if a specified interval has elapsed
 * since the last recorded event.
 *
 * @param currentTime Current system time in milliseconds.
 * @param lastEventTime Reference to the last event timestamp.
 * @param debounceInterval Minimum interval between events in milliseconds.
 * @return True if the event is allowed, otherwise false.
 */
bool isDebounced(unsigned long currentTime, unsigned long &lastEventTime,
                 long debounceInterval) {
  bool canUpdate = currentTime - lastEventTime >= debounceInterval;

  if (canUpdate) {
    lastEventTime = currentTime;
  }

  return canUpdate;
}

/**
 * @brief Handles touch input events and dispatches actions based on the current
 * mode.
 *
 * Processes touch request flags set by interrupt service routines and invokes
 * the corresponding handler functions depending on the active mode.
 *
 * In mode 1:
 *  - X touch triggers waveform control selection
 *  - Up/Down touch adjust the selected waveform parameter
 *
 * In mode 2:
 *  - Up/Down touch cycle through available plot types
 *
 * @param xTouchFunction Function to execute on X touch in mode 1
 * @param uptTouchFunction Function to execute on Up touch in mode 1
 * @param downTouchFunction Function to execute on Down touch in mode 1
 * @param upTouchMode2Function Function to execute on Up touch in mode 2
 * @param downTouchMode2Function Function to execute on Down touch in mode 2
 */
void handleTouchInputs(void (*xTouchFunction)(void),
                       void (*upTouchFunction)(void),
                       void (*downTouchFunction)(void),
                       void (*xTouchMode2Function)(void),
                       void (*upTouchMode2Function)(void),
                       void (*downTouchMode2Function)(void)) {

  long currentTime = systemClock.getCurrentTimeMilliseconds();

  if (!isDebounced(systemClock.getCurrentTimeMilliseconds(),
                   lastControlSwitchTime, CONTROL_SWITCH_DEBOUNCE_MS))
    return;

  lastControlSwitchTime = currentTime;

  if (xTouchRequested && currentMode == 1) {
    xTouchRequested = false;
    xTouchFunction();
  }

  if (upTouchRequested && currentMode == 1) {
    upTouchRequested = false;
    upTouchFunction();
  }

  if (downTouchRequested && currentMode == 1) {
    downTouchRequested = false;
    downTouchFunction();
  }

  if (xTouchRequested && currentMode == 2) {
    xTouchRequested = false;
    xTouchMode2Function();
  }

  if (upTouchRequested && currentMode == 2) {
    upTouchRequested = false;
    upTouchMode2Function();
  }

  if (downTouchRequested && currentMode == 2) {
    downTouchRequested = false;
    downTouchMode2Function();
  }
}