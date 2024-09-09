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

//-----------------------
//	Golan Arms Flechette
//-----------------------

//---------------------------------------------------------
static void WP_FlechetteMainFire( gentity_t *ent )
//---------------------------------------------------------
{
	vec3_t		fwd, angs, start;
	gentity_t	*missile;
	float		damage = weaponData[WP_FLECHETTE].damage, vel = FLECHETTE_VEL;

	if ( ent && ent->client && ent->client->NPC_class == CLASS_UGNAUGHT
		)
	{
		damage *= 1.5; // Elementali del Fango fanno 50% di danno in più!
	}
	else if ( ent && ent->client && ent->client->NPC_class == CLASS_LIZARD )
	{
		damage *= 2; // Elementali di Pietra fanno 50% di danno doppio!!
	}

				
				// DAMAGE EARTH AND SOLID PROJECTILES -  STASIS 	
	if ( ent->client->NPC_class == CLASS_ZEPH ||
				ent->client->NPC_class == CLASS_DECEIVER ||
				ent->client->NPC_class == CLASS_DUMAHIM || 
				ent->client->NPC_class == CLASS_RAHABIM ||
				ent->client->NPC_class == CLASS_VAMPIRE ||
				ent->client->NPC_class == CLASS_VAMPIRE_DEMON ||
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
				ent->client->NPC_class == CLASS_VAMPIRE_FEAR2 ||  
				ent->client->NPC_class == CLASS_VAMPIRE_BLOOD ||
				ent->client->NPC_class == CLASS_VAMPIRE_SOUND || 
				ent->client->NPC_class == CLASS_VAMPIRE_LYCAN || 
				ent->client->NPC_class == CLASS_VAMPIRE_WATER || 
				ent->client->NPC_class == CLASS_VAMPIRE_FROST ||  
				ent->client->NPC_class == CLASS_VAMPIRE_ARCHER || 
				ent->client->NPC_class == CLASS_HALFVAMPIRE || 
				ent->client->NPC_class == CLASS_RAZIELHIM ||  
				ent->client->NPC_class == CLASS_ESKANDOR ||  
				ent->client->NPC_class == CLASS_VAMPIRE_BOUNTY || 
				ent->client->NPC_class == CLASS_VAMPIRE_ZOMBIE || 
				ent->client->NPC_class == CLASS_VAMPIRE_SARAFAN ||  
				ent->client->NPC_class == CLASS_REBORN || 
				ent->client->NPC_class == CLASS_KYLE ||
				ent->client->NPC_class == CLASS_ALORA ||
				ent->client->NPC_class == CLASS_NOGHRI ||
				ent->client->NPC_class == CLASS_FLIER2 ||
				ent->client->NPC_class == CLASS_SWAMPTROOPER || 
				ent->client->NPC_class == CLASS_GRAN ||
				ent->client->NPC_class == CLASS_JAN || 
				ent->client->NPC_class == CLASS_GALAK || 
				ent->client->NPC_class == CLASS_MARK1 ||
				ent->client->NPC_class == CLASS_MARK2 ||
				ent->client->NPC_class == CLASS_SARAFAN || 
				ent->client->NPC_class == CLASS_ARCHER ||  
				ent->client->NPC_class == CLASS_WARRIOR || 
				ent->client->NPC_class == CLASS_HUNTER || 
				ent->client->NPC_class == CLASS_SCOUT ||  
				ent->client->NPC_class == CLASS_ALCHEMIST )
	{
		damage *= 0.75;
	}
	// ACID  
	else if ( ent->client->NPC_class == CLASS_INTERROGATOR || 
				ent->client->NPC_class == CLASS_MINEMONSTER ||
				ent->client->NPC_class == CLASS_POISON || 
				ent->client->NPC_class == CLASS_BEAST || 
				ent->client->NPC_class == CLASS_GOLEM_POISON ||
				ent->client->NPC_class == CLASS_DEMON_POISON || 
				ent->client->NPC_class == CLASS_R2D2 ||
				ent->client->NPC_class == CLASS_R5D2 )
 	{
		damage *= 1.75;
	}
	// MUD  
	else if ( ent->client->NPC_class == CLASS_UGNAUGHT ||
				ent->client->NPC_class == CLASS_WATER || 
				ent->client->NPC_class == CLASS_GOLEM_WATER || 
				ent->client->NPC_class == CLASS_SHARK || 
				ent->client->NPC_class == CLASS_REELO || 
				ent->client->NPC_class == CLASS_LYCAN ||
				ent->client->NPC_class == CLASS_SPIDERREAVER3 )
	{
		damage *= 1.5;
	}
	// STONE  
	else if ( ent->client->NPC_class == CLASS_EARTHREAVER || 
				ent->client->NPC_class == CLASS_LIZARD ||
				ent->client->NPC_class == CLASS_DEMON_BLADE )
	{
		damage *= 2;
	}
	// ASH
	else if ( ent->client->NPC_class == CLASS_IMPERIAL || 
				ent->client->NPC_class == CLASS_BESPIN_COP ||
				ent->client->NPC_class == CLASS_HAZARD_TROOPER ||
				ent->client->NPC_class == CLASS_CLAW || 
				ent->client->NPC_class == CLASS_WAMPA || 
				ent->client->NPC_class == CLASS_TURELIM || 
				ent->client->NPC_class == CLASS_FIREREAVER || 
				ent->client->NPC_class == CLASS_TANKREAVER || 
				ent->client->NPC_class == CLASS_FIRE || 
				ent->client->NPC_class == CLASS_GOLEM_FIRE || 
				ent->client->NPC_class == CLASS_HELLGOLEM || 
				ent->client->NPC_class == CLASS_GOLEM_LAVA || 
				ent->client->NPC_class == CLASS_DEMON_FIRE || 
				ent->client->NPC_class == CLASS_GOLEM_SMOKE )
	{
		damage *= 1.4;
	}
	// GREEN CRYSTAL 
	else if ( ent->client->NPC_class == CLASS_GALAKMECH || 
				ent->client->NPC_class == CLASS_SABER_DROID ||
				ent->client->NPC_class == CLASS_PROTOCOL || 
				ent->client->NPC_class == CLASS_CRYSTAL || 
				ent->client->NPC_class == CLASS_LIGHT || 
				ent->client->NPC_class == CLASS_GOLEM_LIGHT || 
				ent->client->NPC_class == CLASS_GOLEM_GOLD || 
				ent->client->NPC_class == CLASS_GOLEM_ICE ||
				ent->client->NPC_class == CLASS_GOLEM_METAL ||
				ent->client->NPC_class == CLASS_DEMON_ELECTRO )
	{
		damage *= 2.5;
	}
	// DUST  
	else if ( ent->client->NPC_class == CLASS_SABOTEUR || 
				ent->client->NPC_class == CLASS_DARK || 
				ent->client->NPC_class == CLASS_GOLEM_DARK || 
				ent->client->NPC_class == CLASS_VOID || 
				ent->client->NPC_class == CLASS_GOLEM_VOID || 
				ent->client->NPC_class == CLASS_DARKREAVER || 
				ent->client->NPC_class == CLASS_SHADOWTROOPER ||
				ent->client->NPC_class == CLASS_PROBE ||
				ent->client->NPC_class == CLASS_POLTER || 
				ent->client->NPC_class == CLASS_AIR || 
				ent->client->NPC_class == CLASS_GOLEM_AIR || 
				ent->client->NPC_class == CLASS_ROCKETTROOPER || 
				ent->client->NPC_class == CLASS_BOBAFETT || 
				ent->client->NPC_class == CLASS_SPIDERREAVER2 || 
				ent->client->NPC_class == CLASS_THRALL || 
				ent->client->NPC_class == CLASS_BIRD )
	{
		damage *= 1.25;
	}
	// SPECTRAL 
	else if ( ent->client->NPC_class == CLASS_TUSKEN || 
				ent->client->NPC_class == CLASS_MELC || 
				ent->client->NPC_class == CLASS_SUMMONER || 
				ent->client->NPC_class == CLASS_SLUAGH || 
				ent->client->NPC_class == CLASS_DREADNOUGHT || 
				ent->client->NPC_class == CLASS_REAPER || 
				ent->client->NPC_class == CLASS_RANCOR ||
				ent->client->NPC_class == CLASS_JAWA || 
				ent->client->NPC_class == CLASS_GHOST || 				
				ent->client->NPC_class == CLASS_NECRO || 
				ent->client->NPC_class == CLASS_GOLEM_NECRO )
	{
		damage *= 0.5;
	}
	// SPECTRAL BOSSES  
	else if (  	ent->client->NPC_class == CLASS_GLIDER || ent->client->NPC_class == CLASS_ARIEL ||
			ent->client->NPC_class == CLASS_FISH || ent->client->NPC_class == CLASS_VAMPIRE_GHOST ||
			ent->client->NPC_class == CLASS_ELDERGOD || 	ent->client->NPC_class == CLASS_TENTACLE || 
			ent->client->NPC_class == CLASS_ATST  || ent->client->NPC_class == CLASS_SAND_CREATURE || 
			ent->client->NPC_class == CLASS_MOUSE )
	{
		damage *= 2.25;
	}
	// THORN 
	else if ( ent->client->NPC_class == CLASS_SWAMP )
	{
		damage *= 0.75;
	}
	// IVY
	else if ( ent->client->NPC_class == CLASS_PRISONER || 
				ent->client->NPC_class == CLASS_SENTRY ||
				ent->client->NPC_class == CLASS_IVY ) 
	{
		damage *= 0.75;
	}
	// HELLISH IVY
	else if ( ent->client->NPC_class == CLASS_CENOBITA || 
				ent->client->NPC_class == CLASS_PROPHET || 
				ent->client->NPC_class == CLASS_TAVION || 
				ent->client->NPC_class == CLASS_DESANN ||
				ent->client->NPC_class == CLASS_HYLDEN || 
				ent->client->NPC_class == CLASS_HYLDENLORD ) 
	{
		damage *= 1.8;
	}
	// DEFAULT EARTH SHOOT   
	else if ( ent->client->NPC_class == CLASS_DRUID || 
				ent->client->NPC_class == CLASS_VAMPIRE_ELF || 
				ent->client->NPC_class == CLASS_VAMPIRE_ELF2 ||
				ent->client->NPC_class == CLASS_VAMPIRE_ELF3 ||
				ent->client->NPC_class == CLASS_REAVER || 
				ent->client->NPC_class == CLASS_RAZIEL ||
				ent->client->NPC_class == CLASS_NECROREAVER ||
				ent->client->NPC_class == CLASS_LIGHTREAVER ||
				ent->client->NPC_class == CLASS_DRUIDREAVER ||
				ent->client->NPC_class == CLASS_ICEREAVER || 
				ent->client->NPC_class == CLASS_SPIDERREAVER ||
				ent->client->NPC_class == CLASS_AIRREAVER || 
				ent->client->NPC_class == CLASS_MENTALIST || 
				ent->client->NPC_class == CLASS_HOLY ||  
				ent->client->NPC_class == CLASS_SONIC || 
				ent->client->NPC_class == CLASS_GOLEM ||
				ent->client->NPC_class == CLASS_GOLEM_SONIC ||
				ent->client->NPC_class == CLASS_GOLEM_HOLY || 				 
				ent->client->NPC_class == CLASS_COMMANDO || 
				ent->client->NPC_class == CLASS_STORMTROOPER || 
				ent->client->NPC_class == CLASS_GONK ||
				ent->client->NPC_class == CLASS_HOWLER || 
				ent->client->NPC_class == CLASS_IMPWORKER || 
				ent->client->NPC_class == CLASS_WIZARD || 
				ent->client->NPC_class == CLASS_JEDI || 
				ent->client->NPC_class == CLASS_LANDO || 
				ent->client->NPC_class == CLASS_WEEQUAY || 
				ent->client->NPC_class == CLASS_LUKE || 
				ent->client->NPC_class == CLASS_MONMOTHA || 
				ent->client->NPC_class == CLASS_MORGANKATARN || 
				ent->client->NPC_class == CLASS_MURJJ || 
				ent->client->NPC_class == CLASS_REBEL || 
				ent->client->NPC_class == CLASS_REMOTE || 
				ent->client->NPC_class == CLASS_RODIAN || 
				ent->client->NPC_class == CLASS_SEEKER || 
				ent->client->NPC_class == CLASS_TRANDOSHAN || 
				ent->client->NPC_class == CLASS_BARTENDER || 
				ent->client->NPC_class == CLASS_ASSASSIN_DROID || 
				ent->client->NPC_class == CLASS_PLAYER || 
				ent->client->NPC_class == CLASS_VEHICLE )				
	{
		damage *= 1;
	}

	VectorCopy( muzzle, start );
	WP_TraceSetStart( ent, start, vec3_origin, vec3_origin );//make sure our start point isn't on the other side of a wall

	// If we aren't the player, we will cut the velocity and damage of the shots
	if ( ent->s.number )
	{
		damage *= 0.75f;
		vel *= 0.5f;
	}

//	if ( ent->client && ent->client->ps.powerups[PW_WEAPON_OVERCHARGE] > 0 && ent->client->ps.powerups[PW_WEAPON_OVERCHARGE] > cg.time )
//	{
//		// in overcharge mode, so doing double damage
//		damage *= 2;
//	}

	for ( int i = 0; i < FLECHETTE_SHOTS; i++ )
	{
		vectoangles( forwardVec, angs );

		if ( i == 0 && ent->s.number == 0 )
		{
			// do nothing on the first shot for the player, this one will hit the crosshairs
		}
		else
		{
			angs[PITCH] += crandom() * FLECHETTE_SPREAD;
			angs[YAW]	+= crandom() * FLECHETTE_SPREAD;
		}

		AngleVectors( angs, fwd, NULL, NULL );

		WP_MissileTargetHint(ent, start, fwd);

		missile = CreateMissile( start, fwd, vel, 10000, ent );

		missile->classname = "flech_proj";
		missile->s.weapon = WP_FLECHETTE;

		VectorSet( missile->maxs, FLECHETTE_SIZE, FLECHETTE_SIZE, FLECHETTE_SIZE );
		VectorScale( missile->maxs, -1, missile->mins );

		missile->damage = damage;

//		if ( ent->client && ent->client->ps.powerups[PW_WEAPON_OVERCHARGE] > 0 && ent->client->ps.powerups[PW_WEAPON_OVERCHARGE] > cg.time )
//		{
//			missile->flags |= FL_OVERCHARGED;
//		}
			
		missile->dflags = (DAMAGE_DEATH_KNOCKBACK|DAMAGE_EXTRA_KNOCKBACK);
		
		missile->methodOfDeath = MOD_FLECHETTE;
		missile->clipmask = MASK_SHOT | CONTENTS_LIGHTSABER;

		// we don't want it to bounce forever
		missile->bounceCount = 0; // Q_irand(1,2);

		// missile->s.eFlags |= EF_BOUNCE_SHRAPNEL;
		ent->client->sess.missionStats.shotsFired++;
	}
}

