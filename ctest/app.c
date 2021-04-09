#include "stm32mp157cxx_ca7.h"
#include "stm32mp1xx_ll_gpio.h"

#include <stdint.h>

#define UART4_TDR 0x40010028
volatile uint8_t *uart0 = (uint8_t *)UART4_TDR;
const uint32_t *UART4_ISR = (uint32_t *)0x40010028;
const uint32_t TXFE = (1 << 23); // FIFO Empty
const uint32_t TXFNF = (1 << 7); // TX FIFO Not Full
const uint32_t TC = (1 << 6);    // Tx Complete

// what is this?
// #define GPIO1 0x4804C000
// #define GPIO_OE 0x134
/* .equ CM_PER, 0x44E00000 */
/* .equ CM_PER_GPIO1_CLKCTRL, 0xAC */
/* .equ GPIO_DATAOUT, 0x13C */

/* GPIO I, pin 8 = red D2 */
/* GPIO I, pin 9 = green D2 */
/* GPIO Z, pin 6 = red D1 */
/* GPIO Z, pin 7 = green D1 */
/* all LEDs are active low */

void delay_for_uart(void) {
  int i = 255;
  while (i--)
    ;
}

uint32_t uart_ready() {
  uint32_t isr = (*UART4_ISR);
  if (isr & TC)
    return 1;
  else
    return 0;
}

void write(const char *str) {
  while (*str) {
    *uart0 = *str++;
    delay_for_uart();
  }
}

int main() {
  RCC->MC_AHB4ENSETR |= RCC_MC_AHB4ENSETR_GPIOIEN;
  RCC->MC_AHB5ENSETR |= RCC_MC_AHB5ENSETR_GPIOZEN;

  LL_GPIO_SetPinMode(GPIOI, LL_GPIO_PIN_8, LL_GPIO_MODE_OUTPUT);
  LL_GPIO_SetPinOutputType(GPIOI, LL_GPIO_PIN_8, LL_GPIO_OUTPUT_PUSHPULL);
  LL_GPIO_SetPinSpeed(GPIOI, LL_GPIO_PIN_8, LL_GPIO_SPEED_FREQ_MEDIUM);

  LL_GPIO_SetPinMode(GPIOI, LL_GPIO_PIN_9, LL_GPIO_MODE_OUTPUT);
  LL_GPIO_SetPinOutputType(GPIOI, LL_GPIO_PIN_9, LL_GPIO_OUTPUT_PUSHPULL);
  LL_GPIO_SetPinSpeed(GPIOI, LL_GPIO_PIN_9, LL_GPIO_SPEED_FREQ_MEDIUM);

  LL_GPIO_SetOutputPin(GPIOI, LL_GPIO_PIN_8);
  LL_GPIO_ResetOutputPin(GPIOI, LL_GPIO_PIN_8);
  LL_GPIO_SetOutputPin(GPIOI, LL_GPIO_PIN_9);
  LL_GPIO_ResetOutputPin(GPIOI, LL_GPIO_PIN_9);

  LL_GPIO_SetOutputPin(GPIOZ, LL_GPIO_PIN_6);
  LL_GPIO_ResetOutputPin(GPIOZ, LL_GPIO_PIN_6);
  LL_GPIO_SetOutputPin(GPIOZ, LL_GPIO_PIN_7);
  LL_GPIO_ResetOutputPin(GPIOZ, LL_GPIO_PIN_7);

  *uart0 = 'X';
  *uart0 = 'Y';
  *uart0 = 'Z';
  *uart0 = '\r';
  *uart0 = '\n';

  const char *s = "Hello world from bare-metal!\r\n";
  write(s);

  const char *s2 = "And hi to you too!\r\n";
  while (*s2 != '\0') {
    *uart0 = *s2;
    s2++;
    delay_for_uart();
  }

  while (1) {
  };

  return 0;
}

void SystemInit(void) { return; }
