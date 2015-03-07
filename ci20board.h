#define EXTCLK 48000000 /* 48 MHz external clock driving the serial line */

/* Peripherals */
#define CPM_BASE   0xb0000000
#define TCU_BASE   0xb0002000
#define GPIO_BASE  0xb0010000
#define UART0_BASE 0xb0030000
#define UART1_BASE 0xb0031000
#define UART2_BASE 0xb0032000
#define UART3_BASE 0xb0033000
#define MSC0_BASE  0xb3450000
#define MSC1_BASE  0xb3460000
#define MSC2_BASE  0xb3470000

/* GPIOs */

#define GPIO_BANK_A 0x0
#define GPIO_BANK_B 0x100
#define GPIO_BANK_C 0x200
#define GPIO_BANK_D 0x300
#define GPIO_BANK_E 0x400
#define GPIO_BANK_F 0x500

#define GPIO_INTC   0x18
#define GPIO_MSKS   0x24
#define GPIO_MSKC   0x28
#define GPIO_PAT1S  0x34
#define GPIO_PAT1C  0x38
#define GPIO_PAT0S  0x44
#define GPIO_PAT0C  0x48
#define GPIO_PEC    0x78

#define GPIO(bank, name) (GPIO_BASE + (GPIO_BANK_##bank + GPIO_##name))

/* Clock */
#define CPM_CPCCR         (CPM_BASE)
#define CPM_CLKGR0        (CPM_BASE + 0x20)
#define CPM_CLKGR1        (CPM_BASE + 0x28)
#define CPM_CLKGR1_UART4  (1 << 10)

/* Pin names: GPIO bank C, mode 2 */
#define PIN_UART4_TXD (1 << 10)
#define PIN_UART4_RXD (1 << 20)

/* Pin names: GPIO bank F */
#define PIN_LED_PIN   (1 << 15)

