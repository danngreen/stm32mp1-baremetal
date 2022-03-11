#include <stdint.h>

static inline void printstr(const char *str)
{
	while (*str) {
		UART4->TDR = *str++;
		while ((UART4->ISR & USART_ISR_TXFT) == 0)
			;
	}
}

static inline void printnum(uint32_t value)
{
	if (!value) {
		printstr("0");
		return;
	}

	char buf[10 + 1];
	int len = 0;
	do {
		const char digit = (char)(value % 10);
		buf[len++] = '0' + digit;
		value /= 10;
	} while (value && (len < 10));
	buf[len] = '\0';

	for (int i = 0; i < len / 2; i++) {
		char tmp = buf[i];
		buf[i] = buf[len - i - 1];
		buf[len - i - 1] = tmp;
	}

	printstr(buf);
}
