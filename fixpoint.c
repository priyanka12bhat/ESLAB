#include "fixpoint.h"

//Mingxi LI
//fixmul -- multiply fixed 18+14 bits to float
q14	fixmul(q14 x1, q14 x2)
{
	q14 y;
	int64_t temp; // because we multiply two 32bit numbers
	temp = (int64_t)x1 * (int64_t)x2;
	temp += (1 << (Q_FIXEDPOINT - 1));
	y = (q14)(temp >> Q_FIXEDPOINT);
	return y;
}
