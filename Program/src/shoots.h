/*
 * shoots.h
 *
 *  Created on: 30 mai 2019
 *      Author: valpiros
 */

#ifndef SHOOTS_H_
#define SHOOTS_H_

typedef enum {shoot_ennemy, shoot_ally} team;

typedef struct {
	u_int8 x;
	u_int8 y;
	team who;		// shoot_ennemy : 0 / shoot_ally : 1
} shoot_pos;

void move_shoots (T_list *p_shoot_list);
void ennemy_shooting (T_list *p_ship_list, T_list *p_shoot_list, pos *ship);
void hitbox (T_list *p_ship_list, T_list *p_shoot_list, pos *ship, u_int8 *lives);
void shoot_new (T_list *p_shoot_list, team owner, u_int8 X, u_int8 Y);

#endif /* SHOOTS_H_ */
