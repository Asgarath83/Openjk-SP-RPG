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

// Golan Arms Flechette Weapon

// this line must stay at top so the whole PCH thing works...
#include "cg_headers.h"

//#include "cg_local.h"
#include "cg_media.h"
#include "FxScheduler.h"

/*
-------------------------
FX_FlechetteProjectileThink
-------------------------
*/

void FX_FlechetteProjectileThink( centity_t *cent, const struct weaponInfo_s *weapon )
{
	vec3_t forward;

	EvaluateTrajectoryDelta( &cent->gent->s.pos, cg.time, forward );

	if ( VectorNormalize( forward ) == 0.0f )
	{
		forward[2] = 1.0f;
	}

	if ( cent->gent && cent->gent->owner && cent->gent->owner->client )
		 {
			switch ( cent->gent->owner->client->NPC_class )
			{
					// FANGO
				case CLASS_UGNAUGHT:
				case CLASS_WATER: 
				case CLASS_GOLEM_WATER: 
				case CLASS_SHARK: 
				case CLASS_REELO: 
				case CLASS_LYCAN:
				case CLASS_SPIDERREAVER3: 	
				theFxScheduler.PlayEffect( cgs.effects.mudShotEffect, cent->lerpOrigin, forward );
				break;
				// PIETRA 
				case CLASS_EARTHREAVER: 
				case CLASS_LIZARD:
				case CLASS_DEMON_BLADE: 
				theFxScheduler.PlayEffect( cgs.effects.stoneShotEffect, cent->lerpOrigin, forward );
				break;
				// ACIDO
				case CLASS_INTERROGATOR: 
				case CLASS_MINEMONSTER:
				case CLASS_POISON: 
				case CLASS_BEAST: 
				case CLASS_GOLEM_POISON:
				case CLASS_DEMON_POISON: 
				case CLASS_R2D2:
				case CLASS_R5D2: 
				theFxScheduler.PlayEffect( cgs.effects.acidShotEffect, cent->lerpOrigin, forward );
				break;
				// IVY
				case CLASS_PRISONER: 
				case CLASS_SENTRY:
				case CLASS_IVY: 
				theFxScheduler.PlayEffect( cgs.effects.ivyShotEffect, cent->lerpOrigin, forward );
				break;
				// HELLISHIVY
				case CLASS_CENOBITA: 
				case CLASS_PROPHET: 
				case CLASS_TAVION: 
				case CLASS_DESANN:
				case CLASS_HYLDEN: 
				case CLASS_HYLDENLORD: 
				theFxScheduler.PlayEffect( cgs.effects.ivy2ShotEffect, cent->lerpOrigin, forward );
				break;
				// THORN 
				case CLASS_SWAMP: 
				theFxScheduler.PlayEffect( cgs.effects.thornShotEffect, cent->lerpOrigin, forward );
				break;
				// SPECTRAL
				case CLASS_ARIEL: 
				case CLASS_TUSKEN: 
				case CLASS_GLIDER:
				case CLASS_MELC: 
				case CLASS_SUMMONER: 
				case CLASS_SLUAGH: 
				case CLASS_DREADNOUGHT: 
				case CLASS_REAPER: 
				case CLASS_VAMPIRE_GHOST: 
				case CLASS_RANCOR:
				case CLASS_JAWA: 
				case CLASS_ATST:
				case CLASS_SAND_CREATURE:
				case CLASS_MOUSE: 
				case CLASS_GHOST: 
				case CLASS_ELDERGOD: 
				case CLASS_TENTACLE: 
				case CLASS_NECRO: 
				case CLASS_GOLEM_NECRO: 
				case CLASS_FISH: 
				theFxScheduler.PlayEffect( cgs.effects.spectralShotEffect, cent->lerpOrigin, forward );
				break;
				// STASIS
				case CLASS_ZEPH:
				case CLASS_DECEIVER:
				case CLASS_DUMAHIM: 
				case CLASS_RAHABIM:
				case CLASS_VAMPIRE:
				case CLASS_VAMPIRE_DEMON:
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
				case CLASS_VAMPIRE_WATER: 
				case CLASS_VAMPIRE_FROST:  
				case CLASS_VAMPIRE_ARCHER: 
				case CLASS_HALFVAMPIRE: 
				case CLASS_RAZIELHIM:
				case CLASS_ESKANDOR: 
				case CLASS_VAMPIRE_BOUNTY: 
				case CLASS_VAMPIRE_ZOMBIE: 
				case CLASS_VAMPIRE_SARAFAN:  
				case CLASS_REBORN: 
				case CLASS_KYLE:
				case CLASS_ALORA:
				case CLASS_NOGHRI:
				case CLASS_FLIER2:
				case CLASS_SWAMPTROOPER: 
				case CLASS_GRAN:
				case CLASS_JAN: 
				case CLASS_GALAK: 
				case CLASS_MARK1:
				case CLASS_MARK2:
				case CLASS_SARAFAN: 
				case CLASS_ARCHER:  
				case CLASS_WARRIOR: 
				case CLASS_HUNTER: 
				case CLASS_SCOUT:  
				case CLASS_ALCHEMIST: 
				theFxScheduler.PlayEffect( cgs.effects.stasisShotEffect, cent->lerpOrigin, forward );
				break;
				// DUST
				case CLASS_SABOTEUR: 
				case CLASS_DARK: 
				case CLASS_GOLEM_DARK: 
				case CLASS_VOID: 
				case CLASS_GOLEM_VOID: 
				case CLASS_DARKREAVER: 
				case CLASS_SHADOWTROOPER:
				case CLASS_PROBE:
				case CLASS_POLTER: 
				case CLASS_AIR: 
				case CLASS_GOLEM_AIR: 
				case CLASS_ROCKETTROOPER: 
				case CLASS_BOBAFETT: 
				case CLASS_SPIDERREAVER2: 
				case CLASS_THRALL: 
				case CLASS_BIRD: 
				theFxScheduler.PlayEffect( cgs.effects.dustShotEffect, cent->lerpOrigin, forward );
				break;
				// ASH
				case CLASS_IMPERIAL: 
				case CLASS_BESPIN_COP:
				case CLASS_HAZARD_TROOPER:
				case CLASS_CLAW: 
				case CLASS_WAMPA: 
				case CLASS_TURELIM: 
				case CLASS_FIREREAVER: 
				case CLASS_TANKREAVER: 
				case CLASS_FIRE: 
				case CLASS_GOLEM_FIRE: 
				case CLASS_HELLGOLEM: 
				case CLASS_GOLEM_LAVA: 
				case CLASS_DEMON_FIRE: 
				case CLASS_GOLEM_SMOKE: 
				theFxScheduler.PlayEffect( cgs.effects.ashShotEffect, cent->lerpOrigin, forward );
				break;
				// GCRYSTAL 
				case CLASS_GALAKMECH: 
				case CLASS_SABER_DROID:
				case CLASS_PROTOCOL: 
				case CLASS_CRYSTAL: 
				case CLASS_LIGHT: 
				case CLASS_GOLEM_LIGHT: 
				case CLASS_GOLEM_GOLD: 
				case CLASS_GOLEM_ICE:
				case CLASS_GOLEM_METAL:
				case CLASS_DEMON_ELECTRO: 
				theFxScheduler.PlayEffect( cgs.effects.gcrystalShotEffect, cent->lerpOrigin, forward );
				break;
				// DEFAULT 
				case CLASS_DRUID: 
				case CLASS_VAMPIRE_ELF: 
				case CLASS_VAMPIRE_ELF2:
				case CLASS_VAMPIRE_ELF3:
				case CLASS_REAVER: 
				case CLASS_RAZIEL:
				case CLASS_NECROREAVER:
				case CLASS_LIGHTREAVER:
				case CLASS_DRUIDREAVER:
				case CLASS_ICEREAVER: 
				case CLASS_SPIDERREAVER:
				case CLASS_AIRREAVER: 
				case CLASS_MENTALIST: 
				case CLASS_HOLY:  
				case CLASS_SONIC: 
				case CLASS_GOLEM:
				case CLASS_GOLEM_SONIC:
				case CLASS_GOLEM_HOLY: 				 
				case CLASS_COMMANDO: 
				case CLASS_STORMTROOPER: 
				case CLASS_GONK:
				case CLASS_HOWLER: 
				case CLASS_IMPWORKER: 
				case CLASS_WIZARD: 
				case CLASS_JEDI: 
				case CLASS_LANDO: 
				case CLASS_WEEQUAY: 
				case CLASS_LUKE: 
				case CLASS_MONMOTHA: 
				case CLASS_MORGANKATARN: 
				case CLASS_MURJJ: 
				case CLASS_REBEL: 
				case CLASS_REMOTE: 
				case CLASS_RODIAN: 
				case CLASS_SEEKER: 
				case CLASS_TRANDOSHAN: 
				case CLASS_BARTENDER: 
				case CLASS_ASSASSIN_DROID: 
				case CLASS_PLAYER: 
				case CLASS_VEHICLE: 
				theFxScheduler.PlayEffect( cgs.effects.flechetteShotEffect, cent->lerpOrigin, forward );
				break;
				case CLASS_FIGHTER: 
				theFxScheduler.PlayEffect( cgs.effects.shrapnel0ShotEffect, cent->lerpOrigin, forward );
				break;
			default:
				break;
				}
		}
// theFxScheduler.PlayEffect( cgs.effects.flechetteShotEffect, cent->lerpOrigin, forward );*/
}

