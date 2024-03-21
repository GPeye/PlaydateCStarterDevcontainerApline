#ifndef _constants_h_
#define _constants_h_

#include "pd_api.h"

extern PlaydateAPI* pd;

#define FALSE         0
#define TRUE          1

#define SPACE_FREQ    5000.0f
#define MARK_FREQ     10000.0f
#define SAMPLE_RATE   44100.0f
#define SAMPLE_PERIOD 0.00002268f

// 44,100 / 40 = 1102.5 baud
#define SYMBOL_SAMPLE_COUNT 40
#define START_BITS 1

// idk why min() isn't defined in stdlib.h...
#ifndef min
#define min(a,b)            (((a) < (b)) ? (a) : (b))
#endif

#endif