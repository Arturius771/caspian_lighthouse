/**
 * @file UserInterface.h
 * @brief User interface and plotting module for data visualization.
 *
 * It includes support for:
 *  - Plot data representation and history buffering
 *  - Mapping data and time values to screen coordinates
 *  - Drawing plots, waveforms, grid lines, and text
 *  - Configuring and scaling plot axes
 */

#pragma once
#include <Screen.h>

/**
 * @brief Structure representing a single data point in a plot history buffer.
 */
struct PlotHistoryDataPoint {
  /**
   * @brief Time at which the data point was recorded.
   */
  long dataRecordedTime;

  /**
   * @brief Recorded data value.
   */
  float dataValue;
};
/**
 * @brief Configuration for changing the Y-axis scale of a plot.
 */
struct PlotYAxisIncrement {
  /**
   * @brief Amount by which the Y-axis limits change per increment step.
   */
  int incrementAmount;

  /**
   * @brief Lowest allowed division value for the Y-axis.
   */
  int lowestDivision;

  /**
   * @brief Highest allowed division value for the Y-axis.
   */
  int highestDivision;

  /**
   * @brief Indicates whether the lower Y-axis bound is fixed.
   */
  bool freezeLower;

  /**
   * @brief Indicates whether the upper Y-axis bound is fixed.
   */
  bool freezeUpper;
};
/**
 * @brief Mapping parameters for converting plot data values to screen
 * coordinates.
 */
struct PlotDataMappingParameters {
  /**
   * @brief Minimum data value for mapping.
   */
  float lowerDivisionValue;

  /**
   * @brief Maximum data value for mapping.
   */
  float upperDivisionValue;

  /**
   * @brief Y coordinate for the lower division value on the plot.
   */
  int lowerDivisionValueY;

  /**
   * @brief Y coordinate for the upper division value on the plot.
   */
  int upperDivisionValueY;

  /**
   * @brief Unit label displayed for the plot.
   */
  String unitType;

  /**
   * @brief Configuration for adjusting the Y-axis scale.
   */
  PlotYAxisIncrement yIncrement;
};

/**
 * @brief Mapping parameters for converting time values to screen coordinates.
 */
struct PloTimescaleMappingParameters {
  /**
   * @brief Start of the displayed time range in milliseconds.
   */
  int timerangeStart;

  /**
   * @brief End of the displayed time range in milliseconds.
   */
  int timerangeEnd;

  /**
   * @brief Start point of the plot area on screen.
   */
  Pixel plotStartPoint;

  /**
   * @brief End point of the plot area on screen.
   */
  Pixel plotEndPoint;

  /**
   * @brief Start position of the first vertical grid line.
   */
  Pixel firstGridLineStart;

  /**
   * @brief Start position of the second vertical grid line.
   */
  Pixel secondGridLineStart;
};
/**
 * @brief Supported plot types on the screen.
 *
 * @note Plot types meet R6.2
 */
enum PlotType {
  /**
   * @brief Accelerometer plot.
   */
  ACCELEROMETER = 0,

  /**
   * @brief Pressure plot.
   */
  PRESSURE = 1,

  /**
   * @brief ADC plot.
   */
  ADC = 2,

  /**
   * @brief MQTT plot.
   */
  MQTT = 3,

  /**
   * @brief DAC plot.
   */
  DAC = 4
};

/**
 * @brief Default top-left start pixel for plot drawing.
 */
constexpr Pixel PLOT_START_PIXEL = {SCREEN_WIDTH / 5, SCREEN_HEIGHT / 5};

/**
 * @brief Default plot width in pixels.
 */
constexpr int PLOT_WIDTH = SCREEN_WIDTH - PLOT_START_PIXEL.x;

/**
 * @brief Structure containing all data required to draw a plot.
 */
struct PlotData {
  /**
   * @brief Type of plot.
   */
  PlotType plotType;

  /**
   * @brief Parameters for mapping data values to screen coordinates.
   */
  PlotDataMappingParameters mappingParameters;

  /**
   * @brief Parameters for mapping time values to screen coordinates.
   */
  PloTimescaleMappingParameters timescale;

