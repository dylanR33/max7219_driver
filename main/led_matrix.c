#include <stdio.h>
#include "MAX7219.h"

void app_main(void) {
    max7219_config_t max_config = {
        .mosi_gpio = GPIO_NUM_23,
        .cs_gpio = GPIO_NUM_22,
        .clk_gpio = GPIO_NUM_21,
        .display_size = {.x = 32,
                         .y = 8},
        .display_map = {0},
        .invert = 1
    };

    matrix_init(&max_config);

    //while(1) {
    //    // Runs through each cell.
    //    for (int y = 0; y < max_config.display_size.y; y++) {
    //        for (int x = 0; x < max_config.display_size.x; x++) {
    //            draw_pixel(x,y, &max_config);
    //            vTaskDelay(50/portTICK_PERIOD_MS);
    //            clear_screen(&max_config);
    //        }
    //    }
    //}
    
    //write_reg(DIGIT_0_REG, 0x01, &max_config);

    //uint8_t buf[] = {DIGIT_0_REG, 0x01, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00};
    //spi_transaction_t trans = {
    //    .length = 16 * 4,
    //    .tx_buffer = buf,
    //    .rx_buffer = NULL
    //};
    //spi_device_transmit(max_config.spi_handle, &trans);

    draw_pixel(0,8, &max_config);

    //uint8_t plane[][8] = { {1,0,1,0,1,0,1,0},
    //                       {0,1,0,1,0,1,0,1},
    //                       {1,0,1,0,1,0,1,0},
    //                       {0,1,0,1,0,1,0,1},
    //                       {1,0,1,0,1,0,1,0},
    //                       {0,1,0,1,0,1,0,1},
    //                       {1,0,1,0,1,0,1,0},
    //                       {0,1,0,1,0,1,0,1} };

    //draw_plane(plane, &max_config);    
}
