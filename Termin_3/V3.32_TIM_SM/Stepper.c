/*----------------------------------------------------------------------------
  Funktionen für Schrittmotor mit LL-Library
	
	06/2020
 *----------------------------------------------------------------------------*/

#include <stm32f4xx_ll_bus.h>
#include <stm32f4xx_ll_gpio.h>
#include <stm32f4xx_ll_utils.h>
#include <stm32f4xx_ll_cortex.h>
#include "stepper.h"
#include "USART2.h"

int StepDir = 0;  						// Richtung des Schrittmotors
int AktPos=0;									// aktuelle Position des Schrittmotors
int ZielPos=0;								// Zielposition des Schrittmotors
int StepMode=StepMode_NONE;		// Modus des Schrittmotors
int Ticks=0;									// Zähler, der bei jedem Systick-Aufruf erhöht wird
int cw[] = {0x3, 0x2, 0x6, 0x4, 0xC, 0x8, 0x9, 0x1};

void SysTickInit (int freq)
// initialisiert SysTick für freq Hz und startet ihn
{
	AktPos = 0;
	ZielPos = 0;
	Ticks = 0;
	LL_InitTick(SystemCoreClock,freq);			// SysTick initialisieren und starten
	LL_SYSTICK_EnableIT();					// Interrupt aktivieren
}

void StepperInit(void)
// initialisiert Schrittmotorausgänge GPIOB0...3 und Referenzeingang GPIOB4
{
	LL_AHB1_GRP1_EnableClock(LL_AHB1_GRP1_PERIPH_GPIOB);
	// ... (Ausgänge konfigurieren)
	LL_GPIO_SetPinMode(GPIOB,LL_GPIO_PIN_0,LL_GPIO_MODE_OUTPUT);
	LL_GPIO_SetPinMode(GPIOB,LL_GPIO_PIN_1,LL_GPIO_MODE_OUTPUT);
	LL_GPIO_SetPinMode(GPIOB,LL_GPIO_PIN_2,LL_GPIO_MODE_OUTPUT);
	LL_GPIO_SetPinMode(GPIOB,LL_GPIO_PIN_3,LL_GPIO_MODE_OUTPUT);
	// Referenzeingang GPIOB4
	LL_GPIO_SetPinMode(GPIOB,LL_GPIO_PIN_4,LL_GPIO_MODE_INPUT);
	// ... (SysTick initialisieren, ca. 300 Hz)
	SysTickInit(200);
}

void StepOut(int StepPattern)
// gibt das Schrittmotormuster StepPattern an Motor aus
{	
	// ... (Ausgabe an die unteren vier Bit von Port B
	 LL_GPIO_WriteOutputPort(GPIOB, (LL_GPIO_ReadOutputPort(GPIOB) & 0xF0) | (StepPattern & 0x0F));
}


/*----------------------------------------------------------------------------
  Interrupt-Handler SysTick
 *----------------------------------------------------------------------------*/
void SysTick_Handler (void)
{ 
	int index;
	if (StepDir == StepMode_FWD) index = Ticks % 8;
	else if (StepDir == StepMode_BWD) index = 7 - (Ticks % 8);
	
	switch (StepMode)
	{
		case StepMode_NONE:
			break;
		case StepMode_POS:
			if(AktPos == ZielPos)
			{
				StepMode = StepMode_NONE;
			}
			else
			{
				if(StepDir == StepMode_FWD)
				{
					Ticks++;
					AktPos++;
					StepOut(cw[index]);
				}
				else if(StepDir == StepMode_BWD)
				{
					Ticks++;
					AktPos--;
					StepOut(cw[index]);
				}
			}
			break;
		case StepMode_REF:
			StepOut(cw[index]);
			AktPos++;
			Ticks++;
			break;
		default:
			break;
	}
}

