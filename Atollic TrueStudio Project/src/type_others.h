/*
 * type_others.h
 *
 *  Created on: 7 janv. 2019
 *      Author: valen
 */

#include "stm32f4xx.h"
#include "stm32f4_discovery.h"
#include <serial.h>
#include <vt100.h>
#include <stdint.h>

#ifndef TYPE_OTHERS_H_
#define TYPE_OTHERS_H_

typedef unsigned char u_int8;
typedef unsigned short u_int16;
typedef unsigned long u_int32;



typedef struct {
	u_int8 x;
	u_int8 y;
} pos;

extern pos ship;
extern u_int8 ship_size;
extern u_int8 PRNG;
typedef enum {shoot_ennemy, shoot_ally} team;

typedef struct {
	u_int8 x;
	u_int8 y;
	team who;		// shoot_ennemy : 0 / shoot_ally : 1
} shoot_pos;

void move_ship (pos *ship, u_int8 ship_size);
void serial_putship ();
void serial_shoot (shoot_pos *shoot_tab, u_int8 i);
void border_init ();
void move_shoots (shoot_pos *shoot_tab);
void delay (u_int32 a);
void ennemy_type1 (pos *ennemy_tab, u_int8 *lenght_ship);
void ennemy_shooting (pos *ennemy_tab, shoot_pos *shoot_tab);
void hitbox (pos *ennemy_tab, shoot_pos *shoot_tab, pos *shi, u_int8 lenght);
u_int8 Ps_RandomNumberGeneratory ();
void ally_shooting (u_int8 *cd_shoot, shoot_pos *shoot_tab);

#endif /* TYPE_OTHERS_H_ */
