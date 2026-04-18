#include <UserInterface.h>
#include <Utils.h>

/**
 * @brief Supported waveform types for DAC signal generation.
 */
enum Waveform {
  /**
   * @brief Sine waveform.
   */
  SINE = 0,

  /**
   * @brief Triangle waveform.
   */
  TRIANGLE = 1,

  /**
   * @brief Square waveform.
   */
  SQUARE = 2,

  /**
   * @brief Sawtooth waveform.
   */
  SAWTOOTH = 3
};

/**
 * @brief Defines waveform parameters for DAC signal generation.
 */
struct Wave {
  /**
   * @brief Wave amplitude.
   */
  float amplitude;

  /**
   * @brief Wave frequency in hertz.
   */
  float frequency;
};

/**
 * @brief Maximum DAC code value.
 */
constexpr float DAC_MAX_VOLTAGE = 4095.0f;

/**
 * @brief Minimum DAC code value.
 */
constexpr float DAC_MIN_VOLTAGE = 0.0f;

/**
 * @brief Number of samples in the DAC sine lookup table.
 */
constexpr int LOOKUP_TABLE_SIZE = 512;

/**
 * @brief External full-cycle sine lookup table for DAC waveform generation.
 *
 * This lookup table contains one complete sine wave represented as DAC codes.
 */
extern const uint16_t DACLookup_FullSine_9Bit[LOOKUP_TABLE_SIZE];

/**
 * @brief Outputs a waveform signal through the DAC.
 *
 * This function writes waveform samples from the provided plot data to the DAC
 * according to the waveform configuration.
 *
 * @param data Plot data containing waveform samples.
 * @param wave Wave configuration containing amplitude and frequency.
 */
void outputSignal(PlotData data, Wave wave);

/**
 * @brief Returns the estimated current DAC output voltage.
 *
 * @return Estimated DAC output voltage in volts.
 */
float getDACOutputVoltage();

/**
 * @brief Estimates the DAC output voltage for a given DAC code.
 *
 * @param value DAC code value.
 * @return Estimated output voltage in volts.
 */
float getEstimatedDACVoltage(int value);

/**
 * @brief Converts a DAC code to a voltage.
 *
 * @param dacCode DAC code value.
 * @return Corresponding voltage.
 */
float dacCodeToVolts(float dacCode);
/**
 * @brief Converts a voltage value to a DAC code.
 *
 * @param volts Input voltage.
 * @return Corresponding DAC code.
 */
float voltsToDacCode(float volts);