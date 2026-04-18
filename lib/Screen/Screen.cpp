#include <Screen.h>

/**
 * @brief Update interval for screen refreshes in milliseconds.
 *
 * Defines how often the screen is allowed to refresh. Currently set to
 * approximately 20 Hz.
 */
const int SCREEN_UPDATE_INTERVAL = calculateMillisecondIntervalFromFrequency(
    20.0f); // 20 Hz update rate (R1.1)
/**
 * @brief Timestamp of the last screen update.
 */
unsigned long lastScreenUpdateTime = 0;

/**
 * @brief Clears the screen buffer.
 *
 * This function clears all content from the display buffer.
 */
void clearScreen() { screen.clearDisplay(); }

/**
 * @brief Displays the current screen buffer.
 *
 * This function pushes the current buffer contents to the display.
 */
void updateScreen() { screen.display(); }

/**
 * @brief Clamps a Y coordinate to be within the screen height.
 *
 * @param yValue The value to clamp.
 * @return Clamped Y coordinate.
 */
int clampToScreenHeight(int yValue) {
  return clampValue(yValue, SCREEN_HEIGHT);
}

/**
 * @brief Clamps an X coordinate to be within the screen width.
 *
 * @param xValue The value to clamp.
 * @return Clamped X coordinate.
 */
int clampToScreenWidth(int xValue) { return clampValue(xValue, SCREEN_WIDTH); }

/**
 * @brief Determines whether the screen may be refreshed.
 *
 * This function checks whether the required update interval has elapsed
 * since the last screen update.
 *
 * @param currentTime Current system time in milliseconds.
 * @return True if the screen can be updated, otherwise false.
 */
bool screenMayRefresh(long currentTime) {
  return isDebounced(currentTime, lastScreenUpdateTime, SCREEN_UPDATE_INTERVAL);
}