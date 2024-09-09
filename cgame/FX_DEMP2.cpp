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

// DEMP2 Weapon

// this line must stay at top so the whole PCH thing works...
#include "cg_headers.h"

//#include "cg_local.h"
#include "cg_media.h"
#include "FxScheduler.h"
#include "FxUtil.h"

/*
---------------------------
FX_DEMP2_ProjectileThink
---------------------------
*/

void FX_DEMP2_ProjectileThink( centity_t *cent, const struct weaponInfo_s *weapon )
{
	vec3_t forward;

	if ( VectorNormalize2( cent->currentState.pos.trDelta, forward ) == 0.0f )
	{
		forward[2] = 1.0f;
	}

//	theFxScheduler.PlayEffect( "demp2/shot", cent->lerpOrigin, forward );
//	theFxScheduler.PlayEffect( "demp2/shot2", cent->lerpOrigin, forward );
//	theFxScheduler.PlayEffect( "demp2/projectile", cent->lerpOrigin, forward );

	if ( cent->gent && cent->gent->owner && cent->gent->owner->client )
		 	{
				switch ( cent->gent->owner->client->NPC_class )
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
			theFxScheduler.PlayEffect( "demp/projectile", cent->lerpOrigin, forward );
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
			theFxScheduler.PlayEffect( "demp0/projectile", cent->lerpOrigin, forward );
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
			theFxScheduler.PlayEffect( "demp1/projectile", cent->lerpOrigin, forward );
			break;
			// SPIRIT
			case CLASS_RAZIEL: 
			case CLASS_IMPWORKER: 
			theFxScheduler.PlayEffect( "demp2/projectile", cent->lerpOrigin, forward );
			break;
				// HOLY
			case CLASS_HOLY: 
			case CLASS_GOLEM_HOLY: 
			case CLASS_VAMPIRE_GHOST: 
			theFxScheduler.PlayEffect( "demp3/projectile", cent->lerpOrigin, forward );
			break;
				// LIFE 
				case CLASS_LUKE: 
				case CLASS_IVY: 
				case CLASS_BEAST: 
				case CLASS_BIRD: 
				case CLASS_SHARK: 
			theFxScheduler.PlayEffect( "demp4/projectile", cent->lerpOrigin, forward );
			break;
				// DRUIDIC FIRE
				case CLASS_TRANDOSHAN:
				case CLASS_LANDO: 
				case CLASS_DRUIDREAVER: 
				case CLASS_DRUID: 
				case CLASS_LYCAN: 
			theFxScheduler.PlayEffect( "demp5/projectile", cent->lerpOrigin, forward );
			break;
				// HOLY FIRE
				case CLASS_JAN:
				case CLASS_SARAFAN: 
				case CLASS_STORMTROOPER: 
				case CLASS_MARK1:
				case CLASS_MARK2: 
				case CLASS_WIZARD: 
			theFxScheduler.PlayEffect( "demp6/projectile", cent->lerpOrigin, forward );
			break;
				// REVERB
				case CLASS_GALAKMECH:
				case CLASS_GONK: 
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
				theFxScheduler.PlayEffect( "demp7/projectile", cent->lerpOrigin, forward );
				break;
				// BALANCE 
				case CLASS_KYLE: 
				case CLASS_KAIN3: 
				case CLASS_ARIEL: 
				theFxScheduler.PlayEffect( "demp8/projectile", cent->lerpOrigin, forward );
				break;
				// WRACK IMPORTANT: NEVER. NEVER GET THIS TO AN NPC!   
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
				theFxScheduler.PlayEffect( "demp9/projectile", cent->lerpOrigin, forward );
			break;	
				case CLASS_FIGHTER:  
				theFxScheduler.PlayEffect( "demp00/projectile", cent->lerpOrigin, forward );
			break;								
		default:
			break;
				}
			}
}