//---------------------------------------------------------
void prox_mine_think( gentity_t *ent )
//---------------------------------------------------------
{
	int			count;
	qboolean	blow = qfalse;

	// if it isn't time to auto-explode, do a small proximity check
	if ( ent->delay > level.time )
	{
		count = G_RadiusList( ent->currentOrigin, FLECHETTE_MINE_RADIUS_CHECK, ent, qtrue, ent_list );

		for ( int i = 0; i < count; i++ )
		{
			if ( ent_list[i]->client && ent_list[i]->health > 0 && ent->activator && ent_list[i]->s.number != ent->activator->s.number )
			{
				blow = qtrue;
				break;
			}
		}
	}
	else
	{
		// well, we must die now
		blow = qtrue;
	}

	if ( blow )
	{
//		G_Sound( ent, G_SoundIndex( "sound/weapons/flechette/warning.wav" ));
		ent->e_ThinkFunc = thinkF_WP_Explode;
		ent->nextthink = level.time + 200;
	}
	else
	{
		// we probably don't need to do this thinking logic very often...maybe this is fast enough?
		ent->nextthink = level.time + 500;
	}
}

//---------------------------------------------------------
void prox_mine_stick( gentity_t *self, gentity_t *other, trace_t *trace )
//---------------------------------------------------------
{
	// turn us into a generic entity so we aren't running missile code
	self->s.eType = ET_GENERAL;

	self->s.modelindex = G_ModelIndex("models/weapons2/golan_arms/prox_mine.md3");
	self->e_TouchFunc = touchF_NULL;

	self->contents = CONTENTS_SOLID;
	self->takedamage = qtrue;
	self->health = 5;
	self->e_DieFunc = dieF_WP_ExplosiveDie;

	VectorSet( self->maxs, 5, 5, 5 );
	VectorScale( self->maxs, -1, self->mins );

	self->activator = self->owner;
	self->owner = NULL;

	WP_Stick( self, trace );
	
	self->e_ThinkFunc = thinkF_prox_mine_think;
	self->nextthink = level.time + 450;

	// sticks for twenty seconds, then auto blows.
	self->delay = level.time + 20000;

	gi.linkentity( self );
}
/* Old Flechette alt-fire code....
//---------------------------------------------------------
static void WP_FlechetteProxMine( gentity_t *ent )
//---------------------------------------------------------
{
	gentity_t	*missile = CreateMissile( muzzle, forwardVec, FLECHETTE_MINE_VEL, 10000, ent, qtrue );

	missile->fxID = G_EffectIndex( "flechette/explosion" );

	missile->classname = "proxMine";
	missile->s.weapon = WP_FLECHETTE;

	missile->s.pos.trType = TR_GRAVITY;

	missile->s.eFlags |= EF_MISSILE_STICK;
	missile->e_TouchFunc = touchF_prox_mine_stick;

	missile->damage = FLECHETTE_MINE_DAMAGE;
	missile->methodOfDeath = MOD_EXPLOSIVE;

	missile->splashDamage = FLECHETTE_MINE_SPLASH_DAMAGE;
	missile->splashRadius = FLECHETTE_MINE_SPLASH_RADIUS;
	missile->splashMethodOfDeath = MOD_EXPLOSIVE_SPLASH;

	missile->clipmask = MASK_SHOT;

	// we don't want it to bounce forever
	missile->bounceCount = 0; 
}
*/
//----------------------------------------------
void WP_flechette_alt_blow( gentity_t *ent )
//----------------------------------------------
{
	EvaluateTrajectory( &ent->s.pos, level.time, ent->currentOrigin ); // Not sure if this is even necessary, but correct origins are cool?

	G_RadiusDamage( ent->currentOrigin, ent->owner, ent->splashDamage, ent->splashRadius, NULL, MOD_FLECHETTE_ALT );

	G_PlayEffect( "flechette/alt_blow", ent->currentOrigin );

	G_FreeEntity( ent );
}

