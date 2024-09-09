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


//---------------
//	Blaster
//---------------

//---------------------------------------------------------
void WP_FireBlasterMissile( gentity_t *ent, vec3_t start, vec3_t dir, qboolean altFire )
//---------------------------------------------------------
{
	int velocity	= BLASTER_VELOCITY;
	int	damage		= altFire ? weaponData[WP_BLASTER].damage : weaponData[WP_BLASTER].damage;

	if ( ent && ent->client && ent->client->NPC_class == CLASS_VEHICLE )
	{
		damage *= 3;
		velocity = ATST_MAIN_VEL + ent->client->ps.speed;
	}
	else if ( ent && ent->client && ent->client->NPC_class == CLASS_HAZARD_TROOPER )
	{
		damage *= 1.5; // Elementali del Calore fanno 50% di danno in più!
	}
	else if ( ent && ent->client && ent->client->NPC_class == CLASS_CLAW )
	{
		damage *= 2; // Elementali del Magma fanno 50% di danno doppio!!
	}
	else
	{
		// If an enemy is shooting at us, lower the velocity so you have a chance to evade
		if ( ent->client && ent->client->ps.clientNum != 0 && ent->client->NPC_class != CLASS_BOBAFETT )
		{
			if ( g_spskill->integer < 2 )
			{		
				velocity *= BLASTER_NPC_VEL_CUT;
			}
			else
			{
				velocity *= BLASTER_NPC_HARD_VEL_CUT;
			}
		}
	}

	WP_TraceSetStart( ent, start, vec3_origin, vec3_origin );//make sure our start point isn't on the other side of a wall

	WP_MissileTargetHint(ent, start, dir);

	gentity_t *missile = CreateMissile( start, dir, velocity, 10000, ent, altFire );

	missile->classname = "blaster_proj";
	missile->s.weapon = WP_BLASTER;

	// Do the damages
	if ( ent->s.number != 0 && ent->client->NPC_class != CLASS_BOBAFETT )
	{
		if ( g_spskill->integer == 0 )
		{
			damage = BLASTER_NPC_DAMAGE_EASY;
		}
		else if ( g_spskill->integer == 1 )
		{
			damage = BLASTER_NPC_DAMAGE_NORMAL;
		}
		else
		{
			damage = BLASTER_NPC_DAMAGE_HARD;
		}
	}

if ( altFire )
{
	// THE BLOOD FLAMES  
	if ( ent->client->NPC_class == CLASS_ALORA || 
			ent->client->NPC_class == CLASS_REBORN ||
			ent->client->NPC_class == CLASS_KYLE ||
			ent->client->NPC_class == CLASS_FLIER2 || 
			ent->client->NPC_class == CLASS_NOGHRI ||
			ent->client->NPC_class == CLASS_SWAMPTROOPER ||
			ent->client->NPC_class == CLASS_FISH ||
			ent->client->NPC_class == CLASS_GRAN ||
			ent->client->NPC_class == CLASS_MONMOTHA || 
			ent->client->NPC_class == CLASS_DECEIVER || 
			ent->client->NPC_class == CLASS_ZEPH || 
			ent->client->NPC_class == CLASS_TURELIM || 
			ent->client->NPC_class == CLASS_RAHABIM || 
			ent->client->NPC_class == CLASS_VAMPIRE ||   
			ent->client->NPC_class == CLASS_KAIN ||
			ent->client->NPC_class == CLASS_KAIN2 ||  
			ent->client->NPC_class == CLASS_KAIN3 ||
			ent->client->NPC_class == CLASS_UMAH || 
			ent->client->NPC_class == CLASS_VORADOR || 
			ent->client->NPC_class == CLASS_JANOS ||  
			ent->client->NPC_class == CLASS_NECROMANCER ||  
			ent->client->NPC_class == CLASS_VAMPIRE_QUEEN ||  
			ent->client->NPC_class == CLASS_VAMPIRE_DARK ||  
			ent->client->NPC_class == CLASS_VAMPIRE_NECRO ||   
			ent->client->NPC_class == CLASS_VAMPIRE_NECRO2 || 
			ent->client->NPC_class == CLASS_VAMPIRE_FEAR ||  
			ent->client->NPC_class == CLASS_VAMPIRE_BLOOD || 
			ent->client->NPC_class == CLASS_VAMPIRE_SOUND || 
			ent->client->NPC_class == CLASS_VAMPIRE_LYCAN ||  
			ent->client->NPC_class == CLASS_VAMPIRE_ELF ||  
			ent->client->NPC_class == CLASS_VAMPIRE_ELF2 ||  
			ent->client->NPC_class == CLASS_VAMPIRE_ELF3 || 
			ent->client->NPC_class == CLASS_VAMPIRE_WATER ||  
			ent->client->NPC_class == CLASS_VAMPIRE_FROST || 
			ent->client->NPC_class == CLASS_VAMPIRE_ARCHER || 
			ent->client->NPC_class == CLASS_HALFVAMPIRE ||
			ent->client->NPC_class == CLASS_RAZIELHIM ||  
			ent->client->NPC_class == CLASS_ESKANDOR ||    
			ent->client->NPC_class == CLASS_VAMPIRE_BOUNTY || 
			ent->client->NPC_class == CLASS_VAMPIRE_SARAFAN ||
			ent->client->NPC_class == CLASS_GOLEM_DARK || 
			ent->client->NPC_class == CLASS_DARK || 
			ent->client->NPC_class == CLASS_VOID || 
			ent->client->NPC_class == CLASS_GOLEM_VOID )
	{
		{
			damage *= 1.25;
			G_PlayEffect( G_EffectIndex( "fire_blood/altmuzzle_flash" ), ent->client->renderInfo.handRPoint );
		}
	}
	// THE WARM BALLS  
	else if ( ent->client->NPC_class == CLASS_PROBE || 
			ent->client->NPC_class == CLASS_HAZARD_TROOPER ||
			ent->client->NPC_class == CLASS_SABOTEUR || 
			ent->client->NPC_class == CLASS_SHADOWTROOPER || 
			ent->client->NPC_class == CLASS_IMPERIAL ||
			ent->client->NPC_class == CLASS_WAMPA ||
			ent->client->NPC_class == CLASS_BOBAFETT ||
			ent->client->NPC_class == CLASS_DEMON_FIRE || 
			ent->client->NPC_class == CLASS_HELLGOLEM || 
			ent->client->NPC_class == CLASS_VAMPIRE_DEMON || 
			ent->client->NPC_class == CLASS_FIREREAVER ||
			ent->client->NPC_class == CLASS_GOLEM_SMOKE || 
			ent->client->NPC_class == CLASS_AIR || 			
			ent->client->NPC_class == CLASS_GOLEM_AIR ||
			ent->client->NPC_class == CLASS_LIGHT || 
			ent->client->NPC_class == CLASS_GOLEM_LIGHT ||
			ent->client->NPC_class == CLASS_GOLEM_GOLD ||
			ent->client->NPC_class == CLASS_LYCAN )
	{
		{
			damage *= 1.5;
			G_PlayEffect( G_EffectIndex( "blaster2/altmuzzle_flash" ), ent->client->renderInfo.handRPoint );
		}
	}
	// THE MAGMA BALLS  
	else if ( ent->client->NPC_class == CLASS_CLAW ||
			ent->client->NPC_class == CLASS_EARTH || 
			ent->client->NPC_class == CLASS_GOLEM || 
			ent->client->NPC_class == CLASS_GOLEM_LAVA || 
			ent->client->NPC_class == CLASS_REELO || 
			ent->client->NPC_class == CLASS_SPIDERREAVER3 || 
			ent->client->NPC_class == CLASS_GONK || 
			ent->client->NPC_class == CLASS_CRYSTAL ||
			ent->client->NPC_class == CLASS_VAMPIRE_GHOST )
	{
		{
			damage *= 2;
			G_PlayEffect( G_EffectIndex( "blaster3/altmuzzle_flash" ), ent->client->renderInfo.handRPoint );
		}
	}
	// THE HELL FIRE - BOSSES 
	else if ( ent->client->NPC_class == CLASS_MOUSE || 
			ent->client->NPC_class == CLASS_ATST ||
			ent->client->NPC_class == CLASS_SAND_CREATURE || 
			ent->client->NPC_class == CLASS_ELDERGOD || 
			ent->client->NPC_class == CLASS_TENTACLE || 
			ent->client->NPC_class == CLASS_CENOBITA )
	{
		{
			damage *= 4;
			G_PlayEffect( G_EffectIndex( "fire_hell/altmuzzle_flash" ), ent->client->renderInfo.handRPoint );
		}
	}
	// THE GHOST FIRE 
	else if ( ent->client->NPC_class == CLASS_TUSKEN ||
			ent->client->NPC_class == CLASS_VAMPIRE_ZOMBIE || 
			ent->client->NPC_class == CLASS_JAWA ||
			ent->client->NPC_class == CLASS_SENTRY ||
			ent->client->NPC_class == CLASS_RANCOR ||
			ent->client->NPC_class == CLASS_GHOST || 
			ent->client->NPC_class == CLASS_SLUAGH || 
			ent->client->NPC_class == CLASS_REAPER || 
			ent->client->NPC_class == CLASS_DREADNOUGHT || 
			ent->client->NPC_class == CLASS_MELC || 
			ent->client->NPC_class == CLASS_SUMMONER || 
			ent->client->NPC_class == CLASS_VAMPIRE_FEAR2 || 
			ent->client->NPC_class == CLASS_PROPHET || 
			ent->client->NPC_class == CLASS_POLTER ||
			ent->client->NPC_class == CLASS_NECRO || 
			ent->client->NPC_class == CLASS_GOLEM_NECRO )
	{
		{
			damage *= 1.6;
			G_PlayEffect( G_EffectIndex( "fire_spectral/altmuzzle_flash" ), ent->client->renderInfo.handRPoint );
		}
	}
	// THE GHOST FIRE - BOSSES
	else if ( ent->client->NPC_class == CLASS_GLIDER ||
			ent->client->NPC_class == CLASS_MURJJ )
	{
		{
			damage *= 2.5;
			G_PlayEffect( G_EffectIndex( "fire_spectral/altmuzzle_flash" ), ent->client->renderInfo.handRPoint );
		}
	}
	// THE HYLDEN GLIPHIC FIRE  
	else if ( ent->client->NPC_class == CLASS_TAVION ||
			ent->client->NPC_class == CLASS_DESANN || 
			ent->client->NPC_class == CLASS_HYLDEN || 
			ent->client->NPC_class == CLASS_HYLDENLORD )
	{
		{
			damage *= 3;
			G_PlayEffect( G_EffectIndex( "fire_green/altmuzzle_flash" ), ent->client->renderInfo.handRPoint );
		}
	}
	// THE POISON FIRE   
	else if ( ent->client->NPC_class == CLASS_R2D2 ||
			ent->client->NPC_class == CLASS_R5D2 ||
			ent->client->NPC_class == CLASS_LIZARD ||
			ent->client->NPC_class == CLASS_MINEMONSTER || 
			ent->client->NPC_class == CLASS_POISON || 
			ent->client->NPC_class == CLASS_GOLEM_POISON || 
			ent->client->NPC_class == CLASS_IVY || 
			ent->client->NPC_class == CLASS_BEAST || 
			ent->client->NPC_class == CLASS_BIRD ||
			ent->client->NPC_class == CLASS_SHARK || 
			ent->client->NPC_class == CLASS_DEMON_POISON || 
			ent->client->NPC_class == CLASS_DEMON_BLADE || 
			ent->client->NPC_class == CLASS_DEMON_ELECTRO || 
			ent->client->NPC_class == CLASS_INTERROGATOR )
	{
		{
			damage *= 2;
			G_PlayEffect( G_EffectIndex( "fire_poison/altmuzzle_flash" ), ent->client->renderInfo.handRPoint );
		}
	}
	// DEFAULT FIREBALLS  
	else if ( ent->client->NPC_class == CLASS_BARTENDER ||
			ent->client->NPC_class == CLASS_BESPIN_COP ||
			ent->client->NPC_class == CLASS_COMMANDO || 
			ent->client->NPC_class == CLASS_GALAK || 
			ent->client->NPC_class == CLASS_HOWLER || 
			ent->client->NPC_class == CLASS_IMPWORKER || 
			ent->client->NPC_class == CLASS_WIZARD || 
			ent->client->NPC_class == CLASS_JAN || 
			ent->client->NPC_class == CLASS_JEDI || 
			ent->client->NPC_class == CLASS_LANDO || 
			ent->client->NPC_class == CLASS_LUKE || 
			ent->client->NPC_class == CLASS_MARK1 || 
			ent->client->NPC_class == CLASS_MARK2 || 
			ent->client->NPC_class == CLASS_GALAKMECH || 
			ent->client->NPC_class == CLASS_MORGANKATARN || 
			ent->client->NPC_class == CLASS_PRISONER || 
			ent->client->NPC_class == CLASS_PROTOCOL || 
			ent->client->NPC_class == CLASS_REBEL || 
			ent->client->NPC_class == CLASS_REMOTE || 
			ent->client->NPC_class == CLASS_RODIAN || 
			ent->client->NPC_class == CLASS_SEEKER || 
			ent->client->NPC_class == CLASS_STORMTROOPER || 
			ent->client->NPC_class == CLASS_SWAMP ||
			ent->client->NPC_class == CLASS_TRANDOSHAN || 
			ent->client->NPC_class == CLASS_UGNAUGHT || 
			ent->client->NPC_class == CLASS_WEEQUAY || 
			ent->client->NPC_class == CLASS_ROCKETTROOPER || 
			ent->client->NPC_class == CLASS_SABER_DROID || 
			ent->client->NPC_class == CLASS_ASSASSIN_DROID || 
			ent->client->NPC_class == CLASS_PLAYER || 
			ent->client->NPC_class == CLASS_VEHICLE || 
			ent->client->NPC_class == CLASS_HUNTER || 
			ent->client->NPC_class == CLASS_SCOUT ||  
			ent->client->NPC_class == CLASS_ALCHEMIST || 
			ent->client->NPC_class == CLASS_DRUID || 
			ent->client->NPC_class == CLASS_SARAFAN || 
			ent->client->NPC_class == CLASS_ARCHER ||  
			ent->client->NPC_class == CLASS_WARRIOR || 
			ent->client->NPC_class == CLASS_REAVER || 
			ent->client->NPC_class == CLASS_RAZIEL ||
			ent->client->NPC_class == CLASS_NECROREAVER ||
			ent->client->NPC_class == CLASS_DARKREAVER || 
			ent->client->NPC_class == CLASS_TANKREAVER ||
			ent->client->NPC_class == CLASS_LIGHTREAVER ||
			ent->client->NPC_class == CLASS_DRUIDREAVER ||
			ent->client->NPC_class == CLASS_ICEREAVER || 
			ent->client->NPC_class == CLASS_SPIDERREAVER ||
			ent->client->NPC_class == CLASS_SPIDERREAVER2 ||
			ent->client->NPC_class == CLASS_AIRREAVER || 
			ent->client->NPC_class == CLASS_MENTALIST || 
			ent->client->NPC_class == CLASS_EARTHREAVER || 
			ent->client->NPC_class == CLASS_THRALL || 
			ent->client->NPC_class == CLASS_FIRE ||  
			ent->client->NPC_class == CLASS_WATER || 
			ent->client->NPC_class == CLASS_HOLY ||  
			ent->client->NPC_class == CLASS_SONIC || 
			ent->client->NPC_class == CLASS_GOLEM_FIRE ||
			ent->client->NPC_class == CLASS_GOLEM_WATER ||
			ent->client->NPC_class == CLASS_GOLEM_ICE ||
			ent->client->NPC_class == CLASS_GOLEM_METAL ||
			ent->client->NPC_class == CLASS_GOLEM_SONIC ||
			ent->client->NPC_class == CLASS_GOLEM_HOLY  )
	{
		{
			damage *= 1;
			G_PlayEffect( G_EffectIndex( "blaster/altmuzzle_flash" ), ent->client->renderInfo.handRPoint );
		}
	}
	else if ( ent->client->NPC_class == CLASS_FIGHTER )
	{
		{
			damage *= 1;
			G_PlayEffect( G_EffectIndex( "blaster0/altmuzzle_flash" ), ent->client->renderInfo.handRPoint );
		}
	}
}
else 
{
	// THE BLOOD FLAMES  
	if ( ent->client->NPC_class == CLASS_ALORA || 
			ent->client->NPC_class == CLASS_REBORN ||
			ent->client->NPC_class == CLASS_KYLE ||
			ent->client->NPC_class == CLASS_FLIER2 || 
			ent->client->NPC_class == CLASS_NOGHRI ||
			ent->client->NPC_class == CLASS_SWAMPTROOPER ||
			ent->client->NPC_class == CLASS_FISH ||
			ent->client->NPC_class == CLASS_GRAN ||
			ent->client->NPC_class == CLASS_MONMOTHA || 
			ent->client->NPC_class == CLASS_DECEIVER || 
			ent->client->NPC_class == CLASS_ZEPH || 
			ent->client->NPC_class == CLASS_TURELIM || 
			ent->client->NPC_class == CLASS_RAHABIM || 
			ent->client->NPC_class == CLASS_VAMPIRE ||   
			ent->client->NPC_class == CLASS_KAIN ||
			ent->client->NPC_class == CLASS_KAIN2 ||  
			ent->client->NPC_class == CLASS_KAIN3 ||
			ent->client->NPC_class == CLASS_UMAH || 
			ent->client->NPC_class == CLASS_VORADOR || 
			ent->client->NPC_class == CLASS_JANOS ||  
			ent->client->NPC_class == CLASS_NECROMANCER ||  
			ent->client->NPC_class == CLASS_VAMPIRE_QUEEN ||  
			ent->client->NPC_class == CLASS_VAMPIRE_DARK ||  
			ent->client->NPC_class == CLASS_VAMPIRE_NECRO ||   
			ent->client->NPC_class == CLASS_VAMPIRE_NECRO2 || 
			ent->client->NPC_class == CLASS_VAMPIRE_FEAR ||  
			ent->client->NPC_class == CLASS_VAMPIRE_BLOOD || 
			ent->client->NPC_class == CLASS_VAMPIRE_SOUND || 
			ent->client->NPC_class == CLASS_VAMPIRE_LYCAN ||  
			ent->client->NPC_class == CLASS_VAMPIRE_ELF ||  
			ent->client->NPC_class == CLASS_VAMPIRE_ELF2 ||  
			ent->client->NPC_class == CLASS_VAMPIRE_ELF3 || 
			ent->client->NPC_class == CLASS_VAMPIRE_WATER ||  
			ent->client->NPC_class == CLASS_VAMPIRE_FROST || 
			ent->client->NPC_class == CLASS_VAMPIRE_ARCHER || 
			ent->client->NPC_class == CLASS_HALFVAMPIRE ||
			ent->client->NPC_class == CLASS_RAZIELHIM ||  
			ent->client->NPC_class == CLASS_ESKANDOR ||   
			ent->client->NPC_class == CLASS_VAMPIRE_BOUNTY || 
			ent->client->NPC_class == CLASS_VAMPIRE_SARAFAN ||
			ent->client->NPC_class == CLASS_GOLEM_DARK || 
			ent->client->NPC_class == CLASS_DARK || 
			ent->client->NPC_class == CLASS_VOID || 
			ent->client->NPC_class == CLASS_GOLEM_VOID )
	{
		{
			damage *= 1.25;
			G_PlayEffect( G_EffectIndex( "fire_blood/muzzle_flash" ), ent->client->renderInfo.handRPoint );
		}
	}
	// THE WARM BALLS  
	else if ( ent->client->NPC_class == CLASS_PROBE || 
			ent->client->NPC_class == CLASS_HAZARD_TROOPER ||
			ent->client->NPC_class == CLASS_SABOTEUR || 
			ent->client->NPC_class == CLASS_SHADOWTROOPER || 
			ent->client->NPC_class == CLASS_IMPERIAL ||
			ent->client->NPC_class == CLASS_WAMPA ||
			ent->client->NPC_class == CLASS_BOBAFETT ||
			ent->client->NPC_class == CLASS_DEMON_FIRE || 
			ent->client->NPC_class == CLASS_HELLGOLEM || 
			ent->client->NPC_class == CLASS_VAMPIRE_DEMON || 
			ent->client->NPC_class == CLASS_FIREREAVER ||
			ent->client->NPC_class == CLASS_GOLEM_SMOKE || 
			ent->client->NPC_class == CLASS_AIR || 			
			ent->client->NPC_class == CLASS_GOLEM_AIR ||
			ent->client->NPC_class == CLASS_LIGHT || 
			ent->client->NPC_class == CLASS_GOLEM_LIGHT ||
			ent->client->NPC_class == CLASS_GOLEM_GOLD ||
			ent->client->NPC_class == CLASS_LYCAN )
	{
		{
			damage *= 1.5;
			G_PlayEffect( G_EffectIndex( "blaster2/muzzle_flash" ), ent->client->renderInfo.handRPoint );
		}
	}
	// THE MAGMA BALLS  
	else if ( ent->client->NPC_class == CLASS_CLAW ||
			ent->client->NPC_class == CLASS_EARTH || 
			ent->client->NPC_class == CLASS_GOLEM || 
			ent->client->NPC_class == CLASS_GOLEM_LAVA || 
			ent->client->NPC_class == CLASS_REELO || 
			ent->client->NPC_class == CLASS_SPIDERREAVER3 || 
			ent->client->NPC_class == CLASS_GONK || 
			ent->client->NPC_class == CLASS_CRYSTAL ||
			ent->client->NPC_class == CLASS_VAMPIRE_GHOST )
	{
		{
			damage *= 2;
			G_PlayEffect( G_EffectIndex( "blaster3/muzzle_flash" ), ent->client->renderInfo.handRPoint );
		}
	}
	// THE HELL FIRE - BOSSES 
	else if ( ent->client->NPC_class == CLASS_MOUSE || 
			ent->client->NPC_class == CLASS_ATST ||
			ent->client->NPC_class == CLASS_SAND_CREATURE || 
			ent->client->NPC_class == CLASS_ELDERGOD || 
			ent->client->NPC_class == CLASS_TENTACLE || 
			ent->client->NPC_class == CLASS_CENOBITA ) 
	{
		{
			damage *= 4;
			G_PlayEffect( G_EffectIndex( "fire_hell/muzzle_flash" ), ent->client->renderInfo.handRPoint );
		}
	}
	// THE GHOST FIRE 
	else if ( ent->client->NPC_class == CLASS_TUSKEN ||
			ent->client->NPC_class == CLASS_VAMPIRE_ZOMBIE || 
			ent->client->NPC_class == CLASS_JAWA ||
			ent->client->NPC_class == CLASS_SENTRY ||
			ent->client->NPC_class == CLASS_RANCOR ||
			ent->client->NPC_class == CLASS_GHOST || 
			ent->client->NPC_class == CLASS_SLUAGH || 
			ent->client->NPC_class == CLASS_REAPER || 
			ent->client->NPC_class == CLASS_DREADNOUGHT || 
			ent->client->NPC_class == CLASS_MELC || 
			ent->client->NPC_class == CLASS_SUMMONER || 
			ent->client->NPC_class == CLASS_VAMPIRE_FEAR2 || 
			ent->client->NPC_class == CLASS_PROPHET || 
			ent->client->NPC_class == CLASS_POLTER ||
			ent->client->NPC_class == CLASS_NECRO || 
			ent->client->NPC_class == CLASS_GOLEM_NECRO )
	{
		{
			damage *= 1.6;
			G_PlayEffect( G_EffectIndex( "fire_spectral/muzzle_flash" ), ent->client->renderInfo.handRPoint );
		}
	}
	// THE GHOST FIRE - BOSSES
	else if ( ent->client->NPC_class == CLASS_GLIDER || ent->client->NPC_class == CLASS_MURJJ )
	{
		{
			damage *= 2.5;
			G_PlayEffect( G_EffectIndex( "fire_spectral/muzzle_flash" ), ent->client->renderInfo.handRPoint );
		}
	}
	// THE HYLDEN GLIPHIC FIRE  
	else if (  ent->client->NPC_class == CLASS_TAVION ||
			ent->client->NPC_class == CLASS_DESANN || 
			ent->client->NPC_class == CLASS_HYLDEN || 
			ent->client->NPC_class == CLASS_HYLDENLORD  )
	{
		{
			damage *= 3;
			G_PlayEffect( G_EffectIndex( "fire_green/muzzle_flash" ), ent->client->renderInfo.handRPoint );
		}
	}
	// THE POISON FIRE   
	else if ( ent->client->NPC_class == CLASS_R2D2 ||
			ent->client->NPC_class == CLASS_R5D2 ||
			ent->client->NPC_class == CLASS_LIZARD ||
			ent->client->NPC_class == CLASS_MINEMONSTER || 
			ent->client->NPC_class == CLASS_POISON || 
			ent->client->NPC_class == CLASS_GOLEM_POISON || 
			ent->client->NPC_class == CLASS_IVY || 
			ent->client->NPC_class == CLASS_BEAST || 
			ent->client->NPC_class == CLASS_BIRD ||
			ent->client->NPC_class == CLASS_SHARK || 
			ent->client->NPC_class == CLASS_DEMON_POISON || 
			ent->client->NPC_class == CLASS_DEMON_BLADE || 
			ent->client->NPC_class == CLASS_DEMON_ELECTRO || 
			ent->client->NPC_class == CLASS_INTERROGATOR )
	{
		{
			damage *= 2;
			G_PlayEffect( G_EffectIndex( "fire_poison/muzzle_flash" ), ent->client->renderInfo.handRPoint );
		}
	}
	// DEFAULT FIREBALLS  
	else if (ent->client->NPC_class == CLASS_BARTENDER ||
			ent->client->NPC_class == CLASS_BESPIN_COP ||
			ent->client->NPC_class == CLASS_COMMANDO || 
			ent->client->NPC_class == CLASS_GALAK || 
			ent->client->NPC_class == CLASS_HOWLER || 
			ent->client->NPC_class == CLASS_IMPWORKER || 
			ent->client->NPC_class == CLASS_WIZARD || 
			ent->client->NPC_class == CLASS_JAN || 
			ent->client->NPC_class == CLASS_JEDI || 
			ent->client->NPC_class == CLASS_LANDO || 
			ent->client->NPC_class == CLASS_LUKE || 
			ent->client->NPC_class == CLASS_MARK1 || 
			ent->client->NPC_class == CLASS_MARK2 || 
			ent->client->NPC_class == CLASS_GALAKMECH || 
			ent->client->NPC_class == CLASS_MORGANKATARN || 
			ent->client->NPC_class == CLASS_PRISONER || 
			ent->client->NPC_class == CLASS_PROTOCOL || 
			ent->client->NPC_class == CLASS_REBEL || 
			ent->client->NPC_class == CLASS_REMOTE || 
			ent->client->NPC_class == CLASS_RODIAN || 
			ent->client->NPC_class == CLASS_SEEKER || 
			ent->client->NPC_class == CLASS_STORMTROOPER || 
			ent->client->NPC_class == CLASS_SWAMP ||
			ent->client->NPC_class == CLASS_TRANDOSHAN || 
			ent->client->NPC_class == CLASS_UGNAUGHT || 
			ent->client->NPC_class == CLASS_WEEQUAY || 
			ent->client->NPC_class == CLASS_ROCKETTROOPER || 
			ent->client->NPC_class == CLASS_SABER_DROID || 
			ent->client->NPC_class == CLASS_ASSASSIN_DROID || 
			ent->client->NPC_class == CLASS_PLAYER || 
			ent->client->NPC_class == CLASS_VEHICLE || 
			ent->client->NPC_class == CLASS_HUNTER || 
			ent->client->NPC_class == CLASS_SCOUT ||  
			ent->client->NPC_class == CLASS_ALCHEMIST || 
			ent->client->NPC_class == CLASS_DRUID || 
			ent->client->NPC_class == CLASS_SARAFAN || 
			ent->client->NPC_class == CLASS_ARCHER ||  
			ent->client->NPC_class == CLASS_WARRIOR || 
			ent->client->NPC_class == CLASS_REAVER || 
			ent->client->NPC_class == CLASS_RAZIEL ||
			ent->client->NPC_class == CLASS_NECROREAVER ||
			ent->client->NPC_class == CLASS_DARKREAVER || 
			ent->client->NPC_class == CLASS_TANKREAVER ||
			ent->client->NPC_class == CLASS_LIGHTREAVER ||
			ent->client->NPC_class == CLASS_DRUIDREAVER ||
			ent->client->NPC_class == CLASS_ICEREAVER || 
			ent->client->NPC_class == CLASS_SPIDERREAVER ||
			ent->client->NPC_class == CLASS_SPIDERREAVER2 ||
			ent->client->NPC_class == CLASS_AIRREAVER || 
			ent->client->NPC_class == CLASS_MENTALIST || 
			ent->client->NPC_class == CLASS_EARTHREAVER || 
			ent->client->NPC_class == CLASS_THRALL || 
			ent->client->NPC_class == CLASS_FIRE ||  
			ent->client->NPC_class == CLASS_WATER || 
			ent->client->NPC_class == CLASS_HOLY ||  
			ent->client->NPC_class == CLASS_SONIC || 
			ent->client->NPC_class == CLASS_GOLEM_FIRE ||
			ent->client->NPC_class == CLASS_GOLEM_WATER ||
			ent->client->NPC_class == CLASS_GOLEM_ICE ||
			ent->client->NPC_class == CLASS_GOLEM_METAL ||
			ent->client->NPC_class == CLASS_GOLEM_SONIC ||
			ent->client->NPC_class == CLASS_GOLEM_HOLY )
	{
		{
			damage *= 1;
			G_PlayEffect( G_EffectIndex( "blaster/muzzle_flash" ), ent->client->renderInfo.handRPoint );
		}
	}
	else if ( ent->client->NPC_class == CLASS_FIGHTER )
	{
		{
			damage *= 1;
			G_PlayEffect( G_EffectIndex( "blaster0/muzzle_flash" ), ent->client->renderInfo.handRPoint );
		}
	}
}

//	if ( ent->client )
//	{
//		if ( ent->client->ps.powerups[PW_WEAPON_OVERCHARGE] > 0 && ent->client->ps.powerups[PW_WEAPON_OVERCHARGE] > cg.time )
//		{
//			// in overcharge mode, so doing double damage
//			missile->flags |= FL_OVERCHARGED;
//			damage *= 2;
//		}
//	}

	missile->damage = damage;
	missile->dflags = DAMAGE_DEATH_KNOCKBACK;
	if ( altFire )
	{
		missile->methodOfDeath = MOD_BLASTER_ALT;
	}
	else
	{
		missile->methodOfDeath = MOD_BLASTER;
	}
	missile->clipmask = MASK_SHOT | CONTENTS_LIGHTSABER;

	// we don't want it to bounce forever
	 missile->bounceCount = 0;
}

