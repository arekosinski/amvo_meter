# AMVO Meter Project

This project is responsible for measuring and displaying voltage, current, and power using the INA226 sensor and an OLED display. The project is built using the PlatformIO framework and is designed to run on an Arduino-compatible board.

## Libraries Used

The following libraries are used in this project:

- **Arduino**: The core library for Arduino development.
- **Wire**: A library for I2C communication.
- **INA226**: A library for interfacing with the INA226 current and power sensor.
- **Adafruit GFX Library**: A graphics library for drawing shapes and text on displays.
- **Adafruit SSD1306**: A library for controlling SSD1306-based OLED displays.

## Project Structure

The project is organized into the following directories:

- **src/**: Contains the main source code files.
  - `amvo_meter.cpp`: The main application code.
  - `ina226_calibration.cpp`: Code for calibrating the INA226 sensor.
  - `test_avarager.cpp`: Test code for the DataSeries class.
  - `test_i2c_screen.cpp`: Test code for the I2C OLED screen.

- **lib/**: Contains project-specific libraries.
  - `avarager/`: Contains the DataSeries class for handling data series operations.

## Getting Started

To get started with this project, you will need to have PlatformIO installed. You can then build and upload the project to your Arduino-compatible board using the following commands:

```sh
pio run
pio run --target upload
```

For more information on PlatformIO, please refer to the PlatformIO documentation.