//------------------------------------------------------------------------------
static void WP_CreateFlechetteBouncyThing( vec3_t start, vec3_t fwd, gentity_t *self )
//------------------------------------------------------------------------------
{
	gentity_t	*missile = CreateMissile( start, fwd, 950 + random() * 700, 1500 + random() * 2000, self, qtrue );
	
	missile->e_ThinkFunc = thinkF_WP_flechette_alt_blow;

	missile->s.weapon = WP_FLECHETTE;
	missile->classname = "flech_alt";
	missile->mass = 4;

	// How 'bout we give this thing a size...
	VectorSet( missile->mins, -3.0f, -3.0f, -3.0f );
	VectorSet( missile->maxs, 3.0f, 3.0f, 3.0f );
	missile->clipmask = MASK_SHOT;
	missile->clipmask &= ~CONTENTS_CORPSE;

	// normal ones bounce, alt ones explode on impact
	missile->s.pos.trType = TR_GRAVITY;

	// missile->s.eFlags |= EF_BOUNCE_HALF;

	missile->damage = weaponData[WP_FLECHETTE].altDamage;
	missile->dflags = 0;
	missile->splashDamage = weaponData[WP_FLECHETTE].altSplashDamage;
	missile->splashRadius = weaponData[WP_FLECHETTE].altSplashRadius;

	missile->svFlags = SVF_USE_CURRENT_ORIGIN;

	missile->methodOfDeath = MOD_FLECHETTE_ALT;
	missile->splashMethodOfDeath = MOD_FLECHETTE_ALT;

	VectorCopy( start, missile->pos2 );
}

//---------------------------------------------------------
static void WP_FlechetteAltFire( gentity_t *self )
//---------------------------------------------------------
{
	vec3_t 	dir, fwd, start, angs;

	vectoangles( forwardVec, angs );
	VectorCopy( muzzle, start );

	WP_TraceSetStart( self, start, vec3_origin, vec3_origin );//make sure our start point isn't on the other side of a wall

	for ( int i = 0; i < 2; i++ )
	{
		VectorCopy( angs, dir );

		dir[PITCH] -= random() * 4 + 8; // make it fly upwards
		dir[YAW] += crandom() * 2;
		AngleVectors( dir, fwd, NULL, NULL );

		WP_CreateFlechetteBouncyThing( start, fwd, self );
		self->client->sess.missionStats.shotsFired++;
	}
}

