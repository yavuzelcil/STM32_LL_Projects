/*----------------------------------------------------------------------------
	MCT-Praktikumsversuch V5 Nucleo
	Bonbonautomat
	Mechatronik mit Bibliotheken
	
	04/2022
 *----------------------------------------------------------------------------*/

// Hier die Werte auf dem Aufkleber des benutzen Bonbonautomaten einsetzen
#include <stdio.h>
#include <stm32f4xx_ll_bus.h>
#include <stm32f4xx_ll_tim.h>
#include <stm32f4xx_ll_gpio.h>
#define SERVO_MIN   1.08	/* Wert hier eintragen*/			// minimale Position des Servo in ms
#define SERVO_MAX   1.58	/* Wert hier eintragen*/			// maximale Position des Servo in ms
#include "ADC.h"
#include "TIM3_PWM.h"

void DelayMs(int ms)
// wartet ms Millisekunden unter Verwendung von Timer6
{
	if (ms==0)																							// wenn Wartezeit=0, dann sofort zurueck
		return;
	
	// Timer initialisieren
	LL_APB1_GRP1_EnableClock(LL_APB1_GRP1_PERIPH_TIM6);			// Tim6 Takt einschalten
		
	LL_TIM_SetPrescaler(TIM6,SystemCoreClock/1000-1);				// Prescaler für 1kHz = 1ms
	LL_TIM_SetAutoReload(TIM6,ms);													// Wartezeit in Auto-Reload-Register
	LL_TIM_SetCounter(TIM6,0);															// Zähler auf 0 zurücksetzen
	LL_TIM_EnableCounter(TIM6);															// Zähler starten
		
	while (LL_TIM_GetCounter(TIM6)<ms)
		;
	
	LL_TIM_DisableCounter(TIM6);
}


int main(void)
{
	
	// Initialisierungen
	// ...
	//Taster init
	LL_AHB1_GRP1_EnableClock(LL_AHB1_GRP1_PERIPH_GPIOC);
	LL_GPIO_SetPinMode(GPIOC, LL_GPIO_PIN_12,LL_GPIO_MODE_INPUT);
	
	ADCInit(1);
	TIM3InitPWM();
	
	int x = 0;
	int y = 0;
	
	while (1)
	{
		// ... (Funktion)
		y = LL_GPIO_ReadInputPort(GPIOC);
		x = ADCRead();
		
		if ((y & 0x00001000) == 0) {
			TIM3RGB(0, 0, 255);
			for(int i = 0; i < 101; i++){
				TIM3Servo(i);
				DelayMs(10);
			}
			for(int i = 100; i >= 0; i--){
				TIM3Servo(i);
				DelayMs(10);
			}
		}
		
		
		else if(x <= 0x300) {
			TIM3RGB(255, 0, 0);
		} else {
			TIM3RGB(0, 255, 0);
		}
		//DelayMs(x);
		
	}
}
