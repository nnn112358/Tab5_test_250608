# CLAUDE.md

This file provides guidance to Claude Code (claude.ai/code) when working with code in this repository.

## Project Overview

M5Tab5 Simple Demo is a simplified embedded application specifically designed for the M5Stack Tab5 device (ESP32-P4). This is a streamlined implementation that directly interfaces with hardware without abstraction layers, making it easier to understand and modify.

## Build Commands

**Prerequisites:** ESP-IDF v5.4.1

**Fetch dependencies:**
```bash
python ./fetch_repos.py
```

**Build, flash and monitor:**
```bash
idf.py build
idf.py flash
idf.py monitor  # Monitor serial output
```

**Config management:**
```bash
idf.py menuconfig  # Configure project via GUI
```

## Development Tools

### Clean Build
```bash
idf.py fullclean  # Clean all build artifacts
idf.py build      # Fresh build
```

### Size Analysis
```bash
idf.py size       # Analyze binary size and memory usage
```

### WiFi C6 Firmware Update
```bash
cd wifi_c6_fw
./flash.sh
```

### Component Testing
Run tests for specific components:
```bash
# Camera sensor detection test
cd components/esp_cam_sensor/test_apps/detect
idf.py build flash monitor

# LVGL performance tests  
cd components/espressif__esp_lvgl_port/test_apps/simd
idf.py build flash monitor
```

## Code Quality

**Format check:** The project uses clang-format-13 for code formatting. CI automatically checks formatting on all C/C++ files except components directories.

## Architecture Overview

### Simplified Single-File Architecture

This is a streamlined implementation with direct hardware integration:

- **Main Application (`main/simple_main.cpp`)**: Single file containing all application logic
- **Hardware Integration**: Direct calls to M5Stack Tab5 BSP functions
- **LVGL UI**: Simple UI creation with direct LVGL API calls
- **No Abstraction Layers**: Direct hardware access for simplicity and performance

### Key Dependencies

- **LVGL v9.2.2**: UI framework for display and touch input
- **Mooncake Log v1.0.0**: Simple logging system  
- **M5Stack Tab5 BSP**: Board support package for hardware access
- **INA226**: Power monitoring IC driver (direct hardware instance)
- **RX8130**: Real-time clock driver (direct hardware instance)

### Hardware Support

**Target Device:** M5Stack Tab5 (ESP32-P4 with 1280x720 display)
- ESP32-P4 with PSRAM and custom partitioning
- Power monitoring via INA226 (direct instance)
- RTC via RX8130 (direct instance)
- Display with touch input via BSP
- USB Host support
- Audio codec initialization

### File Structure

- `main/simple_main.cpp`: Main application entry point (single-file implementation)
- `main/hal/utils/rx8130/`: RTC utility library
- `components/`: ESP-IDF component libraries (camera, video, power monitoring, etc.)
- `dependencies/`: External libraries fetched via `repos.json` (LVGL, logging)
- `wifi_c6_fw/`: WiFi co-processor firmware
- `lv_conf.h`: LVGL configuration file
- `sdkconfig.defaults`: ESP-IDF default configuration

### Important Implementation Details

- All application logic is consolidated in `main/simple_main.cpp`
- Hardware instances (INA226, RX8130) are declared as static globals
- LVGL touch handling is implemented with direct BSP calls
- No HAL abstraction - direct hardware function calls throughout
- Simple UI displays system status, device info, and power readings