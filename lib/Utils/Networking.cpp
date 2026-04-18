#include <Networking.h>

/**
 * @brief Global MQTT manager instance.
 */
MQTTManager mqttManager;
/**
 * @brief Global Wi-Fi manager instance.
 */
WiFiManager wifiManager;

/**
 * @brief Initializes and connects the MQTT client to the broker.
 *
 * Generates a unique client ID and subscribes to the telecommand topic
 * hierarchy upon successful connection.
 *
 * @return True if the connection was successful, otherwise false.
 */
bool mqttConnect() {
  /**
   * @brief Unique MQTT client identifier.
   */
  String mqttClientId = MQTT_PREFIX + "-" + String(MQTT_BOARD_ID);

  wifiManager.getWifiClient().setCACert(tlsPublicCertificateR13);

  bool connected = mqttManager.getMqttClient().connect(
      mqttClientId.c_str(), MQTT_USER, MQTT_PASSWORD);

  if (connected) {
    mqttManager.getMqttClient().subscribe(
        (mqttManager.getMqttSubscribe() + "telecommand/#").c_str());
  }

  return connected;
}

/**
 * @brief Configures and starts the MQTT client.
 *
 * @param mqttCallbackFunction Callback function for handling incoming MQTT
 * messages.
 */
void startMQTT(void (*mqttCallbackFunction)(char *topic, byte *payload,
                                            unsigned int length)) {
  /**
   * @brief Configure MQTT server and callback handler.
   */
  mqttManager.getMqttClient().setServer(MQTT_BROKER, MQTT_PORT);
  mqttManager.getMqttClient().setCallback(mqttCallbackFunction);

  mqttConnect();
}

/**
 * @brief Checks whether the MQTT client is connected to the broker.
 *
 * @return Status indicating connection state.
 */
ComponentStatus mqttConnectedToBroker() {
  return {"", mqttManager.getMqttClient().connected()};
}

/**
 * @brief Processes incoming MQTT messages.
 */
void checkForMessages() {
  if (!mqttConnectedToBroker().isLoaded)
    return;

  mqttManager.getMqttClient().loop();
}