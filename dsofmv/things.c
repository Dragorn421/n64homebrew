#define MS(s) extern int s;
#define MSP(s, d) d;
#include "missing_symbols.h"
#undef MS
#undef MSP

void *things[] = {
#define MS(s) &s,
#define MSP(s, d) &s,
#include "missing_symbols.h"
#undef MS
#undef MSP
};
