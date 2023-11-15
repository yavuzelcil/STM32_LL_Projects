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
#include "ESP_USART3.h"
#include "TIM3_PWM.h"
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
	USART2WriteString("Yavuz und Haris s WLAN\n");
	TIM3InitPWM();
	I2CInit(0x08);
	LCDInit();
	StepperInit();
	//SysTickInit(1000);
	Start_ESP_Wifi(1,12345678);
	
	char cmd;
	int value = -1;
	char buffer[32];
	
	int red=0;
	int green=0;
	int blue=0;
	int servo_value = 50;
	int stepper_value = 200;
	
	while(1)
	{		
		if (USART2CmdFlag){
			if(USART2InputBuffer[0] == 's')
			{
			  sscanf(USART2InputBuffer, "%c%d", &cmd, &value);
				TIM3Servo(value);
			}
			else if(USART2InputBuffer[0] == 'r')
			{
				sscanf(USART2InputBuffer, "r%d""g%d""b%d", &red,&green,&blue);
				TIM3RGB(red,green,blue);
			}
			else if(USART2InputBuffer[0] == 'm')
			{
			  sscanf(USART2InputBuffer, "%c%d", &cmd, &value);
				ZielPos = value;
				if (ZielPos > AktPos)
				{
					StepDir = StepMode_FWD;
				}
				else if (ZielPos < AktPos)
				{
					StepDir = StepMode_BWD;
				}
				StepMode = StepMode_POS;
			}
			USART2CmdFlag = 0;
		}
	}
}


