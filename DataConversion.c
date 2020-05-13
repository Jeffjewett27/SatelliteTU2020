/*
 * Author: Jeff Jewett
 * Modification Date: 5/10/20
*/

#include "simpletools.h"

/* 'reduceFloat16bit'
  reduces a 32 bit (single precision) float down to a 16 bit
  parameters:
    f- the float to convert
    isSigned- 0 if unsigned, 1 to keep the sign bit
    k- the number of bits to store for the exponent (k <= 8)
    the rest of the bits will fill in the fraction
*/
uint16_t reduceFloat16bit(float f, int isSigned, uint8_t k) {
  if (isSigned != 0) {
    isSigned = 1; //ensure we don't get an odd value
  }    
  if (k > 8) {
    k = 8;
  }    
  uint32_t c = *(uint32_t *)&f; //cast float to unsigned integer for bit manipulation
  uint32_t frac = (c << 9) >> 9; //remove the top 9 bits to keep the 23 fraction bits
  uint8_t exp = (c << 1) >> 24; //remove the top sign bit and the 23 fraction bits
  uint16_t sign = isSigned ? c >> 31 : 0; //remove all but the sign bit
  uint8_t m = 16 - k - isSigned; //get number of fraction bits
  uint32_t bias = 127; //the bias constant for single precision
  uint8_t biasSmall = (1 << (k - 1)) - 1; //the bias for k exponent bits
  uint16_t fracSmall = frac >> (23 - m); //take the top m bits from the fraction
  int32_t expVal = exp - bias + biasSmall;
  if (expVal < 0) {
    expVal = 0; //if exponent underflows, denormalize
  }    
  if (expVal > (1 << k) - 1) {
    expVal = (1 << k) - 1; //if exponent overflows, set to all 1s (infinity)
  }
  uint8_t expSmall = expVal; //downcast 32 bit to 8 bit
  return (expSmall << m) + fracSmall + (sign << 15); //join components together
}  


/* 'extendFloatFrom16bit'
  extends a 16 bit float to a single precision float
  parameters:
    b- the 16 bit value
    isSigned- whether the sign bit was included
    k- how many bits of the exponent are stored (k <= 8)
    the rest of the bits will fill in the fraction
*/
float extendFloatFrom16bit(uint16_t b, int isSigned, int k) {
  if (isSigned != 0) {
    isSigned = 1; //ensure we don't get an odd value
  }    
  if (k > 8) {
    k = 8;
  }   
  uint8_t m = 16 - k - isSigned; //get number of fraction bits
  uint16_t fracMask = (1 << m) - 1;
  uint16_t fracSmall = b & fracMask; //get lower m bits
  uint16_t expMask = (1 << k) - 1;
  uint8_t expSmall = (b >> m) & expMask; //get exponent bits
  uint8_t sign = isSigned ? b >> 15 : 0; //get sign bit or 0 if unsigned
  uint8_t bias = 127; //single precision bias constant
  uint8_t biasSmall = (1 << (k - 1)) - 1; //bias for k bit exponent
  uint32_t frac = fracSmall << (23 - m); //extend fraction to 23 bits
  uint8_t exp = expSmall - biasSmall + bias; //convert exponent back to 8 bit
  if (expSmall == 0) {
    exp = 0; //if expSmall was denormalized, denormalize exp
  }    
  if (expSmall == (1 << k) - 1) {
    exp = ~0; //if expSmall was max val, exp is max val
  }    
  uint32_t val = (exp << 23) + frac + (sign << 31); //join components together
  float f = *((float *)&val); //cast back to float
  return f;
}