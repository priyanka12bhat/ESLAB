#define BUTTER_B0 16384
#define BUTTER_B1 -5885
#define BUTTER_B2 3186
#define BUTTER_A0 3420
#define BUTTER_A1 6841
#define BUTTER_A2 3420

#define KALMAN_P2PHI_THETA 262
#define KALMAN_C1_THETA 64
#define KALMAN_C2_THETA 1

#define KALMAN_P2PHI_PHI 262
#define KALMAN_C1_PHI 64
#define KALMAN_C2_PHI 1

int16_t butterworth(int16_t sample);
void kalman();