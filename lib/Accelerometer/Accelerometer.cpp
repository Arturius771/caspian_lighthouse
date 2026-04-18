#include <Accelerometer.h>

/**
 * @brief Buffer holding the acceleration history for plotting.
 */
PlotHistoryDataPoint accelerationXHistory[PLOT_BUFFER_SIZE];
/**
 * @brief Mapping configuration for the accelerometer plot.
 */
PlotDataMappingParameters accelerometerPlotMapping =
    createPlotMappings(-1.0f, 1.0f, (SCREEN_HEIGHT / 3) * 2, SCREEN_HEIGHT / 3,
                       "G", 1, 1, 10, false, false);
/**
 * @brief Plot object for displaying accelerometer data.
 */
PlotData accelerometerPlot =
    createPlot(ACCELEROMETER, accelerometerPlotMapping, DEFAULT_TIMESCALE,
               accelerationXHistory);

/**
 * @brief Struct to hold the latest accelerometer, gyroscope and temperature
 * data from the IMU.
 */
struct AccelerometerData {
  /**
   * @brief Latest accelerometer sensor event.
   */
  sensors_event_t acccelerometer;

  /**
   * @brief Latest gyroscope sensor event.
   */
  sensors_event_t gyroscope;

  /**
   * @brief Latest temperature sensor event.
   */
  sensors_event_t temperature;
};

/**
 * @brief Converts raw acceleration values from metres per second squared to G.
 *
 * @param accelerationData Raw acceleration value in m/s^2.
 * @return Acceleration value converted to G.
 */
float convertToG(float accelerationData) {
  /**
   * @brief Gravitational acceleration value in m/s^2.
   */
  const float G_ACCELERATION = 9.81f;
  return accelerationData / G_ACCELERATION;
}

/**
 * @brief Updates the acceleration history buffer with the latest accelerometer
 * data.
 */
void updateAccelerationHistory() {
  /**
   * @brief Current system time in milliseconds.
   */
  long currentTime = systemClock.getCurrentTimeMilliseconds();

  // Calculate the buffer index based on the current time.
  int bufferIndex = (currentTime / SENSOR_UPDATE_INTERVAL) % PLOT_BUFFER_SIZE;

  // Store the data point in the history buffer.
  accelerationXHistory[bufferIndex] = {currentTime, getAccelerationData().x};
}

/**
 * @brief Converts an angular value from radians to degrees.
 *
 * @param radians Angle in radians.
 * @return Angle converted to degrees.
 */
float convertRadToDegrees(float radians) {
  return radians * (180.0 / 3.14159265);
}

/**
 * @brief Gets the latest accelerometer, gyroscope and temperature data from the
 * IMU.
 *
 * @return AccelerometerData containing the latest IMU sensor events.
 */
AccelerometerData getAccelerometerData() {
  /**
   * @brief Temporary storage for the latest accelerometer event.
   */
  sensors_event_t acccelerometer;

  /**
   * @brief Temporary storage for the latest gyroscope event.
   */
  sensors_event_t gyroscope;

  /**
   * @brief Temporary storage for the latest temperature event.
   */
  sensors_event_t temperature;

  imu.getEvent(&acccelerometer, &gyroscope, &temperature);

  return {acccelerometer, gyroscope, temperature};
}

/**
 * @brief Gets acceleration data in G from the accelerometer.
 *
 * @return Latest acceleration values in G.
 */
AccelerationData getAccelerationData() {
  sensors_event_t acccelerometer = getAccelerometerData().acccelerometer;

  // Negate the X axis as it seems to be mounted upside down
  return {-convertToG(acccelerometer.acceleration.x),
          convertToG(acccelerometer.acceleration.y),
          convertToG(acccelerometer.acceleration.z)};
}

/**
 * @brief Gets angular rate data in degrees per second from the gyroscope.
 *
 * @return Latest angular rate values in degrees per second.
 */
GyroscopeData getGyroscopeData() {
  sensors_event_t gyroscope = getAccelerometerData().gyroscope;

  return {convertRadToDegrees(gyroscope.gyro.x),
          convertRadToDegrees(gyroscope.gyro.y),
          convertRadToDegrees(gyroscope.gyro.z)};
}