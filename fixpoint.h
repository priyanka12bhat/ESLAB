#include <inttypes.h>

#define Q 14
typedef int32_t q14;

int16_t fix2normal(q14 x);
q14	fixmul(q14 x1, q14 x2);
q14 fixdivision(q14 a, q14 b);
q14 normal2fix(int16_t x);
q14 float2fix(double x);
double fix2float(q14 x);