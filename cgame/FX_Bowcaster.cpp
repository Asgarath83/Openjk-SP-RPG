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

// Bowcaster Weapon

// this line must stay at top so the whole PCH thing works...
#include "cg_headers.h"

//#include "cg_local.h"
#include "cg_media.h"
#include "FxScheduler.h"

/*
---------------------------
FX_BowcasterProjectileThink
---------------------------
*/

void FX_BowcasterProjectileThink( centity_t *cent, const struct weaponInfo_s *weapon )
{
	vec3_t forward;

	if ( VectorNormalize2( cent->gent->s.pos.trDelta, forward ) == 0.0f )
	{
		if ( VectorNormalize2( cent->currentState.pos.trDelta, forward ) == 0.0f )
		{
			forward[2] = 1.0f;
		}
	}

	// hack the scale of the forward vector if we were just fired or bounced...this will shorten up the tail for a split second so tails don't clip so harshly
	int dif = cg.time - cent->gent->s.pos.trTime;

	if ( dif < 75 )
	{
		if ( dif < 0 )
		{
			dif = 0;
		}

		float scale = ( dif / 75.0f ) * 0.95f + 0.05f;

		VectorScale( forward, scale, forward );
	}
	
	if ( cent->gent && cent->gent->owner && cent->gent->owner->client )
		{
			switch ( cent->gent->owner->client->NPC_class )
			{
			// FULMINE
			case CLASS_LIGHTREAVER: 
			case CLASS_LYCAN: 
			case CLASS_GOLEM_LIGHT: 
			case CLASS_SABER_DROID:
			case CLASS_ASSASSIN_DROID: 
			theFxScheduler.PlayEffect( cgs.effects.shockShotEffect, cent->lerpOrigin, forward );
			break;
			// RADIANZA 
			case CLASS_MORGANKATARN:
			case CLASS_GOLEM_GOLD: 
			case CLASS_PROTOCOL: 
			case CLASS_JAN: 
			case CLASS_CLAW: 
			theFxScheduler.PlayEffect( cgs.effects.sunShotEffect, cent->lerpOrigin, forward );
			break;
			// ENERGY BOLT 
			case CLASS_REBORN:
			case CLASS_KYLE:
			case CLASS_ALORA:
			case CLASS_MONMOTHA:
			case CLASS_TUSKEN:
			case CLASS_GLIDER:
			case CLASS_NOGHRI:
			case CLASS_FISH:
			case CLASS_GRAN:
			case CLASS_LIZARD:
			case CLASS_SWAMPTROOPER:
			case CLASS_REBEL:
			case CLASS_ROCKETTROOPER:
			case CLASS_BOBAFETT: 
			case CLASS_R2D2:
			case CLASS_R5D2:
			case CLASS_WAMPA: 
			case CLASS_GOLEM_METAL: 
			case CLASS_DEMON_ELECTRO: 			 
			case CLASS_MELC:  
			case CLASS_SUMMONER:
			case CLASS_ZEPH:
			case CLASS_DUMAHIM: 
			case CLASS_RAHABIM:
			case CLASS_TURELIM:
			case CLASS_VAMPIRE:
			case CLASS_VAMPIRE_DEMON:
			case CLASS_VAMPIRE_GHOST: 
			case CLASS_KAIN:
			case CLASS_KAIN2: 
			case CLASS_KAIN3:
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
			case CLASS_RAZIELHIM:  
			case CLASS_ESKANDOR: 
			case CLASS_VAMPIRE_BOUNTY: 
			case CLASS_VAMPIRE_ZOMBIE:
			case CLASS_VAMPIRE_SARAFAN: 
			case CLASS_AIR: 
			case CLASS_GOLEM_AIR: 
			case CLASS_BIRD: 
			case CLASS_IVY: 
			theFxScheduler.PlayEffect( cgs.effects.ebShotEffect, cent->lerpOrigin, forward );
			break; 
			// LIGHT KNIFE
			case CLASS_GALAK:
			case CLASS_RODIAN:
			case CLASS_MARK1:
			case CLASS_MARK2: 
			case CLASS_HUNTER: 
			case CLASS_SCOUT:  
			case CLASS_SARAFAN: 
			case CLASS_ARCHER:  
			case CLASS_WARRIOR:  
			theFxScheduler.PlayEffect( cgs.effects.lightknifeShotEffect, cent->lerpOrigin, forward );
			break;
			// LIGHT SKULL 
			case CLASS_TAVION:
			case CLASS_DESANN:
			case CLASS_HYLDEN: 
			case CLASS_HYLDENLORD: 
			case CLASS_SAND_CREATURE:
			case CLASS_ATST:
			case CLASS_SENTRY:
			case CLASS_MOUSE: 
			case CLASS_TENTACLE:  
			case CLASS_ELDERGOD: 
			case CLASS_SONIC: 
			case CLASS_GOLEM_SONIC: 
			case CLASS_PROPHET: 
			case CLASS_DECEIVER: 
			case CLASS_FLIER2: 
			case CLASS_BEAST:  
			case CLASS_SHARK:
			theFxScheduler.PlayEffect( cgs.effects.madShotEffect, cent->lerpOrigin, forward );
			break;
			case CLASS_ARIEL: 
			case CLASS_MINEMONSTER:
			case CLASS_DEMON_BLADE: 
			case CLASS_DEMON_POISON: 
			case CLASS_DEMON_FIRE: 
			case CLASS_JAWA: 
			case CLASS_RANCOR: 
			case CLASS_SHADOWTROOPER: 
			case CLASS_SABOTEUR: 
			case CLASS_MURJJ:
			case CLASS_NECRO: 
			case CLASS_THRALL: 
			case CLASS_IMPERIAL: 
			case CLASS_CENOBITA: 
			case CLASS_DARK: 
			case CLASS_VOID: 
			case CLASS_POISON: 
			case CLASS_GOLEM_DARK: 
			case CLASS_HELLGOLEM: 
			case CLASS_GOLEM_NECRO:
			case CLASS_GOLEM_SMOKE: 
			case CLASS_GOLEM_VOID:
			case CLASS_GOLEM_POISON: 
			case CLASS_GOLEM_LAVA:
			case CLASS_POLTER: 
			theFxScheduler.PlayEffect( cgs.effects.lightskullShotEffect, cent->lerpOrigin, forward );
			break;
			case CLASS_BARTENDER:
			case CLASS_BESPIN_COP:
			case CLASS_COMMANDO: 
			case CLASS_HOWLER: 
			case CLASS_IMPWORKER: 
			case CLASS_WIZARD: 
			case CLASS_INTERROGATOR:
			case CLASS_JEDI: 
			case CLASS_LANDO: 
			case CLASS_LUKE: 
			case CLASS_GONK: 
			case CLASS_PRISONER: 
			case CLASS_PROBE: 
			case CLASS_HAZARD_TROOPER: 
			case CLASS_REELO: 
			case CLASS_REMOTE: 
			case CLASS_SEEKER: 
			case CLASS_STORMTROOPER: 
			case CLASS_SWAMP: 
			case CLASS_TRANDOSHAN: 
			case CLASS_UGNAUGHT: 
			case CLASS_WEEQUAY: 
			case CLASS_PLAYER: 
			case CLASS_VEHICLE: 
			case CLASS_GALAKMECH:
			case CLASS_ALCHEMIST: 
			case CLASS_DRUID:
			case CLASS_REAVER: 
			case CLASS_RAZIEL:
			case CLASS_NECROREAVER:
			case CLASS_DARKREAVER: 
			case CLASS_TANKREAVER:			
			case CLASS_DRUIDREAVER:
			case CLASS_ICEREAVER: 
			case CLASS_SPIDERREAVER:
			case CLASS_SPIDERREAVER2:
			case CLASS_SPIDERREAVER3:
			case CLASS_FIREREAVER: 
			case CLASS_AIRREAVER: 
			case CLASS_MENTALIST: 
			case CLASS_EARTHREAVER:		 
			case CLASS_LIGHT: 
			case CLASS_FIRE:  
			case CLASS_WATER: 
			case CLASS_CRYSTAL: 
			case CLASS_EARTH: 
			case CLASS_HOLY:  
			case CLASS_GOLEM_FIRE:
			case CLASS_GOLEM_WATER:
			case CLASS_GOLEM_ICE:			
			case CLASS_GOLEM:
			case CLASS_GOLEM_HOLY:
			theFxScheduler.PlayEffect( cgs.effects.bowcasterShotEffect, cent->lerpOrigin, forward );
			break; 
			case CLASS_FIGHTER:
			theFxScheduler.PlayEffect( cgs.effects.dardus0ShotEffect, cent->lerpOrigin, forward );
			break; 
		default:
			break;
			}
		}

		// theFxScheduler.PlayEffect( cgs.effects.bowcasterShotEffect, cent->lerpOrigin, forward );

}

