
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
//	DEMP2
//-------------------

//---------------------------------------------------------
static void WP_DEMP2_MainFire( gentity_t *ent )
//---------------------------------------------------------
{
	vec3_t	start;
	int		damage	= weaponData[WP_DEMP2].damage;
	// DMG SACRO
	if ( ent && ent->client && ent->client->NPC_class == CLASS_TRANDOSHAN )
	{
		damage *= 1.5;
	}
	// DMG VITA
	else if ( ent && ent->client && ent->client->NPC_class == CLASS_LUKE 	)
	{
		damage *= 2; // Elementali del Calore fanno 50% di danno in più!
	}

	// HOLY 
	if ( ent->client->NPC_class == CLASS_HOLY || 
			ent->client->NPC_class == CLASS_GOLEM_HOLY || 
			ent->client->NPC_class == CLASS_VAMPIRE_GHOST )
	{
		damage *= 1.5;
	}
	// LIFE 
	else if ( ent->client->NPC_class == CLASS_LUKE || 
				ent->client->NPC_class == CLASS_IVY || 
				ent->client->NPC_class == CLASS_BEAST || 
				ent->client->NPC_class == CLASS_BIRD || 
				ent->client->NPC_class == CLASS_SHARK )
	{
		damage *= 2;
	}
	// SPIRITUAL 
	else if ( ent->client->NPC_class == CLASS_RAZIEL || 
			ent->client->NPC_class == CLASS_IMPWORKER )
	{
		damage *= 1;
	}
	// DRUIDIC FIRE 
	else if ( ent->client->NPC_class == CLASS_TRANDOSHAN ||
				ent->client->NPC_class == CLASS_LANDO || 
				ent->client->NPC_class == CLASS_DRUIDREAVER || 
				ent->client->NPC_class == CLASS_DRUID || 
				ent->client->NPC_class == CLASS_LYCAN )
	{
		damage *= 0.5;
	}
	// WHITE FIRE
	else if ( ent->client->NPC_class == CLASS_JAN ||
				ent->client->NPC_class == CLASS_SARAFAN || 
				ent->client->NPC_class == CLASS_STORMTROOPER || 
				ent->client->NPC_class == CLASS_MARK1 ||
				ent->client->NPC_class == CLASS_MARK2 || 
				ent->client->NPC_class == CLASS_WIZARD )
	{
		damage *= 0.75;
	}
	// REVERB - ONLY FOR SPECTRAL CREATURE AND UNDEAD. ARCHIMOND CLASS
	else if ( ent->client->NPC_class == CLASS_GALAKMECH ||
				ent->client->NPC_class == CLASS_GONK ||
				ent->client->NPC_class == CLASS_NECROREAVER ||
				ent->client->NPC_class == CLASS_JAWA ||
				ent->client->NPC_class == CLASS_PROBE ||
				ent->client->NPC_class == CLASS_COMMANDO ||
				ent->client->NPC_class == CLASS_SABER_DROID ||
				ent->client->NPC_class == CLASS_PROTOCOL ||
				ent->client->NPC_class == CLASS_HOWLER || 
				ent->client->NPC_class == CLASS_THRALL || 
				ent->client->NPC_class == CLASS_POLTER ||
				ent->client->NPC_class == CLASS_DARK || 
				ent->client->NPC_class == CLASS_VOID ||  
				ent->client->NPC_class == CLASS_LIGHT || 
			  	ent->client->NPC_class == CLASS_CRYSTAL || 
				ent->client->NPC_class == CLASS_AIR ||  
				ent->client->NPC_class == CLASS_EARTH || 
				ent->client->NPC_class == CLASS_NECRO || 
			  	ent->client->NPC_class == CLASS_SONIC || 
			 	ent->client->NPC_class == CLASS_GOLEM_DARK || 
				ent->client->NPC_class == CLASS_GOLEM_LIGHT ||
				ent->client->NPC_class == CLASS_GOLEM_GOLD || 
				ent->client->NPC_class == CLASS_GOLEM_AIR ||
				ent->client->NPC_class == CLASS_GOLEM_METAL ||
				ent->client->NPC_class == CLASS_GOLEM ||
				ent->client->NPC_class == CLASS_GOLEM_SONIC ||
			 	ent->client->NPC_class == CLASS_GOLEM_NECRO ||
				ent->client->NPC_class == CLASS_GOLEM_SMOKE || 
				ent->client->NPC_class == CLASS_GOLEM_VOID ||
				ent->client->NPC_class == CLASS_GHOST || 
				ent->client->NPC_class == CLASS_PLAYER || 
				ent->client->NPC_class == CLASS_VEHICLE )
	{
		damage *= 0.25;
	}
	// BALANCE SHOOT - ONLY FOR KAIN WITH BALANCE REAVER 
	else if ( ent->client->NPC_class == CLASS_KYLE || 
				ent->client->NPC_class == CLASS_KAIN3 || 
				ent->client->NPC_class == CLASS_ARIEL )
	{
		damage *= 3;
	}
	// WRACK - VAMPIRES - NOTE ALSO WEEQUAY BECAUSE ANTEO NOT USE TELEKINESIS 
	else if ( ent->client->NPC_class == CLASS_REBORN ||
				ent->client->NPC_class == CLASS_ALORA ||
				ent->client->NPC_class == CLASS_MONMOTHA ||
				ent->client->NPC_class == CLASS_VAMPIRE ||
				ent->client->NPC_class == CLASS_VAMPIRE_DEMON ||
				ent->client->NPC_class == CLASS_KAIN ||
				ent->client->NPC_class == CLASS_KAIN2 || 
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
				ent->client->NPC_class == CLASS_VAMPIRE_BOUNTY || 
				ent->client->NPC_class == CLASS_VAMPIRE_ZOMBIE || 
				ent->client->NPC_class == CLASS_VAMPIRE_SARAFAN )
	{
		damage *= 0.5;
	}
	// HELL WRACK - DEMONS AND HYLDENS 
	else if ( ent->client->NPC_class == CLASS_PROPHET || 
			ent->client->NPC_class == CLASS_CENOBITA || 
			ent->client->NPC_class == CLASS_WAMPA ||
			ent->client->NPC_class == CLASS_TAVION ||
			ent->client->NPC_class == CLASS_IMPERIAL ||
			ent->client->NPC_class == CLASS_DESANN ||
			ent->client->NPC_class == CLASS_ATST ||
			ent->client->NPC_class == CLASS_MOUSE ||
			ent->client->NPC_class == CLASS_SAND_CREATURE ||
			ent->client->NPC_class == CLASS_ELDERGOD || 
			ent->client->NPC_class == CLASS_TENTACLE || 
			ent->client->NPC_class == CLASS_MURJJ || 	
			ent->client->NPC_class == CLASS_HYLDEN || 
			ent->client->NPC_class == CLASS_HYLDENLORD || 
			ent->client->NPC_class == CLASS_DEMON_FIRE ||
			ent->client->NPC_class == CLASS_DEMON_POISON ||
			ent->client->NPC_class == CLASS_DEMON_BLADE || 
			ent->client->NPC_class == CLASS_DEMON_ELECTRO || 
			ent->client->NPC_class == CLASS_POISON || 
			ent->client->NPC_class == CLASS_GOLEM_POISON || 
			ent->client->NPC_class == CLASS_FIRE || 
			ent->client->NPC_class == CLASS_GOLEM_FIRE || 
			ent->client->NPC_class == CLASS_GOLEM_LAVA || 
			ent->client->NPC_class == CLASS_GRAN || 
			ent->client->NPC_class == CLASS_TURELIM || 
			ent->client->NPC_class == CLASS_CLAW || 
			ent->client->NPC_class == CLASS_TUSKEN ||
			ent->client->NPC_class == CLASS_GLIDER ||
			ent->client->NPC_class == CLASS_NOGHRI ||
			ent->client->NPC_class == CLASS_FLIER2 ||
			ent->client->NPC_class == CLASS_FISH ||
			ent->client->NPC_class == CLASS_SWAMPTROOPER ||			 
			ent->client->NPC_class == CLASS_MELC ||  
			ent->client->NPC_class == CLASS_SUMMONER ||
			ent->client->NPC_class == CLASS_ZEPH ||
			ent->client->NPC_class == CLASS_DECEIVER ||
			ent->client->NPC_class == CLASS_DUMAHIM || 
			ent->client->NPC_class == CLASS_RAHABIM ||
			ent->client->NPC_class == CLASS_RAZIELHIM ||  
			ent->client->NPC_class == CLASS_ESKANDOR ||  
			ent->client->NPC_class == CLASS_HELLGOLEM )
	{
		damage *= 2;
	}
	// SPECTRAL - FOR ALL REAPERS 
	else if ( ent->client->NPC_class == CLASS_REELO ||
			ent->client->NPC_class == CLASS_RANCOR ||
			ent->client->NPC_class == CLASS_SENTRY ||
			ent->client->NPC_class == CLASS_MINEMONSTER ||
			ent->client->NPC_class == CLASS_SABOTEUR ||
			ent->client->NPC_class == CLASS_SHADOWTROOPER ||
			ent->client->NPC_class == CLASS_ASSASSIN_DROID ||
			ent->client->NPC_class == CLASS_MORGANKATARN ||
			ent->client->NPC_class == CLASS_HAZARD_TROOPER ||
			ent->client->NPC_class == CLASS_REBEL || 
			ent->client->NPC_class == CLASS_ROCKETTROOPER ||
			ent->client->NPC_class == CLASS_BOBAFETT ||
			ent->client->NPC_class == CLASS_BARTENDER ||
			ent->client->NPC_class == CLASS_UGNAUGHT ||
			ent->client->NPC_class == CLASS_JEDI ||
			ent->client->NPC_class == CLASS_REAVER || 
			ent->client->NPC_class == CLASS_LIZARD || 
			ent->client->NPC_class == CLASS_WEEQUAY || 
			ent->client->NPC_class == CLASS_DARKREAVER || 
			ent->client->NPC_class == CLASS_TANKREAVER ||
			ent->client->NPC_class == CLASS_LIGHTREAVER ||
			ent->client->NPC_class == CLASS_ICEREAVER || 
			ent->client->NPC_class == CLASS_SPIDERREAVER ||
			ent->client->NPC_class == CLASS_SPIDERREAVER2 ||
			ent->client->NPC_class == CLASS_SPIDERREAVER3 ||
			ent->client->NPC_class == CLASS_FIREREAVER || 
			ent->client->NPC_class == CLASS_AIRREAVER || 
			ent->client->NPC_class == CLASS_MENTALIST || 
			ent->client->NPC_class == CLASS_EARTHREAVER || 
			ent->client->NPC_class == CLASS_SLUAGH || 
			ent->client->NPC_class == CLASS_REAPER || 
			ent->client->NPC_class == CLASS_DREADNOUGHT )
	{
		damage *= 0.25;
	}
	// BENEDICTUM 
	else if ( ent->client->NPC_class == CLASS_HUNTER || 
			ent->client->NPC_class == CLASS_SCOUT ||  
			ent->client->NPC_class == CLASS_ALCHEMIST || 
			ent->client->NPC_class == CLASS_ARCHER ||  
			ent->client->NPC_class == CLASS_WARRIOR || 
			ent->client->NPC_class == CLASS_RODIAN ||
			ent->client->NPC_class == CLASS_PRISONER ||
			ent->client->NPC_class == CLASS_GALAK ||
			ent->client->NPC_class == CLASS_INTERROGATOR ||
			ent->client->NPC_class == CLASS_SWAMP ||
			ent->client->NPC_class == CLASS_WATER || 
			ent->client->NPC_class == CLASS_GOLEM_WATER || 
			ent->client->NPC_class == CLASS_GOLEM_ICE || 
			ent->client->NPC_class == CLASS_R2D2 ||
			ent->client->NPC_class == CLASS_R5D2 ||
			ent->client->NPC_class == CLASS_SEEKER ||
			ent->client->NPC_class == CLASS_REMOTE )
	{
		damage *= 0.75;
	}


	VectorCopy( muzzle, start );
	WP_TraceSetStart( ent, start, vec3_origin, vec3_origin );//make sure our start point isn't on the other side of a wall

	WP_MissileTargetHint(ent, start, forwardVec);

	gentity_t *missile = CreateMissile( start, forwardVec, DEMP2_VELOCITY, 10000, ent );

	missile->classname = "demp2_proj";
	missile->s.weapon = WP_DEMP2;

	// Do the damages
	if ( ent->s.number != 0 )
	{
		if ( g_spskill->integer == 0 )
		{
			damage = DEMP2_NPC_DAMAGE_EASY;
		}
		else if ( g_spskill->integer == 1 )
		{
			damage = DEMP2_NPC_DAMAGE_NORMAL;
		}
		else
		{
			damage = DEMP2_NPC_DAMAGE_HARD;
		}
	}

	// DMG SACRO
	if ( ent->client->NPC_class ==  CLASS_TRANDOSHAN )
	{
		damage *= 1.5;
	}
	// DMG VITA
	else if ( ent->client->NPC_class == CLASS_LUKE 	)
	{
		damage *= 2; // Elementali del Calore fanno 50% di danno in più!
	}

	VectorSet( missile->maxs, DEMP2_SIZE, DEMP2_SIZE, DEMP2_SIZE );
	VectorScale( missile->maxs, -1, missile->mins );

//	if ( ent->client && ent->client->ps.powerups[PW_WEAPON_OVERCHARGE] > 0 && ent->client->ps.powerups[PW_WEAPON_OVERCHARGE] > cg.time )
//	{
//		// in overcharge mode, so doing double damage
//		missile->flags |= FL_OVERCHARGED;
//		damage *= 2;
//	}

	missile->damage = damage;
	missile->dflags = DAMAGE_DEATH_KNOCKBACK;
	missile->methodOfDeath = MOD_DEMP2;
	missile->clipmask = MASK_SHOT | CONTENTS_LIGHTSABER;

	// we don't want it to ever bounce
	missile->bounceCount = 0;
}

