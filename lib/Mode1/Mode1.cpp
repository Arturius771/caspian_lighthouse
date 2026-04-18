#include <Mode1.h>

/**
 * @brief Margin layout spacing.
 */
const int MARGIN = 9;
/**
 * @brief Half the number of steps in the waveform lookup table.
 *
 * Used for waveform triangle and square wave symmetry.
 */
const int HALF_PHASE_STEPS = LOOKUP_TABLE_SIZE / 2;
/**
 * @brief Mapping configuration for DAC waveform plot.
 */
PlotDataMappingParameters dacSourcePlotMapping =
    createPlotMappings(DAC_MIN_VOLTAGE, DAC_MAX_VOLTAGE, SCREEN_HEIGHT - MARGIN,
                       MARGIN, "", 1, 1, 3000, false, false);
/**
 * @brief Buffer holding DAC waveform data history.
 */
PlotHistoryDataPoint dacData[PLOT_BUFFER_SIZE];
/**
 * @brief First vertical grid line position.
 */
Pixel firstGridLineStart = {SCREEN_WIDTH / 3, MARGIN};
/**
 * @brief Second vertical grid line position.
 */
Pixel secondGridLineStart = {(SCREEN_WIDTH / 3) * 2, MARGIN};
/**
 * @brief Time scale mapping for waveform plot.
 */
PloTimescaleMappingParameters signal_timescale = createTimescale(
    0, DEFAULT_TIMESCALE.timerangeStart, DEFAULT_TIMESCALE.plotStartPoint,
    DEFAULT_TIMESCALE.plotEndPoint, firstGridLineStart, secondGridLineStart);
/**
 * @brief DAC waveform plot object.
 */
PlotData dacPlot =
    createPlot(DAC, dacSourcePlotMapping, signal_timescale, dacData);
/**
 * @brief Current waveform configuration (amplitude and frequency).
 */
Wave wave = {0.5f, calculateFrequencyFromMillisecond(2000)};
/**
 * @brief Currently selected waveform type.
 */
Waveform currentWaveform = SINE;
/**
 * @brief Currently selected control parameter.
 *
 * 0 = frequency, 1 = amplitude, 2 = waveform type
 */
int selectedControl = 0;
/**
 * @brief DAC midpoint value in code units.
 */
constexpr float DAC_MIDPOINT_CODE = DAC_MAX_VOLTAGE / 2.0f;
/**
 * @brief Maximum allowed waveform amplitude in volts.
 */
constexpr float DAC_MAX_AMPLITUDE_VOLTS = V_REF / 2.0f;
/**
 * @brief Maximum allowed waveform frequency in hertz.
 */
constexpr float MAX_FREQUENCY_HZ = 10000.0f;

/**
 * @brief Cycles through the selected waveform control parameter.
 */
void incrementWaveformControl() { selectedControl = (selectedControl + 1) % 3; }

/**
 * @brief Increments the currently selected waveform control parameter.
 */
void incrementCurrentWaveformControl() {
  if (selectedControl == 0 && wave.frequency > 0.0f &&
      wave.frequency <= MAX_FREQUENCY_HZ) {
    setFrequency(wave.frequency + 0.1f);
  } else if (selectedControl == 1 && wave.amplitude > 0.0f &&
             wave.amplitude <= DAC_MAX_AMPLITUDE_VOLTS) {
    setAmplitude(wave.amplitude + 0.1f);
  } else {
    setWaveformFromString(getWaveform() == "sine"       ? "sawtooth"
                          : getWaveform() == "sawtooth" ? "square"
                          : getWaveform() == "square"   ? "triangle"
                                                        : "sine");
  }
}

/**
 * @brief Decrements the currently selected waveform control parameter.
 */
void decrementCurrentWaveformControl() {
  if (selectedControl == 0 && wave.frequency > 0.0f &&
      wave.frequency <= MAX_FREQUENCY_HZ) {
    setFrequency(wave.frequency - 0.1f);
  } else if (selectedControl == 1 && wave.amplitude > 0.0f &&
             wave.amplitude <= DAC_MAX_AMPLITUDE_VOLTS) {
    setAmplitude(wave.amplitude - 0.1f);
  } else {
    setWaveformFromString(getWaveform() == "sine"       ? "triangle"
                          : getWaveform() == "triangle" ? "square"
                          : getWaveform() == "square"   ? "sawtooth"
                                                        : "sine");
  }
}

/**
 * @brief Returns the peak-to-peak voltage of the waveform.
 *
 * @return Peak-to-peak voltage.
 */
float getWavePeakToPeakVoltage() { return wave.amplitude * 2.0f; }

/**
 * @brief Draws overlay information for the DAC waveform plot.
 *
 * Includes axis labels, waveform parameters, and current DAC output value.
 */
void drawDACOverlay() {
  drawYAxisGrid(dacPlot.mappingParameters, false);
  drawText("V", {2, 0});
  drawText(
      formatValueToDecimalPlaces(getEstimatedDACVoltage(DAC_MAX_VOLTAGE), 1),
      {0, MARGIN});
  drawText("0", {0, SCREEN_HEIGHT - MARGIN});

  Pixel frequencyPosition = {30, 0};
  Pixel amplitudePosition = {80, 0};
  Pixel waveformPosition = {30, SCREEN_HEIGHT - MARGIN + 1};

  bool flashOff =
      ((systemClock.getCurrentTimeMilliseconds() / FLASHING_TEXT_INTERVAL_MS) %
           2 ==
       0);

  String frequencyText = "Hz=" + formatValueToDecimalPlaces(wave.frequency, 1);
  String amplitudeText = "A=" + formatValueToDecimalPlaces(wave.amplitude, 1);
  String waveformText = getWaveform();

  // Flash the current mode for UX
  if (flashOff) {
    if (selectedControl == 0) {
      frequencyText = "";
    } else if (selectedControl == 1) {
      amplitudeText = "";
    } else if (selectedControl == 2) {
      waveformText = "";
    }
  }

  drawText(frequencyText, frequencyPosition);
  drawText(amplitudeText, amplitudePosition);
  drawText(waveformText, waveformPosition);

  drawText("DAC:", {1, (SCREEN_HEIGHT / 2) - 8});
  drawText(formatValueToDecimalPlaces(getDACOutputVoltage(), 2),
           {1, SCREEN_HEIGHT / 2});
}

