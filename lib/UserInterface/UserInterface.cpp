#include <UserInterface.h>

/**
 * @brief Pixel colour used for drawing on the display.
 *
 * The display is monochrome, so all drawn elements use the same colour.
 */
const int PIXEL_COLOR = SSD1306_WHITE;
/**
 * @brief Text size used for rendered text.
 *
 * The default font size corresponds to a 6x8 pixel character cell.
 */
const int TEXT_SIZE = 1;
/**
 * @brief Height of one text character in pixels.
 *
 * This value matches the configured text size.
 */
const int FONT_HEIGHT = 8;
/**
 * @brief Width of one text character in pixels.
 *
 * This value matches the configured text size.
 */
const int FONT_WIDTH = 6;
/**
 * @brief Margin used for plot labels and spacing.
 */
const int PLOT_MARGIN = 2;
/**
 * @brief Width of each dash segment in pixels.
 */
const int DASH_WIDTH = 2;
/**
 * @brief Interval between dashes when drawing dashed lines.
 */
const int DASH_INTERVAL = 5;
/**
 * @brief Time range threshold above which plots are drawn as pixels only.
 *
 * When the displayed time range is large, drawing connecting lines may make
 * the plot too dense, so individual pixels are drawn instead.
 */
const int PLOT_TIMERANGE_LINE_THRESHOLD = 10000;

/**
 * @brief Converts an X screen position to a time value on a plot timescale.
 *
 * @param pixelX X coordinate on the screen.
 * @param timecale Time scale mapping parameters.
 * @return Time value in milliseconds corresponding to the X position.
 */
int calculateTimeAtPositionOnTimescale(int pixelX,
                                       PloTimescaleMappingParameters timecale) {
  return map(pixelX, timecale.plotStartPoint.x, timecale.plotEndPoint.x,
             timecale.timerangeStart, timecale.timerangeEnd);
}

/**
 * @brief Formats a float value to a string with fixed decimal places.
 *
 * @param value The float value to format.
 * @param decimalPlaces The number of decimal places to include in the formatted
 * string.
 * @return representation of the float value with the specified number of
 * decimal places.
 */
String formatValueToDecimalPlaces(float value, int decimalPlaces) {
  return String(value, decimalPlaces);
}

/**
 * @brief Converts a time value in milliseconds to a string in seconds.
 *
 * @param timeValueMs Time value in milliseconds.
 * @return Time string formatted in seconds.
 */
String timeInSeconds(int timeValueMs) {
  return formatValueToDecimalPlaces(timeValueMs / 1000, 0) + "s";
}

/**
 * @brief Sets the displayed time range of a plot.
 *
 * @param plot Plot whose time range should be updated.
 * @param newTimeMs New time range start value in milliseconds.
 */
void setTimerange(PlotData &plot, int newTimeMs) {
  // Early return if new value is out of range.
  if (newTimeMs < 1 || newTimeMs > PLOT_BUFFER_SIZE * SENSOR_UPDATE_INTERVAL)
    return;

  plot.timescale.timerangeStart = newTimeMs;
}

/**
 * @brief Maps a data value to a Y coordinate on the screen.
 *
 * @param dataValue The data value to map.
 * @param mappingParameters Plot mapping parameters.
 * @return Mapped Y coordinate on the screen.
 */
int mapDataToPlot(float dataValue,
                  PlotDataMappingParameters mappingParameters) {
  /**
   * @brief Scale factor used to improve precision during integer mapping.
   */
  float scaleFactor = 100.0f;
  /**
   * @brief Scaled data value.
   */
  long scaledValue = (long)(dataValue * scaleFactor);
  /**
   * @brief Scaled lower plot bound.
   */
  long scaledMin = (long)(mappingParameters.lowerDivisionValue * scaleFactor);
  /**
   * @brief Scaled upper plot bound.
   */
  long scaledMax = (long)(mappingParameters.upperDivisionValue * scaleFactor);

  return clampToScreenHeight(map(scaledValue, scaledMin, scaledMax,
                                 mappingParameters.lowerDivisionValueY,
                                 mappingParameters.upperDivisionValueY));
}

