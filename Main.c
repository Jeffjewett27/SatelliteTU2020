/*
 * Author: Mitchell Toth
 * Modification Date: 10/12/20
*/

#define NUM_1_BYTE_READINGS 32
#define NUM_2_BYTE_READINGS 16
#define NUM_4_BYTE_READINGS 8

#include "simpletools.h"
#include "UVSensors.h"
#include "IMUSensor.h"
#include "GammaLightSensors.h"
#include "TemperatureSensors.h"
#include "CurrentSenseResistor.h"
#include "queue.h"
#include "Packet.h"
#include "DataConversion.h"
#include "PacketGeneration.h"
#include "SensorReadings.h"
#include "SerialOutput.h"
#include "ADC.h"


//Function declarations.
void initializeAllSensors();
uint8_t enqueuePacket(uint8_t packetsCounter, queue *serialBusQueue, Packet sensorPacket);


//Main function
int main() {
  //Packet queue.
  queue *serialBusQueue;
  serialBusQueue = malloc(sizeof(queue));
  initializeQueue(serialBusQueue);
  serialOutputThread(serialBusQueue);
  
  //Get all the sensors ready to read and transmit data.
  initializeAllSensors();
  
  //Keep track of total packets sent, as well as main loop iteration.
  uint8_t packetsCounter = 0;
  uint8_t iteration = 0;
  
  //Main loop, iterate as long as there is power:
  while (1) {
  	//The goal is to gather and store full packets of data specific to each sensor.
    //While gathering enough data to do this, we create "general sensor values" packets.
    //This allows us to at least send some data immediately when the serial busy line goes low.
    
    SensorReadings sensors;
    lightToFrequency_read_reset();
    gamma_read_reset();
  	//For approximately 1 minute (32 iterations -- approx 2 sec each):
    for (int i=0; i<32; i++) {
  		 //If reading sensor N on this iteration, read it and add to sensor N's packet(s):

      //2 and 4 byte sensors:
      
      if (i%2 == 0) {
        sensors.accelerationReadings[i/2] = imu_accelerometerRead();
        sensors.gyroscopeReadings[i/2] = imu_gyroscopeRead();
        sensors.magnetometerReadings[i/2] = imu_magnetometerRead();
        sensors.uv1Readings[i/2] = uvRead();
        sensors.temp1Readings[i/2] = temperature1_read();
        sensors.temp2Readings[i/2] = temperature2_read();
        sensors.currentSenseReadings[i/2] = currentSenseResistor_read();
      }
      
      //If iteration is a multiple of 8 (0, 8, 16, 24):		// Sec: 0, 16, 32, 48
      //Create "general sensor" packet containing data from a variety of sensors.
      if (i%8 == 0 && getQueueSize(serialBusQueue) < 2) {
        Packet generalSensorPacket = generateGeneralSensorPacket(iteration, packetsCounter, i, &sensors);
        packetsCounter = enqueuePacket(packetsCounter, serialBusQueue, generalSensorPacket);
      }

      pause(2000);
      
      //read light and gamma after a 2 second pause
      sensors.gammaReadings[i] = gamma_read_reset();
      if (i%2 == 0) {
        sensors.lightToFrequencyReadings[i/2] = lightToFrequency_read_reset();
      } 
    }
    
    Packet sensorPacket;
    
    if (iteration % 2 == 0) {
      sensorPacket = generateMagXCompressed(sensors.magnetometerReadings, iteration, packetsCounter);
      packetsCounter = enqueuePacket(packetsCounter, serialBusQueue, sensorPacket);
      
      sensorPacket = generateMagYCompressed(sensors.magnetometerReadings, iteration, packetsCounter);
      packetsCounter = enqueuePacket(packetsCounter, serialBusQueue, sensorPacket);
      
      sensorPacket = generateMagZCompressed(sensors.magnetometerReadings, iteration, packetsCounter);
      packetsCounter = enqueuePacket(packetsCounter, serialBusQueue, sensorPacket);
    } else {
      sensorPacket = generateMagX(sensors.magnetometerReadings, iteration, packetsCounter);
      packetsCounter = enqueuePacket(packetsCounter, serialBusQueue, sensorPacket);
      
      sensorPacket = generateMagY(sensors.magnetometerReadings, iteration, packetsCounter);
      packetsCounter = enqueuePacket(packetsCounter, serialBusQueue, sensorPacket);
      
      sensorPacket = generateMagZ(sensors.magnetometerReadings, iteration, packetsCounter);
      packetsCounter = enqueuePacket(packetsCounter, serialBusQueue, sensorPacket);
    }
    
    if (iteration % 2 == 0) {
      sensorPacket = generateGyroXCompressed(sensors.gyroscopeReadings, iteration, packetsCounter);
      packetsCounter = enqueuePacket(packetsCounter, serialBusQueue, sensorPacket);
      
      sensorPacket = generateGyroYCompressed(sensors.gyroscopeReadings, iteration, packetsCounter);
      packetsCounter = enqueuePacket(packetsCounter, serialBusQueue, sensorPacket);
      
      sensorPacket = generateGyroZCompressed(sensors.gyroscopeReadings, iteration, packetsCounter);
      packetsCounter = enqueuePacket(packetsCounter, serialBusQueue, sensorPacket);
    } else {
      sensorPacket = generateGyroX(sensors.gyroscopeReadings, iteration, packetsCounter);
      packetsCounter = enqueuePacket(packetsCounter, serialBusQueue, sensorPacket);
    
      sensorPacket = generateGyroY(sensors.gyroscopeReadings, iteration, packetsCounter);
      packetsCounter = enqueuePacket(packetsCounter, serialBusQueue, sensorPacket);

      sensorPacket = generateGyroZ(sensors.gyroscopeReadings, iteration, packetsCounter);
      packetsCounter = enqueuePacket(packetsCounter, serialBusQueue, sensorPacket);
    }
    
    if (iteration % 2 == 0) {
      sensorPacket = generateAccXCompressed(sensors.gyroscopeReadings, iteration, packetsCounter);
      packetsCounter = enqueuePacket(packetsCounter, serialBusQueue, sensorPacket);
   
      sensorPacket = generateAccYCompressed(sensors.gyroscopeReadings, iteration, packetsCounter);
      packetsCounter = enqueuePacket(packetsCounter, serialBusQueue, sensorPacket);
    
      sensorPacket = generateAccZCompressed(sensors.gyroscopeReadings, iteration, packetsCounter);
      packetsCounter = enqueuePacket(packetsCounter, serialBusQueue, sensorPacket);
    } else {
      sensorPacket = generateAccX(sensors.accelerationReadings, iteration, packetsCounter);
      packetsCounter = enqueuePacket(packetsCounter, serialBusQueue, sensorPacket);
       
      sensorPacket = generateAccY(sensors.accelerationReadings, iteration, packetsCounter);
      packetsCounter = enqueuePacket(packetsCounter, serialBusQueue, sensorPacket);
      
      sensorPacket = generateAccZ(sensors.accelerationReadings, iteration, packetsCounter);
      packetsCounter = enqueuePacket(packetsCounter, serialBusQueue, sensorPacket);
      
    }
    
    sensorPacket = generateUV1(sensors.uv1Readings, iteration, packetsCounter);
    packetsCounter = enqueuePacket(packetsCounter, serialBusQueue, sensorPacket);
    
    if (iteration % 2 == 0) {
      sensorPacket = generateTemp1Compressed(sensors.temp1Readings, iteration, packetsCounter);
      packetsCounter = enqueuePacket(packetsCounter, serialBusQueue, sensorPacket);
    } else {     
      sensorPacket = generateTemp1(sensors.temp1Readings, iteration, packetsCounter);
      packetsCounter = enqueuePacket(packetsCounter, serialBusQueue, sensorPacket);
    }   
    
    sensorPacket = generateTemp2(sensors.temp2Readings, iteration, packetsCounter);
    packetsCounter = enqueuePacket(packetsCounter, serialBusQueue, sensorPacket);
    
    sensorPacket = generateLightToFrequency(sensors.lightToFrequencyReadings, iteration, packetsCounter);
    packetsCounter = enqueuePacket(packetsCounter, serialBusQueue, sensorPacket);
    
    //decide whether to compress gamma readings
    int gamma2ByteCount = 0;
    print("gamma: ");
    for (int i = 0; i < NUM_1_BYTE_READINGS; ++i) {
      print("%d ", sensors.gammaReadings[i]);
      if (sensors.gammaReadings[i] > 255) {
        gamma2ByteCount++;
      }        
    }      
    if (gamma2ByteCount > 1) {
      sensorPacket = generateGamma(sensors.gammaReadings, iteration, packetsCounter);
    } else {
      sensorPacket = generateGammaComp(sensors.gammaReadings, iteration, packetsCounter);
    }      
    packetsCounter = enqueuePacket(packetsCounter, serialBusQueue, sensorPacket);
    
    sensorPacket = generateCurrentSenseResistor(sensors.currentSenseReadings, iteration, packetsCounter);
    packetsCounter = enqueuePacket(packetsCounter, serialBusQueue, sensorPacket);
    
    //wait for queue to clear out
    while (getQueueSize(serialBusQueue) > 2) {
      pause(100);
    }     
  
    iteration++;
  }
}


void initializeAllSensors() {
  startLtfThread();
  startGammaThread();
  imu_initialize();
}

uint8_t enqueuePacket(uint8_t packetsCounter, queue *serialBusQueue, Packet sensorPacket) {
  packetsCounter++; 
  enqueue(serialBusQueue, sensorPacket);
  //printf("Completed packet with fnCode: 0x%x, pc: %d\n", sensorPacket.fnCode, packetsCounter);
  //necessary to prevent memory overflow
  while (getQueueSize(serialBusQueue) > 2) {
      pause(100);
  }
  return packetsCounter;
}