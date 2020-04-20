/*
 * Author: Mitchell Toth
 * Modification Date: April 15, 2020
 * Purpose: Coordinate and control all sending/receiving of sensor data.
 */

#define NUM_1_BYTE_READINGS 32
#define NUM_2_BYTE_READINGS 16
#define NUM_4_BYTE_READINGS 8

#include "simpletools.h"
#include "UVSensor.h"
#include "IMUSensor.h"
#include "GammaSensor.h"
#include "TemperatureSensors.h"
#include "AmbientLight.h"
#include "LightToFrequency.h"
#include "queue.h"
#include "Packet.h"
#include "EEPROM.h"
#include "DataConversion.h"
#include "PacketGeneration.h"
#include "MultithreadTest.h"


//Function declarations.
void flashLEDs();
void initializeAllSensors();
uint8_t enqueuePacket(uint8_t packetsCounter, queue *serialBusQueue, Packet sensorPacket);

//0 = not calibrated, 1 = calibrated, global to all threads.
volatile int magnetometerCalibrated = 0;



//Main function
int main() {
  //startTest();
  //pause(100000);
  
  //Packet queue.
  queue *serialBusQueue;
  serialBusQueue = malloc(sizeof(queue));
  initializeQueue(serialBusQueue);
  
  //Powerup event occurs.
  //Ensure IMU is powered up. Involves solid state relay.
  //ensureIMUPowered();
  
  //Get all the sensors ready to read and transmit data.
  initializeAllSensors();
  
  //LED flash to show that the parallax board is connected and working.
  flashLEDs();
  
  //Keep track of total packets sent, as well as main loop iteration.
  uint8_t packetsCounter;
  if (isByteCheckSet()) {
    packetsCounter = readPacketCount();
  } else {
    setByteCheck();
    setPacketCount(0);
    packetsCounter = 0;
  }    
  uint8_t iteration = 0;
  
  
  //Main loop, iterate as long as there is power:
  while (1) {
  	//The goal is to gather and store full packets of data specific to each sensor.
    //While gathering enough data to do this, we create "general sensor values" packets.
    //This allows us to at least send some data immediately when the serial busy line goes low.
    
    Vector3 accelerationReadings[NUM_2_BYTE_READINGS];
    Vector3 gyroscopeReadings[NUM_2_BYTE_READINGS];
    Vector3 magnetometerReadings[NUM_2_BYTE_READINGS];
    float uvReadings[NUM_2_BYTE_READINGS];
    float temp1Readings[NUM_1_BYTE_READINGS];
    uint8_t temp2Readings[NUM_1_BYTE_READINGS];
    uint8_t temp3Readings[NUM_1_BYTE_READINGS];
    uint8_t ambientLightReadings[NUM_1_BYTE_READINGS];
    uint8_t lightToFrequencyReadings[NUM_1_BYTE_READINGS];
    
  	//For approximately 1 minute (32 iterations -- approx 2 sec each):
    for (int i=0; i<32; i++) {
  		//If iteration is a multiple of 8 (0, 8, 16, 24):		// Sec: 0, 16, 32, 48
      //Create "general sensor" packet containing data from a variety of sensors.
      if (i%8 == 0) {
        Packet generalSensorPacket = generateGeneralSensorPacket(iteration, packetsCounter, i, accelerationReadings, 
            gyroscopeReadings, magnetometerReadings, uvReadings, temp1Readings, temp2Readings, temp3Readings,
            ambientLightReadings, lightToFrequencyReadings);
        packetsCounter = enqueuePacket(packetsCounter, serialBusQueue, generalSensorPacket);
      }

      
      //If reading sensor N on this iteration, read it and add to sensor N's packet(s):
      
      //1 byte sensors:
      temp2Readings[i] = temperature2_read();
      temp3Readings[i] = temperature2_read();
      ambientLightReadings[i] = ambientLight_read();
      lightToFrequencyReadings[i] = lightToFrequency_read();
      
      //2 and 4 byte sensors:
      if (i%2 == 0) {
        accelerationReadings[i/2] = imu_accelerometerRead();
        gyroscopeReadings[i/2] = imu_gyroscopeRead();
        magnetometerReadings[i/2] = imu_magnetometerRead();
        uvReadings[i/2] = uvRead();
        temp1Readings[i] = temperature1_read();
      }       

      //Delay 2 sec.
      pause(2000);
    }
    
  	//Clear write queue of any "general sensor" packets.
  	clearQueue(serialBusQueue);
    
    Packet sensorPacket;
    
    /*--------------------------------------------------------------------------------------*/
    /*------------------------------------ Magnetometer ------------------------------------*/
    sensorPacket = generateMagX(magnetometerReadings, iteration, packetsCounter);
    packetsCounter = enqueuePacket(packetsCounter, serialBusQueue, sensorPacket);
    
    sensorPacket = generateMagXCompressed(magnetometerReadings, iteration, packetsCounter);
    packetsCounter = enqueuePacket(packetsCounter, serialBusQueue, sensorPacket);
    
    sensorPacket = generateMagY(magnetometerReadings, iteration, packetsCounter);
    packetsCounter = enqueuePacket(packetsCounter, serialBusQueue, sensorPacket);
    
    sensorPacket = generateMagYCompressed(magnetometerReadings, iteration, packetsCounter);
    packetsCounter = enqueuePacket(packetsCounter, serialBusQueue, sensorPacket);
    
    sensorPacket = generateMagZ(magnetometerReadings, iteration, packetsCounter);
    packetsCounter = enqueuePacket(packetsCounter, serialBusQueue, sensorPacket);
    
    sensorPacket = generateMagZCompressed(magnetometerReadings, iteration, packetsCounter);
    packetsCounter = enqueuePacket(packetsCounter, serialBusQueue, sensorPacket);
    
    
    /*-----------------------------------------------------------------------------------*/
    /*------------------------------------ Gyroscope ------------------------------------*/
    sensorPacket = generateGyroX(gyroscopeReadings, iteration, packetsCounter);
    packetsCounter = enqueuePacket(packetsCounter, serialBusQueue, sensorPacket);
    
    sensorPacket = generateGyroXCompressed(gyroscopeReadings, iteration, packetsCounter);
    packetsCounter = enqueuePacket(packetsCounter, serialBusQueue, sensorPacket);
    
    sensorPacket = generateGyroY(gyroscopeReadings, iteration, packetsCounter);
    packetsCounter = enqueuePacket(packetsCounter, serialBusQueue, sensorPacket);
    
    sensorPacket = generateGyroYCompressed(gyroscopeReadings, iteration, packetsCounter);
    packetsCounter = enqueuePacket(packetsCounter, serialBusQueue, sensorPacket);
    
    sensorPacket = generateGyroZ(gyroscopeReadings, iteration, packetsCounter);
    packetsCounter = enqueuePacket(packetsCounter, serialBusQueue, sensorPacket);
    
    sensorPacket = generateGyroZCompressed(gyroscopeReadings, iteration, packetsCounter);
    packetsCounter = enqueuePacket(packetsCounter, serialBusQueue, sensorPacket);
    
    
    /*--------------------------------------------------------------------------------------*/
    /*------------------------------------ Acceleration ------------------------------------*/
    sensorPacket = generateAccX(accelerationReadings, iteration, packetsCounter);
    packetsCounter = enqueuePacket(packetsCounter, serialBusQueue, sensorPacket);
    
    sensorPacket = generateAccXCompressed(accelerationReadings, iteration, packetsCounter);
    packetsCounter = enqueuePacket(packetsCounter, serialBusQueue, sensorPacket);
    
    sensorPacket = generateAccY(accelerationReadings, iteration, packetsCounter);
    packetsCounter = enqueuePacket(packetsCounter, serialBusQueue, sensorPacket);
    
    sensorPacket = generateAccYCompressed(accelerationReadings, iteration, packetsCounter);
    packetsCounter = enqueuePacket(packetsCounter, serialBusQueue, sensorPacket);
    
    sensorPacket = generateAccZ(accelerationReadings, iteration, packetsCounter);
    packetsCounter = enqueuePacket(packetsCounter, serialBusQueue, sensorPacket);
    
    sensorPacket = generateAccZCompressed(accelerationReadings, iteration, packetsCounter);
    packetsCounter = enqueuePacket(packetsCounter, serialBusQueue, sensorPacket);
    
    
    /*----------------------------------------------------------------------------*/
    /*------------------------------------ UV ------------------------------------*/
    sensorPacket = generateUV(uvReadings, iteration, packetsCounter);
    packetsCounter = enqueuePacket(packetsCounter, serialBusQueue, sensorPacket);
    
    sensorPacket = generateUVCompressed(uvReadings, iteration, packetsCounter);
    packetsCounter = enqueuePacket(packetsCounter, serialBusQueue, sensorPacket);
    
    
    /*-------------------------------------------------------------------------------------*/
    /*------------------------------------ Temperature ------------------------------------*/
    sensorPacket = generateTemp1(temp1Readings, iteration, packetsCounter);
    packetsCounter = enqueuePacket(packetsCounter, serialBusQueue, sensorPacket);
    
    sensorPacket = generateTemp1Compressed(temp1Readings, iteration, packetsCounter);
    packetsCounter = enqueuePacket(packetsCounter, serialBusQueue, sensorPacket);
    
    sensorPacket = generateTemp2(temp2Readings, iteration, packetsCounter);
    packetsCounter = enqueuePacket(packetsCounter, serialBusQueue, sensorPacket);
    
    sensorPacket = generateTemp3(temp3Readings, iteration, packetsCounter);
    packetsCounter = enqueuePacket(packetsCounter, serialBusQueue, sensorPacket);
    
    
    /*---------------------------------------------------------------------------------------*/
    /*------------------------------------ Ambient Light ------------------------------------*/
    sensorPacket = generateAmbientLight(ambientLightReadings, iteration, packetsCounter);
    packetsCounter = enqueuePacket(packetsCounter, serialBusQueue, sensorPacket);
    
    
    /*--------------------------------------------------------------------------------------------*/
    /*------------------------------------ Light to Frequency ------------------------------------*/
    sensorPacket = generateLightToFrequency(lightToFrequencyReadings, iteration, packetsCounter);
    packetsCounter = enqueuePacket(packetsCounter, serialBusQueue, sensorPacket);
    
    
    /*-----------------------------------------------------------
      All queued sensor packets are written in a separate thread.
    ----------------------------------------------------------- */

    //Don't move on until queue is small enough in size (to avoid overflow issues).
    //while (getQueueSize(serialBusQueue) >= 4) {pause(10);}  --Wouldn't this make the general sensor loop useless?
    while (!isQueueEmpty(serialBusQueue)) {pause(10);}
  
    iteration++;
  }
}


void initializeAllSensors() {
  eeprom_initI2C();
  imu_initialize();
  uvInit();
  temperature1_initialize();
  temperature2_initialize();
  temperature3_initialize();
  gamma_initialize();
  ambientLight_initialize();
  lightToFrequency_initialize();
}  

void flashLEDs() {
  high(26);
  high(27);
  pause(500);
  low(26);
  low(27);
}

uint8_t enqueuePacket(uint8_t packetsCounter, queue *serialBusQueue, Packet sensorPacket) {
  packetsCounter++; 
  setPacketCount(packetsCounter); 
  enqueue(serialBusQueue, sensorPacket);
  printf("Completed packet with fnCode: 0x%x\n", sensorPacket.fnCode);
  return packetsCounter;
}  