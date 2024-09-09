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

// Rocket Launcher Weapon

// this line must stay at top so the whole PCH thing works...
#include "cg_headers.h"

//#include "cg_local.h"
#include "cg_media.h"
#include "FxScheduler.h"

/*
---------------------------
FX_RocketProjectileThink
---------------------------
*/

void FX_RocketProjectileThink( centity_t *cent, const struct weaponInfo_s *weapon )
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
			// FOG
			case CLASS_LIGHT: 
			case CLASS_GOLEM_LIGHT: 
			case CLASS_GOLEM_GOLD: 
			case CLASS_ASSASSIN_DROID: 
			case CLASS_SABER_DROID:
			case CLASS_PROTOCOL: 
			case CLASS_PROBE: 
			case CLASS_POLTER: 
			case CLASS_AIRREAVER: 
			case CLASS_REBEL: 
			case CLASS_SHARK:
			case CLASS_LYCAN:  
			case CLASS_WATER:
			case CLASS_GOLEM_WATER:
			case CLASS_GOLEM_ICE:
			theFxScheduler.PlayEffect( "rocket2/shot", cent->lerpOrigin, forward );
			break;
			case CLASS_ROCKETTROOPER:
			case CLASS_GOLEM_METAL: 
			case CLASS_DEMON_ELECTRO: 
			case CLASS_BOBAFETT:  
			theFxScheduler.PlayEffect( "rocket3/shot", cent->lerpOrigin, forward );
			break;
			case CLASS_MURJJ:
			case CLASS_THRALL: 
			case CLASS_NECRO: 
			case CLASS_GOLEM_NECRO: 
			case CLASS_FISH:
			case CLASS_GLIDER:
			case CLASS_MARK1:
			case CLASS_MARK2:
			case CLASS_IMPERIAL: 
			case CLASS_PROPHET: 
			theFxScheduler.PlayEffect( "thrallbreath/shot", cent->lerpOrigin, forward );
			break;
			case CLASS_REBORN:
			case CLASS_KYLE:
			case CLASS_ALORA: 
			case CLASS_MONMOTHA: 
			case CLASS_FLIER2: 
			case CLASS_LIZARD: 
			case CLASS_SWAMPTROOPER: 
			case CLASS_NOGHRI:
			case CLASS_TUSKEN: 
			case CLASS_WAMPA: 
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
			case CLASS_FIRE: 
			case CLASS_DEMON_FIRE: 
			case CLASS_HELLGOLEM: 
			case CLASS_GOLEM_FIRE: 
			case CLASS_GOLEM_LAVA: 
			case CLASS_MELC: 
			case CLASS_SUMMONER: 
			case CLASS_ZEPH: 
			case CLASS_DECEIVER:
			case CLASS_DUMAHIM: 
			case CLASS_RAHABIM: 
			case CLASS_WEEQUAY: 
			case CLASS_VAMPIRE_GHOST: 
			theFxScheduler.PlayEffect( "rocket4/shot", cent->lerpOrigin, forward );
			break;
			case CLASS_MOUSE:
			case CLASS_SENTRY:
			case CLASS_ATST:
			case CLASS_SAND_CREATURE: 
			case CLASS_ELDERGOD: 
			case CLASS_TENTACLE: 
				theFxScheduler.PlayEffect( "eldersphere/shot", cent->lerpOrigin, forward );
				break;
			case CLASS_SHADOWTROOPER: 
			case CLASS_GOLEM_VOID: 
			case CLASS_VOID: 
			case CLASS_DARKREAVER: 
			case CLASS_DARK: 
			case CLASS_GOLEM_DARK: 
			case CLASS_SABOTEUR:
			case CLASS_TAVION:
			case CLASS_DESANN: 
			case CLASS_HYLDEN: 
			case CLASS_HYLDENLORD: 
				theFxScheduler.PlayEffect( "blackhole/shot", cent->lerpOrigin, forward );
				break;
			case CLASS_MINEMONSTER: 
			case CLASS_IVY: 
			case CLASS_POISON: 
			case CLASS_GOLEM_POISON: 
			case CLASS_DEMON_POISON: 
			case CLASS_BEAST: 
			case CLASS_REELO: 
			case CLASS_ARCHER:
			case CLASS_ALCHEMIST: 
				theFxScheduler.PlayEffect( "poisonfog/shot", cent->lerpOrigin, forward );
				break;
			case CLASS_JAWA: 
			case CLASS_RANCOR: 
			case CLASS_GHOST: 
			case CLASS_DREADNOUGHT: 
			case CLASS_REAPER: 
			case CLASS_ARIEL: 
			case CLASS_CENOBITA: 			 
				theFxScheduler.PlayEffect( "spectralfog/shot", cent->lerpOrigin, forward );
				break;
			case CLASS_GRAN:
			case CLASS_TURELIM: 
			case CLASS_CLAW: 
				theFxScheduler.PlayEffect( "tk/shot", cent->lerpOrigin, forward );
				break;
			case CLASS_R2D2:
			case CLASS_R5D2:
			case CLASS_GONK:
			case CLASS_GOLEM: 
			case CLASS_EARTH: 
			case CLASS_EARTHREAVER: 
			case CLASS_SPIDERREAVER2: 
			case CLASS_SPIDERREAVER3: 
			case CLASS_GALAKMECH: 
			case CLASS_SARAFAN: 
			case CLASS_DEMON_BLADE:
			case CLASS_SCOUT: 
				theFxScheduler.PlayEffect( "sandstorm/shot", cent->lerpOrigin, forward );
				break;
			case CLASS_LANDO: 
				theFxScheduler.PlayEffect( "gravity/shot", cent->lerpOrigin, forward );
				break;
			case CLASS_BARTENDER:
			case CLASS_BESPIN_COP:
			case CLASS_COMMANDO: 
			case CLASS_GALAK: 			
			case CLASS_HOWLER: 
			case CLASS_IMPWORKER: 
			case CLASS_WIZARD: 
			case CLASS_INTERROGATOR:
			case CLASS_JAN: 
			case CLASS_JEDI: 
			case CLASS_LUKE: 			
			case CLASS_MORGANKATARN: 
			case CLASS_PRISONER: 			
			case CLASS_REMOTE: 
			case CLASS_RODIAN: 
			case CLASS_SEEKER: 
			case CLASS_STORMTROOPER: 
			case CLASS_SWAMP:  
			case CLASS_TRANDOSHAN: 
			case CLASS_UGNAUGHT: 			
			case CLASS_HAZARD_TROOPER: 			
			case CLASS_PLAYER: 
			case CLASS_VEHICLE:
			case CLASS_HUNTER: 			
			case CLASS_REAVER: 
			case CLASS_RAZIEL:
			case CLASS_NECROREAVER:
			case CLASS_TANKREAVER:
			case CLASS_LIGHTREAVER:
			case CLASS_DRUIDREAVER:
			case CLASS_ICEREAVER: 
			case CLASS_SPIDERREAVER:
			case CLASS_FIREREAVER: 
			case CLASS_MENTALIST: 
			case CLASS_CRYSTAL: 
			case CLASS_AIR:  			 
			case CLASS_HOLY: 			  
			case CLASS_SONIC: 
			case CLASS_BIRD: 			
			case CLASS_WARRIOR: 
			case CLASS_DRUID:
			case CLASS_GOLEM_AIR:
			case CLASS_GOLEM_SONIC:
			case CLASS_GOLEM_HOLY: 
			case CLASS_GOLEM_SMOKE: 
			theFxScheduler.PlayEffect( "rocket/shot", cent->lerpOrigin, forward );
			break;
			case CLASS_FIGHTER: 
			theFxScheduler.PlayEffect( "rocket0/shot", cent->lerpOrigin, forward );
			break;
		default:
			break;
			}
		}
		
	//theFxScheduler.PlayEffect( "rocket/shot", cent->lerpOrigin, forward );
}

