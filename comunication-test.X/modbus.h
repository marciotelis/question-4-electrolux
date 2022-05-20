#include <stdint.h>

/******************************************************************************/
/* User Level #define Macros                                                  */
/******************************************************************************/
#define NR_OF_REGISTERS 20
#define BUFFERS_SIZE 125

/******************************************************************************/
/* Global variables                                                           */
/******************************************************************************/
volatile int holdingReg[NR_OF_REGISTERS];
volatile char response[BUFFERS_SIZE];
volatile char received[BUFFERS_SIZE];
volatile char modbus_message, message_length;
unsigned long time_last_recieved;
unsigned char modbus_address = 1;

/******************************************************************************/
/* Pointers                                                                   */
/******************************************************************************/


/******************************************************************************/
/* User Function Prototypes                                                   */
/******************************************************************************/
unsigned char checkCRC(void);
void clearResponse(void);
void readReg(void);         /* I/O and Peripheral Initialization */
void readInputReg(void);
void writeReg(void);
void writeMultipleRegs(void);
void modbusDelay(void);              /* Writes to Timer0 for 1.04ms delay*/
void decodeIt(void);
unsigned int generateCRC(unsigned char);