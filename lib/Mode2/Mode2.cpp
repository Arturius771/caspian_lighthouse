/**
 * @brief Implements Mode 2 functionality for plot viewing and plot switching.
 */
#include <Mode2.h>

/**
 * @brief Pointer to the plot currently displayed on screen.
 */
PlotData *currentPlot = &accelerometerPlot;
/**
 * @brief Pointer to the default plot selected at startup.
 */
PlotData *defaultPlot = &accelerometerPlot;
/**
 * @brief Pointer to the next plot to be displayed.
 *
 * This is used to defer plot switching until the mode update runs.
 */
PlotData *nextPlot = &accelerometerPlot;
/**
 * @brief Mapping configuration for MQTT plot data.
 */
PlotDataMappingParameters mqttSourcePlotMapping =
    createPlotMappings(-1.0f, 1.0f, (SCREEN_HEIGHT / 3) * 2, SCREEN_HEIGHT / 3,
                       "", 1, 1, 50, false, false);
/**
 * @brief Buffer holding MQTT plot history data.
 */
PlotHistoryDataPoint mqttSourceHistory[PLOT_BUFFER_SIZE];
/**
 * @brief Plot object used for displaying MQTT data.
 */
PlotData mqttPlot = createPlot(MQTT, mqttSourcePlotMapping, DEFAULT_TIMESCALE,
                               mqttSourceHistory);
/**
 * @brief Currently selected control parameter.
 *
 * 0 = plot, 1 = x, 2 = y
 */
int plotSelectedControl = 0;
/**
 * @brief Tracks which index to insert into `mqttSourceHistory`.
 */
int mqttHistoryWriteIndex = 0;

/**
 * @brief Cycles through the selected plot control parameter.
 */
void incrementPlotControl() {
  plotSelectedControl = (plotSelectedControl + 1) % 3;
}

/**
 * @brief Switches to the next plot type in forward order.
 *
 * Plot switching is subject to debounce timing.
 */
void incrementPlotType() {
  switch (currentPlot->plotType) {
  case ACCELEROMETER:
    nextPlot = &mqttPlot;
    break;
  case PRESSURE:
    nextPlot = &accelerometerPlot;
    break;
  case ADC:
    nextPlot = &pressurePlot;
    break;
  case MQTT:
    nextPlot = &adcPlot;
    break;
  }
}

/**
 * @brief Switches to the previous plot type in reverse order.
 *
 * Plot switching is subject to debounce timing.
 */
void decrementPlotType() {
  switch (currentPlot->plotType) {
  case ACCELEROMETER:
    nextPlot = &pressurePlot;
    break;
  case PRESSURE:
    nextPlot = &adcPlot;
    break;
  case ADC:
    nextPlot = &mqttPlot;
    break;
  case MQTT:
    nextPlot = &accelerometerPlot;
    break;
  }
}

/**
 * @brief Increments the currently selected plot control parameter.
 */
void incrementCurrentPlotControl() {
  if (plotSelectedControl == 0) {
    // Set the source signal to plot as per R6.2
    incrementPlotType();
  } else if (plotSelectedControl == 1) {
    setTimerange(*currentPlot, currentPlot->timescale.timerangeStart += 1000);
  } else {
    incrementYAxis();
  }
}

/**
 * @brief Decrements the currently selected plot control parameter.
 */
void decrementCurrentPlotControl() {
  if (plotSelectedControl == 0) {
    // Set the source signal to plot as per R6.2
    decrementPlotType();
  } else if (plotSelectedControl == 1) {
    setTimerange(*currentPlot, currentPlot->timescale.timerangeStart -= 1000);
  } else {
    decrementYAxis();
  }
}

/**
 * @brief Updates the MQTT history buffer with a new data value.
 *
 * @param dataValue New MQTT data value to store.
 */
void updateMQTTHistory(float dataValue) {
  long currentTime = systemClock.getCurrentTimeMilliseconds();

  mqttSourceHistory[mqttHistoryWriteIndex] = {currentTime, dataValue};
  mqttHistoryWriteIndex = (mqttHistoryWriteIndex + 1) % PLOT_BUFFER_SIZE;
}

/**
 * @brief Converts a plot type string to the corresponding plot object.
 *
 * @param plotString Plot type as a string.
 * @return Pointer to the corresponding plot object.
 */
PlotData *convertStringToPlot(String plotString) {
  if (plotString == "pressure") {
    return &pressurePlot;
  } else if (plotString == "adc") {
    return &adcPlot;
  } else if (plotString == "mqtt") {
    return &mqttPlot;
  }

  return &accelerometerPlot;
}

/**
 * @brief Sets the next plot type to be displayed.
 *
 * @param newPlotType Plot type as a string.
 */
void setPlotType(String newPlotType) {
  nextPlot = convertStringToPlot(newPlotType);
}

/**
 * @brief Sets the default plot type.
 *
 * @param newPlotType Plot type as a string.
 */
void setDefaultPlotType(String newPlotType) {
  defaultPlot = convertStringToPlot(newPlotType);
}

/**
 * @brief Persists the default plot type to storage.
 *
 * @param writeFile Function used to write a string to a file.
 */
void persistDefaultPlotType(void (*writeFile)(const char *filePath,
                                              const String &content)) {
  writeFile(DEFAULT_PLOT_PATH, getDefaultPlotType());
}

/**
 * @brief Increases the Y-axis scale range of a plot.
 *
 * @param plot Plot whose scale should be increased.
 */
