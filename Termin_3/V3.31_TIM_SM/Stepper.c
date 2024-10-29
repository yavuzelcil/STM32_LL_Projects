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

void SysTickInit (int freq)
// initialisiert SysTick für freq Hz und startet ihn
{
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
	SysTickInit(100);
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
	AktPos = LL_GPIO_ReadOutputPort(GPIOB) & 0x0F;
	if(StepDir)
	{
		if(AktPos == 0x0)
			ZielPos = 0x3;
		else if (AktPos == 0x3 && StepDir == StepMode_BWD)
			ZielPos = 0x9;
		else if (AktPos == 0x9 && StepDir == StepMode_BWD)
			ZielPos = 0xc;
		else if (AktPos == 0xc && StepDir == StepMode_FWD)
			ZielPos = 0x9;
		else if (AktPos == 0x9 && StepDir == StepMode_FWD)
			ZielPos = 0x3;
		else 
			if(StepDir == StepMode_BWD)
				ZielPos = AktPos >> 1;
			else
				ZielPos = AktPos << 1;
			
		StepOut(ZielPos);
	}
}
