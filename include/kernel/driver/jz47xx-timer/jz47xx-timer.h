#include "inttypes.h"

typedef void(*timer_callback)(void);

void ostimer_init(void);
void ostimer_interrupt(void);
int ostimer_register_callback(timer_callback);

