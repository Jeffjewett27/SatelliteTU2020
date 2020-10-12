#include "Packet.h"

void startLtfThread();
void ltfThread();
void initLtf();
uint16_t lightToFrequency_read_reset();

void startGammaThread();
void radThread();
void initRad();
uint16_t gamma_read_reset();