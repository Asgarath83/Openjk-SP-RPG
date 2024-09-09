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

// Concussion Rifle Weapon

// this line must stay at top so the whole PCH thing works...
#include "cg_headers.h"

//#include "cg_local.h"
#include "cg_media.h"
#include "FxScheduler.h"

/*
---------------------------
FX_ConcProjectileThink
---------------------------
*/

void FX_ConcProjectileThink( centity_t *cent, const struct weaponInfo_s *weapon )
{
	vec3_t forward;

	if ( VectorNormalize2( cent->currentState.pos.trDelta, forward ) == 0.0f )
	{
		forward[2] = 1.0f;
	}

	// HELL STRIKE MELCHIAHIM ATTACK  
	if ( cent->gent && cent->gent->owner && cent->gent->owner->client )
		 	{
				switch ( cent->gent->owner->client->NPC_class )
				{
				// HELL STRIKE 
				case CLASS_VAMPIRE_ZOMBIE: 
				case CLASS_GLIDER:
				case CLASS_TUSKEN:
				case CLASS_MELC:
				case CLASS_SUMMONER: 		
				theFxScheduler.PlayEffect( "concussion/shot", cent->lerpOrigin, forward );
				break;
				// BLACK FIRE 
				case CLASS_SWAMPTROOPER:
				case CLASS_FISH: 
				case CLASS_RAHABIM: 
				case CLASS_COMMANDO: 
				case CLASS_LYCAN: 
				case CLASS_CENOBITA: 
				case CLASS_WIZARD: 
				theFxScheduler.PlayEffect( "blackfire/shot", cent->lerpOrigin, forward );
				break;
				// SPIRITH DEATH  
				case CLASS_REBORN:
				case CLASS_ALORA:
				case CLASS_MONMOTHA:
				case CLASS_NOGHRI:
				case CLASS_FLIER2: 
				case CLASS_ZEPH:
				case CLASS_DECEIVER:
				case CLASS_DUMAHIM: 
				case CLASS_VAMPIRE:
				case CLASS_KAIN2: 
				case CLASS_UMAH: 
				case CLASS_NECROMANCER: 
				case CLASS_VAMPIRE_QUEEN:
				case CLASS_VAMPIRE_DARK: 
				case CLASS_VAMPIRE_NECRO: 
				case CLASS_VAMPIRE_NECRO2:
				case CLASS_VAMPIRE_FEAR: 
				case CLASS_VAMPIRE_SOUND: 
				case CLASS_VAMPIRE_LYCAN: 
				case CLASS_VAMPIRE_WATER: 
				case CLASS_VAMPIRE_ARCHER: 
				case CLASS_HALFVAMPIRE: 
				case CLASS_RAZIELHIM:
				case CLASS_ESKANDOR: 
				case CLASS_VAMPIRE_BOUNTY: 
				case CLASS_VAMPIRE_SARAFAN:  
				theFxScheduler.PlayEffect( "spirith_death/shot", cent->lerpOrigin, forward );
				break;
				// IMMOLATE 
				case CLASS_KYLE:
				case CLASS_KAIN: 
				case CLASS_KAIN3:
				case CLASS_TURELIM: 
				case CLASS_VORADOR:
				case CLASS_JANOS:
				case CLASS_CLAW: 
				case CLASS_GOLEM_LAVA: 
				case CLASS_VAMPIRE_FEAR2: 
				case CLASS_VAMPIRE_BLOOD: 
				case CLASS_VAMPIRE_DEMON: 
				theFxScheduler.PlayEffect( "immolate/shot", cent->lerpOrigin, forward );
				break;
				// HOLYIMMOLATE 
				case CLASS_HUNTER: 
				case CLASS_SCOUT:  
				case CLASS_ALCHEMIST: 
				case CLASS_SARAFAN: 
				case CLASS_ARCHER:  
				case CLASS_WARRIOR: 
				case CLASS_TRANDOSHAN:
				case CLASS_STORMTROOPER:
				case CLASS_PRISONER:
				case CLASS_RODIAN:
				case CLASS_IMPWORKER:
				case CLASS_GALAK:
				case CLASS_JAN:
				case CLASS_MARK1:
				case CLASS_MARK2:
				case CLASS_R2D2:
				case CLASS_R5D2:
				case CLASS_SEEKER:
				case CLASS_REMOTE:
				case CLASS_DRUIDREAVER: 
				case CLASS_RAZIEL: 
				case CLASS_HOLY: 
				case CLASS_GOLEM_HOLY: 
				theFxScheduler.PlayEffect( "holyimmolate/shot", cent->lerpOrigin, forward );
				break;
				// NECRO MIND
				case CLASS_PROBE:
				case CLASS_POLTER: 
				case CLASS_REBEL: 
				case CLASS_MENTALIST:
				case CLASS_AIRREAVER: 
				case CLASS_AIR: 
				case CLASS_GOLEM_AIR: 
				case CLASS_BIRD: 
				theFxScheduler.PlayEffect( "necro_fog/shot", cent->lerpOrigin, forward );
				break;
				// NECRO DIMENSION
				case CLASS_HOWLER:
				case CLASS_SONIC: 
				case CLASS_GOLEM_SONIC: 
				case CLASS_LANDO: 
				theFxScheduler.PlayEffect( "necro_dim/shot", cent->lerpOrigin, forward );
				break;
				// NECRO CONFLICT 
				case CLASS_FIREREAVER: 
				case CLASS_FIRE: 
				case CLASS_GOLEM_FIRE: 
				case CLASS_HELLGOLEM: 
				case CLASS_HAZARD_TROOPER:
				case CLASS_GRAN:
				case CLASS_VAMPIRE_FROST: 
				case CLASS_BOBAFETT: 
				case CLASS_WAMPA: 
				case CLASS_DEMON_FIRE: 
				theFxScheduler.PlayEffect( "necro_conflict/shot", cent->lerpOrigin, forward );
				break;
				// NECRO NATURE
				case CLASS_LUKE: 
				case CLASS_IVY: 
				case CLASS_DRUID: 
				theFxScheduler.PlayEffect( "necro_nature/shot", cent->lerpOrigin, forward );
				break;
				// NECRO ENERGY 
				case CLASS_LIGHT: 
				case CLASS_GOLEM_LIGHT: 
				case CLASS_GOLEM_GOLD: 
				case CLASS_MORGANKATARN:
				case CLASS_PROTOCOL: 
				theFxScheduler.PlayEffect( "necro_energy/shot", cent->lerpOrigin, forward );
				break;
				// NECRO ELECTRO 
				case CLASS_LIGHTREAVER: 
				case CLASS_GOLEM_METAL: 
				case CLASS_DEMON_ELECTRO: 
				case CLASS_ASSASSIN_DROID: 
				case CLASS_SABER_DROID: 
				theFxScheduler.PlayEffect( "necro_shock/shot", cent->lerpOrigin, forward );
				break;
				// NECRO POISON
				case CLASS_SPIDERREAVER3: 
				case CLASS_BEAST: 
				case CLASS_MINEMONSTER:
				case CLASS_REELO: 
				case CLASS_POISON: 
				case CLASS_DEMON_POISON: 
				case CLASS_GOLEM_POISON:
				theFxScheduler.PlayEffect( "necro_poison/shot", cent->lerpOrigin, forward );
				break;
				// NECRO TIME 
				case CLASS_ROCKETTROOPER:
				case CLASS_VAMPIRE_ELF3: 
				theFxScheduler.PlayEffect( "necro_time/shot", cent->lerpOrigin, forward );
				break;
				// NECRO STONE  
				case CLASS_GONK:
				case CLASS_LIZARD:
				case CLASS_VAMPIRE_ELF: 
				case CLASS_VAMPIRE_ELF2: 
				case CLASS_EARTHREAVER: 
				case CLASS_EARTH: 
				case CLASS_GOLEM: 
				case CLASS_DEMON_BLADE: 
				theFxScheduler.PlayEffect( "necro_stone/shot", cent->lerpOrigin, forward );
				break;
				// NECRO CHAOS
				case CLASS_PROPHET: 
				case CLASS_TAVION:
				case CLASS_DESANN:
				case CLASS_HYLDEN: 
				case CLASS_HYLDENLORD: 
				case CLASS_ELDERGOD: 
				case CLASS_TENTACLE: 
				case CLASS_ATST:
				case CLASS_SAND_CREATURE:
				case CLASS_MOUSE: 
				case CLASS_SENTRY: 
				theFxScheduler.PlayEffect( "necro_chaos/shot", cent->lerpOrigin, forward );
				break;
				// NECRO DEATH  
				case CLASS_ARIEL: 
				case CLASS_GHOST: 
				case CLASS_REAPER: 
				case CLASS_DREADNOUGHT: 
				case CLASS_VAMPIRE_GHOST: 
				case CLASS_JEDI:
				case CLASS_REAVER: 
				case CLASS_NECROREAVER: 
				case CLASS_THRALL: 
				case CLASS_NECRO: 
				case CLASS_GOLEM_NECRO: 
				case CLASS_JAWA:
				case CLASS_MURJJ: 
				case CLASS_RANCOR: 
				case CLASS_PLAYER:
				case CLASS_VEHICLE: 
				theFxScheduler.PlayEffect( "necro/shot", cent->lerpOrigin, forward );
				break;
				// NECRO SMOG 
				case CLASS_BESPIN_COP: 				
				case CLASS_DARK: 			 
				case CLASS_GOLEM_DARK: 
				case CLASS_TANKREAVER: 
				case CLASS_GOLEM_SMOKE: 
				theFxScheduler.PlayEffect( "necro_smog/shot", cent->lerpOrigin, forward );
				break;
				// NECRO VOID  
				case CLASS_SABOTEUR: 
				case CLASS_SHADOWTROOPER: 
				case CLASS_VOID: 
				case CLASS_GOLEM_VOID: 
				case CLASS_DARKREAVER: 
				theFxScheduler.PlayEffect( "necro_void/shot", cent->lerpOrigin, forward );
				break;
				// NECRO MUD 
				case CLASS_SWAMP:
				case CLASS_INTERROGATOR:
				case CLASS_SHARK: 
				case CLASS_WATER: 
				case CLASS_GOLEM_WATER: 
				case CLASS_UGNAUGHT: 
				case CLASS_SPIDERREAVER2: 
				case CLASS_SPIDERREAVER:
				theFxScheduler.PlayEffect( "necro_mud/shot", cent->lerpOrigin, forward );
				break;
				// NECRO CRYSTAL 
				case CLASS_WEEQUAY:
				case CLASS_BARTENDER:
				case CLASS_GOLEM_ICE: 
				case CLASS_ICEREAVER: 
				case CLASS_CRYSTAL: 
				case CLASS_GALAKMECH: 
				theFxScheduler.PlayEffect( "necro_crystal/shot", cent->lerpOrigin, forward );
					break;	
				// Antimatter rocket					;
				case CLASS_FIGHTER: 
				theFxScheduler.PlayEffect( "concussion0/shot", cent->lerpOrigin, forward );
					break;	
				default:
					break;
				}
			}
	// theFxScheduler.PlayEffect( "concussion/shot", cent->lerpOrigin, forward );
}

