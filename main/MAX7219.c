#include "MAX7219.h"

const uint8_t digit_addresses[] = {DIGIT_0_REG, DIGIT_1_REG, DIGIT_2_REG, DIGIT_3_REG,
                                   DIGIT_4_REG, DIGIT_5_REG, DIGIT_6_REG, DIGIT_7_REG};

void draw_plane(uint8_t plane[][8], display_select_t disp, max7219_config_t* cfg) {
    uint8_t* row_values = (uint8_t*)calloc(cfg->display_size.y, sizeof(uint8_t));
    for (int row = 0; row < cfg->display_size.y; row++) {
        for (int col = 0; col < cfg->display_size.x; col++) {
            row_values[row] |= (plane[row][col] << ((cfg->display_size.x - 1) - col));
        }
    }

    for (int i = 0; i < cfg->display_size.y; i++) {
        write_reg(digit_addresses[i], row_values[i], disp, cfg);
    }

    free(row_values);
}

// Start x and y at 0.
void draw_pixel(int x, int y, max7219_config_t* cfg) {
    // If the specified cell to draw to is out of the bounds of
    // the display, then return.
    if ((x >= cfg->display_size.x) || (y >= cfg->display_size.y)) {
        return;
    }
    
    int display_num = x / 8;
    printf("Display num: %d\n", display_num);
    uint8_t cell = 0x00;
    
    // 0x80 represents the origin of the display, which is taken
    // to be the top left corner. Shift its bits according to the desired
    // x value to get the correct positioning.
    cell |= (0x80 >> x);

    if (cfg->invert) {
        cell = reverse_bits(cell);
    }


    write_reg(digit_addresses[y], (cfg->display_map[y] | cell), display_num, cfg);

    // Set the new map state.
    cfg->display_map[y] |= cell;
}

void flicker_screen(display_select_t disp, max7219_config_t* cfg) {
    display_test(disp, cfg);
    vTaskDelay(1000/portTICK_PERIOD_MS);
    stop_display_test(disp, cfg);
    shut_down(disp, cfg);
    vTaskDelay(1000/portTICK_PERIOD_MS);
    normal_operation(disp, cfg);
}

void smiley_face(display_select_t disp, max7219_config_t* cfg) {
    write_reg(DIGIT_1_REG, 0x66, disp, cfg);
    write_reg(DIGIT_2_REG, 0x66, disp, cfg);
    write_reg(DIGIT_4_REG, 0x42, disp, cfg);
    write_reg(DIGIT_5_REG, 0x24, disp, cfg);
    write_reg(DIGIT_6_REG, 0x18, disp, cfg);
}

void heart(display_select_t disp, max7219_config_t* cfg) {

    write_reg(DIGIT_2_REG, 0x66, disp, cfg);
    write_reg(DIGIT_3_REG, 0x99, disp, cfg);
    write_reg(DIGIT_4_REG, 0x42, disp, cfg);
    write_reg(DIGIT_5_REG, 0x24, disp, cfg);
    write_reg(DIGIT_6_REG, 0x18, disp, cfg);
}

// Writing zeros to all digit addresses.
void clear_screen(max7219_config_t* cfg) {
    for (int i = 0; i < DIGIT_REGISTER_COUNT; i++) {
        write_reg(digit_addresses[i], 0x00, display1, cfg);
        write_reg(digit_addresses[i], 0x00, display2, cfg);
        write_reg(digit_addresses[i], 0x00, display3, cfg);
        write_reg(digit_addresses[i], 0x00, display4, cfg);
    }
    for (int i = 0; i < (DIGIT_REGISTER_COUNT * DISPLAY_COUNT); i++)
        cfg->display_map[i] = 0x00;
}

void write_reg(uint8_t address, uint8_t data_out, int display_num, max7219_config_t* cfg) {
    uint8_t* buf = (uint8_t*)calloc(2 * (display_num + 1), sizeof(uint8_t));
    spi_transaction_t transaction = {
        .length = (sizeof(uint8_t) * (2 * (display_num + 1))),
        .tx_buffer = buf,
        .rx_buffer = NULL
    };
    spi_device_transmit(cfg->spi_handle, &transaction);
};

void display_test(display_select_t display, max7219_config_t* cfg) {
    write_reg(DISPLAY_TEST_REG, 0x01, display, cfg);
}

void stop_display_test(display_select_t display, max7219_config_t* cfg) {
    write_reg(DISPLAY_TEST_REG, 0x00, display, cfg);
}

void shut_down(display_select_t display, max7219_config_t* cfg) {
    write_reg(SHUTDOWN_REG, 0x00, display, cfg);
}

void normal_operation(display_select_t display, max7219_config_t* cfg) {
    write_reg(SHUTDOWN_REG, 0x01, display, cfg);
}

void matrix_init(max7219_config_t* cfg) {

    // Setup SPI configurations.
    spi_bus_config_t bus_config = {
        .mosi_io_num = cfg->mosi_gpio,
        .sclk_io_num = cfg->clk_gpio 
    };

    spi_device_interface_config_t device_config = {
        .clock_speed_hz = 10*1000*1000,
        .spics_io_num = cfg->cs_gpio,
        .queue_size = 1,
        .mode = 0
    };
    
    // SPI initializations.
    spi_bus_initialize(MATRIX_HOST, &bus_config, SPI_DMA_CH_AUTO);
    spi_bus_add_device(MATRIX_HOST, &device_config, &cfg->spi_handle);
    
    // Setting necessary registers to prepare display for writing.
    for (display_select_t disp = display1; disp <=DISPLAY_COUNT; disp++) {
        write_reg(SHUTDOWN_REG, 0x01, disp, cfg);
        write_reg(DECODE_MODE_REG, 0x00, disp, cfg);
        write_reg(DISPLAY_TEST_REG, 0x00, disp, cfg);
        write_reg(SCAN_LIMIT_REG, 0x07, disp, cfg);
        write_reg(INTENSITY_REG, 0x07, disp, cfg);
    }

    clear_screen(cfg);
}

uint8_t reverse_bits(uint8_t val) {
    val = (val & 0xF0) >> 4 | (val & 0x0F) << 4;
    val = (val & 0xCC) >> 2 | (val & 0x33) << 2;
    val = (val & 0xAA) >> 1 | (val & 0x55) << 1;
    return val;
}