  /**
   * @brief Pointer to the plot history buffer.
   */
  PlotHistoryDataPoint *dataHistory;
};

/**
 * @brief Default start position of the first vertical grid line.
 */
constexpr Pixel DEFAULT_FIRST_GRIDLINE = {PLOT_START_PIXEL.x + (PLOT_WIDTH / 4),
                                          PLOT_START_PIXEL.y};

/**
 * @brief Default start position of the second vertical grid line.
 */
constexpr Pixel DEFAULT_SECOND_GRIDLINE = {DEFAULT_FIRST_GRIDLINE.x * 2,
                                           PLOT_START_PIXEL.y};

/**
 * @brief Default timescale used for plots.
 */
constexpr PloTimescaleMappingParameters DEFAULT_TIMESCALE = {
    5000,
    0,
    PLOT_START_PIXEL,
    {SCREEN_WIDTH - 1, PLOT_START_PIXEL.y},
    DEFAULT_FIRST_GRIDLINE,
    DEFAULT_SECOND_GRIDLINE};

/**
 * @brief Number of samples stored in each plot history buffer.
 *
 * Setting this too high results in data being rendered between pixels.
 */
constexpr int PLOT_BUFFER_SIZE = 1000;

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
                        bool drawPixelOnly);

/**
 * @brief Draws text on the screen at the specified position.
 *
 * @param text The text to draw.
 * @param pixel Position at which the text should be drawn.
 */
void drawText(const String &text, Pixel pixel);

/**
 * @brief Draws a line on the screen between two points.
 *
 * Credit:
 * https://learn.adafruit.com/adafruit-gfx-graphics-library/graphics-primitives
 *
 * @param start Start point of the line.
 * @param end End point of the line.
 */
void drawLine(Pixel start, Pixel end);

/**
 * @brief Formats a float value to a string with fixed decimal places.
 *
 * @param value The float value to format.
 * @param decimalPlaces The number of decimal places to include in the formatted
 * string.
 * @return representation of the float value with the specified number of
 * decimal places.
 */
String formatValueToDecimalPlaces(float value, int decimalPlaces);

/**
 * @brief Sets the displayed time range of a plot.
 *
 * @param plot Plot whose time range should be updated.
 * @param newTimeMs New time range start value in milliseconds.
 */
void setTimerange(PlotData &plot, int newTimeMs);

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
                    PlotHistoryDataPoint history[]);

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
                   int highestDivision, bool freezeLower, bool freezeUpper);

/**
 * @brief Draws a waveform plot across the screen for the specified frequency.
 *
 * @param plot Plot containing waveform sample data.
 * @param overlayFunction Function used to draw plot overlay elements.
 * @param frequencyHz Waveform frequency in hertz.
 */
void drawWaveformPlot(PlotData plot, void (*overlayFunction)(void),
                      float frequencyHz);

/**
 * @brief Draws Y-axis grid lines and optional labels for a plot.
 *
 * @param mappingParameters Plot mapping parameters containing axis values and
 * positions.
 * @param labelGrid True to draw labels, false to draw only the grid lines.
 */
void drawYAxisGrid(PlotDataMappingParameters mappingParameters, bool labelGrid);

/**
 * @brief Draws X-axis grid lines and optional labels for a plot timescale.
 *
 * @param timescale Time scale mapping parameters.
 * @param labelGrid True to draw labels, false to draw only the grid lines.
 */
void drawXAxisGridOnTimescale(PloTimescaleMappingParameters timescale,
                              bool labelGrid);

/**
 * @brief Creates timescale mapping parameters for a plot.
 *
 * @param timerangeStart Start of the displayed time range.
 * @param timerangeEnd End of the displayed time range.
 * @param plotStartPoint Start point of the plot area.
 * @param plotEndPoint End point of the plot area.
 * @param firstGridLineStart Start point of the first vertical grid line.
 * @param secondGridLineStart Start point of the second vertical grid line.
 * @return Constructed timescale mapping parameters.
 */
PloTimescaleMappingParameters
createTimescale(int timerangeStart, int timerangeEnd, Pixel plotStartPoint,
                Pixel plotEndPoint, Pixel firstGridLineStart,
                Pixel secondGridLineStart);