void FX_SonicProjectileThink( centity_t *cent, const struct weaponInfo_s *weapon )
{
	vec3_t forward;

	if ( VectorNormalize2( cent->currentState.pos.trDelta, forward ) == 0.0f )
	{
		forward[2] = 1.0f;
	}

	theFxScheduler.PlayEffect( "sonic/shot", cent->lerpOrigin, forward );
}

/*
---------------------------
FX_ConcHitWall
---------------------------
*/

void FX_ConcHitWall( vec3_t origin, vec3_t normal )
{
	theFxScheduler.PlayEffect( "concussion/explosion", origin, normal );
}

void FX_SonicHitWall( vec3_t origin, vec3_t normal )
{
	theFxScheduler.PlayEffect( "sonic/explosion", origin, normal );
}

void FX_BlackFireHitWall( vec3_t origin, vec3_t normal )
{
	theFxScheduler.PlayEffect( "blackfire/explosion", origin, normal );
}

void FX_ImmolationHitWall( vec3_t origin, vec3_t normal )
{
	theFxScheduler.PlayEffect( "immolate/explosion", origin, normal );
}

void FX_HolyImmolationHitWall( vec3_t origin, vec3_t normal )
{
	theFxScheduler.PlayEffect( "holyimmolate/explosion", origin, normal );
}

void FX_SDHitWall( vec3_t origin, vec3_t normal )
{
	theFxScheduler.PlayEffect( "spirith_death/explosion", origin, normal );
}

void FX_MindHitWall( vec3_t origin, vec3_t normal )
{
	theFxScheduler.PlayEffect( "necro_fog/explosion", origin, normal );
}

void FX_DimHitWall( vec3_t origin, vec3_t normal )
{
	theFxScheduler.PlayEffect( "necro_dim/explosion", origin, normal );
}

void FX_ConflictHitWall( vec3_t origin, vec3_t normal )
{
	theFxScheduler.PlayEffect( "necro_conflict/explosion", origin, normal );
}

void FX_NatureHitWall( vec3_t origin, vec3_t normal )
{
	theFxScheduler.PlayEffect( "necro_nature/explosion", origin, normal );
}

void FX_ElectroHitWall( vec3_t origin, vec3_t normal )
{
	theFxScheduler.PlayEffect( "necro_shock/explosion", origin, normal );
}
void FX_EnergyHitWall( vec3_t origin, vec3_t normal )
{
	theFxScheduler.PlayEffect( "necro_energy/explosion", origin, normal );
}

void FX_NecroPoisonHitWall( vec3_t origin, vec3_t normal )
{
	theFxScheduler.PlayEffect( "necro_poison/explosion", origin, normal );
}
void FX_TimeHitWall( vec3_t origin, vec3_t normal )
{
	theFxScheduler.PlayEffect( "necro_time/explosion", origin, normal );
}

void FX_RockHitWall( vec3_t origin, vec3_t normal )
{
	theFxScheduler.PlayEffect( "necro_stone/explosion", origin, normal );
}

void FX_ChaosHitWall( vec3_t origin, vec3_t normal )
{
	theFxScheduler.PlayEffect( "necro_chaos/explosion", origin, normal );
}

