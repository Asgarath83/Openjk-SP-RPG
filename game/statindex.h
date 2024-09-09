/*
This file is part of Jedi Academy.

    Jedi Academy is free software: you can redistribute it and/or modify
    it under the terms of the GNU General Public License as published by
    the Free Software Foundation, either version 2 of the License, or
    (at your option) any later version.

    Jedi Academy is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
    GNU General Public License for more details.

    You should have received a copy of the GNU General Public License
    along with Jedi Academy.  If not, see <http://www.gnu.org/licenses/>.
*/
// Copyright 2001-2013 Raven Software

// Filename:	statindex.h
//
// accessed from both server and game modules

#ifndef STATINDEX_H
#define STATINDEX_H


// player_state->stats[] indexes
typedef enum {
	STAT_HEALTH,
	STAT_ITEMS,
	STAT_WEAPONS,					// 16 bit fields
	STAT_ARMOR,				
	STAT_DEAD_YAW,					// look this direction when dead (FIXME: get rid of?)
	STAT_CLIENTS_READY,				// bit mask of clients wishing to exit the intermission (FIXME: configstring?)
	STAT_MAX_HEALTH,					// health / armor limit, changable by handicap
	STAT_MAX_ARMOR, 
//	STAT_WEAPONS2
} statIndex_t;	   

/*qboolean BG_WeaponArrayCheck( const playerState_t *ps, int value )
{
	int choice = STAT_WEAPONS;
	if ( value > 31 )
	{
		choice = STAT_WEAPONS2;
		value -= 32;
	}

	return (ps->stats[choice] & (1 << value)) ? qtrue : qfalse;
}

void BG_WeaponArraySet( playerState_t *ps, int value )
{
	int choice = STAT_WEAPONS;
	if ( value > 31 )
	{
		choice = STAT_WEAPONS2;
		value -= 32;
	}

	ps->stats[choice] |= (1 << value);
}

void BG_WeaponArrayRemove( playerState_t *ps, int value )
{
	int choice = STAT_WEAPONS;
	if ( value > 31 )
	{
		choice = STAT_WEAPONS2;
		value -= 32;
	}

	ps->stats[choice] &= ~(1 << value);
}*/

#endif	// #ifndef STATINDEX_H


/////////////////////// eof /////////////////////