//---------------------------------------------------------
void WP_FireFlechette( gentity_t *ent, qboolean alt_fire )
//---------------------------------------------------------
{
	if ( alt_fire )
	{
		WP_FlechetteAltFire( ent );
	}
	else
	{
		WP_FlechetteMainFire( ent );
	}

	if ( alt_fire )
{
	// STASIS
	if ( ent->client->NPC_class == CLASS_ZEPH ||
				ent->client->NPC_class == CLASS_DECEIVER ||
				ent->client->NPC_class == CLASS_DUMAHIM || 
				ent->client->NPC_class == CLASS_RAHABIM ||
				ent->client->NPC_class == CLASS_VAMPIRE ||
				ent->client->NPC_class == CLASS_VAMPIRE_DEMON ||
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
				ent->client->NPC_class == CLASS_VAMPIRE_FEAR2 ||  
				ent->client->NPC_class == CLASS_VAMPIRE_BLOOD ||
				ent->client->NPC_class == CLASS_VAMPIRE_SOUND || 
				ent->client->NPC_class == CLASS_VAMPIRE_LYCAN || 
				ent->client->NPC_class == CLASS_VAMPIRE_WATER || 
				ent->client->NPC_class == CLASS_VAMPIRE_FROST ||  
				ent->client->NPC_class == CLASS_VAMPIRE_ARCHER || 
				ent->client->NPC_class == CLASS_HALFVAMPIRE || 
				ent->client->NPC_class == CLASS_RAZIELHIM ||  
				ent->client->NPC_class == CLASS_ESKANDOR ||  
				ent->client->NPC_class == CLASS_VAMPIRE_BOUNTY || 
				ent->client->NPC_class == CLASS_VAMPIRE_ZOMBIE || 
				ent->client->NPC_class == CLASS_VAMPIRE_SARAFAN ||  
				ent->client->NPC_class == CLASS_REBORN || 
				ent->client->NPC_class == CLASS_KYLE ||
				ent->client->NPC_class == CLASS_ALORA ||
				ent->client->NPC_class == CLASS_NOGHRI ||
				ent->client->NPC_class == CLASS_FLIER2 ||
				ent->client->NPC_class == CLASS_SWAMPTROOPER || 
				ent->client->NPC_class == CLASS_GRAN ||
				ent->client->NPC_class == CLASS_JAN || 
				ent->client->NPC_class == CLASS_GALAK || 
				ent->client->NPC_class == CLASS_MARK1 ||
				ent->client->NPC_class == CLASS_MARK2 ||
				ent->client->NPC_class == CLASS_SARAFAN || 
				ent->client->NPC_class == CLASS_ARCHER ||  
				ent->client->NPC_class == CLASS_WARRIOR || 
				ent->client->NPC_class == CLASS_HUNTER || 
				ent->client->NPC_class == CLASS_SCOUT ||  
				ent->client->NPC_class == CLASS_ALCHEMIST )
	{
		{
			G_PlayEffect( G_EffectIndex( "stasis/altmuzzle_flash" ), ent->client->renderInfo.handRPoint );
		}
	}
	// ACID  
	else if ( ent->client->NPC_class == CLASS_INTERROGATOR || 
				ent->client->NPC_class == CLASS_MINEMONSTER ||
				ent->client->NPC_class == CLASS_POISON || 
				ent->client->NPC_class == CLASS_BEAST || 
				ent->client->NPC_class == CLASS_GOLEM_POISON ||
				ent->client->NPC_class == CLASS_DEMON_POISON || 
				ent->client->NPC_class == CLASS_R2D2 ||
				ent->client->NPC_class == CLASS_R5D2 )
 	{
		{
			G_PlayEffect( G_EffectIndex( "acid/altmuzzle_flash" ), ent->client->renderInfo.handRPoint );
		}
	}
	// MUD  
	else if ( ent->client->NPC_class == CLASS_UGNAUGHT ||
				ent->client->NPC_class == CLASS_WATER || 
				ent->client->NPC_class == CLASS_GOLEM_WATER || 
				ent->client->NPC_class == CLASS_SHARK || 
				ent->client->NPC_class == CLASS_REELO || 
				ent->client->NPC_class == CLASS_LYCAN ||
				ent->client->NPC_class == CLASS_SPIDERREAVER3 )
	{
		{
			G_PlayEffect( G_EffectIndex( "flechette2/altmuzzle_flash" ), ent->client->renderInfo.handRPoint );
		}
	}
	// STONE  
	else if ( ent->client->NPC_class == CLASS_EARTHREAVER || 
				ent->client->NPC_class == CLASS_LIZARD ||
				ent->client->NPC_class == CLASS_DEMON_BLADE )
	{
		{
			G_PlayEffect( G_EffectIndex( "flechette3/altmuzzle_flash" ), ent->client->renderInfo.handRPoint );
		}
	}
	// ASH
	else if ( ent->client->NPC_class == CLASS_IMPERIAL || 
				ent->client->NPC_class == CLASS_BESPIN_COP ||
				ent->client->NPC_class == CLASS_HAZARD_TROOPER ||
				ent->client->NPC_class == CLASS_CLAW || 
				ent->client->NPC_class == CLASS_WAMPA || 
				ent->client->NPC_class == CLASS_TURELIM || 
				ent->client->NPC_class == CLASS_FIREREAVER || 
				ent->client->NPC_class == CLASS_TANKREAVER || 
				ent->client->NPC_class == CLASS_FIRE || 
				ent->client->NPC_class == CLASS_GOLEM_FIRE || 
				ent->client->NPC_class == CLASS_HELLGOLEM || 
				ent->client->NPC_class == CLASS_GOLEM_LAVA || 
				ent->client->NPC_class == CLASS_DEMON_FIRE || 
				ent->client->NPC_class == CLASS_GOLEM_SMOKE )
	{
		{
			G_PlayEffect( G_EffectIndex( "ash/altmuzzle_flash" ), ent->client->renderInfo.handRPoint );
		}
	}
	// GREEN CRYSTAL 
	else if ( ent->client->NPC_class == CLASS_GALAKMECH || 
				ent->client->NPC_class == CLASS_SABER_DROID ||
				ent->client->NPC_class == CLASS_PROTOCOL || 
				ent->client->NPC_class == CLASS_CRYSTAL || 
				ent->client->NPC_class == CLASS_LIGHT || 
				ent->client->NPC_class == CLASS_GOLEM_LIGHT || 
				ent->client->NPC_class == CLASS_GOLEM_GOLD || 
				ent->client->NPC_class == CLASS_GOLEM_ICE ||
				ent->client->NPC_class == CLASS_GOLEM_METAL ||
				ent->client->NPC_class == CLASS_DEMON_ELECTRO )
	{
		{
			G_PlayEffect( G_EffectIndex( "gcrystal/altmuzzle_flash" ), ent->client->renderInfo.handRPoint );
		}
	}
	// DUST  
	else if ( ent->client->NPC_class == CLASS_SABOTEUR || 
				ent->client->NPC_class == CLASS_DARK || 
				ent->client->NPC_class == CLASS_GOLEM_DARK || 
				ent->client->NPC_class == CLASS_VOID || 
				ent->client->NPC_class == CLASS_GOLEM_VOID || 
				ent->client->NPC_class == CLASS_DARKREAVER || 
				ent->client->NPC_class == CLASS_SHADOWTROOPER ||
				ent->client->NPC_class == CLASS_PROBE ||
				ent->client->NPC_class == CLASS_POLTER || 
				ent->client->NPC_class == CLASS_AIR || 
				ent->client->NPC_class == CLASS_GOLEM_AIR || 
				ent->client->NPC_class == CLASS_ROCKETTROOPER || 
				ent->client->NPC_class == CLASS_BOBAFETT || 
				ent->client->NPC_class == CLASS_SPIDERREAVER2 || 
				ent->client->NPC_class == CLASS_THRALL || 
				ent->client->NPC_class == CLASS_BIRD )
	{
		{
			G_PlayEffect( G_EffectIndex( "dust/altmuzzle_flash" ), ent->client->renderInfo.handRPoint );
		}
	}
	// SPECTRAL  
	else if ( ent->client->NPC_class == CLASS_TUSKEN || 
				ent->client->NPC_class == CLASS_MELC || 
				ent->client->NPC_class == CLASS_SUMMONER || 
				ent->client->NPC_class == CLASS_SLUAGH || 
				ent->client->NPC_class == CLASS_DREADNOUGHT || 
				ent->client->NPC_class == CLASS_REAPER || 
				ent->client->NPC_class == CLASS_RANCOR ||
				ent->client->NPC_class == CLASS_JAWA || 
				ent->client->NPC_class == CLASS_GHOST || 				
				ent->client->NPC_class == CLASS_NECRO || 
				ent->client->NPC_class == CLASS_GOLEM_NECRO ||
				ent->client->NPC_class == CLASS_GLIDER || 
				ent->client->NPC_class == CLASS_ARIEL ||
				ent->client->NPC_class == CLASS_FISH || 
				ent->client->NPC_class == CLASS_VAMPIRE_GHOST ||
				ent->client->NPC_class == CLASS_ELDERGOD || 
				ent->client->NPC_class == CLASS_TENTACLE || 
				ent->client->NPC_class == CLASS_ATST || 
				ent->client->NPC_class == CLASS_SAND_CREATURE ) 
	{
		{
			G_PlayEffect( G_EffectIndex( "spectral/altmuzzle_flash" ), ent->client->renderInfo.handRPoint );
		}
	}
	// THORN 
	else if ( ent->client->NPC_class == CLASS_SWAMP )
	{
		{
			G_PlayEffect( G_EffectIndex( "thorn/altmuzzle_flash" ), ent->client->renderInfo.handRPoint );
		}
	}
	// IVY
	else if ( ent->client->NPC_class == CLASS_PRISONER || 
				ent->client->NPC_class == CLASS_SENTRY ||
				ent->client->NPC_class == CLASS_IVY ) 
	{
		{
			G_PlayEffect( G_EffectIndex( "ivy/altmuzzle_flash" ), ent->client->renderInfo.handRPoint );
		}
	}
	// HELLISH IVY
	else if ( ent->client->NPC_class == CLASS_CENOBITA || 
				ent->client->NPC_class == CLASS_PROPHET || 
				ent->client->NPC_class == CLASS_TAVION || 
				ent->client->NPC_class == CLASS_DESANN ||
				ent->client->NPC_class == CLASS_HYLDEN || 
				ent->client->NPC_class == CLASS_HYLDENLORD ) 
	{
		{
			G_PlayEffect( G_EffectIndex( "ivy2/altmuzzle_flash" ), ent->client->renderInfo.handRPoint );
		}
	}
	// DEFAULT EARTH SHOOT   
	else if ( ent->client->NPC_class == CLASS_DRUID || 
				ent->client->NPC_class == CLASS_VAMPIRE_ELF || 
				ent->client->NPC_class == CLASS_VAMPIRE_ELF2 ||
				ent->client->NPC_class == CLASS_VAMPIRE_ELF3 ||
				ent->client->NPC_class == CLASS_REAVER || 
				ent->client->NPC_class == CLASS_RAZIEL ||
				ent->client->NPC_class == CLASS_NECROREAVER ||
				ent->client->NPC_class == CLASS_LIGHTREAVER ||
				ent->client->NPC_class == CLASS_DRUIDREAVER ||
				ent->client->NPC_class == CLASS_ICEREAVER || 
				ent->client->NPC_class == CLASS_SPIDERREAVER ||
				ent->client->NPC_class == CLASS_AIRREAVER || 
				ent->client->NPC_class == CLASS_MENTALIST || 
				ent->client->NPC_class == CLASS_HOLY ||  
				ent->client->NPC_class == CLASS_SONIC || 
				ent->client->NPC_class == CLASS_GOLEM ||
				ent->client->NPC_class == CLASS_GOLEM_SONIC ||
				ent->client->NPC_class == CLASS_GOLEM_HOLY || 				 
				ent->client->NPC_class == CLASS_COMMANDO || 
				ent->client->NPC_class == CLASS_STORMTROOPER || 
				ent->client->NPC_class == CLASS_GONK ||
				ent->client->NPC_class == CLASS_HOWLER || 
				ent->client->NPC_class == CLASS_IMPWORKER || 
				ent->client->NPC_class == CLASS_WIZARD || 
				ent->client->NPC_class == CLASS_JEDI || 
				ent->client->NPC_class == CLASS_LANDO || 
				ent->client->NPC_class == CLASS_WEEQUAY || 
				ent->client->NPC_class == CLASS_LUKE || 
				ent->client->NPC_class == CLASS_MONMOTHA || 
				ent->client->NPC_class == CLASS_MORGANKATARN || 
				ent->client->NPC_class == CLASS_MURJJ || 
				ent->client->NPC_class == CLASS_REBEL || 
				ent->client->NPC_class == CLASS_REMOTE || 
				ent->client->NPC_class == CLASS_RODIAN || 
				ent->client->NPC_class == CLASS_SEEKER || 
				ent->client->NPC_class == CLASS_TRANDOSHAN || 
				ent->client->NPC_class == CLASS_BARTENDER || 
				ent->client->NPC_class == CLASS_ASSASSIN_DROID || 
				ent->client->NPC_class == CLASS_PLAYER || 
				ent->client->NPC_class == CLASS_VEHICLE )
	{
		{
			G_PlayEffect( G_EffectIndex( "flechette/altmuzzle_flash" ), ent->client->renderInfo.handRPoint );
		}
	}
	else if ( ent->client->NPC_class == CLASS_FIGHTER )
	{
		{
			G_PlayEffect( G_EffectIndex( "flechette0/altmuzzle_flash" ), ent->client->renderInfo.handRPoint );
		}
	}
}
else 
{
	// STASIS
	if ( ent->client->NPC_class == CLASS_ZEPH ||
				ent->client->NPC_class == CLASS_DECEIVER ||
				ent->client->NPC_class == CLASS_DUMAHIM || 
				ent->client->NPC_class == CLASS_RAHABIM ||
				ent->client->NPC_class == CLASS_VAMPIRE ||
				ent->client->NPC_class == CLASS_VAMPIRE_DEMON ||
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
				ent->client->NPC_class == CLASS_VAMPIRE_FEAR2 ||  
				ent->client->NPC_class == CLASS_VAMPIRE_BLOOD ||
				ent->client->NPC_class == CLASS_VAMPIRE_SOUND || 
				ent->client->NPC_class == CLASS_VAMPIRE_LYCAN || 
				ent->client->NPC_class == CLASS_VAMPIRE_WATER || 
				ent->client->NPC_class == CLASS_VAMPIRE_FROST ||  
				ent->client->NPC_class == CLASS_VAMPIRE_ARCHER || 
				ent->client->NPC_class == CLASS_HALFVAMPIRE || 
				ent->client->NPC_class == CLASS_RAZIELHIM ||  
				ent->client->NPC_class == CLASS_ESKANDOR ||  
				ent->client->NPC_class == CLASS_VAMPIRE_BOUNTY || 
				ent->client->NPC_class == CLASS_VAMPIRE_ZOMBIE || 
				ent->client->NPC_class == CLASS_VAMPIRE_SARAFAN ||  
				ent->client->NPC_class == CLASS_REBORN || 
				ent->client->NPC_class == CLASS_KYLE ||
				ent->client->NPC_class == CLASS_ALORA ||
				ent->client->NPC_class == CLASS_NOGHRI ||
				ent->client->NPC_class == CLASS_FLIER2 ||
				ent->client->NPC_class == CLASS_SWAMPTROOPER || 
				ent->client->NPC_class == CLASS_GRAN ||
				ent->client->NPC_class == CLASS_JAN || 
				ent->client->NPC_class == CLASS_GALAK || 
				ent->client->NPC_class == CLASS_MARK1 ||
				ent->client->NPC_class == CLASS_MARK2 ||
				ent->client->NPC_class == CLASS_SARAFAN || 
				ent->client->NPC_class == CLASS_ARCHER ||  
				ent->client->NPC_class == CLASS_WARRIOR || 
				ent->client->NPC_class == CLASS_HUNTER || 
				ent->client->NPC_class == CLASS_SCOUT ||  
				ent->client->NPC_class == CLASS_ALCHEMIST )
	{
		{
			G_PlayEffect( G_EffectIndex( "stasis/muzzle_flash" ), ent->client->renderInfo.handRPoint );
		}
	}
	// ACID  
	else if ( ent->client->NPC_class == CLASS_INTERROGATOR || 
				ent->client->NPC_class == CLASS_MINEMONSTER ||
				ent->client->NPC_class == CLASS_POISON || 
				ent->client->NPC_class == CLASS_BEAST || 
				ent->client->NPC_class == CLASS_GOLEM_POISON ||
				ent->client->NPC_class == CLASS_DEMON_POISON || 
				ent->client->NPC_class == CLASS_R2D2 ||
				ent->client->NPC_class == CLASS_R5D2 )
 	{
		{
			G_PlayEffect( G_EffectIndex( "acid/muzzle_flash" ), ent->client->renderInfo.handRPoint );
		}
	}
	// MUD  
	else if ( ent->client->NPC_class == CLASS_UGNAUGHT ||
				ent->client->NPC_class == CLASS_WATER || 
				ent->client->NPC_class == CLASS_GOLEM_WATER || 
				ent->client->NPC_class == CLASS_SHARK || 
				ent->client->NPC_class == CLASS_REELO || 
				ent->client->NPC_class == CLASS_LYCAN ||
				ent->client->NPC_class == CLASS_SPIDERREAVER3 )
	{
		{
			G_PlayEffect( G_EffectIndex( "flechette2/muzzle_flash" ), ent->client->renderInfo.handRPoint );
		}
	}
	// STONE  
	else if ( ent->client->NPC_class == CLASS_EARTHREAVER || 
				ent->client->NPC_class == CLASS_LIZARD ||
				ent->client->NPC_class == CLASS_DEMON_BLADE )
	{
		{
			G_PlayEffect( G_EffectIndex( "flechette3/muzzle_flash" ), ent->client->renderInfo.handRPoint );
		}
	}
	// ASH
	else if ( ent->client->NPC_class == CLASS_IMPERIAL || 
				ent->client->NPC_class == CLASS_BESPIN_COP ||
				ent->client->NPC_class == CLASS_HAZARD_TROOPER ||
				ent->client->NPC_class == CLASS_CLAW || 
				ent->client->NPC_class == CLASS_WAMPA || 
				ent->client->NPC_class == CLASS_TURELIM || 
				ent->client->NPC_class == CLASS_FIREREAVER || 
				ent->client->NPC_class == CLASS_TANKREAVER || 
				ent->client->NPC_class == CLASS_FIRE || 
				ent->client->NPC_class == CLASS_GOLEM_FIRE || 
				ent->client->NPC_class == CLASS_HELLGOLEM || 
				ent->client->NPC_class == CLASS_GOLEM_LAVA || 
				ent->client->NPC_class == CLASS_DEMON_FIRE || 
				ent->client->NPC_class == CLASS_GOLEM_SMOKE )
	{
		{
			G_PlayEffect( G_EffectIndex( "ash/muzzle_flash" ), ent->client->renderInfo.handRPoint );
		}
	}
	// GREEN CRYSTAL 
	else if ( ent->client->NPC_class == CLASS_GALAKMECH || 
				ent->client->NPC_class == CLASS_SABER_DROID ||
				ent->client->NPC_class == CLASS_PROTOCOL || 
				ent->client->NPC_class == CLASS_CRYSTAL || 
				ent->client->NPC_class == CLASS_LIGHT || 
				ent->client->NPC_class == CLASS_GOLEM_LIGHT || 
				ent->client->NPC_class == CLASS_GOLEM_GOLD || 
				ent->client->NPC_class == CLASS_GOLEM_ICE ||
				ent->client->NPC_class == CLASS_GOLEM_METAL ||
				ent->client->NPC_class == CLASS_DEMON_ELECTRO )
	{
		{
			G_PlayEffect( G_EffectIndex( "gcrystal/muzzle_flash" ), ent->client->renderInfo.handRPoint );
		}
	}
	// DUST  
	else if ( ent->client->NPC_class == CLASS_SABOTEUR || 
				ent->client->NPC_class == CLASS_DARK || 
				ent->client->NPC_class == CLASS_GOLEM_DARK || 
				ent->client->NPC_class == CLASS_VOID || 
				ent->client->NPC_class == CLASS_GOLEM_VOID || 
				ent->client->NPC_class == CLASS_DARKREAVER || 
				ent->client->NPC_class == CLASS_SHADOWTROOPER ||
				ent->client->NPC_class == CLASS_PROBE ||
				ent->client->NPC_class == CLASS_POLTER || 
				ent->client->NPC_class == CLASS_AIR || 
				ent->client->NPC_class == CLASS_GOLEM_AIR || 
				ent->client->NPC_class == CLASS_ROCKETTROOPER || 
				ent->client->NPC_class == CLASS_BOBAFETT || 
				ent->client->NPC_class == CLASS_SPIDERREAVER2 || 
				ent->client->NPC_class == CLASS_THRALL || 
				ent->client->NPC_class == CLASS_BIRD )
	{
		{
			G_PlayEffect( G_EffectIndex( "dust/muzzle_flash" ), ent->client->renderInfo.handRPoint );
		}
	}
	// SPECTRAL  
	else if ( ent->client->NPC_class == CLASS_TUSKEN || 
				ent->client->NPC_class == CLASS_MELC || 
				ent->client->NPC_class == CLASS_SUMMONER || 
				ent->client->NPC_class == CLASS_SLUAGH || 
				ent->client->NPC_class == CLASS_DREADNOUGHT || 
				ent->client->NPC_class == CLASS_REAPER || 
				ent->client->NPC_class == CLASS_RANCOR ||
				ent->client->NPC_class == CLASS_JAWA || 
				ent->client->NPC_class == CLASS_GHOST || 				
				ent->client->NPC_class == CLASS_NECRO || 
				ent->client->NPC_class == CLASS_GOLEM_NECRO ||
				ent->client->NPC_class == CLASS_GLIDER || 
				ent->client->NPC_class == CLASS_ARIEL ||
				ent->client->NPC_class == CLASS_FISH || 
				ent->client->NPC_class == CLASS_VAMPIRE_GHOST ||
				ent->client->NPC_class == CLASS_ELDERGOD || 
				ent->client->NPC_class == CLASS_TENTACLE || 
				ent->client->NPC_class == CLASS_ATST || 
				ent->client->NPC_class == CLASS_SAND_CREATURE ) 
	{
		{
			G_PlayEffect( G_EffectIndex( "spectral/muzzle_flash" ), ent->client->renderInfo.handRPoint );
		}
	}
	// THORN 
	else if ( ent->client->NPC_class == CLASS_SWAMP )
	{
		{
			G_PlayEffect( G_EffectIndex( "thorn/muzzle_flash" ), ent->client->renderInfo.handRPoint );
		}
	}
	// IVY
	else if ( ent->client->NPC_class == CLASS_PRISONER || 
				ent->client->NPC_class == CLASS_SENTRY ||
				ent->client->NPC_class == CLASS_IVY ) 
	{
		{
			G_PlayEffect( G_EffectIndex( "ivy/muzzle_flash" ), ent->client->renderInfo.handRPoint );
		}
	}
	// HELLISH IVY
	else if ( ent->client->NPC_class == CLASS_CENOBITA || 
				ent->client->NPC_class == CLASS_PROPHET || 
				ent->client->NPC_class == CLASS_TAVION || 
				ent->client->NPC_class == CLASS_DESANN ||
				ent->client->NPC_class == CLASS_HYLDEN || 
				ent->client->NPC_class == CLASS_HYLDENLORD ) 
	{
		{
			G_PlayEffect( G_EffectIndex( "ivy2/muzzle_flash" ), ent->client->renderInfo.handRPoint );
		}
	}
	// DEFAULT EARTH SHOOT   
	else if ( ent->client->NPC_class == CLASS_DRUID || 
				ent->client->NPC_class == CLASS_VAMPIRE_ELF || 
				ent->client->NPC_class == CLASS_VAMPIRE_ELF2 ||
				ent->client->NPC_class == CLASS_VAMPIRE_ELF3 ||
				ent->client->NPC_class == CLASS_REAVER || 
				ent->client->NPC_class == CLASS_RAZIEL ||
				ent->client->NPC_class == CLASS_NECROREAVER ||
				ent->client->NPC_class == CLASS_LIGHTREAVER ||
				ent->client->NPC_class == CLASS_DRUIDREAVER ||
				ent->client->NPC_class == CLASS_ICEREAVER || 
				ent->client->NPC_class == CLASS_SPIDERREAVER ||
				ent->client->NPC_class == CLASS_AIRREAVER || 
				ent->client->NPC_class == CLASS_MENTALIST || 
				ent->client->NPC_class == CLASS_HOLY ||  
				ent->client->NPC_class == CLASS_SONIC || 
				ent->client->NPC_class == CLASS_GOLEM ||
				ent->client->NPC_class == CLASS_GOLEM_SONIC ||
				ent->client->NPC_class == CLASS_GOLEM_HOLY || 				 
				ent->client->NPC_class == CLASS_COMMANDO || 
				ent->client->NPC_class == CLASS_STORMTROOPER || 
				ent->client->NPC_class == CLASS_GONK ||
				ent->client->NPC_class == CLASS_HOWLER || 
				ent->client->NPC_class == CLASS_IMPWORKER || 
				ent->client->NPC_class == CLASS_WIZARD || 
				ent->client->NPC_class == CLASS_JEDI || 
				ent->client->NPC_class == CLASS_LANDO || 
				ent->client->NPC_class == CLASS_WEEQUAY || 
				ent->client->NPC_class == CLASS_LUKE || 
				ent->client->NPC_class == CLASS_MONMOTHA || 
				ent->client->NPC_class == CLASS_MORGANKATARN || 
				ent->client->NPC_class == CLASS_MURJJ || 
				ent->client->NPC_class == CLASS_REBEL || 
				ent->client->NPC_class == CLASS_REMOTE || 
				ent->client->NPC_class == CLASS_RODIAN || 
				ent->client->NPC_class == CLASS_SEEKER || 
				ent->client->NPC_class == CLASS_TRANDOSHAN || 
				ent->client->NPC_class == CLASS_BARTENDER || 
				ent->client->NPC_class == CLASS_ASSASSIN_DROID || 
				ent->client->NPC_class == CLASS_PLAYER || 
				ent->client->NPC_class == CLASS_VEHICLE )
	{
		{
			G_PlayEffect( G_EffectIndex( "flechette/muzzle_flash" ), ent->client->renderInfo.handRPoint );
		}
	}
	else if ( ent->client->NPC_class == CLASS_FIGHTER )
	{
		{
			G_PlayEffect( G_EffectIndex( "flechette0/muzzle_flash" ), ent->client->renderInfo.handRPoint );
		}
	}
}
}

