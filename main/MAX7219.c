#include "MAX7219.h"

static spi_device_handle_t led_matrix_spi;

const uint8_t digit_addresses[] = {DIGIT_0_REG, DIGIT_1_REG, DIGIT_2_REG, DIGIT_3_REG,
                                   DIGIT_4_REG, DIGIT_5_REG, DIGIT_6_REG, DIGIT_7_REG};

// Start x and y at 0.
void draw_pixel(int x, int y, max7219_config_t* cfg) {
    // If the specified cell to draw to is out of the bounds of
    // the display, then return.
    if ((x > cfg->display_size.x) || (y > cfg->display_size.y)) {
        return;
    }

    uint8_t cell = 0x00;
    
    // 0x80 represents the origin of the display, which is taken
    // to be the top left corner. Shift its bits according to the desired
    // x value to get the correct positioning.
    cell |= (0x80 >> x);

    write_reg(digit_addresses[y], (cfg->display_map[y] | cell));

    // Set the new map state.
    cfg->display_map[y] |= cell;
}

void flicker_screen() {
    display_test();
    vTaskDelay(1000/portTICK_PERIOD_MS);
    stop_display_test();
    shut_down();
    vTaskDelay(1000/portTICK_PERIOD_MS);
    normal_operation();
}
void smiley_face() {
    write_reg(DIGIT_1_REG, 0x66);
    write_reg(DIGIT_2_REG, 0x66);
    write_reg(DIGIT_4_REG, 0x42);
    write_reg(DIGIT_5_REG, 0x24);
    write_reg(DIGIT_6_REG, 0x18);
}

void heart() {

    write_reg(DIGIT_2_REG, 0x66);
    write_reg(DIGIT_3_REG, 0x99);
    write_reg(DIGIT_4_REG, 0x42);
    write_reg(DIGIT_5_REG, 0x24);
    write_reg(DIGIT_6_REG, 0x18);
}

// Writing zeros to all digit addresses.
void clear_screen() {
    for (int i = 0; i < 8; i++) {
        write_reg(digit_addresses[i], 0x00);
    }
}

void write_reg(uint8_t address, uint8_t data_out) {
    uint8_t buf[] = {address, data_out};
    spi_transaction_t transaction = {
        .length = 16,
        .tx_buffer = buf,
        .rx_buffer = NULL
    };
    spi_device_transmit(led_matrix_spi, &transaction);
};

void display_test() {
    write_reg(DISPLAY_TEST_REG, 0x01);
}

void stop_display_test() {
    write_reg(DISPLAY_TEST_REG, 0x00);
}

void shut_down() {
    write_reg(SHUTDOWN_REG, 0x00);
}

void normal_operation() {
    write_reg(SHUTDOWN_REG, 0x01);
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
    spi_bus_add_device(MATRIX_HOST, &device_config, &led_matrix_spi);
    
    // Setting necessary registers to prepare display for writing.
    write_reg(SHUTDOWN_REG, 0x01);
    write_reg(DECODE_MODE_REG, 0x00);
    write_reg(DISPLAY_TEST_REG, 0x00);
    write_reg(SCAN_LIMIT_REG, 0x07);
    write_reg(INTENSITY_REG, 0x07);

    clear_screen();
}

