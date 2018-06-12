#define BUTTER_B0 16384
#define BUTTER_B1 -25526
#define BUTTER_B2 10475
#define BUTTER_A0 332
#define BUTTER_A1 666
#define BUTTER_A2 332

#define KALMAN_P2PHI_THETA 265
#define KALMAN_C1_THETA 63
#define KALMAN_C2_THETA 2

#define KALMAN_P2PHI_PHI 265
#define KALMAN_C1_PHI 63
#define KALMAN_C2_PHI 2


int16_t butterworth(int16_t sample);
void kalman();