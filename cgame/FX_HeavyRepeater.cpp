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

// Heavy Repeater Weapon

// this line must stay at top so the whole PCH thing works...
#include "cg_headers.h"

//#include "cg_local.h"
#include "cg_media.h"
#include "FxScheduler.h"

/*
---------------------------
FX_RepeaterProjectileThink
---------------------------
*/

void FX_RepeaterProjectileThink( centity_t *cent, const struct weaponInfo_s *weapon )
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
			// FONT OF PUTRESCENCE
				case CLASS_REBORN:
				case CLASS_KYLE:
				case CLASS_TUSKEN:
				case CLASS_LIZARD:
				case CLASS_MONMOTHA: 
				case CLASS_GLIDER:
				case CLASS_NOGHRI: 
				case CLASS_FLIER2: 
				case CLASS_GRAN: 
				case CLASS_R2D2:
				case CLASS_R5D2:
				case CLASS_RANCOR:
				case CLASS_SLUAGH: 
				case CLASS_MINEMONSTER: 
				case CLASS_POISON: 
				case CLASS_DEMON_POISON: 
				case CLASS_GOLEM_POISON: 
				case CLASS_MELC:  
				case CLASS_SUMMONER:
				case CLASS_ZEPH:
				case CLASS_DECEIVER:
				case CLASS_DUMAHIM: 
				case CLASS_TURELIM:
				case CLASS_VAMPIRE:
				case CLASS_KAIN:
				case CLASS_KAIN2: 
				case CLASS_KAIN3:
				case CLASS_UMAH: 
				case CLASS_VORADOR:
				case CLASS_JANOS:
				case CLASS_VAMPIRE_DARK: 
				case CLASS_VAMPIRE_NECRO: 
				case CLASS_VAMPIRE_FEAR: 
				case CLASS_VAMPIRE_BLOOD:
				case CLASS_VAMPIRE_SOUND: 
				case CLASS_VAMPIRE_LYCAN: 
				case CLASS_VAMPIRE_ELF: 
				case CLASS_VAMPIRE_ELF2:
				case CLASS_VAMPIRE_ELF3:
			  	case CLASS_VAMPIRE_ARCHER: 
				case CLASS_HALFVAMPIRE: 
				case CLASS_VAMPIRE_BOUNTY: 
				case CLASS_VAMPIRE_ZOMBIE: 
				case CLASS_VAMPIRE_SARAFAN:  
				case CLASS_VAMPIRE_GHOST: 
				case CLASS_BEAST: 
			theFxScheduler.PlayEffect( "putrescence/projectile", cent->lerpOrigin, forward );
			break; 
			// DEMONIC PUTRESCENSE 
				case CLASS_IMPERIAL:
				case CLASS_CENOBITA: 
				case CLASS_PROPHET: 
				case CLASS_WAMPA:
				case CLASS_VAMPIRE_DEMON:
				case CLASS_DEMON_FIRE: 
				case CLASS_DEMON_BLADE: 
				case CLASS_DEMON_ELECTRO: 
				case CLASS_HYLDEN: 
				case CLASS_HYLDENLORD: 
				case CLASS_TAVION:
				case CLASS_DESANN: 
			theFxScheduler.PlayEffect( "hellputrescence/projectile", cent->lerpOrigin, forward );
			break; 
				case CLASS_VAMPIRE_FEAR2:
				case CLASS_GOLEM_NECRO: 
				case CLASS_NECRO: 
				case CLASS_NECROMANCER: 
				case CLASS_VAMPIRE_QUEEN: 
				case CLASS_VAMPIRE_NECRO2: 
				case CLASS_MURJJ: 
				case CLASS_THRALL: 
			// danno di elemento morte, efx tipo proiettili della reaver di veleno ma con teschietti
				theFxScheduler.PlayEffect( "necrotic/projectile", cent->lerpOrigin, forward );
				break; 
			// ICE PROJECTILES 
				case CLASS_BARTENDER:
				case CLASS_RAZIELHIM:
				case CLASS_LYCAN:
				case CLASS_ICEREAVER: 
				case CLASS_VAMPIRE_FROST:
				case CLASS_GOLEM_ICE: 
				case CLASS_POLTER: 
				case CLASS_PROBE: 
			theFxScheduler.PlayEffect( "repeater2/projectile", cent->lerpOrigin, forward );
			break; 
			// THE CRYSTAL PROJECTILES  
				case CLASS_ESKANDOR: 
				case CLASS_GALAKMECH:
				case CLASS_CRYSTAL: 
				case CLASS_WEEQUAY:
				case CLASS_GONK: 
				case CLASS_GOLEM: 
				case CLASS_EARTH: 
			theFxScheduler.PlayEffect( "repeater3/projectile", cent->lerpOrigin, forward );
			break; 
			// THE VAPOUR PROJECTILES  
			case CLASS_INTERROGATOR:
				case CLASS_HAZARD_TROOPER: 
				case CLASS_SABER_DROID:
				case CLASS_PROTOCOL:
				case CLASS_CLAW: 
				case CLASS_JAWA: 
				case CLASS_GHOST: 
				case CLASS_GOLEM_LIGHT:
				case CLASS_GOLEM_GOLD:
				case CLASS_FIRE: 
				case CLASS_GOLEM_FIRE: 
				case CLASS_HELLGOLEM: 
				case CLASS_GOLEM_LAVA: 
				case CLASS_BIRD: 
				case CLASS_AIR: 
				case CLASS_GOLEM_AIR:
				case CLASS_LIGHT: 
				case CLASS_GOLEM_SMOKE: 
			theFxScheduler.PlayEffect( "vapor/projectile", cent->lerpOrigin, forward );
			break; 
			// TAR TENTACLE MUD  
				case CLASS_SHADOWTROOPER:
				case CLASS_VOID: 
				case CLASS_GOLEM_VOID: 
				case CLASS_ELDERGOD: 
				case CLASS_TENTACLE:
				case CLASS_REAPER: 
				case CLASS_DREADNOUGHT: 
				case CLASS_SENTRY:
				case CLASS_MOUSE:
				case CLASS_ATST:
				case CLASS_SAND_CREATURE:
			theFxScheduler.PlayEffect( "eldertar/projectile", cent->lerpOrigin, forward );
			break; 
			// TAR BROWN MUD 
			case CLASS_SABOTEUR:
				case CLASS_DARK: 
				case CLASS_GOLEM_DARK: 
				case CLASS_UGNAUGHT: 
				case CLASS_SPIDERREAVER2:
				case CLASS_SPIDERREAVER3:
				case CLASS_REELO: 
				case CLASS_ALCHEMIST: 
				case CLASS_IVY: 
				case CLASS_SWAMP:  
			theFxScheduler.PlayEffect( "tar/projectile", cent->lerpOrigin, forward );
			break; 
			// THE HOLY WATER
				case CLASS_RODIAN:
				case CLASS_GALAK:
				case CLASS_STORMTROOPER:
				case CLASS_JAN:
				case CLASS_MARK1:
				case CLASS_MARK2: 
				case CLASS_ARIEL: 	
				case CLASS_SARAFAN: 
				case CLASS_ARCHER:  
				case CLASS_WARRIOR: 
				case CLASS_HUNTER: 
				case CLASS_SCOUT:  
				case CLASS_HOLY: 
				case CLASS_GOLEM_HOLY: 
			theFxScheduler.PlayEffect( "holywater/projectile", cent->lerpOrigin, forward );
			break; 
			// RAHABIM IDROCINESIS   
				case CLASS_ALORA:
				case CLASS_VAMPIRE_WATER: 
				case CLASS_RAHABIM: 
				case CLASS_SWAMPTROOPER: 
				case CLASS_GOLEM_WATER:
			theFxScheduler.PlayEffect( "hydrotk/projectile", cent->lerpOrigin, forward );
			break; 
			// RAHABIM IDROCINESIS BOSS   
				case CLASS_FISH: 
			theFxScheduler.PlayEffect( "hydrotk2/projectile", cent->lerpOrigin, forward );
			break; 
			// DEFAULT WATERBALLS  
				case CLASS_HOWLER: 
				case CLASS_JEDI: 
				case CLASS_LANDO: 
				case CLASS_LUKE: 
				case CLASS_MORGANKATARN: 
				case CLASS_PRISONER: 
				case CLASS_REBEL: 
				case CLASS_REMOTE: 
				case CLASS_SEEKER: 
				case CLASS_TRANDOSHAN: 
				case CLASS_BESPIN_COP: 
				case CLASS_BOBAFETT: 
				case CLASS_ROCKETTROOPER: 
				case CLASS_ASSASSIN_DROID: 
				case CLASS_IMPWORKER: 
				case CLASS_WIZARD: 
				case CLASS_PLAYER: 
				case CLASS_VEHICLE: 
				case CLASS_DRUID: 
				case CLASS_REAVER: 
				case CLASS_RAZIEL:
				case CLASS_NECROREAVER:
				case CLASS_DARKREAVER: 
				case CLASS_TANKREAVER:
				case CLASS_LIGHTREAVER:
				case CLASS_DRUIDREAVER:
			 	case CLASS_SPIDERREAVER: 				
				case CLASS_FIREREAVER: 
				case CLASS_AIRREAVER: 
				case CLASS_MENTALIST: 
				case CLASS_EARTHREAVER: 			  
				case CLASS_WATER:			 
			  	case CLASS_SONIC: 
				case CLASS_SHARK:  
				case CLASS_GOLEM_METAL:			
				case CLASS_GOLEM_SONIC:	
				theFxScheduler.PlayEffect( "repeater/projectile", cent->lerpOrigin, forward );
				break;
				case CLASS_FIGHTER: 
				theFxScheduler.PlayEffect( "repeater0/projectile", cent->lerpOrigin, forward );
				break; 
			}
		}


	// theFxScheduler.PlayEffect( "repeater/projectile", cent->lerpOrigin, forward );
}