// NOTE: this is 100% for the demp2 alt-fire effect, so changes to the visual effect will affect game side demp2 code
//--------------------------------------------------
void DEMP2_AltRadiusDamage( gentity_t *ent )
{
	float		frac = ( level.time - ent->fx_time ) / 1300.0f; // synchronize with demp2 effect
	float		dist, radius;
	gentity_t	*gent;
	gentity_t	*entityList[MAX_GENTITIES];
	int			numListedEntities, i, e;
	vec3_t		mins, maxs;
	vec3_t		v, dir;

	frac *= frac * frac; // yes, this is completely ridiculous...but it causes the shell to grow slowly then "explode" at the end
	
	radius = frac * 200.0f; // 200 is max radius...the model is aprox. 100 units tall...the fx draw code mults. this by 2.

	for ( i = 0 ; i < 3 ; i++ ) 
	{
		mins[i] = ent->currentOrigin[i] - radius;
		maxs[i] = ent->currentOrigin[i] + radius;
	}

	numListedEntities = gi.EntitiesInBox( mins, maxs, entityList, MAX_GENTITIES );

	for ( e = 0 ; e < numListedEntities ; e++ ) 
	{
		gent = entityList[ e ];

		if ( !gent->takedamage || !gent->contents )
		{
			continue;
		}

		// find the distance from the edge of the bounding box
		for ( i = 0 ; i < 3 ; i++ ) 
		{
			if ( ent->currentOrigin[i] < gent->absmin[i] ) 
			{
				v[i] = gent->absmin[i] - ent->currentOrigin[i];
			} 
			else if ( ent->currentOrigin[i] > gent->absmax[i] ) 
			{
				v[i] = ent->currentOrigin[i] - gent->absmax[i];
			} 
			else 
			{
				v[i] = 0;
			}
		}

		// shape is an ellipsoid, so cut vertical distance in half`
		v[2] *= 0.5f;

		dist = VectorLength( v );

		if ( dist >= radius ) 
		{
			// shockwave hasn't hit them yet
			continue;
		}

		if ( dist < ent->radius )
		{
			// shockwave has already hit this thing...
			continue;
		}

		VectorCopy( gent->currentOrigin, v );
		VectorSubtract( v, ent->currentOrigin, dir);

		// push the center of mass higher than the origin so players get knocked into the air more
		dir[2] += 12;

		G_Damage( gent, ent, ent->owner, dir, ent->currentOrigin, weaponData[WP_DEMP2].altDamage, DAMAGE_DEATH_KNOCKBACK, ent->splashMethodOfDeath );
		if ( gent->takedamage && gent->client ) 
		{
			/*gent->s.powerups |= ( 1 << PW_SHOCKED );
			gent->client->ps.powerups[PW_SHOCKED] = level.time + 2000;*/
			Saboteur_Decloak( gent, Q_irand( 3000, 10000 ) );
		}
	}

	// store the last fraction so that next time around we can test against those things that fall between that last point and where the current shockwave edge is
	ent->radius = radius;

	if ( frac < 1.0f )
	{
		// shock is still happening so continue letting it expand
		ent->nextthink = level.time + 50;
	}
}


