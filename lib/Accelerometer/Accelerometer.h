/**
 * @file Accelerometer.h
 * @brief Inertial measurement module for acceleration and angular rate data.
 *
 * This module provides access to inertial sensor data, including linear
 * acceleration and angular velocity measurements.
 */

#include <UserInterface.h>
#include <Utils.h>

/**
 * @brief Represents acceleration data in units of G.
 */
struct AccelerationData {
  /**
   * @brief Acceleration along the X-axis in G.
   */
  float x;

  /**
   * @brief Acceleration along the Y-axis in G.
   */
  float y;

  /**
   * @brief Acceleration along the Z-axis in G.
   */
  float z;
};

/**
 * @brief Represents angular velocity data in degrees per second.
 */
struct GyroscopeData {
  /**
   * @brief Angular velocity around the X-axis in degrees per second.
   */
  float x;

  /**
   * @brief Angular velocity around the Y-axis in degrees per second.
   */
  float y;

  /**
   * @brief Angular velocity around the Z-axis in degrees per second.
   */
  float z;
};

/**
 * @brief Plot object for displaying accelerometer data.
 */
extern PlotData accelerometerPlot;

/**
 * @brief Updates the acceleration history buffer with the latest accelerometer
 * data.
 */
void updateAccelerationHistory();

/**
 * @brief Gets acceleration data in G from the accelerometer.
 *
 * @return Latest acceleration values in G.
 */
AccelerationData getAccelerationData();

/**
 * @brief Gets angular rate data in degrees per second from the gyroscope.
 *
 * @return Latest angular rate values in degrees per second.
 */
GyroscopeData getGyroscopeData();