/*
------------------------
FX_RepeaterHitWall
------------------------
*/
// ACQUA 
void FX_RepeaterHitWall( vec3_t origin, vec3_t normal )
{
	theFxScheduler.PlayEffect( "repeater/wall_impact", origin, normal );
}
// ACQUA SACRA
void FX_HolyWaterHitWall( vec3_t origin, vec3_t normal )
{
	theFxScheduler.PlayEffect( "holywater/wall_impact", origin, normal );
}
// TK RAHABIM 
void FX_TkWaterHitWall( vec3_t origin, vec3_t normal )
{
	theFxScheduler.PlayEffect( "hydrotk/wall_impact", origin, normal );
}
// TK RAHABIM BOSS 
void FX_TkWater2HitWall( vec3_t origin, vec3_t normal )
{
	theFxScheduler.PlayEffect( "hydrotk2/wall_impact", origin, normal );
}

// GHIACCIO

void FX_IceHitWall( vec3_t origin, vec3_t normal )
{
	theFxScheduler.PlayEffect( "repeater2/wall_impact", origin, normal );
}

// CRISTALLO

void FX_CrystalHitWall( vec3_t origin, vec3_t normal )
{
	theFxScheduler.PlayEffect( "repeater3/wall_impact", origin, normal );
}

// VAPOR

