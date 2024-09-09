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

//---------------------
//	Thermal Detonator
//---------------------

//---------------------------------------------------------
void thermalDetonatorExplode( gentity_t *ent )
//---------------------------------------------------------
{
	if ( (ent->s.eFlags&EF_HELD_BY_SAND_CREATURE) )
	{
		ent->takedamage = qfalse; // don't allow double deaths!

		G_Damage( ent->activator, ent, ent->owner, vec3_origin, ent->currentOrigin, weaponData[WP_THERMAL].altDamage, 0, MOD_EXPLOSIVE );
		G_PlayEffect( "thermal/explosion", ent->currentOrigin );
		G_PlayEffect( "thermal/shockwave", ent->currentOrigin );

		G_FreeEntity( ent );
	}
	else if ( !ent->count )
	{
		G_Sound( ent, G_SoundIndex( "sound/weapons/thermal/warning.wav" ) );
		ent->count = 1;
		ent->nextthink = level.time + 800;
		ent->svFlags |= SVF_BROADCAST;//so everyone hears/sees the explosion?
	}
	else
	{
		vec3_t	pos;

		VectorSet( pos, ent->currentOrigin[0], ent->currentOrigin[1], ent->currentOrigin[2] + 8 );

		ent->takedamage = qfalse; // don't allow double deaths!

		G_RadiusDamage( ent->currentOrigin, ent->owner, weaponData[WP_THERMAL].splashDamage, weaponData[WP_THERMAL].splashRadius, NULL, MOD_EXPLOSIVE_SPLASH );

		
		if (  ent->owner->client )
			{
				switch ( ent->owner->client->NPC_class )
				{
				// FORCE
				case CLASS_ARCHER: 
				case CLASS_REAVER: 
				case CLASS_MENTALIST: 
				case CLASS_JEDI:
				case CLASS_IMPWORKER: 
				case CLASS_WIZARD: 
				case CLASS_PROBE: 
				case CLASS_AIR: 
				case CLASS_GOLEM_AIR: 
				case CLASS_AIRREAVER: 
				G_PlayEffect( "thermal8/explosion",  ent->currentOrigin );
				G_PlayEffect( "thermal8/shockwave", ent->currentOrigin );
				G_RadiusDamage( ent->currentOrigin, ent->owner, weaponData[WP_THERMAL].splashDamage, weaponData[WP_THERMAL].splashRadius, NULL, MOD_ROCKET );
				break;
				// STONE
				case CLASS_DRUID: 
				case CLASS_VAMPIRE_ELF:
				case CLASS_VAMPIRE_ELF2: 
				case CLASS_VAMPIRE_ELF3: 
				case CLASS_EARTHREAVER: 
				case CLASS_EARTH: 
				case CLASS_GOLEM: 
				case CLASS_DEMON_BLADE: 				
				case CLASS_MONMOTHA:
				case CLASS_GONK:
				case CLASS_UGNAUGHT: 
				G_PlayEffect( "thermal7/explosion",  ent->currentOrigin );
				G_PlayEffect( "thermal7/shockwave", ent->currentOrigin );
				G_RadiusDamage( ent->currentOrigin, ent->owner, weaponData[WP_THERMAL].splashDamage, weaponData[WP_THERMAL].splashRadius, NULL, MOD_FLECHETTE );
				if ( ent && ent->client )
				{
					ent->client->ps.pm_type = PM_DEAD;
					ent->s.pos.trTime += 6000;
					ent->painDebounceTime += 6000;
					ent->client->ps.weaponTime = ent->client->ps.torsoAnimTimer;
				}
				break;
				// SOUND 
				case CLASS_BIRD: 
				case CLASS_LYCAN: 
				case CLASS_SONIC: 
				case CLASS_GOLEM_SONIC: 
				case CLASS_TRANDOSHAN: 
				case CLASS_HOWLER: 
				case CLASS_REBEL: 
				case CLASS_VAMPIRE_SOUND: 
				G_PlayEffect( "thermal6/explosion",  ent->currentOrigin );
				G_PlayEffect( "thermal6/shockwave", ent->currentOrigin );
				G_RadiusDamage( ent->currentOrigin, ent->owner, weaponData[WP_THERMAL].splashDamage, weaponData[WP_THERMAL].splashRadius, NULL, MOD_SONIC );
				if ( ent->activator && ent->activator->client )
				{
					NPC_SetAnim( ent->activator, SETANIM_LEGS, BOTH_SONICPAIN_HOLD, SETANIM_FLAG_NORMAL );
					NPC_SetAnim( ent->activator, SETANIM_TORSO, BOTH_SONICPAIN_HOLD, SETANIM_FLAG_OVERRIDE|SETANIM_FLAG_HOLD );
					ent->activator->client->ps.torsoAnimTimer += 5000;  
					ent->activator->client->ps.weaponTime = ent->owner->client->ps.torsoAnimTimer;
				}
				break;
				// WATER 
				case CLASS_WARRIOR: 
				case CLASS_VAMPIRE_WATER: 
				case CLASS_WATER: 
				case CLASS_GOLEM_WATER: 
				case CLASS_RAHABIM: 
				case CLASS_SHARK: 
				case CLASS_ALORA: 
				case CLASS_SWAMPTROOPER:
				case CLASS_FISH:
				case CLASS_SWAMP:
				G_PlayEffect( "thermal5/explosion",  ent->currentOrigin );
				G_PlayEffect( "thermal5/shockwave", ent->currentOrigin );
				G_RadiusDamage( ent->currentOrigin, ent->owner, weaponData[WP_THERMAL].splashDamage, weaponData[WP_THERMAL].splashRadius, NULL, MOD_REPEATER );
				if ( ent && ent->client->NPC_class == CLASS_REBORN || ent && ent->client->NPC_class == CLASS_KYLE || 
						ent && ent->client->NPC_class == CLASS_ALORA || ent && ent->client->NPC_class == CLASS_MONMOTHA || 
						ent && ent->client->NPC_class == CLASS_TUSKEN || ent && ent->client->NPC_class == CLASS_GLIDER || 
						ent && ent->client->NPC_class == CLASS_NOGHRI || ent && ent->client->NPC_class == CLASS_FLIER2 || 
						ent && ent->client->NPC_class == CLASS_SWAMPTROOPER || ent && ent->client->NPC_class == CLASS_FISH || 
						ent && ent->client->NPC_class == CLASS_WEEQUAY || ent && ent->client->NPC_class == CLASS_LIZARD || 
						ent && ent->client->NPC_class == CLASS_GRAN || ent && ent->client->NPC_class == CLASS_CLAW || 
						ent && ent->client->NPC_class == CLASS_JEDI || ent && ent->client->NPC_class == CLASS_LUKE || 
						ent && ent->client->NPC_class == CLASS_WAMPA || ent && ent->client->NPC_class == CLASS_MINEMONSTER || 
						ent && ent->client->NPC_class == CLASS_TAVION ||ent && ent->client->NPC_class == CLASS_COMMANDO ||
						ent && ent->client->NPC_class == CLASS_MURJJ	 || ent && ent->client->NPC_class == CLASS_TRANDOSHAN ||
						ent && ent->client->NPC_class == CLASS_LANDO ) 
				{
					NPC_SetAnim( ent, SETANIM_LEGS, BOTH_COWER1, SETANIM_FLAG_NORMAL );
					NPC_SetAnim( ent, SETANIM_TORSO, BOTH_COWER1, SETANIM_FLAG_OVERRIDE|SETANIM_FLAG_HOLD );
					ent->client->ps.torsoAnimTimer += 5000;  
					ent->client->ps.weaponTime = ent->client->ps.torsoAnimTimer;
				}
				break;
				// SUNLIGHT 
				case CLASS_ALCHEMIST:				 
				case CLASS_VAMPIRE_ARCHER: 
				case CLASS_LIGHT: 
				case CLASS_GOLEM_LIGHT: 
				case CLASS_GOLEM_GOLD: 
				case CLASS_GALAK:
				case CLASS_PROTOCOL:
				case CLASS_MORGANKATARN: 
				G_PlayEffect( "thermal3/explosion",  ent->currentOrigin );
				G_PlayEffect( "thermal3/shockwave", ent->currentOrigin );
				G_RadiusDamage( ent->currentOrigin, ent->owner, weaponData[WP_THERMAL].splashDamage, weaponData[WP_THERMAL].splashRadius, NULL, MOD_BOWCASTER );
				if ( ent && ent->client )
				{
					NPC_SetAnim( ent, SETANIM_LEGS, BOTH_COWER1, SETANIM_FLAG_NORMAL );
					NPC_SetAnim( ent, SETANIM_TORSO, BOTH_COWER1, SETANIM_FLAG_OVERRIDE|SETANIM_FLAG_HOLD );
					ent->client->ps.torsoAnimTimer += 5000;  
					ent->client->ps.weaponTime = ent->client->ps.torsoAnimTimer;
					if (  ent &&  ent->client && ent->health <= 0 )
						{
							ent->s.powerups |= ( 1 << PW_DISRUPTION );
							ent->client->ps.powerups[PW_DISRUPTION] = cg.time + 1000;
							if ( ent->ghoul2.size() && ent->headBolt != -1 )
							{//FIXME: what if already playing effect?
								G_PlayEffect( G_EffectIndex( "thermal3/disintegration" ), ent->playerModel, ent->chestBolt, ent->s.number, ent->currentOrigin ); // Huge flesh effect with body parts flyings everywhere 
							}
						}
				}
				break;
				// ELETTRICA 
				case CLASS_VAMPIRE_LYCAN: 
				case CLASS_LIGHTREAVER: 
				case CLASS_GOLEM_METAL: 
				case CLASS_HALFVAMPIRE: 
				case CLASS_DEMON_ELECTRO: 
				case CLASS_GRAN: 
				case CLASS_SABER_DROID:
				case CLASS_ASSASSIN_DROID: 
				case CLASS_ROCKETTROOPER:
				case CLASS_BOBAFETT: 
				G_PlayEffect( "thermal9/explosion",  ent->currentOrigin );
				G_PlayEffect( "thermal9/shockwave", ent->currentOrigin );
				G_RadiusDamage( ent->currentOrigin, ent->owner, weaponData[WP_THERMAL].splashDamage, weaponData[WP_THERMAL].splashRadius, NULL, MOD_ELECTROCUTE );
				if ( ent && ent->client ) 
				{
					ent->s.powerups |= ( 1 << PW_SHOCKED );
					ent->client->ps.powerups[PW_SHOCKED] = cg.time + 5000;
				}
				break;
				// HOLY 
				case CLASS_CENOBITA: 
				case CLASS_STORMTROOPER:
				case CLASS_RAZIEL: 
				case CLASS_HOLY: 
				case CLASS_GOLEM_HOLY: 
				case CLASS_LUKE: 
				G_PlayEffect( "thermal10/explosion",  ent->currentOrigin );
				G_PlayEffect( "thermal10/shockwave", ent->currentOrigin );
				G_RadiusDamage( ent->currentOrigin, ent->owner, weaponData[WP_THERMAL].splashDamage, weaponData[WP_THERMAL].splashRadius, NULL, MOD_DEMP2 );
				/*if ( other->client->NPC_class == CLASS_REBORN ||  other->client->NPC_class == CLASS_KYLE || 
						 other->client->NPC_class == CLASS_ALORA ||  other->client->NPC_class == CLASS_MONMOTHA || 
						 other->client->NPC_class == CLASS_TUSKEN || other->client->NPC_class == CLASS_GLIDER || 
						 other->client->NPC_class == CLASS_NOGHRI || other->client->NPC_class == CLASS_FLIER2 || 
						 other->client->NPC_class == CLASS_SWAMPTROOPER || other->client->NPC_class == CLASS_FISH || 
						 other->client->NPC_class == CLASS_WEEQUAY || other->client->NPC_class == CLASS_LIZARD || 
						 other->client->NPC_class == CLASS_GRAN || other->client->NPC_class == CLASS_CLAW || 
						 other->client->NPC_class == CLASS_JEDI || other->client->NPC_class == CLASS_LUKE || 
						 other->client->NPC_class == CLASS_WAMPA || other->client->NPC_class == CLASS_MINEMONSTER || 
						 other->client->NPC_class == CLASS_TAVION || other->client->NPC_class == CLASS_COMMANDO ||
						 other->client->NPC_class == CLASS_MURJJ	 ||  other->client->NPC_class == CLASS_TRANDOSHAN ||
						 other->client->NPC_class == CLASS_LANDO ||
						 other->client->NPC_class == CLASS_VAMPIRE ||  other->client->NPC_class == CLASS_KAIN || 
						 other->client->NPC_class == CLASS_RAHABIM ||  other->client->NPC_class == CLASS_SUMMONER || 
						 other->client->NPC_class == CLASS_MELC || other->client->NPC_class == CLASS_DECEIVER || 
						 other->client->NPC_class == CLASS_ZEPH || other->client->NPC_class == CLASS_DUMAHIM || 
						 other->client->NPC_class == CLASS_TURELIM || other->client->NPC_class == CLASS_MENTALIST || 
						 other->client->NPC_class == CLASS_RAZIEL || other->client->NPC_class == CLASS_REAVER || 
						 other->client->NPC_class == CLASS_VAMPIRE_DEMON || other->client->NPC_class == CLASS_THRALL || 
						 other->client->NPC_class == CLASS_POLTER || other->client->NPC_class == CLASS_NECRO || 
						 other->client->NPC_class == CLASS_DEMON_POISON || other->client->NPC_class == CLASS_DEMON_FIRE || 
						 other->client->NPC_class == CLASS_DEMON_ELECTRO || other->client->NPC_class == CLASS_LYCAN ||
						 other->client->NPC_class == CLASS_NECROREAVER	 ||  other->client->NPC_class == CLASS_TANKREAVER ||
						 other->client->NPC_class == CLASS_DARKREAVER ||  other->client->NPC_class == CLASS_LIGHTREAVER || 
						 other->client->NPC_class == CLASS_DRUIDREAVER ||  other->client->NPC_class == CLASS_AIRREAVER ||
						 other->client->NPC_class == CLASS_FIREREAVER || other->client->NPC_class == CLASS_ICEREAVER ||
						 other->client->NPC_class == CLASS_EARTHREAVER ) 
				{
					G_ClearEnemy( ent );
					other->client->ps.torsoAnimTimer += 7000;  
					other->client->ps.weaponTime = other->client->ps.torsoAnimTimer;
					NPC_SetAnim( ent, SETANIM_LEGS, BOTH_SONICPAIN_HOLD, SETANIM_FLAG_NORMAL );
					NPC_SetAnim( ent, SETANIM_TORSO, BOTH_SONICPAIN_HOLD, SETANIM_FLAG_OVERRIDE|SETANIM_FLAG_HOLD );
					other->client->poisonDamage = 10;  
					other->client->poisonTime = cg.time + 7000; 
				}





				// other? activator? cent->gent? 
				if ( ent->client->NPC_class == CLASS_REBORN ||  ent->client->NPC_class == CLASS_KYLE || 
						 ent->client->NPC_class == CLASS_ALORA ||  ent->client->NPC_class == CLASS_MONMOTHA || 
						 ent->client->NPC_class == CLASS_TUSKEN || ent->client->NPC_class == CLASS_GLIDER || 
						 ent->client->NPC_class == CLASS_NOGHRI || ent->client->NPC_class == CLASS_FLIER2 || 
						 ent->client->NPC_class == CLASS_SWAMPTROOPER || ent->client->NPC_class == CLASS_FISH || 
						 ent->client->NPC_class == CLASS_WEEQUAY || ent->client->NPC_class == CLASS_LIZARD || 
						 ent->client->NPC_class == CLASS_GRAN || ent->client->NPC_class == CLASS_CLAW || 
						 ent->client->NPC_class == CLASS_JEDI || ent->client->NPC_class == CLASS_LUKE || 
						 ent->client->NPC_class == CLASS_WAMPA || ent->client->NPC_class == CLASS_MINEMONSTER || 
						 ent->client->NPC_class == CLASS_TAVION || ent->client->NPC_class == CLASS_COMMANDO ||
						 ent->client->NPC_class == CLASS_MURJJ	 ||  ent->client->NPC_class == CLASS_TRANDOSHAN ||
						 ent->client->NPC_class == CLASS_LANDO ||
						 ent->client->NPC_class == CLASS_VAMPIRE ||  ent->client->NPC_class == CLASS_KAIN || 
						 ent->client->NPC_class == CLASS_RAHABIM ||  ent->client->NPC_class == CLASS_SUMMONER || 
						 ent->client->NPC_class == CLASS_MELC || ent->client->NPC_class == CLASS_DECEIVER || 
						 ent->client->NPC_class == CLASS_ZEPH || ent->client->NPC_class == CLASS_DUMAHIM || 
						 ent->client->NPC_class == CLASS_TURELIM || ent->client->NPC_class == CLASS_MENTALIST || 
						 ent->client->NPC_class == CLASS_RAZIEL || ent->client->NPC_class == CLASS_REAVER || 
						 ent->client->NPC_class == CLASS_VAMPIRE_DEMON || ent->client->NPC_class == CLASS_THRALL || 
						 ent->client->NPC_class == CLASS_POLTER || ent->client->NPC_class == CLASS_NECRO || 
						 ent->client->NPC_class == CLASS_DEMON_POISON || ent->client->NPC_class == CLASS_DEMON_FIRE || 
						 ent->client->NPC_class == CLASS_DEMON_ELECTRO || ent->client->NPC_class == CLASS_LYCAN ||
						 ent->client->NPC_class == CLASS_NECROREAVER	 ||  ent->client->NPC_class == CLASS_TANKREAVER ||
						 ent->client->NPC_class == CLASS_DARKREAVER ||  ent->client->NPC_class == CLASS_LIGHTREAVER || 
						 ent->client->NPC_class == CLASS_DRUIDREAVER ||  ent->client->NPC_class == CLASS_AIRREAVER ||
						 ent->client->NPC_class == CLASS_FIREREAVER || ent->client->NPC_class == CLASS_ICEREAVER ||
						 ent->client->NPC_class == CLASS_EARTHREAVER ) 
				{
					G_ClearEnemy( ent );
					ent->client->ps.torsoAnimTimer += 7000;  
					ent->client->ps.weaponTime = ent->client->ps.torsoAnimTimer;
					NPC_SetAnim( ent, SETANIM_LEGS, BOTH_SONICPAIN_HOLD, SETANIM_FLAG_NORMAL );
					NPC_SetAnim( ent, SETANIM_TORSO, BOTH_SONICPAIN_HOLD, SETANIM_FLAG_OVERRIDE|SETANIM_FLAG_HOLD );
					ent->client->poisonDamage = 10;  
					ent->client->poisonTime = cg.time + 7000; 
				}*/
				break;
				// DARKNESS 
				case CLASS_VAMPIRE: 
				case CLASS_VAMPIRE_BLOOD: 
				case CLASS_UMAH: 
				case CLASS_VORADOR:			 
				case CLASS_VAMPIRE_BOUNTY:
				case CLASS_DARK: 
				case CLASS_VOID: 
				case CLASS_GOLEM_SMOKE: 
				case CLASS_GOLEM_DARK: 
				case CLASS_GOLEM_VOID: 
				case CLASS_DARKREAVER: 
				case CLASS_TANKREAVER: 
				case CLASS_VAMPIRE_DARK: 
				case CLASS_VAMPIRE_SARAFAN: 
				case CLASS_NOGHRI:
				case CLASS_FLIER2: 
				case CLASS_SHADOWTROOPER:
				case CLASS_BESPIN_COP: 
				case CLASS_SABOTEUR: 
				G_PlayEffect( "thermal2/explosion",  ent->currentOrigin );
				G_PlayEffect( "thermal2/shockwave", ent->currentOrigin );
				G_RadiusDamage( ent->currentOrigin, ent->owner, weaponData[WP_THERMAL].splashDamage, weaponData[WP_THERMAL].splashRadius, NULL, MOD_BRYAR );
					if ( ent && ent->client ) 
				{
					G_ClearEnemy( ent );
					ent->client->ps.torsoAnimTimer += 5000;  
					ent->client->ps.weaponTime = ent->client->ps.torsoAnimTimer;
					NPC_SetAnim( ent, SETANIM_LEGS, BOTH_COWER1, SETANIM_FLAG_NORMAL );
					NPC_SetAnim( ent, SETANIM_TORSO, BOTH_COWER1, SETANIM_FLAG_OVERRIDE|SETANIM_FLAG_HOLD );
					
				}
				break;
				// ACIDIC 
				case CLASS_KYLE:
				case CLASS_KAIN: 
				case CLASS_KAIN2: 
				case CLASS_KAIN3: 
				case CLASS_DUMAHIM: 
				case CLASS_WEEQUAY:
				case CLASS_LIZARD: 
				case CLASS_INTERROGATOR: 
				case CLASS_R2D2:
				case CLASS_R5D2: 	
				case CLASS_REELO: 
				case CLASS_ZEPH:
				case CLASS_DECEIVER:
				case CLASS_SPIDERREAVER:
				case CLASS_SPIDERREAVER2:
				case CLASS_SPIDERREAVER3:	
				case CLASS_DEMON_POISON: 
				case CLASS_BEAST:  			  
				case CLASS_GOLEM_POISON:
				case CLASS_POISON: 
				G_PlayEffect( "thermal/explosion",  ent->currentOrigin );
				G_PlayEffect( "thermal/shockwave", ent->currentOrigin );
				G_RadiusDamage( ent->currentOrigin, ent->owner, weaponData[WP_THERMAL].splashDamage, weaponData[WP_THERMAL].splashRadius, NULL, MOD_POISON );
				if ( ent && ent->client ) 
				{
					ent->client->poisonDamage = 10;  
					ent->client->poisonTime = cg.time + 10000; 
					if (  ent &&  ent->client && ent->health <= 0 )
						{
							ent->s.powerups |= ( 1 << PW_DISRUPTION );
							ent->client->ps.powerups[PW_DISRUPTION] = cg.time + 1000;
							if ( ent->ghoul2.size() && ent->headBolt != -1 )
							{//FIXME: what if already playing effect?
								G_PlayEffect( G_EffectIndex( "thermal/disintegration" ), ent->playerModel, ent->chestBolt, ent->s.number, ent->currentOrigin ); // Huge flesh effect with body parts flyings everywhere 
							}
						}
				}
				break;
				// UNDEAD 
				case CLASS_TENTACLE: 
				case CLASS_ELDERGOD: 
				case CLASS_SLUAGH: 
				case CLASS_REAPER: 
				case CLASS_DREADNOUGHT: 
				case CLASS_NECROREAVER: 
				case CLASS_NECRO: 
				case CLASS_GOLEM_NECRO: 
				case CLASS_RANCOR:
				case CLASS_MURJJ: 
				case CLASS_ATST: 
				case CLASS_MOUSE:
				case CLASS_SAND_CREATURE: 
				case CLASS_MELC:  
				case CLASS_SUMMONER:
				case CLASS_VAMPIRE_ZOMBIE: 
				G_PlayEffect( "thermal11/explosion",  ent->currentOrigin );
				G_PlayEffect( "thermal11/shockwave", ent->currentOrigin );
				G_RadiusDamage( ent->currentOrigin, ent->owner, weaponData[WP_THERMAL].splashDamage, weaponData[WP_THERMAL].splashRadius, NULL, MOD_CONC );
				if ( ent && ent->client ) 
				{
					ent->client->poisonDamage = 8;  
					ent->client->poisonTime = cg.time + 15000; 
					ent->client->ps.torsoAnimTimer += 15000;  
					ent->client->ps.weaponTime = ent->client->ps.torsoAnimTimer;
					NPC_SetAnim( ent, SETANIM_LEGS, BOTH_ROSH_PAIN, SETANIM_FLAG_NORMAL );
					NPC_SetAnim( ent, SETANIM_TORSO, BOTH_ROSH_PAIN, SETANIM_FLAG_OVERRIDE|SETANIM_FLAG_HOLD );
				}
				break;
				// HYLDEN 
				case CLASS_PROPHET: 
				case CLASS_HYLDEN: 
				case CLASS_HYLDENLORD: 
				case CLASS_TAVION:
				case CLASS_DESANN:
				case CLASS_REMOTE:
				case CLASS_SEEKER:
				case CLASS_SENTRY:
				G_PlayEffect( "thermal12/explosion",  ent->currentOrigin );
				G_PlayEffect( "thermal12/shockwave", ent->currentOrigin );
				G_RadiusDamage( ent->currentOrigin, ent->owner, weaponData[WP_THERMAL].splashDamage, weaponData[WP_THERMAL].splashRadius, NULL, MOD_EMPLACED );
				if ( ent && ent->client ) 
				{
					ent->client->poisonDamage = 11;  
					ent->client->poisonTime = cg.time + 11000; 
					ent->client->ps.torsoAnimTimer += 11000;  
					ent->client->ps.weaponTime = ent->client->ps.torsoAnimTimer;
					NPC_SetAnim( ent, SETANIM_LEGS, BOTH_SONICPAIN_HOLD, SETANIM_FLAG_NORMAL );
					NPC_SetAnim( ent, SETANIM_TORSO, BOTH_SONICPAIN_HOLD, SETANIM_FLAG_OVERRIDE|SETANIM_FLAG_HOLD );
					if (  ent &&  ent->client && ent->health <= 0 )
						{
							ent->s.powerups |= ( 1 << PW_DISRUPTION );
							ent->client->ps.powerups[PW_DISRUPTION] = cg.time + 1000;
							if ( ent->ghoul2.size() && ent->headBolt != -1 )
							{//FIXME: what if already playing effect?
								G_PlayEffect( G_EffectIndex( "thermal12/disintegration" ), ent->playerModel, ent->chestBolt, ent->s.number, ent->currentOrigin ); // Huge flesh effect with body parts flyings everywhere 
							}
						}
				}
				break;
				// BOMBA DI FUOCO 
				case CLASS_RAZIELHIM: 
				case CLASS_ESKANDOR: 
				case CLASS_HUNTER:			 
				case CLASS_DEMON_FIRE:
				case CLASS_VAMPIRE_FEAR: 
				case CLASS_VAMPIRE_FEAR2: 
				case CLASS_FIREREAVER: 
				case CLASS_TURELIM: 
				case CLASS_FIRE: 
				case CLASS_VAMPIRE_DEMON: 
				case CLASS_GOLEM_FIRE: 
				case CLASS_HELLGOLEM: 
				case CLASS_GOLEM_LAVA: 
				case CLASS_REBORN: 
				case CLASS_WAMPA: 
				case CLASS_TUSKEN: 
				case CLASS_GLIDER: 
				case CLASS_RODIAN: 
				case CLASS_IMPERIAL: 
				case CLASS_HAZARD_TROOPER: 
				case CLASS_CLAW: 
				case CLASS_PLAYER: 
				case CLASS_VEHICLE: 
				case CLASS_FIGHTER: 
				G_PlayEffect( "thermal4/explosion",  ent->currentOrigin );
				G_PlayEffect( "thermal4/shockwave", ent->currentOrigin );
				G_RadiusDamage( ent->currentOrigin, ent->owner, weaponData[WP_THERMAL].splashDamage, weaponData[WP_THERMAL].splashRadius, NULL, MOD_EXPLOSIVE_SPLASH );
				if ( ent && ent->client ) 
				{
					ent->client->poisonDamage = 5;  
					ent->client->poisonTime = cg.time + 3000; 
				}
				break; 
				// MACE BOMB
				case CLASS_SCOUT: 
				case CLASS_MARK1: 
				case CLASS_MARK2:				 
				G_PlayEffect( "thermal13/explosion",  ent->currentOrigin );
				G_PlayEffect( "thermal13/shockwave", ent->currentOrigin );
				G_RadiusDamage( ent->currentOrigin, ent->owner, weaponData[WP_THERMAL].splashDamage, weaponData[WP_THERMAL].splashRadius, NULL, MOD_MELEE );
				break; 
				// PIANTE SPUNTONE
				case CLASS_IVY: 
				case CLASS_DRUIDREAVER: 
				case CLASS_LANDO: 
				case CLASS_PRISONER: 
				G_PlayEffect( "thermal14/explosion",  ent->currentOrigin );
				G_PlayEffect( "thermal14/shockwave", ent->currentOrigin );
				G_RadiusDamage( ent->currentOrigin, ent->owner, weaponData[WP_THERMAL].splashDamage, weaponData[WP_THERMAL].splashRadius, NULL, MOD_POISON );
				if ( ent && ent->client ) 
				{
					ent->client->poisonDamage = 4;  
					ent->client->poisonTime = cg.time + 10000; 
				}
				break;
				// CRISTALLI DI GHIACCIO 
				case CLASS_ICEREAVER: 
				case CLASS_VAMPIRE_FROST: 
				case CLASS_GOLEM_ICE: 
				case CLASS_CRYSTAL: 
				case CLASS_BARTENDER:
				case CLASS_GALAKMECH: 
				G_PlayEffect( "thermal15/explosion",  ent->currentOrigin );
				G_PlayEffect( "thermal15/shockwave", ent->currentOrigin );
				G_RadiusDamage( ent->currentOrigin, ent->owner, weaponData[WP_THERMAL].splashDamage, weaponData[WP_THERMAL].splashRadius, NULL, MOD_REPEATER_ALT );
				if ( ent && ent->client )
				{
					ent->client->ps.pm_type = PM_DEAD;
					ent->s.pos.trTime += 7000;
					ent->painDebounceTime += 7000;
					ent->client->ps.weaponTime = ent->client->ps.torsoAnimTimer;
				}
				break; 
				// CROCI 
				case CLASS_POLTER: 
				case CLASS_JAWA:
				case CLASS_GHOST: 
				G_PlayEffect( "thermal16/explosion",  ent->currentOrigin );
				G_PlayEffect( "thermal16/shockwave", ent->currentOrigin );
				G_RadiusDamage( ent->currentOrigin, ent->owner, weaponData[WP_THERMAL].splashDamage, weaponData[WP_THERMAL].splashRadius, NULL, MOD_UNKNOWN );
				if ( ent && ent->client ) 
				{
					ent->client->poisonDamage = 16;  
					ent->client->poisonTime = cg.time + 10000; 
					ent->client->ps.pm_type = PM_DEAD;
					ent->s.pos.trTime += 10000;
					ent->painDebounceTime += 10000;
					ent->client->ps.torsoAnimTimer += 10000;  
					ent->client->ps.weaponTime = ent->client->ps.torsoAnimTimer;
					NPC_SetAnim( ent, SETANIM_LEGS, BOTH_PULLED_INAIR_B, SETANIM_FLAG_NORMAL );
					NPC_SetAnim( ent, SETANIM_TORSO, BOTH_PULLED_INAIR_B, SETANIM_FLAG_OVERRIDE|SETANIM_FLAG_HOLD );
				}
				break; 
				// BARA 
				case CLASS_COMMANDO: 
				case CLASS_THRALL: 
				case CLASS_NECROMANCER: 
				case CLASS_VAMPIRE_QUEEN:
				case CLASS_VAMPIRE_NECRO: 
				case CLASS_VAMPIRE_NECRO2:
				G_PlayEffect( "thermal17/explosion",  ent->currentOrigin );
				G_PlayEffect( "thermal17/shockwave", ent->currentOrigin );
				G_RadiusDamage( ent->currentOrigin, ent->owner, weaponData[WP_THERMAL].splashDamage, weaponData[WP_THERMAL].splashRadius, NULL, MOD_CONC_ALT );
				if ( ent && ent->client ) 
				{
					ent->client->poisonDamage = 8;  
					ent->client->poisonTime = cg.time + 8000; 
					ent->client->ps.pm_type = PM_DEAD;
					ent->s.pos.trTime += 8000;
					ent->painDebounceTime += 8000;
					ent->client->ps.torsoAnimTimer += 8000;  
					ent->client->ps.weaponTime = ent->client->ps.torsoAnimTimer;
					NPC_SetAnim( ent, SETANIM_LEGS, BOTH_PULLED_INAIR_F, SETANIM_FLAG_NORMAL );
					NPC_SetAnim( ent, SETANIM_TORSO, BOTH_PULLED_INAIR_F, SETANIM_FLAG_OVERRIDE|SETANIM_FLAG_HOLD );
				}
				break; 
				// ANGELO 
				case CLASS_JANOS: 
				case CLASS_JAN: 
				case CLASS_ARIEL: 
				case CLASS_SARAFAN: 
				G_PlayEffect( "thermal18/explosion",  ent->currentOrigin );
				G_PlayEffect( "thermal18/shockwave", ent->currentOrigin );
				G_RadiusDamage( ent->currentOrigin, ent->owner, weaponData[WP_THERMAL].splashDamage, weaponData[WP_THERMAL].splashRadius, NULL, MOD_DEMP2_ALT );
				if ( ent && ent->client->NPC_class == CLASS_REBORN || ent && ent->client->NPC_class == CLASS_KYLE || 
						ent && ent->client->NPC_class == CLASS_ALORA || ent && ent->client->NPC_class == CLASS_MONMOTHA || 
						ent && ent->client->NPC_class == CLASS_TUSKEN || ent && ent->client->NPC_class == CLASS_GLIDER || 
						ent && ent->client->NPC_class == CLASS_NOGHRI || ent && ent->client->NPC_class == CLASS_FLIER2 || 
						ent && ent->client->NPC_class == CLASS_SWAMPTROOPER || ent && ent->client->NPC_class == CLASS_FISH || 
						ent && ent->client->NPC_class == CLASS_WEEQUAY || ent && ent->client->NPC_class == CLASS_LIZARD || 
						ent && ent->client->NPC_class == CLASS_GRAN || ent && ent->client->NPC_class == CLASS_CLAW || 
						ent && ent->client->NPC_class == CLASS_JEDI || ent && ent->client->NPC_class == CLASS_LUKE || 
						ent && ent->client->NPC_class == CLASS_WAMPA || ent && ent->client->NPC_class == CLASS_MINEMONSTER || 
						ent && ent->client->NPC_class == CLASS_TAVION ||ent && ent->client->NPC_class == CLASS_COMMANDO ||
						ent && ent->client->NPC_class == CLASS_MURJJ	 || ent && ent->client->NPC_class == CLASS_TRANDOSHAN ||
						ent && ent->client->NPC_class == CLASS_LANDO ) 
				{
					G_ClearEnemy( ent );
					ent->client->ps.torsoAnimTimer += 10000;  
					ent->client->ps.weaponTime = ent->client->ps.torsoAnimTimer;
					NPC_SetAnim( ent, SETANIM_LEGS, BOTH_SONICPAIN_HOLD, SETANIM_FLAG_NORMAL );
					NPC_SetAnim( ent, SETANIM_TORSO, BOTH_SONICPAIN_HOLD, SETANIM_FLAG_OVERRIDE|SETANIM_FLAG_HOLD );
					ent->client->poisonDamage = 12;  
					ent->client->poisonTime = cg.time + 10000; 
				}
				break;				
			default:
				break;
				}
			}
		
		
		//G_PlayEffect( "thermal/explosion", ent->currentOrigin );
		//G_PlayEffect( "thermal/shockwave", ent->currentOrigin );

		G_FreeEntity( ent );
	}
}

