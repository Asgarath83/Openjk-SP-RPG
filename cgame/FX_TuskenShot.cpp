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

// Tusken Rifle

// this line must stay at top so the whole PCH thing works...
#include "cg_headers.h"

#include "cg_local.h"
#include "cg_media.h"
#include "FxScheduler.h"

/*
-------------------------
FX_TuskenShotProjectileThink
-------------------------
*/

void FX_TuskenShotProjectileThink( centity_t *cent, const struct weaponInfo_s *weapon )
{
	vec3_t forward;

	//BY BLASTER CODE 
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
	//-- BY BLASTER CODE 

	/*if ( VectorNormalize2( cent->gent->s.pos.trDelta, forward ) == 0.0f )
	{
		if ( VectorNormalize2( cent->currentState.pos.trDelta, forward ) == 0.0f )
		{
			forward[2] = 1.0f;
		}
	}*/

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

	// BOWCASTER IRONGUARD SHOT 
	
	if ( cent->gent && cent->gent->owner && cent->gent->owner->s.number > 0 )
	{
	if ( cent->gent && cent->gent->owner && cent->gent->owner->client )
		{
				switch ( cent->gent->owner->client->NPC_class )
				{
				// FORCE
				case CLASS_BIRD: 
				case CLASS_REAVER: 
				case CLASS_TANKREAVER:
				case CLASS_AIRREAVER: 
				case CLASS_MENTALIST: 
				case CLASS_AIR: 
				case CLASS_GOLEM_AIR: 
				case CLASS_TURELIM: 				
				case CLASS_JEDI:
				case CLASS_GRAN: 
				case CLASS_PROBE: 
				case CLASS_ROCKETTROOPER:
				case CLASS_BOBAFETT:
				case CLASS_BESPIN_COP: 
				case CLASS_IMPWORKER:
				case CLASS_WIZARD: 
				case CLASS_ARCHER: 
				theFxScheduler.PlayEffect( "tusken2/shot", cent->lerpOrigin, forward );
				break;
				// STONE
				case CLASS_REELO: 
				case CLASS_MONMOTHA:
				case CLASS_LIZARD: 
				case CLASS_WEEQUAY:
				case CLASS_JAN:
				case CLASS_GONK:
				case CLASS_GALAKMECH: 
				case CLASS_UGNAUGHT: 	
				case CLASS_SPIDERREAVER:
				case CLASS_SPIDERREAVER2:
				case CLASS_EARTHREAVER: 
				case CLASS_EARTH: 
				case CLASS_GOLEM: 
				case CLASS_CRYSTAL: 
				case CLASS_DUMAHIM: 
				case CLASS_VAMPIRE_ELF: 
				case CLASS_VAMPIRE_ELF2: 
				case CLASS_VAMPIRE_ELF3: 
				case CLASS_WARRIOR: 
				case CLASS_DEMON_BLADE: 
				theFxScheduler.PlayEffect( "tusken3/shot", cent->lerpOrigin, forward );
				break;
				// SOUND 
				case CLASS_TRANDOSHAN: 
				case CLASS_DRUIDREAVER: 
				case CLASS_LANDO:
				case CLASS_HOWLER: 
				case CLASS_SONIC: 
				case CLASS_GOLEM_SONIC: 
				case CLASS_DRUID:
				case CLASS_LYCAN: 
				case CLASS_VAMPIRE_SOUND: 
				case CLASS_R2D2:
				case CLASS_R5D2: 
				theFxScheduler.PlayEffect( "tusken4/shot", cent->lerpOrigin, forward );
				break;
				// WATER 
				case CLASS_SHARK: 
				case CLASS_SWAMP: 
				case CLASS_CENOBITA: 
				case CLASS_FISH:
				case CLASS_BARTENDER:
				case CLASS_SWAMPTROOPER: 
				case CLASS_ICEREAVER:
				case CLASS_WATER: 
				case CLASS_GOLEM_WATER: 
				case CLASS_GOLEM_ICE: 
				case CLASS_RAHABIM:
				case CLASS_ALORA: 
				case CLASS_VAMPIRE_WATER: 
				case CLASS_VAMPIRE_FROST: 
				theFxScheduler.PlayEffect( "tusken5/shot", cent->lerpOrigin, forward );
				break;
				// SUNLIGHT 
				case CLASS_GALAK:
				case CLASS_ASSASSIN_DROID: 
				case CLASS_SABER_DROID:
				case CLASS_PROTOCOL:
				case CLASS_MORGANKATARN: 
				case CLASS_LIGHTREAVER:
				case CLASS_GOLEM_LIGHT: 
				case CLASS_GOLEM_GOLD: 
				case CLASS_LIGHT: 
				case CLASS_GOLEM_METAL: 
				case CLASS_VAMPIRE_ARCHER: 
				case CLASS_SCOUT: 
				case CLASS_DEMON_ELECTRO: 
				theFxScheduler.PlayEffect( "tusken6/shot", cent->lerpOrigin, forward );
				break;
				// HOLY 
				case CLASS_STORMTROOPER:
				case CLASS_LUKE: 
				case CLASS_RAZIEL: 
				case CLASS_HOLY: 
				case CLASS_GOLEM_HOLY: 
				case CLASS_ARIEL: 
				case CLASS_MARK1: 
				case CLASS_MARK2:
				case CLASS_SARAFAN:  
				theFxScheduler.PlayEffect( "tusken7/shot", cent->lerpOrigin, forward );
				break;
				// DARKNESS 
				case CLASS_SHADOWTROOPER:
				case CLASS_SABOTEUR: 
				case CLASS_DARKREAVER:
				case CLASS_DARK: 
				case CLASS_VOID: 
				case CLASS_GOLEM_SMOKE: 
				case CLASS_GOLEM_VOID: 
				case CLASS_GOLEM_DARK: 
				case CLASS_VAMPIRE_DARK: 
				case CLASS_VAMPIRE_SARAFAN: 
				theFxScheduler.PlayEffect( "tusken1/shot", cent->lerpOrigin, forward );
				break;
				// VAMPIRIC 
				case CLASS_REBORN:
				case CLASS_KYLE:
				case CLASS_NOGHRI: 
				case CLASS_FLIER2:
				case CLASS_WAMPA: 
				case CLASS_ZEPH:
				case CLASS_DECEIVER:
				case CLASS_VAMPIRE:
				case CLASS_VAMPIRE_DEMON:
				case CLASS_KAIN:
				case CLASS_KAIN2: 
				case CLASS_KAIN3:
				case CLASS_UMAH: 
				case CLASS_VORADOR:
				case CLASS_JANOS:
				case CLASS_VAMPIRE_LYCAN: 
				case CLASS_VAMPIRE_BLOOD: 
				case CLASS_HALFVAMPIRE: 
				case CLASS_RAZIELHIM:  
				case CLASS_ESKANDOR: 
				case CLASS_VAMPIRE_BOUNTY: 
				theFxScheduler.PlayEffect( "tusken8/shot", cent->lerpOrigin, forward );
				break;
				// UNDEAD 
				case CLASS_JAWA:
				case CLASS_RANCOR:
				case CLASS_COMMANDO: 
				case CLASS_MURJJ: 
				case CLASS_NECROREAVER:
				case CLASS_NECRO: 
				case CLASS_GOLEM_NECRO: 
				case CLASS_THRALL: 
				case CLASS_NECROMANCER: 
				case CLASS_VAMPIRE_QUEEN:
				case CLASS_VAMPIRE_NECRO: 
				case CLASS_VAMPIRE_NECRO2:
				case CLASS_VAMPIRE_ZOMBIE: 
				case CLASS_POLTER: 
				theFxScheduler.PlayEffect( "tusken0/shot", cent->lerpOrigin, forward );
				break;
				// HYLDEN 
				case CLASS_PROPHET: 	
				case CLASS_TAVION:
				case CLASS_DESANN:
				case CLASS_HYLDEN: 
				case CLASS_HYLDENLORD: 
				case CLASS_REMOTE:
				case CLASS_SEEKER:
				case CLASS_SENTRY:
				case CLASS_ATST: 
				case CLASS_MOUSE:
				case CLASS_SAND_CREATURE: 	
				case CLASS_TENTACLE:  
				case CLASS_ELDERGOD: 
				theFxScheduler.PlayEffect( "tusken9/shot", cent->lerpOrigin, forward );
				break;
				// POISON
				case CLASS_SPIDERREAVER3: 
				case CLASS_MINEMONSTER:
				case CLASS_INTERROGATOR: 
				case CLASS_IVY: 
				case CLASS_POISON: 
				case CLASS_GOLEM_POISON: 		
				case CLASS_DEMON_POISON:
				case CLASS_BEAST: 				
				case CLASS_ALCHEMIST: 
				theFxScheduler.PlayEffect( "tusken10/shot", cent->lerpOrigin, forward );
				break;
				// DEFAULT 
				case CLASS_TUSKEN:
				case CLASS_RODIAN: 
				case CLASS_GLIDER: 
				case CLASS_PRISONER: 
				case CLASS_IMPERIAL: 
				case CLASS_HAZARD_TROOPER: 
				case CLASS_CLAW: 
				case CLASS_PLAYER: 
				case CLASS_VEHICLE: 
				case CLASS_FIREREAVER:
				case CLASS_FIRE: 
				case CLASS_HELLGOLEM: 
				case CLASS_GOLEM_FIRE: 
				case CLASS_GOLEM_LAVA: 
				case CLASS_MELC:  
				case CLASS_SUMMONER:
				case CLASS_VAMPIRE_FEAR: 
				case CLASS_VAMPIRE_FEAR2:
				case CLASS_DEMON_FIRE: 
				case CLASS_HUNTER: 
				theFxScheduler.PlayEffect( "tusken/shot", cent->lerpOrigin, forward );;	
				break;  
			default:
				break;
				}
			}
	}
	else 
	{
			if ( cent->gent && cent->gent->owner && cent->gent->owner->client )
		{
				switch ( cent->gent->owner->client->NPC_class )
				{
				// FORCE
				case CLASS_BIRD: 
				case CLASS_REAVER: 
				case CLASS_TANKREAVER:
				case CLASS_AIRREAVER: 
				case CLASS_MENTALIST: 
				case CLASS_AIR: 
				case CLASS_GOLEM_AIR: 
				case CLASS_TURELIM: 				
				case CLASS_JEDI:
				case CLASS_GRAN: 
				case CLASS_PROBE: 
				case CLASS_ROCKETTROOPER:
				case CLASS_BOBAFETT:
				case CLASS_BESPIN_COP: 
				case CLASS_IMPWORKER:
				case CLASS_WIZARD: 
				case CLASS_ARCHER: 
				theFxScheduler.PlayEffect( "tusken2/shot", cent->lerpOrigin, forward );
				break;
				// STONE
				case CLASS_REELO: 
				case CLASS_MONMOTHA:
				case CLASS_LIZARD: 
				case CLASS_WEEQUAY:
				case CLASS_JAN:
				case CLASS_GONK:
				case CLASS_GALAKMECH: 
				case CLASS_UGNAUGHT: 	
				case CLASS_SPIDERREAVER:
				case CLASS_SPIDERREAVER2:
				case CLASS_EARTHREAVER: 
				case CLASS_EARTH: 
				case CLASS_GOLEM: 
				case CLASS_CRYSTAL: 
				case CLASS_DUMAHIM: 
				case CLASS_VAMPIRE_ELF: 
				case CLASS_VAMPIRE_ELF2: 
				case CLASS_VAMPIRE_ELF3: 
				case CLASS_WARRIOR: 
				case CLASS_DEMON_BLADE: 
				theFxScheduler.PlayEffect( "tusken3/shot", cent->lerpOrigin, forward );
				break;
				// SOUND 
				case CLASS_TRANDOSHAN: 
				case CLASS_DRUIDREAVER: 
				case CLASS_LANDO:
				case CLASS_HOWLER: 
				case CLASS_SONIC: 
				case CLASS_GOLEM_SONIC: 
				case CLASS_DRUID:
				case CLASS_LYCAN: 
				case CLASS_VAMPIRE_SOUND: 
				case CLASS_R2D2:
				case CLASS_R5D2: 
				theFxScheduler.PlayEffect( "tusken4/shot", cent->lerpOrigin, forward );
				break;
				// WATER 
				case CLASS_SHARK: 
				case CLASS_SWAMP: 
				case CLASS_CENOBITA: 
				case CLASS_FISH:
				case CLASS_BARTENDER:
				case CLASS_SWAMPTROOPER: 
				case CLASS_ICEREAVER:
				case CLASS_WATER: 
				case CLASS_GOLEM_WATER: 
				case CLASS_GOLEM_ICE: 
				case CLASS_RAHABIM:
				case CLASS_ALORA: 
				case CLASS_VAMPIRE_WATER: 
				case CLASS_VAMPIRE_FROST: 
				theFxScheduler.PlayEffect( "tusken5/shot", cent->lerpOrigin, forward );
				break;
				// SUNLIGHT 
				case CLASS_GALAK:
				case CLASS_ASSASSIN_DROID: 
				case CLASS_SABER_DROID:
				case CLASS_PROTOCOL:
				case CLASS_MORGANKATARN: 
				case CLASS_LIGHTREAVER:
				case CLASS_GOLEM_LIGHT: 
				case CLASS_GOLEM_GOLD: 
				case CLASS_LIGHT: 
				case CLASS_GOLEM_METAL: 
				case CLASS_VAMPIRE_ARCHER: 
				case CLASS_SCOUT: 
				case CLASS_DEMON_ELECTRO: 
				theFxScheduler.PlayEffect( "tusken6/shot", cent->lerpOrigin, forward );
				break;
				// HOLY 
				case CLASS_STORMTROOPER:
				case CLASS_LUKE: 
				case CLASS_RAZIEL: 
				case CLASS_HOLY: 
				case CLASS_GOLEM_HOLY: 
				case CLASS_ARIEL: 
				case CLASS_MARK1: 
				case CLASS_MARK2:
				case CLASS_SARAFAN:  
				theFxScheduler.PlayEffect( "tusken7/shot", cent->lerpOrigin, forward );
				break;
				// DARKNESS 
				case CLASS_SHADOWTROOPER:
				case CLASS_SABOTEUR: 
				case CLASS_DARKREAVER:
				case CLASS_DARK: 
				case CLASS_VOID: 
				case CLASS_GOLEM_SMOKE: 
				case CLASS_GOLEM_VOID: 
				case CLASS_GOLEM_DARK: 
				case CLASS_VAMPIRE_DARK: 
				case CLASS_VAMPIRE_SARAFAN: 
				theFxScheduler.PlayEffect( "tusken1/shot", cent->lerpOrigin, forward );
				break;
				// VAMPIRIC 
				case CLASS_REBORN:
				case CLASS_KYLE:
				case CLASS_NOGHRI: 
				case CLASS_FLIER2:
				case CLASS_WAMPA: 
				case CLASS_ZEPH:
				case CLASS_DECEIVER:
				case CLASS_VAMPIRE:
				case CLASS_VAMPIRE_DEMON:
				case CLASS_KAIN:
				case CLASS_KAIN2: 
				case CLASS_KAIN3:
				case CLASS_UMAH: 
				case CLASS_VORADOR:
				case CLASS_JANOS:
				case CLASS_VAMPIRE_LYCAN: 
				case CLASS_VAMPIRE_BLOOD: 
				case CLASS_HALFVAMPIRE: 
				case CLASS_RAZIELHIM:  
				case CLASS_ESKANDOR: 
				case CLASS_VAMPIRE_BOUNTY: 
				theFxScheduler.PlayEffect( "tusken8/shot", cent->lerpOrigin, forward );
				break;
				// UNDEAD 
				case CLASS_JAWA:
				case CLASS_RANCOR:
				case CLASS_COMMANDO: 
				case CLASS_MURJJ: 
				case CLASS_NECROREAVER:
				case CLASS_NECRO: 
				case CLASS_GOLEM_NECRO: 
				case CLASS_THRALL: 
				case CLASS_NECROMANCER: 
				case CLASS_VAMPIRE_QUEEN:
				case CLASS_VAMPIRE_NECRO: 
				case CLASS_VAMPIRE_NECRO2:
				case CLASS_VAMPIRE_ZOMBIE: 
				case CLASS_POLTER: 
				theFxScheduler.PlayEffect( "tusken0/shot", cent->lerpOrigin, forward );
				break;
				// HYLDEN 
				case CLASS_PROPHET: 	
				case CLASS_TAVION:
				case CLASS_DESANN:
				case CLASS_HYLDEN: 
				case CLASS_HYLDENLORD: 
				case CLASS_REMOTE:
				case CLASS_SEEKER:
				case CLASS_SENTRY:
				case CLASS_ATST: 
				case CLASS_MOUSE:
				case CLASS_SAND_CREATURE: 	
				case CLASS_TENTACLE:  
				case CLASS_ELDERGOD: 
				theFxScheduler.PlayEffect( "tusken9/shot", cent->lerpOrigin, forward );
				break;
				// POISON
				case CLASS_SPIDERREAVER3: 
				case CLASS_MINEMONSTER:
				case CLASS_INTERROGATOR: 
				case CLASS_IVY: 
				case CLASS_POISON: 
				case CLASS_GOLEM_POISON: 		
				case CLASS_DEMON_POISON:
				case CLASS_BEAST: 				
				case CLASS_ALCHEMIST: 
				theFxScheduler.PlayEffect( "tusken10/shot", cent->lerpOrigin, forward );
				break;
				// DEFAULT 
				case CLASS_TUSKEN:
				case CLASS_RODIAN: 
				case CLASS_GLIDER: 
				case CLASS_PRISONER: 
				case CLASS_IMPERIAL: 
				case CLASS_HAZARD_TROOPER: 
				case CLASS_CLAW: 
				case CLASS_PLAYER: 
				case CLASS_VEHICLE: 
				case CLASS_FIREREAVER:
				case CLASS_FIRE: 
				case CLASS_HELLGOLEM: 
				case CLASS_GOLEM_FIRE: 
				case CLASS_GOLEM_LAVA: 
				case CLASS_MELC:  
				case CLASS_SUMMONER:
				case CLASS_VAMPIRE_FEAR: 
				case CLASS_VAMPIRE_FEAR2:
				case CLASS_DEMON_FIRE: 
				case CLASS_HUNTER: 
				theFxScheduler.PlayEffect( "tusken/shot", cent->lerpOrigin, forward );;	
				break;  
			default:
				break;
				}
			}
	}
	// theFxScheduler.PlayEffect( "tusken/shot", cent->lerpOrigin, forward );
}