void FX_VaporHitWall( vec3_t origin, vec3_t normal )
{
	theFxScheduler.PlayEffect( "vapor/wall_impact", origin, normal );
}

// TAR

void FX_TarHitWall( vec3_t origin, vec3_t normal )
{
	theFxScheduler.PlayEffect( "tar/wall_impact", origin, normal );
}

// ELDER TAR

void FX_ElderTarHitWall( vec3_t origin, vec3_t normal )
{
	theFxScheduler.PlayEffect( "eldertar/wall_impact", origin, normal );
}

// GREEN MUD

void FX_GreenMudHitWall( vec3_t origin, vec3_t normal )
{
	theFxScheduler.PlayEffect( "putrescence/wall_impact", origin, normal );
}

// HELL MUD

void FX_HellMudHitWall( vec3_t origin, vec3_t normal )
{
	theFxScheduler.PlayEffect( "hellputrescence/wall_impact", origin, normal );
}

// NECROTIC
void FX_NecroticHitWall( vec3_t origin, vec3_t normal )
{
	theFxScheduler.PlayEffect( "necrotic/wall_impact", origin, normal );
}

// PLASMA
void FX_PlasmaHitWall( vec3_t origin, vec3_t normal )
{
	theFxScheduler.PlayEffect( "repeater0/wall_impact", origin, normal );
}