//---------------------------------------------------------
void WP_FireBlaster( gentity_t *ent, qboolean alt_fire )
//---------------------------------------------------------
{
	vec3_t	dir, angs;

	vectoangles( forwardVec, angs );

	if ( ent->client && ent->client->NPC_class == CLASS_VEHICLE )
	{//no inherent aim screw up
	}
	else if ( !(ent->client->ps.forcePowersActive&(1<<FP_SEE))
		|| ent->client->ps.forcePowerLevel[FP_SEE] < FORCE_LEVEL_2 )
	{//force sight 2+ gives perfect aim
		//FIXME: maybe force sight level 3 autoaims some?
		if ( alt_fire )
		{
			// add some slop to the alt-fire direction
			angs[PITCH] += crandom() * BLASTER_ALT_SPREAD;
			angs[YAW]	+= crandom() * BLASTER_ALT_SPREAD;
		}
		else
		{
			// Troopers use their aim values as well as the gun's inherent inaccuracy
			// so check for all classes of stormtroopers and anyone else that has aim error
			if ( ent->client && ent->NPC &&
				( ent->client->NPC_class == CLASS_GRAN ||
				ent->client->NPC_class == CLASS_HAZARD_TROOPER ) )
			{
				angs[PITCH] += ( crandom() * (BLASTER_NPC_SPREAD+(6-ent->NPC->currentAim)*0.25f));//was 0.5f
				angs[YAW]	+= ( crandom() * (BLASTER_NPC_SPREAD+(6-ent->NPC->currentAim)*0.25f));//was 0.5f
			}
			else
			{
				// add some slop to the main-fire direction
				angs[PITCH] += crandom() * BLASTER_MAIN_SPREAD;
				angs[YAW]	+= crandom() * BLASTER_MAIN_SPREAD;
			}
		}
	}

	AngleVectors( angs, dir, NULL, NULL );

	// FIXME: if temp_org does not have clear trace to inside the bbox, don't shoot!
	WP_FireBlasterMissile( ent, muzzle, dir, alt_fire );
}