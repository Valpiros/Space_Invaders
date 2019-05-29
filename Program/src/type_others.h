/*
 * type_others.h
 *
 *  Created on: 7 janv. 2019
 *      Author: valen
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
	u_int8 alive;
	u_int8 lenght;
	char skin[8];
} pos;

typedef struct{
	u_int8 min_x;
	u_int8 max_x;
	u_int8 max_y;
} extremum;

extern pos ship;
extern u_int8 ship_size;

typedef enum {shoot_ennemy, shoot_ally} team;
typedef enum {INIT, LOGO,TEST, START, GAME, END} state;
typedef enum {right, left} direction;
//typedef enum {loose, win} end_game;

typedef struct {
	u_int8 x;
	u_int8 y;
	team who;		// shoot_ennemy : 0 / shoot_ally : 1
} shoot_pos;

void move_ship (pos *ship, u_int8 ship_size);
void serial_putship (void);
//void serial_shoot (shoot_pos *shoot_tab, u_int8 i);
void border_init (void);
void move_shoots (T_list *p_shoot_list);
void delay (u_int32 a);
void ennemy_type1 (T_list *p_ship_list, extremum *ennemy);
void ennemy_type2 (T_list *p_ship_list, extremum *ennemy);
void ennemy_shooting (T_list *p_ship_list, T_list *p_shoot_list, pos *ship);
void hitbox (T_list *p_ship_list, T_list *p_shoot_list, pos *ship, u_int8 *lives);
u_int8 Ps_RandomNumberGeneratory (void);
void ally_shooting (u_int8 *cd_shoot, T_list *p_shoot_list, pos *ship);
void ennemy_moving (T_list *p_ship_list, direction *fleet, extremum *ennemy);
u_int8 ennemy_defeated (T_list *p_ship_list);
void new_minmax (T_list *p_ship_list, extremum *ennemy);
void missile_new (T_list *p_shoot_list, team owner, u_int8 X, u_int8 Y);
void shoot_suppr (shoot_pos *shoot);

#endif /* TYPE_OTHERS_H_ */
