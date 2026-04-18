/**
 * @file Hardware.h
 * @brief Hardware abstraction and system configuration module.
 *
 * This module defines all hardware-related constants, configurations, and
 * interfaces used across the system. It centralizes pin mappings, peripheral
 * addresses, and hardware component instances, providing a single point of
 * reference for interacting with the underlying device hardware.
 */

#pragma once
#include <Adafruit_ADS1X15.h>
#include <Adafruit_BME280.h>
#include <Adafruit_LSM6DS3TRC.h>
#include <Adafruit_MCP4725.h>
#include <Adafruit_SSD1306.h>
#include <FS.h>
#include <SD_MMC.h>
#include <SPI.h>
#include <WiFi.h>
#include <WiFiClientSecure.h>
#include <arduino_secrets.h>

/**
 * @brief I2C data pin.
 */
constexpr int SDA_PIN = 8;
/**
 * @brief I2C clock pin.
 */
constexpr int SCL_PIN = 9;
/**
 * @brief Chip select pin for the IMU.
 */
constexpr int IMU_CS = 10;
/**
 * @brief I2C address of the SSD1306 display.
 */
constexpr int SSD1306_I2C = 0x3C;
/**
 * @brief I2C address of the MCP23017 port expander.
 */
constexpr int MCP23017_I2C = 0x20;
/**
 * @brief I2C address of the ADS1X15 ADC.
 */
constexpr int ADS1X15_I2C = 0x48;
/**
 * @brief I2C address of the MCP4725 DAC.
 */
constexpr int MCP4725_I2C = 0x60;
/**
 * @brief I2C address of the BME280 environmental sensor.
 */
constexpr int BME280_I2C = 0x76;
/**
 * @brief Screen width in pixels.
 */
constexpr int SCREEN_WIDTH = 128;
/**
 * @brief Screen height in pixels.
 */
constexpr int SCREEN_HEIGHT = 64;
/**
 * @brief Pin number for the right bumper button.
 */
constexpr int RIGHT_BUMPER_BUTTON_PIN = 15;
/**
 * @brief Touch pin for the up control.
 */
constexpr int TOUCH_UP_PIN = 1;
/**
 * @brief Touch pin for the left control.
 */
constexpr int TOUCH_LEFT_PIN = 2;
/**
 * @brief Touch pin for the X control.
 */
constexpr int TOUCH_X_PIN = 4;
/**
 * @brief Touch pin for the down control.
 */
constexpr int TOUCH_DOWN_PIN = 5;
/**
 * @brief Touch pin for the right control.
 */
constexpr int TOUCH_RIGHT_PIN = 6;
/**
 * @brief Threshold used for touch detection.
 */
constexpr int TOUCH_DETECTION_THRESHOLD = 60000;
/**
 * @brief PWM channel used for the buzzer.
 */
constexpr int BUZZER_CHANNEL = 0;
/**
 * @brief Output pin connected to the buzzer.
 */
constexpr int BUZZER_PIN = 14;
/**
 * @brief Pin connected to the boot button LED.
 */
constexpr int LED_BOOT_BUTTON_PIN = 0;
/**
 * @brief ADC pin used for battery voltage measurement.
 */
constexpr gpio_num_t BATTERY_VOLTAGE_PIN = GPIO_NUM_7;
/**
 * @brief ADC pin used for USB voltage measurement.
 */
constexpr gpio_num_t USB_VOLTAGE_PIN = GPIO_NUM_3;
/**
 * @brief SD card clock pin.
 */
constexpr int SD_CLOCK = 18;
/**
 * @brief SD card command pin.
 */
constexpr int SD_COMMAND = 17;
/**
 * @brief SD card data pin 0.
 */
constexpr int SD_D0 = 21;
/**
 * @brief SD card data pin 1.
 */
constexpr int SD_D1 = 38;
/**
 * @brief SD card data pin 2.
 */
constexpr int SD_D2 = 47;
/**
 * @brief SD card data pin 3.
 */
constexpr int SD_D3 = 48;
/**
 * @brief MCP23017 I/O direction register A.
 */
constexpr uint8_t IODIRA = 0x00;
/**
 * @brief MCP23017 I/O direction register B.
 */
constexpr uint8_t IODIRB = 0x01;
/**
 * @brief MCP23017 GPIO register A.
 */