void FX_NecroHitWall( vec3_t origin, vec3_t normal )
{
	theFxScheduler.PlayEffect( "necro/explosion", origin, normal );
}

void FX_NecrovoidHitWall( vec3_t origin, vec3_t normal )
{
	theFxScheduler.PlayEffect( "necro_void/explosion", origin, normal );
}

void FX_NecrosmogHitWall( vec3_t origin, vec3_t normal )
{
	theFxScheduler.PlayEffect( "necro_smog/explosion", origin, normal );
}
void FX_NecromudHitWall( vec3_t origin, vec3_t normal )
{
	theFxScheduler.PlayEffect( "necro_mud/explosion", origin, normal );
}

void FX_NecroCrystalHitWall( vec3_t origin, vec3_t normal )
{
	theFxScheduler.PlayEffect( "necro_crystal/explosion", origin, normal );
}

void FX_AMHitWall( vec3_t origin, vec3_t normal )
{
	theFxScheduler.PlayEffect( "concussion0/explosion", origin, normal );
}
/*
---------------------------
FX_ConcHitPlayer
---------------------------
*/

void FX_ConcHitPlayer( vec3_t origin, vec3_t normal, qboolean humanoid )
{
	theFxScheduler.PlayEffect( "concussion/explosion", origin, normal );
}

void FX_SonicHitPlayer( vec3_t origin, vec3_t normal, qboolean humanoid )
{
	theFxScheduler.PlayEffect( "sonic/explosion", origin, normal );
}

void FX_BlackFireHitPlayer( vec3_t origin, vec3_t normal, qboolean humanoid )
{
	theFxScheduler.PlayEffect( "blackfire/explosion", origin, normal );
}

void FX_ImmolationHitPlayer( vec3_t origin, vec3_t normal, qboolean humanoid )
{
	theFxScheduler.PlayEffect( "immolate/explosion", origin, normal );
}

void FX_HolyImmolationHitPlayer( vec3_t origin, vec3_t normal, qboolean humanoid )
{
	theFxScheduler.PlayEffect( "holyimmolate/explosion", origin, normal );
}

void FX_SDHitPlayer( vec3_t origin, vec3_t normal, qboolean humanoid )
{
	theFxScheduler.PlayEffect( "spirith_death/explosion", origin, normal );
}

void FX_MindHitPlayer( vec3_t origin, vec3_t normal, qboolean humanoid )
{
	theFxScheduler.PlayEffect( "necro_fog/explosion", origin, normal );
}

void FX_DimHitPlayer( vec3_t origin, vec3_t normal, qboolean humanoid )
{
	theFxScheduler.PlayEffect( "necro_dim/explosion", origin, normal );
}

void FX_ConflictHitPlayer( vec3_t origin, vec3_t normal, qboolean humanoid )
{
	theFxScheduler.PlayEffect( "necro_conflict/explosion", origin, normal );
}

void FX_NatureHitPlayer( vec3_t origin, vec3_t normal, qboolean humanoid )
{
	theFxScheduler.PlayEffect( "necro_nature/explosion", origin, normal );
}

void FX_ElectroHitPlayer( vec3_t origin, vec3_t normal, qboolean humanoid )
{
	theFxScheduler.PlayEffect( "necro_shock/explosion", origin, normal );
}
void FX_EnergyHitPlayer( vec3_t origin, vec3_t normal, qboolean humanoid )
{
	theFxScheduler.PlayEffect( "necro_energy/explosion", origin, normal );
}

void FX_NecroPoisonHitPlayer( vec3_t origin, vec3_t normal, qboolean humanoid )
{
	theFxScheduler.PlayEffect( "necro_poison/explosion", origin, normal );
}
void FX_TimeHitPlayer( vec3_t origin, vec3_t normal, qboolean humanoid )
{
	theFxScheduler.PlayEffect( "necro_time/explosion", origin, normal );
}

void FX_RockHitPlayer( vec3_t origin, vec3_t normal, qboolean humanoid )
{
	theFxScheduler.PlayEffect( "necro_stone/explosion", origin, normal );
}

void FX_ChaosHitPlayer( vec3_t origin, vec3_t normal, qboolean humanoid )
{
	theFxScheduler.PlayEffect( "necro_chaos/explosion", origin, normal );
}

void FX_NecroHitPlayer( vec3_t origin, vec3_t normal, qboolean humanoid )
{
	theFxScheduler.PlayEffect( "necro/explosion", origin, normal );
}

void FX_NecrovoidHitPlayer( vec3_t origin, vec3_t normal, qboolean humanoid )
{
	theFxScheduler.PlayEffect( "necro_void/explosion", origin, normal );
}

void FX_NecrosmogHitPlayer( vec3_t origin, vec3_t normal, qboolean humanoid )
{
	theFxScheduler.PlayEffect( "necro_smog/explosion", origin, normal );
}
void FX_NecromudHitPlayer( vec3_t origin, vec3_t normal, qboolean humanoid )
{
	theFxScheduler.PlayEffect( "necro_mud/explosion", origin, normal );
}

void FX_NecroCrystalHitPlayer( vec3_t origin, vec3_t normal, qboolean humanoid )
{
	theFxScheduler.PlayEffect( "necro_crystal/explosion", origin, normal );
}

void FX_AMHitPlayer( vec3_t origin, vec3_t normal, qboolean humanoid )
{
	theFxScheduler.PlayEffect( "concussion0/explosion", origin, normal );
}

/*
---------------------------
FX_ConcAltShot
---------------------------
*/
static vec3_t WHITE	={1.0f,1.0f,1.0f};

void FX_ConcAltShot( vec3_t start, vec3_t end )
{
	//"concussion/beam"
	FX_AddLine( -1, start, end, 0.1f, 10.0f, 0.0f, 
							1.0f, 0.0f, 0.0f,
							WHITE, WHITE, 0.0f,
							175, cgi_R_RegisterShader( "gfx/effects/blueLine" ), 
							0, FX_SIZE_LINEAR | FX_ALPHA_LINEAR );

	vec3_t	BRIGHT={0.75f,0.5f,1.0f};

	// add some beef
	FX_AddLine( -1, start, end, 0.1f, 7.0f, 0.0f, 
						1.0f, 0.0f, 0.0f,
						BRIGHT, BRIGHT, 0.0f,
						150, cgi_R_RegisterShader( "gfx/misc/whiteline2" ), 
						0, FX_SIZE_LINEAR | FX_ALPHA_LINEAR );
}

void FX_SonicAltShot( vec3_t start, vec3_t end )
{
	//"concussion/beam"
	FX_AddLine( -1, start, end, 0.1f, 10.0f, 0.0f, 
							1.0f, 0.0f, 0.0f,
							WHITE, WHITE, 0.0f,
							175, cgi_R_RegisterShader( "gfx/effects/sabers/purple_Line" ), 
							0, FX_SIZE_LINEAR | FX_ALPHA_LINEAR );

	vec3_t	BRIGHT={0.75f,0.5f,1.0f};

	// add some beef
	FX_AddLine( -1, start, end, 0.1f, 7.0f, 0.0f, 
						1.0f, 0.0f, 0.0f,
						BRIGHT, BRIGHT, 0.0f,
						150, cgi_R_RegisterShader( "gfx/misc/whiteline2" ), 
						0, FX_SIZE_LINEAR | FX_ALPHA_LINEAR );
}

