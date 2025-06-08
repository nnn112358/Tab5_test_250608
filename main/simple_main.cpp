/*
 * SPDX-FileCopyrightText: 2025 M5Stack Technology CO LTD
 *
 * SPDX-License-Identifier: MIT
 */
#include <memory>
#include <freertos/FreeRTOS.h>
#include <freertos/task.h>
#include <mooncake_log.h>
#include <bsp/m5stack_tab5.h>
#include <ina226.hpp>
#include "utils/rx8130/rx8130.h"

// Direct hardware instances - no HAL abstraction
static INA226 ina226;
static RX8130_Class rx8130;
static lv_disp_t* lvDisp = nullptr;
static lv_indev_t* lvTouchpad = nullptr;
static const std::string TAG = "M5Tab5";

// LVGL touch callback
static void lvgl_read_cb(lv_indev_t* indev, lv_indev_data_t* data)
{
    extern esp_lcd_touch_handle_t _lcd_touch_handle;
    
    if (_lcd_touch_handle == NULL) {
        data->state = LV_INDEV_STATE_REL;
        return;
    }

    uint16_t touch_x[1];
    uint16_t touch_y[1];
    uint16_t touch_strength[1];
    uint8_t touch_cnt = 0;

    esp_lcd_touch_read_data(_lcd_touch_handle);
    bool touchpad_pressed = esp_lcd_touch_get_coordinates(_lcd_touch_handle, touch_x, touch_y, touch_strength, &touch_cnt, 1);

    if (!touchpad_pressed) {
        data->state = LV_INDEV_STATE_REL;
    } else {
        data->state = LV_INDEV_STATE_PR;
        data->point.x = touch_x[0];
        data->point.y = touch_y[0];
    }
}

// Hardware initialization
void hardware_init()
{
    mclog::tagInfo(TAG, "Initializing M5Tab5 hardware...");
    
    // Camera
    mclog::tagInfo(TAG, "Camera init");
    bsp_cam_osc_init();
    
    // I2C
    mclog::tagInfo(TAG, "I2C init");
    bsp_i2c_init();
    i2c_master_bus_handle_t i2c_bus_handle = bsp_i2c_get_handle();
    
    // IO Expander
    mclog::tagInfo(TAG, "IO expander init");
    bsp_io_expander_pi4ioe_init(i2c_bus_handle);
    
    // Audio codec
    mclog::tagInfo(TAG, "Codec init");
    vTaskDelay(pdMS_TO_TICKS(200));
    bsp_codec_init();
    
    // Power monitoring
    mclog::tagInfo(TAG, "INA226 init");
    ina226.begin(i2c_bus_handle, 0x41);
    ina226.configure(INA226_AVERAGES_16, INA226_BUS_CONV_TIME_1100US, INA226_SHUNT_CONV_TIME_1100US, INA226_MODE_SHUNT_BUS_CONT);
    ina226.calibrate(0.005, 8.192);
    
    // RTC
    mclog::tagInfo(TAG, "RX8130 init");
    rx8130.begin(i2c_bus_handle, 0x32);
    rx8130.initBat();
    rx8130.clearIrqFlags();
    rx8130.disableIrq();
    
    // Display
    mclog::tagInfo(TAG, "Display init");
    bsp_reset_tp();
    bsp_display_cfg_t cfg = {
        .lvgl_port_cfg = ESP_LVGL_PORT_INIT_CONFIG(),
        .buffer_size = BSP_LCD_H_RES * BSP_LCD_V_RES,
        .double_buffer = true,
        .flags = {
#if CONFIG_BSP_LCD_COLOR_FORMAT_RGB888
            .buff_dma = false,
#else
            .buff_dma = true,
#endif
            .buff_spiram = true,
            .sw_rotate = true,
        }
    };
    lvDisp = bsp_display_start_with_config(&cfg);
    lv_display_set_rotation(lvDisp, LV_DISPLAY_ROTATION_90);
    bsp_display_backlight_on();
    
    // Touchpad
    mclog::tagInfo(TAG, "Create LVGL touchpad");
    lvTouchpad = lv_indev_create();
    lv_indev_set_type(lvTouchpad, LV_INDEV_TYPE_POINTER);
    lv_indev_set_read_cb(lvTouchpad, lvgl_read_cb);
    lv_indev_set_display(lvTouchpad, lvDisp);
    
    // USB Host
    mclog::tagInfo(TAG, "USB host init");
    bsp_usb_host_start(BSP_USB_HOST_POWER_MODE_USB_DEV, true);
    
    bsp_display_unlock();
    
    mclog::tagInfo(TAG, "Hardware initialization complete");
}

// Simple app launcher
void create_simple_ui()
{
    // Create a simple test screen
    lv_obj_t* scr = lv_scr_act();
    lv_obj_set_style_bg_color(scr, lv_color_hex(0x000000), 0);
    
    // Title
    lv_obj_t* title = lv_label_create(scr);
    lv_label_set_text(title, "M5Stack Tab5");
    lv_obj_set_style_text_color(title, lv_color_hex(0xFFFFFF), 0);
    lv_obj_set_style_text_font(title, &lv_font_montserrat_32, 0);
    lv_obj_center(title);
    lv_obj_align(title, LV_ALIGN_TOP_MID, 0, 50);
    
    // Status info
    lv_obj_t* status = lv_label_create(scr);
    lv_label_set_text(status, "System Ready");
    lv_obj_set_style_text_color(status, lv_color_hex(0x00FF00), 0);
    lv_obj_set_style_text_font(status, &lv_font_montserrat_20, 0);
    lv_obj_align_to(status, title, LV_ALIGN_OUT_BOTTOM_MID, 0, 30);
    
    // Power info
    lv_obj_t* power_label = lv_label_create(scr);
    float voltage = ina226.readBusVoltage();
    char power_text[64];
    snprintf(power_text, sizeof(power_text), "Bus Voltage: %.2fV", voltage);
    lv_label_set_text(power_label, power_text);
    lv_obj_set_style_text_color(power_label, lv_color_hex(0xFFFF00), 0);
    lv_obj_set_style_text_font(power_label, &lv_font_montserrat_16, 0);
    lv_obj_align_to(power_label, status, LV_ALIGN_OUT_BOTTOM_MID, 0, 30);
}

extern "C" void app_main(void)
{
    mclog::tagInfo(TAG, "Starting M5Stack Tab5 Simple Demo");
    
    // Initialize hardware
    hardware_init();
    
    // Create simple UI
    create_simple_ui();
    
    mclog::tagInfo(TAG, "Entering main loop");
    
    // Main loop
    while (true) {
        lv_timer_handler();
        vTaskDelay(pdMS_TO_TICKS(10));
    }
}