// Artur Foden
// Milestone 2 for Space Electronics II project
// TU Berlin
// Code complete: 30.03.2026

#include <Mode0.h>
#include <Mode3.h>
#include <Telemetry.h>

/**
 * @brief Collects current device state and sensor data and publishes telemetry.
 *
 * This function fills a TelemetryPayload with the current operating mode,
 * default mode, plot selection, power measurements, IMU data, environmental
 * data, Wi-Fi signal strength, uptime, ADC and DAC values, waveform settings,
 * and supported commands, then publishes the payload.
 */
void sendTelemetry() {
  /**
   * @brief Telemetry payload containing the current device status and
   * measurements.
   */
  TelemetryPayload payload;

  payload.currentMode = String(currentMode);
  payload.defaultMode = String(defaultMode);
  payload.currentPlot = getCurrentPlotType();
  payload.defaultPlot = getDefaultPlotType();
  payload.batteryVoltage = formatValueToDecimalPlaces(getBatteryVoltage(), 2);
  payload.usbVoltage = formatValueToDecimalPlaces(getUSBVoltage(), 2);
  payload.angularRateX = formatValueToDecimalPlaces(getGyroscopeData().x, 1);
  payload.angularRateY = formatValueToDecimalPlaces(getGyroscopeData().y, 1);
  payload.angularRateZ = formatValueToDecimalPlaces(getGyroscopeData().z, 1);
  payload.accelerationX =
      formatValueToDecimalPlaces(getAccelerationData().x, 1);
  payload.accelerationY =
      formatValueToDecimalPlaces(getAccelerationData().y, 1);
  payload.accelerationZ =
      formatValueToDecimalPlaces(getAccelerationData().z, 1);
  payload.temperature =
      formatValueToDecimalPlaces(getEnvironmentalData().tempratureC, 2);
  payload.humidity =
      formatValueToDecimalPlaces(getEnvironmentalData().humidityPercent, 2);
  payload.pressure =
      formatValueToDecimalPlaces(getEnvironmentalData().pressureHpa, 2);
  payload.wifiRSSI = formatValueToDecimalPlaces(getWifiSignalStrength(), 1);
  payload.uptimeMs =
      formatValueToDecimalPlaces(systemClock.getCurrentTimeMilliseconds(), 0);
  payload.adcMeasurement = formatValueToDecimalPlaces(getADCInputVoltage(), 1);
  payload.dacOutput = formatValueToDecimalPlaces(getDACOutputVoltage(), 2);
  payload.amplitude = formatValueToDecimalPlaces(wave.amplitude, 1);
  payload.frequency = formatValueToDecimalPlaces(wave.frequency, 1);
  payload.waveform = getWaveform();
  payload.availableCommands = getAvailableCommands();

  updateTelemetryPublication(payload);
}

/**
 * @brief Reads the default mode from persistent storage and applies it.
 *
 * The function reads the stored mode value from DEFAULT_MODE_PATH, trims any
 * surrounding whitespace, converts supported values to the corresponding mode,
 * falls back to mode 0 if the value is invalid, and initializes the selected
 * mode.
 *
 * @note Persistent storage is an SD card.
 */
void parseDefaultModeFromStorage() {
  /**
   * @brief Mode string read from persistent storage.
   */
  String modeString = readFile(DEFAULT_MODE_PATH);
  modeString.trim();

  if (modeString == "1") {
    defaultMode = 1;
  } else if (modeString == "2") {
    defaultMode = 2;
  } else if (modeString == "3") {
    defaultMode = 3;
  } else {
    defaultMode = 0;
  }

  initialiseMode(defaultMode);
}

/**
 * @brief Reads the default plot type from persistent storage and applies it.
 *
 * The function reads the stored plot type from DEFAULT_PLOT_PATH, trims any
 * surrounding whitespace, sets it as the default plot type, and also applies
 * it as the current plot type.
 *
 * @note Persistent storage is an SD cardd.
 */
void parseDefaultPlotFromStorage() {
  /**
   * @brief Plot type string read from persistent storage.
   */
  String plotString = readFile(DEFAULT_PLOT_PATH);
  plotString.trim();

  setDefaultPlotType(plotString);
  setPlotType(plotString);
}

/**
 * @brief Updates all necessary sensors at a consistent frequency.
 */
void updateSensors() {
  if (mayUpdateSensor()) {
    updateAccelerationHistory();
    updatePressureSensor();
    updateADCSensor();
  }
}

/**
 * @brief Performs device setup during boot.
 *
 * This function boots the device infrastructure and then reads the default mode
 * and plot settings from persistent storage.
 */
void setup() {
  bootDevice("Device booting", mqttCallback);

  // Read from SD card default modes and plots.
  parseDefaultModeFromStorage();
  parseDefaultPlotFromStorage();

  while (!deviceHasBootedSuccessfully) {
    // Hold in loop until all online.
    continue;
  }
}

/**
 * @brief Main execution loop for runtime device behaviour.
 */
void loop() {
  if (!mqttConnectedToBroker().isLoaded) {
    bootDevice("Device rebooting", mqttCallback);
    return;
  }
  checkForMessages();
  handleTouchInputs(incrementWaveformControl, incrementCurrentWaveformControl,
                    decrementCurrentWaveformControl, incrementPlotControl,
                    incrementCurrentPlotControl, decrementCurrentPlotControl);
  updateSensors();
  manageModes(mode0, mode1, mode2, mode3);
  outputSignal(dacPlot, wave);
  sendTelemetry();
}