/**
 * @brief Maps a time value in milliseconds to an X coordinate on the screen.
 *
 * @param timeMs Time in milliseconds.
 * @param timecale Time scale mapping parameters.
 * @return Mapped X coordinate on the screen.
 */
int mapTimeToPlot(long timeMs, PloTimescaleMappingParameters timecale) {
  return clampToScreenWidth(
      map(timeMs, timecale.timerangeStart, timecale.timerangeEnd,
          timecale.plotStartPoint.x, timecale.plotEndPoint.x));
}

/**
 * @brief Draws a single pixel on the screen.
 *
 * @param pixel Pixel position to draw.
 */
void drawPixel(Pixel pixel) { screen.drawPixel(pixel.x, pixel.y, PIXEL_COLOR); }

/**
 * @brief Draws text on the screen at the specified position.
 *
 * @param text The text to draw.
 * @param pixel Position at which the text should be drawn.
 */
void drawText(const String &text, Pixel pixel) {
  screen.setTextSize(TEXT_SIZE);
  screen.setTextColor(PIXEL_COLOR);
  screen.setCursor(pixel.x, pixel.y);
  screen.println(text);
}

/**
 * @brief Draws a line on the screen between two points.
 *
 * @note Credit:
 * https://learn.adafruit.com/adafruit-gfx-graphics-library/graphics-primitives
 *
 * @param start Start point of the line.
 * @param end End point of the line.
 */
void drawLine(Pixel start, Pixel end) {
  screen.drawLine(start.x, start.y, end.x, end.y, PIXEL_COLOR);
}

/**
 * @brief Draws a short horizontal dash on the screen.
 *
 * @note Credit:
 * https://learn.adafruit.com/adafruit-gfx-graphics-library/graphics-primitives
 *
 * @param startPixel Starting pixel of the dash.
 */
void drawHorizontalDash(Pixel startPixel) {
  screen.drawFastHLine(startPixel.x, startPixel.y, DASH_WIDTH, PIXEL_COLOR);
}

/**
 * @brief Draws a short vertical dash on the screen.
 *
 * @note Credit:
 * https://learn.adafruit.com/adafruit-gfx-graphics-library/graphics-primitives
 *
 * @param startPixel Starting pixel of the dash.
 */
void drawVerticalDash(Pixel startPixel) {
  screen.drawFastVLine(startPixel.x, startPixel.y, DASH_WIDTH, PIXEL_COLOR);
}

/**
 * @brief Draws a dashed horizontal line between two points.
 *
 * @param startPixel Start point of the dashed line.
 * @param endPixel End point of the dashed line.
 */
void drawDashedHorizontalLine(Pixel startPixel, Pixel endPixel) {
  for (int i = startPixel.x; i < endPixel.x; i += DASH_INTERVAL) {
    drawHorizontalDash({i, startPixel.y});
  }
}

/**
 * @brief Draws a dashed vertical line between two points.
 *
 * @param startPixel Start point of the dashed line.
 * @param endPixel End point of the dashed line.
 */
void drawDashedVerticalLine(Pixel startPixel, Pixel endPixel) {
  for (int i = startPixel.y; i < endPixel.y; i += DASH_INTERVAL) {
    drawVerticalDash({startPixel.x, i});
  }
}

/**
 * @brief Draws the main Y-axis guide lines for a plot.
 *
 * @param mappingParameters Plot mapping parameters containing Y-axis bounds.
 */
