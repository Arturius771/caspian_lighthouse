/**
 * @file BootSequence.h
 * @brief Device bootstrapping and system readiness management.
 *
 * This module is responsible for determining whether the device has completed
 * its boot sequence successfully and is ready for normal operation.
 */

#include <Mode2.h>
#include <UserInterface.h>

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
                                     unsigned int length));

/**
 * @brief Determines whether the full device boot sequence completed
 * successfully.
 *
 * @return True if all required components are loaded and battery is available,
 * otherwise false.
 */
bool deviceHasBootedSuccessfully();