/*
------------------------
FX_RepeaterHitPlayer
------------------------
*/

void FX_RepeaterHitPlayer( vec3_t origin, vec3_t normal, qboolean humanoid )
{
	theFxScheduler.PlayEffect( "repeater/wall_impact", origin, normal );
//	theFxScheduler.PlayEffect( "repeater/flesh_impact", origin, normal );
}

void FX_TkWaterHitPlayer( vec3_t origin, vec3_t normal, qboolean humanoid )
{
	theFxScheduler.PlayEffect( "hydrotk/wall_impact", origin, normal );
//	theFxScheduler.PlayEffect( "repeater/flesh_impact", origin, normal );
}

void FX_TkWater2HitPlayer( vec3_t origin, vec3_t normal, qboolean humanoid )
{
	theFxScheduler.PlayEffect( "hydrotk2/wall_impact", origin, normal );
//	theFxScheduler.PlayEffect( "repeater/flesh_impact", origin, normal );
}

// GHIACCIO
void FX_IceHitPlayer( vec3_t origin, vec3_t normal, qboolean humanoid )
{
	theFxScheduler.PlayEffect( "repeater2/wall_impact", origin, normal );
//	theFxScheduler.PlayEffect( "repeater/flesh_impact", origin, normal );
}

// CRISTALLO 
void FX_CrystalHitPlayer( vec3_t origin, vec3_t normal, qboolean humanoid )
{
	theFxScheduler.PlayEffect( "repeater3/wall_impact", origin, normal );
//	theFxScheduler.PlayEffect( "repeater/flesh_impact", origin, normal );
}
// VAPOR
void FX_VaporHitPlayer( vec3_t origin, vec3_t normal, qboolean humanoid )
{
	theFxScheduler.PlayEffect( "vapor/wall_impact", origin, normal );
//	theFxScheduler.PlayEffect( "repeater/flesh_impact", origin, normal );
}

// HOLY WATER 
void FX_HolyWaterHitPlayer( vec3_t origin, vec3_t normal, qboolean humanoid )
{
	theFxScheduler.PlayEffect( "holywater/flesh_impact", origin, normal );
//	theFxScheduler.PlayEffect( "repeater/flesh_impact", origin, normal );
}

// TAR  
void FX_TarHitPlayer( vec3_t origin, vec3_t normal, qboolean humanoid )
{
	theFxScheduler.PlayEffect( "tar/wall_impact", origin, normal );
//	theFxScheduler.PlayEffect( "repeater/flesh_impact", origin, normal );
}
// ELDER TAR 
void FX_ElderTarHitPlayer( vec3_t origin, vec3_t normal, qboolean humanoid )
{
	theFxScheduler.PlayEffect( "eldertar/flesh_impact", origin, normal );
//	theFxScheduler.PlayEffect( "repeater/flesh_impact", origin, normal );
}
// PUTRESCENCE
void FX_GreenMudHitPlayer( vec3_t origin, vec3_t normal, qboolean humanoid )
{
	theFxScheduler.PlayEffect( "putrescence/flesh_impact", origin, normal );
//	theFxScheduler.PlayEffect( "repeater/flesh_impact", origin, normal );
}

// DEMONIC PUTRESCENCE 
void FX_HellMudHitPlayer( vec3_t origin, vec3_t normal, qboolean humanoid )
{
	theFxScheduler.PlayEffect("hellputrescence/flesh_impact", origin, normal );
//	theFxScheduler.PlayEffect( "repeater/flesh_impact", origin, normal );
}

void FX_NecroticHitPlayer( vec3_t origin, vec3_t normal, qboolean humanoid )
{
	theFxScheduler.PlayEffect("necrotic/flesh_impact", origin, normal );
//	theFxScheduler.PlayEffect( "repeater/flesh_impact", origin, normal );
}

void FX_PlasmaHitPlayer( vec3_t origin, vec3_t normal, qboolean humanoid )
{
	theFxScheduler.PlayEffect( "repeater0/wall_impact", origin, normal );
//	theFxScheduler.PlayEffect( "repeater/flesh_impact", origin, normal );
}
/*
------------------------------
FX_RepeaterAltProjectileThink
-----------------------------
*/

