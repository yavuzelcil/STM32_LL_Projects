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
#include "DAC.h"
#include <math.h>
#include "USART2.h"

#define A 1850u //amplitude
#define K 2048u //koeficient
#define T 32.0f //period
#define PI 3.14 //pi constant



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
	USART2Init();
	USART2WriteChar('h');
	USART2WriteString("Best program ever");
	DACInit(); 
	int d = 1;
	int a = 1;
	
	while(1){
		for(int i = 1; i <= 32; i++)
		{
			float f = i / T; //frequency
      // x = k + A sin(2pf)
      //printf("%d. sinus wert = %d\n", i, (int)(K + A * sin(2 * PI * f)));
			int wert = K + (A / a) * sin(2 * PI * f);
			DACWrite(wert);
			DelayMs(d);
		}
		if(USART2CmdFlag)
		{
			sscanf(USART2InputBuffer, "d%da%d", &d, &a);
			USART2CmdFlag = 0;
			
		}
		
	}



}
