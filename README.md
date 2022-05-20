# Question 4

## Description

> Imagine that you have a microcontroller that communicates to a generic system that may consist of several other boards via UART. How do you guarantee each message you send is delivered correctly?  
> Now imagine that you receive a message and an interruption is triggered every time a new information is received. Inside every message you have a different command, and for every command you have different payloads, receiving positive and negative values.  
> Implement the interrupt and the parser functions in a generic way using the C language and share this code through a public repository on GitHub. Low-level methods can be abstracted. If you prefer, use frameworks like Arduino, ESP32 or even other market platforms in this low-level code abstraction.

## Comments

> The exercise asked to perform a communication that has different commands with different payloads, with both positive and negative values. I implemented the modbus protocol, as it has these characteristics and more, being very robust and widely used.  
- It checks if the message is for itself (via addressing, in the exercise software I left it set to 1, it can be selected via software and/or hardware)
- Check which command received
- Handles the command, adapting the message reading according to the command
- Save payload in registers
- At the end, it performs the checksum (CRC) to certify that the message received is correct  

> Main code flow:  
> In eusart1.c the interrupt is handled by saving each incoming byte. The timer checks when each message has ended (by the time between receiving one byte and another). With the message ready, the decodeIt() function is called, which is in the modbus.c file. In this, the treatment of the message is carried out, with the implementation of the protocol and the storage of the payload in the registers.

## Features

- [x] UART driver
- [x] CRC
- [x] Comunication protocol
- [x] Interruption for incoming messages

## Build settings

> For this application I considered using an 8 bit PIC as hardware, to abstract the low-level code (device config, interrupts, timer, inputs).
>- xc8 compiler (v2.36)
>- MPLAB X IDE (v5.50)
>- MCC driver (v2.0)

## Languages used

>- C

## Authors and contributions

> Márcio Telis Silveira
