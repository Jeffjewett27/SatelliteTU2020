/*
 * Author: Mitchell Toth
 * Modification Date: April 10, 2020
 * Purpose: Coordinate and control all sending/receiving of sensor data.
 */

#include "simpletools.h"
#include "UVSensor.h"
#include "IMUSensor.h"
#include "queue.h"
#include "Packet.h"
#include "EEPROM.h"
#include "DataConversion.h"

#define NUM_1_BYTE_READINGS 32
#define NUM_2_BYTE_READINGS 16
#define NUM_4_BYTE_READINGS 8

//Function declarations.
void flashLEDs();
void initializeAllSensors();
uint8_t setPacketFieldsAndEnqueue(Packet sensorPacket, queue *serialBusQueue, uint8_t fnCode, uint8_t iteration, uint8_t packetsCounter);

//0 = not calibrated, 1 = calibrated, global to all threads.
volatile int magnetometerCalibrated = 0;



//Main function
int main() {
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
  setPacketCount(0);  //Need to have this be 0 on initial power up.
  uint8_t packetsCounter = readPacketCount();
  uint8_t iteration = 0;

  struct Vector3 accelerationReadings[NUM_2_BYTE_READINGS];
  struct Vector3 gyroscopeReadings[NUM_2_BYTE_READINGS];
  struct Vector3 magnetometerReadings[NUM_2_BYTE_READINGS];
  float uv1Readings[NUM_2_BYTE_READINGS];
  
  //Make arrays for each sensor, assume everything requires 2 bytes.
  
  Packet generalSensorPacket;
  
  
  //Main loop, iterate as long as there is power:
  while (1) {
  	//The goal is to gather and store full packets of data specific to each sensor.
    //While gathering enough data to do this, we create "general sensor values" packets.
    //This allows us to at least send some data immediately when the serial busy line goes low.
    
  	//For approximately 1 minute (32 iterations -- approx 2 sec each):
    for (int i=0; i<32; i++) {
  		//If iteration is a multiple of 8 (0, 8, 16, 24):		// Sec: 0, 16, 32, 48
      if (i%8 == 0) {
        //Create "general sensor values" packets, both compressed and uncompressed.
        //Add data:
        //Acceleration readings
        generalSensorPacket.ArrayType.twoByte[0] = reduceFloat16bit(accelerationReadings[i/2].x, 1, 5);
        generalSensorPacket.ArrayType.twoByte[2] = reduceFloat16bit(accelerationReadings[i/2].y, 1, 5);
        generalSensorPacket.ArrayType.twoByte[4] = reduceFloat16bit(accelerationReadings[i/2].z, 1, 5);
        //Gyroscope readings
        generalSensorPacket.ArrayType.twoByte[6] = reduceFloat16bit(gyroscopeReadings[i/2].x, 1, 5);
        generalSensorPacket.ArrayType.twoByte[8] = reduceFloat16bit(gyroscopeReadings[i/2].y, 1, 5);
        generalSensorPacket.ArrayType.twoByte[10] = reduceFloat16bit(gyroscopeReadings[i/2].z, 1, 5);
        //Magnetometer readings
        generalSensorPacket.ArrayType.twoByte[12] = reduceFloat16bit(magnetometerReadings[i/2].x, 0, 5);  //Is magnetometer data signed?
        generalSensorPacket.ArrayType.twoByte[14] = reduceFloat16bit(magnetometerReadings[i/2].y, 0, 5);
        generalSensorPacket.ArrayType.twoByte[16] = reduceFloat16bit(magnetometerReadings[i/2].z, 0, 5);
        //UV reading
        generalSensorPacket.ArrayType.twoByte[18] = reduceFloat16bit(uv1Readings[i/2], 0, 5);
        
        packetsCounter = setPacketFieldsAndEnqueue(generalSensorPacket, serialBusQueue, 0x01, iteration, packetsCounter);
      }
      
      /*--------------------------------------------
      //In another file, another thread.
      //if (busy_line_is_low && !serialBusQueue.isEmpty()) {
        writeOnePacket();
      }
      //-------------------------------------------- */
      
      //If reading sensor N on this iteration, read it and add to sensor N's packet(s):
      //Read based on 1 byte or 2 byte.
      
      //Read from all 1 byte sensors here. Are there any 1-byte sensors?
      //array[i] = ...
      
      //2-byte sensors.
      if (i%2 == 0) {
        //Also read from all 2 byte sensors here.
        accelerationReadings[i/2] = imu_accelerometerRead();
        gyroscopeReadings[i/2] = imu_gyroscopeRead();
        magnetometerReadings[i/2] = imu_magnetometerRead();
        uv1Readings[i/2] = uvRead();
      }       

      //Delay 2 sec.
      pause(2000);
    }
  
  	//Clear write queue of any "general sensor values" packets.
  	clearQueue(serialBusQueue);


    Packet sensorPacket;
    //------------------------------------------Packets for Acceleration X------------------------------------------
    //Compressed  
    for (int i=0; i<NUM_2_BYTE_READINGS; i++) {
      sensorPacket.ArrayType.twoByte[i] = reduceFloat16bit(accelerationReadings[i].x, 1, 5);
    }
    packetsCounter = setPacketFieldsAndEnqueue(sensorPacket, serialBusQueue, 0x2e, iteration, packetsCounter);
    
    //Un-compressed
    for (int i=0; i<NUM_4_BYTE_READINGS; i++) {
      sensorPacket.ArrayType.fourByte[i] = accelerationReadings[i*2].x;
    }
    packetsCounter = setPacketFieldsAndEnqueue(sensorPacket, serialBusQueue, 0x4e, iteration, packetsCounter);
    
    
    
    //------------------------------------------Packets for Acceleration Y------------------------------------------
    //Compressed
    for (int i=0; i<NUM_2_BYTE_READINGS; i++) {
      sensorPacket.ArrayType.twoByte[i] = reduceFloat16bit(accelerationReadings[i].y, 1, 5);
    }
    packetsCounter = setPacketFieldsAndEnqueue(sensorPacket, serialBusQueue, 0x2f, iteration, packetsCounter);
    
    //Un-compressed
    for (int i=0; i<NUM_4_BYTE_READINGS; i++) {
      sensorPacket.ArrayType.fourByte[i] = accelerationReadings[i*2].y;
    }
    packetsCounter = setPacketFieldsAndEnqueue(sensorPacket, serialBusQueue, 0x4f, iteration, packetsCounter);
    
    
    
    //------------------------------------------Packets for Acceleration Z------------------------------------------
    //Compressed
    for (int i=0; i<NUM_2_BYTE_READINGS; i++) {
      sensorPacket.ArrayType.twoByte[i] = reduceFloat16bit(accelerationReadings[i].z, 1, 5);
    }
    packetsCounter = setPacketFieldsAndEnqueue(sensorPacket, serialBusQueue, 0x10, iteration, packetsCounter);   //What should this function code be?
    
    //Un-compressed
    for (int i=0; i<NUM_4_BYTE_READINGS; i++) {
      sensorPacket.ArrayType.fourByte[i] = accelerationReadings[i*2].z;
    }
    packetsCounter = setPacketFieldsAndEnqueue(sensorPacket, serialBusQueue, 0x10, iteration, packetsCounter);   //What should this function code be?
    
    
    
    //------------------------------------------Packets for Gyro X------------------------------------------
    //Compressed  
    for (int i=0; i<NUM_2_BYTE_READINGS; i++) {
      sensorPacket.ArrayType.twoByte[i] = reduceFloat16bit(gyroscopeReadings[i].x, 1, 5);
    }
    packetsCounter = setPacketFieldsAndEnqueue(sensorPacket, serialBusQueue, 0x2b, iteration, packetsCounter);
    
    //Un-compressed
    for (int i=0; i<NUM_4_BYTE_READINGS; i++) {
      sensorPacket.ArrayType.fourByte[i] = gyroscopeReadings[i*2].x;
    }
    packetsCounter = setPacketFieldsAndEnqueue(sensorPacket, serialBusQueue, 0x4b, iteration, packetsCounter);
    
    
    
    //------------------------------------------Packets for Gyro Y------------------------------------------
    //Compressed
    for (int i=0; i<NUM_2_BYTE_READINGS; i++) {
      sensorPacket.ArrayType.twoByte[i] = reduceFloat16bit(gyroscopeReadings[i].y, 1, 5);
    }
    packetsCounter = setPacketFieldsAndEnqueue(sensorPacket, serialBusQueue, 0x2c, iteration, packetsCounter);
    
    //Un-compressed
    for (int i=0; i<NUM_4_BYTE_READINGS; i++) {
      sensorPacket.ArrayType.fourByte[i] = gyroscopeReadings[i*2].y;
    }
    packetsCounter = setPacketFieldsAndEnqueue(sensorPacket, serialBusQueue, 0x4c, iteration, packetsCounter);
    
    
    
    //------------------------------------------Packets for Gyro Z------------------------------------------
    //Compressed
    for (int i=0; i<NUM_2_BYTE_READINGS; i++) {
      sensorPacket.ArrayType.twoByte[i] = reduceFloat16bit(gyroscopeReadings[i].z, 1, 5);
    }
    packetsCounter = setPacketFieldsAndEnqueue(sensorPacket, serialBusQueue, 0x2d, iteration, packetsCounter);
    
    //Un-compressed
    for (int i=0; i<NUM_4_BYTE_READINGS; i++) {
      sensorPacket.ArrayType.fourByte[i] = gyroscopeReadings[i*2].z;
    }
    packetsCounter = setPacketFieldsAndEnqueue(sensorPacket, serialBusQueue, 0x4d, iteration, packetsCounter);
    
    
    
    //------------------------------------------Packets for MagX------------------------------------------
    //Compressed  
    for (int i=0; i<NUM_2_BYTE_READINGS; i++) {
      sensorPacket.ArrayType.twoByte[i] = reduceFloat16bit(magnetometerReadings[i].x, 0, 5);
    }
    packetsCounter = setPacketFieldsAndEnqueue(sensorPacket, serialBusQueue, 0x24, iteration, packetsCounter);
    
    //Un-compressed
    for (int i=0; i<NUM_4_BYTE_READINGS; i++) {
      sensorPacket.ArrayType.fourByte[i] = magnetometerReadings[i*2].x;
    }
    packetsCounter = setPacketFieldsAndEnqueue(sensorPacket, serialBusQueue, 0x44, iteration, packetsCounter);
    
    
    
    //------------------------------------------Packets for MagY------------------------------------------
    //Compressed
    for (int i=0; i<NUM_2_BYTE_READINGS; i++) {
      sensorPacket.ArrayType.twoByte[i] = reduceFloat16bit(magnetometerReadings[i].y, 0, 5);
    }
    packetsCounter = setPacketFieldsAndEnqueue(sensorPacket, serialBusQueue, 0x25, iteration, packetsCounter);
    
    //Un-compressed
    for (int i=0; i<NUM_4_BYTE_READINGS; i++) {
      sensorPacket.ArrayType.fourByte[i] = magnetometerReadings[i*2].y;
    }
    packetsCounter = setPacketFieldsAndEnqueue(sensorPacket, serialBusQueue, 0x45, iteration, packetsCounter);
    
    
    
    //------------------------------------------Packets for MagZ------------------------------------------
    //Compressed
    for (int i=0; i<NUM_2_BYTE_READINGS; i++) {
      sensorPacket.ArrayType.twoByte[i] = reduceFloat16bit(magnetometerReadings[i].z, 0, 5);
    }
    packetsCounter = setPacketFieldsAndEnqueue(sensorPacket, serialBusQueue, 0x26, iteration, packetsCounter);
    
    //Un-compressed
    for (int i=0; i<NUM_4_BYTE_READINGS; i++) {
      sensorPacket.ArrayType.fourByte[i] = magnetometerReadings[i*2].z;
    }
    packetsCounter = setPacketFieldsAndEnqueue(sensorPacket, serialBusQueue, 0x46, iteration, packetsCounter);
    
    
    
    //------------------------------------------Packets for UV1------------------------------------------
    //Compressed
    for (int i=0; i<NUM_2_BYTE_READINGS; i++) {
      sensorPacket.ArrayType.twoByte[i] = reduceFloat16bit(uv1Readings[i], 0, 5);
    }
    packetsCounter = setPacketFieldsAndEnqueue(sensorPacket, serialBusQueue, 0x23, iteration, packetsCounter);
    
    //Un-compressed
    for (int i=0; i<NUM_4_BYTE_READINGS; i++) {
      sensorPacket.ArrayType.fourByte[i] = uv1Readings[i*2];
    }
    packetsCounter = setPacketFieldsAndEnqueue(sensorPacket, serialBusQueue, 0x43, iteration, packetsCounter);
    
    
    //etc.
    
    /*---------------------------------------------------
    //In a separate thread, write all queued data packets.
    //--------------------------------------------------- */

    //Don't move on until queue is emptied (to avoid overflow issues).
    while (!isQueueEmpty(serialBusQueue)) {pause(10);}
  
    iteration++;
  }
}


void initializeAllSensors() {
  eeprom_initI2C();
  imu_initialize();
  //temperature1_initialize();
  //temperature2_initialize();
  //radiation_initialize();
  uvInit();
  //ambientLightInit();
  //currentSenseResistor1_initialize();
  //currentSenseResistor2_initialize();
  //solidStateRelay_initialize();
  //microController_initialize();
}  

void flashLEDs() {
  high(26);
  high(27);
  pause(500);
  low(26);
  low(27);
}

uint8_t setPacketFieldsAndEnqueue(Packet sensorPacket, queue *serialBusQueue, uint8_t fnCode, uint8_t iteration, uint8_t packetsCounter) {
  sensorPacket.fnCode = fnCode;      
  sensorPacket.iteration = iteration;
  sensorPacket.packetsCounter = packetsCounter;
  packetsCounter++;
  setPacketCount(packetsCounter);
  enqueue(serialBusQueue, sensorPacket);
  print("Created packet with fnCode: %x\n", sensorPacket.fnCode);
  return packetsCounter;
}  


