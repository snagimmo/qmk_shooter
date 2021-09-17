#include QMK_KEYBOARD_H
#include "font_block.h"

static uint8_t buf_blocks = 0;
static uint8_t offset = 0;

void set_rotation(uint8_t on) {
    if (on) {
        // vertical
        buf_blocks = OLED_DISPLAY_HEIGHT / OLED_FONT_WIDTH; // 5 (default)
        offset     = OLED_DISPLAY_HEIGHT % OLED_FONT_WIDTH; // 2px (default)
    } else {
        // horizontal
        buf_blocks = OLED_DISPLAY_WIDTH / OLED_FONT_WIDTH; // 21 (default)
        offset     = OLED_DISPLAY_WIDTH % OLED_FONT_WIDTH; // 2px (default)
    }
}

void write_font_block(const unsigned char* p, uint8_t rIdx, uint8_t wIdx) {
    unsigned char raw_byte;
    uint8_t line_number = wIdx / buf_blocks; // Get a line number to write

    for (int i = 0; i < OLED_FONT_WIDTH; i++) {
        raw_byte = pgm_read_byte(p + (rIdx * OLED_FONT_WIDTH + i));
        oled_write_raw_byte(raw_byte, wIdx * OLED_FONT_WIDTH + i + (line_number * offset)); // Skip 2*N px every line
    }
}

void write_font_blocks(const unsigned char* p, uint8_t row, uint8_t col, uint8_t p_rIdx, uint8_t p_wIdx) {
    uint8_t rIdx, wIdx;

    for (int i = 0; i < row; i++) {
        // Read blocks per line from OLED font (default 32 blocks)
        rIdx  = p_rIdx + (i * 32);
        // Write blocks per line to OLED buffer (default 5 or 21 blocks)
        wIdx = p_wIdx + (i * buf_blocks);
        for (int j = 0; j < col; j++) {
            write_font_block(p, rIdx + j, wIdx + j);
        }
    }
}

void clear_display(void) {
    uint8_t row = (buf_blocks == 5 ? 16   // vertical
                                   :  3); // horizontal
    for (int i = 0; i < row; i++) {
        for (int j = 0; j < buf_blocks; j++) {
            for (int k = 0; k < OLED_FONT_WIDTH; k++) {
                // Skip 2*N px every line
                // 0 + i * buf_blocks + j = wIdx + j (L36, L38)
                oled_write_raw_byte(0x00, (i * buf_blocks + j) * OLED_FONT_WIDTH + k + (i * offset));
            }
        }
    }
}

void clear_block(uint8_t wIdx) {
    uint8_t row = wIdx / buf_blocks; // Get a row number to write
    uint8_t col = wIdx % buf_blocks; // Get a column number to write

    for (int i = 0; i < OLED_FONT_WIDTH; i++) {
        oled_write_raw_byte(0x00, (row * buf_blocks + col) * OLED_FONT_WIDTH + i + (row * offset)); // Skip 2*N px every line
    }
}
