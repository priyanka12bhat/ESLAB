#define BUTTER_B0 16384
#define BUTTER_B1 -29243
#define BUTTER_B2 13203
#define BUTTER_A0 85
#define BUTTER_A1 172
#define BUTTER_A2 85

#define KALMAN_P2PHI_THETA 265
#define KALMAN_C1_THETA 63
#define KALMAN_C2_THETA 2

#define KALMAN_P2PHI_PHI 265
#define KALMAN_C1_PHI 63
#define KALMAN_C2_PHI 2


void butterworth();
void kalman();