//-----------------------
//	Alchemic Cannon 
//-----------------------

//---------------------------------------------------------
static void WP_CannonMainFire( gentity_t *ent )
//---------------------------------------------------------
{
	vec3_t		fwd, angs, start;
	gentity_t	*missile;
	float		damage = weaponData[WP_CANNON].damage, vel = CANNON_VEL;

	VectorCopy( muzzle, start );
	WP_TraceSetStart( ent, start, vec3_origin, vec3_origin );//make sure our start point isn't on the other side of a wall

	// If we aren't the player, we will cut the velocity and damage of the shots
	if ( ent->s.number )
	{
		damage *= 0.75f;
		vel *= 0.5f;
	}

//	if ( ent->client && ent->client->ps.powerups[PW_WEAPON_OVERCHARGE] > 0 && ent->client->ps.powerups[PW_WEAPON_OVERCHARGE] > cg.time )
//	{
//		// in overcharge mode, so doing double damage
//		damage *= 2;
//	}

	for ( int i = 0; i < CANNON_SHOTS; i++ )
	{
		vectoangles( forwardVec, angs );

		if ( i == 0 && ent->s.number == 0 )
		{
			// do nothing on the first shot for the player, this one will hit the crosshairs
		}
		else
		{
			angs[PITCH] += crandom() * CANNON_SPREAD;
			angs[YAW]	+= crandom() * CANNON_SPREAD;
		}

		AngleVectors( angs, fwd, NULL, NULL );

		WP_MissileTargetHint(ent, start, fwd);

		missile = CreateMissile( start, fwd, vel, 10000, ent );

		missile->classname = "flech_proj";
		missile->s.weapon = WP_CANNON;

		VectorSet( missile->maxs, CANNON_SIZE, CANNON_SIZE, CANNON_SIZE );
		VectorScale( missile->maxs, -1, missile->mins );

		missile->damage = damage;

//		if ( ent->client && ent->client->ps.powerups[PW_WEAPON_OVERCHARGE] > 0 && ent->client->ps.powerups[PW_WEAPON_OVERCHARGE] > cg.time )
//		{
//			missile->flags |= FL_OVERCHARGED;
//		}
			
		missile->dflags = (DAMAGE_DEATH_KNOCKBACK|DAMAGE_EXTRA_KNOCKBACK);
		
		missile->methodOfDeath = MOD_ALCHEMIC;
		missile->clipmask = MASK_SHOT | CONTENTS_LIGHTSABER;

		// we don't want it to bounce forever
		missile->bounceCount = Q_irand(1,2);

		missile->s.eFlags |= EF_BOUNCE_SHRAPNEL;
		ent->client->sess.missionStats.shotsFired++;
	}
}