void FX_BlackFireAltShot( vec3_t start, vec3_t end )
{
	//"concussion/beam"
	FX_AddLine( -1, start, end, 0.1f, 10.0f, 0.0f, 
							1.0f, 0.0f, 0.0f,
							WHITE, WHITE, 0.0f,
							175, cgi_R_RegisterShader( "gfx/effects/blueLine" ), 
							0, FX_SIZE_LINEAR | FX_ALPHA_LINEAR );

	vec3_t	BRIGHT={0.75f,0.5f,1.0f};

	// add some beef
	FX_AddLine( -1, start, end, 0.1f, 7.0f, 0.0f, 
						1.0f, 0.0f, 0.0f,
						BRIGHT, BRIGHT, 0.0f,
						150, cgi_R_RegisterShader( "gfx/effects/sabers/purple_Line" ), 
						0, FX_SIZE_LINEAR | FX_ALPHA_LINEAR );
}

void FX_ImmolationAltShot( vec3_t start, vec3_t end )
{
	//"concussion/beam"
	FX_AddLine( -1, start, end, 0.1f, 10.0f, 0.0f, 
							1.0f, 0.0f, 0.0f,
							WHITE, WHITE, 0.0f,
							175, cgi_R_RegisterShader( "gfx/effects/blueLine" ), 
							0, FX_SIZE_LINEAR | FX_ALPHA_LINEAR );

	vec3_t	BRIGHT={0.75f,0.5f,1.0f};

	// add some beef
	FX_AddLine( -1, start, end, 0.1f, 7.0f, 0.0f, 
						1.0f, 0.0f, 0.0f,
						BRIGHT, BRIGHT, 0.0f,
						150, cgi_R_RegisterShader( "gfx/misc/whiteline2" ), 
						0, FX_SIZE_LINEAR | FX_ALPHA_LINEAR );
}

void FX_HolyImmolationAltShot( vec3_t start, vec3_t end )
{
	//"concussion/beam"
	FX_AddLine( -1, start, end, 0.1f, 10.0f, 0.0f, 
							1.0f, 0.0f, 0.0f,
							WHITE, WHITE, 0.0f,
							175, cgi_R_RegisterShader( "gfx/effects/sabers/blue_Line" ), 
							0, FX_SIZE_LINEAR | FX_ALPHA_LINEAR );

	vec3_t	BRIGHT={0.75f,0.5f,1.0f};

	// add some beef
	FX_AddLine( -1, start, end, 0.1f, 7.0f, 0.0f, 
						1.0f, 0.0f, 0.0f,
						BRIGHT, BRIGHT, 0.0f,
						150, cgi_R_RegisterShader( "gfx/misc/whiteline2" ), 
						0, FX_SIZE_LINEAR | FX_ALPHA_LINEAR );
}

void FX_SDAltShot( vec3_t start, vec3_t end )
{
	//"concussion/beam"
	FX_AddLine( -1, start, end, 0.1f, 10.0f, 0.0f, 
							1.0f, 0.0f, 0.0f,
							WHITE, WHITE, 0.0f,
							175, cgi_R_RegisterShader( "gfx/effects/blueLine" ), 
							0, FX_SIZE_LINEAR | FX_ALPHA_LINEAR );

	vec3_t	BRIGHT={0.75f,0.5f,1.0f};

	// add some beef
	FX_AddLine( -1, start, end, 0.1f, 7.0f, 0.0f, 
						1.0f, 0.0f, 0.0f,
						BRIGHT, BRIGHT, 0.0f,
						150, cgi_R_RegisterShader( "gfx/misc/whiteline2" ), 
						0, FX_SIZE_LINEAR | FX_ALPHA_LINEAR );
}

void FX_MindAltShot( vec3_t start, vec3_t end )
{
	//"concussion/beam"
	FX_AddLine( -1, start, end, 0.1f, 10.0f, 0.0f, 
							1.0f, 0.0f, 0.0f,
							WHITE, WHITE, 0.0f,
							175, cgi_R_RegisterShader( "gfx/effects/sabers/orange_Line" ), 
							0, FX_SIZE_LINEAR | FX_ALPHA_LINEAR );

	vec3_t	BRIGHT={0.75f,0.5f,1.0f};

	// add some beef
	FX_AddLine( -1, start, end, 0.1f, 7.0f, 0.0f, 
						1.0f, 0.0f, 0.0f,
						BRIGHT, BRIGHT, 0.0f,
						150, cgi_R_RegisterShader( "gfx/misc/whiteline2" ), 
						0, FX_SIZE_LINEAR | FX_ALPHA_LINEAR );
}

void FX_DimAltShot( vec3_t start, vec3_t end )
{
	//"concussion/beam"
	FX_AddLine( -1, start, end, 0.1f, 10.0f, 0.0f, 
							1.0f, 0.0f, 0.0f,
							WHITE, WHITE, 0.0f,
							175, cgi_R_RegisterShader( "gfx/effects/sabers/green_Line" ), 
							0, FX_SIZE_LINEAR | FX_ALPHA_LINEAR );

	vec3_t	BRIGHT={0.75f,0.5f,1.0f};

	// add some beef
	FX_AddLine( -1, start, end, 0.1f, 7.0f, 0.0f, 
						1.0f, 0.0f, 0.0f,
						BRIGHT, BRIGHT, 0.0f,
						150, cgi_R_RegisterShader( "gfx/misc/whiteline2" ), 
						0, FX_SIZE_LINEAR | FX_ALPHA_LINEAR );
}



void FX_ConflictAltShot( vec3_t start, vec3_t end )
{
	//"concussion/beam"
	FX_AddLine( -1, start, end, 0.1f, 10.0f, 0.0f, 
							1.0f, 0.0f, 0.0f,
							WHITE, WHITE, 0.0f,
							175, cgi_R_RegisterShader( "gfx/effects/sabers/red_line" ), 
							0, FX_SIZE_LINEAR | FX_ALPHA_LINEAR );

	vec3_t	BRIGHT={0.75f,0.5f,1.0f};

	// add some beef
	FX_AddLine( -1, start, end, 0.1f, 7.0f, 0.0f, 
						1.0f, 0.0f, 0.0f,
						BRIGHT, BRIGHT, 0.0f,
						150, cgi_R_RegisterShader( "gfx/misc/whiteline2" ), 
						0, FX_SIZE_LINEAR | FX_ALPHA_LINEAR );
}

void FX_NatureAltShot( vec3_t start, vec3_t end )
{
	//"concussion/beam"
	FX_AddLine( -1, start, end, 0.1f, 10.0f, 0.0f, 
							1.0f, 0.0f, 0.0f,
							WHITE, WHITE, 0.0f,
							175, cgi_R_RegisterShader( "gfx/effects/blueLine" ), 
							0, FX_SIZE_LINEAR | FX_ALPHA_LINEAR );

	vec3_t	BRIGHT={0.75f,0.5f,1.0f};

	// add some beef
	FX_AddLine( -1, start, end, 0.1f, 7.0f, 0.0f, 
						1.0f, 0.0f, 0.0f,
						BRIGHT, BRIGHT, 0.0f,
						150, cgi_R_RegisterShader( "gfx/effects/sabers/green_line" ), 
						0, FX_SIZE_LINEAR | FX_ALPHA_LINEAR );
}

