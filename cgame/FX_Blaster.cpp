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

// Blaster Weapon

// this line must stay at top so the whole PCH thing works...
#include "cg_headers.h"

#include "cg_local.h"
#include "cg_media.h"
#include "FxScheduler.h"

/*
-------------------------
FX_BlasterProjectileThink
-------------------------
*/
void FX_BlasterProjectileThink( centity_t *cent, const struct weaponInfo_s *weapon )
{
	vec3_t forward;
	if (cent->currentState.eFlags & EF_USE_ANGLEDELTA)
	{
		AngleVectors(cent->currentState.angles, forward, 0, 0);
	}
	else
	{
		if ( VectorNormalize2( cent->gent->s.pos.trDelta, forward ) == 0.0f )
		{
			if ( VectorNormalize2( cent->currentState.pos.trDelta, forward ) == 0.0f )
			{
				forward[2] = 1.0f;
			}
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

	if ( cent->gent && cent->gent->owner && cent->gent->owner->s.number > 0 )
	{
		//theFxScheduler.PlayEffect( "blaster/NPCshot", cent->lerpOrigin, forward );
		if ( cent->gent && cent->gent->owner && cent->gent->owner->client )
		{
			switch ( cent->gent->owner->client->NPC_class )
			{
			case CLASS_PROBE: 
			case CLASS_HAZARD_TROOPER:
			case CLASS_SABOTEUR: 
			case CLASS_SHADOWTROOPER: 
			case CLASS_IMPERIAL:
			case CLASS_WAMPA:
			case CLASS_BOBAFETT:
			case CLASS_DEMON_FIRE: 
			case CLASS_HELLGOLEM: 
			case CLASS_VAMPIRE_DEMON: 
			case CLASS_FIREREAVER:
			case CLASS_GOLEM_SMOKE: 
			case CLASS_AIR: 			
			case CLASS_GOLEM_AIR:
			case CLASS_LIGHT: 
			case CLASS_GOLEM_LIGHT:
			case CLASS_GOLEM_GOLD:
			case CLASS_LYCAN:  
			theFxScheduler.PlayEffect( "blaster2/NPCshot", cent->lerpOrigin, forward );
			break;
			case CLASS_CLAW:
			case CLASS_EARTH: 
			case CLASS_GOLEM: 
			case CLASS_GOLEM_LAVA: 
			case CLASS_REELO: 
			case CLASS_SPIDERREAVER3: 
			case CLASS_GONK: 
			case CLASS_CRYSTAL:
			case CLASS_VAMPIRE_GHOST: 
			theFxScheduler.PlayEffect( "blaster3/NPCshot", cent->lerpOrigin, forward );
			break;
			case CLASS_ALORA: 
			case CLASS_REBORN:
			case CLASS_KYLE:
			case CLASS_FLIER2: 
			case CLASS_NOGHRI:
			case CLASS_SWAMPTROOPER:
			case CLASS_FISH:
			case CLASS_GRAN:
			case CLASS_MONMOTHA: 
			case CLASS_DECEIVER: 
			case CLASS_ZEPH: 
			case CLASS_TURELIM: 
			case CLASS_RAHABIM: 
			case CLASS_VAMPIRE:   
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
			case CLASS_VAMPIRE_SARAFAN:
			case CLASS_GOLEM_DARK: 
			case CLASS_DARK: 
			case CLASS_VOID: 
			case CLASS_GOLEM_VOID:
			theFxScheduler.PlayEffect( "fire_blood/NPCshot", cent->lerpOrigin, forward );
			break;
			case CLASS_MOUSE: 
			case CLASS_ATST:
			case CLASS_SAND_CREATURE: 
			case CLASS_ELDERGOD: 
			case CLASS_TENTACLE: 
			case CLASS_CENOBITA: 
			theFxScheduler.PlayEffect( "fire_hell/NPCshot", cent->lerpOrigin, forward );
			break;
			case CLASS_TUSKEN:
			case CLASS_VAMPIRE_ZOMBIE: 
			case CLASS_GLIDER:
			case CLASS_MURJJ: 
			case CLASS_JAWA:
			case CLASS_SENTRY:
			case CLASS_RANCOR:
			case CLASS_GHOST: 
			case CLASS_SLUAGH: 
			case CLASS_REAPER: 
			case CLASS_DREADNOUGHT: 
			case CLASS_MELC: 
			case CLASS_SUMMONER: 
			case CLASS_VAMPIRE_FEAR2: 
			case CLASS_PROPHET: 
			case CLASS_POLTER:
			case CLASS_NECRO: 
			case CLASS_GOLEM_NECRO:
			theFxScheduler.PlayEffect( "fire_spectral/NPCshot", cent->lerpOrigin, forward );
			break;
			case CLASS_TAVION:
			case CLASS_DESANN: 
			case CLASS_HYLDEN: 
			case CLASS_HYLDENLORD: 
			theFxScheduler.PlayEffect( "fire_green/NPCshot", cent->lerpOrigin, forward );
			break;
			case CLASS_R2D2:
			case CLASS_R5D2:
			case CLASS_LIZARD:
			case CLASS_MINEMONSTER: 
			case CLASS_POISON: 
			case CLASS_GOLEM_POISON: 
			case CLASS_IVY: 
			case CLASS_BEAST: 
			case CLASS_BIRD:
			case CLASS_SHARK: 
			case CLASS_DEMON_POISON: 
			case CLASS_DEMON_BLADE: 
			case CLASS_DEMON_ELECTRO: 
			case CLASS_INTERROGATOR: 
			theFxScheduler.PlayEffect( "fire_poison/NPCshot", cent->lerpOrigin, forward );
			break;
			case CLASS_BARTENDER:
			case CLASS_BESPIN_COP:
			case CLASS_COMMANDO: 
			case CLASS_GALAK: 
			case CLASS_HOWLER: 
			case CLASS_IMPWORKER: 
			case CLASS_WIZARD: 
			case CLASS_JAN: 
			case CLASS_JEDI: 
			case CLASS_LANDO: 
			case CLASS_LUKE: 
			case CLASS_MARK1: 
			case CLASS_MARK2: 
			case CLASS_GALAKMECH: 
			case CLASS_MORGANKATARN: 
			case CLASS_PRISONER: 
			case CLASS_PROTOCOL: 
			case CLASS_REBEL: 
			case CLASS_REMOTE: 
			case CLASS_RODIAN: 
			case CLASS_SEEKER: 
			case CLASS_STORMTROOPER: 
			case CLASS_SWAMP:
			case CLASS_TRANDOSHAN: 
			case CLASS_UGNAUGHT: 
			case CLASS_WEEQUAY: 
			case CLASS_ROCKETTROOPER: 
			case CLASS_SABER_DROID: 
			case CLASS_ASSASSIN_DROID: 
			case CLASS_PLAYER: 
			case CLASS_VEHICLE: 
			case CLASS_HUNTER: 
			case CLASS_SCOUT:  
			case CLASS_ALCHEMIST: 
			case CLASS_DRUID: 
			case CLASS_SARAFAN: 
			case CLASS_ARCHER:  
			case CLASS_WARRIOR: 
			case CLASS_REAVER: 
			case CLASS_RAZIEL:
			case CLASS_NECROREAVER:
			case CLASS_DARKREAVER: 
			case CLASS_TANKREAVER:
			case CLASS_LIGHTREAVER:
			case CLASS_DRUIDREAVER:
			case CLASS_ICEREAVER: 
			case CLASS_SPIDERREAVER:
			case CLASS_SPIDERREAVER2:
			case CLASS_AIRREAVER: 
			case CLASS_MENTALIST: 
			case CLASS_EARTHREAVER: 
			case CLASS_THRALL: 
			case CLASS_FIRE:  
			case CLASS_WATER: 
			case CLASS_HOLY:  
			case CLASS_SONIC: 
			case CLASS_GOLEM_FIRE:
			case CLASS_GOLEM_WATER:
			case CLASS_GOLEM_ICE:
			case CLASS_GOLEM_METAL:
			case CLASS_GOLEM_SONIC:
			case CLASS_GOLEM_HOLY: 
			theFxScheduler.PlayEffect( "blaster/NPCshot", cent->lerpOrigin, forward );
			break; 
			case CLASS_FIGHTER: 
			theFxScheduler.PlayEffect( "blaster0/NPCshot", cent->lerpOrigin, forward );
			break;
		default:
			break;
			}
		}

	}
	else 
	{
		//theFxScheduler.PlayEffect( cgs.effects.blasterShotEffect, cent->lerpOrigin, forward );

		if ( cent->gent && cent->gent->owner && cent->gent->owner->client )
		{
			switch ( cent->gent->owner->client->NPC_class )
			{
			case CLASS_PROBE: 
			case CLASS_HAZARD_TROOPER:
			case CLASS_SABOTEUR: 
			case CLASS_SHADOWTROOPER: 
			case CLASS_IMPERIAL:
			case CLASS_WAMPA:
			case CLASS_BOBAFETT:
			case CLASS_DEMON_FIRE: 
			case CLASS_HELLGOLEM: 
			case CLASS_VAMPIRE_DEMON: 
			case CLASS_FIREREAVER:
			case CLASS_GOLEM_SMOKE: 
			case CLASS_AIR: 			
			case CLASS_GOLEM_AIR:
			case CLASS_LIGHT: 
			case CLASS_GOLEM_LIGHT:
			case CLASS_GOLEM_GOLD:
			case CLASS_LYCAN:  
			theFxScheduler.PlayEffect( cgs.effects.warmShotEffect, cent->lerpOrigin, forward );
			break;
			case CLASS_CLAW:
			case CLASS_EARTH: 
			case CLASS_GOLEM: 
			case CLASS_GOLEM_LAVA: 
			case CLASS_REELO: 
			case CLASS_SPIDERREAVER3: 
			case CLASS_GONK: 
			case CLASS_CRYSTAL:
			case CLASS_VAMPIRE_GHOST: 
			theFxScheduler.PlayEffect( cgs.effects.magmaShotEffect, cent->lerpOrigin, forward );
			break;
			case CLASS_ALORA: 
			case CLASS_REBORN:
			case CLASS_KYLE:
			case CLASS_FLIER2: 
			case CLASS_NOGHRI:
			case CLASS_SWAMPTROOPER:
			case CLASS_FISH:
			case CLASS_GRAN:
			case CLASS_MONMOTHA: 
			case CLASS_DECEIVER: 
			case CLASS_ZEPH: 
			case CLASS_TURELIM: 
			case CLASS_RAHABIM: 
			case CLASS_VAMPIRE:   
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
			case CLASS_VAMPIRE_SARAFAN:
			case CLASS_GOLEM_DARK: 
			case CLASS_DARK: 
			case CLASS_VOID: 
			case CLASS_GOLEM_VOID:
			theFxScheduler.PlayEffect( cgs.effects.vampfireShotEffect, cent->lerpOrigin, forward );break;
			case CLASS_MOUSE: 
			case CLASS_ATST:
			case CLASS_SAND_CREATURE: 
			case CLASS_ELDERGOD: 
			case CLASS_TENTACLE: 
			case CLASS_CENOBITA: 
			theFxScheduler.PlayEffect( cgs.effects.hellfireShotEffect, cent->lerpOrigin, forward );
			break;
			case CLASS_TUSKEN:
			case CLASS_VAMPIRE_ZOMBIE: 
			case CLASS_GLIDER:
			case CLASS_MURJJ: 
			case CLASS_JAWA:
			case CLASS_SENTRY:
			case CLASS_RANCOR:
			case CLASS_GHOST: 
			case CLASS_SLUAGH: 
			case CLASS_REAPER: 
			case CLASS_DREADNOUGHT: 
			case CLASS_MELC: 
			case CLASS_SUMMONER: 
			case CLASS_VAMPIRE_FEAR2: 
			case CLASS_PROPHET: 
			case CLASS_POLTER:
			case CLASS_NECRO: 
			case CLASS_GOLEM_NECRO:
			theFxScheduler.PlayEffect( cgs.effects.ghostfireShotEffect, cent->lerpOrigin, forward );break;
			case CLASS_TAVION:
			case CLASS_DESANN: 
			case CLASS_HYLDEN: 
			case CLASS_HYLDENLORD: 
			theFxScheduler.PlayEffect( cgs.effects.greenfireShotEffect, cent->lerpOrigin, forward );
			break;
			case CLASS_R2D2:
			case CLASS_R5D2:
			case CLASS_LIZARD:
			case CLASS_MINEMONSTER: 
			case CLASS_POISON: 
			case CLASS_GOLEM_POISON: 
			case CLASS_IVY: 
			case CLASS_BEAST: 
			case CLASS_BIRD:
			case CLASS_SHARK: 
			case CLASS_DEMON_POISON: 
			case CLASS_DEMON_BLADE: 
			case CLASS_DEMON_ELECTRO: 
			case CLASS_INTERROGATOR: 
			theFxScheduler.PlayEffect( cgs.effects.poisonfireShotEffect, cent->lerpOrigin, forward );
			break;
			case CLASS_BARTENDER:
			case CLASS_BESPIN_COP:
			case CLASS_COMMANDO: 
			case CLASS_GALAK: 
			case CLASS_HOWLER: 
			case CLASS_IMPWORKER: 
			case CLASS_WIZARD: 
			case CLASS_JAN: 
			case CLASS_JEDI: 
			case CLASS_LANDO: 
			case CLASS_LUKE: 
			case CLASS_MARK1: 
			case CLASS_MARK2: 
			case CLASS_GALAKMECH: 
			case CLASS_MORGANKATARN: 
			case CLASS_PRISONER: 
			case CLASS_PROTOCOL: 
			case CLASS_REBEL: 
			case CLASS_REMOTE: 
			case CLASS_RODIAN: 
			case CLASS_SEEKER: 
			case CLASS_STORMTROOPER: 
			case CLASS_SWAMP:
			case CLASS_TRANDOSHAN: 
			case CLASS_UGNAUGHT: 
			case CLASS_WEEQUAY: 
			case CLASS_ROCKETTROOPER: 
			case CLASS_SABER_DROID: 
			case CLASS_ASSASSIN_DROID: 
			case CLASS_PLAYER: 
			case CLASS_VEHICLE: 
			case CLASS_HUNTER: 
			case CLASS_SCOUT:  
			case CLASS_ALCHEMIST: 
			case CLASS_DRUID: 
			case CLASS_SARAFAN: 
			case CLASS_ARCHER:  
			case CLASS_WARRIOR: 
			case CLASS_REAVER: 
			case CLASS_RAZIEL:
			case CLASS_NECROREAVER:
			case CLASS_DARKREAVER: 
			case CLASS_TANKREAVER:
			case CLASS_LIGHTREAVER:
			case CLASS_DRUIDREAVER:
			case CLASS_ICEREAVER: 
			case CLASS_SPIDERREAVER:
			case CLASS_SPIDERREAVER2:
			case CLASS_AIRREAVER: 
			case CLASS_MENTALIST: 
			case CLASS_EARTHREAVER: 
			case CLASS_THRALL: 
			case CLASS_FIRE:  
			case CLASS_WATER: 
			case CLASS_HOLY:  
			case CLASS_SONIC: 
			case CLASS_GOLEM_FIRE:
			case CLASS_GOLEM_WATER:
			case CLASS_GOLEM_ICE:
			case CLASS_GOLEM_METAL:
			case CLASS_GOLEM_SONIC:
			case CLASS_GOLEM_HOLY: 
			theFxScheduler.PlayEffect( cgs.effects.blasterShotEffect, cent->lerpOrigin, forward );
			break; 
			case CLASS_FIGHTER: 
			theFxScheduler.PlayEffect( cgs.effects.greenlaser0ShotEffect, cent->lerpOrigin, forward );
			break;
		default:
			break;
			}
		}
		//theFxScheduler.PlayEffect( cgs.effects.blasterShotEffect, cent->lerpOrigin, forward );
	}
}

/*
-------------------------
FX_BlasterAltFireThink
-------------------------
*/
void FX_BlasterAltFireThink( centity_t *cent, const struct weaponInfo_s *weapon )
{
	FX_BlasterProjectileThink( cent, weapon );
}

/*
-------------------------
FX_BlasterWeaponHitWall
-------------------------
*/
void FX_BlasterWeaponHitWall( vec3_t origin, vec3_t normal )
{
		theFxScheduler.PlayEffect( cgs.effects.blasterWallImpactEffect, origin, normal );
}

void FX_WarmHitWall( vec3_t origin, vec3_t normal )
{
	
		theFxScheduler.PlayEffect( cgs.effects.warmWallImpactEffect, origin, normal );
}

void FX_MagmaHitWall( vec3_t origin, vec3_t normal )
{
	
		theFxScheduler.PlayEffect( cgs.effects.magmaWallImpactEffect, origin, normal );
}

void FX_VampFireHitWall( vec3_t origin, vec3_t normal )
{
		theFxScheduler.PlayEffect( cgs.effects.vampfireWallImpactEffect, origin, normal );
}

void FX_GreenFireHitWall( vec3_t origin, vec3_t normal )
{
	
		theFxScheduler.PlayEffect( cgs.effects.greenfireWallImpactEffect, origin, normal );
}

void FX_HellFireHitWall( vec3_t origin, vec3_t normal )
{
	
		theFxScheduler.PlayEffect( cgs.effects.hellfireWallImpactEffect, origin, normal );
}

void FX_GhostFireHitWall( vec3_t origin, vec3_t normal )
{
	
		theFxScheduler.PlayEffect( cgs.effects.ghostfireWallImpactEffect, origin, normal );
}

void FX_PoisonFireHitWall( vec3_t origin, vec3_t normal )
{
	
		theFxScheduler.PlayEffect( cgs.effects.poisonfireWallImpactEffect, origin, normal );
}

void FX_GreenLaserHitWall( vec3_t origin, vec3_t normal )
{
		theFxScheduler.PlayEffect( cgs.effects.greenlaser0WallImpactEffect, origin, normal );
}

/*
-------------------------
FX_BlasterWeaponHitPlayer
-------------------------
*/
void FX_BlasterWeaponHitPlayer( gentity_t *hit, vec3_t origin, vec3_t normal, qboolean humanoid )
{
	//temporary? just testing out the damage skin stuff -rww
	if ( hit && hit->client && hit->ghoul2.size() )
	{
	//	theFxScheduler.PlayEffect( cgs.effects.blasterFleshImpactEffect, origin, normal );
		CG_AddGhoul2Mark(cgs.media.bdecal_burnmark1, flrand(3.5, 4.0), origin, normal, hit->s.number,
		hit->client->ps.origin, hit->client->renderInfo.legsYaw, hit->ghoul2, hit->s.modelScale, Q_irand(10000, 13000));
	}
        
	theFxScheduler.PlayEffect( cgs.effects.blasterFleshImpactEffect, origin, normal );
}

void FX_WarmHitPlayer( gentity_t *hit, vec3_t origin, vec3_t normal, qboolean humanoid )
{
	//temporary? just testing out the damage skin stuff -rww
	if ( hit && hit->client && hit->ghoul2.size() )
	{
	//	theFxScheduler.PlayEffect( cgs.effects.blasterFleshImpactEffect, origin, normal );
		CG_AddGhoul2Mark(cgs.media.bdecal_burnmark1, flrand(3.5, 4.0), origin, normal, hit->s.number,
		hit->client->ps.origin, hit->client->renderInfo.legsYaw, hit->ghoul2, hit->s.modelScale, Q_irand(10000, 13000));
	}
        
	theFxScheduler.PlayEffect( cgs.effects.warmFleshImpactEffect, origin, normal );
}

void FX_MagmaHitPlayer( gentity_t *hit, vec3_t origin, vec3_t normal, qboolean humanoid )
{
	//temporary? just testing out the damage skin stuff -rww
	if ( hit && hit->client && hit->ghoul2.size() )
	{
	//	theFxScheduler.PlayEffect( cgs.effects.blasterFleshImpactEffect, origin, normal );
		CG_AddGhoul2Mark(cgs.media.bdecal_burnmark1, flrand(3.5, 4.0), origin, normal, hit->s.number,
		hit->client->ps.origin, hit->client->renderInfo.legsYaw, hit->ghoul2, hit->s.modelScale, Q_irand(10000, 13000));
	}
        
	theFxScheduler.PlayEffect( cgs.effects.magmaFleshImpactEffect, origin, normal );
}

void FX_VampFireHitPlayer( gentity_t *hit, vec3_t origin, vec3_t normal, qboolean humanoid )
{
	//temporary? just testing out the damage skin stuff -rww
	if ( hit && hit->client && hit->ghoul2.size() )
	{
	//	theFxScheduler.PlayEffect( cgs.effects.blasterFleshImpactEffect, origin, normal );
		CG_AddGhoul2Mark(cgs.media.bdecal_burnmark1, flrand(3.5, 4.0), origin, normal, hit->s.number,
		hit->client->ps.origin, hit->client->renderInfo.legsYaw, hit->ghoul2, hit->s.modelScale, Q_irand(10000, 13000));
	}
        
	theFxScheduler.PlayEffect( cgs.effects.vampfireFleshImpactEffect, origin, normal );
}

void FX_GreenFireHitPlayer( gentity_t *hit, vec3_t origin, vec3_t normal, qboolean humanoid )
{
	//temporary? just testing out the damage skin stuff -rww
	if ( hit && hit->client && hit->ghoul2.size() )
	{
	//	theFxScheduler.PlayEffect( cgs.effects.blasterFleshImpactEffect, origin, normal );
		CG_AddGhoul2Mark(cgs.media.bdecal_burnmark1, flrand(3.5, 4.0), origin, normal, hit->s.number,
		hit->client->ps.origin, hit->client->renderInfo.legsYaw, hit->ghoul2, hit->s.modelScale, Q_irand(10000, 13000));
	}
        
	theFxScheduler.PlayEffect( cgs.effects.greenfireFleshImpactEffect, origin, normal );
}

void FX_HellFireHitPlayer( gentity_t *hit, vec3_t origin, vec3_t normal, qboolean humanoid )
{
	//temporary? just testing out the damage skin stuff -rww
	if ( hit && hit->client && hit->ghoul2.size() )
	{
	//	theFxScheduler.PlayEffect( cgs.effects.blasterFleshImpactEffect, origin, normal );
		CG_AddGhoul2Mark(cgs.media.bdecal_burnmark1, flrand(3.5, 4.0), origin, normal, hit->s.number,
		hit->client->ps.origin, hit->client->renderInfo.legsYaw, hit->ghoul2, hit->s.modelScale, Q_irand(10000, 13000));
	}
        
	theFxScheduler.PlayEffect( cgs.effects.hellfireFleshImpactEffect, origin, normal );
}

void FX_PoisonFireHitPlayer( gentity_t *hit, vec3_t origin, vec3_t normal, qboolean humanoid )
{
	theFxScheduler.PlayEffect( cgs.effects.poisonfireFleshImpactEffect, origin, normal );
}

void FX_GhostFireHitPlayer( gentity_t *hit, vec3_t origin, vec3_t normal, qboolean humanoid )
{
	theFxScheduler.PlayEffect( cgs.effects.ghostfireFleshImpactEffect, origin, normal );
}
void FX_GreenLaserHitPlayer( gentity_t *hit, vec3_t origin, vec3_t normal, qboolean humanoid )
{
	theFxScheduler.PlayEffect( cgs.effects.greenlaser0FleshImpactEffect, origin, normal );
}