//---------------------------------------------------------
void alchemic_mine_think( gentity_t *ent )
//---------------------------------------------------------
{
	int			count;
	qboolean	blow = qfalse;

	// if it isn't time to auto-explode, do a small proximity check
	if ( ent->delay > level.time )
	{
		count = G_RadiusList( ent->currentOrigin, CANNON_MINE_RADIUS_CHECK, ent, qtrue, ent_list );

		for ( int i = 0; i < count; i++ )
		{
			if ( ent_list[i]->client && ent_list[i]->health > 0 && ent->activator && ent_list[i]->s.number != ent->activator->s.number )
			{
				blow = qtrue;
				break;
			}
		}
	}
	else
	{
		// well, we must die now
		blow = qtrue;
	}

	if ( blow )
	{
//		G_Sound( ent, G_SoundIndex( "sound/weapons/flechette/warning.wav" ));
		ent->e_ThinkFunc = thinkF_WP_Explode;
		ent->nextthink = level.time + 200;
	}
	else
	{
		// we probably don't need to do this thinking logic very often...maybe this is fast enough?
		ent->nextthink = level.time + 500;
	}
}

//---------------------------------------------------------
void alchemic_mine_stick( gentity_t *self, gentity_t *other, trace_t *trace )
//---------------------------------------------------------
{
	// turn us into a generic entity so we aren't running missile code
	self->s.eType = ET_GENERAL;

	self->s.modelindex = G_ModelIndex("models/weapons2/cannon/prox_mine.md3");
	self->e_TouchFunc = touchF_NULL;

	self->contents = CONTENTS_SOLID;
	self->takedamage = qtrue;
	self->health = 5;
	self->e_DieFunc = dieF_WP_ExplosiveDie;

	VectorSet( self->maxs, 5, 5, 5 );
	VectorScale( self->maxs, -1, self->mins );

	self->activator = self->owner;
	self->owner = NULL;

	WP_Stick( self, trace );
	
	self->e_ThinkFunc = thinkF_prox_mine_think;
	self->nextthink = level.time + 450;

	// sticks for twenty seconds, then auto blows.
	self->delay = level.time + 20000;

	gi.linkentity( self );
}
/* Old Flechette alt-fire code....
//---------------------------------------------------------
static void WP_FlechetteProxMine( gentity_t *ent )
//---------------------------------------------------------
{
	gentity_t	*missile = CreateMissile( muzzle, forwardVec, FLECHETTE_MINE_VEL, 10000, ent, qtrue );

	missile->fxID = G_EffectIndex( "flechette/explosion" );

	missile->classname = "proxMine";
	missile->s.weapon = WP_FLECHETTE;

	missile->s.pos.trType = TR_GRAVITY;

	missile->s.eFlags |= EF_MISSILE_STICK;
	missile->e_TouchFunc = touchF_prox_mine_stick;

	missile->damage = FLECHETTE_MINE_DAMAGE;
	missile->methodOfDeath = MOD_EXPLOSIVE;

	missile->splashDamage = FLECHETTE_MINE_SPLASH_DAMAGE;
	missile->splashRadius = FLECHETTE_MINE_SPLASH_RADIUS;
	missile->splashMethodOfDeath = MOD_EXPLOSIVE_SPLASH;

	missile->clipmask = MASK_SHOT;

	// we don't want it to bounce forever
	missile->bounceCount = 0; 
}
*/
//----------------------------------------------
void WP_cannon_alt_blow( gentity_t *ent )
//----------------------------------------------
{
	EvaluateTrajectory( &ent->s.pos, level.time, ent->currentOrigin ); // Not sure if this is even necessary, but correct origins are cool?

	G_RadiusDamage( ent->currentOrigin, ent->owner, ent->splashDamage, ent->splashRadius, NULL, MOD_CANNON_ALT );
	G_PlayEffect( "cannon/alt_blow", ent->currentOrigin );

	G_FreeEntity( ent );
}

