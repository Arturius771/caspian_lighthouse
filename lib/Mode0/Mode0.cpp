#include <Mode0.h>

/**
 * @brief Mode 0: Idle state waiting for external commands.
 */
void mode0() {
  clearScreen();
  drawText("Waiting for commands", {1, 10});
  updateScreen();
}