void FX_Poison_ProjectileThink( centity_t *cent, const struct weaponInfo_s *weapon )
{
	vec3_t forward;

	if ( VectorNormalize2( cent->currentState.pos.trDelta, forward ) == 0.0f )
	{
		forward[2] = 1.0f;
	}

//	theFxScheduler.PlayEffect( "demp2/shot", cent->lerpOrigin, forward );
//	theFxScheduler.PlayEffect( "demp2/shot2", cent->lerpOrigin, forward );
	theFxScheduler.PlayEffect( "poison/projectile", cent->lerpOrigin, forward );
}

/*
---------------------------
FX_DEMP2_HitWall
---------------------------
*/

void FX_DEMP_HitWall( vec3_t origin, vec3_t normal )
{
	theFxScheduler.PlayEffect( "demp/wall_impact", origin, normal );
}

void FX_DEMP0_HitWall( vec3_t origin, vec3_t normal )
{
	theFxScheduler.PlayEffect( "demp0/wall_impact", origin, normal );
}

void FX_DEMP1_HitWall( vec3_t origin, vec3_t normal )
{
	theFxScheduler.PlayEffect( "demp1/wall_impact", origin, normal );
}

void FX_DEMP2_HitWall( vec3_t origin, vec3_t normal )
{
	theFxScheduler.PlayEffect( "demp2/wall_impact", origin, normal );
}

void FX_DEMP3_HitWall( vec3_t origin, vec3_t normal )
{
	theFxScheduler.PlayEffect( "demp3/wall_impact", origin, normal );
}

void FX_DEMP4_HitWall( vec3_t origin, vec3_t normal )
{
	theFxScheduler.PlayEffect( "demp4/wall_impact", origin, normal );
}

void FX_DEMP5_HitWall( vec3_t origin, vec3_t normal )
{
	theFxScheduler.PlayEffect( "demp5/wall_impact", origin, normal );
}

void FX_DEMP6_HitWall( vec3_t origin, vec3_t normal )
{
	theFxScheduler.PlayEffect( "demp6/wall_impact", origin, normal );
}

void FX_DEMP7_HitWall( vec3_t origin, vec3_t normal )
{
	theFxScheduler.PlayEffect( "demp7/wall_impact", origin, normal );
}

void FX_DEMP8_HitWall( vec3_t origin, vec3_t normal )
{
	theFxScheduler.PlayEffect( "demp8/wall_impact", origin, normal );
}

void FX_DEMP9_HitWall( vec3_t origin, vec3_t normal )
{
	theFxScheduler.PlayEffect( "demp9/wall_impact", origin, normal );
}

void FX_Poison_HitWall( vec3_t origin, vec3_t normal )
{
	theFxScheduler.PlayEffect( "poison/wall_impact", origin, normal );
}

void FX_PULSE_HitWall( vec3_t origin, vec3_t normal )
{
	theFxScheduler.PlayEffect( "demp00/wall_impact", origin, normal );
}

/*
---------------------------
FX_DEMP2_HitPlayer
---------------------------
*/

void FX_DEMP_HitPlayer( vec3_t origin, vec3_t normal, qboolean humanoid )
{
	theFxScheduler.PlayEffect( "demp/flesh_impact", origin, normal );
}

void FX_DEMP0_HitPlayer( vec3_t origin, vec3_t normal, qboolean humanoid )
{
	theFxScheduler.PlayEffect( "demp0/flesh_impact", origin, normal );
}

void FX_DEMP1_HitPlayer( vec3_t origin, vec3_t normal, qboolean humanoid )
{
	theFxScheduler.PlayEffect( "demp1/flesh_impact", origin, normal );
}

void FX_DEMP2_HitPlayer( vec3_t origin, vec3_t normal, qboolean humanoid )
{
	theFxScheduler.PlayEffect( "demp2/flesh_impact", origin, normal );
}

void FX_DEMP3_HitPlayer( vec3_t origin, vec3_t normal, qboolean humanoid )
{
	theFxScheduler.PlayEffect( "demp3/flesh_impact", origin, normal );
}

void FX_DEMP4_HitPlayer( vec3_t origin, vec3_t normal, qboolean humanoid )
{
	theFxScheduler.PlayEffect( "demp4/flesh_impact", origin, normal );
}

