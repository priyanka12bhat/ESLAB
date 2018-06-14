#define BUTTER_B0 16384
#define BUTTER_B1 -31303
#define BUTTER_B2 14981
#define BUTTER_A0 15
#define BUTTER_A1 31
#define BUTTER_A2 15

#define KALMAN_P2PHI_THETA 0.001
#define KALMAN_C1_THETA 60000
#define KALMAN_C2_THETA 3000000000

#define KALMAN_P2PHI_PHI 0.001
#define KALMAN_C1_PHI 60000
#define KALMAN_C2_PHI 3000000000

#define normal2fixM(x)((q14)(x)*(1<<Q));
#define fix2normalM(x)(x/(1<<Q));

void butterworth();
void kalman();