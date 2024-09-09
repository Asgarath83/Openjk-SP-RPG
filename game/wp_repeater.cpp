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
//	Heavy Repeater
//-------------------

//---------------------------------------------------------
static void WP_RepeaterMainFire( gentity_t *ent, vec3_t dir )
//---------------------------------------------------------
{
	vec3_t	start;
	int		damage	= weaponData[WP_REPEATER].damage;
	// DMG GHIACCIO
	if ( ent && ent->client && ent->client->NPC_class == CLASS_BARTENDER )
	{
		damage *= 1.5;
	}
	// DMG CRISTALLO
	else if ( ent && ent->client && ent->client->NPC_class == CLASS_WEEQUAY ||
		      ent && ent->client && ent->client->NPC_class == CLASS_GALAKMECH )
	{
		damage *= 2; // Elementali del Calore fanno 50% di danno in più!
	}
	VectorCopy( muzzle, start );
	WP_TraceSetStart( ent, start, vec3_origin, vec3_origin );//make sure our start point isn't on the other side of a wall

	WP_MissileTargetHint(ent, start, dir);

	gentity_t *missile = CreateMissile( start, dir, REPEATER_VELOCITY, 10000, ent );

	missile->classname = "repeater_proj";
	missile->s.weapon = WP_REPEATER;

	// Do the damages
	if ( ent->s.number != 0 )
	{
		if ( g_spskill->integer == 0 )
		{
			damage = REPEATER_NPC_DAMAGE_EASY;
		}
		else if ( g_spskill->integer == 1 )
		{
			damage = REPEATER_NPC_DAMAGE_NORMAL;
		}
		else
		{
			damage = REPEATER_NPC_DAMAGE_HARD;
		}
	}

	// FONT OF PUTRESCENCE 
	if ( ent->client->NPC_class == CLASS_REBORN || 
				ent->client->NPC_class == CLASS_KYLE || 
				ent->client->NPC_class == CLASS_TUSKEN || 
				ent->client->NPC_class == CLASS_LIZARD || 
				ent->client->NPC_class == CLASS_MONMOTHA || 
				ent->client->NPC_class == CLASS_GLIDER || 
				ent->client->NPC_class == CLASS_NOGHRI || 
				ent->client->NPC_class == CLASS_FLIER2 || 
				ent->client->NPC_class == CLASS_GRAN || 
				ent->client->NPC_class == CLASS_R2D2 || 
				ent->client->NPC_class == CLASS_R5D2 || 
				ent->client->NPC_class == CLASS_RANCOR || 
				ent->client->NPC_class == CLASS_SLUAGH || 
				ent->client->NPC_class == CLASS_MINEMONSTER || 
				ent->client->NPC_class == CLASS_POISON || 
				ent->client->NPC_class == CLASS_DEMON_POISON || 
				ent->client->NPC_class == CLASS_GOLEM_POISON || 
				ent->client->NPC_class == CLASS_MELC ||  
				ent->client->NPC_class == CLASS_SUMMONER || 
				ent->client->NPC_class == CLASS_ZEPH || 
				ent->client->NPC_class == CLASS_DECEIVER || 
				ent->client->NPC_class == CLASS_DUMAHIM || 
				ent->client->NPC_class == CLASS_TURELIM || 
				ent->client->NPC_class == CLASS_VAMPIRE || 
				ent->client->NPC_class == CLASS_KAIN || 
				ent->client->NPC_class == CLASS_KAIN2 || 
				ent->client->NPC_class == CLASS_KAIN3 || 
				ent->client->NPC_class == CLASS_UMAH || 
				ent->client->NPC_class == CLASS_VORADOR || 
				ent->client->NPC_class == CLASS_JANOS || 
				ent->client->NPC_class == CLASS_VAMPIRE_DARK || 
				ent->client->NPC_class == CLASS_VAMPIRE_NECRO || 
				ent->client->NPC_class == CLASS_VAMPIRE_FEAR || 
				ent->client->NPC_class == CLASS_VAMPIRE_BLOOD || 
				ent->client->NPC_class == CLASS_VAMPIRE_SOUND || 
				ent->client->NPC_class == CLASS_VAMPIRE_LYCAN || 
				ent->client->NPC_class == CLASS_VAMPIRE_ELF || 
				ent->client->NPC_class == CLASS_VAMPIRE_ELF2 || 
				ent->client->NPC_class == CLASS_VAMPIRE_ELF3 || 
			  	ent->client->NPC_class == CLASS_VAMPIRE_ARCHER || 
				ent->client->NPC_class == CLASS_HALFVAMPIRE || 
				ent->client->NPC_class == CLASS_VAMPIRE_BOUNTY || 
				ent->client->NPC_class == CLASS_VAMPIRE_ZOMBIE || 
				ent->client->NPC_class == CLASS_VAMPIRE_SARAFAN ||  
				ent->client->NPC_class == CLASS_VAMPIRE_GHOST || 
				ent->client->NPC_class == CLASS_BEAST )
	{
		damage *= 2.25;
	}
	// Necrotic 
	else if ( ent->client->NPC_class == CLASS_VAMPIRE_FEAR2 ||
				ent->client->NPC_class == CLASS_THRALL || 
				ent->client->NPC_class == CLASS_NECRO || 
				ent->client->NPC_class == CLASS_GOLEM_NECRO 	)
	{
		damage *= 2.3;
	}
	else if ( ent->client->NPC_class == CLASS_MURJJ ||
				ent->client->NPC_class == CLASS_NECROMANCER || 
				ent->client->NPC_class == CLASS_VAMPIRE_QUEEN || 
				ent->client->NPC_class == CLASS_VAMPIRE_NECRO2 )
	{
		damage *= 2.5;
	}
	// DEMONIC PUTRESCENSE 
	else if ( ent->client->NPC_class == CLASS_IMPERIAL || 
				ent->client->NPC_class == CLASS_CENOBITA || 
				ent->client->NPC_class == CLASS_PROPHET || 
				ent->client->NPC_class == CLASS_WAMPA || 
				ent->client->NPC_class == CLASS_VAMPIRE_DEMON || 
				ent->client->NPC_class == CLASS_DEMON_FIRE || 
				ent->client->NPC_class == CLASS_DEMON_BLADE || 
				ent->client->NPC_class == CLASS_DEMON_ELECTRO || 
				ent->client->NPC_class == CLASS_HYLDEN || 
				ent->client->NPC_class == CLASS_HYLDENLORD || 
				ent->client->NPC_class == CLASS_TAVION || 
				ent->client->NPC_class == CLASS_DESANN )
	{
		damage *= 2.5;
	}
	// ICE PROJECTILES 
	else if (  ent->client->NPC_class == CLASS_RAZIELHIM ||
		ent->client->NPC_class == CLASS_BARTENDER || 
				ent->client->NPC_class == CLASS_LYCAN || 
				ent->client->NPC_class == CLASS_ICEREAVER || 
				ent->client->NPC_class == CLASS_VAMPIRE_FROST || 
				ent->client->NPC_class == CLASS_GOLEM_ICE || 
				ent->client->NPC_class == CLASS_POLTER || 
				ent->client->NPC_class == CLASS_PROBE )
	{
		damage *= 1.5;
	}
	// THE CRYSTAL PROJECTILES  
	else if ( ent->client->NPC_class == CLASS_ESKANDOR || 
				ent->client->NPC_class == CLASS_GALAKMECH || 
				ent->client->NPC_class == CLASS_CRYSTAL || 
				ent->client->NPC_class == CLASS_WEEQUAY || 
				ent->client->NPC_class == CLASS_GONK || 
				ent->client->NPC_class == CLASS_GOLEM || 
				ent->client->NPC_class == CLASS_EARTH )
	{
		damage *= 2;
	}
	// THE VAPOUR PROJECTILES  
	else if ( ent->client->NPC_class == CLASS_INTERROGATOR || 
				ent->client->NPC_class == CLASS_HAZARD_TROOPER || 
				ent->client->NPC_class == CLASS_SABER_DROID || 
				ent->client->NPC_class == CLASS_PROTOCOL || 
				ent->client->NPC_class == CLASS_CLAW || 
				ent->client->NPC_class == CLASS_JAWA || 
				ent->client->NPC_class == CLASS_GHOST || 
				ent->client->NPC_class == CLASS_GOLEM_LIGHT || 
				ent->client->NPC_class == CLASS_GOLEM_GOLD || 
				ent->client->NPC_class == CLASS_FIRE || 
				ent->client->NPC_class == CLASS_GOLEM_FIRE || 
				ent->client->NPC_class == CLASS_HELLGOLEM || 
				ent->client->NPC_class == CLASS_GOLEM_LAVA || 
				ent->client->NPC_class == CLASS_BIRD || 
				ent->client->NPC_class == CLASS_AIR || 
				ent->client->NPC_class == CLASS_GOLEM_AIR || 
				ent->client->NPC_class == CLASS_LIGHT || 
				ent->client->NPC_class == CLASS_GOLEM_SMOKE  )
	{
		damage *= 1.25;
	}
	// TAR TENTACLE MUD  
	else if ( ent->client->NPC_class == CLASS_SHADOWTROOPER || 
				ent->client->NPC_class == CLASS_VOID || 
				ent->client->NPC_class == CLASS_GOLEM_VOID || 
				ent->client->NPC_class == CLASS_ELDERGOD || 
				ent->client->NPC_class == CLASS_TENTACLE || 
				ent->client->NPC_class == CLASS_REAPER || 
				ent->client->NPC_class == CLASS_DREADNOUGHT || 
				ent->client->NPC_class == CLASS_SENTRY || 
				ent->client->NPC_class == CLASS_MOUSE || 
				ent->client->NPC_class == CLASS_ATST || 
				ent->client->NPC_class == CLASS_SAND_CREATURE )
	{
		damage *= 3;
	}
	// TAR BROWN MUD 
	else if ( ent->client->NPC_class == CLASS_SABOTEUR || 
				ent->client->NPC_class == CLASS_DARK || 
				ent->client->NPC_class == CLASS_GOLEM_DARK || 
				ent->client->NPC_class == CLASS_UGNAUGHT || 
				ent->client->NPC_class == CLASS_SPIDERREAVER2 || 
				ent->client->NPC_class == CLASS_SPIDERREAVER3 || 
				ent->client->NPC_class == CLASS_REELO || 
				ent->client->NPC_class == CLASS_ALCHEMIST || 
				ent->client->NPC_class == CLASS_IVY || 
				ent->client->NPC_class == CLASS_SWAMP )
	{
		damage *= 1.25;
	}
	// THE HOLY WATER
	else if ( ent->client->NPC_class == CLASS_RODIAN || 
				ent->client->NPC_class == CLASS_GALAK || 
				ent->client->NPC_class == CLASS_STORMTROOPER || 
				ent->client->NPC_class == CLASS_JAN || 
				ent->client->NPC_class == CLASS_MARK1 || 
				ent->client->NPC_class == CLASS_MARK2 || 
				ent->client->NPC_class == CLASS_ARIEL || 	
				ent->client->NPC_class == CLASS_SARAFAN || 
				ent->client->NPC_class == CLASS_ARCHER ||  
				ent->client->NPC_class == CLASS_WARRIOR || 
				ent->client->NPC_class == CLASS_HUNTER || 
				ent->client->NPC_class == CLASS_SCOUT ||  
				ent->client->NPC_class == CLASS_HOLY || 
				ent->client->NPC_class == CLASS_GOLEM_HOLY  )
	{
		damage *= 1.6;
	}
	// RAHABIM IDROCINESIS   
	else if ( ent->client->NPC_class == CLASS_ALORA || 
				ent->client->NPC_class == CLASS_VAMPIRE_WATER || 
				ent->client->NPC_class == CLASS_RAHABIM || 
				ent->client->NPC_class == CLASS_SWAMPTROOPER || 
				ent->client->NPC_class == CLASS_GOLEM_WATER )
	{
		damage *= 1.1;
	}
	// RAHABIM IDROCINESIS BOSS   
	else if ( ent->client->NPC_class == CLASS_FISH )
	{
		damage *= 1.25;
	}
	// DEFAULT WATERBALLS  
	else if (	ent->client->NPC_class == CLASS_HOWLER || 
				ent->client->NPC_class == CLASS_JEDI || 
				ent->client->NPC_class == CLASS_LANDO || 
				ent->client->NPC_class == CLASS_LUKE || 
				ent->client->NPC_class == CLASS_MORGANKATARN || 
				ent->client->NPC_class == CLASS_PRISONER || 
				ent->client->NPC_class == CLASS_REBEL || 
				ent->client->NPC_class == CLASS_REMOTE || 
				ent->client->NPC_class == CLASS_SEEKER || 
				ent->client->NPC_class == CLASS_TRANDOSHAN || 
				ent->client->NPC_class == CLASS_BESPIN_COP || 
				ent->client->NPC_class == CLASS_BOBAFETT || 
				ent->client->NPC_class == CLASS_ROCKETTROOPER || 
				ent->client->NPC_class == CLASS_ASSASSIN_DROID || 
				ent->client->NPC_class == CLASS_IMPWORKER || 
				ent->client->NPC_class == CLASS_WIZARD || 
				ent->client->NPC_class == CLASS_PLAYER || 
				ent->client->NPC_class == CLASS_VEHICLE || 
				ent->client->NPC_class == CLASS_DRUID || 
				ent->client->NPC_class == CLASS_REAVER || 
				ent->client->NPC_class == CLASS_RAZIEL ||
				ent->client->NPC_class == CLASS_NECROREAVER ||
				ent->client->NPC_class == CLASS_DARKREAVER || 
				ent->client->NPC_class == CLASS_TANKREAVER ||
				ent->client->NPC_class == CLASS_LIGHTREAVER ||
				ent->client->NPC_class == CLASS_DRUIDREAVER ||
			 	ent->client->NPC_class == CLASS_SPIDERREAVER || 				
				ent->client->NPC_class == CLASS_FIREREAVER || 
				ent->client->NPC_class == CLASS_AIRREAVER || 
				ent->client->NPC_class == CLASS_MENTALIST || 
				ent->client->NPC_class == CLASS_EARTHREAVER || 			  
				ent->client->NPC_class == CLASS_WATER || 			 
			  	ent->client->NPC_class == CLASS_SONIC || 
				ent->client->NPC_class == CLASS_SHARK ||  
				ent->client->NPC_class == CLASS_GOLEM_METAL ||			
				ent->client->NPC_class == CLASS_GOLEM_SONIC )
	{
		damage *= 1;
	}

//	if ( ent->client && ent->client->ps.powerups[PW_WEAPON_OVERCHARGE] > 0 && ent->client->ps.powerups[PW_WEAPON_OVERCHARGE] > cg.time )
//	{
//		// in overcharge mode, so doing double damage
//		missile->flags |= FL_OVERCHARGED;
//		damage *= 2;
//	}

	missile->damage = damage;
	missile->dflags = DAMAGE_DEATH_KNOCKBACK;
	missile->methodOfDeath = MOD_REPEATER;
	missile->clipmask = MASK_SHOT | CONTENTS_LIGHTSABER;

	// we don't want it to bounce forever
	missile->bounceCount = 8;
}

