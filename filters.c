#include "fixpoint.h"
#include "in4073.h"
#include "filters.h"

// initial values for butterworth filter
q14 x[3] = {0};
q14 y[3] = {0};

// initial values and declarations for the kalman filter
q14 bias_sp, bias_sq= 0;
q14 kalman_theta, kalman_phi = 0;
q14 kalman_sp, kalman_sq = 0;
q14 kalman_sax, kalman_say = 0;
q14 error_phi, error_theta = 0;

int16_t butterworth(int16_t sample)
{
	for (int n = 2; n > 0; n--)
	{
		x[n] = x[n - 1];
		y[n] = y[n - 1];
	}

	x[0] = normal2fixM(sample);
	y[0] = fixmul((q14)BUTTER_A0, x[0]) + fixmul((q14)BUTTER_A1, x[1]) + fixmul((q14)BUTTER_A2, x[2]) - fixmul((q14)BUTTER_B1, y[1]) - fixmul((q14)BUTTER_B2, y[2]);
	//printf("Butterworth say: %d\n", fix2normal(y[0]));
	//convert Q14 numbers to int16_t
	return fix2normalM(y[0]);
}


void kalman()
{
	// first convert the sensor values to Q14 so we can multiply them with a fraction
	kalman_sp = normal2fixM(sp);
	kalman_sq = normal2fixM(sq);
	kalman_sax = normal2fixM(sax);
	kalman_say = normal2fixM(say);

	// kalman filtering for theta phi psi
	kalman_sp = kalman_sp - bias_sp;
	kalman_phi = kalman_phi + fixmul(kalman_sp, KALMAN_P2PHI_PHI);
	error_phi = kalman_phi - kalman_say;
	kalman_phi = kalman_phi - fixmul(error_phi, KALMAN_C1_PHI);
	bias_sp = bias_sp + fixmul(error_phi, KALMAN_C2_PHI);

	kalman_sq = kalman_sq - bias_sq;
	kalman_theta = kalman_theta + fixmul(kalman_sq, KALMAN_P2PHI_THETA);
	error_theta = kalman_theta - kalman_sax;
	kalman_theta = kalman_theta - fixmul(error_theta, KALMAN_C1_THETA);
	bias_sq = bias_sq + fixmul(error_theta, KALMAN_C2_THETA);

	// convert the sensor values back to integer so we can use them
	sp = fix2normalM(kalman_sp);
	sq = fix2normalM(kalman_sq);
	theta = fix2normalM(kalman_theta);
	phi = fix2normalM(kalman_phi);
}