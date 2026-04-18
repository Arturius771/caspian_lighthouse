#include <ADC.h>

/**
 * @brief Buffer holding ADC voltage history for plotting.
 */
PlotHistoryDataPoint adcHistory[PLOT_BUFFER_SIZE];
/**
 * @brief Mapping configuration for the ADC plot.
 */
PlotDataMappingParameters adcPlotDataMappings =
    createPlotMappings(0.0f, 3.3f, (SCREEN_HEIGHT / 5) * 4, SCREEN_HEIGHT / 3,
                       "V", 1, 1, 30, true, false);
/**
 * @brief Plot object for ADC voltage visualization.
 */
PlotData adcPlot =
    createPlot(ADC, adcPlotDataMappings, DEFAULT_TIMESCALE, adcHistory);

/**
 * @brief Reads the raw ADC voltage from the ADC hardware.
 *
 * @return Raw ADC voltage in volts.
 */
float getADCRawVoltage() {
  return adc.computeVolts(adc.readADC_SingleEnded(0));
}

/**
 * @brief Raw ADC voltage measured when DAC is at minimum output.
 *
 * Used as a calibration reference point for scaling ADC readings.
 */
constexpr float ADC_RAW_AT_DAC_MIN = 1.445f;

/**
 * @brief Raw ADC voltage measured when DAC is at maximum output.
 *
 * Used as a calibration reference point for scaling ADC readings.
 */
constexpr float ADC_RAW_AT_DAC_MAX = 1.58f;

/**
 * @brief Converts raw ADC voltage to calibrated input voltage.
 *
 * This function normalizes the raw ADC reading using calibration values
 * and scales it to the expected input voltage range (0–3.3V) of the DAC.
 * Negative results are clamped to zero. No upper limit is enforced, allowing
 * for readings above 3.3V.
 *
 * @return Calibrated input voltage in volts.
 */
float getADCInputVoltage() {
  /**
   * @brief Raw ADC voltage reading.
   */
  float vRaw = getADCRawVoltage();

  /**
   * @brief Normalized value based on calibration points.
   */
  float normalized =
      (vRaw - ADC_RAW_AT_DAC_MIN) / (ADC_RAW_AT_DAC_MAX - ADC_RAW_AT_DAC_MIN);

  /**
   * @brief Scaled input voltage.
   */
  float vIn = normalized * 3.3f;

  if (vIn < 0.0f)
    vIn = 0.0f;

  return vIn;
}

/**
 * @brief Updates the ADC history buffer with the latest measurement.
 *
 * The function calculates the buffer index based on the current system time
 * and sensor update interval, then stores the latest ADC input voltage.
 */
void updateADCSensor() {
  /**
   * @brief Current system time in milliseconds.
   */
  long currentTime = systemClock.getCurrentTimeMilliseconds();

  /**
   * @brief Index in the circular history buffer.
   */
  int bufferIndex = (currentTime / SENSOR_UPDATE_INTERVAL) % PLOT_BUFFER_SIZE;

  adcHistory[bufferIndex] = {currentTime, getADCInputVoltage()};
}