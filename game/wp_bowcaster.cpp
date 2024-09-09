/*
This file is part of OpenJK.

    OpenJK is free software: you can redistribute it and/or modify
    it under the terms of the GNU General Public License as published by
    the Free Software Foundation, either version 2 of the License, or
    (at your option) any later version.

    OpenJK is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
    GNU General Public License for more details.

    You should have received a copy of the GNU General Public License
    along with OpenJK.  If not, see <http://www.gnu.org/licenses/>.
*/
// Copyright 2013 OpenJK

#include "g_headers.h"

#include "g_local.h"
#include "b_local.h"
#include "g_functions.h"
#include "wp_saber.h"
#include "w_local.h"

//-------------------
//	Wookiee Bowcaster
//-------------------

//---------------------------------------------------------
static void WP_BowcasterMainFire( gentity_t *ent )
//---------------------------------------------------------
{
	vec3_t	start;
	int		damage	= weaponData[WP_BOWCASTER].altDamage;

	// DMG FOLGORE
	if ( ent && ent->client && ent->client->NPC_class == CLASS_ASSASSIN_DROID )
	{
		damage *= 1.5;
	}
	// DMG PLASMA
	else if ( ent && ent->client && ent->client->NPC_class == CLASS_MORGANKATARN 	)
	{
		damage *= 2; // Elementali del Calore fanno 50% di danno in più!
	}

	VectorCopy( muzzle, start );
	WP_TraceSetStart( ent, start, vec3_origin, vec3_origin );//make sure our start point isn't on the other side of a wall

	WP_MissileTargetHint(ent, start, forwardVec);

	gentity_t *missile = CreateMissile( start, forwardVec, BOWCASTER_VELOCITY, 10000, ent, qtrue );

	missile->classname = "bowcaster_alt_proj";
	missile->s.weapon = WP_BOWCASTER;

	// Do the damages
	if ( ent->s.number != 0 )
	{
		if ( g_spskill->integer == 0 )
		{
			damage = BOWCASTER_NPC_DAMAGE_EASY;
		}
		else if ( g_spskill->integer == 1 )
		{
			damage = BOWCASTER_NPC_DAMAGE_NORMAL;
		}
		else
		{
			damage = BOWCASTER_NPC_DAMAGE_HARD;
		}
	}

	// DAMAGES 
	
	// LIGHTNING 
	if ( ent->client->NPC_class == CLASS_LIGHTREAVER || 
			ent->client->NPC_class == CLASS_LYCAN || 
			ent->client->NPC_class == CLASS_GOLEM_LIGHT || 
			ent->client->NPC_class == CLASS_SABER_DROID ||
			ent->client->NPC_class == CLASS_ASSASSIN_DROID )
	{
		{
			damage *= 1.5;
		}
	}
	// RADIANZA 
	else if ( ent->client->NPC_class == CLASS_MORGANKATARN ||
			ent->client->NPC_class == CLASS_GOLEM_GOLD || 
			ent->client->NPC_class == CLASS_PROTOCOL || 
			ent->client->NPC_class == CLASS_JAN || 
			ent->client->NPC_class == CLASS_CLAW )
	{
		{
			damage *= 2;
		}
	}
	// ENERGY BOLT 
	else if ( ent->client->NPC_class == CLASS_REBORN ||
			ent->client->NPC_class == CLASS_KYLE ||
			ent->client->NPC_class == CLASS_ALORA ||
			ent->client->NPC_class == CLASS_MONMOTHA ||
			ent->client->NPC_class == CLASS_TUSKEN ||
			ent->client->NPC_class == CLASS_GLIDER ||
			ent->client->NPC_class == CLASS_NOGHRI ||
			ent->client->NPC_class == CLASS_FISH ||
			ent->client->NPC_class == CLASS_GRAN ||
			ent->client->NPC_class == CLASS_LIZARD ||
			ent->client->NPC_class == CLASS_SWAMPTROOPER ||
			ent->client->NPC_class == CLASS_REBEL ||
			ent->client->NPC_class == CLASS_ROCKETTROOPER ||
			ent->client->NPC_class == CLASS_BOBAFETT || 
			ent->client->NPC_class == CLASS_R2D2 ||
			ent->client->NPC_class == CLASS_R5D2 ||
			ent->client->NPC_class == CLASS_WAMPA || 
			ent->client->NPC_class == CLASS_GOLEM_METAL || 
			ent->client->NPC_class == CLASS_DEMON_ELECTRO || 			 
			ent->client->NPC_class == CLASS_MELC ||  
			ent->client->NPC_class == CLASS_SUMMONER ||
			ent->client->NPC_class == CLASS_ZEPH ||
			ent->client->NPC_class == CLASS_DUMAHIM || 
			ent->client->NPC_class == CLASS_RAHABIM ||
			ent->client->NPC_class == CLASS_TURELIM ||
			ent->client->NPC_class == CLASS_VAMPIRE ||
			ent->client->NPC_class == CLASS_VAMPIRE_DEMON ||
			ent->client->NPC_class == CLASS_VAMPIRE_GHOST || 
			ent->client->NPC_class == CLASS_KAIN ||
			ent->client->NPC_class == CLASS_KAIN2 || 
			ent->client->NPC_class == CLASS_KAIN3 ||
			ent->client->NPC_class == CLASS_UMAH || 
			ent->client->NPC_class == CLASS_VORADOR ||
			ent->client->NPC_class == CLASS_JANOS ||
			ent->client->NPC_class == CLASS_NECROMANCER || 
			ent->client->NPC_class == CLASS_VAMPIRE_QUEEN ||
			ent->client->NPC_class == CLASS_VAMPIRE_DARK || 
			ent->client->NPC_class == CLASS_VAMPIRE_NECRO || 
			ent->client->NPC_class == CLASS_VAMPIRE_NECRO2 ||
			ent->client->NPC_class == CLASS_VAMPIRE_FEAR || 
			ent->client->NPC_class == CLASS_VAMPIRE_FEAR2 ||  
			ent->client->NPC_class == CLASS_VAMPIRE_BLOOD ||
			ent->client->NPC_class == CLASS_VAMPIRE_SOUND || 
			ent->client->NPC_class == CLASS_VAMPIRE_LYCAN || 
			ent->client->NPC_class == CLASS_VAMPIRE_ELF || 
			ent->client->NPC_class == CLASS_VAMPIRE_ELF2 ||
			ent->client->NPC_class == CLASS_VAMPIRE_ELF3 ||
			ent->client->NPC_class == CLASS_VAMPIRE_WATER || 
			ent->client->NPC_class == CLASS_VAMPIRE_FROST ||  
			ent->client->NPC_class == CLASS_VAMPIRE_ARCHER || 
			ent->client->NPC_class == CLASS_RAZIELHIM ||  
			ent->client->NPC_class == CLASS_ESKANDOR ||  
			ent->client->NPC_class == CLASS_VAMPIRE_ZOMBIE ||
			ent->client->NPC_class == CLASS_AIR || 
			ent->client->NPC_class == CLASS_GOLEM_AIR || 
			ent->client->NPC_class == CLASS_BIRD || 
			ent->client->NPC_class == CLASS_IVY )
	{
		{
			damage *= 1.25;
		}
	}
	// LIGHT KNIVES 
	else if ( ent->client->NPC_class == CLASS_GALAK ||
			ent->client->NPC_class == CLASS_RODIAN ||
			ent->client->NPC_class == CLASS_MARK1 ||
			ent->client->NPC_class == CLASS_MARK2 || 
			ent->client->NPC_class == CLASS_HUNTER || 
			ent->client->NPC_class == CLASS_SCOUT ||  
			ent->client->NPC_class == CLASS_SARAFAN || 
			ent->client->NPC_class == CLASS_ARCHER ||  
			ent->client->NPC_class == CLASS_WARRIOR )
	{
		{
			damage *= 1.25;
		}
	}
	// SKULL LIGHT 
	else if ( ent->client->NPC_class == CLASS_ARIEL || 
			ent->client->NPC_class == CLASS_MINEMONSTER ||
			ent->client->NPC_class == CLASS_DEMON_BLADE || 
			ent->client->NPC_class == CLASS_DEMON_POISON || 
			ent->client->NPC_class == CLASS_DEMON_FIRE || 
			ent->client->NPC_class == CLASS_JAWA || 
			ent->client->NPC_class == CLASS_RANCOR || 
			ent->client->NPC_class == CLASS_SHADOWTROOPER || 
			ent->client->NPC_class == CLASS_SABOTEUR || 
			ent->client->NPC_class == CLASS_MURJJ ||
			ent->client->NPC_class == CLASS_NECRO || 
			ent->client->NPC_class == CLASS_THRALL || 
			ent->client->NPC_class == CLASS_IMPERIAL || 
			ent->client->NPC_class == CLASS_CENOBITA || 
			ent->client->NPC_class == CLASS_DARK || 
			ent->client->NPC_class == CLASS_VOID || 
			ent->client->NPC_class == CLASS_POISON || 
			ent->client->NPC_class == CLASS_GOLEM_DARK || 
			ent->client->NPC_class == CLASS_HELLGOLEM || 
			ent->client->NPC_class == CLASS_GOLEM_NECRO ||
			ent->client->NPC_class == CLASS_GOLEM_SMOKE || 
			ent->client->NPC_class == CLASS_GOLEM_VOID ||
			ent->client->NPC_class == CLASS_GOLEM_POISON || 
			ent->client->NPC_class == CLASS_GOLEM_LAVA ||
			ent->client->NPC_class == CLASS_POLTER )
	{
		{
			damage *= 2.5;
		}
	}
	// MADNESS 
	else if ( ent->client->NPC_class == CLASS_TAVION ||
			ent->client->NPC_class == CLASS_DESANN ||
			ent->client->NPC_class == CLASS_HYLDEN || 
			ent->client->NPC_class == CLASS_HYLDENLORD || 
			ent->client->NPC_class == CLASS_SAND_CREATURE ||
			ent->client->NPC_class == CLASS_ATST ||
			ent->client->NPC_class == CLASS_SENTRY ||
			ent->client->NPC_class == CLASS_MOUSE || 
			ent->client->NPC_class == CLASS_TENTACLE ||  
			ent->client->NPC_class == CLASS_ELDERGOD || 
			ent->client->NPC_class == CLASS_SONIC || 
			ent->client->NPC_class == CLASS_GOLEM_SONIC || 
			ent->client->NPC_class == CLASS_PROPHET || 
			ent->client->NPC_class == CLASS_DECEIVER || 
			ent->client->NPC_class == CLASS_FLIER2 || 
			ent->client->NPC_class == CLASS_BEAST ||  
			ent->client->NPC_class == CLASS_SHARK )
	{
		{
			damage *= 3;
			
		}
	}
	// LIGHT DEFAULT SHOOTS 
	else if ( ent->client->NPC_class == CLASS_BARTENDER ||
			ent->client->NPC_class == CLASS_BESPIN_COP ||
			ent->client->NPC_class == CLASS_COMMANDO || 
			ent->client->NPC_class == CLASS_HOWLER || 
			ent->client->NPC_class == CLASS_IMPWORKER || 
			ent->client->NPC_class == CLASS_WIZARD || 
			ent->client->NPC_class == CLASS_INTERROGATOR ||
			ent->client->NPC_class == CLASS_JEDI || 
			ent->client->NPC_class == CLASS_LANDO || 
			ent->client->NPC_class == CLASS_LUKE || 
			ent->client->NPC_class == CLASS_GONK || 
			ent->client->NPC_class == CLASS_PRISONER || 
			ent->client->NPC_class == CLASS_PROBE || 
			ent->client->NPC_class == CLASS_HAZARD_TROOPER || 
			ent->client->NPC_class == CLASS_REELO || 
			ent->client->NPC_class == CLASS_REMOTE || 
			ent->client->NPC_class == CLASS_SEEKER || 
			ent->client->NPC_class == CLASS_STORMTROOPER || 
			ent->client->NPC_class == CLASS_SWAMP || 
			ent->client->NPC_class == CLASS_TRANDOSHAN || 
			ent->client->NPC_class == CLASS_UGNAUGHT || 
			ent->client->NPC_class == CLASS_WEEQUAY || 
			ent->client->NPC_class == CLASS_PLAYER || 
			ent->client->NPC_class == CLASS_VEHICLE || 
			ent->client->NPC_class == CLASS_GALAKMECH ||
			ent->client->NPC_class == CLASS_ALCHEMIST || 
			ent->client->NPC_class == CLASS_DRUID ||
			ent->client->NPC_class == CLASS_REAVER || 
			ent->client->NPC_class == CLASS_RAZIEL ||
			ent->client->NPC_class == CLASS_NECROREAVER ||
			ent->client->NPC_class == CLASS_DARKREAVER || 
			ent->client->NPC_class == CLASS_TANKREAVER ||			
			ent->client->NPC_class == CLASS_DRUIDREAVER ||
			ent->client->NPC_class == CLASS_ICEREAVER || 
			ent->client->NPC_class == CLASS_SPIDERREAVER ||
			ent->client->NPC_class == CLASS_SPIDERREAVER2 ||
			ent->client->NPC_class == CLASS_SPIDERREAVER3 ||
			ent->client->NPC_class == CLASS_DRAGONREAVER || 
			ent->client->NPC_class == CLASS_AIRREAVER || 
			ent->client->NPC_class == CLASS_MENTALIST || 
			ent->client->NPC_class == CLASS_EARTHREAVER ||		 
			ent->client->NPC_class == CLASS_LIGHT || 
			ent->client->NPC_class == CLASS_FIRE ||  
			ent->client->NPC_class == CLASS_WATER || 
			ent->client->NPC_class == CLASS_CRYSTAL || 
			ent->client->NPC_class == CLASS_EARTH || 
			ent->client->NPC_class == CLASS_HOLY ||  
			ent->client->NPC_class == CLASS_GOLEM_FIRE ||
			ent->client->NPC_class == CLASS_GOLEM_WATER ||
			ent->client->NPC_class == CLASS_GOLEM_ICE ||			
			ent->client->NPC_class == CLASS_GOLEM ||
			ent->client->NPC_class == CLASS_GOLEM_HOLY )
	{
		{
			damage *= 1;
		}
	}


	VectorSet( missile->maxs, BOWCASTER_SIZE, BOWCASTER_SIZE, BOWCASTER_SIZE );
	VectorScale( missile->maxs, -1, missile->mins );

//	if ( ent->client && ent->client->ps.powerups[PW_WEAPON_OVERCHARGE] > 0 && ent->client->ps.powerups[PW_WEAPON_OVERCHARGE] > cg.time )
//	{
//		// in overcharge mode, so doing double damage
//		missile->flags |= FL_OVERCHARGED;
//		damage *= 2;
//	}

	//missile->s.eFlags |= EF_BOUNCE;
	missile->bounceCount = 0;

	missile->damage = damage;
	//missile->dflags = DAMAGE_DISMEMBER; NOT WORK 
	missile->dflags = DAMAGE_DEATH_KNOCKBACK;
	missile->methodOfDeath = MOD_BOWCASTER_ALT;
	missile->clipmask = MASK_SHOT | CONTENTS_LIGHTSABER;
	missile->splashDamage = weaponData[WP_BOWCASTER].altSplashDamage;
	missile->splashRadius = weaponData[WP_BOWCASTER].altSplashRadius;
}

