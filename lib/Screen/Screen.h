/**
 * @file Screen.h
 * @brief Display abstraction module for screen rendering and updates.
 *
 * Additionally, it manages screen update timing to control refresh rate and
 * prevent unnecessary redraws, supporting efficient and stable rendering of
 * visual content.
 */

#pragma once
#include <Utils.h>

/**
 * @brief Represents a 2D pixel coordinate on the screen.
 */
struct Pixel {
  /**
   * @brief Pixel x coordinate.
   */
  int x;

  /**
   * @brief Pixel y coordinate
   */
  int y;
};

/**
 * @brief Clears the screen buffer.
 *
 * This function clears all content from the display buffer.
 */
void clearScreen();

/**
 * @brief Displays the current screen buffer.
 *
 * This function pushes the current buffer contents to the display.
 */
void updateScreen();

/**
 * @brief Clamps a Y coordinate to be within the screen height.
 *
 * @param yValue The value to clamp.
 * @return Clamped Y coordinate.
 */
int clampToScreenHeight(int yValue);

/**
 * @brief Clamps an X coordinate to be within the screen width.
 *
 * @param xValue The value to clamp.
 * @return Clamped X coordinate.
 */
int clampToScreenWidth(int xValue);

/**
 * @brief Determines whether the screen may be refreshed.
 *
 * This function checks whether the required update interval has elapsed
 * since the last screen update.
 *
 * @param currentTime Current system time in milliseconds.
 * @return True if the screen can be updated, otherwise false.
 */
bool screenMayRefresh(long currentTime);