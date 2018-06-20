#define BUTTER_B0 16384
#define BUTTER_B1 -29243
#define BUTTER_B2 13203
#define BUTTER_A0 85
#define BUTTER_A1 172
#define BUTTER_A2 85

#define KALMAN_P2PHI_THETA 0.0025
#define KALMAN_C1_THETA 60000
#define KALMAN_C2_THETA 3000000000

#define KALMAN_P2PHI_PHI 0.0025
#define KALMAN_C1_PHI 60000
#define KALMAN_C2_PHI 3000000000


void butterworth();
void kalman();