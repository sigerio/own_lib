#ifndef _CRC32_H_
#define _CRC32_H_

#include "stdint.h"


void CRC32Init(void);
uint32_t GetCRC32(uint8_t* buf, uint32_t len);
uint8_t crc8( uint8_t * p_buffer, uint16_t buf_size );
uint16_t crc16( uint8_t* data, uint16_t len);

#endif
