/*----------------------------------------------------------------------------
  Funktionen f�r Schrittmotor mit LL-Library
	
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
int Ticks=0;									// Z�hler, der bei jedem Systick-Aufruf erh�ht wird

void SysTickInit (int freq)
// initialisiert SysTick f�r freq Hz und startet ihn
{
	LL_InitTick(SystemCoreClock,freq);			// SysTick initialisieren und starten
	LL_SYSTICK_EnableIT();					// Interrupt aktivieren
}

void StepperInit(void)
// initialisiert Schrittmotorausg�nge GPIOB0...3 und Referenzeingang GPIOB4
{
	// ... (Ausg�nge konfigurieren)
	
	// ... (SysTick initialisieren, ca. 300 Hz)
}

void StepOut(int StepPattern)
// gibt das Schrittmotormuster StepPattern an Motor aus
{	
	// ... (Ausgabe an die unteren vier Bit von Port B
}

/*----------------------------------------------------------------------------
  Interrupt-Handler SysTick
 *----------------------------------------------------------------------------*/
void SysTick_Handler (void)
{
	// ...
}