void FX_CannonProjectileThink( centity_t *cent, const struct weaponInfo_s *weapon )
{
	vec3_t forward;

	EvaluateTrajectoryDelta( &cent->gent->s.pos, cg.time, forward );

	if ( VectorNormalize( forward ) == 0.0f )
	{
		forward[2] = 1.0f;
	}

	theFxScheduler.PlayEffect( cgs.effects.cannonShotEffect, cent->lerpOrigin, forward );
}

/*
-------------------------
FX_FlechetteWeaponHitWall
-------------------------
*/
void FX_FlechetteWeaponHitWall( vec3_t origin, vec3_t normal )
{
	theFxScheduler.PlayEffect( cgs.effects.flechetteShotDeathEffect, origin, normal );
}

void FX_MudHitWall( vec3_t origin, vec3_t normal )
{
	theFxScheduler.PlayEffect( cgs.effects.mudShotDeathEffect, origin, normal );
}

void FX_StoneHitWall( vec3_t origin, vec3_t normal )
{
	theFxScheduler.PlayEffect( cgs.effects.stoneShotDeathEffect, origin, normal );
}

void FX_CannonWeaponHitWall( vec3_t origin, vec3_t normal )
{
	theFxScheduler.PlayEffect( cgs.effects.cannonShotDeathEffect, origin, normal );
}

