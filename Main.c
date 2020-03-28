//Main.c
//Purpose: Coordinate and control all sending/receiving of sensor data.

#include "simpletools.h"
#include "UVSensor.h"
#include "IMUSensor.h"


//Function declarations.
void flashLEDs();
void initializeAllSensors();

const int NUM_1_BYTE_READINGS = 32;
const int NUM_2_BYTE_READINGS = 16;

//--Need serial bus queue. Caleb.--

struct Packet {
  uint8_t fnCode;
  union {
    uint8_t oneByte[32];
    //Use 16-bit float below maybe.
    uint16_t twoByte[16];
  } ArrayType;
  uint8_t iteration;
  uint8_t packetsCounter;
};


//Check if compiler has queue type already.
//volatile Packet serialBusQueue[10];

//0 = not calibrated, 1 = calibrated, global to all threads.
volatile int magnetometerCalibrated = 0;


//Main function
int main() {
  
  //LED flash to show that the parallax board is connected and working.
  flashLEDs();
  
  //Powerup event occurs.
  //Ensure IMU is powered up. Involves solid state relay.
  //ensureIMUPowered();
  
  //Get all the sensors ready to read and transmit data.
  initializeAllSensors();
  
  //Keep track of total packets sent, as well as main loop iteration.
  uint8_t packetsCounter = 0;
  uint8_t iteration = 0;

  struct Vector3 accelerationReadings[NUM_2_BYTE_READINGS];
  struct Vector3 gyroscopeReadings[NUM_2_BYTE_READINGS];
  struct Vector3 magnetometerReadings[NUM_2_BYTE_READINGS];
  float uv1Readings[NUM_2_BYTE_READINGS];
  
  //Make arrays for each sensor, assume everything requires 2 bytes.
  
  struct Packet generalSensorPacket;
  
  
  //Main loop, iterate as long as there is power:
  while (1) {
  	//The goal is to gather and store full packets of data specific to each sensor.
    //While gathering enough data to do this, we create "general sensor values" packets.
    //This allows us to at least send some data immediately when the serial busy line goes low.
    
  	//For approximately 1 minute (15 iterations -- approx 4 sec each):
    for (int i=0; i<32; i++) {
  		//If iteration is a multiple of 8 (0, 8, 16, 24):		// Sec: 0, 16, 32, 48
      if (i%8 == 0) {
        //Create "general sensor values" packet.
        //Add packet to serial bus write queue.
        //Packet generalSensorPacket;
        //serialBusQueue.enqueue(generalSensorPacket);
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
  	//serialBusQueue.clear();
   
    //Packet for UV
    struct Packet sensorPacket;
    sensorPacket.fnCode = 0x03;
    for (int i=0; i<NUM_2_BYTE_READINGS; i++) {
      sensorPacket.ArrayType.twoByte[i] = uv1Readings[i];
    }
    sensorPacket.iteration = iteration;
    sensorPacket.packetsCounter = packetsCounter;
    packetsCounter++;
    
    //enqueue sensorPacket
    //Now re-do sensorPacket for other sensors...
    
    //Packet for MagX
    sensorPacket.fnCode = 0x04;
    for (int i=0; i<NUM_2_BYTE_READINGS; i++) {
      sensorPacket.ArrayType.twoByte[i] = magnetometerReadings[i].x;
    }
    sensorPacket.iteration = iteration;
    sensorPacket.packetsCounter = packetsCounter;
    packetsCounter++;
    
    //enqueue sensorPacket
    
    //Packet for MagY
    sensorPacket.fnCode = 0x05;
    for (int i=0; i<NUM_2_BYTE_READINGS; i++) {
      sensorPacket.ArrayType.twoByte[i] = magnetometerReadings[i].y;
    }
    sensorPacket.iteration = iteration;
    sensorPacket.packetsCounter = packetsCounter;
    packetsCounter++;
    
    //enqueue sensorPacket
    
    //Packet for MagZ
    sensorPacket.fnCode = 0x06;
    for (int i=0; i<NUM_2_BYTE_READINGS; i++) {
      sensorPacket.ArrayType.twoByte[i] = magnetometerReadings[i].z;
    }
    sensorPacket.iteration = iteration;
    sensorPacket.packetsCounter = packetsCounter;
    packetsCounter++;
    
    //enqueue sensorPacket
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
    //Maybe use a shared variable. Or maybe proceed when queue size is low enough.
    //while (!queueEmpty) {pause(10);}
  
    iteration++;
  }
}


void initializeAllSensors() {
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