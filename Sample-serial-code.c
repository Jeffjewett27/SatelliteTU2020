/**
 * @file Sample-serial-code.c
 * @brief Sample code writing/reading from Parallax Propeller mini to ThinSat.
 * @author Stefan Brandle, The ThinSat 2020 team
 */

#include "simpletools.h"
#include "fdserial.h"

const int BUSY_LINE = 10;

/*
int main() {
    char ack1 = 'A', ack2='B', ack3='C';

    fdserial *sr = fdserial_open(8, 14, 0, 38400);

    while(1) {       

        // Wait for Busy to be deasserted.
        while(input(BUSY_LINE) == 1) {
            pause(100);
        }

        // flush rx/tx buffers to be safe
        fdserial_rxFlush(sr);
        fdserial_txFlush(sr);

        // Send the preamble
        fdserial_txChar(sr, 0x50);
        fdserial_txChar(sr, 0x50);
        fdserial_txChar(sr, 0x50);

        // Send 35-byte test pattern:  ABCDEF ... ('A'+34)
        for(int i=0; i<35; i++) {
            fdserial_txChar(sr, (char)('A'+i));
        }

        // Print size of RX buffer
        print("Count=%d\n", fdserial_rxCount(sr));

        // Read 1st letter of ACK/NACK
        while(fdserial_rxCount(sr)==0) {        // Wait until there is something in the RX buffer
            pause(10);
        }
        ack1 = fdserial_rxChar(sr);
        print("A1:%2x, ", ack1);

        // Read 2nd letter of ACK/NACK
        while(fdserial_rxCount(sr)==0) {
            pause(10);
        }
        ack2 = fdserial_rxChar(sr);
        print("A2:%2x, ", ack2);

        // Read 3rd letter of ACK/NACK
        while(fdserial_rxCount(sr)==0) {
            pause(10);
        }
        ack3 = fdserial_rxChar(sr);
        print("A3:%2x\n", ack3);

		// Stop loop for 1 second -- could skip this entirely and loop round to wait for BUSY deassert
        pause(1000);            
    }

    return 0;
}
*/