/*
 * Author: Mitchell Toth
 * Modification Date: April 5, 2020
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
        //Create "general sensor values" packet.
        generalSensorPacket.fnCode = 0x01;
        //Add data
        generalSensorPacket.iteration = iteration;
        generalSensorPacket.packetsCounter = packetsCounter;
        packetsCounter++;
        setPacketCount(packetsCounter);
        enqueue(serialBusQueue, generalSensorPacket);
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
   
   
    //-------------------------Packets for UV-------------------------
    //Packet for UV1 (compressed)
    Packet sensorPacket;
    sensorPacket.fnCode = 0x23;
    for (int i=0; i<NUM_2_BYTE_READINGS; i++) {
      sensorPacket.ArrayType.twoByte[i] = reduceFloat16bit(uv1Readings[i], 0, 5);
    }
    sensorPacket.iteration = iteration;
    sensorPacket.packetsCounter = packetsCounter;
    packetsCounter++;
    setPacketCount(packetsCounter);
    enqueue(serialBusQueue, sensorPacket);
    
    //Packet for UV1 (un-compressed)
    sensorPacket.fnCode = 0x43;
    for (int i=0; i<NUM_4_BYTE_READINGS; i++) {
      sensorPacket.ArrayType.fourByte[i] = uv1Readings[i*2];
    }
    sensorPacket.iteration = iteration;
    sensorPacket.packetsCounter = packetsCounter;
    packetsCounter++;
    setPacketCount(packetsCounter);
    enqueue(serialBusQueue, sensorPacket);
    
    
    
    //-------------------------Packets for MagX-------------------------
    //Packet for MagX (compressed)
    sensorPacket.fnCode = 0x24;
    for (int i=0; i<NUM_2_BYTE_READINGS; i++) {
      sensorPacket.ArrayType.twoByte[i] = reduceFloat16bit(magnetometerReadings[i].x, 0, 5);
    }
    sensorPacket.iteration = iteration;
    sensorPacket.packetsCounter = packetsCounter;
    packetsCounter++;
    setPacketCount(packetsCounter);
    enqueue(serialBusQueue, sensorPacket);
    
    //Packet for MagX (un-compressed)
    sensorPacket.fnCode = 0x44;
    for (int i=0; i<NUM_4_BYTE_READINGS; i++) {
      sensorPacket.ArrayType.fourByte[i] = magnetometerReadings[i*2].x;
    }
    sensorPacket.iteration = iteration;
    sensorPacket.packetsCounter = packetsCounter;
    packetsCounter++;
    setPacketCount(packetsCounter);
    enqueue(serialBusQueue, sensorPacket);
    
    
    
    //-------------------------Packets for MagY-------------------------
    //Packet for MagY (compressed)
    sensorPacket.fnCode = 0x25;
    for (int i=0; i<NUM_2_BYTE_READINGS; i++) {
      sensorPacket.ArrayType.twoByte[i] = reduceFloat16bit(magnetometerReadings[i].y, 0, 5);
    }
    sensorPacket.iteration = iteration;
    sensorPacket.packetsCounter = packetsCounter;
    packetsCounter++;
    setPacketCount(packetsCounter);
    enqueue(serialBusQueue, sensorPacket);
    
    //Packet for MagY (un-compressed)
    sensorPacket.fnCode = 0x45;
    for (int i=0; i<NUM_4_BYTE_READINGS; i++) {
      sensorPacket.ArrayType.fourByte[i] = magnetometerReadings[i].y;
    }
    sensorPacket.iteration = iteration;
    sensorPacket.packetsCounter = packetsCounter;
    packetsCounter++;
    setPacketCount(packetsCounter);
    enqueue(serialBusQueue, sensorPacket);
    
    
    
    //-------------------------Packets for MagZ-------------------------
    //Packet for MagZ (compressed)
    sensorPacket.fnCode = 0x26;
    for (int i=0; i<NUM_2_BYTE_READINGS; i++) {
      sensorPacket.ArrayType.twoByte[i] = reduceFloat16bit(magnetometerReadings[i].z, 0, 5);
    }
    sensorPacket.iteration = iteration;
    sensorPacket.packetsCounter = packetsCounter;
    packetsCounter++;
    setPacketCount(packetsCounter);
    enqueue(serialBusQueue, sensorPacket);
    
    //Packet for MagZ (un-compressed)
    sensorPacket.fnCode = 0x46;
    for (int i=0; i<NUM_4_BYTE_READINGS; i++) {
      sensorPacket.ArrayType.fourByte[i] = magnetometerReadings[i].z;
    }
    sensorPacket.iteration = iteration;
    sensorPacket.packetsCounter = packetsCounter;
    packetsCounter++;
    setPacketCount(packetsCounter);
    enqueue(serialBusQueue, sensorPacket);
    
    
    //etc.
    
    
    /*---------------------------------------------------
    //--To be done in a separate file. Jeff.--
    	//Now write all queued data packets -- however long it takes: 
    	//At 1 packet/15 seconds, N sensor packets/4 minutes for full transmission.
    
    	//'writeAllPackets' polls busy line, writes one packet, removes packet from queue
    	//writeAllPackets();
      //Separate thread. Writes to serial port.
    //--------------------------------------------------- */

    //Don't move on until queue is emptied. Because we don't want overflow issues.
    while (!isQueueEmpty(serialBusQueue)) {pause(10);}
  
    iteration++;  //Maybe in EEPROM
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

/*
uint8_t createSensorPackets(queue *serialBusQueue, float *readings, int compressData, uint8_t fnCode, uint8_t iteration, uint8_t packetsCounter, int isSigned, int k) {
  Packet sensorPacket;
  sensorPacket.fnCode = fnCode;
  
  if (compressData) {
    for (int i=0; i<NUM_2_BYTE_READINGS; i++) {
      sensorPacket.ArrayType.twoByte[i] = reduceFloat16bit(readings[i], isSigned, k);
    }
  }  
  else {
    for (int i=0; i<NUM_4_BYTE_READINGS; i++) {
      sensorPacket.ArrayType.fourByte[i] = readings[i*2], isSigned, k);
    }
  }      
  
  sensorPacket.iteration = iteration;
  sensorPacket.packetsCounter = packetsCounter;
  packetsCounter++;
  setPacketCount(packetsCounter);
  enqueue(serialBusQueue, sensorPacket);
  return packetsCounter;
}  
*/

