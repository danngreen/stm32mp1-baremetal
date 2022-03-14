#pragma once
#include "stm32mp1xx.h"
#include "stm32mp1xx_ll_gpio.h"

#define DEBUG2_OFF LL_GPIO_ResetOutputPin(GPIOA, (1<<12));
#define DEBUG2_ON LL_GPIO_SetOutputPin(GPIOA, (1<<12));
