/* TCU, the Timer and Counter Unit peripheral */
#define TCU_BASE   0xb0002000

/* Individual registers */

#define TSTR   (TCU_BASE + 0xF0)
#define TSTSR  (TCU_BASE + 0xF4)
#define TSTCR  (TCU_BASE + 0xF8)
#define TSR    (TCU_BASE + 0x1C)
#define TSSR   (TCU_BASE + 0x2C)
#define TSCR   (TCU_BASE + 0x3C)
#define TER    (TCU_BASE + 0x10)
#define TESR   (TCU_BASE + 0x14)
#define TECR   (TCU_BASE + 0x18)
#define TFR    (TCU_BASE + 0x20)
#define TFSR   (TCU_BASE + 0x24)
#define TFCR   (TCU_BASE + 0x28)
#define TMR    (TCU_BASE + 0x30)
#define TMSR   (TCU_BASE + 0x34)
#define TMCR   (TCU_BASE + 0x38)
#define TDFR0  (TCU_BASE + 0x40)
#define TDHR0  (TCU_BASE + 0x44)
#define TCNT0  (TCU_BASE + 0x48)
#define TCSR0  (TCU_BASE + 0x4C)
#define TDFR1  (TCU_BASE + 0x50)
#define TDHR1  (TCU_BASE + 0x54)
#define TCNT1  (TCU_BASE + 0x58)
#define TCSR1  (TCU_BASE + 0x5C)
#define TDFR2  (TCU_BASE + 0x60)
#define TDHR2  (TCU_BASE + 0x64)
#define TCNT2  (TCU_BASE + 0x68)
#define TCSR2  (TCU_BASE + 0x6C)
#define TDFR3  (TCU_BASE + 0x70)
#define TDHR3  (TCU_BASE + 0x74)
#define TCNT3  (TCU_BASE + 0x78)
#define TCSR3  (TCU_BASE + 0x7C)
#define TDFR4  (TCU_BASE + 0x80)
#define TDHR4  (TCU_BASE + 0x84)
#define TCNT4  (TCU_BASE + 0x88)
#define TCSR4  (TCU_BASE + 0x8C)
#define TDFR5  (TCU_BASE + 0x90)
#define TDHR5  (TCU_BASE + 0x94)
#define TCNT5  (TCU_BASE + 0x98)
#define TCSR5  (TCU_BASE + 0x9C)
#define TDFR6  (TCU_BASE + 0xA0)
#define TDHR6  (TCU_BASE + 0xA4)
#define TCNT6  (TCU_BASE + 0xA8)
#define TCSR6  (TCU_BASE + 0xAC)
#define TDFR7  (TCU_BASE + 0xB0)
#define TDHR7  (TCU_BASE + 0xB4)
#define TCNT7  (TCU_BASE + 0xB8)
#define TCSR7  (TCU_BASE + 0xBC)
#define TCUMOD0  (TCU_BASE + 0x00)
#define TCUMOD3  (TCU_BASE + 0x10)
#define TCUMOD4  (TCU_BASE + 0x20)
#define TCUMOD5  (TCU_BASE + 0x30)
#define TFWD0    (TCU_BASE + 0x04)
#define TFWD3    (TCU_BASE + 0x14)
#define TFWD4    (TCU_BASE + 0x24)
#define TFWD5    (TCU_BASE + 0x34)
#define TFIFOSR0 (TCU_BASE + 0x08)
#define TFIFOSR3 (TCU_BASE + 0x18)
#define TFIFOSR4 (TCU_BASE + 0x28)
#define TFIFOSR5 (TCU_BASE + 0x38)

/* TCU OS Timer sub-unit */
#define TCU_OSTCSR     (TCU_BASE + 0xec)
#define TCU_OSTDR      (TCU_BASE + 0xe0)
#define TCU_OSTCNTH    (TCU_BASE + 0xe8)
#define TCU_OSTCNTL    (TCU_BASE + 0xe4)
#define TCU_OSTCNTHBUF (TCU_BASE + 0xfc)

/* Timer control register */
#define TCSR_BYPASS           (1 << 11)
#define TCSR_CLRZ             (1 << 10)
#define TCSR_SD               (1 << 9)
#define TCSR_INITL            (1 << 8)
#define TCSR_PWM_EN           (1 << 7)
#define TCSR_PWM_IN_EN        (1 << 6)
#define TCSR_PRESCALE_DIV1    (0 << 3)
#define TCSR_PRESCALE_DIV4    (1 << 3)
#define TCSR_PRESCALE_DIV16   (2 << 3)
#define TCSR_PRESCALE_DIV64   (3 << 3)
#define TCSR_PRESCALE_DIV256  (4 << 3)
#define TCSR_PRESCALE_DIV1024 (5 << 3)
#define TCSR_EXT_EN           (1 << 2)
#define TCSR_RTC_EN           (1 << 1)
#define TCSR_PCK_EN           (1 << 0)

/* Timer enable register */
#define TER_OSTEN             (1 << 15) /* OS Timer */
#define TER_TCEN7             (1 << 7)  /* Timer 7 */
#define TER_TCEN6             (1 << 6)  /* Timer 6 */
#define TER_TCEN5             (1 << 5)  /* Timer 5 */
#define TER_TCEN4             (1 << 4)  /* Timer 4 */
#define TER_TCEN3             (1 << 3)  /* Timer 3 */
#define TER_TCEN2             (1 << 2)  /* Timer 2 */
#define TER_TCEN1             (1 << 1)  /* Timer 1 */
#define TER_TCEN0             (1 << 0)  /* Timer 0 */

/* Timer mask register */
#define TMR_OSTMASK           (1 << 15) /* OS timer */

/* Timer flag register */
#define TFR_OSTFLAG           (1 << 15) /* OS timer */

/* OS Timer control register */
#define OSTCSR_CNT_MD         (1 << 15) /* 0: reset to 0 at compare value; 1: ignore compare value */
#define OSTCSR_SD             (1 << 9)  /* 0: graceful shutdown; 1: abrupt shutdown */
#define OSTCSR_PRESCALE_1     0         /* Don't divide the clock */
#define OSTCSR_PRESCALE_4     (1 << 3)  /* Prescale by 4 */
#define OSTCSR_PRESCALE_16    (2 << 3)  /* Prescale by 16 */
#define OSTCSR_PRESCALE_64    (3 << 3)  /* ... */
#define OSTCSR_PRESCALE_256   (4 << 3)
#define OSTCSR_PRESCALE_1024  (5 << 3)
#define OSTCSR_EXT_EN         (1 << 2)  /* Use EXTAL as clock source */
#define OSTCSR_RTC_EN         (1 << 1)  /* Use RTCCLK as clock source */
#define OSTCSR_PCK_EN         (1 << 0)  /* Use PCLK as clock source */