/*
---------------------------
FX_BowcasterHitWall
---------------------------
*/

void FX_BowcasterHitWall( vec3_t origin, vec3_t normal )
{
	theFxScheduler.PlayEffect( cgs.effects.bowcasterImpactEffect, origin, normal );
}

void FX_ShockHitWall( vec3_t origin, vec3_t normal )
{
	theFxScheduler.PlayEffect( cgs.effects.shockImpactEffect, origin, normal );
}

void FX_SunHitWall( vec3_t origin, vec3_t normal )
{
	theFxScheduler.PlayEffect( cgs.effects.sunImpactEffect, origin, normal );
}

void FX_LightKnifeHitWall( vec3_t origin, vec3_t normal )
{
	theFxScheduler.PlayEffect( cgs.effects.lightknifeImpactEffect, origin, normal );
}

void FX_LightSkullHitWall( vec3_t origin, vec3_t normal )
{
	theFxScheduler.PlayEffect( cgs.effects.lightskullImpactEffect, origin, normal );
}

void FX_MadnessHitWall( vec3_t origin, vec3_t normal )
{
	theFxScheduler.PlayEffect( cgs.effects.madImpactEffect, origin, normal );
}

void FX_EbHitWall( vec3_t origin, vec3_t normal )
{
	theFxScheduler.PlayEffect( cgs.effects.ebImpactEffect, origin, normal );
}

