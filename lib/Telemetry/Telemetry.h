/**
 * @file Telemetry.h
 * @brief Telemetry publishing module for system state and sensor data.
 *
 * It provides functionality to periodically publish telemetry data over MQTT.
 */

#include <Telecommand.h>

/**
 * @brief Structure representing telemetry data payload.
 *
 * This structure contains all system state and sensor values that are
 * published via MQTT telemetry.
 */
struct TelemetryPayload {
  /**
   * @brief Current operating mode.
   */
  String currentMode;
  /**
   * @brief Default operating mode.
   */
  String defaultMode;
  /**
   * @brief Currently selected plot type.
   */
  String currentPlot;
  /**
   * @brief Default plot type.
   */
  String defaultPlot;
  /**
   * @brief Measured battery voltage.
   */
  String batteryVoltage;
  /**
   * @brief Measured USB input voltage.
   */
  String usbVoltage;
  /**
   * @brief Angular rate around X-axis (degrees per second).
   */
  String angularRateX;
  /**
   * @brief Angular rate around Y-axis (degrees per second).
   */
  String angularRateY;
  /**
   * @brief Angular rate around Z-axis (degrees per second).
   */
  String angularRateZ;
  /**
   * @brief Acceleration along X-axis (G).
   */
  String accelerationX;
  /**
   * @brief Acceleration along Y-axis (G).
   */
  String accelerationY;
  /**
   * @brief Acceleration along Z-axis (G).
   */
  String accelerationZ;
  /**
   * @brief Temperature in degrees Celsius.
   */
  String temperature;
  /**
   * @brief Relative humidity in percent.
   */
  String humidity;
  /**
   * @brief Atmospheric pressure in hectopascals.
   */
  String pressure;
  /**
   * @brief Wi-Fi signal strength (RSSI).
   */
  String wifiRSSI;
  /**
   * @brief System uptime in milliseconds.
   */
  String uptimeMs;
  /**
   * @brief Measured ADC input voltage.
   */
  String adcMeasurement;
  /**
   * @brief Current DAC output voltage.
   */
  String dacOutput;
  /**
   * @brief Current waveform type.
   */
  String waveform;
  /**
   * @brief Current waveform frequency in hertz.
   */
  String frequency;
  /**
   * @brief Current waveform amplitude in volts.
   */
  String amplitude;
  /**
   * @brief List of available telecommands.
   */
  String availableCommands;
};

/**
 * @brief Updates telemetry publication if the publish interval has elapsed.
 *
 * This function checks whether the telemetry publish interval has passed,
 * and if so, publishes the telemetry payload.
 *
 * @param payload Telemetry payload containing system state and sensor data.
 */
void updateTelemetryPublication(TelemetryPayload payload);