void drawYAxisLines(PlotDataMappingParameters mappingParameters) {
  /**
   * @brief Start point of the upper horizontal grid line.
   */
  Pixel firstLineStart = {PLOT_START_PIXEL.x,
                          mappingParameters.upperDivisionValueY};
  /**
   * @brief End point of the upper horizontal grid line.
   */
  Pixel firstLineEnd = {SCREEN_WIDTH, mappingParameters.upperDivisionValueY};
  /**
   * @brief Start point of the lower horizontal grid line.
   */
  Pixel secondLineStart = {PLOT_START_PIXEL.x,
                           mappingParameters.lowerDivisionValueY};
  /**
   * @brief End point of the lower horizontal grid line.
   */
  Pixel secondLineEnd = {SCREEN_WIDTH, mappingParameters.lowerDivisionValueY};

  drawDashedHorizontalLine(firstLineStart, firstLineEnd);
  drawDashedHorizontalLine(secondLineStart, secondLineEnd);
}

/**
 * @brief Draws Y-axis grid lines and optional labels for a plot.
 *
 * @param mappingParameters Plot mapping parameters containing axis values and
 * positions.
 * @param labelGrid True to draw labels, false to draw only the grid lines.
 *
 * @note Labels and lines implemented to meet R6.2
 */
void drawYAxisGrid(PlotDataMappingParameters mappingParameters,
                   bool labelGrid) {
  drawYAxisLines(mappingParameters);

  if (!labelGrid)
    return;
  /**
   * @brief Y coordinate for the upper grid line label.
   */
  int firstLineLabelY = mappingParameters.upperDivisionValueY - FONT_HEIGHT / 2;
  /**
   * @brief Y coordinate for the lower grid line label.
   */
  int secondLineLabelY =
      mappingParameters.lowerDivisionValueY - FONT_HEIGHT / 2;

  drawText(formatValueToDecimalPlaces(mappingParameters.upperDivisionValue, 0) +
               mappingParameters.unitType,
           {PLOT_MARGIN, firstLineLabelY});
  drawText(formatValueToDecimalPlaces(mappingParameters.lowerDivisionValue, 0) +
               mappingParameters.unitType,
           {PLOT_MARGIN, secondLineLabelY});
}

/**
 * @brief Draws the main X-axis guide lines for a plot timescale.
 *
 * @param timescale Time scale mapping parameters containing grid line
 * positions.
 */
void drawXAxisLines(PloTimescaleMappingParameters timescale) {
  /**
   * @brief First vertical grid line start point.
   */
  Pixel firstGridLine = timescale.firstGridLineStart;
  /**
   * @brief Second vertical grid line start point.
   */
  Pixel secondGridLine = timescale.secondGridLineStart;
  /**
   * @brief End point for the first vertical grid line.
   */
  Pixel firstLineEnd = {firstGridLine.x, SCREEN_HEIGHT};
  /**
   * @brief End point for the second vertical grid line.
   */
  Pixel secondLineEnd = {secondGridLine.x, SCREEN_HEIGHT};

  drawDashedVerticalLine(firstGridLine, firstLineEnd);
  drawDashedVerticalLine(secondGridLine, secondLineEnd);
}

/**
 * @brief Draws X-axis grid lines and optional labels for a plot timescale.
 *
 * @param timescale Time scale mapping parameters.
 * @param labelGrid True to draw labels, false to draw only the grid lines.
 */
void drawXAxisGridOnTimescale(PloTimescaleMappingParameters timescale,
                              bool labelGrid) {
  drawXAxisLines(timescale);

  if (!labelGrid)
    return;
  /**
   * @brief X coordinate for the first time label.
   */
  const int firstLineLabelX = timescale.firstGridLineStart.x - FONT_WIDTH / 2;
  /**
   * @brief X coordinate for the second time label.
   */
  const int secondLineLabelx = timescale.secondGridLineStart.x - FONT_WIDTH / 2;

  drawText(timeInSeconds(calculateTimeAtPositionOnTimescale(
               timescale.firstGridLineStart.x, timescale)),
           {firstLineLabelX, PLOT_MARGIN});
  drawText(timeInSeconds(calculateTimeAtPositionOnTimescale(
               timescale.secondGridLineStart.x, timescale)),
           {secondLineLabelx, PLOT_MARGIN});
}