//---------------------------------------------------------
static void WP_BowcasterAltFire( gentity_t *ent )
//---------------------------------------------------------
{
	int			damage	= weaponData[WP_BOWCASTER].damage, count;
	float		vel;
	vec3_t		angs, dir, start;
	gentity_t	*missile;

	// DMG FOLGORE
	if ( ent && ent->client && ent->client->NPC_class == CLASS_ASSASSIN_DROID )
	{
		damage *= 1.5;
	}
	// DMG PLASMA
	else if ( ent && ent->client && ent->client->NPC_class == CLASS_MORGANKATARN 	)
	{
		damage *= 2; 
	}

	VectorCopy( muzzle, start );
	WP_TraceSetStart( ent, start, vec3_origin, vec3_origin );//make sure our start point isn't on the other side of a wall

	// Do the damages
	if ( ent->s.number != 0 )
	{
		if ( g_spskill->integer == 0 )
		{
			damage = BOWCASTER_NPC_DAMAGE_EASY;
		}
		else if ( g_spskill->integer == 1 )
		{
			damage = BOWCASTER_NPC_DAMAGE_NORMAL;
		}
		else
		{
			damage = BOWCASTER_NPC_DAMAGE_HARD;
		}
	}

	// DAMAGES 
	// LIGHTNING 
	if ( ent->client->NPC_class == CLASS_LIGHTREAVER || 
			ent->client->NPC_class == CLASS_LYCAN || 
			ent->client->NPC_class == CLASS_GOLEM_LIGHT || 
			ent->client->NPC_class == CLASS_SABER_DROID ||
			ent->client->NPC_class == CLASS_ASSASSIN_DROID )
	{
		{
			damage *= 1.5;
		}
	}
	// RADIANZA 
	else if ( ent->client->NPC_class == CLASS_MORGANKATARN ||
			ent->client->NPC_class == CLASS_GOLEM_GOLD || 
			ent->client->NPC_class == CLASS_PROTOCOL || 
			ent->client->NPC_class == CLASS_JAN || 
			ent->client->NPC_class == CLASS_CLAW )
	{
		{
			damage *= 2;
		}
	}
	// ENERGY BOLT 
	else if ( ent->client->NPC_class == CLASS_REBORN ||
			ent->client->NPC_class == CLASS_KYLE ||
			ent->client->NPC_class == CLASS_ALORA ||
			ent->client->NPC_class == CLASS_MONMOTHA ||
			ent->client->NPC_class == CLASS_TUSKEN ||
			ent->client->NPC_class == CLASS_GLIDER ||
			ent->client->NPC_class == CLASS_NOGHRI ||
			ent->client->NPC_class == CLASS_FISH ||
			ent->client->NPC_class == CLASS_GRAN ||
			ent->client->NPC_class == CLASS_LIZARD ||
			ent->client->NPC_class == CLASS_SWAMPTROOPER ||
			ent->client->NPC_class == CLASS_REBEL ||
			ent->client->NPC_class == CLASS_ROCKETTROOPER ||
			ent->client->NPC_class == CLASS_BOBAFETT || 
			ent->client->NPC_class == CLASS_R2D2 ||
			ent->client->NPC_class == CLASS_R5D2 ||
			ent->client->NPC_class == CLASS_WAMPA || 
			ent->client->NPC_class == CLASS_GOLEM_METAL || 
			ent->client->NPC_class == CLASS_DEMON_ELECTRO || 			 
			ent->client->NPC_class == CLASS_MELC ||  
			ent->client->NPC_class == CLASS_SUMMONER ||
			ent->client->NPC_class == CLASS_ZEPH ||
			ent->client->NPC_class == CLASS_DUMAHIM || 
			ent->client->NPC_class == CLASS_RAHABIM ||
			ent->client->NPC_class == CLASS_TURELIM ||
			ent->client->NPC_class == CLASS_VAMPIRE ||
			ent->client->NPC_class == CLASS_VAMPIRE_DEMON ||
			ent->client->NPC_class == CLASS_VAMPIRE_GHOST || 
			ent->client->NPC_class == CLASS_KAIN ||
			ent->client->NPC_class == CLASS_KAIN2 || 
			ent->client->NPC_class == CLASS_KAIN3 ||
			ent->client->NPC_class == CLASS_UMAH || 
			ent->client->NPC_class == CLASS_VORADOR ||
			ent->client->NPC_class == CLASS_JANOS ||
			ent->client->NPC_class == CLASS_NECROMANCER || 
			ent->client->NPC_class == CLASS_VAMPIRE_QUEEN ||
			ent->client->NPC_class == CLASS_VAMPIRE_DARK || 
			ent->client->NPC_class == CLASS_VAMPIRE_NECRO || 
			ent->client->NPC_class == CLASS_VAMPIRE_NECRO2 ||
			ent->client->NPC_class == CLASS_VAMPIRE_FEAR || 
			ent->client->NPC_class == CLASS_VAMPIRE_FEAR2 ||  
			ent->client->NPC_class == CLASS_VAMPIRE_BLOOD ||
			ent->client->NPC_class == CLASS_VAMPIRE_SOUND || 
			ent->client->NPC_class == CLASS_VAMPIRE_LYCAN || 
			ent->client->NPC_class == CLASS_VAMPIRE_ELF || 
			ent->client->NPC_class == CLASS_VAMPIRE_ELF2 ||
			ent->client->NPC_class == CLASS_VAMPIRE_ELF3 ||
			ent->client->NPC_class == CLASS_VAMPIRE_WATER || 
			ent->client->NPC_class == CLASS_VAMPIRE_FROST ||  
			ent->client->NPC_class == CLASS_VAMPIRE_ARCHER || 
			ent->client->NPC_class == CLASS_RAZIELHIM ||  
			ent->client->NPC_class == CLASS_ESKANDOR ||  
			ent->client->NPC_class == CLASS_VAMPIRE_ZOMBIE ||
			ent->client->NPC_class == CLASS_AIR || 
			ent->client->NPC_class == CLASS_GOLEM_AIR || 
			ent->client->NPC_class == CLASS_BIRD || 
			ent->client->NPC_class == CLASS_IVY )
	{
		{
			damage *= 1.25;
		}
	}
	// LIGHT KNIVES 
	else if ( ent->client->NPC_class == CLASS_GALAK ||
			ent->client->NPC_class == CLASS_RODIAN ||
			ent->client->NPC_class == CLASS_MARK1 ||
			ent->client->NPC_class == CLASS_MARK2 || 
			ent->client->NPC_class == CLASS_HUNTER || 
			ent->client->NPC_class == CLASS_SCOUT ||  
			ent->client->NPC_class == CLASS_SARAFAN || 
			ent->client->NPC_class == CLASS_ARCHER ||  
			ent->client->NPC_class == CLASS_WARRIOR )
	{
		{
			damage *= 1.25;
		}
	}
	// SKULL LIGHT 
	else if ( ent->client->NPC_class == CLASS_ARIEL || 
			ent->client->NPC_class == CLASS_MINEMONSTER ||
			ent->client->NPC_class == CLASS_DEMON_BLADE || 
			ent->client->NPC_class == CLASS_DEMON_POISON || 
			ent->client->NPC_class == CLASS_DEMON_FIRE || 
			ent->client->NPC_class == CLASS_JAWA || 
			ent->client->NPC_class == CLASS_RANCOR || 
			ent->client->NPC_class == CLASS_SHADOWTROOPER || 
			ent->client->NPC_class == CLASS_SABOTEUR || 
			ent->client->NPC_class == CLASS_MURJJ ||
			ent->client->NPC_class == CLASS_NECRO || 
			ent->client->NPC_class == CLASS_THRALL || 
			ent->client->NPC_class == CLASS_IMPERIAL || 
			ent->client->NPC_class == CLASS_CENOBITA || 
			ent->client->NPC_class == CLASS_DARK || 
			ent->client->NPC_class == CLASS_VOID || 
			ent->client->NPC_class == CLASS_POISON || 
			ent->client->NPC_class == CLASS_GOLEM_DARK || 
			ent->client->NPC_class == CLASS_HELLGOLEM || 
			ent->client->NPC_class == CLASS_GOLEM_NECRO ||
			ent->client->NPC_class == CLASS_GOLEM_SMOKE || 
			ent->client->NPC_class == CLASS_GOLEM_VOID ||
			ent->client->NPC_class == CLASS_GOLEM_POISON || 
			ent->client->NPC_class == CLASS_GOLEM_LAVA ||
			ent->client->NPC_class == CLASS_POLTER )
	{
		{
			damage *= 2.5;
		}
	}
	// MADNESS 
	else if ( ent->client->NPC_class == CLASS_TAVION ||
			ent->client->NPC_class == CLASS_DESANN ||
			ent->client->NPC_class == CLASS_HYLDEN || 
			ent->client->NPC_class == CLASS_HYLDENLORD || 
			ent->client->NPC_class == CLASS_SAND_CREATURE ||
			ent->client->NPC_class == CLASS_ATST ||
			ent->client->NPC_class == CLASS_SENTRY ||
			ent->client->NPC_class == CLASS_MOUSE || 
			ent->client->NPC_class == CLASS_TENTACLE ||  
			ent->client->NPC_class == CLASS_ELDERGOD || 
			ent->client->NPC_class == CLASS_SONIC || 
			ent->client->NPC_class == CLASS_GOLEM_SONIC || 
			ent->client->NPC_class == CLASS_PROPHET || 
			ent->client->NPC_class == CLASS_DECEIVER || 
			ent->client->NPC_class == CLASS_FLIER2 || 
			ent->client->NPC_class == CLASS_BEAST ||  
			ent->client->NPC_class == CLASS_SHARK )
	{
		{
			damage *= 3;
			
		}
	}
	// LIGHT DEFAULT SHOOTS 
	else if ( ent->client->NPC_class == CLASS_BARTENDER ||
			ent->client->NPC_class == CLASS_BESPIN_COP ||
			ent->client->NPC_class == CLASS_COMMANDO || 
			ent->client->NPC_class == CLASS_HOWLER || 
			ent->client->NPC_class == CLASS_IMPWORKER || 
			ent->client->NPC_class == CLASS_WIZARD || 
			ent->client->NPC_class == CLASS_INTERROGATOR ||
			ent->client->NPC_class == CLASS_JEDI || 
			ent->client->NPC_class == CLASS_LANDO || 
			ent->client->NPC_class == CLASS_LUKE || 
			ent->client->NPC_class == CLASS_GONK || 
			ent->client->NPC_class == CLASS_PRISONER || 
			ent->client->NPC_class == CLASS_PROBE || 
			ent->client->NPC_class == CLASS_HAZARD_TROOPER || 
			ent->client->NPC_class == CLASS_REELO || 
			ent->client->NPC_class == CLASS_REMOTE || 
			ent->client->NPC_class == CLASS_SEEKER || 
			ent->client->NPC_class == CLASS_STORMTROOPER || 
			ent->client->NPC_class == CLASS_SWAMP || 
			ent->client->NPC_class == CLASS_TRANDOSHAN || 
			ent->client->NPC_class == CLASS_UGNAUGHT || 
			ent->client->NPC_class == CLASS_WEEQUAY || 
			ent->client->NPC_class == CLASS_PLAYER || 
			ent->client->NPC_class == CLASS_VEHICLE || 
			ent->client->NPC_class == CLASS_GALAKMECH ||
			ent->client->NPC_class == CLASS_ALCHEMIST || 
			ent->client->NPC_class == CLASS_DRUID ||
			ent->client->NPC_class == CLASS_REAVER || 
			ent->client->NPC_class == CLASS_RAZIEL ||
			ent->client->NPC_class == CLASS_NECROREAVER ||
			ent->client->NPC_class == CLASS_DARKREAVER || 
			ent->client->NPC_class == CLASS_TANKREAVER ||			
			ent->client->NPC_class == CLASS_DRUIDREAVER ||
			ent->client->NPC_class == CLASS_ICEREAVER || 
			ent->client->NPC_class == CLASS_SPIDERREAVER ||
			ent->client->NPC_class == CLASS_SPIDERREAVER2 ||
			ent->client->NPC_class == CLASS_SPIDERREAVER3 ||
			ent->client->NPC_class == CLASS_DRAGONREAVER || 
			ent->client->NPC_class == CLASS_AIRREAVER || 
			ent->client->NPC_class == CLASS_MENTALIST || 
			ent->client->NPC_class == CLASS_EARTHREAVER ||		 
			ent->client->NPC_class == CLASS_LIGHT || 
			ent->client->NPC_class == CLASS_FIRE ||  
			ent->client->NPC_class == CLASS_WATER || 
			ent->client->NPC_class == CLASS_CRYSTAL || 
			ent->client->NPC_class == CLASS_EARTH || 
			ent->client->NPC_class == CLASS_HOLY ||  
			ent->client->NPC_class == CLASS_GOLEM_FIRE ||
			ent->client->NPC_class == CLASS_GOLEM_WATER ||
			ent->client->NPC_class == CLASS_GOLEM_ICE ||			
			ent->client->NPC_class == CLASS_GOLEM ||
			ent->client->NPC_class == CLASS_GOLEM_HOLY )
	{
		{
			damage *= 1;
		}
	}

	count = ( level.time - ent->client->ps.weaponChargeTime ) / BOWCASTER_CHARGE_UNIT;

	if ( count < 1 )
	{
		count = 1;
	}
	else if ( count > 5 )
	{
		count = 5;
	}

	if ( !(count & 1 ))
	{
		// if we aren't odd, knock us down a level
		count--;
	}

//	if ( ent->client && ent->client->ps.powerups[PW_WEAPON_OVERCHARGE] > 0 && ent->client->ps.powerups[PW_WEAPON_OVERCHARGE] > cg.time )
//	{
//		// in overcharge mode, so doing double damage
//		damage *= 2;
//	}

	WP_MissileTargetHint(ent, start, forwardVec);
	for ( int i = 0; i < count; i++ )
	{
		// create a range of different velocities
		vel = BOWCASTER_VELOCITY * ( crandom() * BOWCASTER_VEL_RANGE + 1.0f );

		vectoangles( forwardVec, angs );

		if ( !(ent->client->ps.forcePowersActive&(1<<FP_SEE))
			|| ent->client->ps.forcePowerLevel[FP_SEE] < FORCE_LEVEL_2 )
		{//force sight 2+ gives perfect aim
			//FIXME: maybe force sight level 3 autoaims some?
			// add some slop to the fire direction
			angs[PITCH] += crandom() * BOWCASTER_ALT_SPREAD * 0.2f;
			angs[YAW]	+= ((i+0.5f) * BOWCASTER_ALT_SPREAD - count * 0.5f * BOWCASTER_ALT_SPREAD );
			if ( ent->NPC )
			{
				angs[PITCH] += ( crandom() * (BLASTER_NPC_SPREAD+(6-ent->NPC->currentAim)*0.25f) );
				angs[YAW]	+= ( crandom() * (BLASTER_NPC_SPREAD+(6-ent->NPC->currentAim)*0.25f) );
			}
		}
		
		AngleVectors( angs, dir, NULL, NULL );

		missile = CreateMissile( start, dir, vel, 10000, ent );

		missile->classname = "bowcaster_proj";
		missile->s.weapon = WP_BOWCASTER;

		VectorSet( missile->maxs, BOWCASTER_SIZE, BOWCASTER_SIZE, BOWCASTER_SIZE );
		VectorScale( missile->maxs, -1, missile->mins );

//		if ( ent->client && ent->client->ps.powerups[PW_WEAPON_OVERCHARGE] > 0 && ent->client->ps.powerups[PW_WEAPON_OVERCHARGE] > cg.time )
//		{
//			missile->flags |= FL_OVERCHARGED;
//		}

		missile->damage = damage;
		missile->dflags = DAMAGE_DEATH_KNOCKBACK;
		missile->methodOfDeath = MOD_BOWCASTER;
		missile->clipmask = MASK_SHOT | CONTENTS_LIGHTSABER;
		missile->splashDamage = weaponData[WP_BOWCASTER].splashDamage;
		missile->splashRadius = weaponData[WP_BOWCASTER].splashRadius;

		// we don't want it to bounce
		missile->bounceCount = 0;
		ent->client->sess.missionStats.shotsFired++;
	}
}





