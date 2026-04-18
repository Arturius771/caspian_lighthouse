#include <Telecommand.h>

/**
 * @brief Determines whether an MQTT topic matches a telecommand subtopic.
 *
 * @param topic Full incoming MQTT topic.
 * @param subtopic Telecommand subtopic suffix, e.g. "/current_mode".
 * @return True if the topic matches the telecommand subtopic.
 */
bool isTelecommandTopic(const String &topic, const String &subtopic) {
  String mqttSubscribe = mqttManager.getMqttSubscribe() + "telecommand";
  return topic == (mqttSubscribe + subtopic);
}

/**
 * @brief Processes an incoming MQTT telecommand message.
 *
 * @param topic MQTT topic string.
 * @param message MQTT message payload as a string.
 */
void processIncomingMessage(String topic, String message) {

  // Set the current mode as per R5.1
  if (isTelecommandTopic(topic, "/current_mode")) {
    setCurrentMode(message.toInt());
  }
  // Set the default mode as per R5.2
  else if (isTelecommandTopic(topic, "/default_mode")) {
    setDefaultMode(message.toInt(), writeFile);
  }
  // Set the source signal to plot as per R6.2
  else if (isTelecommandTopic(topic, "/current_plot")) {
    setPlotType(message);
  } else if (isTelecommandTopic(topic, "/default_plot")) {
    setDefaultPlotType(message);
    persistDefaultPlotType(writeFile);
  }
  // Adjust X axis as per R6.2
  else if (isTelecommandTopic(topic, "/plot_timerange_s")) {
    setTimerange(*convertStringToPlot(getCurrentPlotType()),
                 message.toInt() * 1000);
  } else if (isTelecommandTopic(topic, "/increment_y_axis")) {
    incrementYAxis();
  } else if (isTelecommandTopic(topic, "/decrement_y_axis")) {
    decrementYAxis();
  }
  // Allow plotting MQTT data as per R6.2
  else if (isTelecommandTopic(topic, "/mqtt_source")) {
    updateMQTTHistory(message.toFloat());
  } else if (isTelecommandTopic(topic, "/amplitude_voltage")) {
    setAmplitude(message.toFloat());
  } else if (isTelecommandTopic(topic, "/frequency_hz")) {
    setFrequency(message.toFloat());
  } else if (isTelecommandTopic(topic, "/waveform")) {
    setWaveformFromString(message);
  }
}

/**
 * @brief Returns a list of available MQTT telecommands.
 *
 * @return List of supported command topics.
 */
String getAvailableCommands() {
  return "/default_mode /current_mode /current_plot /default_plot "
         "/plot_timerange_s /increment_y_axis /decrement_y_axis /mqtt_source "
         "/amplitude_voltage /frequency_hz /waveform";
}

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
void mqttCallback(char *topic, byte *payload, unsigned int length) {
  /**
   * @brief Parsed message payload.
   */
  String message;

  for (unsigned int i = 0; i < length; i++) {
    message += (char)payload[i];
  }

  processIncomingMessage(String(topic), message);
}