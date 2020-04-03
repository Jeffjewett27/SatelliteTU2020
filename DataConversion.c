#include "simpletools.h"

/*uint8_t float2Byte(float f) {
  uint32_t *c = (uint32_t *)&f;
  uint32_t frac = ((*c) << 9) >> 9;
  uint8_t exp = ((*c) << 1) >> 24;
  print("frac: %x, exp: %x\n", frac, exp);
  uint8_t k = 3;
  uint8_t m = 5;
  uint32_t bias = 127;
  uint8_t biasSmall = (1 << (k - 1)) - 1;
  
  uint8_t fracSmall = frac >> (23 - m);
  int32_t expVal = exp - bias + biasSmall;
  if (expVal < 0) {
    expVal = 0;
  }    
  if (expVal > (1 << k) - 1) {
    expVal = (1 << k) - 1;
  }
  print("expVal: %x, biasSmall: %x\n", expVal, biasSmall);
  uint8_t expSmall = expVal;    
  print("frac: %x, exp: %x\n", fracSmall, expSmall);
  return (expSmall << m) + fracSmall;
}

float byte2Float(uint8_t b) {
  print("byte: %x\n", b);
  uint8_t k = 3;
  uint8_t m = 5;
  uint8_t fracMask = (1 << m) - 1;
  uint8_t fracSmall = b & fracMask;
  uint8_t expSmall = b >> m;
  print("frac: %x, exp: %x\n", fracSmall, expSmall);
  uint32_t bias = 127;
  uint8_t biasSmall = (1 << (k - 1)) - 1;
  uint32_t frac = fracSmall << (23 - m);
  uint8_t exp = expSmall - biasSmall + bias;
  if (expSmall == 0) {
    exp = 0;
  }    
  if (expSmall == (1 << k) - 1) {
    exp = ~0;
  }    
  print("frac: %x, exp: %x\n", frac, exp);
  uint32_t val = (exp << 23) + frac;
  float f = *((float *)&val);
  print("float: %.5f", f);
  while(1);
  return 0;
}   

uint8_t reduceFloat(float f) {
  uint32_t *c = (uint32_t *)&f;
  uint32_t frac = ((*c) << 9) >> 9;
  uint8_t exp = ((*c) << 1) >> 24;
  int8_t E = exp - 127;
  uint8_t eDiff = E + 1; //0.0001 (10^-4) -> 0.1 (10^-1)
  print("frac: %x, exp: %x\n", frac, exp);
  uint32_t M = frac;
  if (exp != 0) {
    M += 1 << 23; //add the leading 1.xxx
  }    
  uint32_t decimal = M;
  if (E < 0) {
    decimal >>= -E;
  } else {
    decimal <<= E;
  }
  uint8_t shortDecimal = (decimal >> (23 - 8));
  print("decimal: %x\n", shortDecimal);
  return shortDecimal;
}  

float extendFloat(uint8_t d) {
  print("d: %x\n", d);
  uint8_t val = d;
  int8_t E = -9;
  while (val > 0) {
    E++;
    val >>= 1;
  }    
  print("E: %d\n", E);
  uint8_t bias = 127;
  uint8_t exp = 0;
  uint32_t frac = (d << (23 - 8));
  if (E > -9) {
    exp = E + bias;
    if (E < 0) {
      frac <<= -E;
    }      
  }
  frac &= (1 << 23) - 1;
  print("frac: %x, exp: %x\n", frac, exp);
  uint32_t extended = (exp << 23) + frac;
  float f = *((float *)&extended);
  print("float: %.5f\n", f);
  while(1);
  return 0;
}*/

///reduces a 32 bit (single precision) float down to a 16 bit
///parameters:
///   f- the float to convert
///   isSigned- 0 if unsigned, 1 to keep the sign bit
///   k- the number of bits to store for the exponent (k <= 8)
///the rest of the bits will fill in the fraction
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
  print("frac: %x, exp: %x, sign: %x\n", frac, exp, sign);
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
  print("expVal: %x, biasSmall: %x\n", expVal, biasSmall);
  uint8_t expSmall = expVal;    
  print("frac: %x, exp: %x\n", fracSmall, expSmall);
  print("m+k: %d\n", m+k);
  return (expSmall << m) + fracSmall + (sign << 15);
}  

///extends a 16 bit float to a single precision float
///parameters:
///   b- the 16 bit value
///   isSigned- whether the sign bit was included
///   k- how many bits of the exponent are stored (k <= 8)
///the rest of the bits will fill in the fraction
float extendFloatFrom16bit(uint16_t b, int isSigned, int k) {
  if (isSigned != 0) {
    isSigned = 1; //ensure we don't get an odd value
  }    
  if (k > 8) {
    k = 8;
  }   
  print("byte: %x\n", b);
  uint8_t m = 16 - k - isSigned; //get number of fraction bits
  uint16_t fracMask = (1 << m) - 1;
  uint16_t fracSmall = b & fracMask; //get lower m bits
  uint16_t expMask = (1 << k) - 1;
  uint8_t expSmall = (b >> m) & expMask; //get exponent bits
  uint8_t sign = isSigned ? b >> 15 : 0; //get sign bit or 0 if unsigned
  print("m: %d, k: %d, isSigned: %d\n", m, k, isSigned);
  print("frac: %x, exp: %x, sign: %x\n", fracSmall, expSmall, sign);
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
  print("frac: %x, exp: %x\n", frac, exp);
  uint32_t val = (exp << 23) + frac + (sign << 31);
  float f = *((float *)&val);
  print("float: %.5f", f);
  while(1);
  return 0;
}   