//---------------------------------------------------------
void WP_FireBowcaster( gentity_t *ent, qboolean alt_fire )
//---------------------------------------------------------
{
	if ( alt_fire )
	{
		WP_BowcasterAltFire( ent );
	}
	else
	{
		WP_BowcasterMainFire( ent );
	}

	if ( alt_fire )
{	// FULMINE
	if ( ent->client->NPC_class == CLASS_LIGHTREAVER || 
			ent->client->NPC_class == CLASS_LYCAN || 
			ent->client->NPC_class == CLASS_GOLEM_LIGHT || 
			ent->client->NPC_class == CLASS_SABER_DROID ||
			ent->client->NPC_class == CLASS_ASSASSIN_DROID )
	{
		{
			G_PlayEffect( G_EffectIndex( "bowcaster2/altmuzzle_flash" ), ent->client->renderInfo.handRPoint );
		}
	}
	// RADIANZA 
	else if ( ent->client->NPC_class == CLASS_MORGANKATARN ||
			ent->client->NPC_class == CLASS_GOLEM_GOLD || 
			ent->client->NPC_class == CLASS_PROTOCOL || 
			ent->client->NPC_class == CLASS_JAN || 
			ent->client->NPC_class == CLASS_CLAW )
	{
		{
			G_PlayEffect( G_EffectIndex( "bowcaster3/altmuzzle_flash" ), ent->client->renderInfo.handRPoint );
		}
	}
	// ENERGY BOLT 
	else if ( ent->client->NPC_class == CLASS_REBORN ||
			ent->client->NPC_class == CLASS_KYLE ||
			ent->client->NPC_class == CLASS_ALORA ||
			ent->client->NPC_class == CLASS_MONMOTHA ||
			ent->client->NPC_class == CLASS_TUSKEN ||
			ent->client->NPC_class == CLASS_GLIDER ||
			ent->client->NPC_class == CLASS_NOGHRI ||
			ent->client->NPC_class == CLASS_FISH ||
			ent->client->NPC_class == CLASS_GRAN ||
			ent->client->NPC_class == CLASS_LIZARD ||
			ent->client->NPC_class == CLASS_SWAMPTROOPER ||
			ent->client->NPC_class == CLASS_REBEL ||
			ent->client->NPC_class == CLASS_ROCKETTROOPER ||
			ent->client->NPC_class == CLASS_BOBAFETT || 
			ent->client->NPC_class == CLASS_R2D2 ||
			ent->client->NPC_class == CLASS_R5D2 ||
			ent->client->NPC_class == CLASS_WAMPA || 
			ent->client->NPC_class == CLASS_GOLEM_METAL || 
			ent->client->NPC_class == CLASS_DEMON_ELECTRO || 			 
			ent->client->NPC_class == CLASS_MELC ||  
			ent->client->NPC_class == CLASS_SUMMONER ||
			ent->client->NPC_class == CLASS_ZEPH ||
			ent->client->NPC_class == CLASS_DUMAHIM || 
			ent->client->NPC_class == CLASS_RAHABIM ||
			ent->client->NPC_class == CLASS_TURELIM ||
			ent->client->NPC_class == CLASS_VAMPIRE ||
			ent->client->NPC_class == CLASS_VAMPIRE_DEMON ||
			ent->client->NPC_class == CLASS_VAMPIRE_GHOST || 
			ent->client->NPC_class == CLASS_KAIN ||
			ent->client->NPC_class == CLASS_KAIN2 || 
			ent->client->NPC_class == CLASS_KAIN3 ||
			ent->client->NPC_class == CLASS_UMAH || 
			ent->client->NPC_class == CLASS_VORADOR ||
			ent->client->NPC_class == CLASS_JANOS ||
			ent->client->NPC_class == CLASS_NECROMANCER || 
			ent->client->NPC_class == CLASS_VAMPIRE_QUEEN ||
			ent->client->NPC_class == CLASS_VAMPIRE_DARK || 
			ent->client->NPC_class == CLASS_VAMPIRE_NECRO || 
			ent->client->NPC_class == CLASS_VAMPIRE_NECRO2 ||
			ent->client->NPC_class == CLASS_VAMPIRE_FEAR || 
			ent->client->NPC_class == CLASS_VAMPIRE_FEAR2 ||  
			ent->client->NPC_class == CLASS_VAMPIRE_BLOOD ||
			ent->client->NPC_class == CLASS_VAMPIRE_SOUND || 
			ent->client->NPC_class == CLASS_VAMPIRE_LYCAN || 
			ent->client->NPC_class == CLASS_VAMPIRE_ELF || 
			ent->client->NPC_class == CLASS_VAMPIRE_ELF2 ||
			ent->client->NPC_class == CLASS_VAMPIRE_ELF3 ||
			ent->client->NPC_class == CLASS_VAMPIRE_WATER || 
			ent->client->NPC_class == CLASS_VAMPIRE_FROST ||  
			ent->client->NPC_class == CLASS_VAMPIRE_ARCHER || 
			ent->client->NPC_class == CLASS_HALFVAMPIRE || 
			ent->client->NPC_class == CLASS_RAZIELHIM ||  
			ent->client->NPC_class == CLASS_ESKANDOR ||  
			ent->client->NPC_class == CLASS_VAMPIRE_BOUNTY || 
			ent->client->NPC_class == CLASS_VAMPIRE_ZOMBIE ||
			ent->client->NPC_class == CLASS_VAMPIRE_SARAFAN || 
			ent->client->NPC_class == CLASS_AIR || 
			ent->client->NPC_class == CLASS_GOLEM_AIR || 
			ent->client->NPC_class == CLASS_BIRD || 
			ent->client->NPC_class == CLASS_IVY )
	{
		{
			G_PlayEffect( G_EffectIndex( "energy_bolt/altmuzzle_flash" ), ent->client->renderInfo.handRPoint );

		}
	}
	// LIGHT KNIVES 
	else if ( ent->client->NPC_class == CLASS_GALAK ||
			ent->client->NPC_class == CLASS_RODIAN ||
			ent->client->NPC_class == CLASS_MARK1 ||
			ent->client->NPC_class == CLASS_MARK2 || 
			ent->client->NPC_class == CLASS_HUNTER || 
			ent->client->NPC_class == CLASS_SCOUT ||  
			ent->client->NPC_class == CLASS_SARAFAN || 
			ent->client->NPC_class == CLASS_ARCHER ||  
			ent->client->NPC_class == CLASS_WARRIOR )
	{
		{
			G_PlayEffect( G_EffectIndex( "light_knife/altmuzzle_flash" ), ent->client->renderInfo.handRPoint );

		}
	}
	// SKULL LIGHT 
	else if ( ent->client->NPC_class == CLASS_ARIEL || 
			ent->client->NPC_class == CLASS_MINEMONSTER ||
			ent->client->NPC_class == CLASS_DEMON_BLADE || 
			ent->client->NPC_class == CLASS_DEMON_POISON || 
			ent->client->NPC_class == CLASS_DEMON_FIRE || 
			ent->client->NPC_class == CLASS_JAWA || 
			ent->client->NPC_class == CLASS_RANCOR || 
			ent->client->NPC_class == CLASS_SHADOWTROOPER || 
			ent->client->NPC_class == CLASS_SABOTEUR || 
			ent->client->NPC_class == CLASS_MURJJ ||
			ent->client->NPC_class == CLASS_NECRO || 
			ent->client->NPC_class == CLASS_THRALL || 
			ent->client->NPC_class == CLASS_IMPERIAL || 
			ent->client->NPC_class == CLASS_CENOBITA || 
			ent->client->NPC_class == CLASS_DARK || 
			ent->client->NPC_class == CLASS_VOID || 
			ent->client->NPC_class == CLASS_POISON || 
			ent->client->NPC_class == CLASS_GOLEM_DARK || 
			ent->client->NPC_class == CLASS_HELLGOLEM || 
			ent->client->NPC_class == CLASS_GOLEM_NECRO ||
			ent->client->NPC_class == CLASS_GOLEM_SMOKE || 
			ent->client->NPC_class == CLASS_GOLEM_VOID ||
			ent->client->NPC_class == CLASS_GOLEM_POISON || 
			ent->client->NPC_class == CLASS_GOLEM_LAVA ||
			ent->client->NPC_class == CLASS_POLTER )
	{
		{
			G_PlayEffect( G_EffectIndex( "light_skull/altmuzzle_flash" ), ent->client->renderInfo.handRPoint );

		}
	}
	// MADNESS 
	else if ( ent->client->NPC_class == CLASS_TAVION ||
			ent->client->NPC_class == CLASS_DESANN ||
			ent->client->NPC_class == CLASS_HYLDEN || 
			ent->client->NPC_class == CLASS_HYLDENLORD || 
			ent->client->NPC_class == CLASS_SAND_CREATURE ||
			ent->client->NPC_class == CLASS_ATST ||
			ent->client->NPC_class == CLASS_SENTRY ||
			ent->client->NPC_class == CLASS_MOUSE || 
			ent->client->NPC_class == CLASS_TENTACLE ||  
			ent->client->NPC_class == CLASS_ELDERGOD || 
			ent->client->NPC_class == CLASS_SONIC || 
			ent->client->NPC_class == CLASS_GOLEM_SONIC || 
			ent->client->NPC_class == CLASS_PROPHET || 
			ent->client->NPC_class == CLASS_DECEIVER || 
			ent->client->NPC_class == CLASS_FLIER2 || 
			ent->client->NPC_class == CLASS_BEAST ||  
			ent->client->NPC_class == CLASS_SHARK )
	{
		{
			G_PlayEffect( G_EffectIndex( "madness/altmuzzle_flash" ), ent->client->renderInfo.handRPoint );

		}
	}
	// LIGHT DEFAULT SHOOTS 
	else if (ent->client->NPC_class == CLASS_BARTENDER ||
			ent->client->NPC_class == CLASS_BESPIN_COP ||
			ent->client->NPC_class == CLASS_COMMANDO || 
			ent->client->NPC_class == CLASS_HOWLER || 
			ent->client->NPC_class == CLASS_IMPWORKER || 
			ent->client->NPC_class == CLASS_WIZARD || 
			ent->client->NPC_class == CLASS_INTERROGATOR ||
			ent->client->NPC_class == CLASS_JEDI || 
			ent->client->NPC_class == CLASS_LANDO || 
			ent->client->NPC_class == CLASS_LUKE || 
			ent->client->NPC_class == CLASS_GONK || 
			ent->client->NPC_class == CLASS_PRISONER || 
			ent->client->NPC_class == CLASS_PROBE || 
			ent->client->NPC_class == CLASS_HAZARD_TROOPER || 
			ent->client->NPC_class == CLASS_REELO || 
			ent->client->NPC_class == CLASS_REMOTE || 
			ent->client->NPC_class == CLASS_SEEKER || 
			ent->client->NPC_class == CLASS_STORMTROOPER || 
			ent->client->NPC_class == CLASS_SWAMP || 
			ent->client->NPC_class == CLASS_TRANDOSHAN || 
			ent->client->NPC_class == CLASS_UGNAUGHT || 
			ent->client->NPC_class == CLASS_WEEQUAY || 
			ent->client->NPC_class == CLASS_PLAYER || 
			ent->client->NPC_class == CLASS_VEHICLE || 
			ent->client->NPC_class == CLASS_GALAKMECH ||
			ent->client->NPC_class == CLASS_ALCHEMIST || 
			ent->client->NPC_class == CLASS_DRUID ||
			ent->client->NPC_class == CLASS_REAVER || 
			ent->client->NPC_class == CLASS_RAZIEL ||
			ent->client->NPC_class == CLASS_NECROREAVER ||
			ent->client->NPC_class == CLASS_DARKREAVER || 
			ent->client->NPC_class == CLASS_TANKREAVER ||			
			ent->client->NPC_class == CLASS_DRUIDREAVER ||
			ent->client->NPC_class == CLASS_ICEREAVER || 
			ent->client->NPC_class == CLASS_SPIDERREAVER ||
			ent->client->NPC_class == CLASS_SPIDERREAVER2 ||
			ent->client->NPC_class == CLASS_SPIDERREAVER3 ||
			ent->client->NPC_class == CLASS_FIREREAVER || 
			ent->client->NPC_class == CLASS_AIRREAVER || 
			ent->client->NPC_class == CLASS_MENTALIST || 
			ent->client->NPC_class == CLASS_EARTHREAVER ||		 
			ent->client->NPC_class == CLASS_LIGHT || 
			ent->client->NPC_class == CLASS_FIRE ||  
			ent->client->NPC_class == CLASS_WATER || 
			ent->client->NPC_class == CLASS_CRYSTAL || 
			ent->client->NPC_class == CLASS_EARTH || 
			ent->client->NPC_class == CLASS_HOLY ||  
			ent->client->NPC_class == CLASS_GOLEM_FIRE ||
			ent->client->NPC_class == CLASS_GOLEM_WATER ||
			ent->client->NPC_class == CLASS_GOLEM_ICE ||			
			ent->client->NPC_class == CLASS_GOLEM ||
			ent->client->NPC_class == CLASS_GOLEM_HOLY )
	{
		{
			G_PlayEffect( G_EffectIndex( "bowcaster/altmuzzle_flash" ), ent->client->renderInfo.handRPoint );

		}
	}
	else if ( ent->client->NPC_class == CLASS_FIGHTER )
	{
		{
			G_PlayEffect( G_EffectIndex( "bowcaster0/altmuzzle_flash" ), ent->client->renderInfo.handRPoint );
		}
	}
}
else 
{
	// FULMINE
	if ( ent->client->NPC_class == CLASS_LIGHTREAVER || 
			ent->client->NPC_class == CLASS_LYCAN || 
			ent->client->NPC_class == CLASS_GOLEM_LIGHT || 
			ent->client->NPC_class == CLASS_SABER_DROID ||
			ent->client->NPC_class == CLASS_ASSASSIN_DROID )
	{
		{
			G_PlayEffect( G_EffectIndex( "bowcaster2/muzzle_flash" ), ent->client->renderInfo.handRPoint );
		}
	}
	// RADIANZA 
	else if ( ent->client->NPC_class == CLASS_MORGANKATARN ||
			ent->client->NPC_class == CLASS_GOLEM_GOLD || 
			ent->client->NPC_class == CLASS_PROTOCOL || 
			ent->client->NPC_class == CLASS_JAN || 
			ent->client->NPC_class == CLASS_CLAW )	{
		{
			G_PlayEffect( G_EffectIndex( "bowcaster3/muzzle_flash" ), ent->client->renderInfo.handRPoint );
		}
	}
	// ENERGY BOLT 
	else if ( ent->client->NPC_class == CLASS_REBORN ||
			ent->client->NPC_class == CLASS_KYLE ||
			ent->client->NPC_class == CLASS_ALORA ||
			ent->client->NPC_class == CLASS_MONMOTHA ||
			ent->client->NPC_class == CLASS_TUSKEN ||
			ent->client->NPC_class == CLASS_GLIDER ||
			ent->client->NPC_class == CLASS_NOGHRI ||
			ent->client->NPC_class == CLASS_FISH ||
			ent->client->NPC_class == CLASS_GRAN ||
			ent->client->NPC_class == CLASS_LIZARD ||
			ent->client->NPC_class == CLASS_SWAMPTROOPER ||
			ent->client->NPC_class == CLASS_REBEL ||
			ent->client->NPC_class == CLASS_ROCKETTROOPER ||
			ent->client->NPC_class == CLASS_BOBAFETT || 
			ent->client->NPC_class == CLASS_R2D2 ||
			ent->client->NPC_class == CLASS_R5D2 ||
			ent->client->NPC_class == CLASS_WAMPA || 
			ent->client->NPC_class == CLASS_GOLEM_METAL || 
			ent->client->NPC_class == CLASS_DEMON_ELECTRO || 			 
			ent->client->NPC_class == CLASS_MELC ||  
			ent->client->NPC_class == CLASS_SUMMONER ||
			ent->client->NPC_class == CLASS_ZEPH ||
			ent->client->NPC_class == CLASS_DUMAHIM || 
			ent->client->NPC_class == CLASS_RAHABIM ||
			ent->client->NPC_class == CLASS_TURELIM ||
			ent->client->NPC_class == CLASS_VAMPIRE ||
			ent->client->NPC_class == CLASS_VAMPIRE_DEMON ||
			ent->client->NPC_class == CLASS_VAMPIRE_GHOST || 
			ent->client->NPC_class == CLASS_KAIN ||
			ent->client->NPC_class == CLASS_KAIN2 || 
			ent->client->NPC_class == CLASS_KAIN3 ||
			ent->client->NPC_class == CLASS_UMAH || 
			ent->client->NPC_class == CLASS_VORADOR ||
			ent->client->NPC_class == CLASS_JANOS ||
			ent->client->NPC_class == CLASS_NECROMANCER || 
			ent->client->NPC_class == CLASS_VAMPIRE_QUEEN ||
			ent->client->NPC_class == CLASS_VAMPIRE_DARK || 
			ent->client->NPC_class == CLASS_VAMPIRE_NECRO || 
			ent->client->NPC_class == CLASS_VAMPIRE_NECRO2 ||
			ent->client->NPC_class == CLASS_VAMPIRE_FEAR || 
			ent->client->NPC_class == CLASS_VAMPIRE_FEAR2 ||  
			ent->client->NPC_class == CLASS_VAMPIRE_BLOOD ||
			ent->client->NPC_class == CLASS_VAMPIRE_SOUND || 
			ent->client->NPC_class == CLASS_VAMPIRE_LYCAN || 
			ent->client->NPC_class == CLASS_VAMPIRE_ELF || 
			ent->client->NPC_class == CLASS_VAMPIRE_ELF2 ||
			ent->client->NPC_class == CLASS_VAMPIRE_ELF3 ||
			ent->client->NPC_class == CLASS_VAMPIRE_WATER || 
			ent->client->NPC_class == CLASS_VAMPIRE_FROST ||  
			ent->client->NPC_class == CLASS_VAMPIRE_ARCHER || 
			ent->client->NPC_class == CLASS_HALFVAMPIRE || 
			ent->client->NPC_class == CLASS_RAZIELHIM ||  
			ent->client->NPC_class == CLASS_ESKANDOR ||  
			ent->client->NPC_class == CLASS_VAMPIRE_BOUNTY || 
			ent->client->NPC_class == CLASS_VAMPIRE_ZOMBIE ||
			ent->client->NPC_class == CLASS_VAMPIRE_SARAFAN || 
			ent->client->NPC_class == CLASS_AIR || 
			ent->client->NPC_class == CLASS_GOLEM_AIR || 
			ent->client->NPC_class == CLASS_BIRD || 
			ent->client->NPC_class == CLASS_IVY )
	{
		{
			G_PlayEffect( G_EffectIndex( "energy_bolt/muzzle_flash" ), ent->client->renderInfo.handRPoint );

		}
	}
	// LIGHT KNIVES 
	else if ( ent->client->NPC_class == CLASS_GALAK ||
			ent->client->NPC_class == CLASS_RODIAN ||
			ent->client->NPC_class == CLASS_MARK1 ||
			ent->client->NPC_class == CLASS_MARK2 || 
			ent->client->NPC_class == CLASS_HUNTER || 
			ent->client->NPC_class == CLASS_SCOUT ||  
			ent->client->NPC_class == CLASS_SARAFAN || 
			ent->client->NPC_class == CLASS_ARCHER ||  
			ent->client->NPC_class == CLASS_WARRIOR )
	{
		{
			G_PlayEffect( G_EffectIndex( "light_knife/muzzle_flash" ), ent->client->renderInfo.handRPoint );

		}
	}
	// SKULL LIGHT 
	else if ( ent->client->NPC_class == CLASS_ARIEL || 
			ent->client->NPC_class == CLASS_MINEMONSTER ||
			ent->client->NPC_class == CLASS_DEMON_BLADE || 
			ent->client->NPC_class == CLASS_DEMON_POISON || 
			ent->client->NPC_class == CLASS_DEMON_FIRE || 
			ent->client->NPC_class == CLASS_JAWA || 
			ent->client->NPC_class == CLASS_RANCOR || 
			ent->client->NPC_class == CLASS_SHADOWTROOPER || 
			ent->client->NPC_class == CLASS_SABOTEUR || 
			ent->client->NPC_class == CLASS_MURJJ ||
			ent->client->NPC_class == CLASS_NECRO || 
			ent->client->NPC_class == CLASS_THRALL || 
			ent->client->NPC_class == CLASS_IMPERIAL || 
			ent->client->NPC_class == CLASS_CENOBITA || 
			ent->client->NPC_class == CLASS_DARK || 
			ent->client->NPC_class == CLASS_VOID || 
			ent->client->NPC_class == CLASS_POISON || 
			ent->client->NPC_class == CLASS_GOLEM_DARK || 
			ent->client->NPC_class == CLASS_HELLGOLEM || 
			ent->client->NPC_class == CLASS_GOLEM_NECRO ||
			ent->client->NPC_class == CLASS_GOLEM_SMOKE || 
			ent->client->NPC_class == CLASS_GOLEM_VOID ||
			ent->client->NPC_class == CLASS_GOLEM_POISON || 
			ent->client->NPC_class == CLASS_GOLEM_LAVA ||
			ent->client->NPC_class == CLASS_POLTER )
	{
		{
			G_PlayEffect( G_EffectIndex( "light_skull/muzzle_flash" ), ent->client->renderInfo.handRPoint );

		}
	}
	// MADNESS 
	else if ( ent->client->NPC_class == CLASS_TAVION ||
			ent->client->NPC_class == CLASS_DESANN ||
			ent->client->NPC_class == CLASS_HYLDEN || 
			ent->client->NPC_class == CLASS_HYLDENLORD || 
			ent->client->NPC_class == CLASS_SAND_CREATURE ||
			ent->client->NPC_class == CLASS_ATST ||
			ent->client->NPC_class == CLASS_SENTRY ||
			ent->client->NPC_class == CLASS_MOUSE || 
			ent->client->NPC_class == CLASS_TENTACLE ||  
			ent->client->NPC_class == CLASS_ELDERGOD || 
			ent->client->NPC_class == CLASS_SONIC || 
			ent->client->NPC_class == CLASS_GOLEM_SONIC || 
			ent->client->NPC_class == CLASS_PROPHET || 
			ent->client->NPC_class == CLASS_DECEIVER || 
			ent->client->NPC_class == CLASS_FLIER2 || 
			ent->client->NPC_class == CLASS_BEAST ||  
			ent->client->NPC_class == CLASS_SHARK )
	{
		{
			G_PlayEffect( G_EffectIndex( "madness/muzzle_flash" ), ent->client->renderInfo.handRPoint );

		}
	}
	// LIGHT DEFAULT SHOOTS 
	else if ( ent->client->NPC_class == CLASS_BARTENDER ||
			ent->client->NPC_class == CLASS_BESPIN_COP ||
			ent->client->NPC_class == CLASS_COMMANDO || 
			ent->client->NPC_class == CLASS_HOWLER || 
			ent->client->NPC_class == CLASS_IMPWORKER || 
			ent->client->NPC_class == CLASS_WIZARD || 
			ent->client->NPC_class == CLASS_INTERROGATOR ||
			ent->client->NPC_class == CLASS_JEDI || 
			ent->client->NPC_class == CLASS_LANDO || 
			ent->client->NPC_class == CLASS_LUKE || 
			ent->client->NPC_class == CLASS_GONK || 
			ent->client->NPC_class == CLASS_PRISONER || 
			ent->client->NPC_class == CLASS_PROBE || 
			ent->client->NPC_class == CLASS_HAZARD_TROOPER || 
			ent->client->NPC_class == CLASS_REELO || 
			ent->client->NPC_class == CLASS_REMOTE || 
			ent->client->NPC_class == CLASS_SEEKER || 
			ent->client->NPC_class == CLASS_STORMTROOPER || 
			ent->client->NPC_class == CLASS_SWAMP || 
			ent->client->NPC_class == CLASS_TRANDOSHAN || 
			ent->client->NPC_class == CLASS_UGNAUGHT || 
			ent->client->NPC_class == CLASS_WEEQUAY || 
			ent->client->NPC_class == CLASS_PLAYER || 
			ent->client->NPC_class == CLASS_VEHICLE || 
			ent->client->NPC_class == CLASS_GALAKMECH ||
			ent->client->NPC_class == CLASS_ALCHEMIST || 
			ent->client->NPC_class == CLASS_DRUID ||
			ent->client->NPC_class == CLASS_REAVER || 
			ent->client->NPC_class == CLASS_RAZIEL ||
			ent->client->NPC_class == CLASS_NECROREAVER ||
			ent->client->NPC_class == CLASS_DARKREAVER || 
			ent->client->NPC_class == CLASS_TANKREAVER ||			
			ent->client->NPC_class == CLASS_DRUIDREAVER ||
			ent->client->NPC_class == CLASS_ICEREAVER || 
			ent->client->NPC_class == CLASS_SPIDERREAVER ||
			ent->client->NPC_class == CLASS_SPIDERREAVER2 ||
			ent->client->NPC_class == CLASS_SPIDERREAVER3 ||
			ent->client->NPC_class == CLASS_FIREREAVER || 
			ent->client->NPC_class == CLASS_AIRREAVER || 
			ent->client->NPC_class == CLASS_MENTALIST || 
			ent->client->NPC_class == CLASS_EARTHREAVER ||		 
			ent->client->NPC_class == CLASS_LIGHT || 
			ent->client->NPC_class == CLASS_FIRE ||  
			ent->client->NPC_class == CLASS_WATER || 
			ent->client->NPC_class == CLASS_CRYSTAL || 
			ent->client->NPC_class == CLASS_EARTH || 
			ent->client->NPC_class == CLASS_HOLY ||  
			ent->client->NPC_class == CLASS_GOLEM_FIRE ||
			ent->client->NPC_class == CLASS_GOLEM_WATER ||
			ent->client->NPC_class == CLASS_GOLEM_ICE ||			
			ent->client->NPC_class == CLASS_GOLEM ||
			ent->client->NPC_class == CLASS_GOLEM_HOLY )
	{
		{
			G_PlayEffect( G_EffectIndex( "bowcaster/muzzle_flash" ), ent->client->renderInfo.handRPoint );

		}
	}
	else if ( ent->client->NPC_class == CLASS_FIGHTER )
	{
		{
			G_PlayEffect( G_EffectIndex( "bowcaster0/muzzle_flash" ), ent->client->renderInfo.handRPoint );
		}
	}
}
}