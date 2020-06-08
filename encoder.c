/* ========================================
 *
 * Concept: https://www.best-microcontroller-projects.com/rotary-encoder.html
 *
 * ========================================
*/

#include "encoder.h"
#include "utilities.h"  //dbg_printf

#define READ_ENCA 0 //((encA_PS & encA_MASK) >> encA_SHIFT)
#define READ_ENCB 0 //((encB_PS & encB_MASK) >> encB_SHIFT)

uint8_t out1, out2, out3;
uint8_t prevNextCode = 0;
uint16_t store = 0;
static int8_t c, val;

void updateEncoder()
{
  val = read_rotary();
  if(val) {
      c += val;
      dbg_printf("Count: %d\n", c);
  }
}


// A vald CW or  CCW move returns 1, invalid returns 0.
int8_t read_rotary() {
  static int8_t rot_enc_table[] = {0,1,1,0,1,0,0,1,1,0,0,1,0,1,1,0};

  prevNextCode <<= 2;
  if (READ_ENCB) prevNextCode |= 0x02;
  if (READ_ENCA) prevNextCode |= 0x01;
  prevNextCode &= 0x0f;

   // If valid then store as 16 bit data.
   if  (rot_enc_table[prevNextCode] ) {
      store <<= 4;
      store |= prevNextCode;
      //if (store==0xd42b) return 1;
      //if (store==0xe817) return -1;
      if ((store&0xff)==0x2b) return -1;
      if ((store&0xff)==0x17) return 1;
   }
   return 0;
}

/* [] END OF FILE */