void FX_RepeaterAltProjectileThink( centity_t *cent, const struct weaponInfo_s *weapon )
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
			// FONT OF PUTRESCENCE
				case CLASS_REBORN:
				case CLASS_KYLE:
				case CLASS_TUSKEN:
				case CLASS_LIZARD:
				case CLASS_MONMOTHA: 
				case CLASS_GLIDER:
				case CLASS_NOGHRI: 
				case CLASS_FLIER2: 
				case CLASS_GRAN: 
				case CLASS_R2D2:
				case CLASS_R5D2:
				case CLASS_RANCOR:
				case CLASS_SLUAGH: 
				case CLASS_MINEMONSTER: 
				case CLASS_POISON: 
				case CLASS_DEMON_POISON: 
				case CLASS_GOLEM_POISON: 
				case CLASS_MELC:  
				case CLASS_SUMMONER:
				case CLASS_ZEPH:
				case CLASS_DECEIVER:
				case CLASS_DUMAHIM: 
				case CLASS_TURELIM:
				case CLASS_VAMPIRE:
				case CLASS_KAIN:
				case CLASS_KAIN2: 
				case CLASS_KAIN3:
				case CLASS_UMAH: 
				case CLASS_VORADOR:
				case CLASS_JANOS:
				case CLASS_VAMPIRE_DARK: 
				case CLASS_VAMPIRE_NECRO: 
				case CLASS_VAMPIRE_FEAR: 
				case CLASS_VAMPIRE_BLOOD:
				case CLASS_VAMPIRE_SOUND: 
				case CLASS_VAMPIRE_LYCAN: 
				case CLASS_VAMPIRE_ELF: 
				case CLASS_VAMPIRE_ELF2:
				case CLASS_VAMPIRE_ELF3:
			  	case CLASS_VAMPIRE_ARCHER: 
				case CLASS_HALFVAMPIRE: 
				case CLASS_VAMPIRE_BOUNTY: 
				case CLASS_VAMPIRE_ZOMBIE: 
				case CLASS_VAMPIRE_SARAFAN:  
				case CLASS_VAMPIRE_GHOST: 
				case CLASS_BEAST: 
			theFxScheduler.PlayEffect( "putrescence/alt_projectile", cent->lerpOrigin, forward );
			break; 
			// DEMONIC PUTRESCENSE 
				case CLASS_IMPERIAL:
				case CLASS_CENOBITA: 
				case CLASS_PROPHET: 
				case CLASS_WAMPA:
				case CLASS_VAMPIRE_DEMON:
				case CLASS_DEMON_FIRE: 
				case CLASS_DEMON_BLADE: 
				case CLASS_DEMON_ELECTRO: 
				case CLASS_HYLDEN: 
				case CLASS_HYLDENLORD: 
				case CLASS_TAVION:
				case CLASS_DESANN: 
			theFxScheduler.PlayEffect( "hellputrescence/alt_projectile", cent->lerpOrigin, forward );
			break; 
				case CLASS_VAMPIRE_FEAR2:
				case CLASS_GOLEM_NECRO: 
				case CLASS_NECRO: 
				case CLASS_NECROMANCER: 
				case CLASS_VAMPIRE_QUEEN: 
				case CLASS_VAMPIRE_NECRO2: 
				case CLASS_MURJJ: 
				case CLASS_THRALL: 
			// danno di elemento morte, efx tipo proiettili della reaver di veleno ma con teschietti
				theFxScheduler.PlayEffect( "necrotic/alt_projectile", cent->lerpOrigin, forward );
				break; 
			// ICE alt_projectileS 
				case CLASS_RAZIELHIM:
				case CLASS_BARTENDER:
				case CLASS_LYCAN:
				case CLASS_ICEREAVER: 
				case CLASS_VAMPIRE_FROST:
				case CLASS_GOLEM_ICE: 
				case CLASS_POLTER: 
				case CLASS_PROBE: 
			theFxScheduler.PlayEffect( "repeater2/alt_projectile", cent->lerpOrigin, forward );
			break; 
			// THE CRYSTAL alt_projectileS  
				case CLASS_ESKANDOR: 
				case CLASS_GALAKMECH:
				case CLASS_CRYSTAL: 
				case CLASS_WEEQUAY:
				case CLASS_GONK: 
				case CLASS_GOLEM: 
				case CLASS_EARTH: 
			theFxScheduler.PlayEffect( "repeater3/alt_projectile", cent->lerpOrigin, forward );
			break; 
			// THE VAPOUR alt_projectileS  
			case CLASS_INTERROGATOR:
				case CLASS_HAZARD_TROOPER: 
				case CLASS_SABER_DROID:
				case CLASS_PROTOCOL:
				case CLASS_CLAW: 
				case CLASS_JAWA: 
				case CLASS_GHOST: 
				case CLASS_GOLEM_LIGHT:
				case CLASS_GOLEM_GOLD:
				case CLASS_FIRE: 
				case CLASS_GOLEM_FIRE: 
				case CLASS_HELLGOLEM: 
				case CLASS_GOLEM_LAVA: 
				case CLASS_BIRD: 
				case CLASS_AIR: 
				case CLASS_GOLEM_AIR:
				case CLASS_LIGHT: 
				case CLASS_GOLEM_SMOKE: 
			theFxScheduler.PlayEffect( "vapor/alt_projectile", cent->lerpOrigin, forward );
			break; 
			// TAR TENTACLE MUD  
				case CLASS_SHADOWTROOPER:
				case CLASS_VOID: 
				case CLASS_GOLEM_VOID: 
				case CLASS_ELDERGOD: 
				case CLASS_TENTACLE:
				case CLASS_REAPER: 
				case CLASS_DREADNOUGHT: 
				case CLASS_SENTRY:
				case CLASS_MOUSE:
				case CLASS_ATST:
				case CLASS_SAND_CREATURE:
			theFxScheduler.PlayEffect( "eldertar/alt_projectile", cent->lerpOrigin, forward );
			break; 
			// TAR BROWN MUD 
			case CLASS_SABOTEUR:
				case CLASS_DARK: 
				case CLASS_GOLEM_DARK: 
				case CLASS_UGNAUGHT: 
				case CLASS_SPIDERREAVER2:
				case CLASS_SPIDERREAVER3:
				case CLASS_REELO: 
				case CLASS_ALCHEMIST: 
				case CLASS_IVY: 
				case CLASS_SWAMP:  
			theFxScheduler.PlayEffect( "tar/alt_projectile", cent->lerpOrigin, forward );
			break; 
			// THE HOLY WATER
				case CLASS_RODIAN:
				case CLASS_GALAK:
				case CLASS_STORMTROOPER:
				case CLASS_JAN:
				case CLASS_MARK1:
				case CLASS_MARK2: 
				case CLASS_ARIEL: 	
				case CLASS_SARAFAN: 
				case CLASS_ARCHER:  
				case CLASS_WARRIOR: 
				case CLASS_HUNTER: 
				case CLASS_SCOUT:  
				case CLASS_HOLY: 
				case CLASS_GOLEM_HOLY: 
			theFxScheduler.PlayEffect( "holywater/alt_projectile", cent->lerpOrigin, forward );
			break; 
			// RAHABIM IDROCINESIS   
				case CLASS_ALORA:
				case CLASS_VAMPIRE_WATER: 
				case CLASS_RAHABIM: 
				case CLASS_SWAMPTROOPER: 
				case CLASS_GOLEM_WATER:
			theFxScheduler.PlayEffect( "hydrotk/alt_projectile", cent->lerpOrigin, forward );
			break; 
			// RAHABIM IDROCINESIS BOSS   
				case CLASS_FISH: 
			theFxScheduler.PlayEffect( "hydrotk2/alt_projectile", cent->lerpOrigin, forward );
			break; 
			// DEFAULT WATERBALLS  
				case CLASS_HOWLER: 
				case CLASS_JEDI: 
				case CLASS_LANDO: 
				case CLASS_LUKE: 
				case CLASS_MORGANKATARN: 
				case CLASS_PRISONER: 
				case CLASS_REBEL: 
				case CLASS_REMOTE: 
				case CLASS_SEEKER: 
				case CLASS_TRANDOSHAN: 
				case CLASS_BESPIN_COP: 
				case CLASS_BOBAFETT: 
				case CLASS_ROCKETTROOPER: 
				case CLASS_ASSASSIN_DROID: 
				case CLASS_IMPWORKER: 
				case CLASS_WIZARD: 
				case CLASS_PLAYER: 
				case CLASS_VEHICLE: 
				case CLASS_DRUID: 
				case CLASS_REAVER: 
				case CLASS_RAZIEL:
				case CLASS_NECROREAVER:
				case CLASS_DARKREAVER: 
				case CLASS_TANKREAVER:
				case CLASS_LIGHTREAVER:
				case CLASS_DRUIDREAVER:
			 	case CLASS_SPIDERREAVER: 				
				case CLASS_FIREREAVER: 
				case CLASS_AIRREAVER: 
				case CLASS_MENTALIST: 
				case CLASS_EARTHREAVER: 			  
				case CLASS_WATER:			 
			  	case CLASS_SONIC: 
				case CLASS_SHARK:  
				case CLASS_GOLEM_METAL:			
				case CLASS_GOLEM_SONIC:	
				theFxScheduler.PlayEffect( "repeater/alt_projectile", cent->lerpOrigin, forward );
				break;
				case CLASS_FIGHTER:	
				theFxScheduler.PlayEffect( "repeater0/alt_projectile", cent->lerpOrigin, forward );
				break;
			}
		}
