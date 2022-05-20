/******************************************************************************/
/* Files to Include                                                           */
/******************************************************************************/
#include <stdint.h>         
#include <stdbool.h>
#include <stdio.h>       
#include "modbus.h"
#include "mcc_generated_files/examples/uart_example.h"
#include "mcc_generated_files/mcc.h"

/* User Functions                                                             */
void clearResponse(void) {
    unsigned char i;
    for (i = 0; i < BUFFERS_SIZE; i++) {
        response[i] = 0;
    }
}

void invalid_address() {
    response[0] = received[0];
    response[1] = received[1] | 0x80;
    response[2] = 0x02;
    unsigned int crc = generateCRC(5);
    response[3] = crc >> 8;
    response[4] = crc;

    for (int i = 0; i < 5; i++) {
        uart[UART0].Write(response[i]);
    }
}

void decodeIt(void) {
    if (received[0] == modbus_address) {
        if (checkCRC()) {
            if (received[1] == 0x03) {
                readReg();
            } else if (received[1] == 0x04) {
                readInputReg();
            } else if (received[1] == 0x06) {
                writeReg();
            } else if (received[1] == 0x10) {
                writeMultipleRegs();
            } else {
               
                response[0] = received[0];
                response[1] = received[1] | 0x80;
                response[2] = 0x01;
                unsigned int crc = generateCRC(5);
                response[3] = crc >> 8;
                response[4] = crc;

                for (int i = 0; i < 5; i++) {
                    uart[UART0].Write(response[i]);
                }
            }
        }
    }
    modbus_message = 0;
    //clearResponse();
}

void readReg(void) {
    unsigned int rr_Address = 0;
    unsigned int rr_numRegs = 0;
    unsigned char j = 3;
    unsigned int crc = 0;
    unsigned int i = 0;

    //Combine address bytes
    rr_Address = received[2];
    rr_Address <<= 8;
    rr_Address |= received[3];

    //Combine number of regs bytes
    rr_numRegs = received[4];
    rr_numRegs <<= 8;
    rr_numRegs |= received[5];

    response[0] = modbus_address;
    response[1] = 0x03;
    response[2] = rr_numRegs * 2; //2 bytes per reg
    if ((rr_Address + rr_numRegs) > NR_OF_REGISTERS) {
        invalid_address();
        return;
    }
    for (i = rr_Address; i < (rr_Address + rr_numRegs); i++) {
        if (holdingReg[i] > 255) {
            //Need to split it up into 2 bytes
            response[j] = holdingReg[i] >> 8;
            j++;
            response[j] = holdingReg[i];
            j++;
        } else {
            response[j] = 0x00;
            j++;
            response[j] = holdingReg[i];
            j++;
        }
    }
    crc = generateCRC(j + 2);
    response[j] = crc >> 8;
    j++;
    response[j] = crc;
    j++;

    
    for (i = 0; i != j; i++) {
        uart[UART0].Write(response[i]);
    }
    
    j = 0;

    clearResponse();
}

void readInputReg(void) {
    unsigned int rr_Address = 0;
    unsigned int rr_numRegs = 0;
    unsigned char j = 3;
    unsigned int crc = 0;
    unsigned int i = 0;

    //Combine address bytes
    rr_Address = received[2];
    rr_Address <<= 8;
    rr_Address |= received[3];

    //Combine number of regs bytes
    rr_numRegs = received[4];
    rr_numRegs <<= 8;
    rr_numRegs |= received[5];

    response[0] = modbus_address;
    response[1] = 0x04;
    response[2] = rr_numRegs * 2; //2 bytes per reg
    if ((rr_Address + rr_numRegs) > NR_OF_REGISTERS) {
        invalid_address();
        return;
    }
    for (i = rr_Address; i < (rr_Address + rr_numRegs); i++) {
        if (holdingReg[i] > 255) {
            //Need to split it up into 2 bytes
            response[j] = holdingReg[i] >> 8;
            j++;
            response[j] = holdingReg[i];
            j++;
        } else {
            response[j] = 0x00;
            j++;
            response[j] = holdingReg[i];
            j++;
        }
    }
    crc = generateCRC(j + 2);
    response[j] = crc >> 8;
    j++;
    response[j] = crc;
    j += 2;

    for (i = 0; i != j; i++) {
        uart[UART0].Write(response[i]);
    }
    j = 0;

    clearResponse();
}

void executarComandos(uint16_t wrAddress, uint16_t valor) {
    holdingReg[wrAddress] = valor;
}