void FX_DustHitWall( vec3_t origin, vec3_t normal )
{
//	theFxScheduler.PlayEffect( cgs.effects.dustShotDeathEffect, origin, normal );
}

void FX_AshHitWall( vec3_t origin, vec3_t normal )
{
	theFxScheduler.PlayEffect( cgs.effects.ashShotDeathEffect, origin, normal );
}

void FX_IvyHitWall( vec3_t origin, vec3_t normal )
{
	theFxScheduler.PlayEffect( cgs.effects.ivyShotDeathEffect, origin, normal );
}

void FX_Ivy2HitWall( vec3_t origin, vec3_t normal )
{
	theFxScheduler.PlayEffect( cgs.effects.ivy2ShotDeathEffect, origin, normal );
}

void FX_ThornHitWall( vec3_t origin, vec3_t normal )
{
	theFxScheduler.PlayEffect( cgs.effects.thornShotDeathEffect, origin, normal );
}

void FX_SpectralHitWall( vec3_t origin, vec3_t normal )
{
	theFxScheduler.PlayEffect( cgs.effects.spectralShotDeathEffect, origin, normal );
}

void FX_AcidHitWall( vec3_t origin, vec3_t normal )
{
	theFxScheduler.PlayEffect( cgs.effects.acidShotDeathEffect, origin, normal );
}