//-------------------------------------------------------------------------------------------------------------
void thermal_die( gentity_t *self, gentity_t *inflictor, gentity_t *attacker, int damage, int mod, int dFlags, int hitLoc )
//-------------------------------------------------------------------------------------------------------------
{
	thermalDetonatorExplode( self );
}

//---------------------------------------------------------
qboolean WP_LobFire( gentity_t *self, vec3_t start, vec3_t target, vec3_t mins, vec3_t maxs, int clipmask, 
				vec3_t velocity, qboolean tracePath, int ignoreEntNum, int enemyNum,
				float minSpeed, float maxSpeed, float idealSpeed, qboolean mustHit )
//---------------------------------------------------------
{
	float	targetDist, shotSpeed, speedInc = 100, travelTime, impactDist, bestImpactDist = Q3_INFINITE;//fireSpeed, 
	vec3_t	targetDir, shotVel, failCase; 
	trace_t	trace;
	trajectory_t	tr;
	qboolean	blocked;
	int		elapsedTime, skipNum, timeStep = 500, hitCount = 0, maxHits = 7;
	vec3_t	lastPos, testPos;
	gentity_t	*traceEnt;
	
	if ( !idealSpeed )
	{
		idealSpeed = 300;
	}
	else if ( idealSpeed < speedInc )
	{
		idealSpeed = speedInc;
	}
	shotSpeed = idealSpeed;
	skipNum = (idealSpeed-speedInc)/speedInc;
	if ( !minSpeed )
	{
		minSpeed = 100;
	}
	if ( !maxSpeed )
	{
		maxSpeed = 900;
	}
	while ( hitCount < maxHits )
	{
		VectorSubtract( target, start, targetDir );
		targetDist = VectorNormalize( targetDir );

		VectorScale( targetDir, shotSpeed, shotVel );
		travelTime = targetDist/shotSpeed;
		shotVel[2] += travelTime * 0.5 * g_gravity->value;

		if ( !hitCount )		
		{//save the first (ideal) one as the failCase (fallback value)
			if ( !mustHit )
			{//default is fine as a return value
				VectorCopy( shotVel, failCase );
			}
		}

		if ( tracePath )
		{//do a rough trace of the path
			blocked = qfalse;

			VectorCopy( start, tr.trBase );
			VectorCopy( shotVel, tr.trDelta );
			tr.trType = TR_GRAVITY;
			tr.trTime = level.time;
			travelTime *= 1000.0f;
			VectorCopy( start, lastPos );
			
			//This may be kind of wasteful, especially on long throws... use larger steps?  Divide the travelTime into a certain hard number of slices?  Trace just to apex and down?
			for ( elapsedTime = timeStep; elapsedTime < floor(travelTime)+timeStep; elapsedTime += timeStep )
			{
				if ( (float)elapsedTime > travelTime )
				{//cap it
					elapsedTime = floor( travelTime );
				}
				EvaluateTrajectory( &tr, level.time + elapsedTime, testPos );
				gi.trace( &trace, lastPos, mins, maxs, testPos, ignoreEntNum, clipmask, (EG2_Collision)0, 0 );

				if ( trace.allsolid || trace.startsolid )
				{
					blocked = qtrue;
					break;
				}
				if ( trace.fraction < 1.0f )
				{//hit something
					if ( trace.entityNum == enemyNum )
					{//hit the enemy, that's perfect!
						break;
					}
					else if ( trace.plane.normal[2] > 0.7 && DistanceSquared( trace.endpos, target ) < 4096 )//hit within 64 of desired location, should be okay
					{//close enough!
						break;
					}
					else
					{//FIXME: maybe find the extents of this brush and go above or below it on next try somehow?
						impactDist = DistanceSquared( trace.endpos, target );
						if ( impactDist < bestImpactDist )
						{
							bestImpactDist = impactDist;
							VectorCopy( shotVel, failCase );
						}
						blocked = qtrue;
						//see if we should store this as the failCase
						if ( trace.entityNum < ENTITYNUM_WORLD )
						{//hit an ent
							traceEnt = &g_entities[trace.entityNum];
							if ( traceEnt && traceEnt->takedamage && !OnSameTeam( self, traceEnt ) )
							{//hit something breakable, so that's okay
								//we haven't found a clear shot yet so use this as the failcase
								VectorCopy( shotVel, failCase );
							}
						}
						break;
					}
				}
				if ( elapsedTime == floor( travelTime ) )
				{//reached end, all clear
					break;
				}
				else
				{
					//all clear, try next slice
					VectorCopy( testPos, lastPos );
				}
			}
			if ( blocked )
			{//hit something, adjust speed (which will change arc)
				hitCount++;
				shotSpeed = idealSpeed + ((hitCount-skipNum) * speedInc);//from min to max (skipping ideal)
				if ( hitCount >= skipNum )
				{//skip ideal since that was the first value we tested
					shotSpeed += speedInc;
				}
			}
			else
			{//made it!
				break;
			}
		}
		else
		{//no need to check the path, go with first calc
			break;
		}
	}

	if ( hitCount >= maxHits )
	{//NOTE: worst case scenario, use the one that impacted closest to the target (or just use the first try...?)
		VectorCopy( failCase, velocity );
		return qfalse;
	}
	VectorCopy( shotVel, velocity );
	return qtrue;
}