/*
-------------------------
FX_TuskenShotWeaponHitWall
-------------------------
*/
void FX_TuskenShotWeaponHitWall( vec3_t origin, vec3_t normal )
{
	theFxScheduler.PlayEffect( "tusken/hitwall", origin, normal );
}

void FX_Tusken2ShotWeaponHitWall( vec3_t origin, vec3_t normal )
{
	theFxScheduler.PlayEffect( "tusken2/hitwall", origin, normal );
}

void FX_Tusken3ShotWeaponHitWall( vec3_t origin, vec3_t normal )
{
	theFxScheduler.PlayEffect( "tusken3/hitwall", origin, normal );
}

void FX_Tusken4ShotWeaponHitWall( vec3_t origin, vec3_t normal )
{
	theFxScheduler.PlayEffect( "tusken4/hitwall", origin, normal );
}

void FX_Tusken5ShotWeaponHitWall( vec3_t origin, vec3_t normal )
{
	theFxScheduler.PlayEffect( "tusken5/hitwall", origin, normal );
}

void FX_Tusken6ShotWeaponHitWall( vec3_t origin, vec3_t normal )
{
	theFxScheduler.PlayEffect( "tusken6/hitwall", origin, normal );
}

void FX_Tusken7ShotWeaponHitWall( vec3_t origin, vec3_t normal )
{
	theFxScheduler.PlayEffect( "tusken7/hitwall", origin, normal );
}

