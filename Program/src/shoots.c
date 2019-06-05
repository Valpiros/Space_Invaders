/*
 * shoots.c
 *
 *  Created on: 30 mai 2019
 *      Author: valpiros
 */

#include <diverse.h>
#include <stdint.h>
#include <stm32f4xx.h>
#include <stm32f4_discovery.h>
#include "serial.h"
#include "vt100.h"
#include "list.h"
#include "heap.h"
#include "shoots.h"
#include "ennemies.h"

void move_shoots (T_list *p_shoot_list)
{
	T_element *p_element = p_shoot_list -> start;
	shoot_pos *shoot;
	u_int8 index = 0;
	while (p_element != NULL)
	{
		shoot = (shoot_pos *)(p_element->data);
		p_element = p_element -> next;

		//shoot_suppr (shoot/*(shoot_pos *)(p_element->data)*/);
		vt100_move (shoot->x, shoot->y);
		serial_puts (" \n\b ");

		if (shoot->who == shoot_ennemy)
		{
			shoot->y += 2;
			if (shoot->y >= VT100_SCREEN_YMAX-1)
			{

				heap_free (list_pop_at (p_shoot_list, index), sizeof(shoot_pos));
				index--;
			}
			else
			{
			vt100_move (shoot->x,shoot->y);
			serial_puts ("|\n\b|");
			}
		}
		else
		{
			shoot->y -= 2;
			if (shoot->y <= VT100_SCREEN_YMIN+1)
			{

				heap_free (list_pop_at (p_shoot_list, index), sizeof(shoot_pos));
				index--;
			}
			else
			{
			vt100_move (shoot->x,shoot->y);
			serial_putchar (186);
			vt100_move (shoot->x,shoot->y+1);
			serial_putchar (186);
			}
		}
		index++;
	}

	return;
}

void ennemy_shooting (T_list *p_ship_list, T_list *p_shoot_list, pos *ship)
{
	T_element *p_element = p_ship_list -> start;
	pos *ennemy_ship;

	while (p_element != NULL)
	{
		ennemy_ship = (pos *)(p_element->data);
		p_element = p_element -> next;

		if (Ps_RandomNumberGeneratory() <= 20)	// RNG will shoot ?
		{
			shoot_new (p_shoot_list, shoot_ennemy, ennemy_ship->x+2, ennemy_ship->y+1);

		}
	}
	return;
}

void hitbox (T_list *p_ship_list, T_list *p_shoot_list, pos *ship, u_int8 *lives)
{
	u_int8 ennemy_index;
	u_int8 shoot_index;
	T_element *p_ele_shoot = p_shoot_list -> start;
	shoot_pos *shoot;
	T_element *p_ele_ship;
	pos *ennemy_ship;

	while (p_ele_shoot != NULL)
	{
		shoot = (shoot_pos *)(p_ele_shoot->data);
		p_ele_shoot = p_ele_shoot -> next;

		if (shoot->who == shoot_ally)
		{
			p_ele_ship = p_ship_list -> start;
			ennemy_index = 0;
			while (p_ele_ship != NULL)
			{
				ennemy_ship = (pos *)(p_ele_ship->data);
				p_ele_ship = p_ele_ship -> next;

				if ((ennemy_ship->y+1 == shoot->y)||(ennemy_ship->y == shoot->y))
				{
					if ((shoot->x >= ennemy_ship->x)&&(shoot->x <= ennemy_ship->x+(ennemy_ship->lenght-1)))
					{
						vt100_move (ennemy_ship->x,ennemy_ship->y);
						serial_puts ("XXXXXXX");
						vt100_move (ennemy_ship->x,ennemy_ship->y);
						serial_puts ("       ");
						vt100_move (shoot->x, shoot->y);
						serial_puts (" \n\b ");
						heap_free (list_pop_at (p_ship_list, ennemy_index), sizeof(pos));
						heap_free (list_pop_at (p_shoot_list, shoot_index), sizeof(shoot_pos));
						shoot_index--;
						//ennemy_index--;
					}
				}
				ennemy_index++;
			}
		}
		else
		{
			if ((ship->y-2 == shoot->y)||(ship->y-1 == shoot->y))
			{
				if ((shoot->x >= ship->x)&&(shoot->x <= ship->x+(5-1)))
				{
					vt100_move (ship->x,ship->y);
					serial_puts ("XXXXX");
					delay (3000000);
					if (*lives == 0 )
					{
						vt100_move (ship->x,ship->y);
						serial_puts ("     ");
					}
					else
					{
						vt100_move (ship->x,ship->y);
						serial_putship ();
					}
					vt100_move (shoot->x, shoot->y);
					serial_puts (" \n\b ");
					heap_free (list_pop_at (p_shoot_list, shoot_index), sizeof(shoot_pos));
					shoot_index--;
					// Got hit
					vt100_move (17+(*lives),2);
					serial_puts ("  ");
					*lives -=1;
				}
			}
		}
		shoot_index++;
	}
	return;
}

void shoot_new (T_list *p_shoot_list, team owner, u_int8 X, u_int8 Y)
{

	shoot_pos *p_shoot = heap_malloc (sizeof(shoot_pos));
	if (p_shoot != NULL)
	{
		p_shoot -> x = X;
		p_shoot -> y = Y;
		p_shoot -> who = owner;

		T_element *shoot_ele = list_create_element (p_shoot);
		list_prepend (p_shoot_list, shoot_ele);

		if (owner == shoot_ally)
		{

			vt100_move (X,Y);
			serial_putchar (186);
			vt100_move (X,Y);
			serial_putchar (186);
		}

		else
		{
			vt100_move (p_shoot->x,p_shoot->y);
			serial_puts ("|\n\b|");
		}
	}
	return;
}
