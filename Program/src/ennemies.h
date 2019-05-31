/*
 * ennemies.h
 *
 *  Created on: 30 mai 2019
 *      Author: valpiros
 */

#ifndef ENNEMIES_H_
#define ENNEMIES_H_

typedef enum {right, left} direction;

typedef struct{
	u_int8 min_x;
	u_int8 max_x;
	u_int8 max_y;
} extremum;

void ennemy_type1 (T_list *p_ship_list);
void ennemy_type2 (T_list *p_ship_list);
void ennemy_moving (T_list *p_ship_list, direction *fleet, extremum *ennemy);
u_int8 ennemy_defeated (T_list *p_ship_list);
void new_minmax (T_list *p_ship_list, extremum *ennemy);

#endif /* ENNEMIES_H_ */