//---------------------------------------------------------
void DEMP2_AltDetonate( gentity_t *ent )
//---------------------------------------------------------
{
	G_SetOrigin( ent, ent->currentOrigin );

	// start the effects, unfortunately, I wanted to do some custom things that I couldn't easily do with the fx system, so part of it uses an event and localEntities
	if ( ent->owner->client )
			{
			switch ( ent->owner->client->NPC_class )
			{
	// BENEDICTUM
			case CLASS_HUNTER: 
			case CLASS_SCOUT:  
			case CLASS_ALCHEMIST: 
			case CLASS_ARCHER:  
			case CLASS_WARRIOR: 
			case CLASS_RODIAN:
			case CLASS_PRISONER:
			case CLASS_GALAK:
			case CLASS_INTERROGATOR:
			case CLASS_SWAMP:
			case CLASS_WATER: 
			case CLASS_GOLEM_WATER: 
			case CLASS_GOLEM_ICE: 
			case CLASS_R2D2:
			case CLASS_R5D2:
			case CLASS_SEEKER:
			case CLASS_REMOTE: 
			G_PlayEffect( "demp/altDetonate", ent->currentOrigin, ent->pos1 );
			break;
			// SPECTRAL
			case CLASS_REELO:
			case CLASS_RANCOR:
			case CLASS_SENTRY:
			case CLASS_MINEMONSTER:
			case CLASS_SABOTEUR:
			case CLASS_SHADOWTROOPER:
			case CLASS_ASSASSIN_DROID:
			case CLASS_MORGANKATARN:
			case CLASS_HAZARD_TROOPER:
			case CLASS_REBEL: 
			case CLASS_ROCKETTROOPER:
			case CLASS_BOBAFETT:
			case CLASS_BARTENDER:
			case CLASS_UGNAUGHT:
			case CLASS_JEDI:
			case CLASS_REAVER: 
			case CLASS_LIZARD: 
			case CLASS_WEEQUAY: 
			case CLASS_DARKREAVER: 
			case CLASS_TANKREAVER:
			case CLASS_LIGHTREAVER:
			case CLASS_ICEREAVER: 
			case CLASS_SPIDERREAVER:
			case CLASS_SPIDERREAVER2:
			case CLASS_SPIDERREAVER3:
			case CLASS_FIREREAVER: 
			case CLASS_AIRREAVER: 
			case CLASS_MENTALIST: 
			case CLASS_EARTHREAVER: 
			case CLASS_SLUAGH: 
			case CLASS_REAPER: 
			case CLASS_DREADNOUGHT: 
			G_PlayEffect( "demp0/altDetonate", ent->currentOrigin, ent->pos1 );
			break;
				// HELL WRACK
			case CLASS_PROPHET: 
			case CLASS_CENOBITA: 
			case CLASS_WAMPA:
			case CLASS_TAVION:
			case CLASS_IMPERIAL:
			case CLASS_DESANN:
			case CLASS_ATST:
			case CLASS_MOUSE:
			case CLASS_SAND_CREATURE:
			case CLASS_ELDERGOD: 
			case CLASS_TENTACLE: 
			case CLASS_MURJJ: 	
			case CLASS_HYLDEN: 
			case CLASS_HYLDENLORD: 
			case CLASS_DEMON_FIRE:
			case CLASS_DEMON_POISON:
			case CLASS_DEMON_BLADE: 
			case CLASS_DEMON_ELECTRO: 
			case CLASS_POISON: 
			case CLASS_GOLEM_POISON: 
			case CLASS_FIRE: 
			case CLASS_GOLEM_FIRE: 
			case CLASS_GOLEM_LAVA: 
			case CLASS_GRAN: 
			case CLASS_TURELIM: 
			case CLASS_CLAW: 
			case CLASS_TUSKEN:
			case CLASS_GLIDER:
			case CLASS_NOGHRI:
			case CLASS_FLIER2:
			case CLASS_FISH:
			case CLASS_SWAMPTROOPER:			 
			case CLASS_MELC:  
			case CLASS_SUMMONER:
			case CLASS_ZEPH:
			case CLASS_DECEIVER:
			case CLASS_DUMAHIM: 
			case CLASS_RAHABIM:
			case CLASS_HELLGOLEM: 
			case CLASS_RAZIELHIM: 
			case CLASS_ESKANDOR: 
			G_PlayEffect( "demp1/altDetonate", ent->currentOrigin, ent->pos1 );
			break;
			// SPIRIT
			case CLASS_RAZIEL: 
			case CLASS_IMPWORKER: 
			G_PlayEffect( "demp2/altDetonate", ent->currentOrigin, ent->pos1 );
			break;
				// HOLY
			case CLASS_HOLY: 
			case CLASS_GOLEM_HOLY: 
			case CLASS_VAMPIRE_GHOST: 
			G_PlayEffect( "demp3/altDetonate", ent->currentOrigin, ent->pos1 );
			break;
				// LIFE 
				case CLASS_LUKE: 
				case CLASS_IVY: 
				case CLASS_BEAST: 
				case CLASS_BIRD: 
				case CLASS_SHARK: 
			G_PlayEffect( "demp4/altDetonate", ent->currentOrigin, ent->pos1 );
			break;
				// DRUIDIC FIRE
				case CLASS_TRANDOSHAN:
				case CLASS_LANDO: 
				case CLASS_DRUIDREAVER: 
				case CLASS_DRUID: 
				case CLASS_LYCAN: 
			G_PlayEffect( "demp5/altDetonate", ent->currentOrigin, ent->pos1 );
			break;
				// HOLY FIRE
				case CLASS_JAN:
				case CLASS_SARAFAN: 
				case CLASS_STORMTROOPER: 
				case CLASS_MARK1:
				case CLASS_MARK2: 
				case CLASS_WIZARD: 
			G_PlayEffect( "demp6/altDetonate", ent->currentOrigin, ent->pos1 );
			break;
				// REVERB
				case CLASS_NECROREAVER:
				case CLASS_JAWA:
				case CLASS_PROBE:
				case CLASS_COMMANDO:
				case CLASS_SABER_DROID:
				case CLASS_PROTOCOL:
				case CLASS_HOWLER: 
				case CLASS_THRALL: 
				case CLASS_POLTER:
				case CLASS_DARK: 
				case CLASS_VOID:  
				case CLASS_LIGHT: 
			  	case CLASS_CRYSTAL: 
				case CLASS_AIR:  
				case CLASS_EARTH: 
				case CLASS_NECRO: 
			  	case CLASS_SONIC: 
			 	case CLASS_GOLEM_DARK: 
				case CLASS_GOLEM_LIGHT:
				case CLASS_GOLEM_GOLD: 
				case CLASS_GOLEM_AIR:
				case CLASS_GOLEM_METAL:
				case CLASS_GOLEM:
				case CLASS_GOLEM_SONIC:
			 	case CLASS_GOLEM_NECRO:
				case CLASS_GOLEM_SMOKE: 
				case CLASS_GOLEM_VOID:
				case CLASS_GHOST: 
				case CLASS_PLAYER: 
				case CLASS_VEHICLE: 
				G_PlayEffect( "demp7/altDetonate", ent->currentOrigin, ent->pos1 );
				break;
				// BALANCE 
				case CLASS_KYLE: 
				case CLASS_KAIN3: 
				case CLASS_ARIEL: 
				G_PlayEffect( "demp8/altDetonate", ent->currentOrigin, ent->pos1 );
				break;
				// WRACK IMPORTANT: NEVER. NEVER GET THIS TO AN NPC!   
				//case CLASS_GALAKMECH:
				//case CLASS_GONK:
				case CLASS_REBORN:
				case CLASS_ALORA:
				case CLASS_MONMOTHA:
				case CLASS_VAMPIRE:
				case CLASS_VAMPIRE_DEMON:
				case CLASS_KAIN:
				case CLASS_KAIN2: 
				case CLASS_UMAH: 
				case CLASS_VORADOR:
				case CLASS_JANOS:
				case CLASS_NECROMANCER: 
				case CLASS_VAMPIRE_QUEEN:
				case CLASS_VAMPIRE_DARK: 
				case CLASS_VAMPIRE_NECRO: 
				case CLASS_VAMPIRE_NECRO2:
				case CLASS_VAMPIRE_FEAR: 
				case CLASS_VAMPIRE_FEAR2:  
				case CLASS_VAMPIRE_BLOOD:
				case CLASS_VAMPIRE_SOUND: 
				case CLASS_VAMPIRE_LYCAN: 
				case CLASS_VAMPIRE_ELF: 
				case CLASS_VAMPIRE_ELF2:
				case CLASS_VAMPIRE_ELF3:
				case CLASS_VAMPIRE_WATER: 
				case CLASS_VAMPIRE_FROST:  
				case CLASS_VAMPIRE_ARCHER: 
				case CLASS_HALFVAMPIRE: 
				case CLASS_VAMPIRE_BOUNTY: 
				case CLASS_VAMPIRE_ZOMBIE: 
				case CLASS_VAMPIRE_SARAFAN:  
				G_PlayEffect( "demp9/altDetonate", ent->currentOrigin, ent->pos1 );
				break;	
				case CLASS_FIGHTER: 
				G_PlayEffect( "demp00/altDetonate", ent->currentOrigin, ent->pos1 );
				break; 
			default: 
				break;
				}
			}
	// G_PlayEffect( "demp2/altDetonate", ent->currentOrigin, ent->pos1 );
	G_AddEvent( ent, EV_DEMP2_ALT_IMPACT, ent->count * 2 );

	ent->fx_time = level.time;
	ent->radius = 0;
	ent->nextthink = level.time + 50;
	ent->e_ThinkFunc = thinkF_DEMP2_AltRadiusDamage;
	ent->s.eType = ET_GENERAL; // make us a missile no longer
}

