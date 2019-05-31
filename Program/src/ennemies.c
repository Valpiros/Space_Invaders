/*
 * ennemies.c
 *
 *  Created on: 30 mai 2019
 *      Author: valpiros
 */
#include <stdint.h>
#include <stm32f4xx.h>
#include <stm32f4_discovery.h>
#include "serial.h"
#include "diverse.h"
#include "vt100.h"
#include "list.h"
#include "heap.h"
#include "shoots.h"
#include "ennemies.h"
#include "conf.h"

void ennemy_type1 (T_list *p_ship_list)
{
	u_int8 index;
	//ennemies are center on the screen
	u_int8 init_x = (VT100_SCREEN_WIDTH - (LENGHT1+DISTANCE_SHIP1)*NUMBER_SHIP1)/2;
	u_int8 init_y = 6;

	pos *p_ship;
	T_element *ship_ele;

	for (index=0;index<NUMBER_SHIP1;index++)
	{
		p_ship = heap_malloc (sizeof(pos));
		if (p_ship != NULL)
		{
			p_ship -> lenght = LENGHT1;
			p_ship -> skin[0] = SKIN1_0;
			p_ship -> skin[1] = SKIN1_1;
			p_ship -> skin[2] = SKIN1_2;
			p_ship -> skin[3] = SKIN1_3;
			p_ship -> skin[4] = SKIN1_4;
			p_ship -> skin[5] = SKIN1_5;
			p_ship -> skin[6] = SKIN1_6;
			p_ship -> skin[7] = SKIN1_7;
			p_ship -> skin[8] = SKIN1_8;
			p_ship -> x = init_x+(p_ship -> lenght+DISTANCE_SHIP1)*index;
			p_ship -> y = init_y;
			vt100_move(init_x+(p_ship -> lenght+DISTANCE_SHIP1)*index,init_y);
			serial_puts(p_ship -> skin);

			ship_ele = list_create_element (p_ship);
			list_prepend (p_ship_list, ship_ele);
		}
	}
	return;
}

void ennemy_type2 (T_list *p_ship_list)
{
	u_int8 index;
	u_int8 init_x = (VT100_SCREEN_WIDTH - (LENGHT2+DISTANCE_SHIP2)*NUMBER_SHIP2)/2;;
	u_int8 init_y = 7;
	pos *p_ship;
	T_element *ship_ele;

	for (index=0;index<NUMBER_SHIP2;index++)
	{
		p_ship = heap_malloc (sizeof(pos));
		if (p_ship != NULL)
		{
			p_ship -> lenght = LENGHT2;
			p_ship -> skin[0] = SKIN2_0;
			p_ship -> skin[1] = SKIN2_1;
			p_ship -> skin[2] = SKIN2_2;
			p_ship -> skin[3] = SKIN2_3;
			p_ship -> skin[4] = SKIN2_4;
			p_ship -> skin[5] = SKIN2_5;
			p_ship -> skin[6] = SKIN2_6;
			p_ship -> skin[7] = SKIN2_7;
			p_ship -> skin[8] = SKIN2_8;
			p_ship -> x = init_x+(p_ship -> lenght+DISTANCE_SHIP2)*index;
			p_ship -> y = init_y;
			vt100_move(init_x+(p_ship -> lenght+DISTANCE_SHIP2)*index,init_y);
			serial_puts(p_ship -> skin);

			ship_ele = list_create_element (p_ship);
			list_prepend (p_ship_list, ship_ele);
		}
	}
	return;
}

void ennemy_moving (T_list *p_ship_list, direction *fleet, extremum *ennemy)
{
	T_element *p_element = p_ship_list -> start;
	pos *ennemy_ship;
	if (*fleet == right)
	{
		if (ennemy->max_x > 117)
		{
			*fleet = left;
			while (p_element != NULL)
			{
				ennemy_ship = (pos *)(p_element->data);
				p_element = p_element -> next;

				vt100_move (ennemy_ship ->x, ennemy_ship ->y);
				serial_puts ("      ");
				ennemy_ship ->y += 1;
				vt100_move (ennemy_ship ->x, ennemy_ship ->y);
				serial_puts (ennemy_ship ->skin);
			}
		}
		else
		{
			while (p_element != NULL)
			{
				ennemy_ship = (pos *)(p_element->data);
				p_element = p_element -> next;

				vt100_move (ennemy_ship ->x, ennemy_ship ->y);
				serial_putchar (' ');
				serial_puts (ennemy_ship ->skin);
				ennemy_ship ->x += 1;
			}
		}
	}
	else
	{
		if (ennemy->min_x <= 3)
		{
			*fleet = right;
			while (p_element != NULL)
			{
				ennemy_ship = (pos *)(p_element->data);
				p_element = p_element -> next;

				vt100_move (ennemy_ship ->x, ennemy_ship ->y);
				serial_puts ("      ");
				ennemy_ship ->y += 1;
				vt100_move (ennemy_ship ->x, ennemy_ship ->y);
				serial_puts (ennemy_ship ->skin);
			}
		}
		else
		{
			while (p_element != NULL)
			{
				ennemy_ship = (pos *)(p_element->data);
				p_element = p_element -> next;

				ennemy_ship ->x -= 1;
				vt100_move (ennemy_ship ->x, ennemy_ship ->y);
				serial_puts (ennemy_ship ->skin);
				serial_putchar (' ');
			}
		}
	}

	return;
}

u_int8 ennemy_defeated (T_list *p_ship_list)
{

	T_element *p_element = p_ship_list -> start;

	if (p_element == NULL)
		return 1;
	return 0;
}

void new_minmax (T_list *p_ship_list, extremum *ennemy)
{
	//RàZ of max and min
	ennemy->min_x = VT100_SCREEN_WIDTH;
	ennemy->max_x = 0;
	T_element *p_element = p_ship_list -> start;
	pos *ennemy_ship;

	//New value of max and min
	while (p_element != NULL)
	{
		ennemy_ship = (pos *)(p_element->data);
		p_element = p_element -> next;

		if (ennemy_ship->x > ennemy->max_x)
		{
			ennemy->max_x = ennemy_ship->x;
		}
		if ((ennemy_ship->x != 0) && (ennemy_ship->x < ennemy->min_x))
		{
			ennemy->min_x = ennemy_ship->x;
		}
		if (ennemy_ship->y > ennemy->max_y)
		{
			ennemy->max_y = ennemy_ship->y;
		}
	}
}
