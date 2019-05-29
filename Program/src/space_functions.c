/*
 * space_functions.c
 *
 *  Created on: 14 janv. 2019
 *      Author: valen
 */
#include <stdint.h>
#include <stm32f4xx.h>
#include <stm32f4_discovery.h>
#include "serial.h"
#include "type_others.h"
#include "vt100.h"
#include "list.h"
#include "heap.h"

void move_ship (pos *ship, u_int8 ship_size)
{

	switch (serial_get_last_char()) {
	case 'd' :
		if (ship->x <= ( (VT100_SCREEN_XMAX-1) - ship_size*2))
		{
			vt100_move(ship->x,ship->y);
			serial_puts ("     ");
			ship->x += 5;
			vt100_move(ship->x,ship->y);
			serial_putship ();
		}
		break;

	case 'q' :
		if (ship->x >= 1 + ship_size*2)
		{
			vt100_move(ship->x,ship->y);
			serial_puts ("     ");
			ship->x -= 5;
			vt100_move(ship->x,ship->y);
			serial_putship ();
		}
		break;
	}
	return;
}

void serial_putship (void)
{
	serial_puts("<-A->"); // actual ship

	return;
}

/*void serial_shoot (shoot_pos *shoot_tab, u_int8 i)
{
	vt100_move (shoot_tab[i].x,shoot_tab[i].y);
	serial_puts ("");

	return;
}*/

void border_init (void)
{
	u_int8 i;

	for (i=(VT100_SCREEN_XMIN+1);i<=(VT100_SCREEN_XMAX-1);i++)
	{
		vt100_move(i,VT100_SCREEN_YMIN);
		serial_putchar('*');
		vt100_move(i,VT100_SCREEN_YMAX);
		serial_putchar('*');
	}

	for (i=(VT100_SCREEN_YMIN+1);i<=(VT100_SCREEN_YMAX-1);i++)
	{
		vt100_move(VT100_SCREEN_XMIN+1,i);
		serial_putchar('|');
		vt100_move(VT100_SCREEN_XMAX-1,i);
		serial_putchar('|');
	}
	return;
}

void ennemy_type1 (T_list *p_ship_list, extremum *ennemy)
{
	u_int8 i;
	u_int8 distance_ship = 1;
	u_int8 number_ship = 15;
	u_int8 init_x = 10;
	u_int8 init_y = 5;

	pos *p_ship;
	T_element *ship_ele;

	ennemy->min_x = init_x;
	for (i=0;i<number_ship;i++)
	{
		p_ship = heap_malloc (sizeof(pos));
		if (p_ship != NULL)
		{
			p_ship -> lenght = 6;
			p_ship -> skin[0] = '\\';
			p_ship -> skin[1] = '-';
			p_ship -> skin[2] = 'V';
			p_ship -> skin[3] = 'V';
			p_ship -> skin[4] = '-';
			p_ship -> skin[5] = '/';
			p_ship -> x = init_x+(p_ship -> lenght+distance_ship)*i;
			p_ship -> y = init_y;
			p_ship -> alive = 1;
			vt100_move(init_x+(p_ship -> lenght+distance_ship)*i,init_y);
			serial_puts(p_ship -> skin);

			ship_ele = list_create_element (p_ship);
			list_prepend (p_ship_list, ship_ele);

			if (p_ship -> x > ennemy->max_x)
			{
				ennemy->max_x = p_ship -> x;
			}
		}
	}
	return;
}

void ennemy_type2 (T_list *p_ship_list, extremum *ennemy)
{
	u_int8 i;
	u_int8 distance_ship = 1;
	u_int8 number_ship = 20;
	u_int8 init_x = 25;
	u_int8 init_y = 7;

	pos *p_ship;
	T_element *ship_ele;

	ennemy->min_x = init_x;
	for (i=0;i<number_ship;i++)
	{
		p_ship = heap_malloc (sizeof(pos));
		if (p_ship != NULL)
		{
			p_ship -> lenght = 3;
			p_ship -> skin[0] = '<';
			p_ship -> skin[1] = 'I';
			p_ship -> skin[2] = '>';
			p_ship -> x = init_x+(p_ship -> lenght+distance_ship)*i;
			p_ship -> y = init_y;
			p_ship -> alive = 1;
			vt100_move(init_x+(p_ship -> lenght+distance_ship)*i,init_y);
			serial_puts(p_ship -> skin);

			ship_ele = list_create_element (p_ship);
			list_prepend (p_ship_list, ship_ele);

			if (p_ship -> x > ennemy->max_x)
			{
				ennemy->max_x = p_ship -> x;
			}
		}
	}
	return;
}

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
				list_pop_at (p_shoot_list, index);
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
				list_pop_at (p_shoot_list, index);
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

void delay (unsigned long a)
{
	u_int32 i=0;
	for (i=0; i< a; i++);

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
			missile_new (p_shoot_list, shoot_ennemy, ennemy_ship->x+2, ennemy_ship->y+1);

		}


	}
	return;
}

u_int8 Ps_RandomNumberGeneratory (void)
{
	static u_int8 PRNG = 59; //Random value to start with
	PRNG = ( ((((PRNG >> 3) & 0x01)^((PRNG >> 4) & 0x01)^((PRNG >> 5) & 0x01)^((PRNG >> 7) & 0x01)) <<7) | (PRNG >> 1) );

	return PRNG;
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
						list_pop_at (p_ship_list, ennemy_index);

						list_pop_at (p_shoot_list, shoot_index);

						shoot_index--;
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
					list_pop_at (p_shoot_list, shoot_index);
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

void ally_shooting (u_int8 *cd_shoot,T_list *p_shoot_list, pos *ship )
{
	*cd_shoot += 1;
	*cd_shoot %=  7;
	if (*cd_shoot == 1)
	{
		missile_new (p_shoot_list, shoot_ally, ship->x+2, ship->y-2);
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

void missile_new (T_list *p_shoot_list, team owner, u_int8 X, u_int8 Y)
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

void shoot_suppr (shoot_pos *shoot)
{
	vt100_move (shoot->x, shoot->y);
	serial_puts (" \n\b ");

}
