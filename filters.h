#define BUTTER_A0 16384
#define BUTTER_A1 -31303 					
#define BUTTER_A2 14981
#define BUTTER_B0 15
#define BUTTER_B1 31			
#define BUTTER_B2 15		

#define KALMAN_P2PHI_THETA 132
#define KALMAN_C1_THETA 4194304
#define KALMAN_C2_THETA 132710400

#define KALMAN_P2PHI_PHI 132
#define KALMAN_C1_PHI 4194304
#define KALMAN_C2_PHI 132710400

void butterworth();
void kalman();