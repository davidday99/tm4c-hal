
#include "system.h"
#include "ssi.h"
#include "gpio.h"
#include "tm4c123gh6pm.h"
#include "st7735.h"

extern void EnableInterrupts();
extern void DisableInterrupts();
extern void StartCritical();
extern void EndCritical();

void Delay(void){unsigned long volatile time;
  time = 8000000;
  while(time){
		time--;
  }
}

int main(void){
  uint8_t delay = 0;

  DisableInterrupts();
  
  PLL_init();
  
  // enable SSI0
  //SYSCTL_RCGCSSI_R |= 1;

  /**
   * Enable the following pins:
   * PA2 = SSI0Clk
   * PA3 = SSI0Fss
   * PA4 = SSI0Rx
   * PA5 = SSI0Tx
   **/
  // enable PA clock
  // init_port_clock(PORTA);
  // delay++;
  // init_pin_as_output(PORTA, PIN2);
  // enable_pin_digital(PORTA, PIN2);


  // set_gpio_output_pin_high(PORTA, PIN2);
  // Delay();
  // set_gpio_output_pin_low(PORTA, PIN2);
  
  //init_SSI(0, 1, 100, 0, 0, 8);

  Output_Init();
  ST7735_OutString("Hello, world!\n");
  Delay();
  ST7735_OutString("Checking diagnostics\n");
  Delay();

  while (1) {
    ST7735_OutString(".");
    Delay();
    ST7735_OutString(".");
    Delay();
    ST7735_OutString(".");
    Delay();
    ST7735_OutChar(8);
    ST7735_OutChar(8);
    ST7735_OutChar(8);
  }
}

// Color    LED(s) PortF
// dark     ---    0
// red      R--    0x02
// blue     --B    0x04
// green    -G-    0x08
// yellow   RG-    0x0A
// sky blue -GB    0x0C
// white    RGB    0x0E
// pink     R-B    0x06
