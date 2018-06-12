#include <inttypes.h>

#define Q_FIXEDPOINT 14
typedef int32_t q14;

q14	fixmul(q14 x1, q14 x2);


#define normal2fixM(x)((q14)(x)*(1<<Q_FIXEDPOINT));
#define fix2normalM(x)(x/(1<<Q_FIXEDPOINT));