void FX_DardusHitWall( vec3_t origin, vec3_t normal )
{
	theFxScheduler.PlayEffect( cgs.effects.dardus0ImpactEffect, origin, normal );
}

/*
---------------------------
FX_BowcasterHitPlayer
---------------------------
*/

void FX_BowcasterHitPlayer( vec3_t origin, vec3_t normal, qboolean humanoid )
{
	theFxScheduler.PlayEffect( cgs.effects.bowcasterImpactEffect, origin, normal );
}

void FX_ShockHitPlayer( vec3_t origin, vec3_t normal, qboolean humanoid )
{
	theFxScheduler.PlayEffect( cgs.effects.shockImpactEffect, origin, normal );
}

void FX_SunHitPlayer( vec3_t origin, vec3_t normal, qboolean humanoid )
{
	theFxScheduler.PlayEffect( cgs.effects.sunImpactEffect, origin, normal );
}

void FX_EbHitPlayer( vec3_t origin, vec3_t normal, qboolean humanoid )
{
	theFxScheduler.PlayEffect( cgs.effects.eb2ImpactEffect, origin, normal );
}

void FX_LightKnifeHitPlayer( vec3_t origin, vec3_t normal, qboolean humanoid )
{
	theFxScheduler.PlayEffect( cgs.effects.lightknife2ImpactEffect, origin, normal );
}

void FX_LightSkullHitPlayer( vec3_t origin, vec3_t normal, qboolean humanoid )
{
	theFxScheduler.PlayEffect( cgs.effects.lightskull2ImpactEffect, origin, normal );
}

void FX_MadnessHitPlayer( vec3_t origin, vec3_t normal, qboolean humanoid )
{
	theFxScheduler.PlayEffect( cgs.effects.mad2ImpactEffect, origin, normal );
}

void FX_DardusHitPlayer( vec3_t origin, vec3_t normal, qboolean humanoid )
{
	theFxScheduler.PlayEffect( cgs.effects.dardus0ImpactEffect, origin, normal );
}