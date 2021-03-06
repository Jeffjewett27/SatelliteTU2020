/*
 * Author: Isacc Wickham and Jeff Jewett
 * Modification Date: 5/10/20
*/

#ifndef VECTOR3_H
#define VECTOR3_H

//defines a 3-vector returned by the IMU
struct Vector3 {
  float x;
  float y;
  float z; 
};

typedef struct Vector3 Vector3;

#endif