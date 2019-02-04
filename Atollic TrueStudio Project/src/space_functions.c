/*
 * space_functions.c
 *
 *  Created on: 14 janv. 2019
 *      Author: valen
 */
#include "stm32f4xx.h"
#include "stm32f4_discovery.h"
#include <serial.h>
#include <type_others.h>
#include <vt100.h>
#include <stdint.h>

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
	serial_puts("<-A->\0"); // actual ship

	return;
}

void serial_shoot (shoot_pos *shoot_tab, u_int8 i)
{
	vt100_move (shoot_tab[i].x,shoot_tab[i].y);
	serial_puts ("");

	return;
}

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

void ennemy_type1 (pos *ennemy_tab, u_int8 *lenght_ship)
{
	u_int8 i;
	u_int8 distance_ship = 1;
	u_int8 number_ship = 9;
	u_int8 init_x = 20;
	u_int8 height = 5;
	*lenght_ship = 7;

	for (i=0;i<number_ship;i++)
	{
		vt100_move(init_x+(*lenght_ship+distance_ship)*i,height);
		serial_puts("\\-V+V-/");
		ennemy_tab[i].x = init_x+(*lenght_ship+distance_ship)*i;
		ennemy_tab[i].y = height;
	}
	return;
}

void move_shoots (shoot_pos *shoot_tab)
{
	u_int8 i = 0;

	while (i <= 29)
	{
		if ((shoot_tab[i].x != 0)||(shoot_tab[i].y != 0))
		{
			vt100_move (shoot_tab[i].x,shoot_tab[i].y);
			serial_puts (" \n\b ");
			//serial_shoot (shoot_tab, i);
			if (shoot_tab[i].who == shoot_ennemy)
			{
				shoot_tab[i].y += 2;
			}
			else
			{
				shoot_tab[i].y -= 2;
			}
			if ((shoot_tab[i].y == 34)&&(shoot_tab[i].who == shoot_ennemy))
			{
				shoot_tab[i].x = 0;
				shoot_tab[i].y = 0;
			}
			else if ((shoot_tab[i].y == 2)&&(shoot_tab[i].who == shoot_ally))
			{
				shoot_tab[i].x = 0;
				shoot_tab[i].y = 0;
			}
			vt100_move (shoot_tab[i].x,shoot_tab[i].y);
			serial_puts ("|\n\b|");
		}
		i++;
	}

	return;
}

void delay (unsigned long a)
{
	u_int32 i=0;
	for (i=0; i< a; i++);

	return;
}



void ennemy_shooting (pos *ennemy_tab, shoot_pos *shoot_tab)
{
	u_int8 j = 0;
	for (u_int8 i = 0; i <= 29; i++)
	{
		if ((ennemy_tab[i].x != 0)&&(ennemy_tab[i].y != 0))
		{
			if (Ps_RandomNumberGeneratory() <= 35)	// tir ?
			{
				while ((shoot_tab[j].x != 0)&&(shoot_tab[j].y != 0) )
				{
					j++;											//continue the tab until a clear address to stock the shoot
				}
					shoot_tab[j].x = ennemy_tab[i].x+2;
					shoot_tab[j].y = ennemy_tab[i].y+1;
					shoot_tab[j].who = shoot_ennemy;
					vt100_move (shoot_tab[j].x,shoot_tab[j].y);
					serial_puts ("|\n\b|");
			}
		}
	}

	return;
}

u_int8 Ps_RandomNumberGeneratory (void)
{
	PRNG = ( ((((PRNG >> 3) & 0x01)^((PRNG >> 4) & 0x01)^((PRNG >> 5) & 0x01)^((PRNG >> 7) & 0x01)) <<7) | (PRNG >> 1) );

	return PRNG;
}

void hitbox (pos *ennemy_tab, shoot_pos *shoot_tab, pos *ship, u_int8 ennemy_lenght, u_int8 *lifes )
{
	u_int8 ennemy_index;
	u_int8 shoot_index;

	for (shoot_index = 0; shoot_index <= 29; shoot_index++)	//can witn some time using car exist in struct
	{
		if (shoot_tab[shoot_index].who == shoot_ally)
		{
			for (ennemy_index = 0; ennemy_index <= 29; ennemy_index++)	//can win some time with var ennemy total
				{
					if (ennemy_tab[ennemy_index].y+1 == shoot_tab[shoot_index].y)
					{
						if ((shoot_tab[shoot_index].x >= ennemy_tab[ennemy_index].x)&&(shoot_tab[shoot_index].x <= ennemy_tab[ennemy_index].x+(ennemy_lenght-1)))
						{
							vt100_move (ennemy_tab[ennemy_index].x,ennemy_tab[ennemy_index].y);
							serial_puts ("XXXXXXX");
							delay (3000000);
							vt100_move (ennemy_tab[ennemy_index].x,ennemy_tab[ennemy_index].y);
							serial_puts ("       ");
							vt100_move (shoot_tab[shoot_index].x, shoot_tab[shoot_index].y);
							serial_puts (" \n\b ");
		                	ennemy_tab[ennemy_index].x = 0;
		                	ennemy_tab[ennemy_index].y = 0;
		                	shoot_tab[shoot_index].x = 0;
		                	shoot_tab[shoot_index].y = 0;
						}
					}
				}
		}
		else
		{
			if (ship->y-2 == shoot_tab[shoot_index].y)
			{
				if ((shoot_tab[shoot_index].x >= ship->x)&&(shoot_tab[shoot_index].x <= ship->x+(5-1)))
				{
					vt100_move (ship->x,ship->y);
					serial_puts ("XXXXX");
					delay (3000000);
					vt100_move (ship->x,ship->y);
					serial_puts ("     ");
					vt100_move (shoot_tab[shoot_index].x, shoot_tab[shoot_index].y);
					serial_puts (" \n\b ");
					shoot_tab[shoot_index].x = 0;
					shoot_tab[shoot_index].y = 0;
					// Got hit
					vt100_move (17+(*lifes),2);
					serial_puts ("  ");
					*lifes -=1;
				}
			}
		}
	}
	return;
}

void ally_shooting (u_int8 *cd_shoot,shoot_pos *shoot_tab )
{
	u_int8 j = 0;
	*cd_shoot += 1;
	*cd_shoot %=  5;
	if (*cd_shoot == 1)
	{
		while ((shoot_tab[j].x != 0)&&(shoot_tab[j].y != 0) )
		{
			j++;											//continue the tab until a clear adress to stock the shoot
		}
		shoot_tab[j].x = ship.x+2;
		shoot_tab[j].y = ship.y-2;
		shoot_tab[j].who = shoot_ally;
		vt100_move (shoot_tab[j].x,shoot_tab[j].y);
		serial_puts ("|\n\b|");
	}
	return;
}

void ennemy_moving (pos *ennemy_tab)
{

	return;
}
