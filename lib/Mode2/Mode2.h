/**
 * @file Mode2.h
 * @brief Plot viewing and system monitoring mode.
 *
 * This module implements the device's monitoring mode, allowing the user to
 * view and interact with different data plots, including ADC input,
 * accelerometer, and environmental sensor data.
 */

#include <ADC.h>
#include <Accelerometer.h>
#include <EnvironmentalSensor.h>

/**
 * @brief Mode 2: Plot viewing and monitoring mode.
 *
 * This function updates and displays the currently selected plot and performs
 * any mode-specific monitoring behaviour.
 */
void mode2();

/**
 * @brief Sets the next plot type to be displayed.
 *
 * @param newPlotType Plot type as a string.
 */
void setPlotType(String newPlotType);

/**
 * @brief Sets the default plot type.
 *
 * @param newPlotType Plot type as a string.
 */
void setDefaultPlotType(String newPlotType);

/**
 * @brief Persists the default plot type to storage.
 *
 * @param writeFile Function used to write a string to a file.
 */
void persistDefaultPlotType(void (*writeFile)(const char *filePath,
                                              const String &content));

/**
 * @brief Returns the current plot type as a string.
 *
 * @return Current plot type.
 */
String getCurrentPlotType();

/**
 * @brief Returns the default plot type as a string.
 *
 * @return Default plot type.
 */
String getDefaultPlotType();

/**
 * @brief Increments the currently selected plot control parameter.
 */
void incrementCurrentPlotControl();
/**
 * @brief Decrements the currently selected plot control parameter.
 */
void decrementCurrentPlotControl();
/**
 * @brief Cycles through the selected plot control parameter.
 */
void incrementPlotControl();

/**
 * @brief Increases the Y-axis range of the currently displayed plot.
 */
void incrementYAxis();

/**
 * @brief Decreases the Y-axis range of the currently displayed plot.
 */
void decrementYAxis();

/**
 * @brief Updates the MQTT history buffer with a new data value.
 *
 * @param dataValue New MQTT data value to store.
 */
void updateMQTTHistory(float dataValue);

/**
 * @brief Converts a plot type string to the corresponding plot object.
 *
 * @param plotString Plot type as a string.
 * @return Pointer to the corresponding plot object.
 */
PlotData *convertStringToPlot(String plotString);