void FX_ElectroAltShot( vec3_t start, vec3_t end )
{
	//"concussion/beam"
	FX_AddLine( -1, start, end, 0.1f, 10.0f, 0.0f, 
							1.0f, 0.0f, 0.0f,
							WHITE, WHITE, 0.0f,
							175, cgi_R_RegisterShader( "gfx/misc/blueline" ), 
							0, FX_SIZE_LINEAR | FX_ALPHA_LINEAR );

	vec3_t	BRIGHT={0.75f,0.5f,1.0f};

	// add some beef
	FX_AddLine( -1, start, end, 0.1f, 7.0f, 0.0f, 
						1.0f, 0.0f, 0.0f,
						BRIGHT, BRIGHT, 0.0f,
						150, cgi_R_RegisterShader( "gfx/misc/whiteline2" ), 
						0, FX_SIZE_LINEAR | FX_ALPHA_LINEAR );
}

void FX_EnergyAltShot( vec3_t start, vec3_t end )
{
	//"concussion/beam"
	FX_AddLine( -1, start, end, 0.1f, 10.0f, 0.0f, 
							1.0f, 0.0f, 0.0f,
							WHITE, WHITE, 0.0f,
							175, cgi_R_RegisterShader( "gfx/effects/sabers/yellow_Line" ), 
							0, FX_SIZE_LINEAR | FX_ALPHA_LINEAR );

	vec3_t	BRIGHT={0.75f,0.5f,1.0f};

	// add some beef
	FX_AddLine( -1, start, end, 0.1f, 7.0f, 0.0f, 
						1.0f, 0.0f, 0.0f,
						BRIGHT, BRIGHT, 0.0f,
						150, cgi_R_RegisterShader( "gfx/misc/whiteline2" ), 
						0, FX_SIZE_LINEAR | FX_ALPHA_LINEAR );
}

void FX_NecroPoisonAltShot( vec3_t start, vec3_t end )
{
	//"concussion/beam"
	FX_AddLine( -1, start, end, 0.1f, 10.0f, 0.0f, 
							1.0f, 0.0f, 0.0f,
							WHITE, WHITE, 0.0f,
							175, cgi_R_RegisterShader( "gfx/effects/sabers/green_Line" ), 
							0, FX_SIZE_LINEAR | FX_ALPHA_LINEAR );

	vec3_t	BRIGHT={0.75f,0.5f,1.0f};

	// add some beef
	FX_AddLine( -1, start, end, 0.1f, 7.0f, 0.0f, 
						1.0f, 0.0f, 0.0f,
						BRIGHT, BRIGHT, 0.0f,
						150, cgi_R_RegisterShader( "gfx/misc/whiteline2" ), 
						0, FX_SIZE_LINEAR | FX_ALPHA_LINEAR );
}


void FX_TimeAltShot( vec3_t start, vec3_t end )
{
	//"concussion/beam"
	FX_AddLine( -1, start, end, 0.1f, 10.0f, 0.0f, 
							1.0f, 0.0f, 0.0f,
							WHITE, WHITE, 0.0f,
							175, cgi_R_RegisterShader( "gfx/effects/blueLine" ), 
							0, FX_SIZE_LINEAR | FX_ALPHA_LINEAR );

	vec3_t	BRIGHT={0.75f,0.5f,1.0f};

	// add some beef
	FX_AddLine( -1, start, end, 0.1f, 7.0f, 0.0f, 
						1.0f, 0.0f, 0.0f,
						BRIGHT, BRIGHT, 0.0f,
						150, cgi_R_RegisterShader( "gfx/effects/blueline" ), 
						0, FX_SIZE_LINEAR | FX_ALPHA_LINEAR );
}

void FX_RockAltShot( vec3_t start, vec3_t end )
{
	//"concussion/beam"
	FX_AddLine( -1, start, end, 0.1f, 10.0f, 0.0f, 
							1.0f, 0.0f, 0.0f,
							WHITE, WHITE, 0.0f,
							175, cgi_R_RegisterShader( "gfx/effects/sabers/yellow_Line" ), 
							0, FX_SIZE_LINEAR | FX_ALPHA_LINEAR );

	vec3_t	BRIGHT={0.75f,0.5f,1.0f};

	// add some beef
	FX_AddLine( -1, start, end, 0.1f, 7.0f, 0.0f, 
						1.0f, 0.0f, 0.0f,
						BRIGHT, BRIGHT, 0.0f,
						150, cgi_R_RegisterShader( "gfx/effects/sabers/red_line" ), 
						0, FX_SIZE_LINEAR | FX_ALPHA_LINEAR );
}

void FX_ChaosAltShot( vec3_t start, vec3_t end )
{
	//"concussion/beam"
	FX_AddLine( -1, start, end, 0.1f, 10.0f, 0.0f, 
							1.0f, 0.0f, 0.0f,
							WHITE, WHITE, 0.0f,
							175, cgi_R_RegisterShader( "gfx/effects/sabers/purple_line" ), 
							0, FX_SIZE_LINEAR | FX_ALPHA_LINEAR );

	vec3_t	BRIGHT={0.75f,0.5f,1.0f};

	// add some beef
	FX_AddLine( -1, start, end, 0.1f, 7.0f, 0.0f, 
						1.0f, 0.0f, 0.0f,
						BRIGHT, BRIGHT, 0.0f,
						150, cgi_R_RegisterShader( "gfx/effects/sabers/green_line" ), 
						0, FX_SIZE_LINEAR | FX_ALPHA_LINEAR );
}

void FX_NecroAltShot( vec3_t start, vec3_t end )
{
	//"concussion/beam"
	FX_AddLine( -1, start, end, 0.1f, 10.0f, 0.0f, 
							1.0f, 0.0f, 0.0f,
							WHITE, WHITE, 0.0f,
							175, cgi_R_RegisterShader( "gfx/effects/sabers/blue_line" ), 
							0, FX_SIZE_LINEAR | FX_ALPHA_LINEAR );

	vec3_t	BRIGHT={0.75f,0.5f,1.0f};

	// add some beef
	FX_AddLine( -1, start, end, 0.1f, 7.0f, 0.0f, 
						1.0f, 0.0f, 0.0f,
						BRIGHT, BRIGHT, 0.0f,
						150, cgi_R_RegisterShader( "gfx/effects/sabers/green_line" ), 
						0, FX_SIZE_LINEAR | FX_ALPHA_LINEAR );
}