void FX_Tusken8ShotWeaponHitWall( vec3_t origin, vec3_t normal )
{
	theFxScheduler.PlayEffect( "tusken8/hitwall", origin, normal );
}

void FX_Tusken9ShotWeaponHitWall( vec3_t origin, vec3_t normal )
{
	theFxScheduler.PlayEffect( "tusken9/hitwall", origin, normal );
}

void FX_Tusken10ShotWeaponHitWall( vec3_t origin, vec3_t normal )
{
	theFxScheduler.PlayEffect( "tusken10/hitwall", origin, normal );
}

void FX_Tusken1ShotWeaponHitWall( vec3_t origin, vec3_t normal )
{
	theFxScheduler.PlayEffect( "tusken1/hitwall", origin, normal );
}

void FX_Tusken0ShotWeaponHitWall( vec3_t origin, vec3_t normal )
{
	theFxScheduler.PlayEffect( "tusken0/hitwall", origin, normal );
}

/*
-------------------------
FX_TuskenShotWeaponHitPlayer
-------------------------
*/
void FX_TuskenShotWeaponHitPlayer( gentity_t *hit, vec3_t origin, vec3_t normal, qboolean humanoid )
{
	//temporary? just testing out the damage skin stuff -rww
	if ( hit && hit->client && hit->ghoul2.size() )
	{
		CG_AddGhoul2Mark(cgs.media.bdecal_burnmark1, flrand(3.5, 4.0), origin, normal, hit->s.number,
			hit->client->ps.origin, hit->client->renderInfo.legsYaw, hit->ghoul2, hit->s.modelScale, Q_irand(10000, 13000));
	}
        
	theFxScheduler.PlayEffect( "tusken/hit", origin, normal );
}