//	theFxScheduler.PlayEffect( "repeater/alt_projectile", cent->lerpOrigin, forward );
}

/*
------------------------
FX_RepeaterAltHitWall
------------------------
*/

// ACQUA 
void FX_RepeaterAltHitWall( vec3_t origin, vec3_t normal )
{
	theFxScheduler.PlayEffect( "repeater/concussion", origin, normal );
}
// ACQUA SACRA
void FX_HolyWaterAltHitWall( vec3_t origin, vec3_t normal )
{
	theFxScheduler.PlayEffect( "holywater/concussion", origin, normal );
}
// TK RAHABIM 
void FX_TkWaterAltHitWall( vec3_t origin, vec3_t normal )
{
	theFxScheduler.PlayEffect( "hydrotk/concussion", origin, normal );
}
// TK RAHABIM BOSS 
void FX_TkWater2AltHitWall( vec3_t origin, vec3_t normal )
{
	theFxScheduler.PlayEffect( "hydrotk2/concussion", origin, normal );
}

// GHIACCIO

void FX_IceAltHitWall( vec3_t origin, vec3_t normal )
{
	theFxScheduler.PlayEffect( "repeater2/concussion", origin, normal );
}

// CRISTALLO

void FX_CrystalAltHitWall( vec3_t origin, vec3_t normal )
{
	theFxScheduler.PlayEffect( "repeater3/concussion", origin, normal );
}

