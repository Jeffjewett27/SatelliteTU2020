/*
 * Author: Jeff Jewett
 * Modification Date: 5/10/20
*/

void startLightGammaThread();
void stopLightGammaThread();

//try not to call from other threads
void gamma_initialize();
void light_initialize();
void gamma_light_thread();

//these functions have the side effect of resetting the respective counter to zero
uint32_t gamma_read_reset();
uint32_t lightToFrequency_read_reset();