//---------------------------------------------------------
static void WP_DEMP2_AltFire( gentity_t *ent )
//---------------------------------------------------------
{
	int		damage	= weaponData[WP_REPEATER].altDamage;
	int		count;
	vec3_t	start;
	trace_t	tr;

	// DMG SACRO
	if ( ent && ent->client && ent->client->NPC_class == CLASS_TRANDOSHAN )
	{
		damage *= 1.5;
	}
	// DMG VITA
	else if ( ent && ent->client && ent->client->NPC_class == CLASS_LUKE 	)
	{
		damage *= 2; // Elementali del Calore fanno 50% di danno in più!
	}

	VectorCopy( muzzle, start );
	WP_TraceSetStart( ent, start, vec3_origin, vec3_origin );//make sure our start point isn't on the other side of a wall

	count = ( level.time - ent->client->ps.weaponChargeTime ) / DEMP2_CHARGE_UNIT;

	if ( count < 1 )
	{
		count = 1;
	}
	else if ( count > 3 )
	{
		count = 3;
	}

	damage *= ( 1 + ( count * ( count - 1 )));// yields damage of 12,36,84...gives a higher bonus for longer charge
		
	// HOLY 
	if ( ent->client->NPC_class == CLASS_HOLY || 
			ent->client->NPC_class == CLASS_GOLEM_HOLY || 
			ent->client->NPC_class == CLASS_VAMPIRE_GHOST )
	{
		damage *= 1.5;
	}
	// LIFE 
	else if ( ent->client->NPC_class == CLASS_LUKE || 
				ent->client->NPC_class == CLASS_IVY || 
				ent->client->NPC_class == CLASS_BEAST || 
				ent->client->NPC_class == CLASS_BIRD || 
				ent->client->NPC_class == CLASS_SHARK )
	{
		damage *= 2;
	}
	// SPIRITUAL 
	else if ( ent->client->NPC_class == CLASS_RAZIEL || 
			ent->client->NPC_class == CLASS_IMPWORKER )
	{
		damage *= 1;
	}
	// DRUIDIC FIRE 
	else if ( ent->client->NPC_class == CLASS_TRANDOSHAN ||
				ent->client->NPC_class == CLASS_LANDO || 
				ent->client->NPC_class == CLASS_DRUIDREAVER || 
				ent->client->NPC_class == CLASS_DRUID || 
				ent->client->NPC_class == CLASS_LYCAN )
	{
		damage *= 0.5;
	}
	// WHITE FIRE
	else if ( ent->client->NPC_class == CLASS_JAN ||
				ent->client->NPC_class == CLASS_SARAFAN || 
				ent->client->NPC_class == CLASS_STORMTROOPER || 
				ent->client->NPC_class == CLASS_MARK1 ||
				ent->client->NPC_class == CLASS_MARK2 || 
				ent->client->NPC_class == CLASS_WIZARD )
	{
		damage *= 0.75;
	}
	// REVERB - ONLY FOR SPECTRAL CREATURE AND UNDEAD. ARCHIMOND CLASS
	else if ( ent->client->NPC_class == CLASS_GALAKMECH ||
				ent->client->NPC_class == CLASS_GONK ||
				ent->client->NPC_class == CLASS_NECROREAVER ||
				ent->client->NPC_class == CLASS_JAWA ||
				ent->client->NPC_class == CLASS_PROBE ||
				ent->client->NPC_class == CLASS_COMMANDO ||
				ent->client->NPC_class == CLASS_SABER_DROID ||
				ent->client->NPC_class == CLASS_PROTOCOL ||
				ent->client->NPC_class == CLASS_HOWLER || 
				ent->client->NPC_class == CLASS_THRALL || 
				ent->client->NPC_class == CLASS_POLTER ||
				ent->client->NPC_class == CLASS_DARK || 
				ent->client->NPC_class == CLASS_VOID ||  
				ent->client->NPC_class == CLASS_LIGHT || 
			  	ent->client->NPC_class == CLASS_CRYSTAL || 
				ent->client->NPC_class == CLASS_AIR ||  
				ent->client->NPC_class == CLASS_EARTH || 
				ent->client->NPC_class == CLASS_NECRO || 
			  	ent->client->NPC_class == CLASS_SONIC || 
			 	ent->client->NPC_class == CLASS_GOLEM_DARK || 
				ent->client->NPC_class == CLASS_GOLEM_LIGHT ||
				ent->client->NPC_class == CLASS_GOLEM_GOLD || 
				ent->client->NPC_class == CLASS_GOLEM_AIR ||
				ent->client->NPC_class == CLASS_GOLEM_METAL ||
				ent->client->NPC_class == CLASS_GOLEM ||
				ent->client->NPC_class == CLASS_GOLEM_SONIC ||
			 	ent->client->NPC_class == CLASS_GOLEM_NECRO ||
				ent->client->NPC_class == CLASS_GOLEM_SMOKE || 
				ent->client->NPC_class == CLASS_GOLEM_VOID ||
				ent->client->NPC_class == CLASS_GHOST || 
				ent->client->NPC_class == CLASS_PLAYER || 
				ent->client->NPC_class == CLASS_VEHICLE )
	{
		damage *= 0.25;
	}
	// BALANCE SHOOT - ONLY FOR KAIN WITH BALANCE REAVER 
	else if ( ent->client->NPC_class == CLASS_KYLE || 
				ent->client->NPC_class == CLASS_KAIN3 || 
				ent->client->NPC_class == CLASS_ARIEL )
	{
		damage *= 3;
	}
	// WRACK - VAMPIRES - NOTE ALSO WEEQUAY BECAUSE ANTEO NOT USE TELEKINESIS 
	else if ( ent->client->NPC_class == CLASS_REBORN ||
				ent->client->NPC_class == CLASS_ALORA ||
				ent->client->NPC_class == CLASS_MONMOTHA ||
				ent->client->NPC_class == CLASS_VAMPIRE ||
				ent->client->NPC_class == CLASS_VAMPIRE_DEMON ||
				ent->client->NPC_class == CLASS_KAIN ||
				ent->client->NPC_class == CLASS_KAIN2 || 
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
				ent->client->NPC_class == CLASS_VAMPIRE_BOUNTY || 
				ent->client->NPC_class == CLASS_VAMPIRE_ZOMBIE || 
				ent->client->NPC_class == CLASS_VAMPIRE_SARAFAN )
	{
		damage *= 0.5;
	}
	// HELL WRACK - DEMONS AND HYLDENS 
	else if ( ent->client->NPC_class == CLASS_PROPHET || 
			ent->client->NPC_class == CLASS_CENOBITA || 
			ent->client->NPC_class == CLASS_WAMPA ||
			ent->client->NPC_class == CLASS_TAVION ||
			ent->client->NPC_class == CLASS_IMPERIAL ||
			ent->client->NPC_class == CLASS_DESANN ||
			ent->client->NPC_class == CLASS_ATST ||
			ent->client->NPC_class == CLASS_MOUSE ||
			ent->client->NPC_class == CLASS_SAND_CREATURE ||
			ent->client->NPC_class == CLASS_ELDERGOD || 
			ent->client->NPC_class == CLASS_TENTACLE || 
			ent->client->NPC_class == CLASS_MURJJ || 	
			ent->client->NPC_class == CLASS_HYLDEN || 
			ent->client->NPC_class == CLASS_HYLDENLORD || 
			ent->client->NPC_class == CLASS_DEMON_FIRE ||
			ent->client->NPC_class == CLASS_DEMON_POISON ||
			ent->client->NPC_class == CLASS_DEMON_BLADE || 
			ent->client->NPC_class == CLASS_DEMON_ELECTRO || 
			ent->client->NPC_class == CLASS_POISON || 
			ent->client->NPC_class == CLASS_GOLEM_POISON || 
			ent->client->NPC_class == CLASS_FIRE || 
			ent->client->NPC_class == CLASS_GOLEM_FIRE || 
			ent->client->NPC_class == CLASS_GOLEM_LAVA || 
			ent->client->NPC_class == CLASS_GRAN || 
			ent->client->NPC_class == CLASS_TURELIM || 
			ent->client->NPC_class == CLASS_CLAW || 
			ent->client->NPC_class == CLASS_TUSKEN ||
			ent->client->NPC_class == CLASS_GLIDER ||
			ent->client->NPC_class == CLASS_NOGHRI ||
			ent->client->NPC_class == CLASS_FLIER2 ||
			ent->client->NPC_class == CLASS_FISH ||
			ent->client->NPC_class == CLASS_SWAMPTROOPER ||			 
			ent->client->NPC_class == CLASS_MELC ||  
			ent->client->NPC_class == CLASS_SUMMONER ||
			ent->client->NPC_class == CLASS_ZEPH ||
			ent->client->NPC_class == CLASS_DECEIVER ||
			ent->client->NPC_class == CLASS_DUMAHIM || 
			ent->client->NPC_class == CLASS_RAHABIM ||
			ent->client->NPC_class == CLASS_HELLGOLEM ||
			ent->client->NPC_class == CLASS_RAZIELHIM ||  
				ent->client->NPC_class == CLASS_ESKANDOR )
	{
		damage *= 2;
	}
	// SPECTRAL - FOR ALL REAPERS 
	else if ( ent->client->NPC_class == CLASS_REELO ||
			ent->client->NPC_class == CLASS_RANCOR ||
			ent->client->NPC_class == CLASS_SENTRY ||
			ent->client->NPC_class == CLASS_MINEMONSTER ||
			ent->client->NPC_class == CLASS_SABOTEUR ||
			ent->client->NPC_class == CLASS_SHADOWTROOPER ||
			ent->client->NPC_class == CLASS_ASSASSIN_DROID ||
			ent->client->NPC_class == CLASS_MORGANKATARN ||
			ent->client->NPC_class == CLASS_HAZARD_TROOPER ||
			ent->client->NPC_class == CLASS_REBEL || 
			ent->client->NPC_class == CLASS_ROCKETTROOPER ||
			ent->client->NPC_class == CLASS_BOBAFETT ||
			ent->client->NPC_class == CLASS_BARTENDER ||
			ent->client->NPC_class == CLASS_UGNAUGHT ||
			ent->client->NPC_class == CLASS_JEDI ||
			ent->client->NPC_class == CLASS_REAVER || 
			ent->client->NPC_class == CLASS_LIZARD || 
			ent->client->NPC_class == CLASS_WEEQUAY || 
			ent->client->NPC_class == CLASS_DARKREAVER || 
			ent->client->NPC_class == CLASS_TANKREAVER ||
			ent->client->NPC_class == CLASS_LIGHTREAVER ||
			ent->client->NPC_class == CLASS_ICEREAVER || 
			ent->client->NPC_class == CLASS_SPIDERREAVER ||
			ent->client->NPC_class == CLASS_SPIDERREAVER2 ||
			ent->client->NPC_class == CLASS_SPIDERREAVER3 ||
			ent->client->NPC_class == CLASS_FIREREAVER || 
			ent->client->NPC_class == CLASS_AIRREAVER || 
			ent->client->NPC_class == CLASS_MENTALIST || 
			ent->client->NPC_class == CLASS_EARTHREAVER || 
			ent->client->NPC_class == CLASS_SLUAGH || 
			ent->client->NPC_class == CLASS_REAPER || 
			ent->client->NPC_class == CLASS_DREADNOUGHT )
	{
		damage *= 0.25;
	}
	// BENEDICTUM 
	else if ( ent->client->NPC_class == CLASS_HUNTER || 
			ent->client->NPC_class == CLASS_SCOUT ||  
			ent->client->NPC_class == CLASS_ALCHEMIST || 
			ent->client->NPC_class == CLASS_ARCHER ||  
			ent->client->NPC_class == CLASS_WARRIOR || 
			ent->client->NPC_class == CLASS_RODIAN ||
			ent->client->NPC_class == CLASS_PRISONER ||
			ent->client->NPC_class == CLASS_GALAK ||
			ent->client->NPC_class == CLASS_INTERROGATOR ||
			ent->client->NPC_class == CLASS_SWAMP ||
			ent->client->NPC_class == CLASS_WATER || 
			ent->client->NPC_class == CLASS_GOLEM_WATER || 
			ent->client->NPC_class == CLASS_GOLEM_ICE || 
			ent->client->NPC_class == CLASS_R2D2 ||
			ent->client->NPC_class == CLASS_R5D2 ||
			ent->client->NPC_class == CLASS_SEEKER ||
			ent->client->NPC_class == CLASS_REMOTE )
	{
		damage *= 0.75;
	}

	// the shot can travel a whopping 4096 units in 1 second. Note that the shot will auto-detonate at 4096 units...we'll see if this looks cool or not
	WP_MissileTargetHint(ent, start, forwardVec);
	gentity_t *missile = CreateMissile( start, forwardVec, DEMP2_ALT_RANGE, 1000, ent, qtrue );

	// letting it know what the charge size is.
	missile->count = count;

//	missile->speed = missile->nextthink;
	VectorCopy( tr.plane.normal, missile->pos1 );

	missile->classname = "demp2_alt_proj";
	missile->s.weapon = WP_DEMP2;

	missile->e_ThinkFunc = thinkF_DEMP2_AltDetonate;

	missile->splashDamage = missile->damage = damage;
	missile->splashMethodOfDeath = missile->methodOfDeath = MOD_DEMP2_ALT;
	missile->splashRadius = weaponData[WP_DEMP2].altSplashRadius;

	missile->dflags = DAMAGE_DEATH_KNOCKBACK;
	missile->clipmask = MASK_SHOT | CONTENTS_LIGHTSABER;

	// we don't want it to ever bounce
	missile->bounceCount = 0;
}

