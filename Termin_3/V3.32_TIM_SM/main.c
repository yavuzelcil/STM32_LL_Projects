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
#include "Stepper.h"

#define A 1850u //amplitude
#define K 2048u //koeficient
#define T 32.0f //period
#define PI 3.14 //pi constant
#define ROTATION 2048



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
	StepperInit();
	
	char cmd = '0';
	int value = 200;
	
	USART2WriteString("Yavuz und Haris s step-motor\r\r");
	while(1){
		// funktion
		
		if (USART2CmdFlag)
		{
			sscanf(USART2InputBuffer, "%c%d", &cmd, &value);
			USART2CmdFlag = 0;
			switch (cmd)
			{
				case '0':
					USART2WriteString("Turning off stepper motor!\n");
					//turn of StepMode_NONE
					StepMode = StepMode_NONE;
					break;
				case '+':
					USART2WriteString("Rotating cw\n");
					StepDir = StepMode_FWD;
					StepMode = StepMode_REF;
					//cw
					break;
				case '-':
					USART2WriteString("Rotating ccw\n");
					StepDir = StepMode_BWD;
					StepMode = StepMode_REF;
					//ccw
					break;
				case 'm':
					USART2WriteString("Rotating to position\n");
					ZielPos = value;
					char* message;
					sprintf(message, "Rotating to position = %d\n", ZielPos);
					USART2WriteString(message);
					sprintf(message, "Actual position = %d\n", AktPos);
					USART2WriteString(message);
					int distance = ZielPos - AktPos;
					sprintf(message, "Distance = %d\n", distance);
					USART2WriteString(message);
					if (ZielPos > AktPos)
					{
						StepDir = StepMode_FWD;
					}
					else if (ZielPos < AktPos)
					{
						StepDir = StepMode_BWD;
					}
					StepMode = StepMode_POS;
					break;
				default:
					USART2WriteString("Invalid cmd\n");
					break;
			}
		}
	}

}