void FX_Tusken2ShotWeaponHitPlayer( gentity_t *hit, vec3_t origin, vec3_t normal, qboolean humanoid )
{
	//temporary? just testing out the damage skin stuff -rww
	if ( hit && hit->client && hit->ghoul2.size() )
	{
		CG_AddGhoul2Mark(cgs.media.bdecal_burnmark1, flrand(3.5, 4.0), origin, normal, hit->s.number,
			hit->client->ps.origin, hit->client->renderInfo.legsYaw, hit->ghoul2, hit->s.modelScale, Q_irand(10000, 13000));
	}
        
	theFxScheduler.PlayEffect( "tusken2/hit", origin, normal );
}

void FX_Tusken3ShotWeaponHitPlayer( gentity_t *hit, vec3_t origin, vec3_t normal, qboolean humanoid )
{
	//temporary? just testing out the damage skin stuff -rww
	if ( hit && hit->client && hit->ghoul2.size() )
	{
		CG_AddGhoul2Mark(cgs.media.bdecal_burnmark1, flrand(3.5, 4.0), origin, normal, hit->s.number,
			hit->client->ps.origin, hit->client->renderInfo.legsYaw, hit->ghoul2, hit->s.modelScale, Q_irand(10000, 13000));
	}
        
	theFxScheduler.PlayEffect( "tusken3/hit", origin, normal );
}