void FX_StasisHitWall( vec3_t origin, vec3_t normal )
{
	theFxScheduler.PlayEffect( cgs.effects.stasisShotDeathEffect, origin, normal );
}

void FX_GcrystalHitWall( vec3_t origin, vec3_t normal )
{
	theFxScheduler.PlayEffect( cgs.effects.gcrystalShotDeathEffect, origin, normal );
}

void FX_ShrapnelHitWall( vec3_t origin, vec3_t normal )
{
	theFxScheduler.PlayEffect( cgs.effects.shrapnel0ShotDeathEffect, origin, normal );
}

/*
-------------------------
FX_BlasterWeaponHitPlayer
-------------------------
*/
void FX_FlechetteWeaponHitPlayer( vec3_t origin, vec3_t normal, qboolean humanoid )
{
//	if ( humanoid )
//	{
		theFxScheduler.PlayEffect( cgs.effects.flechetteFleshImpactEffect, origin, normal );
//	}
//	else
//	{
//		theFxScheduler.PlayEffect( "blaster/droid_impact", origin, normal );
//	}
}

void FX_MudHitPlayer( vec3_t origin, vec3_t normal, qboolean humanoid )
{
//	if ( humanoid )
//	{
		theFxScheduler.PlayEffect( cgs.effects.mudFleshImpactEffect, origin, normal );
//	}
//	else
//	{
//		theFxScheduler.PlayEffect( "blaster/droid_impact", origin, normal );
//	}
}

void FX_StoneHitPlayer( vec3_t origin, vec3_t normal, qboolean humanoid )
{
//	if ( humanoid )
//	{
		theFxScheduler.PlayEffect( cgs.effects.stoneFleshImpactEffect, origin, normal );
//	}
//	else
//	{
//		theFxScheduler.PlayEffect( "blaster/droid_impact", origin, normal );
//	}
}

void FX_CannonWeaponHitPlayer( vec3_t origin, vec3_t normal, qboolean humanoid )
{
//	if ( humanoid )
//	{
		theFxScheduler.PlayEffect( cgs.effects.cannonFleshImpactEffect, origin, normal );
//	}
//	else
//	{
//		theFxScheduler.PlayEffect( "blaster/droid_impact", origin, normal );
//	}
}

void FX_GcrystalHitPlayer( vec3_t origin, vec3_t normal, qboolean humanoid )
{
//	if ( humanoid )
//	{
		theFxScheduler.PlayEffect( cgs.effects.gcrystalFleshImpactEffect, origin, normal );
//	}
//	else
//	{
//		theFxScheduler.PlayEffect( "blaster/droid_impact", origin, normal );
//	}
}

void FX_IvyHitPlayer( vec3_t origin, vec3_t normal, qboolean humanoid )
{
//	if ( humanoid )
//	{
		theFxScheduler.PlayEffect( cgs.effects.ivyFleshImpactEffect, origin, normal );
//	}
//	else
//	{
//		theFxScheduler.PlayEffect( "blaster/droid_impact", origin, normal );
//	}
}

void FX_Ivy2HitPlayer( vec3_t origin, vec3_t normal, qboolean humanoid )
{
//	if ( humanoid )
//	{
		theFxScheduler.PlayEffect( cgs.effects.ivy2FleshImpactEffect, origin, normal );
//	}
//	else
//	{
//		theFxScheduler.PlayEffect( "blaster/droid_impact", origin, normal );
//	}
}

void FX_AcidHitPlayer( vec3_t origin, vec3_t normal, qboolean humanoid )
{
//	if ( humanoid )
//	{
		theFxScheduler.PlayEffect( cgs.effects.acidFleshImpactEffect, origin, normal );
//	}
//	else
//	{
//		theFxScheduler.PlayEffect( "blaster/droid_impact", origin, normal );
//	}
}

