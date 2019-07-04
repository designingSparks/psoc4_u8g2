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
#include "utilities.h"

void dbg_printf(char const * format, ...)      
{
	uint8_t len = 0, i=0;	
	va_list  ap;
	
	if(!format)
		return;
	
	va_start(ap,format);
	len = vsprintf((char *)s_formatBuffer,format,ap);
	va_end(ap);
	
	if(len > MAX_FORMAT_BUFFER_SIZE)
		len = MAX_FORMAT_BUFFER_SIZE; //Throw an error if len is too long
	
  UART_1_UartPutString((char *)s_formatBuffer);
}
  
/* [] END OF FILE */