void FX_Tusken4ShotWeaponHitPlayer( gentity_t *hit, vec3_t origin, vec3_t normal, qboolean humanoid )
{
	//temporary? just testing out the damage skin stuff -rww
	if ( hit && hit->client && hit->ghoul2.size() )
	{
		CG_AddGhoul2Mark(cgs.media.bdecal_burnmark1, flrand(3.5, 4.0), origin, normal, hit->s.number,
			hit->client->ps.origin, hit->client->renderInfo.legsYaw, hit->ghoul2, hit->s.modelScale, Q_irand(10000, 13000));
	}
        
	theFxScheduler.PlayEffect( "tusken4/hit", origin, normal );
}

void FX_Tusken5ShotWeaponHitPlayer( gentity_t *hit, vec3_t origin, vec3_t normal, qboolean humanoid )
{
	//temporary? just testing out the damage skin stuff -rww
	if ( hit && hit->client && hit->ghoul2.size() )
	{
		CG_AddGhoul2Mark(cgs.media.bdecal_burnmark1, flrand(3.5, 4.0), origin, normal, hit->s.number,
			hit->client->ps.origin, hit->client->renderInfo.legsYaw, hit->ghoul2, hit->s.modelScale, Q_irand(10000, 13000));
	}
        
	theFxScheduler.PlayEffect( "tusken5/hit", origin, normal );
}