/*
---------------------------
FX_RocketHitWall
---------------------------
*/

void FX_RocketHitWall( vec3_t origin, vec3_t normal )
{
	theFxScheduler.PlayEffect( "rocket/explosion", origin, normal );
}
// NEBBIA
void FX_FogHitWall( vec3_t origin, vec3_t normal )
{
	theFxScheduler.PlayEffect( "rocket2/explosion", origin, normal );
}
// TEMPESTA 
void FX_StormHitWall( vec3_t origin, vec3_t normal )
{
	theFxScheduler.PlayEffect( "rocket3/explosion", origin, normal );
}
// FLAY 
void FX_FlayHitWall( vec3_t origin, vec3_t normal )
{
	theFxScheduler.PlayEffect( "rocket4/explosion", origin, normal );
}
// SANDSTORM
void FX_SandStormHitWall( vec3_t origin, vec3_t normal )
{
	theFxScheduler.PlayEffect( "sandstorm/explosion", origin, normal );
}
// TELEKINESIS
void FX_TkHitWall( vec3_t origin, vec3_t normal )
{
	theFxScheduler.PlayEffect( "tk/explosion", origin, normal );
}
// SPECTRAL FOG
void FX_SpectralFogHitWall( vec3_t origin, vec3_t normal )
{
	theFxScheduler.PlayEffect( "spectralfog/explosion", origin, normal );
}
// POISON FOG 
void FX_PoisonFogHitWall( vec3_t origin, vec3_t normal )
{
	theFxScheduler.PlayEffect( "poisonfog/explosion", origin, normal );
}