void FX_NecrosmogAltShot( vec3_t start, vec3_t end )
{
	//"concussion/beam"
	FX_AddLine( -1, start, end, 0.1f, 10.0f, 0.0f, 
							1.0f, 0.0f, 0.0f,
							WHITE, WHITE, 0.0f,
							175, cgi_R_RegisterShader( "gfx/damage/burnmark3" ), 
							0, FX_SIZE_LINEAR | FX_ALPHA_LINEAR );

	vec3_t	BRIGHT={0.75f,0.5f,1.0f};

	// add some beef
	FX_AddLine( -1, start, end, 0.1f, 7.0f, 0.0f, 
						1.0f, 0.0f, 0.0f,
						BRIGHT, BRIGHT, 0.0f,
						150, cgi_R_RegisterShader( "gfx/misc/whiteline2" ), 
						0, FX_SIZE_LINEAR | FX_ALPHA_LINEAR );
}

void FX_NecrovoidAltShot( vec3_t start, vec3_t end )
{
	//"concussion/beam"
	FX_AddLine( -1, start, end, 0.1f, 10.0f, 0.0f, 
							1.0f, 0.0f, 0.0f,
							WHITE, WHITE, 0.0f,
							175, cgi_R_RegisterShader( "gfx/damage/burnmark3" ), 
							0, FX_SIZE_LINEAR | FX_ALPHA_LINEAR );

	vec3_t	BRIGHT={0.75f,0.5f,1.0f};

	// add some beef
	FX_AddLine( -1, start, end, 0.1f, 7.0f, 0.0f, 
						1.0f, 0.0f, 0.0f,
						BRIGHT, BRIGHT, 0.0f,
						150, cgi_R_RegisterShader( "gfx/effects/black_smoke" ), 
						0, FX_SIZE_LINEAR | FX_ALPHA_LINEAR );
}

void FX_NecromudAltShot( vec3_t start, vec3_t end )
{
	//"concussion/beam"
	FX_AddLine( -1, start, end, 0.1f, 10.0f, 0.0f, 
							1.0f, 0.0f, 0.0f,
							WHITE, WHITE, 0.0f,
							175, cgi_R_RegisterShader( "gfx/effects/sabers/yellow_Line" ), 
							0, FX_SIZE_LINEAR | FX_ALPHA_LINEAR );

	vec3_t	BRIGHT={0.75f,0.5f,1.0f};

	// add some beef
	FX_AddLine( -1, start, end, 0.1f, 7.0f, 0.0f, 
						1.0f, 0.0f, 0.0f,
						BRIGHT, BRIGHT, 0.0f,
						150, cgi_R_RegisterShader( "gfx/effects/sabers/green_line" ), 
						0, FX_SIZE_LINEAR | FX_ALPHA_LINEAR );
}

void FX_NecroCrystalAltShot( vec3_t start, vec3_t end )
{
	//"concussion/beam"
	FX_AddLine( -1, start, end, 0.1f, 10.0f, 0.0f, 
							1.0f, 0.0f, 0.0f,
							WHITE, WHITE, 0.0f,
							175, cgi_R_RegisterShader( "gfx/effects/sabers/blue_Line" ), 
							0, FX_SIZE_LINEAR | FX_ALPHA_LINEAR );

	vec3_t	BRIGHT={0.75f,0.5f,1.0f};

	// add some beef
	FX_AddLine( -1, start, end, 0.1f, 7.0f, 0.0f, 
						1.0f, 0.0f, 0.0f,
						BRIGHT, BRIGHT, 0.0f,
						150, cgi_R_RegisterShader( "gfx/misc/whiteline2" ), 
						0, FX_SIZE_LINEAR | FX_ALPHA_LINEAR );
}

void FX_AMAltShot( vec3_t start, vec3_t end )
{
	//"concussion/beam"
	FX_AddLine( -1, start, end, 0.1f, 10.0f, 0.0f, 
							1.0f, 0.0f, 0.0f,
							WHITE, WHITE, 0.0f,
							175, cgi_R_RegisterShader( "gfx/effects/sabers/yellow_Line" ), 
							0, FX_SIZE_LINEAR | FX_ALPHA_LINEAR );

	vec3_t	BRIGHT={0.75f,0.5f,1.0f};

	// add some beef
	FX_AddLine( -1, start, end, 0.1f, 7.0f, 0.0f, 
						1.0f, 0.0f, 0.0f,
						BRIGHT, BRIGHT, 0.0f,
						150, cgi_R_RegisterShader( "gfx/misc/whiteline2" ), 
						0, FX_SIZE_LINEAR | FX_ALPHA_LINEAR );
}

void FX_ConcAltMiss( vec3_t origin, vec3_t normal )
{
	//gentity_t self;
	
	vec3_t pos, c1, c2;

	VectorMA( origin, 4.0f, normal, c1 );
	VectorCopy( c1, c2 );
	c1[2] += 4;
	c2[2] += 12;
	
	VectorAdd( origin, normal, pos );
	pos[2] += 28;

	FX_AddBezier( origin, pos, c1, vec3_origin, c2, vec3_origin, 6.0f, 6.0f, 0.0f, 0.0f, 0.2f, 0.5f, WHITE, WHITE, 0.0f, 4000, cgi_R_RegisterShader( "gfx/effects/smokeTrail" ), FX_ALPHA_WAVE );

	theFxScheduler.PlayEffect( "concussion/alt_miss", origin, normal );
}

void FX_SonicAltMiss( vec3_t origin, vec3_t normal )
{
	vec3_t pos, c1, c2;

	VectorMA( origin, 4.0f, normal, c1 );
	VectorCopy( c1, c2 );
	c1[2] += 4;
	c2[2] += 12;
	
	VectorAdd( origin, normal, pos );
	pos[2] += 28;

	FX_AddBezier( origin, pos, c1, vec3_origin, c2, vec3_origin, 6.0f, 6.0f, 0.0f, 0.0f, 0.2f, 0.5f, WHITE, WHITE, 0.0f, 4000, cgi_R_RegisterShader( "gfx/effects/smokeTrail" ), FX_ALPHA_WAVE );

	theFxScheduler.PlayEffect( "sonic/alt_miss", origin, normal );
}

void FX_BlackFireAltMiss( vec3_t origin, vec3_t normal )
{
	vec3_t pos, c1, c2;

	VectorMA( origin, 4.0f, normal, c1 );
	VectorCopy( c1, c2 );
	c1[2] += 4;
	c2[2] += 12;
	
	VectorAdd( origin, normal, pos );
	pos[2] += 28;

	FX_AddBezier( origin, pos, c1, vec3_origin, c2, vec3_origin, 6.0f, 6.0f, 0.0f, 0.0f, 0.2f, 0.5f, WHITE, WHITE, 0.0f, 4000, cgi_R_RegisterShader( "gfx/effects/smokeTrail" ), FX_ALPHA_WAVE );

	theFxScheduler.PlayEffect( "blackfire/alt_miss", origin, normal );
}

void FX_ImmolationAltMiss( vec3_t origin, vec3_t normal )
{
	vec3_t pos, c1, c2;

	VectorMA( origin, 4.0f, normal, c1 );
	VectorCopy( c1, c2 );
	c1[2] += 4;
	c2[2] += 12;
	
	VectorAdd( origin, normal, pos );
	pos[2] += 28;

	FX_AddBezier( origin, pos, c1, vec3_origin, c2, vec3_origin, 6.0f, 6.0f, 0.0f, 0.0f, 0.2f, 0.5f, WHITE, WHITE, 0.0f, 4000, cgi_R_RegisterShader( "gfx/effects/smokeTrail" ), FX_ALPHA_WAVE );

	theFxScheduler.PlayEffect( "immolate/alt_miss", origin, normal );
}