void FX_Tusken6ShotWeaponHitPlayer( gentity_t *hit, vec3_t origin, vec3_t normal, qboolean humanoid )
{
	//temporary? just testing out the damage skin stuff -rww
	if ( hit && hit->client && hit->ghoul2.size() )
	{
		CG_AddGhoul2Mark(cgs.media.bdecal_burnmark1, flrand(3.5, 4.0), origin, normal, hit->s.number,
			hit->client->ps.origin, hit->client->renderInfo.legsYaw, hit->ghoul2, hit->s.modelScale, Q_irand(10000, 13000));
	}
        
	theFxScheduler.PlayEffect( "tusken6/hit", origin, normal );
}

void FX_Tusken7ShotWeaponHitPlayer( gentity_t *hit, vec3_t origin, vec3_t normal, qboolean humanoid )
{
	//temporary? just testing out the damage skin stuff -rww
	if ( hit && hit->client && hit->ghoul2.size() )
	{
		CG_AddGhoul2Mark(cgs.media.bdecal_burnmark1, flrand(3.5, 4.0), origin, normal, hit->s.number,
			hit->client->ps.origin, hit->client->renderInfo.legsYaw, hit->ghoul2, hit->s.modelScale, Q_irand(10000, 13000));
	}
        
	theFxScheduler.PlayEffect( "tusken7/hit", origin, normal );
}

