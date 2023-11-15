/*----------------------------------------------------------------------------
  Funktionen für PWM (Servo und RGB-LED)
	
	06/2020
 *----------------------------------------------------------------------------*/

#include <stm32f4xx_ll_bus.h>
#include <stm32f4xx_ll_gpio.h>
#include <stm32f4xx_ll_tim.h>

void TIM3InitPWM(void)
// Initialisierung des Timer 3 für PWM (Servo + RGB-LED)
{
	LL_AHB1_GRP1_EnableClock(LL_AHB1_GRP1_PERIPH_GPIOA);			// GPIOA Takt einschalten
	LL_AHB1_GRP1_EnableClock(LL_AHB1_GRP1_PERIPH_GPIOC);			// GPIOC Takt einschalten

	// PA6 (Servo) initialisieren
	LL_GPIO_SetPinSpeed(GPIOA,LL_GPIO_PIN_6,LL_GPIO_SPEED_FREQ_LOW);
	LL_GPIO_SetPinMode(GPIOA,LL_GPIO_PIN_6,LL_GPIO_MODE_ALTERNATE);
	LL_GPIO_SetAFPin_0_7(GPIOA,LL_GPIO_PIN_6,LL_GPIO_AF_2);

	// PA7 (LED rot) initialisieren
	LL_GPIO_SetPinSpeed(GPIOA,LL_GPIO_PIN_7,LL_GPIO_SPEED_FREQ_LOW);
	LL_GPIO_SetPinMode(GPIOA,LL_GPIO_PIN_7,LL_GPIO_MODE_ALTERNATE);
	LL_GPIO_SetAFPin_0_7(GPIOA,LL_GPIO_PIN_7,LL_GPIO_AF_2);

	// PC8 (LED grün) initialisieren
	LL_GPIO_SetPinSpeed(GPIOC,LL_GPIO_PIN_8,LL_GPIO_SPEED_FREQ_LOW);
	LL_GPIO_SetPinMode(GPIOC,LL_GPIO_PIN_8,LL_GPIO_MODE_ALTERNATE);
	LL_GPIO_SetAFPin_8_15(GPIOC,LL_GPIO_PIN_8,LL_GPIO_AF_2);

	// PC9 (LED blau) initialisieren
	LL_GPIO_SetPinSpeed(GPIOC,LL_GPIO_PIN_9,LL_GPIO_SPEED_FREQ_LOW);
	LL_GPIO_SetPinMode(GPIOC,LL_GPIO_PIN_9,LL_GPIO_MODE_ALTERNATE);
	LL_GPIO_SetAFPin_8_15(GPIOC,LL_GPIO_PIN_9,LL_GPIO_AF_2);

	// Timer initialisieren
	// ...

	// Kanal 1 (Servo)
	// ...

	// Kanal 2...4 (RGB-LED)
	// ...
	
	// Timer starten
	// ...
}

void TIM3Servo (int pos)
// Servo auf Position fahren (0...100)
{
  if (pos <0 || pos >100)														// Abbruch, weil ausserhalb des Wertebereichs
		return;
	
	// ...
}

void TIM3RGB(int red, int green, int blue)
// RGB-LED-Helligkeit setzen (0...255)
{
	if ((red >=0) && (red <=255))
		; // ...
	if ((green >=0) && (green <=255))
		; // ...
	if ((blue >=0) && (blue <=255))
		; // ...
}