// THRALL BREATH
void FX_ThrallBreathHitWall( vec3_t origin, vec3_t normal )
{
	theFxScheduler.PlayEffect( "thrallbreath/explosion", origin, normal );
}
// BLACK HOLE 
void FX_BlackHoleHitWall( vec3_t origin, vec3_t normal )
{
	theFxScheduler.PlayEffect( "blackhole/explosion", origin, normal );
}
// ELDER GOD SPHERE
void FX_ElderSphereHitWall( vec3_t origin, vec3_t normal )
{
	theFxScheduler.PlayEffect( "eldersphere/explosion", origin, normal );
}

void FX_ElderSphereAltHitWall( vec3_t origin, vec3_t normal )
{
	theFxScheduler.PlayEffect( "eldersphere/altexplosion", origin, normal );
}

void FX_GravityHitWall( vec3_t origin, vec3_t normal )
{
	theFxScheduler.PlayEffect( "gravity/explosion", origin, normal );
}

void FX_SeekerHitWall( vec3_t origin, vec3_t normal )
{
	theFxScheduler.PlayEffect( "rocket0/explosion", origin, normal );
}
/*
---------------------------
FX_RocketHitPlayer
---------------------------
*/

void FX_RocketHitPlayer( vec3_t origin, vec3_t normal, qboolean humanoid )
{
	theFxScheduler.PlayEffect( "rocket/explosion", origin, normal );
}

void FX_FogHitPlayer( vec3_t origin, vec3_t normal, qboolean humanoid )
{
	theFxScheduler.PlayEffect( "rocket2/explosion", origin, normal );
}

void FX_StormHitPlayer( vec3_t origin, vec3_t normal, qboolean humanoid )
{
	theFxScheduler.PlayEffect( "rocket3/explosion", origin, normal );
}

void FX_FlayHitPlayer( vec3_t origin, vec3_t normal, qboolean humanoid )
{
	theFxScheduler.PlayEffect( "rocket4/flesh_impact", origin, normal );
}

void FX_ThrallBreathHitPlayer( vec3_t origin, vec3_t normal, qboolean humanoid )
{
	theFxScheduler.PlayEffect( "thrallbreath/hitp", origin, normal );
}

void FX_PoisonFogHitPlayer( vec3_t origin, vec3_t normal, qboolean humanoid )
{
	theFxScheduler.PlayEffect( "poisonfog/hitp", origin, normal );
}

void FX_SpectralFogHitPlayer( vec3_t origin, vec3_t normal, qboolean humanoid )
{
	theFxScheduler.PlayEffect( "spectralfog/hitp", origin, normal );
}

void FX_SandStormHitPlayer( vec3_t origin, vec3_t normal, qboolean humanoid )
{
	theFxScheduler.PlayEffect( "sandstorm/hitp", origin, normal );
}

void FX_TkHitPlayer( vec3_t origin, vec3_t normal, qboolean humanoid )
{
	theFxScheduler.PlayEffect( "tk/hitp", origin, normal );
}

void FX_BlackHoleHitPlayer( vec3_t origin, vec3_t normal, qboolean humanoid )
{
	theFxScheduler.PlayEffect( "blackhole/hitp", origin, normal );
}