void FX_HolyImmolationAltMiss( vec3_t origin, vec3_t normal )
{
	vec3_t pos, c1, c2;

	VectorMA( origin, 4.0f, normal, c1 );
	VectorCopy( c1, c2 );
	c1[2] += 4;
	c2[2] += 12;
	
	VectorAdd( origin, normal, pos );
	pos[2] += 28;

	FX_AddBezier( origin, pos, c1, vec3_origin, c2, vec3_origin, 6.0f, 6.0f, 0.0f, 0.0f, 0.2f, 0.5f, WHITE, WHITE, 0.0f, 4000, cgi_R_RegisterShader( "gfx/effects/smokeTrail" ), FX_ALPHA_WAVE );

	theFxScheduler.PlayEffect( "holyimmolate/alt_miss", origin, normal );
}

void FX_SDAltMiss( vec3_t origin, vec3_t normal )
{
	vec3_t pos, c1, c2;

	VectorMA( origin, 4.0f, normal, c1 );
	VectorCopy( c1, c2 );
	c1[2] += 4;
	c2[2] += 12;
	
	VectorAdd( origin, normal, pos );
	pos[2] += 28;

	FX_AddBezier( origin, pos, c1, vec3_origin, c2, vec3_origin, 6.0f, 6.0f, 0.0f, 0.0f, 0.2f, 0.5f, WHITE, WHITE, 0.0f, 4000, cgi_R_RegisterShader( "gfx/effects/smokeTrail" ), FX_ALPHA_WAVE );

	theFxScheduler.PlayEffect( "spirith_death/alt_miss", origin, normal );
}

void FX_MindAltMiss( vec3_t origin, vec3_t normal )
{
	vec3_t pos, c1, c2;

	VectorMA( origin, 4.0f, normal, c1 );
	VectorCopy( c1, c2 );
	c1[2] += 4;
	c2[2] += 12;
	
	VectorAdd( origin, normal, pos );
	pos[2] += 28;

	FX_AddBezier( origin, pos, c1, vec3_origin, c2, vec3_origin, 6.0f, 6.0f, 0.0f, 0.0f, 0.2f, 0.5f, WHITE, WHITE, 0.0f, 4000, cgi_R_RegisterShader( "gfx/effects/smokeTrail" ), FX_ALPHA_WAVE );

	theFxScheduler.PlayEffect( "necro_fog/alt_miss", origin, normal );
}

void FX_DimAltMiss( vec3_t origin, vec3_t normal )
{
	vec3_t pos, c1, c2;

	VectorMA( origin, 4.0f, normal, c1 );
	VectorCopy( c1, c2 );
	c1[2] += 4;
	c2[2] += 12;
	
	VectorAdd( origin, normal, pos );
	pos[2] += 28;

	FX_AddBezier( origin, pos, c1, vec3_origin, c2, vec3_origin, 6.0f, 6.0f, 0.0f, 0.0f, 0.2f, 0.5f, WHITE, WHITE, 0.0f, 4000, cgi_R_RegisterShader( "gfx/effects/smokeTrail" ), FX_ALPHA_WAVE );

	theFxScheduler.PlayEffect( "necro_dim/alt_miss", origin, normal );
}

void FX_ConflictAltMiss( vec3_t origin, vec3_t normal )
{
	vec3_t pos, c1, c2;

	VectorMA( origin, 4.0f, normal, c1 );
	VectorCopy( c1, c2 );
	c1[2] += 4;
	c2[2] += 12;
	
	VectorAdd( origin, normal, pos );
	pos[2] += 28;

	FX_AddBezier( origin, pos, c1, vec3_origin, c2, vec3_origin, 6.0f, 6.0f, 0.0f, 0.0f, 0.2f, 0.5f, WHITE, WHITE, 0.0f, 4000, cgi_R_RegisterShader( "gfx/effects/smokeTrail" ), FX_ALPHA_WAVE );

	theFxScheduler.PlayEffect( "necro_conflict/alt_miss", origin, normal );
}

void FX_NatureAltMiss( vec3_t origin, vec3_t normal )
{
	vec3_t pos, c1, c2;

	VectorMA( origin, 4.0f, normal, c1 );
	VectorCopy( c1, c2 );
	c1[2] += 4;
	c2[2] += 12;
	
	VectorAdd( origin, normal, pos );
	pos[2] += 28;

	FX_AddBezier( origin, pos, c1, vec3_origin, c2, vec3_origin, 6.0f, 6.0f, 0.0f, 0.0f, 0.2f, 0.5f, WHITE, WHITE, 0.0f, 4000, cgi_R_RegisterShader( "gfx/effects/smokeTrail" ), FX_ALPHA_WAVE );

	theFxScheduler.PlayEffect( "necro_nature/alt_miss", origin, normal );
}

void FX_ElectroAltMiss( vec3_t origin, vec3_t normal )
{
	vec3_t pos, c1, c2;

	VectorMA( origin, 4.0f, normal, c1 );
	VectorCopy( c1, c2 );
	c1[2] += 4;
	c2[2] += 12;
	
	VectorAdd( origin, normal, pos );
	pos[2] += 28;

	FX_AddBezier( origin, pos, c1, vec3_origin, c2, vec3_origin, 6.0f, 6.0f, 0.0f, 0.0f, 0.2f, 0.5f, WHITE, WHITE, 0.0f, 4000, cgi_R_RegisterShader( "gfx/effects/smokeTrail" ), FX_ALPHA_WAVE );

	theFxScheduler.PlayEffect( "necro_shock/alt_miss", origin, normal );
}

void FX_EnergyAltMiss( vec3_t origin, vec3_t normal )
{
	vec3_t pos, c1, c2;

	VectorMA( origin, 4.0f, normal, c1 );
	VectorCopy( c1, c2 );
	c1[2] += 4;
	c2[2] += 12;
	
	VectorAdd( origin, normal, pos );
	pos[2] += 28;

	FX_AddBezier( origin, pos, c1, vec3_origin, c2, vec3_origin, 6.0f, 6.0f, 0.0f, 0.0f, 0.2f, 0.5f, WHITE, WHITE, 0.0f, 4000, cgi_R_RegisterShader( "gfx/effects/smokeTrail" ), FX_ALPHA_WAVE );

	theFxScheduler.PlayEffect( "necro_energy/alt_miss", origin, normal );
}