//---------------------------------------------------------
static void WP_RepeaterAltFire( gentity_t *ent )
//---------------------------------------------------------
{
	vec3_t	start;
	int		damage	= weaponData[WP_REPEATER].altDamage;
	gentity_t *missile = NULL;

	VectorCopy( muzzle, start );
	WP_TraceSetStart( ent, start, vec3_origin, vec3_origin );//make sure our start point isn't on the other side of a wall

	if ( ent->client && ent->client->NPC_class == CLASS_GALAKMECH )
	{
		missile = CreateMissile( start, ent->client->hiddenDir, ent->client->hiddenDist, 10000, ent, qtrue );
	}
	else
	{
		WP_MissileTargetHint(ent, start, forwardVec);
		missile = CreateMissile( start, forwardVec, REPEATER_ALT_VELOCITY, 10000, ent, qtrue );
	}

	missile->classname = "repeater_alt_proj";
	missile->s.weapon = WP_REPEATER;
	missile->mass = 10;

	// Do the damages
	if ( ent->s.number != 0 )
	{
		if ( g_spskill->integer == 0 )
		{
			damage = REPEATER_ALT_NPC_DAMAGE_EASY;
		}
		else if ( g_spskill->integer == 1 )
		{
			damage = REPEATER_ALT_NPC_DAMAGE_NORMAL;
		}
		else
		{
			damage = REPEATER_ALT_NPC_DAMAGE_HARD;
		}
	}

		
	// FONT OF PUTRESCENCE 
	if ( ent->client->NPC_class == CLASS_REBORN || 
				ent->client->NPC_class == CLASS_KYLE || 
				ent->client->NPC_class == CLASS_TUSKEN || 
				ent->client->NPC_class == CLASS_LIZARD || 
				ent->client->NPC_class == CLASS_MONMOTHA || 
				ent->client->NPC_class == CLASS_GLIDER || 
				ent->client->NPC_class == CLASS_NOGHRI || 
				ent->client->NPC_class == CLASS_FLIER2 || 
				ent->client->NPC_class == CLASS_GRAN || 
				ent->client->NPC_class == CLASS_R2D2 || 
				ent->client->NPC_class == CLASS_R5D2 || 
				ent->client->NPC_class == CLASS_RANCOR || 
				ent->client->NPC_class == CLASS_SLUAGH || 
				ent->client->NPC_class == CLASS_MINEMONSTER || 
				ent->client->NPC_class == CLASS_POISON || 
				ent->client->NPC_class == CLASS_DEMON_POISON || 
				ent->client->NPC_class == CLASS_GOLEM_POISON || 
				ent->client->NPC_class == CLASS_MELC ||  
				ent->client->NPC_class == CLASS_SUMMONER || 
				ent->client->NPC_class == CLASS_ZEPH || 
				ent->client->NPC_class == CLASS_DECEIVER || 
				ent->client->NPC_class == CLASS_DUMAHIM || 
				ent->client->NPC_class == CLASS_TURELIM || 
				ent->client->NPC_class == CLASS_VAMPIRE || 
				ent->client->NPC_class == CLASS_KAIN || 
				ent->client->NPC_class == CLASS_KAIN2 || 
				ent->client->NPC_class == CLASS_KAIN3 || 
				ent->client->NPC_class == CLASS_UMAH || 
				ent->client->NPC_class == CLASS_VORADOR || 
				ent->client->NPC_class == CLASS_JANOS || 
				ent->client->NPC_class == CLASS_VAMPIRE_DARK || 
				ent->client->NPC_class == CLASS_VAMPIRE_NECRO || 
				ent->client->NPC_class == CLASS_VAMPIRE_FEAR || 
				ent->client->NPC_class == CLASS_VAMPIRE_BLOOD || 
				ent->client->NPC_class == CLASS_VAMPIRE_SOUND || 
				ent->client->NPC_class == CLASS_VAMPIRE_LYCAN || 
				ent->client->NPC_class == CLASS_VAMPIRE_ELF || 
				ent->client->NPC_class == CLASS_VAMPIRE_ELF2 || 
				ent->client->NPC_class == CLASS_VAMPIRE_ELF3 || 
			  	ent->client->NPC_class == CLASS_VAMPIRE_ARCHER || 
				ent->client->NPC_class == CLASS_HALFVAMPIRE || 
				ent->client->NPC_class == CLASS_VAMPIRE_BOUNTY || 
				ent->client->NPC_class == CLASS_VAMPIRE_ZOMBIE || 
				ent->client->NPC_class == CLASS_VAMPIRE_SARAFAN ||  
				ent->client->NPC_class == CLASS_VAMPIRE_GHOST || 
				ent->client->NPC_class == CLASS_BEAST )
	{
		damage *= 2.25;
	}
	// Necrotic 
	else if ( ent->client->NPC_class == CLASS_VAMPIRE_FEAR2 ||
				ent->client->NPC_class == CLASS_THRALL || 
				ent->client->NPC_class == CLASS_NECRO || 
				ent->client->NPC_class == CLASS_GOLEM_NECRO 	)
	{
		damage *= 2.3;
	}
	else if ( ent->client->NPC_class == CLASS_MURJJ ||
				ent->client->NPC_class == CLASS_NECROMANCER || 
				ent->client->NPC_class == CLASS_VAMPIRE_QUEEN || 
				ent->client->NPC_class == CLASS_VAMPIRE_NECRO2 )
	{
		damage *= 2.5;
	}
	// DEMONIC PUTRESCENSE 
	else if ( ent->client->NPC_class == CLASS_IMPERIAL || 
				ent->client->NPC_class == CLASS_CENOBITA || 
				ent->client->NPC_class == CLASS_PROPHET || 
				ent->client->NPC_class == CLASS_WAMPA || 
				ent->client->NPC_class == CLASS_VAMPIRE_DEMON || 
				ent->client->NPC_class == CLASS_DEMON_FIRE || 
				ent->client->NPC_class == CLASS_DEMON_BLADE || 
				ent->client->NPC_class == CLASS_DEMON_ELECTRO || 
				ent->client->NPC_class == CLASS_HYLDEN || 
				ent->client->NPC_class == CLASS_HYLDENLORD || 
				ent->client->NPC_class == CLASS_TAVION || 
				ent->client->NPC_class == CLASS_DESANN )
	{
		damage *= 2.5;
	}
	// ICE PROJECTILES 
	else if (  ent->client->NPC_class == CLASS_RAZIELHIM ||
		ent->client->NPC_class == CLASS_BARTENDER || 
				ent->client->NPC_class == CLASS_LYCAN || 
				ent->client->NPC_class == CLASS_ICEREAVER || 
				ent->client->NPC_class == CLASS_VAMPIRE_FROST || 
				ent->client->NPC_class == CLASS_GOLEM_ICE || 
				ent->client->NPC_class == CLASS_POLTER || 
				ent->client->NPC_class == CLASS_PROBE )
	{
		damage *= 1.5;
	}
	// THE CRYSTAL PROJECTILES  
	else if ( ent->client->NPC_class == CLASS_ESKANDOR || 
				ent->client->NPC_class == CLASS_GALAKMECH || 
				ent->client->NPC_class == CLASS_CRYSTAL || 
				ent->client->NPC_class == CLASS_WEEQUAY || 
				ent->client->NPC_class == CLASS_GONK || 
				ent->client->NPC_class == CLASS_GOLEM || 
				ent->client->NPC_class == CLASS_EARTH )
	{
		damage *= 2;
	}
	// THE VAPOUR PROJECTILES  
	else if ( ent->client->NPC_class == CLASS_INTERROGATOR || 
				ent->client->NPC_class == CLASS_HAZARD_TROOPER || 
				ent->client->NPC_class == CLASS_SABER_DROID || 
				ent->client->NPC_class == CLASS_PROTOCOL || 
				ent->client->NPC_class == CLASS_CLAW || 
				ent->client->NPC_class == CLASS_JAWA || 
				ent->client->NPC_class == CLASS_GHOST || 
				ent->client->NPC_class == CLASS_GOLEM_LIGHT || 
				ent->client->NPC_class == CLASS_GOLEM_GOLD || 
				ent->client->NPC_class == CLASS_FIRE || 
				ent->client->NPC_class == CLASS_GOLEM_FIRE || 
				ent->client->NPC_class == CLASS_HELLGOLEM || 
				ent->client->NPC_class == CLASS_GOLEM_LAVA || 
				ent->client->NPC_class == CLASS_BIRD || 
				ent->client->NPC_class == CLASS_AIR || 
				ent->client->NPC_class == CLASS_GOLEM_AIR || 
				ent->client->NPC_class == CLASS_LIGHT || 
				ent->client->NPC_class == CLASS_GOLEM_SMOKE  )
	{
		damage *= 1.25;
	}
	// TAR TENTACLE MUD  
	else if ( ent->client->NPC_class == CLASS_SHADOWTROOPER || 
				ent->client->NPC_class == CLASS_VOID || 
				ent->client->NPC_class == CLASS_GOLEM_VOID || 
				ent->client->NPC_class == CLASS_ELDERGOD || 
				ent->client->NPC_class == CLASS_TENTACLE || 
				ent->client->NPC_class == CLASS_REAPER || 
				ent->client->NPC_class == CLASS_DREADNOUGHT || 
				ent->client->NPC_class == CLASS_SENTRY || 
				ent->client->NPC_class == CLASS_MOUSE || 
				ent->client->NPC_class == CLASS_ATST || 
				ent->client->NPC_class == CLASS_SAND_CREATURE )
	{
		damage *= 3;
	}
	// TAR BROWN MUD 
	else if ( ent->client->NPC_class == CLASS_SABOTEUR || 
				ent->client->NPC_class == CLASS_DARK || 
				ent->client->NPC_class == CLASS_GOLEM_DARK || 
				ent->client->NPC_class == CLASS_UGNAUGHT || 
				ent->client->NPC_class == CLASS_SPIDERREAVER2 || 
				ent->client->NPC_class == CLASS_SPIDERREAVER3 || 
				ent->client->NPC_class == CLASS_REELO || 
				ent->client->NPC_class == CLASS_ALCHEMIST || 
				ent->client->NPC_class == CLASS_IVY || 
				ent->client->NPC_class == CLASS_SWAMP )
	{
		damage *= 1.25;
	}
	// THE HOLY WATER
	else if ( ent->client->NPC_class == CLASS_RODIAN || 
				ent->client->NPC_class == CLASS_GALAK || 
				ent->client->NPC_class == CLASS_STORMTROOPER || 
				ent->client->NPC_class == CLASS_JAN || 
				ent->client->NPC_class == CLASS_MARK1 || 
				ent->client->NPC_class == CLASS_MARK2 || 
				ent->client->NPC_class == CLASS_ARIEL || 	
				ent->client->NPC_class == CLASS_SARAFAN || 
				ent->client->NPC_class == CLASS_ARCHER ||  
				ent->client->NPC_class == CLASS_WARRIOR || 
				ent->client->NPC_class == CLASS_HUNTER || 
				ent->client->NPC_class == CLASS_SCOUT ||  
				ent->client->NPC_class == CLASS_HOLY || 
				ent->client->NPC_class == CLASS_GOLEM_HOLY  )
	{
		damage *= 1.6;
	}
	// RAHABIM IDROCINESIS   
	else if ( ent->client->NPC_class == CLASS_ALORA || 
				ent->client->NPC_class == CLASS_VAMPIRE_WATER || 
				ent->client->NPC_class == CLASS_RAHABIM || 
				ent->client->NPC_class == CLASS_SWAMPTROOPER || 
				ent->client->NPC_class == CLASS_GOLEM_WATER )
	{
		damage *= 1.1;
	}
	// RAHABIM IDROCINESIS BOSS   
	else if ( ent->client->NPC_class == CLASS_FISH )
	{
		damage *= 1.25;
	}
	// DEFAULT WATERBALLS  
	else if (	ent->client->NPC_class == CLASS_HOWLER || 
				ent->client->NPC_class == CLASS_JEDI || 
				ent->client->NPC_class == CLASS_LANDO || 
				ent->client->NPC_class == CLASS_LUKE || 
				ent->client->NPC_class == CLASS_MORGANKATARN || 
				ent->client->NPC_class == CLASS_PRISONER || 
				ent->client->NPC_class == CLASS_REBEL || 
				ent->client->NPC_class == CLASS_REMOTE || 
				ent->client->NPC_class == CLASS_SEEKER || 
				ent->client->NPC_class == CLASS_TRANDOSHAN || 
				ent->client->NPC_class == CLASS_BESPIN_COP || 
				ent->client->NPC_class == CLASS_BOBAFETT || 
				ent->client->NPC_class == CLASS_ROCKETTROOPER || 
				ent->client->NPC_class == CLASS_ASSASSIN_DROID || 
				ent->client->NPC_class == CLASS_IMPWORKER || 
				ent->client->NPC_class == CLASS_WIZARD || 
				ent->client->NPC_class == CLASS_PLAYER || 
				ent->client->NPC_class == CLASS_VEHICLE || 
				ent->client->NPC_class == CLASS_DRUID || 
				ent->client->NPC_class == CLASS_REAVER || 
				ent->client->NPC_class == CLASS_RAZIEL ||
				ent->client->NPC_class == CLASS_NECROREAVER ||
				ent->client->NPC_class == CLASS_DARKREAVER || 
				ent->client->NPC_class == CLASS_TANKREAVER ||
				ent->client->NPC_class == CLASS_LIGHTREAVER ||
				ent->client->NPC_class == CLASS_DRUIDREAVER ||
			 	ent->client->NPC_class == CLASS_SPIDERREAVER || 				
				ent->client->NPC_class == CLASS_FIREREAVER || 
				ent->client->NPC_class == CLASS_AIRREAVER || 
				ent->client->NPC_class == CLASS_MENTALIST || 
				ent->client->NPC_class == CLASS_EARTHREAVER || 			  
				ent->client->NPC_class == CLASS_WATER || 			 
			  	ent->client->NPC_class == CLASS_SONIC || 
				ent->client->NPC_class == CLASS_SHARK ||  
				ent->client->NPC_class == CLASS_GOLEM_METAL ||			
				ent->client->NPC_class == CLASS_GOLEM_SONIC )
	{
		damage *= 1;
	}

	VectorSet( missile->maxs, REPEATER_ALT_SIZE, REPEATER_ALT_SIZE, REPEATER_ALT_SIZE );
	VectorScale( missile->maxs, -1, missile->mins );
	missile->s.pos.trType = TR_GRAVITY;
	missile->s.pos.trDelta[2] += 40.0f; //give a slight boost in the upward direction

//	if ( ent->client && ent->client->ps.powerups[PW_WEAPON_OVERCHARGE] > 0 && ent->client->ps.powerups[PW_WEAPON_OVERCHARGE] > cg.time )
//	{
//		// in overcharge mode, so doing double damage
//		missile->flags |= FL_OVERCHARGED;
//		damage *= 2;
//	}

	missile->damage = damage;
	missile->dflags = DAMAGE_DEATH_KNOCKBACK;
	missile->methodOfDeath = MOD_REPEATER_ALT;
	missile->splashMethodOfDeath = MOD_REPEATER_ALT;
	missile->clipmask = MASK_SHOT | CONTENTS_LIGHTSABER;
	missile->splashDamage = weaponData[WP_REPEATER].splashDamage;
	missile->splashRadius = weaponData[WP_REPEATER].splashRadius;

	// we don't want it to bounce forever
	missile->bounceCount = 8;
}