void FX_GravityHitPlayer( vec3_t origin, vec3_t normal, qboolean humanoid )
{
	theFxScheduler.PlayEffect( "gravity/hitp", origin, normal );
}
void FX_ElderSphereHitPlayer( vec3_t origin, vec3_t normal, qboolean humanoid )
{
	theFxScheduler.PlayEffect( "eldersphere/flesh_impact", origin, normal );
}

void FX_ElderSphereAltHitPlayer( vec3_t origin, vec3_t normal, qboolean humanoid )
{
	theFxScheduler.PlayEffect( "eldersphere/altflesh_impact", origin, normal );
}

void FX_SeekerHitPlayer( vec3_t origin, vec3_t normal, qboolean humanoid )
{
	theFxScheduler.PlayEffect( "rocket0/explosion", origin, normal );
}
/*
---------------------------
FX_RocketAltProjectileThink
---------------------------
*/

void FX_RocketAltProjectileThink( centity_t *cent, const struct weaponInfo_s *weapon )
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
			//FOG 
			case CLASS_LIGHT: 
			case CLASS_GOLEM_LIGHT: 
			case CLASS_GOLEM_GOLD: 
			case CLASS_ASSASSIN_DROID: 
			case CLASS_SABER_DROID:
			case CLASS_PROTOCOL: 
			case CLASS_PROBE: 
			case CLASS_POLTER: 
			case CLASS_AIRREAVER: 
			case CLASS_REBEL: 
			case CLASS_SHARK:
			case CLASS_LYCAN:  
			case CLASS_WATER:
			case CLASS_GOLEM_WATER:
			case CLASS_GOLEM_ICE:
			theFxScheduler.PlayEffect( "rocket2/shot", cent->lerpOrigin, forward );
			break;
			case CLASS_ROCKETTROOPER:
			case CLASS_GOLEM_METAL: 
			case CLASS_DEMON_ELECTRO: 
			case CLASS_BOBAFETT:  
			theFxScheduler.PlayEffect( "rocket3/shot", cent->lerpOrigin, forward );
			break;
			case CLASS_MURJJ:
			case CLASS_THRALL: 
			case CLASS_NECRO: 
			case CLASS_GOLEM_NECRO: 
			case CLASS_FISH:
			case CLASS_GLIDER:
			case CLASS_MARK1:
			case CLASS_MARK2:
			case CLASS_IMPERIAL: 
			case CLASS_PROPHET: 
			theFxScheduler.PlayEffect( "thrallbreath/shot", cent->lerpOrigin, forward );
			break;
			case CLASS_REBORN:
			case CLASS_KYLE:
			case CLASS_ALORA: 
			case CLASS_MONMOTHA: 
			case CLASS_FLIER2: 
			case CLASS_LIZARD: 
			case CLASS_SWAMPTROOPER: 
			case CLASS_NOGHRI:
			case CLASS_TUSKEN: 
			case CLASS_WAMPA: 
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
			case CLASS_FIRE: 
			case CLASS_DEMON_FIRE: 
			case CLASS_HELLGOLEM: 
			case CLASS_GOLEM_FIRE: 
			case CLASS_GOLEM_LAVA: 
			case CLASS_MELC: 
			case CLASS_SUMMONER: 
			case CLASS_ZEPH: 
			case CLASS_DECEIVER:
			case CLASS_DUMAHIM: 
			case CLASS_RAHABIM: 
			case CLASS_WEEQUAY: 
			case CLASS_VAMPIRE_GHOST: 
			theFxScheduler.PlayEffect( "rocket4/shot", cent->lerpOrigin, forward );
			break;
			case CLASS_MOUSE:
			case CLASS_SENTRY:
			case CLASS_ATST:
			case CLASS_SAND_CREATURE: 
			case CLASS_ELDERGOD: 
			case CLASS_TENTACLE: 
				theFxScheduler.PlayEffect( "eldersphere/altshot", cent->lerpOrigin, forward );
				break;
			case CLASS_SHADOWTROOPER: 
			case CLASS_GOLEM_VOID: 
			case CLASS_VOID: 
			case CLASS_DARKREAVER: 
			case CLASS_DARK: 
			case CLASS_GOLEM_DARK: 
			case CLASS_SABOTEUR:
			case CLASS_TAVION:
			case CLASS_DESANN: 
			case CLASS_HYLDEN: 
			case CLASS_HYLDENLORD: 
				theFxScheduler.PlayEffect( "blackhole/shot", cent->lerpOrigin, forward );
				break;
			case CLASS_MINEMONSTER: 
			case CLASS_IVY: 
			case CLASS_POISON: 
			case CLASS_GOLEM_POISON: 
			case CLASS_DEMON_POISON: 
			case CLASS_BEAST: 
			case CLASS_REELO: 
			case CLASS_ARCHER:
			case CLASS_ALCHEMIST: 
				theFxScheduler.PlayEffect( "poisonfog/shot", cent->lerpOrigin, forward );
				break;
			case CLASS_JAWA: 
			case CLASS_RANCOR: 
			case CLASS_GHOST: 
			case CLASS_DREADNOUGHT: 
			case CLASS_REAPER: 
			case CLASS_ARIEL: 
			case CLASS_CENOBITA: 			 
				theFxScheduler.PlayEffect( "spectralfog/shot", cent->lerpOrigin, forward );
				break;
			case CLASS_GRAN:
			case CLASS_TURELIM: 
			case CLASS_CLAW: 
				theFxScheduler.PlayEffect( "tk/shot", cent->lerpOrigin, forward );
				break;
			case CLASS_R2D2:
			case CLASS_R5D2:
			case CLASS_GONK:
			case CLASS_GOLEM: 
			case CLASS_EARTH: 
			case CLASS_EARTHREAVER: 
			case CLASS_SPIDERREAVER2: 
			case CLASS_SPIDERREAVER3: 
			case CLASS_GALAKMECH: 
			case CLASS_SARAFAN: 
			case CLASS_DEMON_BLADE:
			case CLASS_SCOUT: 
				theFxScheduler.PlayEffect( "sandstorm/shot", cent->lerpOrigin, forward );
				break;
			case CLASS_LANDO: 
				theFxScheduler.PlayEffect( "gravity/shot", cent->lerpOrigin, forward );
				break;
			case CLASS_BARTENDER:
			case CLASS_BESPIN_COP:
			case CLASS_COMMANDO: 
			case CLASS_GALAK: 			
			case CLASS_HOWLER: 
			case CLASS_IMPWORKER: 
			case CLASS_WIZARD: 
			case CLASS_INTERROGATOR:
			case CLASS_JAN: 
			case CLASS_JEDI: 
			case CLASS_LUKE: 			
			case CLASS_MORGANKATARN: 
			case CLASS_PRISONER: 			
			case CLASS_REMOTE: 
			case CLASS_RODIAN: 
			case CLASS_SEEKER: 
			case CLASS_STORMTROOPER: 
			case CLASS_SWAMP:  
			case CLASS_TRANDOSHAN: 
			case CLASS_UGNAUGHT: 			
			case CLASS_HAZARD_TROOPER: 			
			case CLASS_PLAYER: 
			case CLASS_VEHICLE:
			case CLASS_HUNTER: 			
			case CLASS_REAVER: 
			case CLASS_RAZIEL:
			case CLASS_NECROREAVER:
			case CLASS_TANKREAVER:
			case CLASS_LIGHTREAVER:
			case CLASS_DRUIDREAVER:
			case CLASS_ICEREAVER: 
			case CLASS_SPIDERREAVER:
			case CLASS_FIREREAVER: 
			case CLASS_MENTALIST: 
			case CLASS_CRYSTAL: 
			case CLASS_AIR:  			 
			case CLASS_HOLY: 			  
			case CLASS_SONIC: 
			case CLASS_BIRD: 			
			case CLASS_WARRIOR: 
			case CLASS_DRUID:
			case CLASS_GOLEM_AIR:
			case CLASS_GOLEM_SONIC:
			case CLASS_GOLEM_HOLY: 
			case CLASS_GOLEM_SMOKE: 
			theFxScheduler.PlayEffect( "rocket/shot", cent->lerpOrigin, forward );
			break; 
			case CLASS_FIGHTER: 
		theFxScheduler.PlayEffect( "rocket0/shot", cent->lerpOrigin, forward );
			break;
		default:
			break;
			}
		}
	// theFxScheduler.PlayEffect( "rocket/shot", cent->lerpOrigin, forward );
}

