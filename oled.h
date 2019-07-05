/* ========================================
 *
 * Copyright YOUR COMPANY, THE YEAR
 * All Rights Reserved
 * UNPUBLISHED, LICENSED SOFTWARE.
 *
 * CONFIDENTIAL AND PROPRIETARY INFORMATION
 * WHICH IS THE PROPERTY OF your company.
 *
 * ========================================
*/
#ifndef OLED_LCD_H__
#define OLED_LCD_H__

#include "u8g2.h"
  
void init_Oled(void);
uint8_t psoc_lcd_callback(u8x8_t *u8x8, uint8_t msg, uint8_t arg_int, void *arg_ptr);
uint8_t psoc_byte_4wire_sw_spi(u8x8_t *u8x8, uint8_t msg, uint8_t arg_int, void *arg_ptr);
//u8g2_Setup_ssd1306_128x64_noname_1(&u8g2, U8G2_R0, psoc_byte_4wire_sw_spi, psoc_gpio_and_delay_psoc);
uint8_t psoc_gpio_and_delay_psoc(u8x8_t *u8x8, uint8_t msg, uint8_t arg_int, void *arg_ptr);
void write_oled(char* s);

#endif 
/* [] END OF FILE */
