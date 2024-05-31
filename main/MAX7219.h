#ifndef MAX7219_H
#define MAX7219_H

#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "esp_check.h"
#include "driver/spi_master.h"
//#include "hal/spi_types.h"
//#include "driver/spi_common.h"
#include "hal/gpio_types.h"

#define NO_OP_REG 0x00

#define DIGIT_0_REG 0x01
#define DIGIT_1_REG 0x02
#define DIGIT_2_REG 0x03
#define DIGIT_3_REG 0x04
#define DIGIT_4_REG 0x05
#define DIGIT_5_REG 0x06
#define DIGIT_6_REG 0x07
#define DIGIT_7_REG 0x08

#define DECODE_MODE_REG 0x09
#define INTENSITY_REG 0x0A
#define SCAN_LIMIT_REG 0x0B
#define SHUTDOWN_REG 0x0C
#define DISPLAY_TEST_REG 0x0F

// SPI_2 HOST
#define MATRIX_HOST HSPI_HOST

typedef struct {
    int x;
    int y;
} display_size_t;

typedef struct {
    gpio_num_t mosi_gpio;
    gpio_num_t cs_gpio;
    gpio_num_t clk_gpio;
    display_size_t display_size;
    uint8_t display_map[8];
} max7219_config_t;

void draw_pixel(int x, int y, max7219_config_t* cfg);

void smiley_face();

void heart();

void flicker_screen();

void clear_screen();

void write_reg(uint8_t address, uint8_t data_out);

void matrix_init(max7219_config_t* cfg);

void display_test();

void stop_display_test();

void shut_down();

void normal_operation();

#endif
