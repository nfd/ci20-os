#define EXTCLK 48000000 /* 48 MHz external clock driving the serial line */

/* CPU and DDR will be configured to use the same clock, so CPU speed divided
 * by DDR speed must be an integer. */
#define CPU_SPEED_HZ     1200000000 /* 1.2 GHz */
#define DDR_SPEED_HZ      400000000 /* 400 MHz */

/* Pin names: GPIO bank C, mode 2 */
#define PIN_UART4_TXD (1 << 10)
#define PIN_UART4_RXD (1 << 20)

/* Pin names: GPIO bank F */
#define PIN_LED_PIN   (1 << 15)

/* We prescale the OS timer by 16, which, with a 48MHz EXTCLK, means our OS
 * timer increments 3000000 times a second.
*/

#define OS_TIMER_HZ (EXTCLK / 16)

/* Multiply a value in usec by this to get the number of ticks to wait for
 * the desired amount of usecs. For this to be at all meaningful, OS_TIMER_HZ
 * must be an integer multiple of 1000000. */
#define OS_TIMER_USEC_DIV (OS_TIMER_HZ / 1000000)