// VAPOR

void FX_VaporAltHitWall( vec3_t origin, vec3_t normal )
{
	theFxScheduler.PlayEffect( "vapor/concussion", origin, normal );
}

// TAR

void FX_TarAltHitWall( vec3_t origin, vec3_t normal )
{
	theFxScheduler.PlayEffect( "tar/concussion", origin, normal );
}

// ELDER TAR

void FX_ElderTarAltHitWall( vec3_t origin, vec3_t normal )
{
	theFxScheduler.PlayEffect( "eldertar/concussion", origin, normal );
}

// GREEN MUD

void FX_GreenMudAltHitWall( vec3_t origin, vec3_t normal )
{
	theFxScheduler.PlayEffect( "putrescence/concussion", origin, normal );
}

// HELL MUD

void FX_HellMudAltHitWall( vec3_t origin, vec3_t normal )
{
	theFxScheduler.PlayEffect( "hellputrescence/concussion", origin, normal );
}

// Necrotic 
void FX_NecroticAltHitWall( vec3_t origin, vec3_t normal )
{
	theFxScheduler.PlayEffect( "necrotic/concussion", origin, normal );
}
// PLASMA BOMB
void FX_PlasmaBombAltHitWall( vec3_t origin, vec3_t normal )
{
	theFxScheduler.PlayEffect( "repeater0/concussion", origin, normal );
}

