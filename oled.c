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
#include <project.h>
#include "u8g2.h"
#include "oled.h"


u8g2_t u8g2;

void init_Oled(void)
{
    //Tried
    //u8g2_Setup_sh1106_128x64_noname_1
    //u8g2_Setup_sh1106_128x64_noname_2 - almost works
    //u8g2_Setup_sh1106_128x64_winstar_1,2 - Upside down
    //u8g2_Setup_sh1106_128x64_vcomh0_2 - grey background
    
    //https://github.com/olikraus/u8g2/wiki/u8g2setupc#introduction
    u8g2_Setup_sh1106_128x64_noname_1(&u8g2, U8G2_R0, psoc_byte_i2c, psoc_gpio_and_delay_psoc); 
    //u8g2_Setup_ssd1306_128x64_noname_f(&u8g2, U8G2_R0, psoc_byte_4wire_sw_spi, psoc_gpio_and_delay_psoc); //_f uses more RAM
    u8g2_InitDisplay(&u8g2); //calls u8x8_InitDisplay
    u8g2_ClearDisplay(&u8g2);
    u8g2_SetPowerSave(&u8g2, 0); // wake up display
    
    /*
    u8g2_ClearBuffer(&u8g2);
    u8g2_SetFont(&u8g2, u8g2_font_ncenB08_tr); //u8g2_font_ncenB08_tr  u8x8_font_amstrad_cpc_extended_f
    u8g2_DrawStr(&u8g2,0,10,"Hello World!");
    u8g2_DrawStr(&u8g2,0,20,"Newton's Attic");
    u8g2_DrawStr(&u8g2,0,30,"Pinball");
    u8g2_DrawStr(&u8g2,0,40,"4");
    u8g2_DrawStr(&u8g2,0,50,"5");
    u8g2_DrawStr(&u8g2,0,60,"6g");
    u8g2_SendBuffer(&u8g2);
    */
    
    //Procedure to print to the OLED display in buffered mode
    //Works with:
    //u8g2_Setup_ssd1306_128x64_noname_1
    //u8g2_Setup_ssd1306_128x64_noname_2
    //The list of fonts is here: https://github.com/olikraus/u8g2/wiki/fntlistall
    u8g2_FirstPage(&u8g2);
    do {
    u8g2_ClearBuffer(&u8g2);
    u8g2_DrawFrame(&u8g2, 0, 0, 128, 64);
    u8g2_SetFont(&u8g2, u8g2_font_helvR14_tr); // //u8g2_font_helvR14_tr
    u8g2_DrawStr(&u8g2, 0,20,"Hello John!");
    u8g2_SetFont(&u8g2, u8g2_font_profont22_tn);
    u8g2_DrawStr(&u8g2, 0,40,"12345");
    u8g2_SetFont(&u8g2, u8g2_font_helvR14_tr);
    u8g2_DrawStr(&u8g2, 0,60,"Jesus Saves");
    } while ( u8g2_NextPage(&u8g2) );
  
}

/* 
 * A PSoC implementation of the delay and gpio callback:
 * 
*/
uint8_t psoc_lcd_callback(u8x8_t *u8x8, uint8_t msg, uint8_t arg_int, void *arg_ptr)
{
    (void)u8x8;
    (void)arg_ptr;
    switch(msg)
    {
        case U8X8_MSG_GPIO_AND_DELAY_INIT:
        break;
        case U8X8_MSG_DELAY_MILLI:
            CyDelay(arg_int);
        break;
        case U8X8_MSG_DELAY_10MICRO:
            CyDelayUs(10);
        break;
        case U8X8_MSG_DELAY_100NANO:
            CyDelayCycles((CYDEV_BCLK__SYSCLK__HZ/1000000) * 100 - 16); //PSoC system reference guide says ~16 extra cycles 
        break;
    }
    return 1;
}


