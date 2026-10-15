# BLUETOOTH-BASED-DIGITAL-NOTICE-BOARDS
 The Bluetooth-Based Digital Notice Board is an embedded system that wirelessly displays text messages on a 16×2 LCD using Bluetooth communication. The system enables real-time notice updates from an Android smartphone without requiring physical access to the display.
## Features
- Wireless message transmission using Bluetooth
- Real-time message display
- Scrolling text display
- Message storage using EEPROM
- Automatic message recovery after power loss
- ## Hardware Components
- LPC2129 ARM7 Microcontroller
- HC-05 Bluetooth Module
- AT24C256 EEPROM
- 8×8 LED Dot Matrix Displays
- 74HC164 Shift Registers
- 74HC573 Latch IC
- ## Software Used
- Embedded C
- Keil µVision
- Flash Magic
- ## Working Principle
The system receives messages from a smartphone
through the HC-05 Bluetooth module. The LPC2129
microcontroller receives data through UART,
stores the message in EEPROM using I2C, and
displays the message as scrolling text on LED
dot matrix displays.
## Demo Video
