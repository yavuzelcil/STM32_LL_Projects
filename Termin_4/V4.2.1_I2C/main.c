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
#include "USART2.h"
#include "I2C.h"
#include "Temperatur.h"
#include <string.h>
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
	
	USART2Init();
	USART2WriteString("Yavuz und Haris s temperatur sensor\n");
	
	I2CInit(0x08);
	LCDInit();
	SysTickInit(1000);
	
	char* cmd;
	float temp = 0;
	int value = -1;
	char buffer[32];
	LCDClear();
	LCDDefineDegree();
	LCDSendData(1);
	
	while(1)
	{	
		if (Ticks >= 1000)
		{
			temp = TempRead() / 16.0;
			sprintf(buffer, "Temp: %.2f%cC", temp, 0x1);
			LCDClear();
			LCDSendString(buffer);
			Ticks = 0;
		}	
		
		if (USART2CmdFlag)
		{
			sscanf(USART2InputBuffer, "%c%c%x", &cmd[0], &cmd[1], &value);
			USART2CmdFlag = 0;	
			
			if (cmd[0] == 't' && cmd[1] == 'r')
			{
				USART2WriteString("Reading temperature:\n");
				temp = TempRead()/16.0;
				sprintf(buffer, "Temp: %.2f%cC", temp, 0x1);
				USART2WriteString(buffer);
				LCDClear();
				LCDSendString(buffer);
			}
			else if (cmd[0] == 't' && cmd[1] == 's')
			{
				USART2WriteString("Setting resolution:\n");
				if (value < 4 && value >=0)
				{
					TempSetRes(value);
					sprintf(buffer, "Resolution: %d\r\n", value);
					USART2WriteString(buffer);
				}
				else
				{	
					USART2WriteString("Can't set resolution, wrong value!\n");
				}
			}
			else if (cmd[0] == 'l' && cmd[1] == 'c')
			{

				LCDSendCmd(value);
			
			}
			else if (cmd[0] == 'l' && cmd[1] == 'd')
			{
				LCDSendData(value);
			}
			else
			{
				USART2WriteString("Invalid cmd\n");
			}
		}
	}
}