void FX_DEMP5_HitPlayer( vec3_t origin, vec3_t normal, qboolean humanoid )
{
	theFxScheduler.PlayEffect( "demp5/flesh_impact", origin, normal );
}

void FX_DEMP6_HitPlayer( vec3_t origin, vec3_t normal, qboolean humanoid )
{
	theFxScheduler.PlayEffect( "demp6/flesh_impact", origin, normal );
}

void FX_DEMP7_HitPlayer( vec3_t origin, vec3_t normal, qboolean humanoid )
{
	theFxScheduler.PlayEffect( "demp7/flesh_impact", origin, normal );
}

void FX_DEMP8_HitPlayer( vec3_t origin, vec3_t normal, qboolean humanoid )
{
	theFxScheduler.PlayEffect( "demp8/flesh_impact", origin, normal );
}

void FX_DEMP9_HitPlayer( vec3_t origin, vec3_t normal, qboolean humanoid )
{
	theFxScheduler.PlayEffect( "demp9/flesh_impact", origin, normal );
}

void FX_Poison_HitPlayer( vec3_t origin, vec3_t normal, qboolean humanoid )
{
	theFxScheduler.PlayEffect( "poison/flesh_impact", origin, normal );
}

void FX_PULSE_HitPlayer( vec3_t origin, vec3_t normal, qboolean humanoid )
{
	theFxScheduler.PlayEffect( "demp00/flesh_impact", origin, normal );
}
/*
---------------------------
FX_DEMP2_AltProjectileThink
---------------------------
*/

void FX_DEMP2_AltProjectileThink( centity_t *cent, const struct weaponInfo_s *weapon )
{
	vec3_t forward;

	if ( VectorNormalize2( cent->currentState.pos.trDelta, forward ) == 0.0f )
	{
		forward[2] = 1.0f;
	}

	
	if ( cent->gent && cent->gent->owner && cent->gent->owner->client )
		{
			switch ( cent->gent->owner->client->NPC_class )
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
			theFxScheduler.PlayEffect( "demp/projectile", cent->lerpOrigin, forward );
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
			theFxScheduler.PlayEffect( "demp0/projectile", cent->lerpOrigin, forward );
			break;
				// HELL WRACK
			case CLASS_RAZIELHIM:
			case CLASS_ESKANDOR: 
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
			theFxScheduler.PlayEffect( "demp1/projectile", cent->lerpOrigin, forward );
			break;
			// SPIRIT
			case CLASS_RAZIEL: 
			case CLASS_IMPWORKER: 
			theFxScheduler.PlayEffect( "demp2/projectile", cent->lerpOrigin, forward );
			break;
				// HOLY
			case CLASS_HOLY: 
			case CLASS_GOLEM_HOLY: 
			case CLASS_VAMPIRE_GHOST: 
			theFxScheduler.PlayEffect( "demp3/projectile", cent->lerpOrigin, forward );
			break;
				// LIFE 
				case CLASS_LUKE: 
				case CLASS_IVY: 
				case CLASS_BEAST: 
				case CLASS_BIRD: 
				case CLASS_SHARK: 
			theFxScheduler.PlayEffect( "demp4/projectile", cent->lerpOrigin, forward );
			break;
				// DRUIDIC FIRE
				case CLASS_TRANDOSHAN:
				case CLASS_LANDO: 
				case CLASS_DRUIDREAVER: 
				case CLASS_DRUID: 
				case CLASS_LYCAN: 
			theFxScheduler.PlayEffect( "demp5/projectile", cent->lerpOrigin, forward );
			break;
				// HOLY FIRE
				case CLASS_JAN:
				case CLASS_SARAFAN: 
				case CLASS_STORMTROOPER: 
				case CLASS_MARK1:
				case CLASS_MARK2: 
				case CLASS_WIZARD: 
			theFxScheduler.PlayEffect( "demp6/projectile", cent->lerpOrigin, forward );
			break;
				// REVERB
				case CLASS_GALAKMECH: 
				case CLASS_GONK: 
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
				theFxScheduler.PlayEffect( "demp7/projectile", cent->lerpOrigin, forward );
				break;
				// BALANCE 
				case CLASS_KYLE: 
				case CLASS_KAIN3: 
				case CLASS_ARIEL: 
				theFxScheduler.PlayEffect( "demp8/projectile", cent->lerpOrigin, forward );
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
				theFxScheduler.PlayEffect( "demp9/projectile", cent->lerpOrigin, forward );
				break;	
				case CLASS_FIGHTER: 
				theFxScheduler.PlayEffect( "demp00/projectile", cent->lerpOrigin, forward );
				break;
			default:
				break;
			}
	}
	// theFxScheduler.PlayEffect( "demp2/projectile", cent->lerpOrigin, forward );
}