void increaseScale(PlotData &plot) {
  int increment = plot.mappingParameters.yIncrement.incrementAmount;
  int highest = plot.mappingParameters.yIncrement.highestDivision;

  bool canIncreaseUpper = !plot.mappingParameters.yIncrement.freezeUpper &&
                          plot.mappingParameters.upperDivisionValue < highest;

  if (canIncreaseUpper) {
    plot.mappingParameters.upperDivisionValue += increment;
    if (plot.mappingParameters.upperDivisionValue > highest) {
      plot.mappingParameters.upperDivisionValue = highest;
    }
  }

  if (!plot.mappingParameters.yIncrement.freezeLower && canIncreaseUpper) {
    plot.mappingParameters.lowerDivisionValue -= increment;
  }
}

/**
 * @brief Decreases the Y-axis scale range of a plot.
 *
 * @param plot Plot whose scale should be decreased.
 */
void decreaseScale(PlotData &plot) {
  int increment = plot.mappingParameters.yIncrement.incrementAmount;
  int lowest = plot.mappingParameters.yIncrement.lowestDivision;

  bool canDecreaseUpper = !plot.mappingParameters.yIncrement.freezeUpper &&
                          plot.mappingParameters.upperDivisionValue > lowest;

  if (canDecreaseUpper) {
    plot.mappingParameters.upperDivisionValue -= increment;
    if (plot.mappingParameters.upperDivisionValue < lowest) {
      plot.mappingParameters.upperDivisionValue = lowest;
    }
  }

  if (!plot.mappingParameters.yIncrement.freezeLower && canDecreaseUpper) {
    plot.mappingParameters.lowerDivisionValue += increment;
  }
}

/**
 * @brief Increases the Y-axis range of the currently displayed plot.
 *
 * @note Implemented to meet R6.2
 */
void incrementYAxis() { increaseScale(*currentPlot); }

/**
 * @brief Decreases the Y-axis range of the currently displayed plot.
 *
 * @note Implemented to meet R6.2
 */
void decrementYAxis() { decreaseScale(*currentPlot); }

/**
 * @brief Converts a plot object to its string representation.
 *
 * @param plot Plot object to convert.
 * @return Plot type as a string.
 */
String convertPlotTypeToString(PlotData plot) {
  if (plot.plotType == PRESSURE) {
    return "pressure";
  } else if (plot.plotType == ADC) {
    return "adc";
  } else if (plot.plotType == MQTT) {
    return "mqtt";
  }
  return "accelerometer";
}

/**
 * @brief Returns the current plot type as a string.
 *
 * @return Current plot type.
 */
String getCurrentPlotType() { return convertPlotTypeToString(*currentPlot); }

/**
 * @brief Returns the default plot type as a string.
 *
 * @return Default plot type.
 */
String getDefaultPlotType() { return convertPlotTypeToString(*defaultPlot); }

/**
 * @brief Draws the overlay for the currently selected plot.
 *
 * This includes the Y-axis grid and the X-axis timescale grid.
 */
void drawCurrentPlotOverlay() {
  drawYAxisGrid(currentPlot->mappingParameters, true);
  drawXAxisGridOnTimescale(currentPlot->timescale, true);

  // Flash current control text
  bool flashOn =
      ((systemClock.getCurrentTimeMilliseconds() / FLASHING_TEXT_INTERVAL_MS) %
           2 ==
       0);

  if (flashOn) {
    if (plotSelectedControl == 0) {
      drawText("Plot", {0, 0});
    } else if (plotSelectedControl == 1) {
      drawText("X", {0, 0});
    } else if (plotSelectedControl == 2) {
      drawText("Y", {0, 0});
    }
  }
}

/**
 * @brief Draws the history of the currently selected plot.
 *
 * The plot is only redrawn when the screen refresh interval allows it.
 */
void drawCurrentPlotHistory() {
  if (screenMayRefresh(systemClock.getCurrentTimeMilliseconds())) {
    clearScreen();
    drawRollingHistory(*currentPlot, drawCurrentPlotOverlay,
                       currentPlot->plotType == MQTT);
    updateScreen();
  }
}

/**
 * @brief Updates RGB LED indicators based on ADC input voltage thresholds.
 *
 * When the current plot type is ADC, this function evaluates the measured
 * input voltage and sets all RGB LEDs to indicate voltage level:
 *
 *  - Orange: voltage > 14.9 V (warning threshold)
 *  - Red:    voltage > 19.9 V (critical threshold)
 *
 * If the plot type is not ADC, or the voltage is below the warning threshold,
 * all LEDs are turned off.
 *
 * @note voltage levels as per R6.2
 */
void handleLEDSForVoltageInput() {
  if (currentPlot->plotType == ADC && getADCInputVoltage() > 14.9) {
    if (getADCInputVoltage() > 19.9) {
      setLEDToRed(RGB_1);
      setLEDToRed(RGB_2);
      setLEDToRed(RGB_3);
    } else {
      setLEDToOrange(RGB_1);
      setLEDToOrange(RGB_2);
      setLEDToOrange(RGB_3);
    }
  } else {
    rgbOff(RGB_1);
    rgbOff(RGB_2);
    rgbOff(RGB_3);
  }
}

/**
 * @brief Mode 2: Plot viewing and monitoring mode.
 *
 * This mode updates the currently displayed plot, draws its history, and
 * changes RGB LED status based on ADC input voltage thresholds.
 */
void mode2() {
  if (nextPlot->plotType != currentPlot->plotType) {
    currentPlot = nextPlot;
  }

  drawCurrentPlotHistory();
  handleLEDSForVoltageInput();
}