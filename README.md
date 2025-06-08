# M5Tab5 Simple Demo

Simplified embedded application for the [M5Stack Tab5](https://docs.m5stack.com/en/products/sku/k145) device (ESP32-P4). This is a streamlined implementation that directly interfaces with hardware without abstraction layers, making it easier to understand and modify.

## Features

- **Direct Hardware Control**: No HAL abstraction, direct M5Stack Tab5 BSP integration
- **Simple UI**: Basic LVGL-based interface with system status display
- **Hardware Support**: Camera, IMU, power monitoring, RTC, display, touch, USB, audio
- **Minimal Dependencies**: Only LVGL and logging libraries
- **Single-File Implementation**: All logic consolidated in `main/simple_main.cpp`

## Hardware Requirements

- **M5Stack Tab5**: ESP32-P4 with 1280x720 display
- **Components**: INA226 power monitor, RX8130 RTC, various camera sensors

## Build Instructions

### Prerequisites

- [ESP-IDF v5.4.1](https://docs.espressif.com/projects/esp-idf/en/v5.4.1/esp32p4/index.html)

### Setup

1. **Fetch dependencies:**
```bash
python ./fetch_repos.py
```

2. **Build:**
```bash
idf.py build
```

3. **Flash:**
```bash
idf.py flash
```

4. **Monitor:**
```bash
idf.py monitor
```

### Configuration

```bash
idf.py menuconfig  # Configure project settings
```

## Development Tools

### WiFi C6 Firmware Update
```bash
cd wifi_c6_fw
./flash.sh
```

### Component Testing
```bash
# Camera sensor detection
cd components/esp_cam_sensor/test_apps/detect
idf.py build flash monitor

# LVGL performance tests
cd components/espressif__esp_lvgl_port/test_apps/simd
idf.py build flash monitor
```

## Project Structure

```
M5Tab5-UserDemo/
├── main/
│   ├── simple_main.cpp          # Main application entry point
│   └── hal/utils/rx8130/        # RTC utility library
├── components/                  # ESP-IDF component libraries
│   ├── esp_cam_sensor/          # Camera sensor drivers
│   ├── esp_video/               # Video processing
│   ├── esp_lvgl_port/           # LVGL integration
│   ├── m5stack_tab5/            # Board support package
│   ├── ina226/                  # Power monitoring
│   └── ...
├── dependencies/                # External libraries
│   ├── lvgl/                    # LVGL UI framework
│   └── mooncake_log/            # Logging system
└── wifi_c6_fw/                  # WiFi co-processor firmware
```

## Key Components

- **LVGL v9.2.2**: UI framework for display and touch input
- **Mooncake Log v1.0.0**: Lightweight logging system
- **M5Stack Tab5 BSP**: Board support package for hardware access
- **INA226**: Power monitoring IC driver
- **RX8130**: Real-time clock driver
- **ESP Camera Sensor**: Multiple camera sensor support

## Code Quality

This project uses clang-format-13 for code formatting. The simplified architecture removes complex abstractions while maintaining clean, readable code.

## Acknowledgments

This project references the following open-source libraries and resources:

- [LVGL](https://github.com/lvgl/lvgl) - Graphics library
- [Mooncake Log](https://github.com/Forairaaaaa/mooncake_log) - Logging system
- [ESP Camera Sensor](https://components.espressif.com/components/espressif/esp_cam_sensor) - Camera drivers
- [ESP Video](https://components.espressif.com/components/espressif/esp_video) - Video processing
- [ESP LVGL Port](https://components.espressif.com/components/espressif/esp_lvgl_port) - LVGL integration
- [INA226 Library](https://github.com/jarzebski/Arduino-INA226) - Power monitoring
- [BMI270 Sensor API](https://github.com/boschsensortec/BMI270_SensorAPI) - IMU support
- [RX8130 Driver](https://github.com/alexreinert/piVCCU/blob/master/kernel/rtc-rx8130.c) - RTC support

## Original Project

This is a simplified version of the original [M5Stack Tab5 User Demo](https://github.com/m5stack/M5Tab5-UserDemo).
