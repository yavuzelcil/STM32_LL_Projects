/*----------------------------------------------------------------------------
    MCT-Praktikumsversuche V2 Nucleo mit LL-Library

    10/2023
 *----------------------------------------------------------------------------*/

#include <stdio.h>
#include <stm32f4xx.h>
#include <stm32f4xx_ll_bus.h>
#include <stm32f4xx_ll_tim.h>
#include <stm32f4xx_ll_adc.h>
#include <stm32f4xx_ll_gpio.h>
#include "main.h"
#include <math.h>
#include "USART2.h"
#include "I2C.h"
#include "Temperatur.h"
#include "LCD.h"
#include "LCDInit.h"
#include "Stepper.h"




/*----------------------------------------------------------------------------
  Main Program
 *----------------------------------------------------------------------------*/
 
 void DelayMs(int ms)
// wartet ms Millisekunden unter Verwendung von Timer6
{
    if (ms==0)                      // wenn Wartezeit=0, dann sofort zurueck
        return;

    // Timer initialisieren
    // Tim6 Takt einschalten
    LL_APB1_GRP1_EnableClock(LL_APB1_GRP1_PERIPH_TIM6);

    // Prescaler fuer 1kHz = 1ms
    LL_TIM_SetPrescaler(TIM6,SystemCoreClock/1000-1);
    // Wartezeit in Auto-Reload-Register
    LL_TIM_SetAutoReload(TIM6, ms);
    // Zaehler auf 0 zuruecksetzen
    LL_TIM_SetCounter(TIM6,0);
    // Zaehler starten
    LL_TIM_EnableCounter(TIM6);

    while (LL_TIM_GetCounter(TIM6) < ms);

    LL_TIM_DisableCounter(TIM6);
		
}

	int main (void)
{
	
	I2CInit(0x08);
	LCDInit();
	SysTickInit(1000);
	
	
	int temperature = 0;
	char buffer[20] = "Yavuz und Haris LCD";
	
	LCDClear();
	LCDSendString(buffer);

	while(1){
		// funktion
		 if (Ticks >= 1000) {  
            
            int temperature = TempRead();  
            sprintf(buffer, "Temp: %d\x01C", temperature); 
						LCDClear();
            LCDSendString(buffer); 
			 Ticks = 0; 
		 }
	}

}