//---------------------------------------------------------
void WP_ThermalThink( gentity_t *ent )
//---------------------------------------------------------
{
	int			count;
	qboolean	blow = qfalse;

	// Thermal detonators for the player do occasional radius checks and blow up if there are entities in the blast radius
	//	This is done so that the main fire is actually useful as an attack.  We explode anyway after delay expires.

	if ( (ent->s.eFlags&EF_HELD_BY_SAND_CREATURE) )
	{//blow once creature is underground (done with anim)
		//FIXME: chance of being spit out?  Especially if lots of delay left...
		ent->e_TouchFunc = touchF_NULL;//don't impact on anything
		if ( !ent->activator 
			|| !ent->activator->client
			|| !ent->activator->client->ps.legsAnimTimer )
		{//either something happened to the sand creature or it's done with it's attack anim
			//blow!
			ent->e_ThinkFunc = thinkF_thermalDetonatorExplode;
			ent->nextthink = level.time + Q_irand( 50, 2000 );
		}
		else
		{//keep checking
			ent->nextthink = level.time + TD_THINK_TIME;
		}
		return;
	}
	else if ( ent->delay > level.time )
	{
		//	Finally, we force it to bounce at least once before doing the special checks, otherwise it's just too easy for the player?
		if ( ent->has_bounced )
		{
			count = G_RadiusList( ent->currentOrigin, TD_TEST_RAD, ent, qtrue, ent_list );

			for ( int i = 0; i < count; i++ )
			{
				if ( ent_list[i]->s.number == 0 )
				{
					// avoid deliberately blowing up next to the player, no matter how close any enemy is..
					//	...if the delay time expires though, there is no saving the player...muwhaaa haa ha
					blow = qfalse;
					break;
				}
				else if ( ent_list[i]->client 
					&& ent_list[i]->client->NPC_class != CLASS_SAND_CREATURE//ignore sand creatures
					&& ent_list[i]->health > 0 )
				{
					//FIXME! sometimes the ent_list order changes, so we should make sure that the player isn't anywhere in this list
					blow = qtrue;
				}
			}
		}
	}
	else
	{
		// our death time has arrived, even if nothing is near us
		blow = qtrue;
	}

	if ( blow )
	{
		ent->e_ThinkFunc = thinkF_thermalDetonatorExplode;
		ent->nextthink = level.time + 50;
	}
	else
	{
		// we probably don't need to do this thinking logic very often...maybe this is fast enough?
		ent->nextthink = level.time + TD_THINK_TIME;
	}
}