void FX_Tusken8ShotWeaponHitPlayer( gentity_t *hit, vec3_t origin, vec3_t normal, qboolean humanoid )
{
	//temporary? just testing out the damage skin stuff -rww
	if ( hit && hit->client && hit->ghoul2.size() )
	{
		CG_AddGhoul2Mark(cgs.media.bdecal_burnmark1, flrand(3.5, 4.0), origin, normal, hit->s.number,
			hit->client->ps.origin, hit->client->renderInfo.legsYaw, hit->ghoul2, hit->s.modelScale, Q_irand(10000, 13000));
	}
        
	theFxScheduler.PlayEffect( "tusken8/hit", origin, normal );
}

void FX_Tusken9ShotWeaponHitPlayer( gentity_t *hit, vec3_t origin, vec3_t normal, qboolean humanoid )
{
	//temporary? just testing out the damage skin stuff -rww
	if ( hit && hit->client && hit->ghoul2.size() )
	{
		CG_AddGhoul2Mark(cgs.media.bdecal_burnmark1, flrand(3.5, 4.0), origin, normal, hit->s.number,
			hit->client->ps.origin, hit->client->renderInfo.legsYaw, hit->ghoul2, hit->s.modelScale, Q_irand(10000, 13000));
	}
        
	theFxScheduler.PlayEffect( "tusken9/hit", origin, normal );
}

void FX_Tusken10ShotWeaponHitPlayer( gentity_t *hit, vec3_t origin, vec3_t normal, qboolean humanoid )
{
	//temporary? just testing out the damage skin stuff -rww
	if ( hit && hit->client && hit->ghoul2.size() )
	{
		CG_AddGhoul2Mark(cgs.media.bdecal_burnmark1, flrand(3.5, 4.0), origin, normal, hit->s.number,
			hit->client->ps.origin, hit->client->renderInfo.legsYaw, hit->ghoul2, hit->s.modelScale, Q_irand(10000, 13000));
	}
        
	theFxScheduler.PlayEffect( "tusken10/hit", origin, normal );
}

void FX_Tusken1ShotWeaponHitPlayer( gentity_t *hit, vec3_t origin, vec3_t normal, qboolean humanoid )
{
	//temporary? just testing out the damage skin stuff -rww
	if ( hit && hit->client && hit->ghoul2.size() )
	{
		CG_AddGhoul2Mark(cgs.media.bdecal_burnmark1, flrand(3.5, 4.0), origin, normal, hit->s.number,
			hit->client->ps.origin, hit->client->renderInfo.legsYaw, hit->ghoul2, hit->s.modelScale, Q_irand(10000, 13000));
	}
        
	theFxScheduler.PlayEffect( "tusken1/hit", origin, normal );
}

void FX_Tusken0ShotWeaponHitPlayer( gentity_t *hit, vec3_t origin, vec3_t normal, qboolean humanoid )
{
	//temporary? just testing out the damage skin stuff -rww
	if ( hit && hit->client && hit->ghoul2.size() )
	{
		CG_AddGhoul2Mark(cgs.media.bdecal_burnmark1, flrand(3.5, 4.0), origin, normal, hit->s.number,
			hit->client->ps.origin, hit->client->renderInfo.legsYaw, hit->ghoul2, hit->s.modelScale, Q_irand(10000, 13000));
	}
        
	theFxScheduler.PlayEffect( "tusken0/hit", origin, normal );
}

