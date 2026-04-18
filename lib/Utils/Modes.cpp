#include <Modes.h>

/**
 * @brief Current operating mode.
 */
int currentMode = 0;
/**
 * @brief Next requested operating mode.
 */
volatile int nextMode = 0;
/**
 * @brief Default operating mode loaded from persistent storage.
 */
int defaultMode;
/**
 * @brief Minimum allowed mode value.
 */
int minModeLimit = 0;
/**
 * @brief Maximum allowed mode value.
 */
int maxModeLimit = 3;

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
                                                const String &content)) {
  if (mode >= minModeLimit && mode <= maxModeLimit) {
    defaultMode = mode;
    writeFile(DEFAULT_MODE_PATH, String(defaultMode));
  }
}

/**
 * @brief Sets the next requested operating mode.
 *
 * The requested mode is only updated if it is within the allowed mode range.
 *
 * @param mode Mode value to request.
 */
void setCurrentMode(int mode) {
  if (mode >= minModeLimit && mode <= maxModeLimit) {
    nextMode = mode;
  }
}

/**
 * @brief Initializes the current and next mode to the given mode.
 *
 * The mode is only applied if it is within the allowed mode range.
 *
 * @param mode Mode value to initialize.
 */
void initialiseMode(int mode) {
  if (mode >= minModeLimit && mode <= maxModeLimit) {
    currentMode = mode;
    nextMode = mode;
  }
}

/**
 * @brief Increases the requested mode number.
 */
void increaseModeNumber() {
  touchRead(TOUCH_RIGHT_PIN);
  setCurrentMode(currentMode + 1);
}

/**
 * @brief Decreases the requested mode number.
 */
void decreaseModeNumber() {
  touchRead(TOUCH_LEFT_PIN);
  setCurrentMode(currentMode - 1);
}

/**
 * @brief Manages mode transitions and runs the active mode handler.
 *
 * If a new mode has been requested, the device signals the change and updates
 * the current mode before calling the corresponding mode function.
 *
 * @param mode0 Function implementing mode 0.
 * @param mode1 Function implementing mode 1.
 * @param mode2 Function implementing mode 2.
 * @param mode3 Function implementing mode 3.
 */
void manageModes(void (*mode0)(), void (*mode1)(), void (*mode2)(),
                 void (*mode3)()) {
  if (nextMode != currentMode) {
    // Beep/buzz as per R3.1
    beepAndBlink(nextMode, 100);
    currentMode = nextMode;
  }

  switch (currentMode) {
  case 0:
    mode0();
    break;
  case 1:
    mode1();
    break;
  case 2:
    mode2();
    break;
  case 3:
    mode3();
    break;
  }
}