//---------------------------------------------------------
void WP_FireDEMP2( gentity_t *ent, qboolean alt_fire )
//---------------------------------------------------------
{
	if ( alt_fire )
	{
		WP_DEMP2_AltFire( ent );
	}
	else
	{
		WP_DEMP2_MainFire( ent );
	}

	if ( alt_fire )
{
	
	if ( ent->client->NPC_class == CLASS_HOLY || 
			ent->client->NPC_class == CLASS_GOLEM_HOLY || 
			ent->client->NPC_class == CLASS_VAMPIRE_GHOST )
	{
		{
			G_PlayEffect( G_EffectIndex( "demp3/altmuzzle_flash" ), ent->client->renderInfo.handRPoint );
		}
	}
	// LIFE 
	else if ( ent->client->NPC_class == CLASS_LUKE || 
				ent->client->NPC_class == CLASS_IVY || 
				ent->client->NPC_class == CLASS_BEAST || 
				ent->client->NPC_class == CLASS_BIRD || 
				ent->client->NPC_class == CLASS_SHARK )
	{
		{
			G_PlayEffect( G_EffectIndex( "demp4/altmuzzle_flash" ), ent->client->renderInfo.handRPoint );
		}
	}
	// SPIRITUAL 
	else if ( ent->client->NPC_class == CLASS_RAZIEL || 
			ent->client->NPC_class == CLASS_IMPWORKER )
	{
		{
			G_PlayEffect( G_EffectIndex( "demp2/altmuzzle_flash" ), ent->client->renderInfo.handRPoint );
		}
	}
	// DRUIDIC FIRE 
	else if ( ent->client->NPC_class == CLASS_TRANDOSHAN ||
				ent->client->NPC_class == CLASS_LANDO || 
				ent->client->NPC_class == CLASS_DRUIDREAVER || 
				ent->client->NPC_class == CLASS_DRUID || 
				ent->client->NPC_class == CLASS_LYCAN )
	{
		{
			G_PlayEffect( G_EffectIndex( "demp5/altmuzzle_flash" ), ent->client->renderInfo.handRPoint );
		}
	}
	// WHITE FIRE
	else if ( ent->client->NPC_class == CLASS_JAN ||
				ent->client->NPC_class == CLASS_SARAFAN || 
				ent->client->NPC_class == CLASS_STORMTROOPER || 
				ent->client->NPC_class == CLASS_MARK1 ||
				ent->client->NPC_class == CLASS_MARK2 || 
				ent->client->NPC_class == CLASS_WIZARD )
	{
		{
			G_PlayEffect( G_EffectIndex( "demp6/altmuzzle_flash" ), ent->client->renderInfo.handRPoint );
		}
	}
	// REVERB - ONLY FOR SPECTRAL CREATURE AND UNDEAD. ARCHIMOND CLASS
	else if ( ent->client->NPC_class == CLASS_GALAKMECH ||
				ent->client->NPC_class == CLASS_GONK ||
				ent->client->NPC_class == CLASS_NECROREAVER ||
				ent->client->NPC_class == CLASS_JAWA ||
				ent->client->NPC_class == CLASS_PROBE ||
				ent->client->NPC_class == CLASS_COMMANDO ||
				ent->client->NPC_class == CLASS_SABER_DROID ||
				ent->client->NPC_class == CLASS_PROTOCOL ||
				ent->client->NPC_class == CLASS_HOWLER || 
				ent->client->NPC_class == CLASS_THRALL || 
				ent->client->NPC_class == CLASS_POLTER ||
				ent->client->NPC_class == CLASS_DARK || 
				ent->client->NPC_class == CLASS_VOID ||  
				ent->client->NPC_class == CLASS_LIGHT || 
			  	ent->client->NPC_class == CLASS_CRYSTAL || 
				ent->client->NPC_class == CLASS_AIR ||  
				ent->client->NPC_class == CLASS_EARTH || 
				ent->client->NPC_class == CLASS_NECRO || 
			  	ent->client->NPC_class == CLASS_SONIC || 
			 	ent->client->NPC_class == CLASS_GOLEM_DARK || 
				ent->client->NPC_class == CLASS_GOLEM_LIGHT ||
				ent->client->NPC_class == CLASS_GOLEM_GOLD || 
				ent->client->NPC_class == CLASS_GOLEM_AIR ||
				ent->client->NPC_class == CLASS_GOLEM_METAL ||
				ent->client->NPC_class == CLASS_GOLEM ||
				ent->client->NPC_class == CLASS_GOLEM_SONIC ||
			 	ent->client->NPC_class == CLASS_GOLEM_NECRO ||
				ent->client->NPC_class == CLASS_GOLEM_SMOKE || 
				ent->client->NPC_class == CLASS_GOLEM_VOID ||
				ent->client->NPC_class == CLASS_GHOST || 
				ent->client->NPC_class == CLASS_PLAYER || 
				ent->client->NPC_class == CLASS_VEHICLE )
	{
		{
			G_PlayEffect( G_EffectIndex( "demp7/altmuzzle_flash" ), ent->client->renderInfo.handRPoint );

		}
	}
	// BALANCE SHOOT - ONLY FOR KAIN WITH BALANCE REAVER 
	else if ( ent->client->NPC_class == CLASS_KYLE || 
				ent->client->NPC_class == CLASS_KAIN3 || 
				ent->client->NPC_class == CLASS_ARIEL )
	{
		{
			G_PlayEffect( G_EffectIndex( "demp8/altmuzzle_flash" ), ent->client->renderInfo.handRPoint );
		}
	}
	// WRACK - VAMPIRES - NOTE ALSO WEEQUAY BECAUSE ANTEO NOT USE TELEKINESIS 
	else if ( ent->client->NPC_class == CLASS_REBORN ||
				ent->client->NPC_class == CLASS_ALORA ||
				ent->client->NPC_class == CLASS_MONMOTHA ||
				ent->client->NPC_class == CLASS_VAMPIRE ||
				ent->client->NPC_class == CLASS_VAMPIRE_DEMON ||
				ent->client->NPC_class == CLASS_KAIN ||
				ent->client->NPC_class == CLASS_KAIN2 || 
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
				ent->client->NPC_class == CLASS_VAMPIRE_BOUNTY || 
				ent->client->NPC_class == CLASS_VAMPIRE_ZOMBIE || 
				ent->client->NPC_class == CLASS_VAMPIRE_SARAFAN )
	{
		{
			G_PlayEffect( G_EffectIndex( "demp9/altmuzzle_flash" ), ent->client->renderInfo.handRPoint );

		}
	}
	// HELL WRACK - DEMONS AND HYLDENS 
	else if ( ent->client->NPC_class == CLASS_PROPHET || 
			ent->client->NPC_class == CLASS_CENOBITA || 
			ent->client->NPC_class == CLASS_WAMPA ||
			ent->client->NPC_class == CLASS_TAVION ||
			ent->client->NPC_class == CLASS_IMPERIAL ||
			ent->client->NPC_class == CLASS_DESANN ||
			ent->client->NPC_class == CLASS_ATST ||
			ent->client->NPC_class == CLASS_MOUSE ||
			ent->client->NPC_class == CLASS_SAND_CREATURE ||
			ent->client->NPC_class == CLASS_ELDERGOD || 
			ent->client->NPC_class == CLASS_TENTACLE || 
			ent->client->NPC_class == CLASS_MURJJ || 	
			ent->client->NPC_class == CLASS_HYLDEN || 
			ent->client->NPC_class == CLASS_HYLDENLORD || 
			ent->client->NPC_class == CLASS_DEMON_FIRE ||
			ent->client->NPC_class == CLASS_DEMON_POISON ||
			ent->client->NPC_class == CLASS_DEMON_BLADE || 
			ent->client->NPC_class == CLASS_DEMON_ELECTRO || 
			ent->client->NPC_class == CLASS_POISON || 
			ent->client->NPC_class == CLASS_GOLEM_POISON || 
			ent->client->NPC_class == CLASS_FIRE || 
			ent->client->NPC_class == CLASS_GOLEM_FIRE || 
			ent->client->NPC_class == CLASS_GOLEM_LAVA || 
			ent->client->NPC_class == CLASS_GRAN || 
			ent->client->NPC_class == CLASS_TURELIM || 
			ent->client->NPC_class == CLASS_CLAW || 
			ent->client->NPC_class == CLASS_TUSKEN ||
			ent->client->NPC_class == CLASS_GLIDER ||
			ent->client->NPC_class == CLASS_NOGHRI ||
			ent->client->NPC_class == CLASS_FLIER2 ||
			ent->client->NPC_class == CLASS_FISH ||
			ent->client->NPC_class == CLASS_SWAMPTROOPER ||			 
			ent->client->NPC_class == CLASS_MELC ||  
			ent->client->NPC_class == CLASS_SUMMONER ||
			ent->client->NPC_class == CLASS_ZEPH ||
			ent->client->NPC_class == CLASS_DECEIVER ||
			ent->client->NPC_class == CLASS_DUMAHIM || 
			ent->client->NPC_class == CLASS_RAHABIM ||
			ent->client->NPC_class == CLASS_RAZIELHIM ||  
			ent->client->NPC_class == CLASS_ESKANDOR ||  
			ent->client->NPC_class == CLASS_HELLGOLEM )
	{
		{
			G_PlayEffect( G_EffectIndex( "demp1/altmuzzle_flash" ), ent->client->renderInfo.handRPoint );

		}
	}
	// SPECTRAL - FOR ALL REAPERS 
	else if ( ent->client->NPC_class == CLASS_REELO ||
			ent->client->NPC_class == CLASS_RANCOR ||
			ent->client->NPC_class == CLASS_SENTRY ||
			ent->client->NPC_class == CLASS_MINEMONSTER ||
			ent->client->NPC_class == CLASS_SABOTEUR ||
			ent->client->NPC_class == CLASS_SHADOWTROOPER ||
			ent->client->NPC_class == CLASS_ASSASSIN_DROID ||
			ent->client->NPC_class == CLASS_MORGANKATARN ||
			ent->client->NPC_class == CLASS_HAZARD_TROOPER ||
			ent->client->NPC_class == CLASS_REBEL || 
			ent->client->NPC_class == CLASS_ROCKETTROOPER ||
			ent->client->NPC_class == CLASS_BOBAFETT ||
			ent->client->NPC_class == CLASS_BARTENDER ||
			ent->client->NPC_class == CLASS_UGNAUGHT ||
			ent->client->NPC_class == CLASS_JEDI ||
			ent->client->NPC_class == CLASS_REAVER || 
			ent->client->NPC_class == CLASS_LIZARD || 
			ent->client->NPC_class == CLASS_WEEQUAY || 
			ent->client->NPC_class == CLASS_DARKREAVER || 
			ent->client->NPC_class == CLASS_TANKREAVER ||
			ent->client->NPC_class == CLASS_LIGHTREAVER ||
			ent->client->NPC_class == CLASS_ICEREAVER || 
			ent->client->NPC_class == CLASS_SPIDERREAVER ||
			ent->client->NPC_class == CLASS_SPIDERREAVER2 ||
			ent->client->NPC_class == CLASS_SPIDERREAVER3 ||
			ent->client->NPC_class == CLASS_FIREREAVER || 
			ent->client->NPC_class == CLASS_AIRREAVER || 
			ent->client->NPC_class == CLASS_MENTALIST || 
			ent->client->NPC_class == CLASS_EARTHREAVER || 
			ent->client->NPC_class == CLASS_SLUAGH || 
			ent->client->NPC_class == CLASS_REAPER || 
			ent->client->NPC_class == CLASS_DREADNOUGHT )
	{
		{
			G_PlayEffect( G_EffectIndex( "demp0/altmuzzle_flash" ), ent->client->renderInfo.handRPoint );

		}
	}
	// BENEDICTUM 
	else if ( ent->client->NPC_class == CLASS_HUNTER || 
			ent->client->NPC_class == CLASS_SCOUT ||  
			ent->client->NPC_class == CLASS_ALCHEMIST || 
			ent->client->NPC_class == CLASS_ARCHER ||  
			ent->client->NPC_class == CLASS_WARRIOR || 
			ent->client->NPC_class == CLASS_RODIAN ||
			ent->client->NPC_class == CLASS_PRISONER ||
			ent->client->NPC_class == CLASS_GALAK ||
			ent->client->NPC_class == CLASS_INTERROGATOR ||
			ent->client->NPC_class == CLASS_SWAMP ||
			ent->client->NPC_class == CLASS_WATER || 
			ent->client->NPC_class == CLASS_GOLEM_WATER || 
			ent->client->NPC_class == CLASS_GOLEM_ICE || 
			ent->client->NPC_class == CLASS_R2D2 ||
			ent->client->NPC_class == CLASS_R5D2 ||
			ent->client->NPC_class == CLASS_SEEKER ||
			ent->client->NPC_class == CLASS_REMOTE )
	{
		{
			G_PlayEffect( G_EffectIndex( "demp/altmuzzle_flash" ), ent->client->renderInfo.handRPoint );

		}
	}
	else if ( ent->client->NPC_class == CLASS_FIGHTER )
	{
		{
			G_PlayEffect( G_EffectIndex( "demp00/altmuzzle_flash" ), ent->client->renderInfo.handRPoint );
		}
	}
}
else 
{
	if ( ent->client->NPC_class == CLASS_HOLY || 
			ent->client->NPC_class == CLASS_GOLEM_HOLY || 
			ent->client->NPC_class == CLASS_VAMPIRE_GHOST )
	{
		{
			G_PlayEffect( G_EffectIndex( "demp3/muzzle_flash" ), ent->client->renderInfo.handRPoint );
		}
	}
	// LIFE 
	else if ( ent->client->NPC_class == CLASS_LUKE || 
				ent->client->NPC_class == CLASS_IVY || 
				ent->client->NPC_class == CLASS_BEAST || 
				ent->client->NPC_class == CLASS_BIRD || 
				ent->client->NPC_class == CLASS_SHARK )
	{
		{
			G_PlayEffect( G_EffectIndex( "demp4/muzzle_flash" ), ent->client->renderInfo.handRPoint );
		}
	}
	// SPIRITUAL 
	else if ( ent->client->NPC_class == CLASS_RAZIEL || 
			ent->client->NPC_class == CLASS_IMPWORKER )
	{
		{
			G_PlayEffect( G_EffectIndex( "demp2/muzzle_flash" ), ent->client->renderInfo.handRPoint );
		}
	}
	// DRUIDIC FIRE 
	else if ( ent->client->NPC_class == CLASS_TRANDOSHAN ||
				ent->client->NPC_class == CLASS_LANDO || 
				ent->client->NPC_class == CLASS_DRUIDREAVER || 
				ent->client->NPC_class == CLASS_DRUID || 
				ent->client->NPC_class == CLASS_LYCAN )
	{
		{
			G_PlayEffect( G_EffectIndex( "demp5/muzzle_flash" ), ent->client->renderInfo.handRPoint );
		}
	}
	// WHITE FIRE
	else if ( ent->client->NPC_class == CLASS_JAN ||
				ent->client->NPC_class == CLASS_SARAFAN || 
				ent->client->NPC_class == CLASS_STORMTROOPER || 
				ent->client->NPC_class == CLASS_MARK1 ||
				ent->client->NPC_class == CLASS_MARK2 || 
				ent->client->NPC_class == CLASS_WIZARD )
	{
		{
			G_PlayEffect( G_EffectIndex( "demp6/muzzle_flash" ), ent->client->renderInfo.handRPoint );
		}
	}
	// REVERB - ONLY FOR SPECTRAL CREATURE AND UNDEAD. ARCHIMOND CLASS
	else if ( ent->client->NPC_class == CLASS_GALAKMECH ||
				ent->client->NPC_class == CLASS_GONK ||
				ent->client->NPC_class == CLASS_NECROREAVER ||
				ent->client->NPC_class == CLASS_JAWA ||
				ent->client->NPC_class == CLASS_PROBE ||
				ent->client->NPC_class == CLASS_COMMANDO ||
				ent->client->NPC_class == CLASS_SABER_DROID ||
				ent->client->NPC_class == CLASS_PROTOCOL ||
				ent->client->NPC_class == CLASS_HOWLER || 
				ent->client->NPC_class == CLASS_THRALL || 
				ent->client->NPC_class == CLASS_POLTER ||
				ent->client->NPC_class == CLASS_DARK || 
				ent->client->NPC_class == CLASS_VOID ||  
				ent->client->NPC_class == CLASS_LIGHT || 
			  	ent->client->NPC_class == CLASS_CRYSTAL || 
				ent->client->NPC_class == CLASS_AIR ||  
				ent->client->NPC_class == CLASS_EARTH || 
				ent->client->NPC_class == CLASS_NECRO || 
			  	ent->client->NPC_class == CLASS_SONIC || 
			 	ent->client->NPC_class == CLASS_GOLEM_DARK || 
				ent->client->NPC_class == CLASS_GOLEM_LIGHT ||
				ent->client->NPC_class == CLASS_GOLEM_GOLD || 
				ent->client->NPC_class == CLASS_GOLEM_AIR ||
				ent->client->NPC_class == CLASS_GOLEM_METAL ||
				ent->client->NPC_class == CLASS_GOLEM ||
				ent->client->NPC_class == CLASS_GOLEM_SONIC ||
			 	ent->client->NPC_class == CLASS_GOLEM_NECRO ||
				ent->client->NPC_class == CLASS_GOLEM_SMOKE || 
				ent->client->NPC_class == CLASS_GOLEM_VOID ||
				ent->client->NPC_class == CLASS_GHOST || 
				ent->client->NPC_class == CLASS_PLAYER || 
				ent->client->NPC_class == CLASS_VEHICLE )
	{
		{
			G_PlayEffect( G_EffectIndex( "demp7/muzzle_flash" ), ent->client->renderInfo.handRPoint );

		}
	}
	// BALANCE SHOOT - ONLY FOR KAIN WITH BALANCE REAVER 
	else if ( ent->client->NPC_class == CLASS_KYLE || 
				ent->client->NPC_class == CLASS_KAIN3 || 
				ent->client->NPC_class == CLASS_ARIEL )
	{
		{
			G_PlayEffect( G_EffectIndex( "demp8/muzzle_flash" ), ent->client->renderInfo.handRPoint );
		}
	}
	// WRACK - VAMPIRES - NOTE ALSO WEEQUAY BECAUSE ANTEO NOT USE TELEKINESIS 
	else if ( ent->client->NPC_class == CLASS_REBORN ||
				ent->client->NPC_class == CLASS_ALORA ||
				ent->client->NPC_class == CLASS_MONMOTHA ||
				ent->client->NPC_class == CLASS_VAMPIRE ||
				ent->client->NPC_class == CLASS_VAMPIRE_DEMON ||
				ent->client->NPC_class == CLASS_KAIN ||
				ent->client->NPC_class == CLASS_KAIN2 || 
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
				ent->client->NPC_class == CLASS_VAMPIRE_BOUNTY || 
				ent->client->NPC_class == CLASS_VAMPIRE_ZOMBIE || 
				ent->client->NPC_class == CLASS_VAMPIRE_SARAFAN )
	{
		{
			G_PlayEffect( G_EffectIndex( "demp9/muzzle_flash" ), ent->client->renderInfo.handRPoint );

		}
	}
	// HELL WRACK - DEMONS AND HYLDENS 
	else if ( ent->client->NPC_class == CLASS_PROPHET || 
			ent->client->NPC_class == CLASS_CENOBITA || 
			ent->client->NPC_class == CLASS_WAMPA ||
			ent->client->NPC_class == CLASS_TAVION ||
			ent->client->NPC_class == CLASS_IMPERIAL ||
			ent->client->NPC_class == CLASS_DESANN ||
			ent->client->NPC_class == CLASS_ATST ||
			ent->client->NPC_class == CLASS_MOUSE ||
			ent->client->NPC_class == CLASS_SAND_CREATURE ||
			ent->client->NPC_class == CLASS_ELDERGOD || 
			ent->client->NPC_class == CLASS_TENTACLE || 
			ent->client->NPC_class == CLASS_MURJJ || 	
			ent->client->NPC_class == CLASS_HYLDEN || 
			ent->client->NPC_class == CLASS_HYLDENLORD || 
			ent->client->NPC_class == CLASS_DEMON_FIRE ||
			ent->client->NPC_class == CLASS_DEMON_POISON ||
			ent->client->NPC_class == CLASS_DEMON_BLADE || 
			ent->client->NPC_class == CLASS_DEMON_ELECTRO || 
			ent->client->NPC_class == CLASS_POISON || 
			ent->client->NPC_class == CLASS_GOLEM_POISON || 
			ent->client->NPC_class == CLASS_FIRE || 
			ent->client->NPC_class == CLASS_GOLEM_FIRE || 
			ent->client->NPC_class == CLASS_GOLEM_LAVA || 
			ent->client->NPC_class == CLASS_GRAN || 
			ent->client->NPC_class == CLASS_TURELIM || 
			ent->client->NPC_class == CLASS_CLAW || 
			ent->client->NPC_class == CLASS_TUSKEN ||
			ent->client->NPC_class == CLASS_GLIDER ||
			ent->client->NPC_class == CLASS_NOGHRI ||
			ent->client->NPC_class == CLASS_FLIER2 ||
			ent->client->NPC_class == CLASS_FISH ||
			ent->client->NPC_class == CLASS_SWAMPTROOPER ||			 
			ent->client->NPC_class == CLASS_MELC ||  
			ent->client->NPC_class == CLASS_SUMMONER ||
			ent->client->NPC_class == CLASS_ZEPH ||
			ent->client->NPC_class == CLASS_DECEIVER ||
			ent->client->NPC_class == CLASS_DUMAHIM || 
			ent->client->NPC_class == CLASS_RAHABIM ||
			ent->client->NPC_class == CLASS_RAZIELHIM ||  
			ent->client->NPC_class == CLASS_ESKANDOR ||  
			ent->client->NPC_class == CLASS_HELLGOLEM )
	{
		{
			G_PlayEffect( G_EffectIndex( "demp1/muzzle_flash" ), ent->client->renderInfo.handRPoint );

		}
	}
	// SPECTRAL - FOR ALL REAPERS 
	else if ( ent->client->NPC_class == CLASS_REELO ||
			ent->client->NPC_class == CLASS_RANCOR ||
			ent->client->NPC_class == CLASS_SENTRY ||
			ent->client->NPC_class == CLASS_MINEMONSTER ||
			ent->client->NPC_class == CLASS_SABOTEUR ||
			ent->client->NPC_class == CLASS_SHADOWTROOPER ||
			ent->client->NPC_class == CLASS_ASSASSIN_DROID ||
			ent->client->NPC_class == CLASS_MORGANKATARN ||
			ent->client->NPC_class == CLASS_HAZARD_TROOPER ||
			ent->client->NPC_class == CLASS_REBEL || 
			ent->client->NPC_class == CLASS_ROCKETTROOPER ||
			ent->client->NPC_class == CLASS_BOBAFETT ||
			ent->client->NPC_class == CLASS_BARTENDER ||
			ent->client->NPC_class == CLASS_UGNAUGHT ||
			ent->client->NPC_class == CLASS_JEDI ||
			ent->client->NPC_class == CLASS_REAVER || 
			ent->client->NPC_class == CLASS_LIZARD || 
			ent->client->NPC_class == CLASS_WEEQUAY || 
			ent->client->NPC_class == CLASS_DARKREAVER || 
			ent->client->NPC_class == CLASS_TANKREAVER ||
			ent->client->NPC_class == CLASS_LIGHTREAVER ||
			ent->client->NPC_class == CLASS_ICEREAVER || 
			ent->client->NPC_class == CLASS_SPIDERREAVER ||
			ent->client->NPC_class == CLASS_SPIDERREAVER2 ||
			ent->client->NPC_class == CLASS_SPIDERREAVER3 ||
			ent->client->NPC_class == CLASS_FIREREAVER || 
			ent->client->NPC_class == CLASS_AIRREAVER || 
			ent->client->NPC_class == CLASS_MENTALIST || 
			ent->client->NPC_class == CLASS_EARTHREAVER || 
			ent->client->NPC_class == CLASS_SLUAGH || 
			ent->client->NPC_class == CLASS_REAPER || 
			ent->client->NPC_class == CLASS_DREADNOUGHT )
	{
		{
			G_PlayEffect( G_EffectIndex( "demp0/muzzle_flash" ), ent->client->renderInfo.handRPoint );

		}
	}
	// BENEDICTUM 
	else if ( ent->client->NPC_class == CLASS_HUNTER || 
			ent->client->NPC_class == CLASS_SCOUT ||  
			ent->client->NPC_class == CLASS_ALCHEMIST || 
			ent->client->NPC_class == CLASS_ARCHER ||  
			ent->client->NPC_class == CLASS_WARRIOR || 
			ent->client->NPC_class == CLASS_RODIAN ||
			ent->client->NPC_class == CLASS_PRISONER ||
			ent->client->NPC_class == CLASS_GALAK ||
			ent->client->NPC_class == CLASS_INTERROGATOR ||
			ent->client->NPC_class == CLASS_SWAMP ||
			ent->client->NPC_class == CLASS_WATER || 
			ent->client->NPC_class == CLASS_GOLEM_WATER || 
			ent->client->NPC_class == CLASS_GOLEM_ICE || 
			ent->client->NPC_class == CLASS_R2D2 ||
			ent->client->NPC_class == CLASS_R5D2 ||
			ent->client->NPC_class == CLASS_SEEKER ||
			ent->client->NPC_class == CLASS_REMOTE )
	{
		{
			G_PlayEffect( G_EffectIndex( "demp/muzzle_flash" ), ent->client->renderInfo.handRPoint );

		}
	}
	else if ( ent->client->NPC_class == CLASS_FIGHTER )
	{
		{
			G_PlayEffect( G_EffectIndex( "demp00/muzzle_flash" ), ent->client->renderInfo.handRPoint );
		}
	}
}
}