void FX_Poison_AltProjectileThink( centity_t *cent, const struct weaponInfo_s *weapon )
{
	vec3_t forward;

	if ( VectorNormalize2( cent->currentState.pos.trDelta, forward ) == 0.0f )
	{
		forward[2] = 1.0f;
	}

	theFxScheduler.PlayEffect( "poison/projectile", cent->lerpOrigin, forward );
}


//---------------------------------------------
void FX_DEMP_AltDetonate( vec3_t org, float size )
{
	localEntity_t	*ex;

	ex = CG_AllocLocalEntity();
	ex->leType = LE_FADE_SCALE_MODEL;
	memset( &ex->refEntity, 0, sizeof( refEntity_t ));

	ex->refEntity.renderfx |= RF_VOLUMETRIC;

	ex->startTime = cg.time;
	ex->endTime = ex->startTime + 1300;
	
	ex->radius = size;
	ex->refEntity.customShader = cgi_R_RegisterShader( "gfx/effects/demp2shell" );

	ex->refEntity.hModel = cgi_R_RegisterModel( "models/items/sphere.md3" );
	VectorCopy( org, ex->refEntity.origin );
		
	ex->color[0] = ex->color[1] = ex->color[2] = 255.0f;
}

void FX_DEMP0_AltDetonate( vec3_t org, float size )
{
	localEntity_t	*ex;

	ex = CG_AllocLocalEntity();
	ex->leType = LE_FADE_SCALE_MODEL;
	memset( &ex->refEntity, 0, sizeof( refEntity_t ));

	ex->refEntity.renderfx |= RF_VOLUMETRIC;

	ex->startTime = cg.time;
	ex->endTime = ex->startTime + 1300;
	
	ex->radius = size;
	ex->refEntity.customShader = cgi_R_RegisterShader( "effects/concussion/omega" );

	ex->refEntity.hModel = cgi_R_RegisterModel( "models/items/sphere.md3" );
	VectorCopy( org, ex->refEntity.origin );
		
	ex->color[0] = ex->color[1] = ex->color[2] = 255.0f;
}

void FX_DEMP1_AltDetonate( vec3_t org, float size )
{
	localEntity_t	*ex;

	ex = CG_AllocLocalEntity();
	ex->leType = LE_FADE_SCALE_MODEL;
	memset( &ex->refEntity, 0, sizeof( refEntity_t ));

	ex->refEntity.renderfx |= RF_VOLUMETRIC;

	ex->startTime = cg.time;
	ex->endTime = ex->startTime + 1300;
	
	ex->radius = size;
	ex->refEntity.customShader = cgi_R_RegisterShader( "effects/flechette/ring" );

	ex->refEntity.hModel = cgi_R_RegisterModel( "models/items/sphere.md3" );
	VectorCopy( org, ex->refEntity.origin );
		
	ex->color[0] = ex->color[1] = ex->color[2] = 255.0f;
}