uint8_t psoc_byte_i2c(u8x8_t *u8x8, uint8_t msg, uint8_t arg_int, void *arg_ptr)
{
    uint8_t *data;
    uint8_t num_bytes;
    static uint8_t buffer[32];		/* u8g2/u8x8 will never send more than 32 bytes between START_TRANSFER and END_TRANSFER */
    static uint8_t buf_idx;
    
    switch(msg)
    {
    case U8X8_MSG_BYTE_SEND:
        //num_bytes = 0;
        data = (uint8_t *)arg_ptr;      
        while( arg_int > 0 )
        {
        	//buffer[buf_idx++] = *data;
            I2C_1_I2CMasterWriteByte(*data, 10);
        	data++;
        	arg_int--;
            //num_bytes++;
        }      
        break;
        
    case U8X8_MSG_BYTE_INIT:
        /* add your custom code to init i2c subsystem */
        break;
        
    case U8X8_MSG_BYTE_SET_DC:
        /* ignored for i2c */
        break;
        
    case U8X8_MSG_BYTE_START_TRANSFER: //with SPI CS is set here
        
        I2C_1_I2CMasterSendStart(u8x8_GetI2CAddress(u8x8) >> 1, I2C_1_I2C_WRITE_XFER_MODE, 100);
        break;
        
    case U8X8_MSG_BYTE_END_TRANSFER:
        //uint32 I2C_1_I2CMasterWriteBuf(uint32 slaveAddress, uint8 * wrData, uint32 cnt, uint32 mode)
        I2C_1_I2CMasterSendStop(100);
        break;
    
    default:
      return 0;
  }
  return 1;
}


//https://github.com/olikraus/u8g2/wiki/Porting-to-new-MCU-platform
uint8_t psoc_byte_4wire_sw_spi(u8x8_t *u8x8, uint8_t msg, uint8_t arg_int, void *arg_ptr)
{
    uint8_t *data;
    (void)u8x8; //prevent warning
    //uint8_t internal_spi_mode; //not needed
    switch(msg) {
      
    //Write a single byte to the spi port
    case U8X8_MSG_BYTE_SEND:
      data = (uint8_t *)arg_ptr;
      while( arg_int > 0 ) {
          //
          SPIMaster_WriteTxData((uint8_t)*data);  //psoc specific SPI write command
          CyDelayUs(15); //a delay between each byte is important!!!
          data++;
          arg_int--;
      }  
      break;
      
    //Set CS default level and init SPI clock.
    //Not needed as this is done elsewhere
    case U8X8_MSG_BYTE_INIT:
      OLED_CS_Write(u8x8->display_info->chip_disable_level); //Other SS line
      //Apply reset pulse - This is now done in hardware
      OLED_RES_Write(1);
      CyDelay(1);
      OLED_RES_Write(0);
      CyDelay(10);
      OLED_RES_Write(1);
      break;
    
    //Set the level of the data/command pin to arg_int
    case U8X8_MSG_BYTE_SET_DC:
      OLED_DC_Write(arg_int);
      break;
    
    //U8X8_MSG_BYTE_START_TRANSFER and U8X8_MSG_BYTE_END_TRANSFER basically just control the SS line via software
    //They basically stretch the SS signal
    case U8X8_MSG_BYTE_START_TRANSFER:
      OLED_CS_Write(u8x8->display_info->chip_enable_level);
      u8x8->gpio_and_delay_cb(u8x8, U8X8_MSG_DELAY_NANO, u8x8->display_info->post_chip_enable_wait_ns, NULL); //delay
      break;
      
    case U8X8_MSG_BYTE_END_TRANSFER: 
      u8x8->gpio_and_delay_cb(u8x8, U8X8_MSG_DELAY_NANO, u8x8->display_info->pre_chip_disable_wait_ns, NULL);
      OLED_CS_Write(u8x8->display_info->chip_disable_level); 
      CyDelayUs(10); //debug
      break;
      
    default:
      return 0;
    }//switch message
    return 1;
    
}//psoc_byte_4wire_sw_spi


uint8_t psoc_gpio_and_delay_psoc(u8x8_t *u8x8, uint8_t msg, uint8_t arg_int, void *arg_ptr)
{
    (void)u8x8; //not used to cast to a void to prevent editor warning
    (void)arg_ptr;
    switch(msg)
    {
        case U8X8_MSG_GPIO_AND_DELAY_INIT: //called once. Could be used to init
        break;
        case U8X8_MSG_DELAY_MILLI:
            CyDelay(arg_int);
        break;
        case U8X8_MSG_DELAY_10MICRO:
            CyDelayUs(10);
        break;
        case U8X8_MSG_DELAY_100NANO:
            CyDelayCycles((CYDEV_BCLK__SYSCLK__HZ/1000000) * 100 - 16); //PSoC system reference guide says ~16 extra cycles 
        break;
    }
    return 1;
}//psoc_gpio_and_delay_psoc


void write_oled(char* msg)
{
  
  u8g2_FirstPage(&u8g2);
  do {
    /* all graphics commands have to appear within the loop body. */    
    //u8g2_SetFont(u8g2_font_ncenB14_tr);
    u8g2_DrawStr(&u8g2, 0,20, msg);
  }while ( u8g2_NextPage(&u8g2) );

}

/* [] END OF FILE */