//---------------------------------------------------------
void WP_FireRepeater( gentity_t *ent, qboolean alt_fire )
//---------------------------------------------------------
{
	vec3_t	dir, angs;

	vectoangles( forwardVec, angs );

	if ( alt_fire )
	{
		WP_RepeaterAltFire( ent );
	}
	else
	{
		if ( !(ent->client->ps.forcePowersActive&(1<<FP_SEE))
			|| ent->client->ps.forcePowerLevel[FP_SEE] < FORCE_LEVEL_2 )
		{//force sight 2+ gives perfect aim
			//FIXME: maybe force sight level 3 autoaims some?
			// Troopers use their aim values as well as the gun's inherent inaccuracy
			// so check for all classes of stormtroopers and anyone else that has aim error
			if ( ent->client && ent->NPC &&
				( ent->client->NPC_class == CLASS_SWAMP ||
				  ent->client->NPC_class == CLASS_BARTENDER ||
				  ent->client->NPC_class == CLASS_WEEQUAY ||
				  ent->client->NPC_class == CLASS_GALAKMECH ||
				  ent->client->NPC_class == CLASS_SWAMPTROOPER ||
				  ent->client->NPC_class == CLASS_FISH ) )
			{
				angs[PITCH] += ( crandom() * (REPEATER_NPC_SPREAD+(6-ent->NPC->currentAim)*0.25f) );
				angs[YAW]	+= ( crandom() * (REPEATER_NPC_SPREAD+(6-ent->NPC->currentAim)*0.25f) );
			}
			else
			{
				// add some slop to the alt-fire direction
				angs[PITCH] += crandom() * REPEATER_SPREAD;
				angs[YAW]	+= crandom() * REPEATER_SPREAD;
			}
		}

		AngleVectors( angs, dir, NULL, NULL );

		// FIXME: if temp_org does not have clear trace to inside the bbox, don't shoot!
		WP_RepeaterMainFire( ent, dir );
	}

	if ( alt_fire )
{
	// FONT OF PUTRESCENSE 
	if ( ent->client->NPC_class == CLASS_REBORN || 
				ent->client->NPC_class == CLASS_KYLE || 
				ent->client->NPC_class == CLASS_TUSKEN || 
				ent->client->NPC_class == CLASS_LIZARD || 
				ent->client->NPC_class == CLASS_MONMOTHA || 
				ent->client->NPC_class == CLASS_GLIDER || 
				ent->client->NPC_class == CLASS_NOGHRI || 
				ent->client->NPC_class == CLASS_FLIER2 || 
				ent->client->NPC_class == CLASS_GRAN || 
				ent->client->NPC_class == CLASS_R2D2 || 
				ent->client->NPC_class == CLASS_R5D2 || 
				ent->client->NPC_class == CLASS_RANCOR || 
				ent->client->NPC_class == CLASS_SLUAGH || 
				ent->client->NPC_class == CLASS_MINEMONSTER || 
				ent->client->NPC_class == CLASS_POISON || 
				ent->client->NPC_class == CLASS_DEMON_POISON || 
				ent->client->NPC_class == CLASS_GOLEM_POISON || 
				ent->client->NPC_class == CLASS_MELC ||  
				ent->client->NPC_class == CLASS_SUMMONER || 
				ent->client->NPC_class == CLASS_ZEPH || 
				ent->client->NPC_class == CLASS_DECEIVER || 
				ent->client->NPC_class == CLASS_DUMAHIM || 
				ent->client->NPC_class == CLASS_TURELIM || 
				ent->client->NPC_class == CLASS_VAMPIRE || 
				ent->client->NPC_class == CLASS_KAIN || 
				ent->client->NPC_class == CLASS_KAIN2 || 
				ent->client->NPC_class == CLASS_KAIN3 || 
				ent->client->NPC_class == CLASS_UMAH || 
				ent->client->NPC_class == CLASS_VORADOR || 
				ent->client->NPC_class == CLASS_JANOS || 
				ent->client->NPC_class == CLASS_VAMPIRE_DARK || 
				ent->client->NPC_class == CLASS_VAMPIRE_NECRO || 
				ent->client->NPC_class == CLASS_VAMPIRE_FEAR || 
				ent->client->NPC_class == CLASS_VAMPIRE_BLOOD || 
				ent->client->NPC_class == CLASS_VAMPIRE_SOUND || 
				ent->client->NPC_class == CLASS_VAMPIRE_LYCAN || 
				ent->client->NPC_class == CLASS_VAMPIRE_ELF || 
				ent->client->NPC_class == CLASS_VAMPIRE_ELF2 || 
				ent->client->NPC_class == CLASS_VAMPIRE_ELF3 || 
			  	ent->client->NPC_class == CLASS_VAMPIRE_ARCHER || 
				ent->client->NPC_class == CLASS_HALFVAMPIRE || 
				ent->client->NPC_class == CLASS_VAMPIRE_BOUNTY || 
				ent->client->NPC_class == CLASS_VAMPIRE_ZOMBIE || 
				ent->client->NPC_class == CLASS_VAMPIRE_SARAFAN ||  
				ent->client->NPC_class == CLASS_VAMPIRE_GHOST || 
				ent->client->NPC_class == CLASS_BEAST )
	{
		{
			G_PlayEffect( G_EffectIndex( "putrescence/altmuzzle_flash" ), ent->client->renderInfo.handRPoint );
		}
	}
	// NECROTIC 
	else if ( ent->client->NPC_class == CLASS_VAMPIRE_FEAR2 ||
				ent->client->NPC_class == CLASS_THRALL || 
				ent->client->NPC_class == CLASS_NECRO || 
				ent->client->NPC_class == CLASS_GOLEM_NECRO ||
				ent->client->NPC_class == CLASS_NECROMANCER ||
				ent->client->NPC_class == CLASS_VAMPIRE_QUEEN ||
				ent->client->NPC_class == CLASS_VAMPIRE_NECRO2 ||
				ent->client->NPC_class == CLASS_MURJJ )
	{
		{
			G_PlayEffect( G_EffectIndex( "necrotic/altmuzzle_flash" ), ent->client->renderInfo.handRPoint );
		}
	}
	// DEMONIC PUTRESCENSE 
	else if ( ent->client->NPC_class == CLASS_IMPERIAL || 
				ent->client->NPC_class == CLASS_CENOBITA || 
				ent->client->NPC_class == CLASS_PROPHET || 
				ent->client->NPC_class == CLASS_WAMPA || 
				ent->client->NPC_class == CLASS_VAMPIRE_DEMON || 
				ent->client->NPC_class == CLASS_DEMON_FIRE || 
				ent->client->NPC_class == CLASS_DEMON_BLADE || 
				ent->client->NPC_class == CLASS_DEMON_ELECTRO || 
				ent->client->NPC_class == CLASS_HYLDEN || 
				ent->client->NPC_class == CLASS_HYLDENLORD || 
				ent->client->NPC_class == CLASS_TAVION || 
				ent->client->NPC_class == CLASS_DESANN )
	{
		{
			G_PlayEffect( G_EffectIndex( "hellputrescence/altmuzzle_flash" ), ent->client->renderInfo.handRPoint );
		}
	}
	// ICE PROJECTILES 
	else if ( ent->client->NPC_class == CLASS_RAZIELHIM || 
		ent->client->NPC_class == CLASS_BARTENDER || 
				ent->client->NPC_class == CLASS_LYCAN || 
				ent->client->NPC_class == CLASS_ICEREAVER || 
				ent->client->NPC_class == CLASS_VAMPIRE_FROST || 
				ent->client->NPC_class == CLASS_GOLEM_ICE || 
				ent->client->NPC_class == CLASS_POLTER || 
				ent->client->NPC_class == CLASS_PROBE )
	{
		{
			G_PlayEffect( G_EffectIndex( "repeater2/altmuzzle_flash" ), ent->client->renderInfo.handRPoint );
		}
	}
	// THE CRYSTAL PROJECTILES  
	else if ( ent->client->NPC_class == CLASS_ESKANDOR || 
		ent->client->NPC_class == CLASS_GALAKMECH || 
				ent->client->NPC_class == CLASS_CRYSTAL || 
				ent->client->NPC_class == CLASS_WEEQUAY || 
				ent->client->NPC_class == CLASS_GONK || 
				ent->client->NPC_class == CLASS_GOLEM || 
				ent->client->NPC_class == CLASS_EARTH )
	{
		{
			G_PlayEffect( G_EffectIndex( "repeater3/altmuzzle_flash" ), ent->client->renderInfo.handRPoint );
		}
	}
	// THE VAPOUR PROJECTILES  
	else if ( ent->client->NPC_class == CLASS_INTERROGATOR || 
				ent->client->NPC_class == CLASS_HAZARD_TROOPER || 
				ent->client->NPC_class == CLASS_SABER_DROID || 
				ent->client->NPC_class == CLASS_PROTOCOL || 
				ent->client->NPC_class == CLASS_CLAW || 
				ent->client->NPC_class == CLASS_JAWA || 
				ent->client->NPC_class == CLASS_GHOST || 
				ent->client->NPC_class == CLASS_GOLEM_LIGHT || 
				ent->client->NPC_class == CLASS_GOLEM_GOLD || 
				ent->client->NPC_class == CLASS_FIRE || 
				ent->client->NPC_class == CLASS_GOLEM_FIRE || 
				ent->client->NPC_class == CLASS_HELLGOLEM || 
				ent->client->NPC_class == CLASS_GOLEM_LAVA || 
				ent->client->NPC_class == CLASS_BIRD || 
				ent->client->NPC_class == CLASS_AIR || 
				ent->client->NPC_class == CLASS_GOLEM_AIR || 
				ent->client->NPC_class == CLASS_LIGHT || 
				ent->client->NPC_class == CLASS_GOLEM_SMOKE )
	{
		{
			G_PlayEffect( G_EffectIndex( "vapor/altmuzzle_flash" ), ent->client->renderInfo.handRPoint );
		}
	}
	// TAR TENTACLE MUD  
	else if (  ent->client->NPC_class == CLASS_SHADOWTROOPER || 
				ent->client->NPC_class == CLASS_VOID || 
				ent->client->NPC_class == CLASS_GOLEM_VOID || 
				ent->client->NPC_class == CLASS_ELDERGOD || 
				ent->client->NPC_class == CLASS_TENTACLE || 
				ent->client->NPC_class == CLASS_REAPER || 
				ent->client->NPC_class == CLASS_DREADNOUGHT || 
				ent->client->NPC_class == CLASS_SENTRY || 
				ent->client->NPC_class == CLASS_MOUSE || 
				ent->client->NPC_class == CLASS_ATST || 
				ent->client->NPC_class == CLASS_SAND_CREATURE )
	{
		{
			
			G_PlayEffect( G_EffectIndex( "eldertar/altmuzzle_flash" ), ent->client->renderInfo.handRPoint );
		}
	}
	// TAR BROWN MUD 
	else if ( ent->client->NPC_class == CLASS_SABOTEUR || 
				ent->client->NPC_class == CLASS_DARK || 
				ent->client->NPC_class == CLASS_GOLEM_DARK || 
				ent->client->NPC_class == CLASS_UGNAUGHT || 
				ent->client->NPC_class == CLASS_SPIDERREAVER2 || 
				ent->client->NPC_class == CLASS_SPIDERREAVER3 || 
				ent->client->NPC_class == CLASS_REELO || 
				ent->client->NPC_class == CLASS_ALCHEMIST || 
				ent->client->NPC_class == CLASS_IVY || 
				ent->client->NPC_class == CLASS_SWAMP )
	{
		{
			G_PlayEffect( G_EffectIndex( "tar/altmuzzle_flash" ), ent->client->renderInfo.handRPoint );
		}
	}
	// THE HOLY WATER
	else if ( ent->client->NPC_class == CLASS_RODIAN || 
				ent->client->NPC_class == CLASS_GALAK || 
				ent->client->NPC_class == CLASS_STORMTROOPER || 
				ent->client->NPC_class == CLASS_JAN || 
				ent->client->NPC_class == CLASS_MARK1 || 
				ent->client->NPC_class == CLASS_MARK2 || 
				ent->client->NPC_class == CLASS_ARIEL || 	
				ent->client->NPC_class == CLASS_SARAFAN || 
				ent->client->NPC_class == CLASS_ARCHER ||  
				ent->client->NPC_class == CLASS_WARRIOR || 
				ent->client->NPC_class == CLASS_HUNTER || 
				ent->client->NPC_class == CLASS_SCOUT ||  
				ent->client->NPC_class == CLASS_HOLY || 
				ent->client->NPC_class == CLASS_GOLEM_HOLY  )
	{
		{
			G_PlayEffect( G_EffectIndex( "holywater/altmuzzle_flash" ), ent->client->renderInfo.handRPoint );
		}
	}
	// RAHABIM IDROCINESIS   
	else if ( ent->client->NPC_class == CLASS_ALORA || 
				ent->client->NPC_class == CLASS_VAMPIRE_WATER || 
				ent->client->NPC_class == CLASS_RAHABIM || 
				ent->client->NPC_class == CLASS_SWAMPTROOPER || 
				ent->client->NPC_class == CLASS_GOLEM_WATER )
	{
		{
			G_PlayEffect( G_EffectIndex( "Hydrotk/altmuzzle_flash" ), ent->client->renderInfo.handRPoint );
		}
	}
	// RAHABIM IDROCINESIS BOSS   
	else if ( ent->client->NPC_class == CLASS_FISH )
	{
		{
			G_PlayEffect( G_EffectIndex( "hydrotk2/altmuzzle_flash" ), ent->client->renderInfo.handRPoint );
		}
	}
	// DEFAULT WATERBALLS  
	else if ( ent->client->NPC_class == CLASS_HOWLER || 
				ent->client->NPC_class == CLASS_JEDI || 
				ent->client->NPC_class == CLASS_LANDO || 
				ent->client->NPC_class == CLASS_LUKE || 
				ent->client->NPC_class == CLASS_MORGANKATARN || 
				ent->client->NPC_class == CLASS_PRISONER || 
				ent->client->NPC_class == CLASS_REBEL || 
				ent->client->NPC_class == CLASS_REMOTE || 
				ent->client->NPC_class == CLASS_SEEKER || 
				ent->client->NPC_class == CLASS_TRANDOSHAN || 
				ent->client->NPC_class == CLASS_BESPIN_COP || 
				ent->client->NPC_class == CLASS_BOBAFETT || 
				ent->client->NPC_class == CLASS_ROCKETTROOPER || 
				ent->client->NPC_class == CLASS_ASSASSIN_DROID || 
				ent->client->NPC_class == CLASS_IMPWORKER || 
				ent->client->NPC_class == CLASS_WIZARD || 
				ent->client->NPC_class == CLASS_PLAYER || 
				ent->client->NPC_class == CLASS_VEHICLE || 
				ent->client->NPC_class == CLASS_DRUID || 
				ent->client->NPC_class == CLASS_REAVER || 
				ent->client->NPC_class == CLASS_RAZIEL ||
				ent->client->NPC_class == CLASS_NECROREAVER ||
				ent->client->NPC_class == CLASS_DARKREAVER || 
				ent->client->NPC_class == CLASS_TANKREAVER ||
				ent->client->NPC_class == CLASS_LIGHTREAVER ||
				ent->client->NPC_class == CLASS_DRUIDREAVER ||
			 	ent->client->NPC_class == CLASS_SPIDERREAVER || 				
				ent->client->NPC_class == CLASS_FIREREAVER || 
				ent->client->NPC_class == CLASS_AIRREAVER || 
				ent->client->NPC_class == CLASS_MENTALIST || 
				ent->client->NPC_class == CLASS_EARTHREAVER || 			  
				ent->client->NPC_class == CLASS_WATER || 			 
			  	ent->client->NPC_class == CLASS_SONIC || 
				ent->client->NPC_class == CLASS_SHARK ||  
				ent->client->NPC_class == CLASS_GOLEM_METAL ||			
				ent->client->NPC_class == CLASS_GOLEM_SONIC )
	{
		{
			G_PlayEffect( G_EffectIndex( "repeater/altmuzzle_flash" ), ent->client->renderInfo.handRPoint );
		}
	}
	else if ( ent->client->NPC_class == CLASS_FIGHTER )
	{
		{
			G_PlayEffect( G_EffectIndex( "repeater0/altmuzzle_flash" ), ent->client->renderInfo.handRPoint );
		}
	}
}
else 
{
	if ( ent->client->NPC_class == CLASS_REBORN || 
				ent->client->NPC_class == CLASS_KYLE || 
				ent->client->NPC_class == CLASS_TUSKEN || 
				ent->client->NPC_class == CLASS_LIZARD || 
				ent->client->NPC_class == CLASS_MONMOTHA || 
				ent->client->NPC_class == CLASS_GLIDER || 
				ent->client->NPC_class == CLASS_NOGHRI || 
				ent->client->NPC_class == CLASS_FLIER2 || 
				ent->client->NPC_class == CLASS_GRAN || 
				ent->client->NPC_class == CLASS_R2D2 || 
				ent->client->NPC_class == CLASS_R5D2 || 
				ent->client->NPC_class == CLASS_RANCOR || 
				ent->client->NPC_class == CLASS_SLUAGH || 
				ent->client->NPC_class == CLASS_MINEMONSTER || 
				ent->client->NPC_class == CLASS_POISON || 
				ent->client->NPC_class == CLASS_DEMON_POISON || 
				ent->client->NPC_class == CLASS_GOLEM_POISON || 
				ent->client->NPC_class == CLASS_MELC ||  
				ent->client->NPC_class == CLASS_SUMMONER || 
				ent->client->NPC_class == CLASS_ZEPH || 
				ent->client->NPC_class == CLASS_DECEIVER || 
				ent->client->NPC_class == CLASS_DUMAHIM || 
				ent->client->NPC_class == CLASS_TURELIM || 
				ent->client->NPC_class == CLASS_VAMPIRE || 
				ent->client->NPC_class == CLASS_KAIN || 
				ent->client->NPC_class == CLASS_KAIN2 || 
				ent->client->NPC_class == CLASS_KAIN3 || 
				ent->client->NPC_class == CLASS_UMAH || 
				ent->client->NPC_class == CLASS_VORADOR || 
				ent->client->NPC_class == CLASS_JANOS || 
				ent->client->NPC_class == CLASS_VAMPIRE_DARK || 
				ent->client->NPC_class == CLASS_VAMPIRE_NECRO || 
				ent->client->NPC_class == CLASS_VAMPIRE_FEAR || 
				ent->client->NPC_class == CLASS_VAMPIRE_BLOOD || 
				ent->client->NPC_class == CLASS_VAMPIRE_SOUND || 
				ent->client->NPC_class == CLASS_VAMPIRE_LYCAN || 
				ent->client->NPC_class == CLASS_VAMPIRE_ELF || 
				ent->client->NPC_class == CLASS_VAMPIRE_ELF2 || 
				ent->client->NPC_class == CLASS_VAMPIRE_ELF3 || 
			  	ent->client->NPC_class == CLASS_VAMPIRE_ARCHER || 
				ent->client->NPC_class == CLASS_HALFVAMPIRE || 
				ent->client->NPC_class == CLASS_VAMPIRE_BOUNTY || 
				ent->client->NPC_class == CLASS_VAMPIRE_ZOMBIE || 
				ent->client->NPC_class == CLASS_VAMPIRE_SARAFAN ||  
				ent->client->NPC_class == CLASS_VAMPIRE_GHOST || 
				ent->client->NPC_class == CLASS_BEAST )
	{
		{
			G_PlayEffect( G_EffectIndex( "putrescence/muzzle_flash" ), ent->client->renderInfo.handRPoint );
		}
	}
	else if ( ent->client->NPC_class == CLASS_VAMPIRE_FEAR2 ||
				ent->client->NPC_class == CLASS_THRALL || 
				ent->client->NPC_class == CLASS_NECRO || 
				ent->client->NPC_class == CLASS_GOLEM_NECRO ||
				ent->client->NPC_class == CLASS_NECROMANCER ||
				ent->client->NPC_class == CLASS_VAMPIRE_QUEEN ||
				ent->client->NPC_class == CLASS_VAMPIRE_NECRO2 ||
				ent->client->NPC_class == CLASS_MURJJ )
	{
		{
			G_PlayEffect( G_EffectIndex( "necrotic/muzzle_flash" ), ent->client->renderInfo.handRPoint );
		}
	}
	// DEMONIC PUTRESCENSE 
	else if ( ent->client->NPC_class == CLASS_IMPERIAL || 
				ent->client->NPC_class == CLASS_CENOBITA || 
				ent->client->NPC_class == CLASS_PROPHET || 
				ent->client->NPC_class == CLASS_WAMPA || 
				ent->client->NPC_class == CLASS_VAMPIRE_DEMON || 
				ent->client->NPC_class == CLASS_DEMON_FIRE || 
				ent->client->NPC_class == CLASS_DEMON_BLADE || 
				ent->client->NPC_class == CLASS_DEMON_ELECTRO || 
				ent->client->NPC_class == CLASS_HYLDEN || 
				ent->client->NPC_class == CLASS_HYLDENLORD || 
				ent->client->NPC_class == CLASS_TAVION || 
				ent->client->NPC_class == CLASS_DESANN )
	{
		{
			G_PlayEffect( G_EffectIndex( "hellputrescence/muzzle_flash" ), ent->client->renderInfo.handRPoint );
		}
	}
	// ICE PROJECTILES 
	else if ( ent->client->NPC_class == CLASS_RAZIELHIM || 
		ent->client->NPC_class == CLASS_BARTENDER || 
				ent->client->NPC_class == CLASS_LYCAN || 
				ent->client->NPC_class == CLASS_ICEREAVER || 
				ent->client->NPC_class == CLASS_VAMPIRE_FROST || 
				ent->client->NPC_class == CLASS_GOLEM_ICE || 
				ent->client->NPC_class == CLASS_POLTER || 
				ent->client->NPC_class == CLASS_PROBE )
	{
		{
			G_PlayEffect( G_EffectIndex( "repeater2/muzzle_flash" ), ent->client->renderInfo.handRPoint );
		}
	}
	// THE CRYSTAL PROJECTILES  
	else if ( ent->client->NPC_class == CLASS_ESKANDOR || 
				ent->client->NPC_class == CLASS_GALAKMECH || 
				ent->client->NPC_class == CLASS_CRYSTAL || 
				ent->client->NPC_class == CLASS_WEEQUAY || 
				ent->client->NPC_class == CLASS_GONK || 
				ent->client->NPC_class == CLASS_GOLEM || 
				ent->client->NPC_class == CLASS_EARTH )
	{
		{
			G_PlayEffect( G_EffectIndex( "repeater3/muzzle_flash" ), ent->client->renderInfo.handRPoint );
		}
	}
	// THE VAPOUR PROJECTILES  
	else if ( ent->client->NPC_class == CLASS_INTERROGATOR || 
				ent->client->NPC_class == CLASS_HAZARD_TROOPER || 
				ent->client->NPC_class == CLASS_SABER_DROID || 
				ent->client->NPC_class == CLASS_PROTOCOL || 
				ent->client->NPC_class == CLASS_CLAW || 
				ent->client->NPC_class == CLASS_JAWA || 
				ent->client->NPC_class == CLASS_GHOST || 
				ent->client->NPC_class == CLASS_GOLEM_LIGHT || 
				ent->client->NPC_class == CLASS_GOLEM_GOLD || 
				ent->client->NPC_class == CLASS_FIRE || 
				ent->client->NPC_class == CLASS_GOLEM_FIRE || 
				ent->client->NPC_class == CLASS_HELLGOLEM || 
				ent->client->NPC_class == CLASS_GOLEM_LAVA || 
				ent->client->NPC_class == CLASS_BIRD || 
				ent->client->NPC_class == CLASS_AIR || 
				ent->client->NPC_class == CLASS_GOLEM_AIR || 
				ent->client->NPC_class == CLASS_LIGHT || 
				ent->client->NPC_class == CLASS_GOLEM_SMOKE )
	{
		{
			G_PlayEffect( G_EffectIndex( "vapor/muzzle_flash" ), ent->client->renderInfo.handRPoint );
		}
	}
	// TAR TENTACLE MUD  
	else if (  ent->client->NPC_class == CLASS_SHADOWTROOPER || 
				ent->client->NPC_class == CLASS_VOID || 
				ent->client->NPC_class == CLASS_GOLEM_VOID || 
				ent->client->NPC_class == CLASS_ELDERGOD || 
				ent->client->NPC_class == CLASS_TENTACLE || 
				ent->client->NPC_class == CLASS_REAPER || 
				ent->client->NPC_class == CLASS_DREADNOUGHT || 
				ent->client->NPC_class == CLASS_SENTRY || 
				ent->client->NPC_class == CLASS_MOUSE || 
				ent->client->NPC_class == CLASS_ATST || 
				ent->client->NPC_class == CLASS_SAND_CREATURE )
	{
		{
			
			G_PlayEffect( G_EffectIndex( "eldertar/muzzle_flash" ), ent->client->renderInfo.handRPoint );
		}
	}
	// TAR BROWN MUD 
	else if ( ent->client->NPC_class == CLASS_SABOTEUR || 
				ent->client->NPC_class == CLASS_DARK || 
				ent->client->NPC_class == CLASS_GOLEM_DARK || 
				ent->client->NPC_class == CLASS_UGNAUGHT || 
				ent->client->NPC_class == CLASS_SPIDERREAVER2 || 
				ent->client->NPC_class == CLASS_SPIDERREAVER3 || 
				ent->client->NPC_class == CLASS_REELO || 
				ent->client->NPC_class == CLASS_ALCHEMIST || 
				ent->client->NPC_class == CLASS_IVY || 
				ent->client->NPC_class == CLASS_SWAMP )
	{
		{
			G_PlayEffect( G_EffectIndex( "tar/muzzle_flash" ), ent->client->renderInfo.handRPoint );
		}
	}
	// THE HOLY WATER
	else if ( ent->client->NPC_class == CLASS_RODIAN || 
				ent->client->NPC_class == CLASS_GALAK || 
				ent->client->NPC_class == CLASS_STORMTROOPER || 
				ent->client->NPC_class == CLASS_JAN || 
				ent->client->NPC_class == CLASS_MARK1 || 
				ent->client->NPC_class == CLASS_MARK2 || 
				ent->client->NPC_class == CLASS_ARIEL || 	
				ent->client->NPC_class == CLASS_SARAFAN || 
				ent->client->NPC_class == CLASS_ARCHER ||  
				ent->client->NPC_class == CLASS_WARRIOR || 
				ent->client->NPC_class == CLASS_HUNTER || 
				ent->client->NPC_class == CLASS_SCOUT ||  
				ent->client->NPC_class == CLASS_HOLY || 
				ent->client->NPC_class == CLASS_GOLEM_HOLY  )
	{
		{
			G_PlayEffect( G_EffectIndex( "holywater/muzzle_flash" ), ent->client->renderInfo.handRPoint );
		}
	}
	// RAHABIM IDROCINESIS   
	else if ( ent->client->NPC_class == CLASS_ALORA || 
				ent->client->NPC_class == CLASS_VAMPIRE_WATER || 
				ent->client->NPC_class == CLASS_RAHABIM || 
				ent->client->NPC_class == CLASS_SWAMPTROOPER || 
				ent->client->NPC_class == CLASS_GOLEM_WATER )
	{
		{
			G_PlayEffect( G_EffectIndex( "Hydrotk/muzzle_flash" ), ent->client->renderInfo.handRPoint );
		}
	}
	// RAHABIM IDROCINESIS BOSS   
	else if ( ent->client->NPC_class == CLASS_FISH )
	{
		{
			G_PlayEffect( G_EffectIndex( "hydrotk2/muzzle_flash" ), ent->client->renderInfo.handRPoint );
		}
	}
	// DEFAULT WATERBALLS  
	else if ( ent->client->NPC_class == CLASS_HOWLER || 
				ent->client->NPC_class == CLASS_JEDI || 
				ent->client->NPC_class == CLASS_LANDO || 
				ent->client->NPC_class == CLASS_LUKE || 
				ent->client->NPC_class == CLASS_MORGANKATARN || 
				ent->client->NPC_class == CLASS_PRISONER || 
				ent->client->NPC_class == CLASS_REBEL || 
				ent->client->NPC_class == CLASS_REMOTE || 
				ent->client->NPC_class == CLASS_SEEKER || 
				ent->client->NPC_class == CLASS_TRANDOSHAN || 
				ent->client->NPC_class == CLASS_BESPIN_COP || 
				ent->client->NPC_class == CLASS_BOBAFETT || 
				ent->client->NPC_class == CLASS_ROCKETTROOPER || 
				ent->client->NPC_class == CLASS_ASSASSIN_DROID || 
				ent->client->NPC_class == CLASS_IMPWORKER || 
				ent->client->NPC_class == CLASS_WIZARD || 
				ent->client->NPC_class == CLASS_PLAYER || 
				ent->client->NPC_class == CLASS_VEHICLE || 
				ent->client->NPC_class == CLASS_DRUID || 
				ent->client->NPC_class == CLASS_REAVER || 
				ent->client->NPC_class == CLASS_RAZIEL ||
				ent->client->NPC_class == CLASS_NECROREAVER ||
				ent->client->NPC_class == CLASS_DARKREAVER || 
				ent->client->NPC_class == CLASS_TANKREAVER ||
				ent->client->NPC_class == CLASS_LIGHTREAVER ||
				ent->client->NPC_class == CLASS_DRUIDREAVER ||
			 	ent->client->NPC_class == CLASS_SPIDERREAVER || 				
				ent->client->NPC_class == CLASS_FIREREAVER || 
				ent->client->NPC_class == CLASS_AIRREAVER || 
				ent->client->NPC_class == CLASS_MENTALIST || 
				ent->client->NPC_class == CLASS_EARTHREAVER || 			  
				ent->client->NPC_class == CLASS_WATER || 			 
			  	ent->client->NPC_class == CLASS_SONIC || 
				ent->client->NPC_class == CLASS_SHARK ||  
				ent->client->NPC_class == CLASS_GOLEM_METAL ||			
				ent->client->NPC_class == CLASS_GOLEM_SONIC )
	{
		{
			G_PlayEffect( G_EffectIndex( "repeater/muzzle_flash" ), ent->client->renderInfo.handRPoint );
		}
	}
	else if ( ent->client->NPC_class == CLASS_FIGHTER )
	{
		{
			G_PlayEffect( G_EffectIndex( "repeater0/muzzle_flash" ), ent->client->renderInfo.handRPoint );
		}
	}
}

}