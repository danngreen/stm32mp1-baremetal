#pragma once
#include "drivers/stm32xx.h"

namespace mdrivlib
{

struct System {
	System() = delete;

	static void SetVectorTable(uint32_t reset_address) {
		SCB->VTOR = reset_address & (uint32_t)0x1FFFFF80;
	}
};

struct SystemClocks {
	SystemClocks() = delete;

	static void init_clocks(const RCC_OscInitTypeDef &osc_def,
							const RCC_ClkInitTypeDef &clk_def,
							const RCC_PeriphCLKInitTypeDef &pclk_def,
							const uint32_t systick_freq_hz = 1000) {
		poweron();

		// Todo: const_cast or fix HAL_RCC_ functions to take a const struct
		RCC_OscInitTypeDef osc_def_ = osc_def;
		HAL_RCC_OscConfig(&osc_def_);

		HAL_PWREx_EnableOverDrive();

		RCC_ClkInitTypeDef clk_def_ = clk_def;
		HAL_RCC_ClockConfig(&clk_def_, FLASH_LATENCY_7);

		RCC_PeriphCLKInitTypeDef pclk_def_ = pclk_def;
		HAL_RCCEx_PeriphCLKConfig(&pclk_def_);

		HAL_RCC_EnableCSS();

		HAL_SYSTICK_Config(HAL_RCC_GetHCLKFreq() / systick_freq_hz);
		HAL_SYSTICK_CLKSourceConfig(SYSTICK_CLKSOURCE_HCLK);
	}

	static void poweron() {
		__HAL_RCC_PWR_CLK_ENABLE();
		__HAL_RCC_SYSCFG_CLK_ENABLE();
		__HAL_PWR_VOLTAGESCALING_CONFIG(PWR_REGULATOR_VOLTAGE_SCALE1);
	}
};

} // namespace mdrivlib