/**
 * @brief Draws a rolling history plot on the screen.
 *
 * The most recent data appears on the right. Depending on the plot settings,
 * samples are rendered either as pixels or connected by lines.
 *
 * @note Pixels are drawn unless the timerange is less than 10 seconds.
 *
 * @param plot Plot containing history data, mapping parameters, and timescale.
 * @param overlayFunction Function used to draw plot overlay elements.
 * @param drawPixelOnly Always draw pixels regardless of timerange.
 */
void drawRollingHistory(PlotData plot, void (*overlayFunction)(void),
                        bool drawPixelOnly) {
  /**
   * @brief Current system time in milliseconds.
   */
  long currentTime = systemClock.getCurrentTimeMilliseconds();

  overlayFunction();

  // Loop over the entire history buffer.
  for (int index = 0; index < PLOT_BUFFER_SIZE; index++) {
    /**
     * @brief Age of the current data point in milliseconds.
     */
    long age1 = currentTime - plot.dataHistory[index].dataRecordedTime;

    if (age1 > plot.timescale.timerangeStart)
      continue;

    /**
     * @brief Screen position of the current data point.
     */
    Pixel pixel1 = {mapTimeToPlot(age1, plot.timescale),
                    mapDataToPlot(plot.dataHistory[index].dataValue,
                                  plot.mappingParameters)};

    if (pixel1.x < plot.timescale.plotStartPoint.x)
      continue;

    if (plot.timescale.timerangeStart > PLOT_TIMERANGE_LINE_THRESHOLD ||
        drawPixelOnly) {
      drawPixel(pixel1);
      continue;
    } else {
      /**
       * @brief Next buffer index, wrapping around the circular buffer.
       */
      int nextIndex = (index + 1) % PLOT_BUFFER_SIZE;

      /**
       * @brief Age of the next data point in milliseconds.
       */
      long age2 = currentTime - plot.dataHistory[nextIndex].dataRecordedTime;

      if (age2 > plot.timescale.timerangeStart)
        continue;

      /**
       * @brief Screen position of the next data point.
       */
      Pixel pixel2 = {mapTimeToPlot(age2, plot.timescale),
                      mapDataToPlot(plot.dataHistory[nextIndex].dataValue,
                                    plot.mappingParameters)};

      if (pixel2.x < plot.timescale.plotStartPoint.x)
        continue;
      if (pixel2.x <= pixel1.x)
        continue;

      drawLine(pixel1, pixel2);
    }
  }
}

/**
 * @brief Draws a waveform plot across the screen for the specified frequency.
 *
 * This function samples waveform values from the plot history buffer according
 * to the displayed time range and draws the resulting waveform.
 *
 * @param plot Plot containing waveform sample data.
 * @param overlayFunction Function used to draw plot overlay elements.
 * @param frequencyHz Waveform frequency in hertz.
 */