//---------------------------------------------------------
gentity_t *WP_FireThermalDetonator( gentity_t *ent, qboolean alt_fire )
//---------------------------------------------------------
{
	gentity_t	*bolt;
	//centity_t	*cent; 
	vec3_t		dir, start;
	float		damageScale = 1.0f;

	VectorCopy( forwardVec, dir );
	VectorCopy( muzzle, start );

	bolt = G_Spawn();
	
	bolt->classname = "thermal_detonator";

	if ( ent->s.number != 0 )
	{
		// If not the player, cut the damage a bit so we don't get pounded on so much
		damageScale = TD_NPC_DAMAGE_CUT;
	}

	if ( !alt_fire && ent->s.number == 0 )
	{
		// Main fires for the players do a little bit of extra thinking
		bolt->e_ThinkFunc = thinkF_WP_ThermalThink;
		bolt->nextthink = level.time + TD_THINK_TIME;
		bolt->delay = level.time + TD_TIME; // How long 'til she blows
	}
	else
	{
		bolt->e_ThinkFunc = thinkF_thermalDetonatorExplode;
		bolt->nextthink = level.time + TD_TIME; // How long 'til she blows
	}

	bolt->mass = 10;

	// How 'bout we give this thing a size...
	VectorSet( bolt->mins, -4.0f, -4.0f, -4.0f );
	VectorSet( bolt->maxs, 4.0f, 4.0f, 4.0f );
	bolt->clipmask = MASK_SHOT;
	bolt->clipmask &= ~CONTENTS_CORPSE;
	bolt->contents = CONTENTS_SHOTCLIP;
	bolt->takedamage = qtrue;
	bolt->health = 15;
	bolt->e_DieFunc = dieF_thermal_die;

	WP_TraceSetStart( ent, start, bolt->mins, bolt->maxs );//make sure our start point isn't on the other side of a wall

	float chargeAmount = 1.0f; // default of full charge
	
	if ( ent->client )
	{
		chargeAmount = level.time - ent->client->ps.weaponChargeTime;
	}

	// get charge amount
	chargeAmount = chargeAmount / (float)TD_VELOCITY;

	if ( chargeAmount > 1.0f )
	{
		chargeAmount = 1.0f;
	}
	else if ( chargeAmount < TD_MIN_CHARGE )
	{
		chargeAmount = TD_MIN_CHARGE;
	}

	float	thrownSpeed = TD_VELOCITY;
	const qboolean thisIsAShooter = !Q_stricmp( "misc_weapon_shooter", ent->classname);

	if (thisIsAShooter)
	{
		if (ent->delay != 0)
		{
			thrownSpeed = ent->delay;
		}
	}

	// normal ones bounce, alt ones explode on impact
	bolt->s.pos.trType = TR_GRAVITY;
	bolt->owner = ent;
	VectorScale( dir, thrownSpeed * chargeAmount, bolt->s.pos.trDelta );

	if ( ent->health > 0 )
	{
		bolt->s.pos.trDelta[2] += 120;

		if ( (ent->NPC || (ent->s.number && thisIsAShooter) )
			&& ent->enemy )
		{//NPC or misc_weapon_shooter
			//FIXME: we're assuming he's actually facing this direction...
			vec3_t	target;
			
			VectorCopy( ent->enemy->currentOrigin, target );
			if ( target[2] <= start[2] )
			{
				vec3_t	vec;
				VectorSubtract( target, start, vec );
				VectorNormalize( vec );
				VectorMA( target, Q_flrand( 0, -32 ), vec, target );//throw a little short
			}

			target[0] += Q_flrand( -5, 5 )+(crandom()*(6-ent->NPC->currentAim)*2);
			target[1] += Q_flrand( -5, 5 )+(crandom()*(6-ent->NPC->currentAim)*2);
			target[2] += Q_flrand( -5, 5 )+(crandom()*(6-ent->NPC->currentAim)*2);

			WP_LobFire( ent, start, target, bolt->mins, bolt->maxs, bolt->clipmask, bolt->s.pos.trDelta, qtrue, ent->s.number, ent->enemy->s.number );
		}
		else if ( thisIsAShooter && ent->target && !VectorCompare( ent->pos1, vec3_origin ) )
		{//misc_weapon_shooter firing at a position
			WP_LobFire( ent, start, ent->pos1, bolt->mins, bolt->maxs, bolt->clipmask, bolt->s.pos.trDelta, qtrue, ent->s.number, ent->enemy->s.number );
		}
	}

	if ( alt_fire )
	{
		bolt->alt_fire = qtrue;
	}
	else
	{
		bolt->s.eFlags |= EF_BOUNCE_HALF;
	}

	bolt->s.loopSound = G_SoundIndex( "sound/weapons/thermal/thermloop.wav" );

	bolt->damage = weaponData[WP_THERMAL].damage * damageScale;
	bolt->dflags = 0;
	bolt->splashDamage = weaponData[WP_THERMAL].splashDamage * damageScale;
	bolt->splashRadius = weaponData[WP_THERMAL].splashRadius;

	bolt->s.eType = ET_MISSILE;
	bolt->svFlags = SVF_USE_CURRENT_ORIGIN;
	bolt->s.weapon = WP_THERMAL;

	if ( alt_fire )
	{
		//bolt->methodOfDeath = MOD_THERMAL_ALT;
		//bolt->splashMethodOfDeath = MOD_THERMAL_ALT;
		//? SPLASH;
		if ( ent && ent->owner && ent->owner->client )
			{
				switch ( ent->owner->client->NPC_class )
				{
				// FORCE
				case CLASS_ARCHER: 
				case CLASS_REAVER: 
				case CLASS_MENTALIST: 
				case CLASS_JEDI:
				case CLASS_IMPWORKER: 
				case CLASS_WIZARD: 
				case CLASS_PROBE: 
				case CLASS_AIR: 
				case CLASS_GOLEM_AIR: 
				case CLASS_AIRREAVER: 
				bolt->methodOfDeath = MOD_ROCKET_ALT;
				bolt->splashMethodOfDeath = MOD_ROCKET_ALT;	
				break;
				// STONE
				case CLASS_DRUID: 
				case CLASS_VAMPIRE_ELF:
				case CLASS_VAMPIRE_ELF2: 
				case CLASS_VAMPIRE_ELF3: 
				case CLASS_EARTHREAVER: 
				case CLASS_EARTH: 
				case CLASS_GOLEM: 
				case CLASS_DEMON_BLADE: 				
				case CLASS_MONMOTHA:
				case CLASS_GONK:
				case CLASS_UGNAUGHT: 
				bolt->methodOfDeath = MOD_FLECHETTE_ALT;
				bolt->splashMethodOfDeath = MOD_FLECHETTE_ALT;	
				break;
				// SOUND 
				case CLASS_BIRD: 
				case CLASS_LYCAN: 
				case CLASS_SONIC: 
				case CLASS_GOLEM_SONIC: 
				case CLASS_TRANDOSHAN: 
				case CLASS_HOWLER: 
				case CLASS_REBEL: 
				case CLASS_VAMPIRE_SOUND: 
				bolt->methodOfDeath = MOD_SONIC_ALT;
				bolt->splashMethodOfDeath = MOD_SONIC_ALT;	
				break;
				// WATER 
				case CLASS_WARRIOR: 
				case CLASS_VAMPIRE_WATER: 
				case CLASS_WATER: 
				case CLASS_GOLEM_WATER: 
				case CLASS_RAHABIM: 
				case CLASS_SHARK: 
				case CLASS_ALORA: 
				case CLASS_SWAMPTROOPER:
				case CLASS_FISH:
				case CLASS_SWAMP:
				bolt->methodOfDeath = MOD_REPEATER;
				bolt->splashMethodOfDeath = MOD_REPEATER;	
				break;
				// SUNLIGHT 
				case CLASS_ALCHEMIST:				 
				case CLASS_VAMPIRE_ARCHER: 
				case CLASS_LIGHT: 
				case CLASS_GOLEM_LIGHT: 
				case CLASS_GOLEM_GOLD: 
				case CLASS_GALAK:
				case CLASS_PROTOCOL:
				case CLASS_MORGANKATARN: 
				bolt->methodOfDeath = MOD_DISRUPTOR;
				bolt->splashMethodOfDeath = MOD_DISRUPTOR;	
				break;
				// ELETTRICA 
				case CLASS_VAMPIRE_LYCAN: 
				case CLASS_LIGHTREAVER: 
				case CLASS_GOLEM_METAL: 
				case CLASS_HALFVAMPIRE: 
				case CLASS_DEMON_ELECTRO: 
				case CLASS_GRAN: 
				case CLASS_SABER_DROID:
				case CLASS_ASSASSIN_DROID: 
				case CLASS_ROCKETTROOPER:
				case CLASS_BOBAFETT: 
				bolt->methodOfDeath = MOD_ELECTROCUTE;
				bolt->splashMethodOfDeath = MOD_ELECTROCUTE;	
				break;
				// HOLY 
				case CLASS_CENOBITA: 
				case CLASS_STORMTROOPER:
				case CLASS_RAZIEL: 
				case CLASS_HOLY: 
				case CLASS_GOLEM_HOLY: 
				case CLASS_LUKE: 
				bolt->methodOfDeath = MOD_DEMP2_ALT;
				bolt->splashMethodOfDeath = MOD_DEMP2_ALT;	
				break;
				// DARKNESS 
				case CLASS_VAMPIRE: 
				case CLASS_VAMPIRE_BLOOD: 
				case CLASS_UMAH: 
				case CLASS_VORADOR:			 
				case CLASS_VAMPIRE_BOUNTY:
				case CLASS_DARK: 
				case CLASS_VOID: 
				case CLASS_GOLEM_SMOKE: 
				case CLASS_GOLEM_DARK: 
				case CLASS_GOLEM_VOID: 
				case CLASS_DARKREAVER: 
				case CLASS_TANKREAVER: 
				case CLASS_VAMPIRE_DARK: 
				case CLASS_VAMPIRE_SARAFAN: 
				case CLASS_NOGHRI:
				case CLASS_FLIER2: 
				case CLASS_SHADOWTROOPER:
				case CLASS_BESPIN_COP: 
				case CLASS_SABOTEUR: 
				bolt->methodOfDeath = MOD_BRYAR_ALT;
				bolt->splashMethodOfDeath = MOD_BRYAR_ALT;	
				break;
				// ACIDIC 
				case CLASS_KYLE:
				case CLASS_KAIN: 
				case CLASS_KAIN2: 
				case CLASS_KAIN3: 
				case CLASS_DUMAHIM: 
				case CLASS_WEEQUAY:
				case CLASS_LIZARD: 
				case CLASS_INTERROGATOR: 
				case CLASS_R2D2:
				case CLASS_R5D2: 	
				case CLASS_REELO: 
				case CLASS_ZEPH:
				case CLASS_DECEIVER:
				case CLASS_SPIDERREAVER:
				case CLASS_SPIDERREAVER2:
				case CLASS_SPIDERREAVER3:	
				case CLASS_DEMON_POISON: 
				case CLASS_BEAST:  			  
				case CLASS_GOLEM_POISON:
				case CLASS_POISON: 
				bolt->methodOfDeath = MOD_SLIME;
				bolt->splashMethodOfDeath = MOD_SLIME;	
				break;
				// UNDEAD 
				case CLASS_TENTACLE: 
				case CLASS_ELDERGOD: 
				case CLASS_SLUAGH: 
				case CLASS_REAPER: 
				case CLASS_DREADNOUGHT: 
				case CLASS_NECROREAVER: 
				case CLASS_NECRO: 
				case CLASS_GOLEM_NECRO: 
				case CLASS_RANCOR:
				case CLASS_MURJJ: 
				case CLASS_ATST: 
				case CLASS_MOUSE:
				case CLASS_SAND_CREATURE: 
				case CLASS_MELC:  
				case CLASS_SUMMONER:
				case CLASS_VAMPIRE_ZOMBIE: 
				bolt->methodOfDeath = MOD_CONC_ALT;
				bolt->splashMethodOfDeath = MOD_CONC_ALT;	
				break;
				// HYLDEN 
				case CLASS_PROPHET: 
				case CLASS_HYLDEN: 
				case CLASS_HYLDENLORD: 
				case CLASS_TAVION:
				case CLASS_DESANN:
				case CLASS_REMOTE:
				case CLASS_SEEKER:
				case CLASS_SENTRY:
				bolt->methodOfDeath = MOD_EMPLACED;
				bolt->splashMethodOfDeath = MOD_EMPLACED;	
				break;
				// BOMBA DI FUOCO 		
				case CLASS_RAZIELHIM: 
				case CLASS_ESKANDOR: 
				case CLASS_HUNTER:			 
				case CLASS_DEMON_FIRE:
				case CLASS_VAMPIRE_FEAR: 
				case CLASS_VAMPIRE_FEAR2: 
				case CLASS_FIREREAVER: 
				case CLASS_TURELIM: 
				case CLASS_FIRE: 
				case CLASS_VAMPIRE_DEMON: 
				case CLASS_GOLEM_FIRE: 
				case CLASS_HELLGOLEM: 
				case CLASS_GOLEM_LAVA: 
				case CLASS_REBORN: 
				case CLASS_WAMPA: 
				case CLASS_TUSKEN: 
				case CLASS_GLIDER: 
				case CLASS_RODIAN: 
				case CLASS_IMPERIAL: 
				case CLASS_HAZARD_TROOPER: 
				case CLASS_CLAW: 
				case CLASS_PLAYER: 
				case CLASS_VEHICLE: 
				case CLASS_FIGHTER: 
				bolt->methodOfDeath = MOD_THERMAL_ALT;
				bolt->splashMethodOfDeath = MOD_THERMAL_ALT;	
				break;  
				// MACE BOMB
				case CLASS_SCOUT: 
				case CLASS_MARK1: 
				case CLASS_MARK2:				 
				bolt->methodOfDeath = MOD_MELEE;
				bolt->splashMethodOfDeath = MOD_MELEE;	
				break;
				// PIANTE SPUNTONE
				case CLASS_IVY: 
				case CLASS_DRUIDREAVER: 
				case CLASS_LANDO: 
				case CLASS_PRISONER: 
				bolt->methodOfDeath = MOD_SLIME;
				bolt->splashMethodOfDeath = MOD_SLIME;	
				break; 
				// CRISTALLI DI GHIACCIO 
				case CLASS_ICEREAVER: 
				case CLASS_VAMPIRE_FROST: 
				case CLASS_GOLEM_ICE: 
				case CLASS_CRYSTAL: 
				case CLASS_BARTENDER:
				case CLASS_GALAKMECH: 
				bolt->methodOfDeath = MOD_REPEATER_ALT;
				bolt->splashMethodOfDeath = MOD_REPEATER_ALT;	
				break;
				// CROCI 
				case CLASS_POLTER: 
				case CLASS_JAWA:
				case CLASS_GHOST: 
				bolt->methodOfDeath = MOD_UNKNOWN;
				bolt->splashMethodOfDeath = MOD_UNKNOWN;	
				break; 
				// BARA 
				case CLASS_COMMANDO: 
				case CLASS_THRALL: 
				case CLASS_NECROMANCER: 
				case CLASS_VAMPIRE_QUEEN:
				case CLASS_VAMPIRE_NECRO: 
				case CLASS_VAMPIRE_NECRO2:
				bolt->methodOfDeath = MOD_CONC_ALT;
				bolt->splashMethodOfDeath = MOD_CONC_ALT;	
				break;
				// ANGELO 
				case CLASS_JANOS: 
				case CLASS_JAN: 
				case CLASS_ARIEL: 
				case CLASS_SARAFAN: 
				bolt->methodOfDeath = MOD_DEMP2_ALT;
				bolt->splashMethodOfDeath = MOD_DEMP2_ALT;	
				break;	
			default:
				break;				
				}
			}
	}
	else
	{
		//bolt->methodOfDeath = MOD_THERMAL;
		//bolt->splashMethodOfDeath = MOD_THERMAL;//? SPLASH;
		if ( ent && ent->owner && ent->owner->client )
			{
				switch ( ent->owner->client->NPC_class )
				{
				// FORCE
				case CLASS_ARCHER: 
				case CLASS_REAVER: 
				case CLASS_MENTALIST: 
				case CLASS_JEDI:
				case CLASS_IMPWORKER: 
				case CLASS_WIZARD: 
				case CLASS_PROBE: 
				case CLASS_AIR: 
				case CLASS_GOLEM_AIR: 
				case CLASS_AIRREAVER: 
				bolt->methodOfDeath = MOD_ROCKET;
				bolt->splashMethodOfDeath = MOD_ROCKET;	
				break;
				// STONE
				case CLASS_DRUID: 
				case CLASS_VAMPIRE_ELF:
				case CLASS_VAMPIRE_ELF2: 
				case CLASS_VAMPIRE_ELF3: 
				case CLASS_EARTHREAVER: 
				case CLASS_EARTH: 
				case CLASS_GOLEM: 
				case CLASS_DEMON_BLADE: 				
				case CLASS_MONMOTHA:
				case CLASS_GONK:
				case CLASS_UGNAUGHT: 
				bolt->methodOfDeath = MOD_FLECHETTE;
				bolt->splashMethodOfDeath = MOD_FLECHETTE;	
				break;
				// SOUND 
				case CLASS_BIRD: 
				case CLASS_LYCAN: 
				case CLASS_SONIC: 
				case CLASS_GOLEM_SONIC: 
				case CLASS_TRANDOSHAN: 
				case CLASS_HOWLER: 
				case CLASS_REBEL: 
				case CLASS_VAMPIRE_SOUND: 
				bolt->methodOfDeath = MOD_SONIC;
				bolt->splashMethodOfDeath = MOD_SONIC;	
				break;
				// WATER 
				case CLASS_WARRIOR: 
				case CLASS_VAMPIRE_WATER: 
				case CLASS_WATER: 
				case CLASS_GOLEM_WATER: 
				case CLASS_RAHABIM: 
				case CLASS_SHARK: 
				case CLASS_ALORA: 
				case CLASS_SWAMPTROOPER:
				case CLASS_FISH:
				case CLASS_SWAMP:
				bolt->methodOfDeath = MOD_REPEATER;
				bolt->splashMethodOfDeath = MOD_REPEATER;	
				break;
				// SUNLIGHT 
				case CLASS_RAZIELHIM: 
				case CLASS_ALCHEMIST:				 
				case CLASS_VAMPIRE_ARCHER: 
				case CLASS_LIGHT: 
				case CLASS_GOLEM_LIGHT: 
				case CLASS_GOLEM_GOLD: 
				case CLASS_GALAK:
				case CLASS_PROTOCOL:
				case CLASS_MORGANKATARN: 
				bolt->methodOfDeath = MOD_DISRUPTOR;
				bolt->splashMethodOfDeath = MOD_DISRUPTOR;	
				break;
				// ELETTRICA 
				case CLASS_VAMPIRE_LYCAN: 
				case CLASS_LIGHTREAVER: 
				case CLASS_GOLEM_METAL: 
				case CLASS_HALFVAMPIRE: 
				case CLASS_DEMON_ELECTRO: 
				case CLASS_GRAN: 
				case CLASS_SABER_DROID:
				case CLASS_ASSASSIN_DROID: 
				case CLASS_ROCKETTROOPER:
				case CLASS_BOBAFETT: 
				bolt->methodOfDeath = MOD_ELECTROCUTE;
				bolt->splashMethodOfDeath = MOD_ELECTROCUTE;	
				break;
				// HOLY 
				case CLASS_CENOBITA: 
				case CLASS_STORMTROOPER:
				case CLASS_RAZIEL: 
				case CLASS_HOLY: 
				case CLASS_GOLEM_HOLY: 
				case CLASS_LUKE: 
				bolt->methodOfDeath = MOD_DEMP2;
				bolt->splashMethodOfDeath = MOD_DEMP2;	
				break;
				// DARKNESS 
				case CLASS_VAMPIRE: 
				case CLASS_VAMPIRE_BLOOD: 
				case CLASS_UMAH: 
				case CLASS_VORADOR:			 
				case CLASS_VAMPIRE_BOUNTY:
				case CLASS_DARK: 
				case CLASS_VOID: 
				case CLASS_GOLEM_SMOKE: 
				case CLASS_GOLEM_DARK: 
				case CLASS_GOLEM_VOID: 
				case CLASS_DARKREAVER: 
				case CLASS_TANKREAVER: 
				case CLASS_VAMPIRE_DARK: 
				case CLASS_VAMPIRE_SARAFAN: 
				case CLASS_NOGHRI:
				case CLASS_FLIER2: 
				case CLASS_SHADOWTROOPER:
				case CLASS_BESPIN_COP: 
				case CLASS_SABOTEUR: 
				bolt->methodOfDeath = MOD_BRYAR;
				bolt->splashMethodOfDeath = MOD_BRYAR;	
				break;
				// ACIDIC 
				case CLASS_KYLE:
				case CLASS_KAIN: 
				case CLASS_KAIN2: 
				case CLASS_KAIN3: 
				case CLASS_DUMAHIM: 
				case CLASS_WEEQUAY:
				case CLASS_LIZARD: 
				case CLASS_INTERROGATOR: 
				case CLASS_R2D2:
				case CLASS_R5D2: 	
				case CLASS_REELO: 
				case CLASS_ZEPH:
				case CLASS_DECEIVER:
				case CLASS_SPIDERREAVER:
				case CLASS_SPIDERREAVER2:
				case CLASS_SPIDERREAVER3:	
				case CLASS_DEMON_POISON: 
				case CLASS_BEAST:  			  
				case CLASS_GOLEM_POISON:
				case CLASS_POISON: 
				bolt->methodOfDeath = MOD_POISON;
				bolt->splashMethodOfDeath = MOD_POISON;	
				break;
				// UNDEAD 
				case CLASS_TENTACLE: 
				case CLASS_ELDERGOD: 
				case CLASS_SLUAGH: 
				case CLASS_REAPER: 
				case CLASS_DREADNOUGHT: 
				case CLASS_NECROREAVER: 
				case CLASS_NECRO: 
				case CLASS_GOLEM_NECRO: 
				case CLASS_RANCOR:
				case CLASS_MURJJ: 
				case CLASS_ATST: 
				case CLASS_MOUSE:
				case CLASS_SAND_CREATURE: 
				case CLASS_MELC:  
				case CLASS_SUMMONER:
				case CLASS_VAMPIRE_ZOMBIE: 
				bolt->methodOfDeath = MOD_CONC;
				bolt->splashMethodOfDeath = MOD_CONC;	
				break;
				// HYLDEN 
				case CLASS_PROPHET: 
				case CLASS_HYLDEN: 
				case CLASS_HYLDENLORD: 
				case CLASS_TAVION:
				case CLASS_DESANN:
				case CLASS_REMOTE:
				case CLASS_SEEKER:
				case CLASS_SENTRY:
				bolt->methodOfDeath = MOD_EMPLACED;
				bolt->splashMethodOfDeath = MOD_EMPLACED;	
				break;
				// BOMBA DI FUOCO 		
				case CLASS_HUNTER:			 
				case CLASS_DEMON_FIRE:
				case CLASS_VAMPIRE_FEAR: 
				case CLASS_VAMPIRE_FEAR2: 
				case CLASS_FIREREAVER: 
				case CLASS_TURELIM: 
				case CLASS_FIRE: 
				case CLASS_VAMPIRE_DEMON: 
				case CLASS_GOLEM_FIRE: 
				case CLASS_HELLGOLEM: 
				case CLASS_GOLEM_LAVA: 
				case CLASS_REBORN: 
				case CLASS_WAMPA: 
				case CLASS_TUSKEN: 
				case CLASS_GLIDER: 
				case CLASS_RODIAN: 
				case CLASS_IMPERIAL: 
				case CLASS_HAZARD_TROOPER: 
				case CLASS_CLAW: 
				case CLASS_PLAYER: 
				case CLASS_VEHICLE:
				case CLASS_FIGHTER: 
				bolt->methodOfDeath = MOD_THERMAL;
				bolt->splashMethodOfDeath = MOD_THERMAL;	
				break;  
				// MACE BOMB
				case CLASS_SCOUT: 
				case CLASS_MARK1: 
				case CLASS_MARK2:				 
				bolt->methodOfDeath = MOD_MELEE;
				bolt->splashMethodOfDeath = MOD_MELEE;	
				break;
				// PIANTE SPUNTONE
				case CLASS_IVY: 
				case CLASS_DRUIDREAVER: 
				case CLASS_LANDO: 
				case CLASS_PRISONER: 
				bolt->methodOfDeath = MOD_POISON;
				bolt->splashMethodOfDeath = MOD_POISON;	
				break; 
				// CRISTALLI DI GHIACCIO 
				case CLASS_ICEREAVER: 
				case CLASS_VAMPIRE_FROST: 
				case CLASS_GOLEM_ICE: 
				case CLASS_CRYSTAL: 
				case CLASS_BARTENDER:
				case CLASS_GALAKMECH: 
				bolt->methodOfDeath = MOD_REPEATER_ALT;
				bolt->splashMethodOfDeath = MOD_REPEATER_ALT;	
				break;
				// CROCI 
				case CLASS_POLTER: 
				case CLASS_JAWA:
				case CLASS_GHOST: 
				bolt->methodOfDeath = MOD_UNKNOWN;
				bolt->splashMethodOfDeath = MOD_UNKNOWN;	
				break; 
				// BARA 
				case CLASS_COMMANDO: 
				case CLASS_THRALL: 
				case CLASS_NECROMANCER: 
				case CLASS_VAMPIRE_QUEEN:
				case CLASS_VAMPIRE_NECRO: 
				case CLASS_VAMPIRE_NECRO2:
				bolt->methodOfDeath = MOD_CONC;
				bolt->splashMethodOfDeath = MOD_CONC;	
				break;
				// ANGELO 
				case CLASS_JANOS: 
				case CLASS_JAN: 
				case CLASS_ARIEL: 
				case CLASS_SARAFAN: 
				bolt->methodOfDeath = MOD_DEMP2;
				bolt->splashMethodOfDeath = MOD_DEMP2;	
				break;	
			default:
				break;
				}
			}
	}

	bolt->s.pos.trTime = level.time;		// move a bit on the very first frame
	VectorCopy( start, bolt->s.pos.trBase );
	
	SnapVector( bolt->s.pos.trDelta );			// save net bandwidth
	VectorCopy (start, bolt->currentOrigin);

	VectorCopy( start, bolt->pos2 );

	return bolt;
}

//---------------------------------------------------------
gentity_t *WP_DropThermal( gentity_t *ent )
//---------------------------------------------------------
{
	AngleVectors( ent->client->ps.viewangles, forwardVec, vrightVec, up );
	CalcEntitySpot( ent, SPOT_WEAPON, muzzle );
	return (WP_FireThermalDetonator( ent, qfalse ));
}
