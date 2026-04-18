#include <EnvironmentalSensor.h>

/**
 * @brief Buffer holding environmental pressure history for plotting.
 */
PlotHistoryDataPoint environmentalHistory[PLOT_BUFFER_SIZE];
/**
 * @brief Mapping configuration for the environmental pressure plot.
 */
PlotDataMappingParameters environmentalPlotMapping =
    createPlotMappings(850, 1050, (SCREEN_HEIGHT / 4) * 3, SCREEN_HEIGHT / 4,
                       "hPa", 50, 1000, 1200, false, false);
/**
 * @brief Plot object for environmental pressure visualization.
 */
PlotData pressurePlot = createPlot(PRESSURE, environmentalPlotMapping,
                                   DEFAULT_TIMESCALE, environmentalHistory);

/**
 * @brief Updates the pressure history buffer with the latest pressure sensor
 * data.
 *
 * This function calculates the current buffer position from the system time and
 * sensor update interval, then stores the latest pressure reading.
 */
void updatePressureSensor() {
  /**
   * @brief Current system time in milliseconds.
   */
  long currentTime = systemClock.getCurrentTimeMilliseconds();

  // Calculate the buffer index based on the current time.
  int bufferIndex = (currentTime / SENSOR_UPDATE_INTERVAL) % PLOT_BUFFER_SIZE;

  // Store the data point in the history buffer.
  environmentalHistory[bufferIndex] = {currentTime,
                                       getEnvironmentalData().pressureHpa};
}

/**
 * @brief Retrieves the latest environmental sensor readings.
 *
 * This function reads pressure, temperature, and humidity values from the
 * environmental sensor and returns them as an EnvironmentalData struct.
 *
 * @return EnvironmentalData Latest environmental sensor measurements.
 */
EnvironmentalData getEnvironmentalData() {
  /**
   * @brief Pressure reading in hectopascals.
   */
  float pressureHpa = bme.readPressure() / 100;

  /**
   * @brief Temperature reading in degrees Celsius.
   */
  float temperatureC = bme.readTemperature();

  /**
   * @brief Relative humidity reading in percent.
   */
  float humidityPercent = bme.readHumidity();

  return {pressureHpa, temperatureC, humidityPercent};
}