void drawWaveformPlot(PlotData plot, void (*overlayFunction)(void),
                      float frequencyHz) {
  overlayFunction();

  if (frequencyHz <= 0.0f)
    return;
  /**
   * @brief Duration of one waveform cycle in milliseconds.
   */
  float cycleDurationMs =
      calculateMillisecondIntervalFromFrequency(frequencyHz);

  if (cycleDurationMs <= 0.0f)
    return;
  /**
   * @brief Time represented by one horizontal screen step.
   */
  float timeStep =
      (plot.timescale.timerangeEnd - plot.timescale.timerangeStart) /
      (float)(plot.timescale.plotEndPoint.x - plot.timescale.plotStartPoint.x);
  /**
   * @brief Current position on the plot timescale.
   */
  float currentPositionOnTimescale = calculateTimeAtPositionOnTimescale(
      plot.timescale.plotStartPoint.x, plot.timescale);

  for (int screenX = plot.timescale.plotStartPoint.x;
       screenX < plot.timescale.plotEndPoint.x; screenX++) {

    /**
     * @brief Time value at the next horizontal screen position.
     */
    float nextTime = currentPositionOnTimescale + timeStep;

    /**
     * @brief Position within the current waveform cycle for the first point.
     */
    float timeInCycle1WithRemainder =
        fmod(currentPositionOnTimescale, cycleDurationMs);

    /**
     * @brief Position within the current waveform cycle for the second point.
     */
    float timeInCycle2WithRemainder = fmod(nextTime, cycleDurationMs);

    /**
     * @brief Buffer index for the first waveform sample.
     */
    int index1 = clampValue(
        (int)((timeInCycle1WithRemainder / cycleDurationMs) * PLOT_BUFFER_SIZE),
        PLOT_BUFFER_SIZE - 1);

    /**
     * @brief Buffer index for the second waveform sample.
     */
    int index2 = clampValue(
        (int)((timeInCycle2WithRemainder / cycleDurationMs) * PLOT_BUFFER_SIZE),
        PLOT_BUFFER_SIZE - 1);

    /**
     * @brief First plotted waveform point.
     */
    Pixel pixel1 = {screenX, mapDataToPlot(plot.dataHistory[index1].dataValue,
                                           plot.mappingParameters)};

    /**
     * @brief Second plotted waveform point.
     */
    Pixel pixel2 = {screenX + 1,
                    mapDataToPlot(plot.dataHistory[index2].dataValue,
                                  plot.mappingParameters)};

    drawLine(pixel1, pixel2);

    currentPositionOnTimescale = nextTime;
  }
}

/**
 * @brief Creates a PlotData object from the provided plot configuration.
 *
 * @param plotType Plot type identifier.
 * @param mapping Plot mapping parameters.
 * @param timescale Plot timescale parameters.
 * @param history Pointer to the history buffer.
 * @return Constructed plot data object.
 */
PlotData createPlot(PlotType plotType, PlotDataMappingParameters mapping,
                    PloTimescaleMappingParameters timescale,
                    PlotHistoryDataPoint history[]) {
  return {plotType, mapping, timescale, history};
}

/**
 * @brief Creates plot mapping parameters for a plot.
 *
 * @param lowerDivisionValue Lower displayed value.
 * @param upperDivisionValue Upper displayed value.
 * @param lowerDivisionValueY Screen Y coordinate of the lower displayed value.
 * @param upperDivisionValueY Screen Y coordinate of the upper displayed value.
 * @param unitType Unit label for the Y axis.
 * @param incrementAmount Increment used when scaling the axis.
 * @param lowestDivision Lowest allowed axis value.
 * @param highestDivision Highest allowed axis value.
 * @param freezeLower True to prevent changing the lower bound.
 * @param freezeUpper True to prevent changing the upper bound.
 * @return Constructed plot mapping parameters.
 */
PlotDataMappingParameters
createPlotMappings(float lowerDivisionValue, float upperDivisionValue,
                   int lowerDivisionValueY, int upperDivisionValueY,
                   String unitType, int incrementAmount, int lowestDivision,
                   int highestDivision, bool freezeLower, bool freezeUpper) {
  return {lowerDivisionValue,
          upperDivisionValue,
          lowerDivisionValueY,
          upperDivisionValueY,
          unitType,
          {incrementAmount, lowestDivision, highestDivision, freezeLower,
           freezeUpper}};
}

/**
 * @brief Creates timescale mapping parameters for a plot.
 *
 * @param timerangeStart Start of the displayed time range.
 * @param timerangeEnd End of the displayed time range.
 * @param plotStartPoint Start point of the plot area.
 * @param plotEndPoint End point of the plot area.
 * @param firstGridLineStart Start point of the first vertical grid line.
 * @param secondGridLineStart Start point of the second vertical grid line.
 * @return PloTimescaleMappingParameters Constructed timescale mapping
 * parameters.
 */
PloTimescaleMappingParameters
createTimescale(int timerangeStart, int timerangeEnd, Pixel plotStartPoint,
                Pixel plotEndPoint, Pixel firstGridLineStart,
                Pixel secondGridLineStart) {
  return {timerangeStart, timerangeEnd,       plotStartPoint,
          plotEndPoint,   firstGridLineStart, secondGridLineStart};
}