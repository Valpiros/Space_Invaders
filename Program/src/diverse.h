/*
 * type_others.h
 *
 *  Created on: 7 janv. 2019
 *      Author: valpiros
 */
#include <stdint.h>
#include <stm32f4xx.h>
#include <stm32f4_discovery.h>
#include <serial.h>
#include <vt100.h>
#include <list.h>
//#include "heap.h"

#ifndef TYPE_OTHERS_H_
#define TYPE_OTHERS_H_

typedef unsigned char u_int8;
typedef unsigned short u_int16;
typedef unsigned long u_int32;
typedef char int8;

typedef struct {
	u_int8 x;
	u_int8 y;
	u_int8 lenght;
	char skin[9];
} pos;

extern pos ship;
extern u_int8 ship_size;

typedef enum {INIT, LOGO,TEST, START, GAME, END} state;

//typedef enum {loose, win} end_game;

void border_init (void);
void delay (u_int32 a);
u_int8 Ps_RandomNumberGeneratory (void);
void serial_putship (void);
void move_ship (pos *ship, u_int8 ship_size);
void ally_shooting (u_int8 *cd_shoot, T_list *p_shoot_list, pos *ship);



#endif /* TYPE_OTHERS_H_ */
