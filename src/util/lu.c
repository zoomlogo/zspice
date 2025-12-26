#include <math.h>
#include <stdlib.h>

#include "error.h"
#include "lu.h"
#include "types.h"

#define TYPE f64
#define FUNC(name) r_##name
#define ABS(x) fabs(x)
#include "lu.def"
#undef ABS
#undef FUNC
#undef TYPE

#define TYPE c64
#define FUNC(name) c_##name
#define ABS(x) cabs(x)
#include "lu.def"
#undef ABS
#undef FUNC
#undef TYPE