void FX_DEMP2_AltDetonate( vec3_t org, float size )
{
	localEntity_t	*ex;

	ex = CG_AllocLocalEntity();
	ex->leType = LE_FADE_SCALE_MODEL;
	memset( &ex->refEntity, 0, sizeof( refEntity_t ));

	ex->refEntity.renderfx |= RF_VOLUMETRIC;

	ex->startTime = cg.time;
	ex->endTime = ex->startTime + 1300;
	
	ex->radius = size;
	ex->refEntity.customShader = cgi_R_RegisterShader( "gfx/effects/demp2shell" );

	ex->refEntity.hModel = cgi_R_RegisterModel( "models/items/sphere.md3" );
	VectorCopy( org, ex->refEntity.origin );
		
	ex->color[0] = ex->color[1] = ex->color[2] = 255.0f;
}

void FX_DEMP3_AltDetonate( vec3_t org, float size )
{
	localEntity_t	*ex;

	ex = CG_AllocLocalEntity();
	ex->leType = LE_FADE_SCALE_MODEL;
	memset( &ex->refEntity, 0, sizeof( refEntity_t ));

	ex->refEntity.renderfx |= RF_VOLUMETRIC;

	ex->startTime = cg.time;
	ex->endTime = ex->startTime + 1300;
	
	ex->radius = size;
	ex->refEntity.customShader = cgi_R_RegisterShader( "effects/concussion/omega" );

	ex->refEntity.hModel = cgi_R_RegisterModel( "models/items/sphere.md3" );
	VectorCopy( org, ex->refEntity.origin );
		
	ex->color[0] = ex->color[1] = ex->color[2] = 255.0f;
}

void FX_DEMP4_AltDetonate( vec3_t org, float size )
{
	localEntity_t	*ex;

	ex = CG_AllocLocalEntity();
	ex->leType = LE_FADE_SCALE_MODEL;
	memset( &ex->refEntity, 0, sizeof( refEntity_t ));

	ex->refEntity.renderfx |= RF_VOLUMETRIC;

	ex->startTime = cg.time;
	ex->endTime = ex->startTime + 1300;
	
	ex->radius = size;
	ex->refEntity.customShader = cgi_R_RegisterShader( "effects/flechette/ring" );

	ex->refEntity.hModel = cgi_R_RegisterModel( "models/items/sphere.md3" );
	VectorCopy( org, ex->refEntity.origin );
		
	ex->color[0] = ex->color[1] = ex->color[2] = 255.0f;
}

void FX_DEMP5_AltDetonate( vec3_t org, float size )
{
	localEntity_t	*ex;

	ex = CG_AllocLocalEntity();
	ex->leType = LE_FADE_SCALE_MODEL;
	memset( &ex->refEntity, 0, sizeof( refEntity_t ));

	ex->refEntity.renderfx |= RF_VOLUMETRIC;

	ex->startTime = cg.time;
	ex->endTime = ex->startTime + 1300;
	
	ex->radius = size;
	ex->refEntity.customShader = cgi_R_RegisterShader( "gfx/effects/demp2shell" );

	ex->refEntity.hModel = cgi_R_RegisterModel( "models/items/sphere.md3" );
	VectorCopy( org, ex->refEntity.origin );
		
	ex->color[0] = ex->color[1] = ex->color[2] = 255.0f;
}

void FX_DEMP6_AltDetonate( vec3_t org, float size )
{
	localEntity_t	*ex;

	ex = CG_AllocLocalEntity();
	ex->leType = LE_FADE_SCALE_MODEL;
	memset( &ex->refEntity, 0, sizeof( refEntity_t ));

	ex->refEntity.renderfx |= RF_VOLUMETRIC;

	ex->startTime = cg.time;
	ex->endTime = ex->startTime + 1300;
	
	ex->radius = size;
	ex->refEntity.customShader = cgi_R_RegisterShader( "effects/concussion/omega" );

	ex->refEntity.hModel = cgi_R_RegisterModel( "models/items/sphere.md3" );
	VectorCopy( org, ex->refEntity.origin );
		
	ex->color[0] = ex->color[1] = ex->color[2] = 255.0f;
}