// THE POISON WEAPON 

//---------------------------------------------------------
static void WP_Poison_MainFire( gentity_t *ent )
//---------------------------------------------------------
{
	vec3_t	start;
	int		damage	= weaponData[WP_POISON].damage;

	VectorCopy( muzzle, start );
	WP_TraceSetStart( ent, start, vec3_origin, vec3_origin );//make sure our start point isn't on the other side of a wall

	WP_MissileTargetHint(ent, start, forwardVec);

	gentity_t *missile = CreateMissile( start, forwardVec, POISON_VELOCITY, 10000, ent );

	missile->classname = "poison_proj";
	missile->s.weapon = WP_POISON;

	// Do the damages
	if ( ent->s.number != 0 )
	{
		if ( g_spskill->integer == 0 )
		{
			damage = POISON_NPC_DAMAGE_EASY;
		}
		else if ( g_spskill->integer == 1 )
		{
			damage = POISON_NPC_DAMAGE_NORMAL;
		}
		else
		{
			damage = POISON_NPC_DAMAGE_HARD;
		}
	}

	VectorSet( missile->maxs, POISON_SIZE, POISON_SIZE, POISON_SIZE );
	VectorScale( missile->maxs, -1, missile->mins );

//	if ( ent->client && ent->client->ps.powerups[PW_WEAPON_OVERCHARGE] > 0 && ent->client->ps.powerups[PW_WEAPON_OVERCHARGE] > cg.time )
//	{
//		// in overcharge mode, so doing double damage
//		missile->flags |= FL_OVERCHARGED;
//		damage *= 2;
//	}

	missile->damage = damage;
	missile->methodOfDeath = MOD_POISON;
	missile->clipmask = MASK_SHOT | CONTENTS_LIGHTSABER;

	// we don't want it to ever bounce
	missile->bounceCount = 0;
}

