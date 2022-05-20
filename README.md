# Question 4

## Description

> Imagine that you have a microcontroller that communicates to a generic system that may consist of several other boards via UART. How do you guarantee each message you send is delivered correctly?  
> Now imagine that you receive a message and an interruption is triggered every time a new information is received. Inside every message you have a different command, and for every command you have different payloads, receiving positive and negative values.  
> Implement the interrupt and the parser functions in a generic way using the C language and share this code through a public repository on GitHub. Low-level methods can be abstracted. If you prefer, use frameworks like Arduino, ESP32 or even other market platforms in this low-level code abstraction.

## Comments

## Features

- [ ] UART driver
- [ ] CRC
- [ ] Comunication protocol
- [ ] Interruption for incoming messages

## Test program

## Build settings

> For this application I considered using an 8 bit PIC as hardware, to abstract the low-level code (device config, interrupts, timer, inputs).
>- xc8 compiler (v2.36)
>- MPLAB X IDE (v5.50)
>- MCC driver (v2.0)

## Languages used

>- C

## Authors and contributions

> Márcio Telis Silveira
