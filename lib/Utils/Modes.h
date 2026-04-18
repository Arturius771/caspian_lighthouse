/**
 * @file Modes.h
 * @brief Operating mode management.
 *
 * This module manages the device's operating modes, including the current,
 * requested, and default mode states.
 */

#include <Hardware.h>

/**
 * @brief Current operating mode.
 */
extern int currentMode;
/**
 * @brief Next requested operating mode.
 */
extern volatile int nextMode;
/**
 * @brief Default operating mode loaded from persistent storage.
 */
extern int defaultMode;

/**
 * @brief Increases the requested mode number.
 */
void increaseModeNumber();

/**
 * @brief Decreases the requested mode number.
 */
void decreaseModeNumber();

/**
 * @brief Sets the default operating mode and persists it to storage.
 *
 * The mode is only updated if it is within the allowed mode range.
 *
 * @param mode Mode value to store as the default.
 * @param writeFile Function used to write the default mode to persistent
 * storage.
 */
void setDefaultMode(int mode, void (*writeFile)(const char *filePath,
                                                const String &content));

/**
 * @brief Sets the next requested operating mode.
 *
 * The requested mode is only updated if it is within the allowed mode range.
 *
 * @param mode Mode value to request.
 */
void setCurrentMode(int mode);

/**
 * @brief Manages mode transitions and executes the active mode.
 *
 * @param mode0 Function implementing mode 0.
 * @param mode1 Function implementing mode 1.
 * @param mode2 Function implementing mode 2.
 * @param mode3 Function implementing mode 3.
 */
void manageModes(void (*mode0)(), void (*mode1)(), void (*mode2)(),
                 void (*mode3)());

/**
 * @brief Initializes the current and next mode to the given mode.
 *
 * The mode is only applied if it is within the allowed mode range.
 *
 * @param mode Mode value to initialize.
 */
void initialiseMode(int mode);