// NOTE: this is 100% for the demp2 alt-fire effect, so changes to the visual effect will affect game side demp2 code
//--------------------------------------------------
void Poison_AltRadiusDamage( gentity_t *ent )
{
	float		frac = ( level.time - ent->fx_time ) / 1300.0f; // synchronize with demp2 effect
	float		dist, radius;
	gentity_t	*gent;
	gentity_t	*entityList[MAX_GENTITIES];
	int			numListedEntities, i, e;
	vec3_t		mins, maxs;
	vec3_t		v, dir;

	frac *= frac * frac; // yes, this is completely ridiculous...but it causes the shell to grow slowly then "explode" at the end
	
	radius = frac * 200.0f; // 200 is max radius...the model is aprox. 100 units tall...the fx draw code mults. this by 2.

	for ( i = 0 ; i < 3 ; i++ ) 
	{
		mins[i] = ent->currentOrigin[i] - radius;
		maxs[i] = ent->currentOrigin[i] + radius;
	}

	numListedEntities = gi.EntitiesInBox( mins, maxs, entityList, MAX_GENTITIES );

	for ( e = 0 ; e < numListedEntities ; e++ ) 
	{
		gent = entityList[ e ];

		if ( !gent->takedamage || !gent->contents )
		{
			continue;
		}

		// find the distance from the edge of the bounding box
		for ( i = 0 ; i < 3 ; i++ ) 
		{
			if ( ent->currentOrigin[i] < gent->absmin[i] ) 
			{
				v[i] = gent->absmin[i] - ent->currentOrigin[i];
			} 
			else if ( ent->currentOrigin[i] > gent->absmax[i] ) 
			{
				v[i] = ent->currentOrigin[i] - gent->absmax[i];
			} 
			else 
			{
				v[i] = 0;
			}
		}

		// shape is an ellipsoid, so cut vertical distance in half`
		v[2] *= 0.5f;

		dist = VectorLength( v );

		if ( dist >= radius ) 
		{
			// shockwave hasn't hit them yet
			continue;
		}

		if ( dist < ent->radius )
		{
			// shockwave has already hit this thing...
			continue;
		}

		VectorCopy( gent->currentOrigin, v );
		VectorSubtract( v, ent->currentOrigin, dir);

		// push the center of mass higher than the origin so players get knocked into the air more
		dir[2] += 12;

		G_Damage( gent, ent, ent->owner, dir, ent->currentOrigin, weaponData[WP_POISON].altDamage, DAMAGE_DEATH_KNOCKBACK, ent->splashMethodOfDeath );
		if ( gent->takedamage && gent->client ) 
		{
			gent->s.powerups |= ( 1 << PW_SHOCKED );
			gent->client->ps.powerups[PW_SHOCKED] = level.time + 2000;
			Saboteur_Decloak( gent, Q_irand( 3000, 10000 ) );
		}
	}

	// store the last fraction so that next time around we can test against those things that fall between that last point and where the current shockwave edge is
	ent->radius = radius;

	if ( frac < 1.0f )
	{
		// shock is still happening so continue letting it expand
		ent->nextthink = level.time + 50;
	}
}


