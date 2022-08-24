
# Modem

## Tool

  STM32CubeIDE

  Version: 1.4.2
  Build: 7643_20200813_1322 (UTC)

  (C) 2020 STMicroelectronics ALL RIGHTS RESERVED

## Purpose

  This library is integrated in a bigger project and is now being shared to help you going faster through your project.

  It has been used with STM32L476 but should be compatible with any version of STM32L4xx

## Dependencies

  Not crucial:
    logging - https://github.com/stm32l4xx-packages/logging

## Layer 1

## Functionalities

  Build modbus packet
  Decode modbus packet
  checks crc packet

  Then you can use serial package to send and received data from uart

## Examples

  extern ModbusRTU modbus;

  modbus_rtu* rtu = new modbus_rtu;

  rtu->slave_id = 1;
	rtu->fc = 3;
	rtu->address = 1000;
	rtu->len = 2; // nmbr of registers
  rtu->data[0] = 0x0A;
  rtu->data[1] = 0x0B;
  rtu->data[2] = 0x0C;
  rtu->data[3] = 0x0D;
  
  uint16_t len = modbus.encode(rtu, frame,6+1+rtu->len*2);
