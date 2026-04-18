# Milestone 2 – Space Electronics II Digital Project – TU Berlin  

**Name:** Artur Foden  
**Student ID:** 0518962  

---

# Project Overview

This project implements a **multi-mode embedded satellite system** on the CADSE board v5.  
It integrates sensing, signal generation, telemetry, telecommand, persistent storage, and a graphical user interface.

The system simulates a small satellite with:
- Multiple operational modes  
- Real-time sensor acquisition  
- DAC waveform generation  
- MQTT-based communication  
- On-device visualization  

---

# Features

## Boot Sequence
- Displays a step by step on the screen
- Initializes:
  - WiFi
  - MQTT
  - IMU (accelerometer + gyroscope)
  - Environmental sensor (BME280)
  - ADC / DAC
  - SD card
- Includes:
  - Status feedback
  - Timeout handling
  - Visual indicators (RGB + screen)

---

## Modes

The system supports **4 operation modes**:

### Mode 0 – Idle / Command Mode
- Displays: *“Waiting for commands”*
- Used as safe default mode

---

### Mode 1 – Waveform Generator
- Generates and displays signals via DAC:
  - Sine  
  - Triangle  
  - Square  
  - Sawtooth  
- Adjustable via touch input:
  - Frequency  
  - Amplitude  
  - Waveform type  
- Displays:
  - Waveform preview plot
  - DAC output voltage
  - Control parameters

---

### Mode 2 – Sensor Plotting
- Real-time rolling plots:
  - Accelerometer  
  - Pressure  
  - ADC input  
  - MQTT data  
- Features:
  - Dynamic Y-axis scaling
  - Plot switching via touch
  - Overlay grids and labels
- Adjustable via touch input:
  - Plot type  
  - X axis  
  - Y axis
- Visual alerts:
  - RGB LEDs indicate high ADC voltage thresholds

---

### Mode 3
- Reserved for future extension

---

# Telemetry (MQTT)

The system publishes telemetry at **1 Hz**.

### Includes:
- Modes:
  - Current mode  
  - Default mode  
- Plot info:
  - Current plot  
  - Default plot  
- Sensors:
  - Acceleration (X/Y/Z)  
  - Angular rate (X/Y/Z)  
  - Pressure  
  - Temperature  
  - Humidity  
- Electrical:
  - Battery voltage  
  - USB voltage  
  - ADC input  
  - DAC output  
- System:
  - WiFi RSSI  
  - Uptime  
- Waveform:
  - Frequency  
  - Amplitude  
  - Type  

---

# Persistent Storage

Stored on SD card:
- Default mode ->  `/default_mode.txt`
- Default plot ->  `/default_plot.txt`

Used to:
- Restore configuration after reboot  
- Allow telecommand-based persistence  

---

# Plotting System

Custom plotting framework:
- `PlotData`
- `PlotHistoryDataPoint`
- `PlotDataMappingParameters`
- `PloTimescaleMappingParameters`

### Key features:
- Rolling history plots (oscilloscope style)
- Adaptive rendering:
  - Line interpolation (high resolution)
  - Pixel-only (low resolution / long timescales)
- Modular overlays via function pointers

---

# Architecture

### Key modules:
- `BootSequence/` ->  Hardware initialization + startup UI  
- `Mode0–3/` ->  Operational modes  
- `UserInterface/` ->  Rendering system  
- `Screen/` ->  Display abstraction  
- `Accelerometer/`, `EnvironmentalSensor/`, `ADC/`, `DAC/` ->  Hardware drivers  
- `Telemetry/`, `Telecommand/` ->  MQTT interface  
- `Networking/` ->  WiFi + MQTT setup  
- `Utils/` ->  Timing, file IO, helpers  

---

#  Timing System

- `SystemClock` used for timing control  
- Debounce-based scheduling:
  - Sensor updates (~20 Hz)  
  - Screen refresh (~20 Hz)  
  - Telemetry (~1 Hz)  

---

# Key Concepts

- **Debouncing instead of delays** ->  used to prevent flow being blocked
- **Function pointers** ->  modular overlays and modes 
- **Circular buffers** ->  efficient plot history  
- **Separation of concerns** ->  maintainability

---

# Build & Upload

## Requirements
- PlatformIO IDE extension  
- CADSE board v5 
- SD card or dummy SD inserted in slot 

## Steps
1. Connect board  
2. Build project  
3. Upload firmware  

---

# Usage

- Use **touch buttons** to:
  - Change modes  
  - Adjust waveform parameters  
  - Switch plots
  - Change plot parameters

- Use the following **MQTT** telecommands:
  - `/default_mode` -> set default mode
  - `/current_mode` -> set current mode (requires SD)
  - `/current_plot` -> set current plot
  - `/default_plot` -> set default plot (requires SD)
  - `/plot_timerange_s` -> set timerange for the plot (in seconds)
  - `/increment_y_axis` -> increase the Y axis scale, does not need a message
  - `/decrement_y_axis` -> decrease the Y axis scale, does not need a message
  - `/mqtt_source` -> Send data to be plotted by the `mqtt` plot
  - `/amplitude_voltage` -> Set the voltage amplitude of the waveform
  - `/frequency_hz` -> Set the frequency of the waveform
  - `/waveform` -> Set the type of waveform

- Observe:
  - Real-time plots  
  - LED indicators  
  - Telemetry output  

---

# Controls

- **Left / Right touch** ->  Switch between modes  
- **X touch** ->  Cycle selected waveform parameter (frequency / amplitude / waveform type)  
- **Up / Down touch**
  - Mode 1 ->  Adjust selected waveform parameter  
  - Mode 2 ->  Switch between available plots  

---

# Notes

- DAC waveform display uses forward time scaling (fixed from earlier versions)
- ADC input is calibrated and scaled to external voltage range
- Plot resolution adapts automatically to time range

---

# TODO 

- Mode 3 functionality
- Add filtering / smoothing to sensor plots  
- Improve ADC calibration for >30V range  
- Add persistent waveform configuration  
- Add unit tests  
- Remove blocking `delay()` calls when blinking LEDs
- EEPROM based persistent storage

---

# Bonus achievements

- Doxygen HTML output: open `index.html` in `/html` folder. (R7.2)
- Plotted data may originate from MQTT source. (R6.3)