constexpr uint8_t GPIOA = 0x12;
/**
 * @brief MCP23017 GPIO register B.
 */
constexpr uint8_t GPIOB = 0x13;
/**
 * @brief MCP23017 output latch register A.
 */
constexpr uint8_t OLATA = 0x14;
/**
 * @brief MCP23017 output latch register B.
 */
constexpr uint8_t OLATB = 0x15;
/**
 * @brief Cached output state for port expander GPIOA.
 *
 * HIGH means off for active-low LEDs.
 */
static uint8_t gpioAState = 0xFF;
/**
 * @brief Cached output state for port expander GPIOB.
 *
 * HIGH means off for active-low LEDs.
 */
static uint8_t gpioBState = 0xFF;
/**
 * @brief Wi-Fi SSID used for network connection.
 */
const String WIFI_SSID = SECRET_WIFI_SSID;
/**
 * @brief Wi-Fi password used for network connection.
 */
const char WIFI_PASSWORD[] = SECRET_WIFI_PASSWORD;

/**
 * @brief External IMU hardware instance.
 */
extern Adafruit_LSM6DS3TRC imu;
/**
 * @brief External OLED display hardware instance.
 */
extern Adafruit_SSD1306 screen;
/**
 * @brief External BME280 environmental sensor instance.
 */
extern Adafruit_BME280 bme;
/**
 * @brief External ADC hardware instance.
 */
extern Adafruit_ADS1115 adc;
/**
 * @brief External DAC hardware instance.
 */
extern Adafruit_MCP4725 dac;

/**
 * @brief Reference voltage used by the electronics.
 */
constexpr float V_REF = 3.3;

/**
 * @brief Pin mapping for an RGB LED connected through the port expander.
 */
struct RGBLed {
  /**
   * @brief Expander pin for the red channel.
   */
  uint8_t r;
  /**
   * @brief Expander pin for the green channel.
   */
  uint8_t g;
  /**
   * @brief Expander pin for the blue channel.
   */
  uint8_t b;
};

/**
 * @brief RGB LED group 1 pin mapping.
 */
const RGBLed RGB_1 = {7, 8, 9};

/**
 * @brief RGB LED group 2 pin mapping.
 */
const RGBLed RGB_2 = {10, 11, 12};

/**
 * @brief RGB LED group 3 pin mapping.
 */
const RGBLed RGB_3 = {13, 14, 15};

/**
 * @brief Status returned when checking or loading a hardware component.
 */
struct ComponentStatus {
  /**
   * @brief Error description, if any.
   */
  String errorStatus;

  /**
   * @brief Indicates whether the component is loaded successfully.
   */
  bool isLoaded;
};

/**
 * @brief Path to the file storing the default mode.
 */
extern const char DEFAULT_MODE_PATH[];
/**
 * @brief Path to the file storing the default plot type.
 */
extern const char DEFAULT_PLOT_PATH[];

/**
 * @brief Checks whether a button is currently pressed.
 *
 * @param pin Pin number of the button.
 * @return True if the button is pressed, otherwise false.
 */
bool buttonPressedDown(int pin);

/**
 * @brief Reads the current battery voltage.
 *
 * @return Battery voltage in volts.
 */
float getBatteryVoltage();

/**
 * @brief Reads the current USB voltage.
 *
 * @return USB voltage in volts.
 */
float getUSBVoltage();

/**
 * @brief Gets the current Wi-Fi signal strength.
 *
 * @return Wi-Fi RSSI value.
 */
float getWifiSignalStrength();

/**
 * @brief Emits repeated buzzer beeps while blinking the boot LED.
 *
 * @param count Number of beep and blink cycles.
 * @param duration Duration of each on and off phase in milliseconds.
 */
void beepAndBlink(int count, int duration);

/**
 * @brief Writes a value to a register on the MCP23017 port expander.
 *
 * @param reg Register address.
 * @param value Value to write.
 */
void mcpWriteRegister(uint8_t reg, uint8_t value);

/**
 * @brief Sets the colour state of an RGB LED.
 *
 * @param led RGB LED pin mapping.
 * @param red True to enable red.
 * @param green True to enable green.
 * @param blue True to enable blue.
 */
void setRGB(const RGBLed &led, bool red, bool green, bool blue);

/**
 * @brief Turns off an RGB LED.
 *
 * @param led RGB LED pin mapping.
 */
void rgbOff(const RGBLed &led);