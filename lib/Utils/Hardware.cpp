#include <Hardware.h>

/**
 * @brief IMU hardware instance.
 *
 * Provides accelerometer and gyroscope measurements.
 */
Adafruit_LSM6DS3TRC imu;
/**
 * @brief OLED screen hardware instance.
 *
 * Configured with the display width, height, I2C bus, and reset pin.
 */
Adafruit_SSD1306 screen(SCREEN_WIDTH, SCREEN_HEIGHT, &Wire, -1);
/**
 * @brief Environmental sensor hardware instance.
 *
 * Provides pressure, temperature, and humidity measurements.
 */
Adafruit_BME280 bme;
/**
 * @brief ADC hardware instance.
 *
 * Used for analog voltage measurements.
 */
Adafruit_ADS1115 adc;
/**
 * @brief DAC hardware instance.
 *
 * Used for analog signal output.
 */
Adafruit_MCP4725 dac;
/**
 * @brief Ground resistor value for the USB voltage divider in ohms.
 */
float usbGroundResistor = 470.0f;
/**
 * @brief Top resistor value for the USB voltage divider in ohms.
 */
float usbVoltageDividerResistor = 220.0f;
/**
 * @brief Groun resistor value for the battery voltage divider in ohms.
 */
float batteryGroundResistor = 150.0f;
/**
 * @brief Top resistor value for the battery voltage divider in ohms.
 */
float batteryVoltageDividerResistor = 470.0f;
/**
 * @brief File path used to store the default mode.
 */
const char DEFAULT_MODE_PATH[] = "/default_mode.txt";
/**
 * @brief File path used to store the default plot type.
 */
const char DEFAULT_PLOT_PATH[] = "/default_plot.txt";

/**
 * @brief Checks whether a button is currently pressed.
 *
 * @param pin Pin number of the button.
 * @return True if the button is pressed, otherwise false.
 */
bool buttonPressedDown(int pin) { return digitalRead(pin) == LOW; }

/**
 * @brief Reads a voltage through a resistor divider and reconstructs the input
 * voltage.
 *
 * @param pin Analog input pin used to read the divided voltage.
 * @param rTop Top resistor of the voltage divider in ohms.
 * @param rBottom Bottom resistor of the voltage divider in ohms.
 * @return Calculated input voltage in volts.
 */
float readDividerVoltage(int pin, float rTop, float rBottom) {
  float vOut = analogReadMilliVolts(pin) / 1000.0f;
  return vOut * ((rTop + rBottom) / rBottom);
}

/**
 * @brief Reads the USB voltage using a voltage divider.
 *
 * @return Calculated USB voltage in volts.
 */
float getUSBVoltage() {
  return readDividerVoltage(USB_VOLTAGE_PIN, usbVoltageDividerResistor,
                            usbGroundResistor);
}

/**
 * @brief Reads the battery voltage using a voltage divider.
 *
 * @return Calculated battery voltage in volts.
 */
float getBatteryVoltage() {
  return readDividerVoltage(BATTERY_VOLTAGE_PIN, batteryVoltageDividerResistor,
                            batteryGroundResistor);
}

/**
 * @brief Gets the current Wi-Fi signal strength.
 *
 * @return Wi-Fi RSSI value.
 */
float getWifiSignalStrength() { return WiFi.RSSI(); }

/**
 * @brief Emits repeated buzzer beeps while blinking the boot LED.
 *
 * @param count Number of beep and blink cycles.
 * @param duration Duration of each on and off phase in milliseconds.
 */
void beepAndBlink(int count, int duration) {
  for (int i = 0; i < count; i++) {
    digitalWrite(LED_BOOT_BUTTON_PIN, LOW);
    ledcWrite(BUZZER_CHANNEL, 2048);
    delay(duration);
    digitalWrite(LED_BOOT_BUTTON_PIN, HIGH);
    ledcWrite(BUZZER_CHANNEL, 0);
    delay(duration);
  }
}

/**
 * @brief Writes a value to a register on the MCP23017 port expander.
 *
 * @param reg Register address.
 * @param value Value to write.
 */
void mcpWriteRegister(uint8_t reg, uint8_t value) {
  Wire.beginTransmission(MCP23017_I2C);
  Wire.write(reg);
  Wire.write(value);
  Wire.endTransmission();
}

/**
 * @brief Converts an expander pin number to its bit position within a port.
 *
 * @param epio Expander pin number.
 * @return Bit position within the port.
 */
uint8_t epioToBit(uint8_t epio) { return epio % 8; }

/**
 * @brief Checks whether an expander pin belongs to port B.
 *
 * @param epio Expander pin number.
 * @return True if the pin is on port B, otherwise false.
 */
bool epioIsPortB(uint8_t epio) { return epio >= 8; }

/**
 * @brief Sets the logic level of a pin on the MCP23017 port expander.
 *
 * @param epio Expander pin number.
 * @param levelHigh True to set the pin high, false to set it low.
 */
void setExpanderPin(uint8_t epio, bool levelHigh) {
  uint8_t bit = epioToBit(epio);

  if (epioIsPortB(epio)) {
    if (levelHigh) {
      gpioBState |= (1u << bit);
    } else {
      gpioBState &= ~(1u << bit);
    }
    mcpWriteRegister(GPIOB, gpioBState);
  } else {
    if (levelHigh) {
      gpioAState |= (1u << bit);
    } else {
      gpioAState &= ~(1u << bit);
    }
    mcpWriteRegister(GPIOA, gpioAState);
  }
}

/**
 * @brief Sets the colour state of an RGB LED connected through the port
 * expander.
 *
 * @param led RGB LED pin mapping.
 * @param red True to enable red.
 * @param green True to enable green.
 * @param blue True to enable blue.
 */
void setRGB(const RGBLed &led, bool red, bool green, bool blue) {
  setExpanderPin(led.r, !red);
  setExpanderPin(led.g, !green);
  setExpanderPin(led.b, !blue);
}

/**
 * @brief Turns off an RGB LED.
 *
 * @param led RGB LED pin mapping.
 */
void rgbOff(const RGBLed &led) { setRGB(led, false, false, false); }