#ifndef __My_MAX30102__
#define __My_MAX30102__

#include <stdint.h>

//my_max30102_setWR.c
void set_write_2(uint8_t i,uint8_t j);
uint8_t set_read_1(uint8_t i,uint8_t j);
void set_write_3(uint8_t i,uint8_t j,uint8_t k);

//my_max30102.c
uint8_t hr_spo2_part_id(void);
uint8_t hr_spo2_revision_id(void);
void hr_spo2_init(void);
uint8_t hr_spo2_wait_PR_ready(void);
uint8_t hr_spo2_wait_WR_ready(void);
void hr_spo2_get_data(char *buf);

#endif
