#include "fixpoint.h"

//Convert float to fixed point 18+14 bits
q14 float2fix(double x)
{
	q14	y;
	y = x * (1 << Q);
	return y;
}

//Convert fixed 18+14 bits to float
double fix2float(q14 x)
{
	double	y;
	y = ((double)x) / (1 << Q);
	return y;
}

//Convert int16_t to fixed point 18+14 bits
q14 normal2fix(int16_t x)
{
	q14 y;
	y = (q14)(x) * (1 << Q);
	return y;
}

//Convert fixed 18+14 bits to int
int16_t fix2normal(q14 x)
{
	int16_t	y;
	y = x / (1 << Q);
	return y;
}


//fixmul -- multiply fixed 18+14 bits to float
q14	fixmul(q14 x1, q14 x2)
{
	q14 y;
	int64_t temp; // because we multiply two 32bit numbers
	temp = (int64_t)x1 * (int64_t)x2;
	temp += (1 << (Q - 1));
	y = (q14)(temp >> Q);
	return y;
}

/***********this function performs the division *************/
q14 fixdivision(q14 a, q14 b)
{
	q14 result;
	int temp;
	temp = (int)a << Q;
	// Rounding: mid values are rounded up.
	if ((temp >= 0 && b >= 0) || (temp < 0 && b < 0))
		temp += b / 2;
	else
		temp -= b / 2;
	result = (q14)(temp / b);
	return result;
}