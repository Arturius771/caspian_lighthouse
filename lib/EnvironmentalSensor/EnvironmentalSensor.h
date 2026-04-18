/**
 * @file EnvironmentalSensor.h
 * @brief Environmental sensor module for data acquisition and visualization.
 */

#include <UserInterface.h>
#include <Utils.h>

/**
 * @brief Represents environmental sensor data.
 */
struct EnvironmentalData {
  /**
   * @brief Atmospheric pressure in hectopascals (hPa).
   */
  float pressureHpa;

  /**
   * @brief Temperature in degrees Celsius.
   */
  float tempratureC;

  /**
   * @brief Relative humidity in percent.
   */
  float humidityPercent;
};

/**
 * @brief Plot object for environmental pressure visualization.
 */
extern PlotData pressurePlot;

/**
 * @brief Updates the pressure history buffer with the latest pressure sensor
 * data.
 *
 * This function calculates the current buffer position from the system time and
 * sensor update interval, then stores the latest pressure reading.
 */
void updatePressureSensor();

/**
 * @brief Retrieves the latest environmental sensor readings.
 *
 * This function reads pressure, temperature, and humidity values from the
 * environmental sensor and returns them as an EnvironmentalData struct.
 *
 * @return EnvironmentalData Latest environmental sensor measurements.
 */
EnvironmentalData getEnvironmentalData();