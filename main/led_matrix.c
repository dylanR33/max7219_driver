#include <stdio.h>
#include "MAX7219.h"

void app_main(void) {
    max7219_config_t max_config = {
        .mosi_gpio = GPIO_NUM_23,
        .cs_gpio = GPIO_NUM_22,
        .clk_gpio = GPIO_NUM_21,
        .display_size = {.x = 8,
                         .y = 8},
        .display_map = {0}
    };

    matrix_init(&max_config);
    
    // Runs through each cell.
    //for (int y = 0; y < max_config.display_size.y; y++) {
    //    for (int x = 0; x < max_config.display_size.x; x++) {
    //        draw_pixel(x,y, &max_config);
    //        vTaskDelay(63/portTICK_PERIOD_MS);
    //        clear_screen();
    //    }
    //}

    int x = 0;
    int y = 0;

    //  Draws y = x
    //for (int i = 0; i < max_config.display_size.x; i++) {
    //    x = i;
    //    y = x;
    //    draw_pixel(x,y,&max_config);
    //}

    // Draws y = x^2
    for (int i = 0; i < max_config.display_size.x; i++) {
        x = i;
        y = (x*x);
        draw_pixel(x,y,&max_config);
    }

}
