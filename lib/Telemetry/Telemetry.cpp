#include <Telemetry.h>

/**
 * @brief Timestamp of the last telemetry publication.
 */
unsigned long lastTelemetryUpdate = 0;

/**
 * @brief Interval between telemetry publications in milliseconds.
 *
 * @note Set to 1 Hz as per requirement R4.1.
 */
const int TELEMETRY_PUBLISH_INTERVAL =
    calculateMillisecondIntervalFromFrequency(1);

void publishData(String topic, String data) {
  PubSubClient &mqttClient = mqttManager.getMqttClient();
  String mqttPublish = mqttManager.getMqttPublish() + "telemetry";

  mqttClient.publish((mqttPublish + topic).c_str(), data.c_str());
}

/**
 * @brief Publishes telemetry data to MQTT topics.
 *
 * This function sends all fields of the TelemetryPayload to their respective
 * MQTT topics using the configured MQTT client.
 *
 * @param payload Telemetry payload containing system state and sensor data.
 */
void mqttPublishTelemetry(TelemetryPayload payload) {
  // Core telemetry for R4.2
  publishData("/current_mode", payload.currentMode);
  publishData("/default_mode", payload.defaultMode);
  publishData("/battery_voltage", payload.batteryVoltage);
  publishData("/usb_voltage", payload.usbVoltage);
  publishData("/angular_rate_x_degrees_sec", payload.angularRateX);
  publishData("/angular_rate_y_degrees_sec", payload.angularRateY);
  publishData("/angular_rate_z_degrees_sec", payload.angularRateZ);
  publishData("/acceleration_x_g", payload.accelerationX);
  publishData("/acceleration_y_g", payload.accelerationY);
  publishData("/acceleration_z_g", payload.accelerationZ);
  publishData("/temperature_c", payload.temperature);
  publishData("/humidity_percent", payload.humidity);
  publishData("/pressure_hpa", payload.pressure);
  publishData("/wifi_rssi_db", payload.wifiRSSI);
  // Additional telemtry
  publishData("/current_plot", payload.currentPlot);
  publishData("/default_plot", payload.defaultPlot);
  publishData("/uptime_ms", payload.uptimeMs);
  publishData("/adc_voltage", payload.adcMeasurement);
  // Output signal received from Mode1 signal generator as per R6.1
  publishData("/dac_output_voltage", payload.dacOutput);
  // Waveform telemetry
  publishData("/amplitude_voltage", payload.amplitude);
  publishData("/frequency_hz", payload.frequency);
  publishData("/waveform", payload.waveform);
  // Helpful stuff
  publishData("/available_commands", payload.availableCommands);
}

/**
 * @brief Updates telemetry publication if the publish interval has elapsed.
 *
 * This function checks whether the telemetry publish interval has passed,
 * and if so, publishes the telemetry payload.
 *
 * @param payload Telemetry payload containing system state and sensor data.
 */
void updateTelemetryPublication(TelemetryPayload payload) {
  if (isDebounced(systemClock.getCurrentTimeMilliseconds(), lastTelemetryUpdate,
                  TELEMETRY_PUBLISH_INTERVAL)) {
    mqttPublishTelemetry(payload);
  }
}