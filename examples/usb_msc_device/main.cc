//#include "drivers/interrupt.hh"
#include "hardware.h"

#include "drivers/leds.hh"
#include "drivers/uart.hh"
#include "irq_ctrl.h"
#include "stm32mp1xx.h"
//#include "system_clk.hh"
#include "usbd_core.h"
#include "usbd_desc.h"
#include "usbd_msc_storage.h"
#include <cstdint>

// Uncomment these to use the OSD32BRK board:
// #include "osd32brk_conf.hh"
// using namespace OSD32BRK;

// Uncomment these to use the STM32MP1Disco board:
// #include "stm32disco_conf.hh"
// using namespace STM32MP1Disco;

using GreenLED = Led<GPIOG_BASE, 9, LedActive::High>;
using GreenLED2 = Led<GPIOA_BASE, 11, LedActive::High>;
using GreenLED3 = Led<GPIOA_BASE, 12, LedActive::High>;

extern PCD_HandleTypeDef hpcd_USB_OTG;
void main()
{
	Uart<UART4_BASE> uart;
	uart.write("\r\n\r\nSimple USB MSC Device test\r\n");
	uart.write("Connect a USB cable to a computer\r\n");
	uart.write("You should see a 128MB unformatted drive appear.\r\n");

	GreenLED green1;
	GreenLED2 green2;
	GreenLED3 green3;

	green1.on();
	green1.off();
	green1.on();
	green1.off();

	green2.on();
	green2.off();
	green2.on();
	green2.off();

	green3.on();
	green3.off();
	green3.on();
	green3.off();

	// SystemClocks::init();

	USBD_HandleTypeDef USBD_Device;

	__disable_irq();
	cpu_initialize(); // в случае ARM - инициализация прерываний и контроллеров, AVR - запрет JTAG
	hardware_timer_initialize(TICKS_FREQUENCY);

	// board_usb_initialize():
	usbd_descriptors_initialize(1);

	auto init_ok = USBD_InitComposite(&USBD_Device);
	if (init_ok != USBD_OK) {
		uart.write("USB Device failed to initialize!\r\n");
		uart.write("Error code: ");
		uart.write(static_cast<int>(init_ok));
	}

	// InterruptManager::registerISR(OTG_IRQn, std::bind_front(HAL_PCD_IRQHandler, &hpcd));
	// InterruptManager::registerISR(OTG_IRQn, [&green2] {
	// 	green2.on();
	// 	HAL_PCD_IRQHandler(&hpcd_USB_OTG);
	// 	green2.off();
	// });
	// GIC_DisableIRQ(OTG_IRQn);
	// GIC_SetTarget(OTG_IRQn, 1);
	// GIC_SetPriority(OTG_IRQn, 0b01111000);
	// GIC_SetConfiguration(OTG_IRQn, 0b10); // Edge triggered

	USBD_AddClass(&USBD_Device, USBD_MSC_CLASS);
	// USBD_MSC_RegisterStorage(&USBD_Device, &USBD_MSC_fops);

	// GIC_EnableIRQ(OTG_IRQn);

	__enable_irq();
	USBD_Start(&USBD_Device); // board_usb_activate()

	// Blink green1 light at 1Hz
	uint32_t last_tm = 0;
	bool led_state = false;
	while (1) {

		// // Check for new USB events
		// HAL_PCD_IRQHandler(&hpcd);

		uint32_t tm = HAL_GetTick();
		if (tm > (last_tm + 500)) {
			last_tm = tm;
			if (led_state) {
				green1.off();
			} else {
				green1.on();
			}
			led_state = !led_state;
		}
	}
}
