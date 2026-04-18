/**
 * @file Telecommand.h
 * @brief Telecommand handling module for MQTT-based device control.
 *
 * The module acts as the interface between external control inputs and internal
 * system functions, enabling configuration changes, mode switching, and other
 * runtime commands.
 */

#include <BootSequence.h>
#include <Mode1.h>
#include <Utils.h>

/**
 * @brief MQTT callback function for handling incoming messages.
 *
 * This function converts the raw payload into a string and forwards it
 * to the telecommand processing function.
 *
 * @param topic MQTT topic.
 * @param payload Raw message payload.
 * @param length Length of the payload in bytes.
 */
void mqttCallback(char *topic, byte *payload, unsigned int length);

/**
 * @brief Returns a list of available MQTT telecommands.
 *
 * @return List of supported command topics.
 */
String getAvailableCommands();