///reduces a 32 bit (single precision) float down to a 16 bit
///parameters:
///   f- the float to convert
///   isSigned- 0 if unsigned, 1 to keep the sign bit
///   k- the number of bits to store for the exponent (k <= 8)
///the rest of the bits will fill in the fraction
uint16_t reduceFloat16bit(float f, int isSigned, uint8_t k);

///extends a 16 bit float to a single precision float
///parameters:
///   b- the 16 bit value
///   isSigned- whether the sign bit was included
///   k- how many bits of the exponent are stored (k <= 8)
///the rest of the bits will fill in the fraction
float extendFloatFrom16bit(uint16_t b, int isSigned, int k);
