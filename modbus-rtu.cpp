/*
 * modbus-rtu.cpp
 *
 *  Created on: Nov 27, 2018
 *      Author: jsilva
 */


 #include "modbus-rtu.h"

ModbusRTU modbus;

 bool ModbusRTU::valid(uint8_t *data, uint16_t len) {

  if(len < 3) return false;

 	uint16_t crc = ModbusRTU::crc(data, len - 2);

 	return ( (((uint8_t) crc) == data[len - 2]) &&
 	         (((uint8_t) (crc >> 8)) == data[len - 1]) );
 }

 bool ModbusRTU::decode(uint8_t *data, uint16_t len, modbus_rtu* rtu) {

  if(data[0] != rtu->slave_id) return false;
 	rtu->fc = data[1];
  rtu->len = data[2];
  if(rtu->len > (len-5)){
    rtu->len = 0;
    return false;
  }

  /*
  if(rtu->len+5 > len)
    return false;
  */
 	rtu->crc          = data[3+rtu->len] | (data[3+rtu->len+1] << 8);
 	rtu->data         = &data[3];

  #ifdef DEBUG_MODBUS
    sprintf(logging.log_msg," >> ");
    logging.log_nnl(logging.log_msg,"modbus");

    uint8_t i = 0;
    while(i < len){
      sprintf(logging.log_msg,"%x ", data[i]);
      logging.log_nnl(logging.log_msg," ");
      i++;
    }

    sprintf(logging.log_msg,"--- message received ---");
    logging.log(logging.log_msg,"modbus");
    sprintf(logging.log_msg,"slave: 0x%x", rtu->slave_id);
    logging.log(logging.log_msg,"modbus");
    sprintf(logging.log_msg,"fc: 0x%x", rtu->fc);
    logging.log(logging.log_msg,"modbus");
    sprintf(logging.log_msg,"length: 0x%x", rtu->len);
    logging.log(logging.log_msg,"modbus");
    sprintf(logging.log_msg,"crc: 0x%x", rtu->crc);
    logging.log(logging.log_msg,"modbus");

    sprintf(logging.log_msg,"data: ");
    logging.log_nnl(logging.log_msg,"modbus");

    i = 0;
    while(i < (rtu->len)){
      sprintf(logging.log_msg,"0x%x ", rtu->data[i]);
      logging.log_nnl(logging.log_msg,"modbus-rtu");
      i++;
    }

    sprintf(logging.log_msg,"   -----    ");
    logging.log(logging.log_msg,"modbus");
 	#endif

  if(rtu->fc > 0x80){
    logging.log("error returned on modbus","modbus");
    return false;
  }

 	return true;
 }

// return size
 uint16_t ModbusRTU::encode(modbus_rtu* rtu, uint8_t *data, uint16_t size) {
  uint16_t i = 0;
 	data[i++] = rtu->slave_id;
 	data[i++] = rtu->fc;
 	data[i++] = rtu->address >> 8;
 	data[i++] = rtu->address;
 	data[i++] = (uint8_t) (rtu->len >> 8);
  data[i++] = rtu->len;

  uint8_t j = 0;
  if(rtu->fc == 15 || rtu->fc == 16){
    while(i<size){
      data[i++] = rtu->data[j++];
    }
  }

 	uint16_t crc = ModbusRTU::crc(&data[0], size);

  data[size] = (uint8_t) (crc);
  data[size+1] = (uint8_t) (crc >> 8);


  #ifdef DEBUG_MODBUS
    sprintf(logging.log_msg,"--- message to send ---");
    logging.log(logging.log_msg,"modbus");
    sprintf(logging.log_msg,"slave: 0x%x", rtu->slave_id);
    logging.log(logging.log_msg,"modbus");
    sprintf(logging.log_msg,"fc: 0x%x", rtu->fc);
    logging.log(logging.log_msg,"modbus");
    sprintf(logging.log_msg,"address: 0x%x", rtu->address);
    logging.log(logging.log_msg,"modbus");
    sprintf(logging.log_msg,"length: 0x%x", rtu->len);
    logging.log(logging.log_msg,"modbus");
    sprintf(logging.log_msg,"crc: 0x%x", crc);
    logging.log(logging.log_msg,"modbus");
    sprintf(logging.log_msg,"   -----    ");
    logging.log(logging.log_msg,"modbus");
 	#endif


 	return size+2;
 }

 uint16_t ModbusRTU::crc(uint8_t *buf, uint16_t len) {
 	uint16_t crc = 0xFFFF;

 	for (int pos = 0; pos < len; pos++) {
 		crc ^= (uint16_t) buf[pos];

 		for (int i = 8; i != 0; i--) {
 			if ((crc & 0x0001) != 0) {
 				crc >>= 1;
 				crc ^= 0xA001;
 			} else {
 				crc >>= 1;
 			}
 		}
 	}

 	return crc;
 }