void FX_NecroPoisonAltMiss( vec3_t origin, vec3_t normal )
{
	vec3_t pos, c1, c2;

	VectorMA( origin, 4.0f, normal, c1 );
	VectorCopy( c1, c2 );
	c1[2] += 4;
	c2[2] += 12;
	
	VectorAdd( origin, normal, pos );
	pos[2] += 28;

	FX_AddBezier( origin, pos, c1, vec3_origin, c2, vec3_origin, 6.0f, 6.0f, 0.0f, 0.0f, 0.2f, 0.5f, WHITE, WHITE, 0.0f, 4000, cgi_R_RegisterShader( "gfx/effects/smokeTrail" ), FX_ALPHA_WAVE );

	theFxScheduler.PlayEffect( "necro_poison/alt_miss", origin, normal );
}
void FX_TimeAltMiss( vec3_t origin, vec3_t normal )
{
	vec3_t pos, c1, c2;

	VectorMA( origin, 4.0f, normal, c1 );
	VectorCopy( c1, c2 );
	c1[2] += 4;
	c2[2] += 12;
	
	VectorAdd( origin, normal, pos );
	pos[2] += 28;

	FX_AddBezier( origin, pos, c1, vec3_origin, c2, vec3_origin, 6.0f, 6.0f, 0.0f, 0.0f, 0.2f, 0.5f, WHITE, WHITE, 0.0f, 4000, cgi_R_RegisterShader( "gfx/effects/smokeTrail" ), FX_ALPHA_WAVE );

	theFxScheduler.PlayEffect( "necro_time/alt_miss", origin, normal );
}

void FX_RockAltMiss( vec3_t origin, vec3_t normal )
{
	vec3_t pos, c1, c2;

	VectorMA( origin, 4.0f, normal, c1 );
	VectorCopy( c1, c2 );
	c1[2] += 4;
	c2[2] += 12;
	
	VectorAdd( origin, normal, pos );
	pos[2] += 28;

	FX_AddBezier( origin, pos, c1, vec3_origin, c2, vec3_origin, 6.0f, 6.0f, 0.0f, 0.0f, 0.2f, 0.5f, WHITE, WHITE, 0.0f, 4000, cgi_R_RegisterShader( "gfx/effects/smokeTrail" ), FX_ALPHA_WAVE );

	theFxScheduler.PlayEffect( "necro_stone/alt_miss", origin, normal );
}

void FX_ChaosAltMiss( vec3_t origin, vec3_t normal )
{
	vec3_t pos, c1, c2;

	VectorMA( origin, 4.0f, normal, c1 );
	VectorCopy( c1, c2 );
	c1[2] += 4;
	c2[2] += 12;
	
	VectorAdd( origin, normal, pos );
	pos[2] += 28;

	FX_AddBezier( origin, pos, c1, vec3_origin, c2, vec3_origin, 6.0f, 6.0f, 0.0f, 0.0f, 0.2f, 0.5f, WHITE, WHITE, 0.0f, 4000, cgi_R_RegisterShader( "gfx/effects/smokeTrail" ), FX_ALPHA_WAVE );

	theFxScheduler.PlayEffect( "necro_chaos/alt_miss", origin, normal );
}

void FX_NecroAltMiss( vec3_t origin, vec3_t normal )
{
	vec3_t pos, c1, c2;

	VectorMA( origin, 4.0f, normal, c1 );
	VectorCopy( c1, c2 );
	c1[2] += 4;
	c2[2] += 12;
	
	VectorAdd( origin, normal, pos );
	pos[2] += 28;

	FX_AddBezier( origin, pos, c1, vec3_origin, c2, vec3_origin, 6.0f, 6.0f, 0.0f, 0.0f, 0.2f, 0.5f, WHITE, WHITE, 0.0f, 4000, cgi_R_RegisterShader( "gfx/effects/smokeTrail" ), FX_ALPHA_WAVE );

	theFxScheduler.PlayEffect( "necro/alt_miss", origin, normal );
}

void FX_NecrovoidAltMiss( vec3_t origin, vec3_t normal )
{
	vec3_t pos, c1, c2;

	VectorMA( origin, 4.0f, normal, c1 );
	VectorCopy( c1, c2 );
	c1[2] += 4;
	c2[2] += 12;
	
	VectorAdd( origin, normal, pos );
	pos[2] += 28;

	FX_AddBezier( origin, pos, c1, vec3_origin, c2, vec3_origin, 6.0f, 6.0f, 0.0f, 0.0f, 0.2f, 0.5f, WHITE, WHITE, 0.0f, 4000, cgi_R_RegisterShader( "gfx/effects/smokeTrail" ), FX_ALPHA_WAVE );

	theFxScheduler.PlayEffect( "necro_void/alt_miss", origin, normal );
}

void FX_NecrosmogAltMiss( vec3_t origin, vec3_t normal )
{
	vec3_t pos, c1, c2;

	VectorMA( origin, 4.0f, normal, c1 );
	VectorCopy( c1, c2 );
	c1[2] += 4;
	c2[2] += 12;
	
	VectorAdd( origin, normal, pos );
	pos[2] += 28;

	FX_AddBezier( origin, pos, c1, vec3_origin, c2, vec3_origin, 6.0f, 6.0f, 0.0f, 0.0f, 0.2f, 0.5f, WHITE, WHITE, 0.0f, 4000, cgi_R_RegisterShader( "gfx/effects/smokeTrail" ), FX_ALPHA_WAVE );

	theFxScheduler.PlayEffect( "necro_smog/alt_miss", origin, normal );
}

void FX_NecromudAltMiss( vec3_t origin, vec3_t normal )
{
	vec3_t pos, c1, c2;

	VectorMA( origin, 4.0f, normal, c1 );
	VectorCopy( c1, c2 );
	c1[2] += 4;
	c2[2] += 12;
	
	VectorAdd( origin, normal, pos );
	pos[2] += 28;

	FX_AddBezier( origin, pos, c1, vec3_origin, c2, vec3_origin, 6.0f, 6.0f, 0.0f, 0.0f, 0.2f, 0.5f, WHITE, WHITE, 0.0f, 4000, cgi_R_RegisterShader( "gfx/effects/smokeTrail" ), FX_ALPHA_WAVE );

	theFxScheduler.PlayEffect( "necro_mud/alt_miss", origin, normal );
}

void FX_NecroCrystalAltMiss( vec3_t origin, vec3_t normal )
{
	vec3_t pos, c1, c2;

	VectorMA( origin, 4.0f, normal, c1 );
	VectorCopy( c1, c2 );
	c1[2] += 4;
	c2[2] += 12;
	
	VectorAdd( origin, normal, pos );
	pos[2] += 28;

	FX_AddBezier( origin, pos, c1, vec3_origin, c2, vec3_origin, 6.0f, 6.0f, 0.0f, 0.0f, 0.2f, 0.5f, WHITE, WHITE, 0.0f, 4000, cgi_R_RegisterShader( "gfx/effects/smokeTrail" ), FX_ALPHA_WAVE );

	theFxScheduler.PlayEffect( "necro_crystal/alt_miss", origin, normal );
}

void FX_AMAltMiss( vec3_t origin, vec3_t normal )
{
	vec3_t pos, c1, c2;

	VectorMA( origin, 4.0f, normal, c1 );
	VectorCopy( c1, c2 );
	c1[2] += 4;
	c2[2] += 12;
	
	VectorAdd( origin, normal, pos );
	pos[2] += 28;

	FX_AddBezier( origin, pos, c1, vec3_origin, c2, vec3_origin, 6.0f, 6.0f, 0.0f, 0.0f, 0.2f, 0.5f, WHITE, WHITE, 0.0f, 4000, cgi_R_RegisterShader( "gfx/effects/smokeTrail" ), FX_ALPHA_WAVE );

	theFxScheduler.PlayEffect( "concussion0/alt_miss", origin, normal );
}