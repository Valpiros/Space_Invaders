/*
 * space_functions.c
 *
 *  Created on: 14 janv. 2019
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
#include "conf.h"

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

void delay (unsigned long a)
{
	u_int32 i=0;
	for (i=0; i< a; i++);

	return;
}

u_int8 Ps_RandomNumberGeneratory (void)
{
	static u_int8 PRNG = 59; //Random value to start with
	PRNG = ( ((((PRNG >> 3) & 0x01)^((PRNG >> 4) & 0x01)^((PRNG >> 5) & 0x01)^((PRNG >> 7) & 0x01)) <<7) | (PRNG >> 1) );

	return PRNG;
}

void ally_shooting (u_int8 *cd_shoot,T_list *p_shoot_list, pos *ship )
{
	*cd_shoot += 1;
	*cd_shoot %=  7;
	if (*cd_shoot == 1)
	{
		shoot_new (p_shoot_list, shoot_ally, ship->x+2, ship->y-2);
	}
	return;
}