//------------------------------------------------------------------------------
static void WP_CreateCannonBouncyThing( vec3_t start, vec3_t fwd, gentity_t *self )
//------------------------------------------------------------------------------
{
	gentity_t	*missile = CreateMissile( start, fwd, 950 + random() * 700, 1500 + random() * 2000, self, qtrue );
	
	missile->e_ThinkFunc = thinkF_WP_cannon_alt_blow;

	missile->s.weapon = WP_CANNON;
	missile->classname = "cannon_alt";
	missile->mass = 4;

	// How 'bout we give this thing a size...
	VectorSet( missile->mins, -3.0f, -3.0f, -3.0f );
	VectorSet( missile->maxs, 3.0f, 3.0f, 3.0f );
	missile->clipmask = MASK_SHOT;
	missile->clipmask &= ~CONTENTS_CORPSE;

	// normal ones bounce, alt ones explode on impact
	missile->s.pos.trType = TR_GRAVITY;

	missile->s.eFlags |= EF_BOUNCE_HALF;

	missile->damage = weaponData[WP_CANNON].altDamage;
	missile->dflags = 0;
	missile->splashDamage = weaponData[WP_CANNON].altSplashDamage;
	missile->splashRadius = weaponData[WP_CANNON].altSplashRadius;

	missile->svFlags = SVF_USE_CURRENT_ORIGIN;

	missile->methodOfDeath = MOD_CANNON_ALT;
	missile->splashMethodOfDeath = MOD_CANNON_ALT;

	VectorCopy( start, missile->pos2 );
}

//---------------------------------------------------------
static void WP_CannonAltFire( gentity_t *self )
//---------------------------------------------------------
{
	vec3_t 	dir, fwd, start, angs;

	vectoangles( forwardVec, angs );
	VectorCopy( muzzle, start );

	WP_TraceSetStart( self, start, vec3_origin, vec3_origin );//make sure our start point isn't on the other side of a wall

	for ( int i = 0; i < 2; i++ )
	{
		VectorCopy( angs, dir );

		dir[PITCH] -= random() * 4 + 8; // make it fly upwards
		dir[YAW] += crandom() * 2;
		AngleVectors( dir, fwd, NULL, NULL );

		WP_CreateCannonBouncyThing( start, fwd, self );
		self->client->sess.missionStats.shotsFired++;
	}
}

//---------------------------------------------------------
void WP_FireCannon( gentity_t *ent, qboolean alt_fire )
//---------------------------------------------------------
{
	if ( alt_fire )
	{
		WP_CannonAltFire( ent );
	}
	else
	{
		WP_CannonMainFire( ent );
	}
}




