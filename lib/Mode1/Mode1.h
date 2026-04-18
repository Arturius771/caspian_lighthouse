/**
 * @file Mode1.h
 * @brief Waveform generation and control mode.
 *
 * This module allows configuration and control of output signals via the DAC.
 * It supports different waveform types.
 *
 * The module also manages user interaction for selecting and modifying
 * waveform parameters, and integrates with the plotting system to visualize
 * the generated signal in real time.
 */

#include <DAC.h>
#include <UserInterface.h>

/**
 * @brief Mode 1: Waveform generation and display mode.
 */
void mode1();

/**
 * @brief Sets waveform type from string representation.
 *
 * @param message Waveform name.
 */
void setWaveformFromString(const String &message);

/**
 * @brief Sets waveform frequency.
 *
 * @param frequencyHz New frequency in hertz.
 */
void setFrequency(float frequencyHz);

/**
 * @brief Sets waveform amplitude.
 *
 * @param newAmplitudeVolts New amplitude in volts.
 */
void setAmplitude(float newAmplitude);

/**
 * @brief Cycles through the selected waveform control parameter.
 */
void incrementWaveformControl();

/**
 * @brief Increments the currently selected waveform control parameter.
 */
void incrementCurrentWaveformControl();

/**
 * @brief Decrements the currently selected waveform control parameter.
 */
void decrementCurrentWaveformControl();

/**
 * @brief Current waveform configuration (amplitude and frequency).
 */
extern Wave wave;

/**
 * @brief DAC waveform plot object.
 */
extern PlotData dacPlot;

/**
 * @brief Returns the current waveform as a string.
 *
 * @return Name of the waveform.
 */
String getWaveform();