/**
 * @brief Maps a buffer index to a waveform phase index.
 *
 * @param index Plot buffer index.
 * @return Corresponding phase index.
 */
int phaseForIndex(int index) {
  return map(index, 0, PLOT_BUFFER_SIZE - 1, 0, LOOKUP_TABLE_SIZE - 1);
}

/**
 * @brief Returns the current waveform as a string.
 *
 * @return Name of the waveform.
 */
String getWaveform() {
  if (currentWaveform == TRIANGLE) {
    return "triangle";
  } else if (currentWaveform == SQUARE) {
    return "square";
  } else if (currentWaveform == SAWTOOTH) {
    return "sawtooth";
  }
  return "sine";
}

/**
 * @brief Stores a waveform value in the plot buffer after scaling.
 *
 * @param rawValue Raw waveform value from generator.
 * @param index Buffer index.
 */
void storeWaveValue(float rawValue, int index) {
  float normalized = (rawValue - DAC_MIDPOINT_CODE) / DAC_MIDPOINT_CODE;
  float amplitudeInCodes = voltsToDacCode(wave.amplitude);
  float scaledValue = DAC_MIDPOINT_CODE + (normalized * amplitudeInCodes);

  dacData[index].dataRecordedTime = index;
  dacData[index].dataValue =
      (float)clampValue((int)scaledValue, (int)DAC_MAX_VOLTAGE + 1);
}

/**
 * @brief Generates raw sine waveform value.
 *
 * @param phase Phase index.
 * @return Raw DAC code.
 */
float sineRawValue(int phase) {
  return (float)pgm_read_word(&(DACLookup_FullSine_9Bit[phase]));
}

/**
 * @brief Generates raw triangle waveform value.
 *
 * @param phase Phase index.
 * @return Raw DAC code.
 */
float triangleRawValue(int phase) {
  if (phase < HALF_PHASE_STEPS) {
    return phase * (DAC_MAX_VOLTAGE / (HALF_PHASE_STEPS - 1));
  }
  return (LOOKUP_TABLE_SIZE - 1 - phase) *
         (DAC_MAX_VOLTAGE / (HALF_PHASE_STEPS - 1));
}

/**
 * @brief Generates raw square waveform value.
 *
 * @param phase Phase index.
 * @return Raw DAC code.
 */
float squareRawValue(int phase) {
  return phase < HALF_PHASE_STEPS ? DAC_MAX_VOLTAGE : DAC_MIN_VOLTAGE;
}

/**
 * @brief Generates raw sawtooth waveform value.
 *
 * @param phase Phase index.
 * @return Raw DAC code.
 */
float sawtoothRawValue(int phase) {
  return (LOOKUP_TABLE_SIZE - 1 - phase) *
         (DAC_MAX_VOLTAGE / (LOOKUP_TABLE_SIZE - 1));
}

/**
 * @brief Generates waveform samples into the plot buffer.
 *
 * @param rawValueForPhase Function pointer to waveform generator.
 */
void drawWave(float (*rawValueForPhase)(int)) {
  for (int i = 0; i < PLOT_BUFFER_SIZE; i++) {
    int phase = phaseForIndex(i);
    storeWaveValue(rawValueForPhase(phase), i);
  }
}

/**
 * @brief Sets waveform frequency.
 *
 * @param frequencyHz New frequency in hertz.
 */
void setFrequency(float frequencyHz) {
  if (frequencyHz <= 0.0f)
    return;
  wave = {wave.amplitude, frequencyHz};
}

/**
 * @brief Sets waveform amplitude.
 *
 * @param newAmplitudeVolts New amplitude in volts.
 */
void setAmplitude(float newAmplitudeVolts) {
  if (newAmplitudeVolts < 0.0f)
    return;
  if (newAmplitudeVolts > DAC_MAX_AMPLITUDE_VOLTS)
    return;
  wave = {newAmplitudeVolts, wave.frequency};
}

/**
 * @brief Sets waveform type from string representation.
 *
 * @param message Waveform name.
 */
void setWaveformFromString(const String &message) {
  if (message == "sine")
    currentWaveform = SINE;
  else if (message == "triangle")
    currentWaveform = TRIANGLE;
  else if (message == "square")
    currentWaveform = SQUARE;
  else if (message == "sawtooth")
    currentWaveform = SAWTOOTH;
}

/**
 * @brief Mode 1: Waveform generation and display.
 *
 * Generates waveform data, updates the display, and renders the waveform plot.
 */
void mode1() {
  switch (currentWaveform) {
  case SINE:
    drawWave(sineRawValue);
    break;
  case TRIANGLE:
    drawWave(triangleRawValue);
    break;
  case SQUARE:
    drawWave(squareRawValue);
    break;
  case SAWTOOTH:
    drawWave(sawtoothRawValue);
    break;
  }

  if (screenMayRefresh(systemClock.getCurrentTimeMilliseconds())) {
    clearScreen();
    drawWaveformPlot(dacPlot, drawDACOverlay, wave.frequency);
    updateScreen();
  }
}