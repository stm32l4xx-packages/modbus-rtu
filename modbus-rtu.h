/*
 * modbus-rtu.h
 *
 *  Created on: Nov 27, 2018
 *      Author: jsilva
 */

#ifndef INC_MODBUS_RTU_H_
#define INC_MODBUS_RTU_H_

//#include "package.h"
#include "logging.h"

#ifdef __cplusplus
 extern "C" {
#endif

struct modbus_rtu{
	uint8_t  slave_id;
	uint8_t  fc;
	uint16_t address;
	uint16_t len;
	uint8_t* data;
	uint16_t crc;
};

class ModbusRTU {
public:
	bool valid(uint8_t *data, uint16_t len);
	bool decode(uint8_t *data, uint16_t len, modbus_rtu* rtu);
	uint16_t encode(modbus_rtu* rtu, uint8_t *data, uint16_t size);
	uint16_t crc(uint8_t *buf, uint16_t len);
};

#ifdef __cplusplus
}
#endif

extern ModbusRTU modbus;

#endif /* INC_MODBUS_RTU_H_ */
