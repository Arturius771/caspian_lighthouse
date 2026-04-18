/**
 * @file ADC.h
 * @brief Analog-to-digital conversion module for input voltage measurement.
 */

#include <UserInterface.h>
#include <Utils.h>

/**
 * @brief External reference to the ADC plot.
 */
extern PlotData adcPlot;

/**
 * @brief Updates the ADC history buffer with the latest measurement.
 *
 * The function calculates the buffer index based on the current system time
 * and sensor update interval, then stores the latest ADC input voltage.
 */
void updateADCSensor();

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
float getADCInputVoltage();