/*
------------------------
FX_RepeaterAltHitPlayer
------------------------
*/

void FX_RepeaterAltHitPlayer( vec3_t origin, vec3_t normal, qboolean humanoid )
{
	theFxScheduler.PlayEffect( "repeater/concussion", origin );
//	theFxScheduler.PlayEffect( "repeater/alt_wall_impact2", origin, normal );
}

// GHIACCIO 
void FX_IceAltHitPlayer( vec3_t origin, vec3_t normal, qboolean humanoid )
{
	theFxScheduler.PlayEffect( "repeater2/concussion", origin );
//	theFxScheduler.PlayEffect( "repeater/alt_wall_impact2", origin, normal );
}

// CRISTALLO 
void FX_CrystalAltHitPlayer( vec3_t origin, vec3_t normal, qboolean humanoid )
{
	theFxScheduler.PlayEffect( "repeater3/concussion", origin );
//	theFxScheduler.PlayEffect( "repeater/alt_wall_impact2", origin, normal );
}

void FX_TkWaterAltHitPlayer( vec3_t origin, vec3_t normal, qboolean humanoid )
{
	theFxScheduler.PlayEffect( "hydrotk/concussion", origin );
//	theFxScheduler.PlayEffect( "repeater/flesh_impact", origin, normal );
}

void FX_TkWater2AltHitPlayer( vec3_t origin, vec3_t normal, qboolean humanoid )
{
	theFxScheduler.PlayEffect( "hydrotk2/concussion", origin );
//	theFxScheduler.PlayEffect( "repeater/flesh_impact", origin, normal );
}

// VAPOR
void FX_VaporAltHitPlayer( vec3_t origin, vec3_t normal, qboolean humanoid )
{
	theFxScheduler.PlayEffect( "vapor/concussion", origin );
//	theFxScheduler.PlayEffect( "repeater/flesh_impact", origin, normal );
}

// HOLY WATER 
void FX_HolyWaterAltHitPlayer( vec3_t origin, vec3_t normal, qboolean humanoid )
{
	theFxScheduler.PlayEffect( "holywater/concussion2", origin );
//	theFxScheduler.PlayEffect( "repeater/flesh_impact", origin, normal );
}

// TAR  
void FX_TarAltHitPlayer( vec3_t origin, vec3_t normal, qboolean humanoid )
{
	theFxScheduler.PlayEffect( "tar/concussion", origin );
//	theFxScheduler.PlayEffect( "repeater/flesh_impact", origin, normal );
}
// ELDER TAR 
void FX_ElderTarAltHitPlayer( vec3_t origin, vec3_t normal, qboolean humanoid )
{
	theFxScheduler.PlayEffect( "eldertar/concussion2", origin );
//	theFxScheduler.PlayEffect( "repeater/flesh_impact", origin, normal );
}
// PUTRESCENCE
void FX_GreenMudAltHitPlayer( vec3_t origin, vec3_t normal, qboolean humanoid )
{
	theFxScheduler.PlayEffect( "putrescence/concussion2", origin );
//	theFxScheduler.PlayEffect( "repeater/flesh_impact", origin, normal );
}

// DEMONIC PUTRESCENCE 
void FX_HellMudAltHitPlayer( vec3_t origin, vec3_t normal, qboolean humanoid )
{
	theFxScheduler.PlayEffect("hellputrescence/concussion2", origin );
//	theFxScheduler.PlayEffect( "repeater/flesh_impact", origin, normal );
}

void FX_NecroticAltHitPlayer( vec3_t origin, vec3_t normal, qboolean humanoid )
{
	theFxScheduler.PlayEffect( "necrotic/concussion2", origin );
//	theFxScheduler.PlayEffect( "repeater/flesh_impact", origin, normal );
}

void FX_PlasmaBombAltHitPlayer( vec3_t origin, vec3_t normal, qboolean humanoid )
{
	theFxScheduler.PlayEffect( "repeater0/concussion", origin );
//	theFxScheduler.PlayEffect( "repeater/alt_wall_impact2", origin, normal );
}
