/*----------------------------------------------------------------------------
    MCT-Praktikumsversuche V1...4 Nucleo mit LL-Library

    08/2022
 *----------------------------------------------------------------------------*/

#include <stdio.h>
#include <stm32f4xx_ll_bus.h>
#include <stm32f4xx_ll_tim.h>
#include <stm32f4xx_ll_adc.h>
#include "main.h"
#include "LED.h"
#include "ADC.h"

void DelayMs(int ms)
// wartet ms Millisekunden unter Verwendung von Timer6
{
    if (ms==0)                      // wenn Wartezeit=0, dann sofort zurueck
        return;

    // Timer initialisieren
    // Tim6 Takt einschalten
    LL_APB1_GRP1_EnableClock(LL_APB1_GRP1_PERIPH_TIM6);

    // Prescaler für 1kHz = 1ms
    LL_TIM_SetPrescaler(TIM6,SystemCoreClock/1000-1);
    // Wartezeit in Auto-Reload-Register
    LL_TIM_SetAutoReload(TIM6,ms);
    // Zähler auf 0 zurücksetzen
    LL_TIM_SetCounter(TIM6,0);
    // Zähler starten
    LL_TIM_EnableCounter(TIM6);

    while (LL_TIM_GetCounter(TIM6)<ms);

    LL_TIM_DisableCounter(TIM6);
}

/*----------------------------------------------------------------------------
  Main Program
 *----------------------------------------------------------------------------*/
int main (void)
{

    //------- Init ----------
    // LEDs initialisieren
    LEDInit();
    // ADC Kanal 0 initialisieren
    ADCInit(0);

    // hoechste wert 4095

    int threshold  = 4095;
    int threshold1 = 1 * (threshold / 5);
    int threshold2 = 2 * (threshold / 5);
    int threshold3 = 3 * (threshold / 5);
    int threshold4 = 4 * (threshold / 5);


    while (1) 
    {

        // ... ADC einlesen
        int adc_value = ADCRead();

        // ... an LEDs 4...7 anzeigen


        if (adc_value < threshold1) {
            // LED 4'ü yak (PC0)
            LEDReset(4);
            LEDReset(5);
            LEDReset(6);
            LEDReset(7);
        } else if (adc_value < threshold2) {
            // LED 5'i yak (PC1)
            LEDSet(4);
            LEDReset(5);
            LEDReset(6);
            LEDReset(7);
        } else if (adc_value < threshold3) {
            // LED 5'i yak (PC1)
            LEDSet(4);
            LEDSet(5);
            LEDReset(6);
            LEDReset(7);
        } else if (adc_value < threshold4/*4095*/) {
            // LED 6'yi yak (PC2)
            LEDSet(4);
            LEDSet(5);
            LEDSet(6);
            LEDReset(7);
        } else {/*>>4095*/
            // LED 7'yi yak (PC3)
            LEDSet(4);
            LEDSet(5);
            LEDSet(6);
            LEDSet(7);
        }
    }
}
