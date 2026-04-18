/**
 * @file Utils.h
 * @brief Utility and system helper functions for timing, input handling, and
 * general operations.
 *
 * This module provides common utility functionality used across the system,
 * including timing utilities, debouncing logic, file helpers, and general
 * data processing.
 */

#pragma once
#include <Arduino.h>
#include <Networking.h>

/**
 * @brief System clock class providing current time in milliseconds.
 */
class SystemClock {
public:
  /**
   * @brief Returns the current system time in milliseconds.
   *
   * @return Current time in milliseconds.
   */
  unsigned long getCurrentTimeMilliseconds() { return millis(); }
};

/**
 * @brief Global system clock instance.
 */
extern SystemClock systemClock;
/**
 * @brief Set by the touch interrupt when the up button is pressed.
 */
extern volatile bool upTouchRequested;
/**
 * @brief Set by the touch interrupt when the down button is pressed.
 */
extern volatile bool downTouchRequested;
/**
 * @brief Set by the touch interrupt when the X button is pressed.
 */
extern volatile bool xTouchRequested;
/**
 * @brief Interval for flashing UI text in milliseconds.
 */
const int FLASHING_TEXT_INTERVAL_MS = 500;
/**
 * @brief Debounce interval for control switching in milliseconds.
 */
const int CONTROL_SWITCH_DEBOUNCE_MS = 1000;
/**
 * @brief Calculates the time interval in milliseconds for a given frequency.
 *
 * @param frequencyHz Frequency in hertz (must be greater than 0).
 * @return Time interval in milliseconds.
 */
constexpr float calculateMillisecondIntervalFromFrequency(float frequencyHz) {
  return frequencyHz <= 0.0f ? 0.0f : (1000.0f / frequencyHz);
}
/**
 * @brief Calculates frequency in hertz from a time interval in milliseconds.
 *
 * @param millisecond Time interval in milliseconds.
 * @return Frequency in hertz.
 */
constexpr float calculateFrequencyFromMillisecond(float millisecond) {
  return millisecond <= 0.0f ? 0.0f : (1000.0f / millisecond);
}
/**
 * @brief Sensor update interval in milliseconds.
 *
 * Set to approximately 20 Hz as per requirement R1.1.
 */
constexpr int SENSOR_UPDATE_INTERVAL =
    calculateMillisecondIntervalFromFrequency(20);
/**
 * @brief Determines whether sensors should be updated based on debounce timing.
 *
 * @return True if sensors may be updated, otherwise false.
 */
bool mayUpdateSensor();

/**
 * @brief Writes content to a file on the SD card.
 *
 * If the file already exists, it is removed before writing.
 *
 * @param filePath Path to the file.
 * @param content Content to write.
 */
void writeFile(const char *filePath, const String &content);

/**
 * @brief Reads the contents of a file from the SD card.
 *
 * @param path Path to the file.
 * @return File contents as a string.
 */
String readFile(const char *path);

/**
 * @brief Clamps a value to be within the range [0, maxValue - 1].
 *
 * @param value The value to clamp.
 * @param maxValue The maximum allowable value.
 * @return Clamped value.
 */
int clampValue(int value, int maxValue);

/**
 * @brief Sets an RGB LED to red.
 *
 * @param targetLED Target RGB LED.
 */
void setLEDToRed(RGBLed targetLED);

/**
 * @brief Sets an RGB LED to an orange colour.
 *
 * This is achieved by toggling red and green channels to approximate
 * orange.
 *
 * @param targetLED Target RGB LED.
 */
void setLEDToOrange(RGBLed targetLED);

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
                 long debounceInterval);

/**
 * @brief Interrupt service routine for the up touch button.
 *
 * Sets the flag indicating that an up touch input has been requested.
 */
void IRAM_ATTR onTouchUp();
/**
 * @brief Interrupt service routine for the down touch button.
 *
 * Sets the flag indicating that a down touch input has been requested.
 */
void IRAM_ATTR onTouchDown();
/**
 * @brief Interrupt service routine for the X touch button.
 *
 * Sets the flag indicating that an X touch input has been requested.
 */
void IRAM_ATTR onTouchX();

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
                       void (*downTouchMode2Function)(void));