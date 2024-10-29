/*----------------------------------------------------------------------------
  Funktionen für Schrittmotor mit LL-Library
	
	06/2020
 *----------------------------------------------------------------------------*/

#include <stm32f4xx_ll_bus.h>
#include <stm32f4xx_ll_gpio.h>
#include <stm32f4xx_ll_utils.h>
#include <stm32f4xx_ll_cortex.h>
#include "stepper.h"


int StepDir = 0;  						// Richtung des Schrittmotors
int AktPos=0;									// aktuelle Position des Schrittmotors
int ZielPos=0;								// Zielposition des Schrittmotors
int StepMode=StepMode_NONE;		// Modus des Schrittmotors
int Ticks=0;									// Zähler, der bei jedem Systick-Aufruf erhöht wird
int pattern[8] = {3, 2, 6, 4, 12, 8, 9, 1};
int tempcount=0;


void SysTickInit (int freq)
// initialisiert SysTick für freq Hz und startet ihn
{
	LL_InitTick(SystemCoreClock,freq);			// SysTick initialisieren und starten
	LL_SYSTICK_EnableIT();					// Interrupt aktivieren
}

void StepperInit(void)
// initialisiert Schrittmotorausgänge GPIOB0...3 und Referenzeingang GPIOB4
{
	// ... (Ausgänge konfigurieren)
	LL_GPIO_SetPinSpeed(GPIOB,LL_GPIO_PIN_0,LL_GPIO_SPEED_FREQ_LOW);
	LL_GPIO_SetPinMode(GPIOB,LL_GPIO_PIN_0,LL_GPIO_MODE_OUTPUT);
	LL_GPIO_SetAFPin_0_7(GPIOB,LL_GPIO_PIN_0,LL_GPIO_AF_2);
	
	LL_GPIO_SetPinSpeed(GPIOB,LL_GPIO_PIN_1,LL_GPIO_SPEED_FREQ_LOW);
	LL_GPIO_SetPinMode(GPIOB,LL_GPIO_PIN_1,LL_GPIO_MODE_OUTPUT);
	LL_GPIO_SetAFPin_0_7(GPIOB,LL_GPIO_PIN_1,LL_GPIO_AF_2);
	
	LL_GPIO_SetPinSpeed(GPIOB,LL_GPIO_PIN_2,LL_GPIO_SPEED_FREQ_LOW);
	LL_GPIO_SetPinMode(GPIOB,LL_GPIO_PIN_2,LL_GPIO_MODE_OUTPUT);
	LL_GPIO_SetAFPin_0_7(GPIOB,LL_GPIO_PIN_2,LL_GPIO_AF_2);
	
	LL_GPIO_SetPinSpeed(GPIOB,LL_GPIO_PIN_3,LL_GPIO_SPEED_FREQ_LOW);
	LL_GPIO_SetPinMode(GPIOB,LL_GPIO_PIN_3,LL_GPIO_MODE_OUTPUT);
	LL_GPIO_SetAFPin_0_7(GPIOB,LL_GPIO_PIN_3,LL_GPIO_AF_2);
	
	LL_GPIO_SetPinSpeed(GPIOB,LL_GPIO_PIN_4,LL_GPIO_SPEED_FREQ_LOW);
	LL_GPIO_SetPinMode(GPIOB,LL_GPIO_PIN_4,LL_GPIO_MODE_INPUT);
	LL_GPIO_SetAFPin_0_7(GPIOB,LL_GPIO_PIN_4,LL_GPIO_AF_2);
	
	// ... (SysTick initialisieren, ca. 300 Hz)
	SysTickInit(300);
	
}

void StepOut(int StepPattern)
// gibt das Schrittmotormuster StepPattern an Motor aus
{	
	// ... (Ausgabe an die unteren vier Bit von Port B
	LL_GPIO_ResetOutputPin(GPIOB, LL_GPIO_PIN_0);
	LL_GPIO_ResetOutputPin(GPIOB, LL_GPIO_PIN_1);
	LL_GPIO_ResetOutputPin(GPIOB, LL_GPIO_PIN_2);
	LL_GPIO_ResetOutputPin(GPIOB, LL_GPIO_PIN_3);
	
	if (StepPattern == 3 || StepPattern == 9 || StepPattern == 1){
		LL_GPIO_SetOutputPin(GPIOB, LL_GPIO_PIN_0);
	} 
	if (StepPattern == 3 || StepPattern == 2 || StepPattern == 6){
		LL_GPIO_SetOutputPin(GPIOB, LL_GPIO_PIN_1);
	}
	if (StepPattern == 6 || StepPattern == 4 || StepPattern == 12){
		LL_GPIO_SetOutputPin(GPIOB, LL_GPIO_PIN_2);
	}
	if (StepPattern == 12 || StepPattern == 8 || StepPattern == 9){
		LL_GPIO_SetOutputPin(GPIOB, LL_GPIO_PIN_3);
	}
}

/*----------------------------------------------------------------------------
  Interrupt-Handler SysTick
 *----------------------------------------------------------------------------*/
void SysTick_Handler (void)
{
	// ...
	tempcount++; 
	if (StepDir == 0) {
		return;
	} else if (StepDir == 1) {
		if (Ticks >= 8){
			Ticks = 0;
		}
		StepOut(pattern[Ticks]);
		Ticks++;
	} else if (StepDir == 2) {
		if (Ticks < 0) {
			Ticks = 7;
		}
		StepOut(pattern[Ticks]);
		Ticks--;
	} else if (StepDir == 3) {
		if (ZielPos > AktPos){
			if (Ticks >= 8){
				Ticks = 0;
			}
			StepOut(pattern[Ticks]);
			Ticks++;
			AktPos++;
		}
		if (ZielPos < AktPos){
			if (Ticks < 0) {
			Ticks = 7;
		}
		StepOut(pattern[Ticks]);
		Ticks--;
		AktPos--;
		}
	}
	
	
	
}