//---------------------------------------------------------
void Poison_AltDetonate( gentity_t *ent )
//---------------------------------------------------------
{
	G_SetOrigin( ent, ent->currentOrigin );

	// start the effects, unfortunately, I wanted to do some custom things that I couldn't easily do with the fx system, so part of it uses an event and localEntities
	G_PlayEffect( "poison/altDetonate", ent->currentOrigin, ent->pos1 );
	G_AddEvent( ent, EV_DEMP2_ALT_IMPACT, ent->count * 2 );

	ent->fx_time = level.time;
	ent->radius = 0;
	ent->nextthink = level.time + 50;
	ent->e_ThinkFunc = thinkF_DEMP2_AltRadiusDamage;
	ent->s.eType = ET_GENERAL; // make us a missile no longer
}

//---------------------------------------------------------
static void WP_Poison_AltFire( gentity_t *ent )
//---------------------------------------------------------
{
	int			damage	= weaponData[WP_POISON].damage, count;
	float		vel;
	vec3_t		angs, dir, start;
	gentity_t	*missile;

	VectorCopy( muzzle, start );
	WP_TraceSetStart( ent, start, vec3_origin, vec3_origin );//make sure our start point isn't on the other side of a wall

	// Do the damages
	if ( ent->s.number != 0 )
	{
		if ( g_spskill->integer == 0 )
		{
			damage = POISON_NPC_DAMAGE_EASY;
		}
		else if ( g_spskill->integer == 1 )
		{
			damage = POISON_NPC_DAMAGE_NORMAL;
		}
		else
		{
			damage = POISON_NPC_DAMAGE_HARD;
		}
	}

	count = ( level.time - ent->client->ps.weaponChargeTime ) / POISON_CHARGE_UNIT;

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
		vel = POISON_VELOCITY * ( crandom() * BOWCASTER_VEL_RANGE + 1.0f );

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

		missile->classname = "poison_proj";
		missile->s.weapon = WP_POISON;

		VectorSet( missile->maxs, POISON_SIZE, POISON_SIZE, POISON_SIZE );
		VectorScale( missile->maxs, -1, missile->mins );

//		if ( ent->client && ent->client->ps.powerups[PW_WEAPON_OVERCHARGE] > 0 && ent->client->ps.powerups[PW_WEAPON_OVERCHARGE] > cg.time )
//		{
//			missile->flags |= FL_OVERCHARGED;
//		}

		missile->damage = damage;
		missile->methodOfDeath = MOD_POISON;
		missile->clipmask = MASK_SHOT | CONTENTS_LIGHTSABER;
		missile->splashDamage = weaponData[WP_POISON].splashDamage;
		missile->splashRadius = weaponData[WP_POISON].splashRadius;

		// we don't want it to bounce
		missile->bounceCount = 0;
		ent->client->sess.missionStats.shotsFired++;
	}
}



//---------------------------------------------------------
void WP_FirePoison( gentity_t *ent, qboolean alt_fire )
//---------------------------------------------------------
{
	if ( alt_fire )
	{
		WP_Poison_AltFire( ent );
	}
	else
	{
		WP_Poison_MainFire( ent );
	}
}



