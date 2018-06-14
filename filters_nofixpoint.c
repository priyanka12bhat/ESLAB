#include "fixpoint.h"
#include "in4073.h"
#include "filters.h"
#include <inttypes.h>

// initial values for butterworth filter
q14 x[6][3] = { 0 };
q14 y[6][3] = { 0 };

// initial values and declarations for the kalman filter
double bias_sp, bias_sq = 0;
double kalman_theta, kalman_phi = 0;
double kalman_sp, kalman_sq = 0;
double kalman_sax, kalman_say = 0;
double error_phi, error_theta = 0;

void butterworth()
{
	for (int i = 0; i < 6; i++)
	{
		for (int n = 2; n > 0; n--)
		{
			x[i][n] = x[i][n - 1];
			y[i][n] = y[i][n - 1];
		}
	}

	x[0][0] = normal2fixM(sp);
	y[0][0] = fixmul((q14)BUTTER_A0, x[0][0]) + fixmul((q14)BUTTER_A1, x[0][1]) + fixmul((q14)BUTTER_A2, x[0][2]) - fixmul((q14)BUTTER_B1, y[0][1]) - fixmul((q14)BUTTER_B2, y[0][2]);
	y[0][0] = fixmul((q14)BUTTER_B0, y[0][0]);

	x[1][0] = normal2fixM(sq);
	y[1][0] = fixmul((q14)BUTTER_A0, x[1][0]) + fixmul((q14)BUTTER_A1, x[1][1]) + fixmul((q14)BUTTER_A2, x[1][2]) - fixmul((q14)BUTTER_B1, y[1][1]) - fixmul((q14)BUTTER_B2, y[1][2]);
	y[1][0] = fixmul((q14)BUTTER_B0, y[1][0]);

	x[2][0] = normal2fixM(sr);
	y[2][0] = fixmul((q14)BUTTER_A0, x[2][0]) + fixmul((q14)BUTTER_A1, x[2][1]) + fixmul((q14)BUTTER_A2, x[2][2]) - fixmul((q14)BUTTER_B1, y[2][1]) - fixmul((q14)BUTTER_B2, y[2][2]);
	y[2][0] = fixmul((q14)BUTTER_B0, y[2][0]);

	x[3][0] = normal2fixM(sax);
	y[3][0] = fixmul((q14)BUTTER_A0, x[3][0]) + fixmul((q14)BUTTER_A1, x[3][1]) + fixmul((q14)BUTTER_A2, x[3][2]) - fixmul((q14)BUTTER_B1, y[3][1]) - fixmul((q14)BUTTER_B2, y[3][2]);
	y[3][0] = fixmul((q14)BUTTER_B0, y[3][0]);

	x[4][0] = normal2fixM(say);
	y[4][0] = fixmul((q14)BUTTER_A0, x[4][0]) + fixmul((q14)BUTTER_A1, x[4][1]) + fixmul((q14)BUTTER_A2, x[4][2]) - fixmul((q14)BUTTER_B1, y[4][1]) - fixmul((q14)BUTTER_B2, y[4][2]);
	y[4][0] = fixmul((q14)BUTTER_B0, y[4][0]);

	x[5][0] = normal2fixM(saz);
	y[5][0] = fixmul((q14)BUTTER_A0, x[5][0]) + fixmul((q14)BUTTER_A1, x[5][1]) + fixmul((q14)BUTTER_A2, x[5][2]) - fixmul((q14)BUTTER_B1, y[5][1]) - fixmul((q14)BUTTER_B2, y[5][2]);
	y[5][0] = fixmul((q14)BUTTER_B0, y[5][0]);
	//convert Q14 numbers to int16_t

	//sp = fix2normalM(y[0][0]);
	//sq = fix2normalM(y[1][0]);
	sr = fix2normalM(y[2][0]);
	sax = fix2normalM(y[3][0]);
	say = fix2normalM(y[4][0]);
	//saz = fix2normalM(y[5][0]);
}


void kalman()
{
	// first convert the sensor values to Q14 so we can multiply them with a fraction
	kalman_sp = (double)sp;
	kalman_sq = (double)sq;
	kalman_sax = (double)sax;
	kalman_say = (double)say;

	// kalman filtering for theta phi psi
	kalman_sp = kalman_sp - bias_sp;
	kalman_phi = kalman_phi + kalman_sp * KALMAN_P2PHI_PHI);
	error_phi = kalman_phi - kalman_say;
	kalman_phi = kalman_phi - error_phi / KALMAN_C1_PHI);
	bias_sp = bias_sp + error_phi / (KALMAN_P2PHI_PHI * KALMAN_C2_PHI);

	kalman_sq = kalman_sq - bias_sq;
	kalman_theta = kalman_theta + kalman_sq * KALMAN_P2PHI_THETA;
	error_theta = kalman_theta - kalman_sax;
	kalman_theta = kalman_theta - error_theta / KALMAN_C1_THETA;
	bias_sq = bias_sq + error_theta / (KALMAN_P2PHI_PHI * KALMAN_C2_PHI);

	// convert the sensor values back to integer so we can use them
	sp = (int16_t)kalman_sp;
	sq = (int16_t)kalman_sq;
	theta = (int16_t)kalman_theta;
	phi = (int16_t)kalman_phi;
}