void writeReg(void) {
    /******************************************************************************/
    /* Works out which reg to write and then responds                             */
    /******************************************************************************/
    unsigned int wr_AddressLow = 0;
    unsigned int wr_AddressHigh = 0;
    unsigned int wr_Address = 0;

    unsigned int wr_valToWrite = 0;
    unsigned int wr_valToWriteLow = 0;
    unsigned int wr_valToWriteHigh = 0;

    unsigned int crc = 0;
    // unsigned int i = 0;

    //Combine address bytes
    wr_Address = received[2];
    wr_Address <<= 8;
    wr_Address |= received[3];

    wr_AddressLow = received[3]; //useful to store
    wr_AddressHigh = received[2];

    //Combine value to write regs
    wr_valToWrite = received[4];
    wr_valToWrite <<= 8;
    wr_valToWrite |= received[5];

    wr_valToWriteLow = received[5];
    wr_valToWriteHigh = received[4];

    if (wr_Address > NR_OF_REGISTERS) {
        invalid_address();
        return;
    }
    executarComandos(wr_Address, wr_valToWrite);
    
    response[0] = modbus_address;
    response[1] = 0x06;
    response[3] = wr_AddressLow; //2 bytes per reg
    response[2] = wr_AddressHigh;

    //TO DO CHECK VALUE IS ACTUALLY WRITTEN//
    response[4] = wr_valToWriteHigh;
    response[5] = wr_valToWriteLow;

    crc = generateCRC(8);

    response[6] = crc >> 8;
    response[7] = crc;

    for (unsigned int i = 0; i < 8; i++) {
        uart[UART0].Write(response[i]);
    }

    clearResponse();
}

void writeMultipleRegs(void) {
    /******************************************************************************/
    /* Works out which reg to write and then responds                             */
    /******************************************************************************/
    unsigned int wmr_Address = 0;
    unsigned int wmr_AddressHigh = 0;
    unsigned int wmr_AddressLow = 0;
    unsigned int wmr_numRegs = 0;
    unsigned int wmr_numRegsHigh = 0;
    unsigned int wmr_numRegsLow = 0;
    unsigned int wmr_numBytes = 0;
    unsigned int wmr_numBytesTST = 0;
    unsigned int valToWrite = 0;
    unsigned int valToWriteHigh = 0;
    unsigned int valToWriteLow = 0;

    unsigned char j = 0;
    unsigned int crc = 0;
    unsigned int i = 0;

    //Combine address bytes
    wmr_Address = received[2];
    wmr_Address <<= 8;
    
    //add address high and low bits
    wmr_Address |= received[3];
    wmr_AddressHigh = received[2];
    wmr_AddressLow = received[3];


    //Combine number of regs bytes
    wmr_numRegs = received[4];
    wmr_numRegs <<= 8;
    wmr_numRegs |= received[5];
    wmr_numRegsHigh = received[4];
    wmr_numRegsLow = received[5];

    wmr_numBytes = received[6];

    j = 7;

    wmr_numBytesTST = wmr_numBytes / 2;
    if ((wmr_Address + wmr_numRegs) > NR_OF_REGISTERS) {
        invalid_address();
        return;
    }
    for (i = 0; i < wmr_numBytesTST; i++) {
        valToWrite = received[j];
        valToWrite <<= 8;
        j++;
        valToWrite |= received[j];
        j++;

        executarComandos(wmr_Address + i, valToWrite);
    }

    response[0] = modbus_address;
    response[1] = 0x10;
    response[2] = wmr_AddressHigh;
    response[3] = wmr_AddressLow;
    response[4] = wmr_numRegsHigh;
    response[5] = wmr_numRegsLow;

    crc = generateCRC(8);

    response[6] = crc >> 8;
    response[7] = crc;

    for (i = 0; i < 8; i++) {
        uart[UART0].Write(response[i]);
    }

    j = 0;

    clearResponse();

}

unsigned int generateCRC(unsigned char message_length) {
    unsigned int crc = 0xFFFF;
    unsigned int crcHigh = 0;
    unsigned int crcLow = 0;
    int i, j = 0;

    for (i = 0; i < message_length - 2; i++) {
        crc ^= response[i];
        for (j = 8; j != 0; j--) {
            if ((crc & 0x0001) != 0) {
                crc >>= 1;
                crc ^= 0xA001;
            } else {
                crc >>= 1;
            }
        }
    }
    //bytes are wrong way round so doing a swap here..
    crcHigh = (crc & 0x00FF) << 8;
    crcLow = (crc & 0xFF00) >> 8;
    crcHigh |= crcLow;
    crc = crcHigh;
    return crc;
}

unsigned char checkCRC(void) {
    unsigned int crc = 0xFFFF;
    unsigned int crcHigh = 0;
    unsigned int crcLow = 0;
    int i, j = 0;

    for (i = 0; i < message_length - 2; i++) {
        crc ^= received[i];
        for (j = 8; j != 0; j--) {
            if ((crc & 0x0001) != 0) {
                crc >>= 1;
                crc ^= 0xA001;
            } else {
                crc >>= 1;
            }
        }
    }
    //bytes are wrong way round so doing a swap here..
    crcHigh = (crc & 0x00FF);
    crcLow = (crc & 0xFF00) >> 8;
    if ((crcHigh == received[i])&&(crcLow == received[i + 1])) {
        return 1;
    } else {
        return 0;
    }
}