void FX_DEMP7_AltDetonate( vec3_t org, float size )
{
	localEntity_t	*ex;

	ex = CG_AllocLocalEntity();
	ex->leType = LE_FADE_SCALE_MODEL;
	memset( &ex->refEntity, 0, sizeof( refEntity_t ));

	ex->refEntity.renderfx |= RF_VOLUMETRIC;

	ex->startTime = cg.time;
	ex->endTime = ex->startTime + 1300;
	
	ex->radius = size;
	ex->refEntity.customShader = cgi_R_RegisterShader( "effects/flechette/ring" );

	ex->refEntity.hModel = cgi_R_RegisterModel( "models/items/sphere.md3" );
	VectorCopy( org, ex->refEntity.origin );
		
	ex->color[0] = ex->color[1] = ex->color[2] = 255.0f;
}

void FX_DEMP8_AltDetonate( vec3_t org, float size )
{
	localEntity_t	*ex;

	ex = CG_AllocLocalEntity();
	ex->leType = LE_FADE_SCALE_MODEL;
	memset( &ex->refEntity, 0, sizeof( refEntity_t ));

	ex->refEntity.renderfx |= RF_VOLUMETRIC;

	ex->startTime = cg.time;
	ex->endTime = ex->startTime + 1300;
	
	ex->radius = size;
	ex->refEntity.customShader = cgi_R_RegisterShader( "gfx/effects/demp2shell" );

	ex->refEntity.hModel = cgi_R_RegisterModel( "models/items/sphere.md3" );
	VectorCopy( org, ex->refEntity.origin );
		
	ex->color[0] = ex->color[1] = ex->color[2] = 255.0f;
}

void FX_DEMP9_AltDetonate( vec3_t org, float size )
{
	localEntity_t	*ex;

	ex = CG_AllocLocalEntity();
	ex->leType = LE_FADE_SCALE_MODEL;
	memset( &ex->refEntity, 0, sizeof( refEntity_t ));

	ex->refEntity.renderfx |= RF_VOLUMETRIC;

	ex->startTime = cg.time;
	ex->endTime = ex->startTime + 1300;
	
	ex->radius = size;
	ex->refEntity.customShader = cgi_R_RegisterShader( "effects/concussion/omega" );

	ex->refEntity.hModel = cgi_R_RegisterModel( "models/items/sphere.md3" );
	VectorCopy( org, ex->refEntity.origin );
		
	ex->color[0] = ex->color[1] = ex->color[2] = 255.0f;
}

void FX_Poison_AltDetonate( vec3_t org, float size )
{
	localEntity_t	*ex;

	ex = CG_AllocLocalEntity();
	ex->leType = LE_FADE_SCALE_MODEL;
	memset( &ex->refEntity, 0, sizeof( refEntity_t ));

	ex->refEntity.renderfx |= RF_VOLUMETRIC;

	ex->startTime = cg.time;
	ex->endTime = ex->startTime + 1300;
	
	ex->radius = size;
	ex->refEntity.customShader = cgi_R_RegisterShader( "gfx/effects/poisonshell" );

	ex->refEntity.hModel = cgi_R_RegisterModel( "models/items/sphere.md3" );
	VectorCopy( org, ex->refEntity.origin );
		
	ex->color[0] = ex->color[1] = ex->color[2] = 255.0f;
}

void FX_PULSE_AltDetonate( vec3_t org, float size )
{
	localEntity_t	*ex;

	ex = CG_AllocLocalEntity();
	ex->leType = LE_FADE_SCALE_MODEL;
	memset( &ex->refEntity, 0, sizeof( refEntity_t ));

	ex->refEntity.renderfx |= RF_VOLUMETRIC;

	ex->startTime = cg.time;
	ex->endTime = ex->startTime + 1300;
	
	ex->radius = size;
	ex->refEntity.customShader = cgi_R_RegisterShader( "gfx/effects/demp2shell" );

	ex->refEntity.hModel = cgi_R_RegisterModel( "models/items/sphere.md3" );
	VectorCopy( org, ex->refEntity.origin );
		
	ex->color[0] = ex->color[1] = ex->color[2] = 255.0f;
}