void FX_AshHitPlayer( vec3_t origin, vec3_t normal, qboolean humanoid )
{
//	if ( humanoid )
//	{
		theFxScheduler.PlayEffect( cgs.effects.ashFleshImpactEffect, origin, normal );
//	}
//	else
//	{
//		theFxScheduler.PlayEffect( "blaster/droid_impact", origin, normal );
//	}
}

void FX_DustHitPlayer( vec3_t origin, vec3_t normal, qboolean humanoid )
{
//	if ( humanoid )
//	{
		theFxScheduler.PlayEffect( cgs.effects.dustFleshImpactEffect, origin, normal );
//	}
//	else
//	{
//		theFxScheduler.PlayEffect( "blaster/droid_impact", origin, normal );
//	}
}

void FX_SpectralHitPlayer( vec3_t origin, vec3_t normal, qboolean humanoid )
{
//	if ( humanoid )
//	{
		theFxScheduler.PlayEffect( cgs.effects.spectralFleshImpactEffect, origin, normal );
//	}
//	else
//	{
//		theFxScheduler.PlayEffect( "blaster/droid_impact", origin, normal );
//	}
}

void FX_StasisHitPlayer( vec3_t origin, vec3_t normal, qboolean humanoid )
{
//	if ( humanoid )
//	{
		theFxScheduler.PlayEffect( cgs.effects.stasisFleshImpactEffect, origin, normal );
//	}
//	else
//	{
//		theFxScheduler.PlayEffect( "blaster/droid_impact", origin, normal );
//	}
}

void FX_ThornHitPlayer( vec3_t origin, vec3_t normal, qboolean humanoid )
{
//	if ( humanoid )
//	{
		theFxScheduler.PlayEffect( cgs.effects.thornFleshImpactEffect, origin, normal );
//	}
//	else
//	{
//		theFxScheduler.PlayEffect( "blaster/droid_impact", origin, normal );
//	}
}

void FX_ShrapnelHitPlayer( vec3_t origin, vec3_t normal, qboolean humanoid )
{
//	if ( humanoid )
//	{
		theFxScheduler.PlayEffect( cgs.effects.shrapnel0FleshImpactEffect, origin, normal );
//	}
//	else
//	{
//		theFxScheduler.PlayEffect( "blaster/droid_impact", origin, normal );
//	}
}

/*
-------------------------
FX_FlechetteAltProjectileThink
-------------------------
*/

