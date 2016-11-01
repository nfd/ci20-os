#include "inttypes.h"

typedef void(*timer_callback)(void);

void spintimer_init(void);
uint64_t spintimer_getval(void);
void spintimer_usleep(uint64_t usec);
void ostimer_init(void);
void ostimer_interrupt(void);
int ostimer_register_callback(timer_callback);

