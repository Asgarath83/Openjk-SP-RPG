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

//---------------------------------------------------------
void WP_FireTuskenRifle( gentity_t *ent )
//---------------------------------------------------------
{
	vec3_t	start;

	VectorCopy( muzzle, start );
	WP_TraceSetStart( ent, start, vec3_origin, vec3_origin );//make sure our start point isn't on the other side of a wall
			// FIRE BOLT   
			if ( ent->client->NPC_class == CLASS_TUSKEN||
				ent->client->NPC_class == CLASS_RODIAN|| 
				ent->client->NPC_class == CLASS_GLIDER|| 
				ent->client->NPC_class == CLASS_PRISONER|| 
				ent->client->NPC_class == CLASS_IMPERIAL|| 
				ent->client->NPC_class == CLASS_HAZARD_TROOPER|| 
				ent->client->NPC_class == CLASS_CLAW|| 
				ent->client->NPC_class == CLASS_PLAYER|| 
				ent->client->NPC_class == CLASS_VEHICLE|| 
				ent->client->NPC_class == CLASS_FIREREAVER||
				ent->client->NPC_class == CLASS_FIRE|| 
				ent->client->NPC_class == CLASS_HELLGOLEM|| 
				ent->client->NPC_class == CLASS_GOLEM_FIRE|| 
				ent->client->NPC_class == CLASS_GOLEM_LAVA|| 
				ent->client->NPC_class == CLASS_MELC||  
				ent->client->NPC_class == CLASS_SUMMONER||
				ent->client->NPC_class == CLASS_VAMPIRE_FEAR|| 
				ent->client->NPC_class == CLASS_VAMPIRE_FEAR2||
				ent->client->NPC_class == CLASS_DEMON_FIRE|| 
				ent->client->NPC_class == CLASS_HUNTER	)
	{
		{
			G_PlayEffect( G_EffectIndex( "tusken/muzzle_flash" ), ent->client->renderInfo.handRPoint );
		}
	}
	// FORCE BOLTS  
	else if ( ent->client->NPC_class == CLASS_BIRD|| 
				ent->client->NPC_class == CLASS_REAVER|| 
				ent->client->NPC_class == CLASS_TANKREAVER||
				ent->client->NPC_class == CLASS_AIRREAVER|| 
				ent->client->NPC_class == CLASS_MENTALIST|| 
				ent->client->NPC_class == CLASS_AIR|| 
				ent->client->NPC_class == CLASS_GOLEM_AIR|| 
				ent->client->NPC_class == CLASS_TURELIM|| 				
				ent->client->NPC_class == CLASS_JEDI||
				ent->client->NPC_class == CLASS_GRAN|| 
				ent->client->NPC_class == CLASS_PROBE|| 
				ent->client->NPC_class == CLASS_ROCKETTROOPER||
				ent->client->NPC_class == CLASS_BOBAFETT||
				ent->client->NPC_class == CLASS_BESPIN_COP|| 
				ent->client->NPC_class == CLASS_IMPWORKER||
				ent->client->NPC_class == CLASS_WIZARD|| 
				ent->client->NPC_class == CLASS_ARCHER )
	{
		{
			G_PlayEffect( G_EffectIndex( "tusken2/muzzle_flash" ), ent->client->renderInfo.handRPoint );
		}
	}
	// STONE BOLTS    
	else if ( ent->client->NPC_class == CLASS_REELO|| 
				ent->client->NPC_class == CLASS_MONMOTHA||
				ent->client->NPC_class == CLASS_LIZARD|| 
				ent->client->NPC_class == CLASS_WEEQUAY||
				ent->client->NPC_class == CLASS_JAN||
				ent->client->NPC_class == CLASS_GONK||
				ent->client->NPC_class == CLASS_GALAKMECH|| 
				ent->client->NPC_class == CLASS_UGNAUGHT|| 	
				ent->client->NPC_class == CLASS_SPIDERREAVER||
				ent->client->NPC_class == CLASS_SPIDERREAVER2||
				ent->client->NPC_class == CLASS_EARTHREAVER|| 
				ent->client->NPC_class == CLASS_EARTH|| 
				ent->client->NPC_class == CLASS_GOLEM|| 
				ent->client->NPC_class == CLASS_CRYSTAL|| 
				ent->client->NPC_class == CLASS_DUMAHIM|| 
				ent->client->NPC_class == CLASS_VAMPIRE_ELF|| 
				ent->client->NPC_class == CLASS_VAMPIRE_ELF2|| 
				ent->client->NPC_class == CLASS_VAMPIRE_ELF3|| 
				ent->client->NPC_class == CLASS_WARRIOR|| 
				ent->client->NPC_class == CLASS_DEMON_BLADE	)
	{
		{
			G_PlayEffect( G_EffectIndex( "tusken3/muzzle_flash" ), ent->client->renderInfo.handRPoint );
		}
	}
	// SONIC BOLTS  
	else if (  ent->client->NPC_class == CLASS_TRANDOSHAN|| 
				ent->client->NPC_class == CLASS_DRUIDREAVER|| 
				ent->client->NPC_class == CLASS_LANDO||
				ent->client->NPC_class == CLASS_HOWLER|| 
				ent->client->NPC_class == CLASS_SONIC|| 
				ent->client->NPC_class == CLASS_GOLEM_SONIC|| 
				ent->client->NPC_class == CLASS_DRUID||
				ent->client->NPC_class == CLASS_LYCAN|| 
				ent->client->NPC_class == CLASS_VAMPIRE_SOUND|| 
				ent->client->NPC_class == CLASS_R2D2||
				ent->client->NPC_class == CLASS_R5D2 )
	{
		{
			G_PlayEffect( G_EffectIndex( "tusken4/muzzle_flash" ), ent->client->renderInfo.handRPoint );
		}
	}
	// WATER BOLTS  
	else if ( ent->client->NPC_class == CLASS_SHARK|| 
				ent->client->NPC_class == CLASS_SWAMP|| 
				ent->client->NPC_class == CLASS_CENOBITA|| 
				ent->client->NPC_class == CLASS_FISH||
				ent->client->NPC_class == CLASS_BARTENDER||
				ent->client->NPC_class == CLASS_SWAMPTROOPER|| 
				ent->client->NPC_class == CLASS_ICEREAVER||
				ent->client->NPC_class == CLASS_WATER|| 
				ent->client->NPC_class == CLASS_GOLEM_WATER|| 
				ent->client->NPC_class == CLASS_GOLEM_ICE|| 
				ent->client->NPC_class == CLASS_RAHABIM||
				ent->client->NPC_class == CLASS_ALORA|| 
				ent->client->NPC_class == CLASS_VAMPIRE_WATER|| 
				ent->client->NPC_class == CLASS_VAMPIRE_FROST )
	{
		{
			G_PlayEffect( G_EffectIndex( "tusken5/muzzle_flash" ), ent->client->renderInfo.handRPoint );
		}
	}
	// SUNLIGHT BOLT  
	else if ( ent->client->NPC_class == CLASS_GALAK||
				ent->client->NPC_class == CLASS_ASSASSIN_DROID|| 
				ent->client->NPC_class == CLASS_SABER_DROID||
				ent->client->NPC_class == CLASS_PROTOCOL||
				ent->client->NPC_class == CLASS_MORGANKATARN|| 
				ent->client->NPC_class == CLASS_LIGHTREAVER||
				ent->client->NPC_class == CLASS_GOLEM_LIGHT|| 
				ent->client->NPC_class == CLASS_GOLEM_GOLD|| 
				ent->client->NPC_class == CLASS_LIGHT|| 
				ent->client->NPC_class == CLASS_GOLEM_METAL|| 
				ent->client->NPC_class == CLASS_VAMPIRE_ARCHER|| 
				ent->client->NPC_class == CLASS_SCOUT|| 
				ent->client->NPC_class == CLASS_DEMON_ELECTRO )
	{
		{
			G_PlayEffect( G_EffectIndex( "tusken6/muzzle_flash" ), ent->client->renderInfo.handRPoint );
		}
	}
	// HOLY BOLT 
	else if ( ent->client->NPC_class == CLASS_STORMTROOPER||
				ent->client->NPC_class == CLASS_LUKE|| 
				ent->client->NPC_class == CLASS_RAZIEL|| 
				ent->client->NPC_class == CLASS_HOLY|| 
				ent->client->NPC_class == CLASS_GOLEM_HOLY|| 
				ent->client->NPC_class == CLASS_ARIEL|| 
				ent->client->NPC_class == CLASS_MARK1|| 
				ent->client->NPC_class == CLASS_MARK2||
				ent->client->NPC_class == CLASS_SARAFAN )
	{
		{
			G_PlayEffect( G_EffectIndex( "tusken7/muzzle_flash" ), ent->client->renderInfo.handRPoint );
		}
	}
	// VAMPIRIC BOLT 
	else if (  ent->client->NPC_class == CLASS_REBORN||
				ent->client->NPC_class == CLASS_KYLE||
				ent->client->NPC_class == CLASS_NOGHRI|| 
				ent->client->NPC_class == CLASS_FLIER2||
				ent->client->NPC_class == CLASS_WAMPA|| 
				ent->client->NPC_class == CLASS_ZEPH||
				ent->client->NPC_class == CLASS_DECEIVER||
				ent->client->NPC_class == CLASS_VAMPIRE||
				ent->client->NPC_class == CLASS_VAMPIRE_DEMON||
				ent->client->NPC_class == CLASS_KAIN||
				ent->client->NPC_class == CLASS_KAIN2|| 
				ent->client->NPC_class == CLASS_KAIN3||
				ent->client->NPC_class == CLASS_UMAH|| 
				ent->client->NPC_class == CLASS_VORADOR||
				ent->client->NPC_class == CLASS_JANOS||
				ent->client->NPC_class == CLASS_VAMPIRE_LYCAN|| 
				ent->client->NPC_class == CLASS_VAMPIRE_BLOOD|| 
				ent->client->NPC_class == CLASS_HALFVAMPIRE|| 
				ent->client->NPC_class == CLASS_RAZIELHIM ||  
				ent->client->NPC_class == CLASS_ESKANDOR ||  
				ent->client->NPC_class == CLASS_VAMPIRE_BOUNTY )
	{
		{
			G_PlayEffect( G_EffectIndex( "tusken8/muzzle_flash" ), ent->client->renderInfo.handRPoint );
		}
	}
	// UNDEADS 
	else if ( ent->client->NPC_class == CLASS_JAWA||
				ent->client->NPC_class == CLASS_RANCOR||
				ent->client->NPC_class == CLASS_COMMANDO|| 
				ent->client->NPC_class == CLASS_MURJJ|| 
				ent->client->NPC_class == CLASS_NECROREAVER||
				ent->client->NPC_class == CLASS_NECRO|| 
				ent->client->NPC_class == CLASS_GOLEM_NECRO|| 
				ent->client->NPC_class == CLASS_THRALL|| 
				ent->client->NPC_class == CLASS_NECROMANCER|| 
				ent->client->NPC_class == CLASS_VAMPIRE_QUEEN||
				ent->client->NPC_class == CLASS_VAMPIRE_NECRO|| 
				ent->client->NPC_class == CLASS_VAMPIRE_NECRO2||
				ent->client->NPC_class == CLASS_VAMPIRE_ZOMBIE|| 
				ent->client->NPC_class == CLASS_POLTER )
	{
		{
			G_PlayEffect( G_EffectIndex( "tusken0/muzzle_flash" ), ent->client->renderInfo.handRPoint );
		}
	}
	// HYLDEN DEADLY BOLTS   
	else if ( ent->client->NPC_class == CLASS_PROPHET|| 	
				ent->client->NPC_class == CLASS_TAVION||
				ent->client->NPC_class == CLASS_DESANN||
				ent->client->NPC_class == CLASS_HYLDEN|| 
				ent->client->NPC_class == CLASS_HYLDENLORD|| 
				ent->client->NPC_class == CLASS_REMOTE||
				ent->client->NPC_class == CLASS_SEEKER||
				ent->client->NPC_class == CLASS_SENTRY||
				ent->client->NPC_class == CLASS_ATST|| 
				ent->client->NPC_class == CLASS_MOUSE||
				ent->client->NPC_class == CLASS_SAND_CREATURE|| 	
				ent->client->NPC_class == CLASS_TENTACLE||  
				ent->client->NPC_class == CLASS_ELDERGOD )
	{
		{
			G_PlayEffect( G_EffectIndex( "tusken9/muzzle_flash" ), ent->client->renderInfo.handRPoint );
		}
	}
	// SHADOW BOLTS 
	else if ( ent->client->NPC_class == CLASS_SHADOWTROOPER||
				ent->client->NPC_class == CLASS_SABOTEUR|| 
				ent->client->NPC_class == CLASS_DARKREAVER||
				ent->client->NPC_class == CLASS_DARK|| 
				ent->client->NPC_class == CLASS_VOID|| 
				ent->client->NPC_class == CLASS_GOLEM_SMOKE|| 
				ent->client->NPC_class == CLASS_GOLEM_VOID|| 
				ent->client->NPC_class == CLASS_GOLEM_DARK|| 
				ent->client->NPC_class == CLASS_VAMPIRE_DARK|| 
				ent->client->NPC_class == CLASS_VAMPIRE_SARAFAN )
	{
		{
			G_PlayEffect( G_EffectIndex( "tusken1/muzzle_flash" ), ent->client->renderInfo.handRPoint );
		}
	}
	// POISON ARROWS 
	else if ( ent->client->NPC_class == CLASS_SPIDERREAVER3 || 
				ent->client->NPC_class == CLASS_MINEMONSTER ||
				ent->client->NPC_class == CLASS_INTERROGATOR || 
				ent->client->NPC_class == CLASS_IVY|| 
				ent->client->NPC_class == CLASS_POISON|| 
				ent->client->NPC_class == CLASS_GOLEM_POISON|| 		
				ent->client->NPC_class == CLASS_DEMON_POISON||
				ent->client->NPC_class == CLASS_BEAST|| 				
				ent->client->NPC_class == CLASS_ALCHEMIST )
	{
		{
			G_PlayEffect( G_EffectIndex( "tusken10/muzzle_flash" ), ent->client->renderInfo.handRPoint );
		}
	}



	if ( !(ent->client->ps.forcePowersActive&(1<<FP_SEE))
		|| ent->client->ps.forcePowerLevel[FP_SEE] < FORCE_LEVEL_2 )
	{//force sight 2+ gives perfect aim
		//FIXME: maybe force sight level 3 autoaims some?
		if ( ent->NPC && ent->NPC->currentAim < 5 )
		{
			vec3_t	angs;

			vectoangles( forwardVec, angs );

			if ( ent->client->NPC_class == CLASS_STORMTROOPER || ent->client->NPC_class == CLASS_MARK1 ||
				ent->client->NPC_class == CLASS_MARK2 )
			{//*sigh*, hack to make impworkers less accurate without affecteing imperial officer accuracy
				angs[PITCH] += ( crandom() * (BLASTER_NPC_SPREAD+(6-ent->NPC->currentAim)*0.25f));//was 0.5f
				angs[YAW]	+= ( crandom() * (BLASTER_NPC_SPREAD+(6-ent->NPC->currentAim)*0.25f));//was 0.5f
			}
			else
			{
				angs[PITCH] += ( crandom() * ((5-ent->NPC->currentAim)*0.25f) );
				angs[YAW]	+= ( crandom() * ((5-ent->NPC->currentAim)*0.25f) );
			}

			AngleVectors( angs, forwardVec, NULL, NULL );
		}
	}

	WP_MissileTargetHint(ent, start, forwardVec);

	gentity_t	*missile = CreateMissile( start, forwardVec, TUSKEN_RIFLE_VEL, 10000, ent, qfalse );

	missile->classname = "trifle_proj";
	missile->s.weapon = WP_TUSKEN_RIFLE;

	if ( ent->s.number < MAX_CLIENTS || g_spskill->integer >= 2 )
	{
		missile->damage = TUSKEN_RIFLE_DAMAGE_HARD;
	}
	else if ( g_spskill->integer > 0 )
	{
		missile->damage = TUSKEN_RIFLE_DAMAGE_MEDIUM;
	}
	else
	{
		missile->damage = TUSKEN_RIFLE_DAMAGE_EASY;
	}
	missile->dflags = DAMAGE_DEATH_KNOCKBACK;

	// FIRE BOLT   
	if ( ent->client->NPC_class == CLASS_TUSKEN||
				ent->client->NPC_class == CLASS_RODIAN|| 
				ent->client->NPC_class == CLASS_GLIDER|| 
				ent->client->NPC_class == CLASS_PRISONER|| 
				ent->client->NPC_class == CLASS_IMPERIAL|| 
				ent->client->NPC_class == CLASS_HAZARD_TROOPER|| 
				ent->client->NPC_class == CLASS_CLAW|| 
				ent->client->NPC_class == CLASS_PLAYER|| 
				ent->client->NPC_class == CLASS_VEHICLE|| 
				ent->client->NPC_class == CLASS_FIREREAVER||
				ent->client->NPC_class == CLASS_FIRE|| 
				ent->client->NPC_class == CLASS_HELLGOLEM|| 
				ent->client->NPC_class == CLASS_GOLEM_FIRE|| 
				ent->client->NPC_class == CLASS_GOLEM_LAVA|| 
				ent->client->NPC_class == CLASS_MELC||  
				ent->client->NPC_class == CLASS_SUMMONER||
				ent->client->NPC_class == CLASS_VAMPIRE_FEAR|| 
				ent->client->NPC_class == CLASS_VAMPIRE_FEAR2||
				ent->client->NPC_class == CLASS_DEMON_FIRE|| 
				ent->client->NPC_class == CLASS_HUNTER )
	{
		missile->methodOfDeath = MOD_BLASTER;
	}
	// FORCE BOLTS  
	else if ( ent->client->NPC_class == CLASS_BIRD|| 
				ent->client->NPC_class == CLASS_REAVER|| 
				ent->client->NPC_class == CLASS_TANKREAVER||
				ent->client->NPC_class == CLASS_AIRREAVER|| 
				ent->client->NPC_class == CLASS_MENTALIST|| 
				ent->client->NPC_class == CLASS_AIR|| 
				ent->client->NPC_class == CLASS_GOLEM_AIR|| 
				ent->client->NPC_class == CLASS_TURELIM|| 				
				ent->client->NPC_class == CLASS_JEDI||
				ent->client->NPC_class == CLASS_GRAN|| 
				ent->client->NPC_class == CLASS_PROBE|| 
				ent->client->NPC_class == CLASS_ROCKETTROOPER||
				ent->client->NPC_class == CLASS_BOBAFETT||
				ent->client->NPC_class == CLASS_BESPIN_COP|| 
				ent->client->NPC_class == CLASS_IMPWORKER||
				ent->client->NPC_class == CLASS_WIZARD|| 
				ent->client->NPC_class == CLASS_ARCHER )
	{
		missile->methodOfDeath = MOD_ROCKET;
	}
	// STONE BOLTS    
	else if ( ent->client->NPC_class == CLASS_REELO|| 
				ent->client->NPC_class == CLASS_MONMOTHA||
				ent->client->NPC_class == CLASS_LIZARD|| 
				ent->client->NPC_class == CLASS_WEEQUAY||
				ent->client->NPC_class == CLASS_JAN||
				ent->client->NPC_class == CLASS_GONK||
				ent->client->NPC_class == CLASS_GALAKMECH|| 
				ent->client->NPC_class == CLASS_UGNAUGHT|| 	
				ent->client->NPC_class == CLASS_SPIDERREAVER||
				ent->client->NPC_class == CLASS_SPIDERREAVER2||
				ent->client->NPC_class == CLASS_EARTHREAVER|| 
				ent->client->NPC_class == CLASS_EARTH|| 
				ent->client->NPC_class == CLASS_GOLEM|| 
				ent->client->NPC_class == CLASS_CRYSTAL|| 
				ent->client->NPC_class == CLASS_DUMAHIM|| 
				ent->client->NPC_class == CLASS_VAMPIRE_ELF|| 
				ent->client->NPC_class == CLASS_VAMPIRE_ELF2|| 
				ent->client->NPC_class == CLASS_VAMPIRE_ELF3|| 
				ent->client->NPC_class == CLASS_WARRIOR|| 
				ent->client->NPC_class == CLASS_DEMON_BLADE 	)
	{
		missile->methodOfDeath = MOD_FLECHETTE;
	}
	// SONIC BOLTS  
	else if ( ent->client->NPC_class == CLASS_TRANDOSHAN|| 
				ent->client->NPC_class == CLASS_DRUIDREAVER|| 
				ent->client->NPC_class == CLASS_LANDO||
				ent->client->NPC_class == CLASS_HOWLER|| 
				ent->client->NPC_class == CLASS_SONIC|| 
				ent->client->NPC_class == CLASS_GOLEM_SONIC|| 
				ent->client->NPC_class == CLASS_DRUID||
				ent->client->NPC_class == CLASS_LYCAN|| 
				ent->client->NPC_class == CLASS_VAMPIRE_SOUND|| 
				ent->client->NPC_class == CLASS_R2D2||
				ent->client->NPC_class == CLASS_R5D2 )
	{
		missile->methodOfDeath = MOD_SONIC;
	}
	// WATER BOLTS  
	else if ( ent->client->NPC_class == CLASS_SHARK|| 
				ent->client->NPC_class == CLASS_SWAMP|| 
				ent->client->NPC_class == CLASS_CENOBITA|| 
				ent->client->NPC_class == CLASS_FISH||
				ent->client->NPC_class == CLASS_BARTENDER||
				ent->client->NPC_class == CLASS_SWAMPTROOPER|| 
				ent->client->NPC_class == CLASS_ICEREAVER||
				ent->client->NPC_class == CLASS_WATER|| 
				ent->client->NPC_class == CLASS_GOLEM_WATER|| 
				ent->client->NPC_class == CLASS_GOLEM_ICE|| 
				ent->client->NPC_class == CLASS_RAHABIM||
				ent->client->NPC_class == CLASS_ALORA|| 
				ent->client->NPC_class == CLASS_VAMPIRE_WATER|| 
				ent->client->NPC_class == CLASS_VAMPIRE_FROST )
	{
		missile->methodOfDeath = MOD_REPEATER;
	}
	// SUNLIGHT BOLT  
	else if ( ent->client->NPC_class == CLASS_GALAK||
				ent->client->NPC_class == CLASS_ASSASSIN_DROID|| 
				ent->client->NPC_class == CLASS_SABER_DROID||
				ent->client->NPC_class == CLASS_PROTOCOL||
				ent->client->NPC_class == CLASS_MORGANKATARN|| 
				ent->client->NPC_class == CLASS_LIGHTREAVER||
				ent->client->NPC_class == CLASS_GOLEM_LIGHT|| 
				ent->client->NPC_class == CLASS_GOLEM_GOLD|| 
				ent->client->NPC_class == CLASS_LIGHT|| 
				ent->client->NPC_class == CLASS_GOLEM_METAL|| 
				ent->client->NPC_class == CLASS_VAMPIRE_ARCHER|| 
				ent->client->NPC_class == CLASS_SCOUT|| 
				ent->client->NPC_class == CLASS_DEMON_ELECTRO )
	{
		missile->methodOfDeath = MOD_SNIPER;
	}
	// HOLY BOLT 
	else if ( ent->client->NPC_class == CLASS_STORMTROOPER||
				ent->client->NPC_class == CLASS_LUKE|| 
				ent->client->NPC_class == CLASS_RAZIEL|| 
				ent->client->NPC_class == CLASS_HOLY|| 
				ent->client->NPC_class == CLASS_GOLEM_HOLY|| 
				ent->client->NPC_class == CLASS_ARIEL|| 
				ent->client->NPC_class == CLASS_MARK1|| 
				ent->client->NPC_class == CLASS_MARK2||
				ent->client->NPC_class == CLASS_SARAFAN )
	{
		missile->methodOfDeath = MOD_DEMP2;
	}
	// VAMPIRIC BOLT 
	else if (ent->client->NPC_class == CLASS_REBORN||
				ent->client->NPC_class == CLASS_KYLE||
				ent->client->NPC_class == CLASS_NOGHRI|| 
				ent->client->NPC_class == CLASS_FLIER2||
				ent->client->NPC_class == CLASS_WAMPA|| 
				ent->client->NPC_class == CLASS_ZEPH||
				ent->client->NPC_class == CLASS_DECEIVER||
				ent->client->NPC_class == CLASS_VAMPIRE||
				ent->client->NPC_class == CLASS_VAMPIRE_DEMON||
				ent->client->NPC_class == CLASS_KAIN||
				ent->client->NPC_class == CLASS_KAIN2|| 
				ent->client->NPC_class == CLASS_KAIN3||
				ent->client->NPC_class == CLASS_UMAH|| 
				ent->client->NPC_class == CLASS_VORADOR||
				ent->client->NPC_class == CLASS_JANOS||
				ent->client->NPC_class == CLASS_VAMPIRE_LYCAN|| 
				ent->client->NPC_class == CLASS_VAMPIRE_BLOOD|| 
				ent->client->NPC_class == CLASS_HALFVAMPIRE|| 
				ent->client->NPC_class == CLASS_RAZIELHIM ||  
				ent->client->NPC_class == CLASS_ESKANDOR ||  
				ent->client->NPC_class == CLASS_VAMPIRE_BOUNTY )
	{
		missile->methodOfDeath = MOD_FORCE_DRAIN;
	}
	// UNDEADS 
	else if ( ent->client->NPC_class == CLASS_JAWA||
				ent->client->NPC_class == CLASS_RANCOR||
				ent->client->NPC_class == CLASS_COMMANDO|| 
				ent->client->NPC_class == CLASS_MURJJ|| 
				ent->client->NPC_class == CLASS_NECROREAVER||
				ent->client->NPC_class == CLASS_NECRO|| 
				ent->client->NPC_class == CLASS_GOLEM_NECRO|| 
				ent->client->NPC_class == CLASS_THRALL|| 
				ent->client->NPC_class == CLASS_NECROMANCER|| 
				ent->client->NPC_class == CLASS_VAMPIRE_QUEEN||
				ent->client->NPC_class == CLASS_VAMPIRE_NECRO|| 
				ent->client->NPC_class == CLASS_VAMPIRE_NECRO2||
				ent->client->NPC_class == CLASS_VAMPIRE_ZOMBIE|| 
				ent->client->NPC_class == CLASS_POLTER ) 
	{
		missile->methodOfDeath = MOD_CONC;
	}
	// HYLDEN DEADLY BOLTS   
	else if ( ent->client->NPC_class == CLASS_PROPHET|| 	
				ent->client->NPC_class == CLASS_TAVION||
				ent->client->NPC_class == CLASS_DESANN||
				ent->client->NPC_class == CLASS_HYLDEN|| 
				ent->client->NPC_class == CLASS_HYLDENLORD|| 
				ent->client->NPC_class == CLASS_REMOTE||
				ent->client->NPC_class == CLASS_SEEKER||
				ent->client->NPC_class == CLASS_SENTRY||
				ent->client->NPC_class == CLASS_ATST|| 
				ent->client->NPC_class == CLASS_MOUSE||
				ent->client->NPC_class == CLASS_SAND_CREATURE|| 	
				ent->client->NPC_class == CLASS_TENTACLE||  
				ent->client->NPC_class == CLASS_ELDERGOD )
	{
		missile->methodOfDeath = MOD_EMPLACED;
	}
	// SHADOW BOLTS 
	else if ( ent->client->NPC_class == CLASS_SHADOWTROOPER||
				ent->client->NPC_class == CLASS_SABOTEUR|| 
				ent->client->NPC_class == CLASS_DARKREAVER||
				ent->client->NPC_class == CLASS_DARK|| 
				ent->client->NPC_class == CLASS_VOID|| 
				ent->client->NPC_class == CLASS_GOLEM_SMOKE|| 
				ent->client->NPC_class == CLASS_GOLEM_VOID|| 
				ent->client->NPC_class == CLASS_GOLEM_DARK|| 
				ent->client->NPC_class == CLASS_VAMPIRE_DARK|| 
				ent->client->NPC_class == CLASS_VAMPIRE_SARAFAN )
	{
		missile->methodOfDeath = MOD_BRYAR;
	}
	// POISON ARROWS 
	else if ( ent->client->NPC_class == CLASS_SPIDERREAVER3|| 
				ent->client->NPC_class == CLASS_MINEMONSTER||
				ent->client->NPC_class == CLASS_INTERROGATOR|| 
				ent->client->NPC_class == CLASS_IVY|| 
				ent->client->NPC_class == CLASS_POISON|| 
				ent->client->NPC_class == CLASS_GOLEM_POISON|| 		
				ent->client->NPC_class == CLASS_DEMON_POISON||
				ent->client->NPC_class == CLASS_BEAST|| 				
				ent->client->NPC_class == CLASS_ALCHEMIST )
	{
		missile->methodOfDeath = MOD_POISON;
	} 
	
	//missile->methodOfDeath = MOD_BRYAR;//???

	missile->clipmask = MASK_SHOT | CONTENTS_LIGHTSABER;

	// we don't want it to bounce forever
	missile->bounceCount = 8;
}