void FX_FlechetteAltProjectileThink( centity_t *cent, const struct weaponInfo_s *weapon )
{
	vec3_t forward;

	if ( VectorNormalize2( cent->currentState.pos.trDelta, forward ) == 0.0f )
	{
		forward[2] = 1.0f;
	}

	// theFxScheduler.PlayEffect( cgs.effects.flechetteAltShotEffect, cent->lerpOrigin, forward );

	if ( cent->gent && cent->gent->owner && cent->gent->owner->client )
		 {
			switch ( cent->gent->owner->client->NPC_class )
			{
					// FANGO
				case CLASS_UGNAUGHT:
				case CLASS_WATER: 
				case CLASS_GOLEM_WATER: 
				case CLASS_SHARK: 
				case CLASS_REELO: 
				case CLASS_LYCAN:
				case CLASS_SPIDERREAVER3: 	
				theFxScheduler.PlayEffect( cgs.effects.mudAltShotEffect, cent->lerpOrigin, forward );
				break;
				// PIETRA 
				case CLASS_EARTHREAVER: 
				case CLASS_LIZARD:
				case CLASS_DEMON_BLADE: 
				theFxScheduler.PlayEffect( cgs.effects.stoneAltShotEffect, cent->lerpOrigin, forward );
				break;
				// ACIDO
				case CLASS_INTERROGATOR: 
				case CLASS_MINEMONSTER:
				case CLASS_POISON: 
				case CLASS_BEAST: 
				case CLASS_GOLEM_POISON:
				case CLASS_DEMON_POISON: 
				case CLASS_R2D2:
				case CLASS_R5D2: 
				theFxScheduler.PlayEffect( cgs.effects.acidAltShotEffect, cent->lerpOrigin, forward );
				break;
				// IVY
				case CLASS_PRISONER: 
				case CLASS_SENTRY:
				case CLASS_IVY: 
				theFxScheduler.PlayEffect( cgs.effects.ivyAltShotEffect, cent->lerpOrigin, forward );
				break;
				// HELLISHIVY
				case CLASS_CENOBITA: 
				case CLASS_PROPHET: 
				case CLASS_TAVION: 
				case CLASS_DESANN:
				case CLASS_HYLDEN: 
				case CLASS_HYLDENLORD: 
				theFxScheduler.PlayEffect( cgs.effects.ivy2AltShotEffect, cent->lerpOrigin, forward );
				break;
				// THORN 
				case CLASS_SWAMP: 
				theFxScheduler.PlayEffect( cgs.effects.thornAltShotEffect, cent->lerpOrigin, forward );
				break;
				// SPECTRAL
				case CLASS_ARIEL: 
				case CLASS_TUSKEN: 
				case CLASS_GLIDER:
				case CLASS_MELC: 
				case CLASS_SUMMONER: 
				case CLASS_SLUAGH: 
				case CLASS_DREADNOUGHT: 
				case CLASS_REAPER: 
				case CLASS_VAMPIRE_GHOST: 
				case CLASS_RANCOR:
				case CLASS_JAWA: 
				case CLASS_ATST:
				case CLASS_SAND_CREATURE:
				case CLASS_MOUSE: 
				case CLASS_GHOST: 
				case CLASS_ELDERGOD: 
				case CLASS_TENTACLE: 
				case CLASS_NECRO: 
				case CLASS_GOLEM_NECRO: 
				case CLASS_FISH: 
				theFxScheduler.PlayEffect( cgs.effects.spectralAltShotEffect, cent->lerpOrigin, forward );
				break;
				// STASIS
				case CLASS_ZEPH:
				case CLASS_DECEIVER:
				case CLASS_DUMAHIM: 
				case CLASS_RAHABIM:
				case CLASS_VAMPIRE:
				case CLASS_VAMPIRE_DEMON:
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
				case CLASS_VAMPIRE_WATER: 
				case CLASS_VAMPIRE_FROST:  
				case CLASS_VAMPIRE_ARCHER: 
				case CLASS_HALFVAMPIRE: 
				case CLASS_RAZIELHIM:
				case CLASS_ESKANDOR: 
				case CLASS_VAMPIRE_BOUNTY: 
				case CLASS_VAMPIRE_ZOMBIE: 
				case CLASS_VAMPIRE_SARAFAN:  
				case CLASS_REBORN: 
				case CLASS_KYLE:
				case CLASS_ALORA:
				case CLASS_NOGHRI:
				case CLASS_FLIER2:
				case CLASS_SWAMPTROOPER: 
				case CLASS_GRAN:
				case CLASS_JAN: 
				case CLASS_GALAK: 
				case CLASS_MARK1:
				case CLASS_MARK2:
				case CLASS_SARAFAN: 
				case CLASS_ARCHER:  
				case CLASS_WARRIOR: 
				case CLASS_HUNTER: 
				case CLASS_SCOUT:  
				case CLASS_ALCHEMIST: 
				theFxScheduler.PlayEffect( cgs.effects.stasisAltShotEffect, cent->lerpOrigin, forward );
				break;
				// DUST
				case CLASS_SABOTEUR: 
				case CLASS_DARK: 
				case CLASS_GOLEM_DARK: 
				case CLASS_VOID: 
				case CLASS_GOLEM_VOID: 
				case CLASS_DARKREAVER: 
				case CLASS_SHADOWTROOPER:
				case CLASS_PROBE:
				case CLASS_POLTER: 
				case CLASS_AIR: 
				case CLASS_GOLEM_AIR: 
				case CLASS_ROCKETTROOPER: 
				case CLASS_BOBAFETT: 
				case CLASS_SPIDERREAVER2: 
				case CLASS_THRALL: 
				case CLASS_BIRD: 
//				theFxScheduler.PlayEffect( cgs.effects.dustAltShotEffect, cent->lerpOrigin, forward );
				break;
				// ASH
				case CLASS_IMPERIAL: 
				case CLASS_BESPIN_COP:
				case CLASS_HAZARD_TROOPER:
				case CLASS_CLAW: 
				case CLASS_WAMPA: 
				case CLASS_TURELIM: 
				case CLASS_FIREREAVER: 
				case CLASS_TANKREAVER: 
				case CLASS_FIRE: 
				case CLASS_GOLEM_FIRE: 
				case CLASS_HELLGOLEM: 
				case CLASS_GOLEM_LAVA: 
				case CLASS_DEMON_FIRE: 
				case CLASS_GOLEM_SMOKE: 
				theFxScheduler.PlayEffect( cgs.effects.ashAltShotEffect, cent->lerpOrigin, forward );
				break;
				// GCRYSTAL 
				case CLASS_GALAKMECH: 
				case CLASS_SABER_DROID:
				case CLASS_PROTOCOL: 
				case CLASS_CRYSTAL: 
				case CLASS_LIGHT: 
				case CLASS_GOLEM_LIGHT: 
				case CLASS_GOLEM_GOLD: 
				case CLASS_GOLEM_ICE:
				case CLASS_GOLEM_METAL:
				case CLASS_DEMON_ELECTRO: 
				theFxScheduler.PlayEffect( cgs.effects.gcrystalAltShotEffect, cent->lerpOrigin, forward );
				break;
				// DEFAULT 
				case CLASS_DRUID: 
				case CLASS_VAMPIRE_ELF: 
				case CLASS_VAMPIRE_ELF2:
				case CLASS_VAMPIRE_ELF3:
				case CLASS_REAVER: 
				case CLASS_RAZIEL:
				case CLASS_NECROREAVER:
				case CLASS_LIGHTREAVER:
				case CLASS_DRUIDREAVER:
				case CLASS_ICEREAVER: 
				case CLASS_SPIDERREAVER:
				case CLASS_AIRREAVER: 
				case CLASS_MENTALIST: 
				case CLASS_HOLY:  
				case CLASS_SONIC: 
				case CLASS_GOLEM:
				case CLASS_GOLEM_SONIC:
				case CLASS_GOLEM_HOLY: 				 
				case CLASS_COMMANDO: 
				case CLASS_STORMTROOPER: 
				case CLASS_GONK:
				case CLASS_HOWLER: 
				case CLASS_IMPWORKER: 
				case CLASS_WIZARD: 
				case CLASS_JEDI: 
				case CLASS_LANDO: 
				case CLASS_WEEQUAY: 
				case CLASS_LUKE: 
				case CLASS_MONMOTHA: 
				case CLASS_MORGANKATARN: 
				case CLASS_MURJJ: 
				case CLASS_REBEL: 
				case CLASS_REMOTE: 
				case CLASS_RODIAN: 
				case CLASS_SEEKER: 
				case CLASS_TRANDOSHAN: 
				case CLASS_BARTENDER: 
				case CLASS_ASSASSIN_DROID: 
				case CLASS_PLAYER: 
				case CLASS_VEHICLE: 
				theFxScheduler.PlayEffect( cgs.effects.flechetteAltShotEffect, cent->lerpOrigin, forward );
				break;	
				case CLASS_FIGHTER: 
				theFxScheduler.PlayEffect( cgs.effects.shrapnel0AltShotEffect, cent->lerpOrigin, forward );
				break;
			default:
				break;
				}
		}
//		theFxScheduler.PlayEffect(cgs.effects.flechetteAltShotEffect, cent->lerpOrigin, forward );
}

void FX_CannonAltProjectileThink( centity_t *cent, const struct weaponInfo_s *weapon )
{
	vec3_t forward;

	if ( VectorNormalize2( cent->currentState.pos.trDelta, forward ) == 0.0f )
	{
		forward[2] = 1.0f;
	}

	theFxScheduler.PlayEffect( cgs.effects.cannonAltShotEffect, cent->lerpOrigin, forward );
}