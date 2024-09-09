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

// leave this line at the top of all AI_xxxx.cpp files for PCH reasons...
#include "g_headers.h"


#include "b_local.h"
#include "g_nav.h"

gentity_t *CreateMissile( vec3_t org, vec3_t dir, float vel, int life, gentity_t *owner, qboolean altFire = qfalse );
extern gitem_t	*FindItemForAmmo( ammo_t ammo );

#define MIN_DISTANCE		256
#define MIN_DISTANCE_SQR	( MIN_DISTANCE * MIN_DISTANCE )

#define SENTRY_FORWARD_BASE_SPEED	10
#define SENTRY_FORWARD_MULTIPLIER	5

#define SENTRY_VELOCITY_DECAY	0.85f
#define SENTRY_STRAFE_VEL		256
#define SENTRY_STRAFE_DIS		200
#define SENTRY_UPWARD_PUSH		32
#define SENTRY_HOVER_HEIGHT		24

//Local state enums
enum
{
	LSTATE_NONE = 0,
	LSTATE_ASLEEP,
	LSTATE_WAKEUP,
	LSTATE_ACTIVE,
	LSTATE_POWERING_UP,
	LSTATE_ATTACKING,
};

/*
-------------------------
NPC_Sentry_Precache
-------------------------
*/
void NPC_Sentry_Precache(void)
{
	G_SoundIndex( "sound/chars/sentry/misc/sentry_explo" );
	G_SoundIndex( "sound/chars/sentry/misc/sentry_pain" );
	G_SoundIndex( "sound/chars/sentry/misc/sentry_shield_open" );
	G_SoundIndex( "sound/chars/sentry/misc/sentry_shield_close" );
	G_SoundIndex( "sound/chars/sentry/misc/sentry_hover_1_lp" );
	G_SoundIndex( "sound/chars/sentry/misc/sentry_hover_2_lp" );

	for ( int i = 1; i < 4; i++)
	{
		G_SoundIndex( va( "sound/chars/sentry/misc/talk%d", i ) );
	}

	G_EffectIndex( "bryar/muzzle_flash");
	G_EffectIndex( "env/med_explode");

	RegisterItem( FindItemForAmmo( AMMO_BLASTER ));
}

/*
================
sentry_use
================
*/
void sentry_use( gentity_t *self, gentity_t *other, gentity_t *activator)
{
	G_ActivateBehavior(self,BSET_USE);

	self->flags &= ~FL_SHIELDED;
	NPC_SetAnim( self, SETANIM_BOTH, BOTH_POWERUP1, SETANIM_FLAG_OVERRIDE|SETANIM_FLAG_HOLD );
//	self->NPC->localState = LSTATE_WAKEUP;
	self->NPC->localState = LSTATE_ACTIVE;
}

/*
-------------------------
NPC_Sentry_Pain
-------------------------
*/
void NPC_Sentry_Pain( gentity_t *self, gentity_t *inflictor, gentity_t *other, const vec3_t point, int damage, int mod,int hitLoc ) 
{		
	NPC_Pain( self, inflictor, other, point, damage, mod );

	if ( mod == MOD_DEMP2 || mod == MOD_DEMP2_ALT )
	{
		self->NPC->burstCount = 0;
		TIMER_Set( self, "attackDelay", Q_irand( 9000, 12000) );
		self->flags |= FL_SHIELDED;
		NPC_SetAnim( self, SETANIM_BOTH, BOTH_FLY_SHIELDED, SETANIM_FLAG_OVERRIDE|SETANIM_FLAG_HOLD );
		G_SoundOnEnt( self, CHAN_AUTO, "sound/chars/sentry/misc/sentry_pain" );		

		self->NPC->localState = LSTATE_ACTIVE;
	}

	// You got hit, go after the enemy
//	if (self->NPC->localState == LSTATE_ASLEEP)
//	{
//		G_Sound( self, G_SoundIndex("sound/chars/sentry/misc/shieldsopen.wav"));
//
//		self->flags &= ~FL_SHIELDED;
//		NPC_SetAnim( self, SETANIM_BOTH, BOTH_POWERUP1, SETANIM_FLAG_OVERRIDE|SETANIM_FLAG_HOLD );
//		self->NPC->localState = LSTATE_WAKEUP;
//	}
}

/*
-------------------------
Sentry_Fire
-------------------------
*/
void Sentry_Fire (void)
{
	vec3_t	muzzle;
	static	vec3_t	forward, vright, up;
	gentity_t	*missile;
	mdxaBone_t	boltMatrix;
	int			bolt;

	NPC->flags &= ~FL_SHIELDED;

	if ( NPCInfo->localState == LSTATE_POWERING_UP )
	{
		if ( TIMER_Done( NPC, "powerup" ))
		{
			NPCInfo->localState = LSTATE_ATTACKING;
			NPC_SetAnim( NPC, SETANIM_BOTH, BOTH_ATTACK1, SETANIM_FLAG_OVERRIDE|SETANIM_FLAG_HOLD );
		}
		else
		{
			// can't do anything right now
			return;
		}
	}
	else if ( NPCInfo->localState == LSTATE_ACTIVE )
	{
		NPCInfo->localState = LSTATE_POWERING_UP;

		G_SoundOnEnt( NPC, CHAN_AUTO, "sound/chars/sentry/misc/sentry_shield_open" );		
		NPC_SetAnim( NPC, SETANIM_BOTH, BOTH_POWERUP1, SETANIM_FLAG_OVERRIDE|SETANIM_FLAG_HOLD );
		TIMER_Set( NPC, "powerup", 250 );
		return;
	}
	else if ( NPCInfo->localState != LSTATE_ATTACKING )
	{
		// bad because we are uninitialized
		NPCInfo->localState = LSTATE_ACTIVE;
		return;
	}

	// Which muzzle to fire from?
	int which = NPCInfo->burstCount % 3;
	switch( which )
	{
	case 0:
		bolt = NPC->genericBolt1; 
		break;
	case 1:
		bolt = NPC->genericBolt2; 
		break;
	case 2:
	default:
		bolt = NPC->genericBolt3; 
	}

	gi.G2API_GetBoltMatrix( NPC->ghoul2, NPC->playerModel, 
				bolt,
				&boltMatrix, NPC->currentAngles, NPC->currentOrigin, (cg.time?cg.time:level.time),
				NULL, NPC->s.modelScale );

	gi.G2API_GiveMeVectorFromMatrix( boltMatrix, ORIGIN, muzzle );

	AngleVectors( NPC->currentAngles, forward, vright, up );
//	G_Sound( NPC, G_SoundIndex("sound/chars/sentry/misc/shoot.wav"));

	G_PlayEffect( "bryar/muzzle_flash", muzzle, forward );

	missile = CreateMissile( muzzle, forward, 1600, 10000, NPC );

	missile->classname = "bryar_proj";
	missile->s.weapon = WP_BRYAR_PISTOL;

	missile->dflags = DAMAGE_DEATH_KNOCKBACK;
	missile->methodOfDeath = MOD_ENERGY;
	missile->clipmask = MASK_SHOT | CONTENTS_LIGHTSABER;

	NPCInfo->burstCount++;
	NPC->attackDebounceTime = level.time + 50;
	missile->damage = 5;

	// now scale for difficulty
	if ( g_spskill->integer == 0 )
	{
		NPC->attackDebounceTime += 200;
		missile->damage = 1;
	}
	else if ( g_spskill->integer == 1 )
	{
		NPC->attackDebounceTime += 100;
		missile->damage = 3;
	}
}

/*
-------------------------
Sentry_MaintainHeight
-------------------------
*/
void Sentry_MaintainHeight( void )
{	
	float	dif;

	NPC->s.loopSound = G_SoundIndex( "sound/chars/sentry/misc/sentry_hover_1_lp" );

	// Update our angles regardless
	NPC_UpdateAngles( qtrue, qtrue );

	// If we have an enemy, we should try to hover at about enemy eye level
	if ( NPC->enemy )
	{
		// Find the height difference
		dif = (NPC->enemy->currentOrigin[2]+NPC->enemy->maxs[2]) - NPC->currentOrigin[2]; 

		// cap to prevent dramatic height shifts
		if ( fabs( dif ) > 8 )
		{
			if ( fabs( dif ) > SENTRY_HOVER_HEIGHT )
			{
				dif = ( dif < 0 ? -24 : 24 );
			}

			NPC->client->ps.velocity[2] = (NPC->client->ps.velocity[2]+dif)/2;
		}
	}
	else
	{
		gentity_t *goal = NULL;

		if ( NPCInfo->goalEntity )	// Is there a goal?
		{
			goal = NPCInfo->goalEntity;
		}
		else
		{
			goal = NPCInfo->lastGoalEntity;
		}

		if (goal)
		{
			dif = goal->currentOrigin[2] - NPC->currentOrigin[2];

			if ( fabs( dif ) > SENTRY_HOVER_HEIGHT )
			{
				ucmd.upmove = ( ucmd.upmove < 0 ? -4 : 4 );
			}
			else
			{
				if ( NPC->client->ps.velocity[2] )
				{
					NPC->client->ps.velocity[2] *= SENTRY_VELOCITY_DECAY;

					if ( fabs( NPC->client->ps.velocity[2] ) < 2 )
					{
						NPC->client->ps.velocity[2] = 0;
					}
				}
			}
		}
		// Apply friction to Z
		else if ( NPC->client->ps.velocity[2] )
		{
			NPC->client->ps.velocity[2] *= SENTRY_VELOCITY_DECAY;

			if ( fabs( NPC->client->ps.velocity[2] ) < 1 )
			{
				NPC->client->ps.velocity[2] = 0;
			}
		}
	}

	// Apply friction
	if ( NPC->client->ps.velocity[0] )
	{
		NPC->client->ps.velocity[0] *= SENTRY_VELOCITY_DECAY;

		if ( fabs( NPC->client->ps.velocity[0] ) < 1 )
		{
			NPC->client->ps.velocity[0] = 0;
		}
	}

	if ( NPC->client->ps.velocity[1] )
	{
		NPC->client->ps.velocity[1] *= SENTRY_VELOCITY_DECAY;

		if ( fabs( NPC->client->ps.velocity[1] ) < 1 )
		{
			NPC->client->ps.velocity[1] = 0;
		}
	}

	NPC_FaceEnemy( qtrue );
}

/*
-------------------------
Sentry_Idle
-------------------------
*/
void Sentry_Idle( void )
{
	Sentry_MaintainHeight();

	// Is he waking up?
	if (NPCInfo->localState == LSTATE_WAKEUP)
	{
		if (NPC->client->ps.torsoAnimTimer<=0)
		{
			NPCInfo->scriptFlags |= SCF_LOOK_FOR_ENEMIES;
			NPCInfo->burstCount = 0;
		}
	}
	else
	{
		NPC_SetAnim( NPC, SETANIM_BOTH, BOTH_SLEEP1, SETANIM_FLAG_OVERRIDE|SETANIM_FLAG_HOLD );
		NPC->flags |= FL_SHIELDED;

		NPC_BSIdle();
	}
}

/*
-------------------------
Sentry_Strafe
-------------------------
*/
void Sentry_Strafe( void )
{
	int		dir;
	vec3_t	end, right;
	trace_t	tr;

	AngleVectors( NPC->client->renderInfo.eyeAngles, NULL, right, NULL );

	// Pick a random strafe direction, then check to see if doing a strafe would be
	//	reasonable valid
	dir = ( rand() & 1 ) ? -1 : 1;
	VectorMA( NPC->currentOrigin, SENTRY_STRAFE_DIS * dir, right, end );

	gi.trace( &tr, NPC->currentOrigin, NULL, NULL, end, NPC->s.number, MASK_SOLID, (EG2_Collision)0, 0 );

	// Close enough
	if ( tr.fraction > 0.9f )
	{
		VectorMA( NPC->client->ps.velocity, SENTRY_STRAFE_VEL * dir, right, NPC->client->ps.velocity );

		// Add a slight upward push
		NPC->client->ps.velocity[2] += SENTRY_UPWARD_PUSH;

		// Set the strafe start time so we can do a controlled roll
		NPC->fx_time = level.time;
		NPCInfo->standTime = level.time + 3000 + random() * 500;
	}
}

/*
-------------------------
Sentry_Hunt
-------------------------
*/
void Sentry_Hunt( qboolean visible, qboolean advance )
{
	float	distance, speed;
	vec3_t	forward;

	//If we're not supposed to stand still, pursue the player
	if ( NPCInfo->standTime < level.time )
	{
		// Only strafe when we can see the player
		if ( visible )
		{
			Sentry_Strafe();
			return;
		}
	}

	//If we don't want to advance, stop here
	if ( !advance && visible )
		return;

	//Only try and navigate if the player is visible
	if ( visible == qfalse )
	{
		// Move towards our goal
		NPCInfo->goalEntity = NPC->enemy;
		NPCInfo->goalRadius = 12;

		NPC_MoveToGoal(qtrue);
		return;
	}
	else
	{
		VectorSubtract( NPC->enemy->currentOrigin, NPC->currentOrigin, forward );
		distance = VectorNormalize( forward );
	}

	speed = SENTRY_FORWARD_BASE_SPEED + SENTRY_FORWARD_MULTIPLIER * g_spskill->integer;
	VectorMA( NPC->client->ps.velocity, speed, forward, NPC->client->ps.velocity );
}

/*
-------------------------
Sentry_RangedAttack
-------------------------
*/
void Sentry_RangedAttack( qboolean visible, qboolean advance )
{
	if ( TIMER_Done( NPC, "attackDelay" ) && NPC->attackDebounceTime < level.time && visible )	// Attack?
	{
		if ( NPCInfo->burstCount > 6 )
		{
			if ( !NPC->fly_sound_debounce_time )
			{//delay closing down to give the player an opening
				NPC->fly_sound_debounce_time = level.time + Q_irand( 500, 2000 );
			}
			else if ( NPC->fly_sound_debounce_time < level.time )
			{
				NPCInfo->localState = LSTATE_ACTIVE;
				NPC->fly_sound_debounce_time = NPCInfo->burstCount = 0;
				TIMER_Set( NPC, "attackDelay", Q_irand( 2000, 3500) );
				NPC->flags |= FL_SHIELDED;
				NPC_SetAnim( NPC, SETANIM_BOTH, BOTH_FLY_SHIELDED, SETANIM_FLAG_OVERRIDE|SETANIM_FLAG_HOLD );
				G_SoundOnEnt( NPC, CHAN_AUTO, "sound/chars/sentry/misc/sentry_shield_close" );
			}
		}
		else
		{
			Sentry_Fire();
		}
	}

	if ( NPCInfo->scriptFlags & SCF_CHASE_ENEMIES )
	{
		Sentry_Hunt( visible, advance );
	}
}

/*
-------------------------
Sentry_AttackDecision
-------------------------
*/
void Sentry_AttackDecision( void )
{
	// Always keep a good height off the ground
	Sentry_MaintainHeight();

	NPC->s.loopSound = G_SoundIndex( "sound/chars/sentry/misc/sentry_hover_2_lp" );

	//randomly talk
	if ( TIMER_Done(NPC,"patrolNoise") )
	{
		if (TIMER_Done(NPC,"angerNoise"))
		{
			G_SoundOnEnt( NPC, CHAN_AUTO, va("sound/chars/sentry/misc/talk%d", Q_irand(1, 3)) );

			TIMER_Set( NPC, "patrolNoise", Q_irand( 4000, 10000 ) );
		}
	}

	// He's dead.
	if (NPC->enemy->health<1)
	{
		NPC->enemy = NULL;
		Sentry_Idle();
		return;
	}

	// If we don't have an enemy, just idle
	if ( NPC_CheckEnemyExt() == qfalse )
	{
		Sentry_Idle();
		return;
	}

	// Rate our distance to the target and visibilty
	float		distance	= (int) DistanceHorizontalSquared( NPC->currentOrigin, NPC->enemy->currentOrigin );	
	qboolean	visible		= NPC_ClearLOS( NPC->enemy );
	qboolean	advance		= (qboolean)(distance > MIN_DISTANCE_SQR);

	// If we cannot see our target, move to see it
	if ( visible == qfalse )
	{
		if ( NPCInfo->scriptFlags & SCF_CHASE_ENEMIES )
		{
			Sentry_Hunt( visible, advance );
			return;
		}
	}

	NPC_FaceEnemy( qtrue );

	Sentry_RangedAttack( visible, advance );
}

qboolean NPC_CheckPlayerTeamStealth( void );

/*
-------------------------
NPC_Sentry_Patrol
-------------------------
*/
void NPC_Sentry_Patrol( void )
{
	Sentry_MaintainHeight();

	//If we have somewhere to go, then do that
	if (!NPC->enemy)
	{
		if ( NPC_CheckPlayerTeamStealth() )
		{
			//NPC_AngerSound();
			NPC_UpdateAngles( qtrue, qtrue );
			return;
		}

		if ( UpdateGoal() )
		{
			//start loop sound once we move
			ucmd.buttons |= BUTTON_WALKING;
			NPC_MoveToGoal( qtrue );
		}

		//randomly talk
		if (TIMER_Done(NPC,"patrolNoise"))
		{
			G_SoundOnEnt( NPC, CHAN_AUTO, va("sound/chars/sentry/misc/talk%d", Q_irand(1, 3)) );

			TIMER_Set( NPC, "patrolNoise", Q_irand( 2000, 4000 ) );
		}
	}

	NPC_UpdateAngles( qtrue, qtrue );
}

/*
-------------------------
NPC_BSSentry_Default
-------------------------
*/
void NPC_BSSentry_Default( void )
{
	if ( NPC->targetname )
	{
		NPC->e_UseFunc = useF_sentry_use;
	}

	if (( NPC->enemy ) && (NPCInfo->localState != LSTATE_WAKEUP))
	{
		// Don't attack if waking up or if no enemy
		Sentry_AttackDecision();
	}
	else if ( NPCInfo->scriptFlags & SCF_LOOK_FOR_ENEMIES )
	{
		NPC_Sentry_Patrol();
	}
	else
	{
		Sentry_Idle();
	}
}

////////////////////////////////////////// FIGHTER AI ///////////////////////////////
/////////////////////////////////////// BASIC FIGHTER AI LIKE AI DEFAULT ////////////
void		Fighter_Precache( void );
void		Fighter_DustFallNear(const vec3_t origin, int dustcount);
void		Fighter_ChangeWeapon2(int wp);
qboolean	Fighter_StopKnockdown(gentity_t *self, gentity_t *pusher, const vec3_t pushDir, qboolean forceKnockdown = qfalse);
// Flight Related Functions (used Only by flying classes)
//--------------------------------------------------------
extern qboolean	Fighter_Flying( gentity_t *self );
extern void		Fighter_FlyStart( gentity_t *self );
extern void		Fighter_FlyStop( gentity_t *self );
// Called From Fighter_Pain()
//-----------------------------
void		Fighter_Pain( gentity_t *self, gentity_t *inflictor, int damage, int mod);


// Local: Other Tactics
//----------------------
void		Fighter_DoAmbushWait( gentity_t *self);
// Local: Respawning 
//-------------------
//bool		Fighter_Respawn();

// Called From Within AI_Jedi && AI_Seeker
//-----------------------------------------
void		Fighter_Fire();
void		Fighter_FireDecide();

// Local: Called From Tactics()
//----------------------------
void		Fighter_TacticsSelect();
bool		Fighter_CanSeeEnemy( gentity_t *self );


// Called From Fighter_RunBehavior()
//-------------------------------
void		Fighter_Update();		// Always Called First, Before Any Other Thinking
bool		Fighter_Tactics();		// If returns true, Jedi and Seeker AI not used
bool		Fighter_Flee();		// If returns true, Jedi and Seeker AI not used
//////////////////////////////////////////////////////////////////////////////
// FLY FUNCTIONS CALLED FOR FLYING! 
////////////////////////////////
void Fighter_MaintainHeight();
void Fighter_Idle();
void Fighter_Strafe();
void Fighter_Steer();
void Fighter_Hunt( qboolean visible, qboolean advance );
void NPC_Fighter_Patrol();
void Fighter_RangedAttack( qboolean visible, qboolean advance );
void Fighter_AttackDecision(); 
qboolean NPC_CheckPlayerTeamStealth();
void NPC_BSFighter_Default();
void Fighter_use( gentity_t *self, gentity_t *other, gentity_t *activator);
void NPC_Fighter_Pain( gentity_t *self, gentity_t *inflictor, gentity_t *other, const vec3_t point, int damage, int mod,int hitLoc );
////////////////////////////////////////////////////////////////////////////////////////
// External Functions 
////////////////////////////////////////////////////////////////////////////////////////
extern void		G_SoundAtSpot( vec3_t org, int soundIndex, qboolean broadcast );
extern void		G_CreateG2AttachedWeaponModel( gentity_t *ent, const char *weaponModel, int boltNum, int weaponNum );
extern void		ChangeWeapon( gentity_t *ent, int newWeapon );
extern void		WP_ResistForcePush( gentity_t *self, gentity_t *pusher, qboolean noPenalty );
extern void		ForceJump( gentity_t *self, usercmd_t *ucmd );
extern void		G_Knockdown( gentity_t *self, gentity_t *attacker, const vec3_t pushDir, float strength, qboolean breakSaberLock );
extern void WP_SaberAddG2SaberModels( gentity_t *ent, int specificSaberNum ); 
extern void WP_GunAddG2GunModels( gentity_t *ent, int specificGunNum );
// DUSTY ADD 
//extern qboolean heavyWeap(int);
//extern qboolean blasterWeap(int);
extern int ChooseBestWeapon(gentity_t* ent);
extern int ChooseWeaponRandom(gentity_t *ent, int wpnGroup);
//extern qboolean lightBlasterWeap(int);
//extern qboolean heavyBlasterWeap(int);
//extern qboolean meleeWeap(int);

// FIGHTER WEAPON CATEGORIES 
extern qboolean seekerWeap(int);//seeker missiles WP_ROCKET // WP_BOWCASTER
extern qboolean bombsWeap(int);//REPEATER, POISON, THERMAL, TRIP, DETPACK  
extern qboolean laserWeap(int);//
// BLASTER PISTOL: green laser - short stunner 
// BRYAR: red laser 
// POISON // chemical weapon
// BLASTER  // green laser big 
// JAWA // yellow 
// BLOODGUN // Blue laser 
extern qboolean chaffWeap(int);// FLECHETTE e DEMP2 
extern qboolean specialWeap(int); // CONCUSSION AM ROCKET
// DISGREGATOR 
// ATST MAIN
// ATST SIDE 
// EMPLACED 
////////////////////////////////////////////////////////////////////////////////////////
// Defines
////////////////////////////////////////////////////////////////////////////////////////
#define		FIGHTER_SHOOTRANGEMIN			1024 // Bombs and contromisure 
#define		FIGHTER_SHOOTRANGEMED			2048 // blasters
#define		FIGHTER_SHOOTRANGEMAX			8192 // max seeker and antimatter distance atk
#define		FIGHTER_SPECIALRANGE			16384 // special weapon ranged used for cruisers class 
#define FIGHTER_FORWARD_BASE_SPEED	75
#define FIGHTER_FORWARD_MULTIPLIER	5
#define FIGHTER_VELOCITY_DECAY	0.85f
#define FIGHTER_STRAFE_VEL		512
#define FIGHTER_STRAFE_DIS		512
#define FIGHTER_UPWARD_PUSH		32
#define FIGHTER_HOVER_HEIGHT		512
extern gitem_t	*FindItemForAmmo( ammo_t ammo );
#define FIGHTER_MIN_DISTANCE		1024
#define FIGHTER_MIN_DISTANCE_SQR	( MIN_DISTANCE * MIN_DISTANCE )
//Local state enums
enum
{
	FSTATE_NONE = 0,
	FSTATE_ASLEEP,
	FSTATE_WAKEUP,
	FSTATE_ACTIVE,
	FSTATE_POWERING_UP,
	FSTATE_ATTACKING,
};



////////////////////////////////////////////////////////////////////////////////////////
// Global Data 
////////////////////////////////////////////////////////////////////////////////////////
bool	FighterHadDeathScript = false;
bool	FighterActive = false;
vec3_t	AverageEnemyDirection3;
int		AverageEnemyDirectionSamples3;



////////////////////////////////////////////////////////////////////////////////////////
// NPC NEW AGILITY SYSTEM FOR AVOID ATTACKS! 
////////////////////////////////////////////////////////////////////////////////////////

enum	FighterTacticsState
{
	FIGHTER_NONE,

	// Attack 
	//--------
	FIGHTER_CHAFF,	// chaff and flare attack for enemy close 
	FIGHTER_BOMBER,		// bombardment mode against strategics targets  
	FIGHTER_LASER,     // laser standard atk   
	FIGHTER_SEEKER,	// seeker ranged atk  
	FIGHTER_SPECIAL,	// special and super weapons atk by lond distance (cruisers, \ star destroyers ) 
	/// Waiting
	//---------
	FIGHTER_AMBUSHWAIT,		// Goto CP & Wait
	FIGHTER_MAX
};

void Fighter_Precache( void )
{
	G_SoundIndex( "sound/chars/fighter/departure.wav" );
	G_SoundIndex( "sound/chars/fighter/engine_lp.wav" );
	G_SoundIndex( "sound/chars/fighter/landing.wav" );
	G_EffectIndex( "volumetric/black_smoke" );
	G_EffectIndex( "chunks/dustFall" );
	G_SoundIndex( "sound/chars/fighter/misc/fighter_explo" );
	G_SoundIndex( "sound/chars/fighter/misc/fighter_pain" );
	G_SoundIndex( "sound/chars/fighter/misc/fighter_shield_open" );
	G_SoundIndex( "sound/chars/fighter/misc/fighter_shield_close" );
	G_SoundIndex( "sound/chars/fighter/misc/fighter_hover_1_lp" );
	G_SoundIndex( "sound/chars/fighter/misc/fighter_hover_2_lp" );
	G_SoundIndex( "sound/chars/fighter/misc/fighter_explo" );
	G_SoundIndex( "sound/chars/fighter/misc/fighter_pain" );
	for ( int i = 1; i < 4; i++)
	{
		G_SoundIndex( va( "sound/chars/fighter/misc/talk%d", i ) );
	}

	AverageEnemyDirectionSamples3 = 0;
	VectorClear(AverageEnemyDirection3);
	FighterHadDeathScript			= false;
	FighterActive					= true;
}

////////////////////////////////////////////////////////////////////////////////////////
// 
////////////////////////////////////////////////////////////////////////////////////////
void	Fighter_DustFallNear(const vec3_t origin, int dustcount)
{
	if (!FighterActive)
	{
		return;
	}

	trace_t		testTrace;
	vec3_t		testDirection;
	vec3_t		testStartPos;
	vec3_t		testEndPos;

	VectorCopy(origin, testStartPos);
	for (int i=0; i<dustcount; i++)
	{
		testDirection[0] = (random() * 2.0f) - 1.0f;
		testDirection[1] = (random() * 2.0f) - 1.0f;
		testDirection[2] = 1.0f;

		VectorMA(origin, 1000.0f, testDirection, testEndPos);
		gi.trace (&testTrace, origin, NULL, NULL, testEndPos, (player && player->inuse)?(0):(ENTITYNUM_NONE), MASK_SHOT, (EG2_Collision)0, 0 );

		if (!testTrace.startsolid && 
			!testTrace.allsolid && 
			testTrace.fraction>0.1f &&
			testTrace.fraction<0.9f)
		{
			G_PlayEffect( "chunks/dustFall", testTrace.endpos, testTrace.plane.normal );
		}
	}
} 


qboolean Fighter_StopKnockdown( gentity_t *self, gentity_t *pusher, const vec3_t pushDir, qboolean forceKnockdown )
{
	if ( self->client->NPC_class == CLASS_BOBAFETT )// for others, not for boba. 
	{
		return qfalse;
	}

	if ( self->client->moveType == MT_FLYSWIM )
	{//can't knock me down when I'm flying
		return qtrue;
	}

	vec3_t	pDir, fwd, right, ang = {0, self->currentAngles[YAW], 0};
	float	fDot, rDot;
	int		strafeTime = Q_irand( 1000, 2000 );

	AngleVectors( ang, fwd, right, NULL );
	VectorNormalize2( pushDir, pDir );
	fDot = DotProduct( pDir, fwd );
	rDot = DotProduct( pDir, right );

	if ( Q_irand( 0, 2 ) )
	{//flip or roll with it
		usercmd_t	tempCmd;
		if ( fDot >= 0.4f )
		{
			tempCmd.forwardmove = 127;
			TIMER_Set( self, "moveforward", strafeTime );
		}
		else if ( fDot <= -0.4f )
		{
			tempCmd.forwardmove = -127;
			TIMER_Set( self, "moveback", strafeTime );
		}
		else if ( rDot > 0 )
		{
			tempCmd.rightmove = 127;
			TIMER_Set( self, "strafeRight", strafeTime );
			TIMER_Set( self, "strafeLeft", -1 );
		}
		else
		{
			tempCmd.rightmove = -127;
			TIMER_Set( self, "strafeLeft", strafeTime );
			TIMER_Set( self, "strafeRight", -1 );
		}
		G_AddEvent( self, EV_JUMP, 0 );
		if ( !Q_irand( 0, 1 ) )
		{//flip
			self->client->ps.forceJumpCharge = 280;//FIXME: calc this intelligently?
			//ForceJump( self, &tempCmd );
		}
		else
		{//roll
			TIMER_Set( self, "duck", strafeTime );
		}
		self->painDebounceTime = 0;//so we do something
	}
	else
	{//fall down
		return qfalse;
	}

	return qtrue;
}

qboolean Fighter_Flying( gentity_t *self )
{
	assert(self && self->client && self->client->NPC_class==CLASS_FIGHTER ||
		self && self->client && self->client->NPC_class==CLASS_OLON 	);//self->NPC && 
	return ((qboolean)(self->client->moveType==MT_FLYSWIM));
// ONLY FOR FLYING CLASSES 
}

////////////////////////////////////////////////////////////////////////////////////////
// 
////////////////////////////////////////////////////////////////////////////////////////
bool	Fighter_CanSeeEnemy( gentity_t *self )
{
	assert(self && self->NPC && self->client && self->client->NPC_class==CLASS_FIGHTER );
 	return ((level.time - self->NPC->enemyLastSeenTime)<1000);
}

////////////////////////////////////////////////////////////////////////////////////////
// 
////////////////////////////////////////////////////////////////////////////////////////
void	Fighter_Pain( gentity_t *self, gentity_t *inflictor, int damage, int mod)
{
	if ( mod==MOD_SABER || mod !=MOD_SABER /*!(NPCInfo->aiFlags&NPCAI_FLAMETHROW)*/)
	{
		TIMER_Set( self, "NPC_TacticsSelect", 0);	// Hurt By The Saber, Time To Try Something New
	}
	
}

////////////////////////////////////////////////////////////////////////////////////////
// 
////////////////////////////////////////////////////////////////////////////////////////
void Fighter_FlyStart( gentity_t *self )
{//switch to seeker AI for a while
	 	
	
	if ( TIMER_Done( self, "jetRecharge" ) 
		&& !Fighter_Flying( self ) )
	{
		self->client->ps.gravity = 0;
		self->svFlags |= SVF_CUSTOM_GRAVITY;
		self->client->moveType = MT_FLYSWIM;
		//start jet effect
		self->client->jetPackTime = level.time + Q_irand( 3000, 10000 );
			//take-off sound
		G_SoundOnEnt( self, CHAN_ITEM, "sound/chars/fighter/departure.wav" );
		//boost loop sound
		self->s.loopSound = G_SoundIndex( "sound/chars/fighter/engine_lp.wav" );
	}
}

////////////////////////////////////////////////////////////////////////////////////////
// 
////////////////////////////////////////////////////////////////////////////////////////
void Fighter_FlyStop( gentity_t *self )
{
	self->client->ps.gravity = g_gravity->value;
	self->svFlags &= ~SVF_CUSTOM_GRAVITY;
	self->client->moveType = MT_FLYSWIM;
	//Stop effect
	self->client->jetPackTime = 0;
	//stop jet loop sound
	G_SoundOnEnt( self, CHAN_ITEM, "sound/chars/fighter/landing.wav" );
	self->s.loopSound = 0;
	if ( self->NPC )
	{
		self->count = 0; // SEEKER shot ammo count
		TIMER_Set( self, "jetRecharge", Q_irand( 1000, 5000 ) );
		TIMER_Set( self, "jumpChaseDebounce", Q_irand( 500, 2000 ) );
	}
}

////////////////////////////////////////////////////////////////////////////////////////
// 
////////////////////////////////////////////////////////////////////////////////////////
void		Fighter_DoAmbushWait( gentity_t *self)
{
  Fighter_TacticsSelect(); 
}

////////////////////////////////////////////////////////////////////////////////////////
// 
////////////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////////////
// Call This function to make Boba actually shoot his current weapon
////////////////////////////////////////////////////////////////////////////////////////
void	Fighter_Fire()
{
	int			bolt;

	WeaponThink(qtrue);
// Which muzzle to fire from?
	int which = NPCInfo->burstCount % 3;
	switch( which )
	{
	case 0:
		bolt = NPC->handLBolt; 
		break;
	case 1:
		bolt = NPC->handRBolt; 
		break;
	case 2:
	default:
		bolt = NPC->genericBolt3; 
	}
	// If Actually Fired, Decide To Apply Alt Fire And Calc Next Attack Delay
	//------------------------------------------------------------------------
	if (ucmd.buttons&BUTTON_ATTACK)
	{
		// DUSTY ADD - THIS WORK WITHOUT TROUBLE! 

		if (seekerWeap(NPC->s.weapon))
		{
		if (NPC->s.weapon == WP_ROCKET_LAUNCHER|| NPC->s.weapon == WP_BOWCASTER )
				TIMER_Set(NPC, "nextAttackDelay", Q_irand(1000, 2000));

			// Half of your rocket and bowcasters are Homing\Grapple Missile
			//-------------------------------------
			if (!Q_irand(0, 1))
			{
				ucmd.buttons &= ~BUTTON_ATTACK;
				ucmd.buttons |= BUTTON_ALT_ATTACK;
				if (NPC->s.weapon == WP_ROCKET_LAUNCHER)
					{
					NPC->client->fireDelay = Q_irand(1000, 3000);
					}
				else if (NPC->s.weapon == WP_BOWCASTER )
					{
					NPC->client->fireDelay = Q_irand(750, 2000);
					}
			}
						
		}
		else if (chaffWeap(NPC->s.weapon))
		{
		if (NPC->s.weapon == WP_FLECHETTE|| NPC->s.weapon == WP_DEMP2 )
				TIMER_Set(NPC, "nextAttackDelay", Q_irand(1000, 2000));

			// Half of your rocket and bowcasters are Homing\Grapple Missile
			//-------------------------------------
			if (!Q_irand(0, 5))
			{
				ucmd.buttons &= ~BUTTON_ATTACK;
				ucmd.buttons |= BUTTON_ALT_ATTACK;
				if (NPC->s.weapon == WP_FLECHETTE)
					{
					NPC->client->fireDelay = Q_irand(1000, 3000);
					}
				else if (NPC->s.weapon == WP_DEMP2 )
					{
					NPC->client->fireDelay = Q_irand(1000, 1500);
					}
			}
						
		}
		else if (laserWeap(NPC->s.weapon))
		{
		if (NPC->s.weapon == WP_BLASTER|| NPC->s.weapon == WP_BRYAR_PISTOL ||
			NPC->s.weapon == WP_POISON || NPC->s.weapon == WP_JAWA )
				TIMER_Set(NPC, "nextAttackDelay", Q_irand(500, 1000));

			// Laser weapons
			//-------------------------------------
			if (!Q_irand(0, 6))
			{
				ucmd.buttons &= ~BUTTON_ATTACK;
				ucmd.buttons |= BUTTON_ALT_ATTACK;
				if (NPC->s.weapon == WP_BLASTER_PISTOL ||
					NPC->s.weapon == WP_BRYAR_PISTOL ) // allied and enemy stunner lasers
					{
						NPC->client->fireDelay = Q_irand(250, 500);
					}
				else if (NPC->s.weapon == WP_BLASTER || WP_JAWA )// medium lasers
					{
						NPC->client->fireDelay = Q_irand(300, 750);
					}
			}
						
		}
		else if (bombsWeap(NPC->s.weapon))
		{
		if (NPC->s.weapon == WP_REPEATER|| NPC->s.weapon == WP_THERMAL ||
			NPC->s.weapon == WP_POISON )
				TIMER_Set(NPC, "nextAttackDelay", Q_irand(1000, 1500));

			// These weapons are shooted ever with half fire. Ever! 
			//-------------------------------------
			NPCInfo->scriptFlags |= SCF_ALT_FIRE;
		
		}		
		else if (specialWeap(NPC->s.weapon))
		{
		if (NPC->s.weapon == WP_CONCUSSION|| NPC->s.weapon == WP_DISRUPTOR ||
			NPC->s.weapon == WP_ATST_MAIN || NPC->s.weapon == WP_ATST_SIDE ||
			NPC->s.weapon == WP_EMPLACED_GUN	 )
				TIMER_Set(NPC, "nextAttackDelay", Q_irand(1500, 2000));

			// Special destructive massival weapons
			//-------------------------------------
			if (!Q_irand(0, 5))// very rare alt fire 
			{
				ucmd.buttons &= ~BUTTON_ATTACK;
				ucmd.buttons |= BUTTON_ALT_ATTACK;
				if (NPC->s.weapon == WP_CONCUSSION ) // allied and enemy stunner lasers
					{
						NPC->client->fireDelay = Q_irand(1000, 5000);
					}
				else if (NPC->s.weapon == WP_DISRUPTOR )// medium lasers
					{
						NPC->client->fireDelay = Q_irand(1000, 2000);
					}
				else if (NPC->s.weapon == WP_ATST_MAIN || WP_ATST_SIDE ) // strong lasers 
					{
						NPC->client->fireDelay = Q_irand(1000, 2000);
					}
				else if (NPC->s.weapon == WP_EMPLACED_GUN ) // strong lasers 
					{
						NPC->client->fireDelay = Q_irand(750, 850);
					}

		}	
		else
		{//blaster-type weapon

			if (TIMER_Done(NPC, "nextBlasterAltFireDecide"))
			{
 			 	if (Q_irand(0, (NPC->count * 2) + 3)>2)
				{
			 		TIMER_Set(NPC, "nextBlasterAltFireDecide", Q_irand(3000, 8000));
					if (!(NPCInfo->scriptFlags&SCF_ALT_FIRE))
					{
//						//NPC_Printf("ALT FIRE On");
						NPCInfo->scriptFlags |= SCF_ALT_FIRE;
						NPC_ChangeWeapon(NPC->s.weapon);					// Update Delay Timers
					}
				}
				else
				{
					TIMER_Set(NPC, "nextBlasterAltFireDecide", Q_irand(2000, 5000));
					if ( (NPCInfo->scriptFlags&SCF_ALT_FIRE))
					{
//						//NPC_Printf("ALT FIRE Off");
						NPCInfo->scriptFlags &= ~SCF_ALT_FIRE;
						NPC_ChangeWeapon(NPC->s.weapon);			// Update Delay Timers
					}
				}
			}

			// Occasionally Alt Fire
			//-----------------------
			if ((NPCInfo->scriptFlags&SCF_ALT_FIRE))
			{
				ucmd.buttons &= ~BUTTON_ATTACK;
				ucmd.buttons |= BUTTON_ALT_ATTACK;
			}
		}
	}
  }
}



////////////////////////////////////////////////////////////////////////////////////////
// Call this function to see if Fett should fire his current weapon
////////////////////////////////////////////////////////////////////////////////////////
//helper function
qboolean isFighterClass(int className)
{
	if (NPC->client->NPC_class == CLASS_FIGHTER )// Add other special classes )
		return qtrue;

	return qfalse;
}

void Fighter_FireDecide( void )
{
	// Any Reason Not To Shoot?
	//--------------------------
	if (!NPC ||											// Only NPCs
		!NPC->client ||									// Only Clients
		 //NPC->client->NPC_class!=CLASS_BOBAFETT ||		// Only Boba
		!isFighterClass(NPC->client->NPC_class) ||		    // Only Boba 
		!NPC->enemy ||									// Only If There Is An Enemy
		NPC->s.weapon == WP_NONE ||				// Only If Using A Valid Weapon
		//!TIMER_Done(NPC, "nextAttackDelay") ||			// Only If Ready To Shoot Again
		!Fighter_CanSeeEnemy(NPC)		// Only If Enemy Recently Seen
		)
	{
		return;
	}

	// Now Check Weapon Specific Parameters To See If We Should Shoot Or Not
	//-----------------------------------------------------------------------
	// GOOD CODE 
	// DUSTY VARIANTE 
	if (laserWeap(NPC->s.weapon) || bombsWeap(NPC->s.weapon) ||  
		chaffWeap(NPC->s.weapon) || seekerWeap(NPC->s.weapon)||
		specialWeap(NPC->s.weapon))
	{
	
	// WHEN FIRE? 
	if (Distance(NPC->currentOrigin, NPC->enemy->currentOrigin) > 100.0f)
		{
			Fighter_Fire();
		}
	    else 
		{
			Fighter_Fire();
		}
	}
	else
	{
		Fighter_Fire();
	}
}

////////////////////////////////////////////////////////////////////////////////////////
// Tactics avaliable to Boba Fett:
// --------------------------------
//	FIGHTER_LASER,			// Uses laser weapons and close \ medium combat
//	FIGHTER_BOMBER,		// Close / medium distance, bombardment. 
//	FIGHTER_CHAFF 		// Defensive action for close combat 
//  FIGHTER_SEEKER, 	// medium \ long range seeker and rockets
//  FIGHTER_SPECIAL,	// special weapons for cruisers, turbolasers, turrets etc  
//	FIGHTER_AMBUSHWAIT,	// Goto CP & Wait
//
extern void WP_DeactivateSaber( gentity_t *self, qboolean clearLength );
////////////////////////////////////////////////////////////////////////////////////////
void	Fighter_TacticsSelect()
{
	// Don't Change Tactics For A Little While
	//------------------------------------------
	TIMER_Set(NPC, "NPC_TacticsSelect", Q_irand(8000, 15000));
	int		nextState = NPCInfo->localState;


	// Get Some Data That Will Help With The Selection Of The Next Tactic
	//--------------------------------------------------------------------
	bool	enemyAlive3			= (NPC->enemy->health>0);
	float	fighterDistance		= Distance(NPC->currentOrigin, NPC->enemy->currentOrigin);
	bool	enemyChaffRange	= (fighterDistance<FIGHTER_SHOOTRANGEMIN);
	bool	enemyBombRange	= (fighterDistance>FIGHTER_SHOOTRANGEMIN && fighterDistance<FIGHTER_SHOOTRANGEMED);
	bool	enemyLaserRange	= (fighterDistance>FIGHTER_SHOOTRANGEMED && fighterDistance<FIGHTER_SHOOTRANGEMAX);
	bool	enemySeekRange	= (fighterDistance>FIGHTER_SHOOTRANGEMAX && fighterDistance<FIGHTER_SPECIALRANGE);
	bool	enemySpecialRange = (fighterDistance>FIGHTER_SPECIALRANGE);

	bool	enemyRecentlySeen3	= Fighter_CanSeeEnemy(NPC);
		
	if (!enemyAlive3)
	{
		nextState = FIGHTER_LASER;
	}
	else if (enemyChaffRange)// Enemy is pretty near, so use Saber!  
	{
		if ( HaveWeapon(NPC, WEAPS_CHAFF ))// If Enemy is near, and NPC have a sword in his inventory, using that sword. 
		{
			nextState = FIGHTER_CHAFF;
		}
		else if ( HaveWeapon(NPC, WEAPS_LASER ))// If Enemy is near, and NPC have a sword in his inventory, using that sword. 
		{
			nextState = FIGHTER_LASER;
		}
	}
	
	else if (enemyBombRange)// Enemy is distance enought to bombardment. on alternative, you can use lasers.
	{
		
		if ( HaveWeapon(NPC, WEAPS_BOMBS ))// Enemy is distance, you are a wizard? shoot with magic 
		{
			nextState = FIGHTER_BOMBER;
		}
		else if ( HaveWeapon(NPC, WEAPS_LASER ))// Enemy is distance? You are an Hunter? use glyphic weapons 
		{
			nextState = FIGHTER_LASER;
		}
	}	
	else if (enemyLaserRange)// Enemy is distance so use shoot atk
	{
		
		if ( HaveWeapon(NPC, WEAPS_LASER ))// Enemy is distance, you are a wizard? shoot with magic 
		{
			nextState = FIGHTER_LASER;
		}
		else if ( HaveWeapon(NPC, WEAPS_SEEKER ))// Enemy is distance? You are an Hunter? use glyphic weapons 
		{
			nextState = FIGHTER_SEEKER;
		}
	}
	else if (enemySeekRange)// Enemy is distance so use shoot atk
	{
		
		if ( HaveWeapon(NPC, WEAPS_LASER ))// Enemy is distance, you are a wizard? shoot with magic 
		{
			nextState = FIGHTER_LASER;
		}
		else if ( HaveWeapon(NPC, WEAPS_SEEKER ))// Enemy is distance? You are an Hunter? use glyphic weapons 
		{
			nextState = FIGHTER_SEEKER;
		}
	}	
	else if (!enemyBombRange && !enemyChaffRange && !enemyLaserRange && !enemySeekRange) // Enemy is pretty far, try distance atk
	{
		// If It's Been Long Enough Since Our Last Flame Blast, Try To Torch The Enemy
		//-----------------------------------------------------------------------------
		//if (TIMER_Done(NPC, "nextFlameDelay"))
		// DUSTY ADD
		if ( HaveWeapon(NPC, WEAPS_SPECIAL ))// Enemy is distance? You are an Hunter? use glyphic weapons 
		{
			nextState = FIGHTER_SPECIAL;
		}
		else if ( HaveWeapon(NPC, WEAPS_BLASTER ))// Enemy is distance, you are a wizard? shoot with magic 
		{
			nextState = FIGHTER_LASER;
		}
		else if ( HaveWeapon(NPC, WEAPS_SEEKER ))// You are an archer? use arrows!  
		{
			nextState = FIGHTER_SEEKER;
		}
	}


	// Recently Saw The Enemy, Time For Some Good Ole Fighten!
	//---------------------------------------------------------
	/*else if (enemyRecentlySeen3)
	{
		// At First, Boba will prefer to use his blaster against the player, but
		//  the more times he is driven away (NPC->count), he will be less likely to
		//  choose the blaster, and more likely to go for the missile launcher
		if (!enemyChaffRange && NPC->client->ps.weapon != WP_FLECHETTE )
		{
		  nextState = FIGHTER_LASER; 
		}
		nextState = (!enemyChaffRange || Q_irand(0, NPC->count)<1)?(FIGHTER_CHAFF):(FIGHTER_SEEKER);  
		


	}*/

	
	NPCInfo->localState = nextState;
	int weapon = 0;

	switch (NPCInfo->localState)
 	{
			
	case FIGHTER_LASER:	
		weapon = ChooseWeaponRandom(NPC, WEAPS_LASER);
		if (weapon)
		{
 			NPC_ChangeWeapon(weapon);
			//NPC_ChangeWeapon(WEAPS_LASER);
			G_RemoveWeaponModels( NPC );
			G_RemoveGunModels( NPC );
 			break;
		}
	case FIGHTER_CHAFF:
		weapon = ChooseWeaponRandom(NPC, WEAPS_CHAFF);
		if (weapon)
		{
 			//NPC_ChangeWeapon(WEAPS_CHAFF);
			NPC_ChangeWeapon(weapon);
			G_RemoveWeaponModels( NPC );
			G_RemoveGunModels( NPC );
 			break;
		}
		
	case FIGHTER_BOMBER: //kinda stuck at this point if doesn't have flamethrower
		//NPC_Printf("NEW TACTIC: Flame Thrower");
		weapon = ChooseWeaponRandom(NPC, WEAPS_BOMBS);
		if (weapon)
		{
 			NPC_ChangeWeapon(weapon);
			//NPC_ChangeWeapon(WEAPS_BOMBS);
			G_RemoveWeaponModels( NPC );
			G_RemoveGunModels( NPC );
 			break;
		}
	case FIGHTER_SPECIAL: //kinda stuck at this point if doesn't have flamethrower
		//NPC_Printf("NEW TACTIC: Flame Thrower");
		weapon = ChooseWeaponRandom(NPC, WEAPS_SPECIAL);
		if (weapon)
		{
 			NPC_ChangeWeapon(weapon);
			//NPC_ChangeWeapon(WEAPS_SPECIAL);
			G_RemoveWeaponModels( NPC );
			G_RemoveGunModels( NPC );
 			break;
		}
	case FIGHTER_SEEKER: //kinda stuck at this point if doesn't have flamethrower
		//NPC_Printf("NEW TACTIC: Flame Thrower");
		weapon = ChooseWeaponRandom(NPC, WEAPS_SEEKER);
		if (weapon)
		{
 			NPC_ChangeWeapon(weapon);
			//NPC_ChangeWeapon(WEAPS_SEEKER);
			G_RemoveWeaponModels( NPC );
			G_RemoveGunModels( NPC );
 			break;
		}
		
	case FIGHTER_AMBUSHWAIT:
		//NPC_Printf("NEW TACTIC: Ambush");
		weapon = ChooseWeaponRandom(NPC, WEAPS_ALL);
		if (weapon)
		{
 			//NPC_Printf("NEW TACTIC: Rocket Launcher");
			NPC_ChangeWeapon(weapon);
			G_RemoveWeaponModels( NPC );
			G_RemoveGunModels( NPC );
 			break;
		}
	}	


/////// Fighter fly behavours/////////////////////////////////////////////////////////
	float	fighterflightDistance		= Distance(NPC->currentOrigin, NPC->enemy->currentOrigin);
	bool	fighterEvasionRange	= (fighterflightDistance<FIGHTER_SHOOTRANGEMIN); // If enemy is Near, try some evasion shooting chaffs
	bool	fighterRoundRange	= (fighterDistance>FIGHTER_SHOOTRANGEMIN && fighterDistance<FIGHTER_SHOOTRANGEMED); // if enemy is close, fly round him shoot him with laser 
	bool	fighterBrakeRange	= (fighterDistance>FIGHTER_SHOOTRANGEMED && fighterDistance<FIGHTER_SHOOTRANGEMAX);// if you are a bomber nd you are sufficiently far from target. stay on target nnd bombarding. 



	// Evasion: (only for fighter, not for cruisers or big ships) if enemy is shooted by player flee with evasion manouvre (rolling, strafing)

}

////////////////////////////////////////////////////////////////////////////////////////
// Tactics
//
// This function is called right after Update()
// If returns true, Jedi and Seeker AI not used for movement
//
////////////////////////////////////////////////////////////////////////////////////////
bool	Fighter_Tactics()
{
	if (!NPC->enemy)
	{
		return false;
	}

	// Think About Changing Tactics
	//------------------------------
	if (TIMER_Done(NPC, "NPC_TacticsSelect"))
	{
		Fighter_TacticsSelect();
	}

	// These Tactics Require Seeker & Jedi Movement
	//----------------------------------------------
	if (!NPCInfo->localState ||
		 NPCInfo->localState==FIGHTER_LASER || 
		 NPCInfo->localState==FIGHTER_CHAFF ||
		 NPCInfo->localState==FIGHTER_BOMBER|| 
		 NPCInfo->localState==FIGHTER_SEEKER ||
		 NPCInfo->localState==FIGHTER_SPECIAL )
	{
		return false;
	}

	// Flame Thrower - Locked In Place
	//---------------------------------
	if (NPCInfo->localState==FIGHTER_BOMBER ||
		NPCInfo->localState==FIGHTER_CHAFF ||
		 NPCInfo->localState==FIGHTER_LASER || 
		 NPCInfo->localState==FIGHTER_SEEKER ||
		 NPCInfo->localState==FIGHTER_SPECIAL )
	{
		Fighter_TacticsSelect();
	}

	// Ambush Wait
	//------------
	else if (NPCInfo->localState==FIGHTER_AMBUSHWAIT)
	{
		Fighter_DoAmbushWait( NPC );
	}


	NPC_FacePosition( NPC->enemy->currentOrigin, qtrue);
	// chase enemy 
	NPC_UpdateAngles(qtrue, qtrue);

	return true;			// Do Not Use Normal Jedi Or Seeker Movement
}


////////////////////////////////////////////////////////////////////////////////////////
// 
////////////////////////////////////////////////////////////////////////////////////////
void	Fighter_Update()
{
	
	// Hey, That Tests The Trace All The Time
	//----------------------------------------------------
	if (NPC->enemy)
	{
		// Never Forget The Player... Never. NEVER!!! 
		NPC->svFlags				|= SVF_LOCKEDENEMY;	// Don't forget about the enemy once you've found him
		if (!(NPC->svFlags&SVF_NOCLIENT))
		{
			trace_t		testTrace;
			vec3_t		eyes;
			CalcEntitySpot( NPC, SPOT_HEAD_LEAN, eyes );
			gi.trace (&testTrace, eyes, NULL, NULL, NPC->enemy->currentOrigin, NPC->s.number, MASK_SHOT, (EG2_Collision)0, 0);

			bool	wasSeen = Fighter_CanSeeEnemy(NPC);

			if (!testTrace.startsolid && 
				!testTrace.allsolid && 
				testTrace.entityNum == NPC->enemy->s.number)
			{
				NPCInfo->enemyLastSeenTime	= level.time;
				NPCInfo->enemyLastHeardTime	= level.time;
				VectorCopy(NPC->enemy->currentOrigin, NPCInfo->enemyLastSeenLocation);
				VectorCopy(NPC->enemy->currentOrigin, NPCInfo->enemyLastHeardLocation);
			}
			else if (gi.inPVS( NPC->enemy->currentOrigin, NPC->currentOrigin))
			{
				NPCInfo->enemyLastHeardTime	= level.time;
				VectorCopy(NPC->enemy->currentOrigin, NPCInfo->enemyLastHeardLocation);
			}
		}
	}


	// Make Sure He Always Appears In The Last Area With Full Health When His Death Script Is Turned On
	//--------------------------------------------------------------------------------------------------
	if (!FighterHadDeathScript && NPC->behaviorSet[BSET_DEATH]!=0)
	{
		if (!gi.inPVS(NPC->enemy->currentOrigin, NPC->currentOrigin))
		{
			//NPC_Printf("Attempting Final Battle Spawn...");
			/*if (Fighter_Respawn())
			{
				FighterHadDeathScript = true;
			}
			else
			{
				//NPC_Printf("Failed");
			}*/
		}
	}
	// Occasionally A Jump Turns Into A Rocket Fly
	//---------------------------------------------
	if ( NPC->client->ps.groundEntityNum == ENTITYNUM_NONE 
		&& NPC->client->ps.forceJumpZStart && NPC->client->NPC_class == CLASS_FIGHTER
		&& !Q_irand( 0, 10 ))
	{//take off
		Fighter_FlyStart( NPC );
	}
	
	// EDIT THIS WITH A FUNCTION FOR TAKE EVER A GOOD DISTANCE BY GROUND 
	// CALL FIGHTER MAINTAINE HEIGHT 
	// ADD 

	// If Hurting, Try To Run Away
	//-----------------------------
	if (!NPCInfo->surrenderTime && (NPC->health<NPC->max_health/10))
	{
		// Find The Closest Flee Point That I Can Get To
		//-----------------------------------------------
		int cp = NPC_FindCombatPoint(NPC->currentOrigin, 0, NPC->currentOrigin, CP_FLEE|CP_HAS_ROUTE|CP_TRYFAR|CP_HORZ_DIST_COLL, 0, -1);
		if (cp!=-1)
		{
			NPC_SetCombatPoint( cp );
			NPC_SetMoveGoal( NPC, level.combatPoints[cp].origin, 8, qtrue, cp );
			if (NPC->count<6)
			{
 	 		 	NPCInfo->surrenderTime = level.time + Q_irand(5000, 10000) + 1000*(6-NPC->count);
			}
			else
			{
 	 			NPCInfo->surrenderTime = level.time + Q_irand(5000, 10000);
			}
		}
	}
}




////////////////////////////////////////////////////////////////////////////////////////
// 
////////////////////////////////////////////////////////////////////////////////////////
bool	Fighter_Flee()
{
	bool	EnemyRecentlySeen	= ((level.time - NPCInfo->enemyLastSeenTime)<10000);
	bool	ReachedEscapePoint	= (Distance(level.combatPoints[NPCInfo->combatPoint].origin, NPC->currentOrigin)<50.0f);
	bool	HasBeenGoneEnough	= (level.time>NPCInfo->surrenderTime || (level.time - NPCInfo->enemyLastSeenTime)>400000);


	// Is It Time To Come Back For Some More?
	//----------------------------------------
 	if (!EnemyRecentlySeen || ReachedEscapePoint)
	{
		NPC->svFlags |= SVF_NOCLIENT;
		if (HasBeenGoneEnough)
		{
			if ((level.time - NPCInfo->enemyLastSeenTime)>400000)
			{
				//NPC_Printf("  Gone Too Long, Attempting Respawn");
			}
		}
	  	else if (ReachedEscapePoint && (NPCInfo->surrenderTime - level.time)>3000)
		{
 			if (TIMER_Done(NPC, "SpookPlayerTimer"))
			{
				vec3_t		testDirection;
				TIMER_Set(NPC, "SpookPlayerTimer", Q_irand(2000, 10000));
				switch(Q_irand(0, 1))
				{
				case 0:
					//NPC_Printf("SPOOK: Dust");
					Fighter_DustFallNear(NPC->enemy->currentOrigin, Q_irand(1,2));
					break;

				case 1:
					//NPC_Printf("SPOOK: Footsteps");
  					testDirection[0] =  (random() * 0.5f) - 1.0f;
	 		 		testDirection[0] += (testDirection[0]>0.0f)?(0.5f):(-0.5f);
					testDirection[1] = (random() * 0.5f) - 1.0f;
					testDirection[1] += (testDirection[1]>0.0f)?(0.5f):(-0.5f);
					testDirection[2] = 1.0f;
		 	 		break;
				}
			}

		 	if (TIMER_Done(NPC, "ResampleEnemyDirection") && NPC->enemy->resultspeed>10.0f)
			{
				TIMER_Set(NPC, "ResampleEnemyDirection", Q_irand(500, 1000));
				AverageEnemyDirectionSamples3 ++;

				vec3_t	moveDir;
				VectorCopy(NPC->enemy->client->ps.velocity, moveDir);
				VectorNormalize(moveDir);

				VectorAdd(AverageEnemyDirection3, moveDir, AverageEnemyDirection3);
			}
		}
	}
	else
	{
		NPCInfo->surrenderTime += 100;
	}

	// Finish The Flame Thrower First...
	// FLAMETHROWER 
	//-----------------------------------
	/*if (NPCInfo->aiFlags&NPCAI_FLAMETHROW)
	{
		Fighter_TacticsSelect();
		NPC_FacePosition( NPC->enemy->currentOrigin, qtrue);
		NPC_UpdateAngles(qtrue, qtrue);
		return true;
	}*/

	bool	IsOnAPath = !!NPC_MoveToGoal(qtrue);
	if (!ReachedEscapePoint &&
		NPCInfo->aiFlags&NPCAI_BLOCKED && 
		NPC->client->moveType!=MT_FLYSWIM && 
		((level.time - NPCInfo->blockedDebounceTime)>1000)
		)
	{
		if (!Fighter_CanSeeEnemy(NPC) && Distance(NPC->currentOrigin, level.combatPoints[NPCInfo->combatPoint].origin)<200)
		{
			//NPC_Printf("BLOCKED: Just Teleporting There");
			G_SetOrigin(NPC, level.combatPoints[NPCInfo->combatPoint].origin);
		}
		else
		{
			//NPC_Printf("BLOCKED: Attempting Jump");

			if (IsOnAPath)
			{
				if (NPC_TryJump(NPCInfo->blockedTargetPosition))
				{
				}
				else
				{
					//NPC_Printf("  Failed");
				}
			}
			else if (EnemyRecentlySeen)
			{
				if (NPC_TryJump(NPCInfo->enemyLastSeenLocation))
				{
	 			}
				else
				{
					//NPC_Printf("  Failed");
				}
			}
		}
	}


	NPC_UpdateAngles( qtrue, qtrue );
	return true;
}


///////////////////////// BY SENTRY CODE FLY FUNCTIONS//////////////////////////

void Fighter_MaintainHeight( void )
{	
	float	dif;

	NPC->s.loopSound = G_SoundIndex( "sound/chars/Fighter/misc/fighter_hover_1_lp" );

	// Update our angles regardless
	NPC_UpdateAngles( qtrue, qtrue );

	// If we have an enemy, we should try to hover at about enemy eye level
	if ( NPC->enemy )
	{
		// Find the height difference
		dif = (NPC->enemy->currentOrigin[2]+NPC->enemy->maxs[2]) - NPC->currentOrigin[2]; 

		// cap to prevent dramatic height shifts
		if ( fabs( dif ) > 8 )
		{
			if ( fabs( dif ) > FIGHTER_HOVER_HEIGHT )
			{
				dif = ( dif < 0 ? -24 : 24 );
			}

			NPC->client->ps.velocity[2] = (NPC->client->ps.velocity[2]+dif)/2;
		}
	}
	else
	{
		gentity_t *goal = NULL;

		if ( NPCInfo->goalEntity )	// Is there a goal?
		{
			goal = NPCInfo->goalEntity;
		}
		else
		{
			goal = NPCInfo->lastGoalEntity;
		}

		if (goal)
		{
			dif = goal->currentOrigin[2] - NPC->currentOrigin[2];

			if ( fabs( dif ) > FIGHTER_HOVER_HEIGHT )
			{
				ucmd.upmove = ( ucmd.upmove < 0 ? -4 : 4 );
			}
			else
			{
				if ( NPC->client->ps.velocity[2] )
				{
					NPC->client->ps.velocity[2] *= FIGHTER_VELOCITY_DECAY;

					if ( fabs( NPC->client->ps.velocity[2] ) < 2 )
					{
						NPC->client->ps.velocity[2] = 0;
					}
				}
			}
		}
		// Apply friction to Z
		else if ( NPC->client->ps.velocity[2] )
		{
			NPC->client->ps.velocity[2] *= FIGHTER_VELOCITY_DECAY;

			if ( fabs( NPC->client->ps.velocity[2] ) < 1 )
			{
				NPC->client->ps.velocity[2] = 0;
			}
		}
	}

	// Apply friction
	if ( NPC->client->ps.velocity[0] )
	{
		NPC->client->ps.velocity[0] *= FIGHTER_VELOCITY_DECAY;

		if ( fabs( NPC->client->ps.velocity[0] ) < 1 )
		{
			NPC->client->ps.velocity[0] = 0;
		}
	}

	if ( NPC->client->ps.velocity[1] )
	{
		NPC->client->ps.velocity[1] *= FIGHTER_VELOCITY_DECAY;

		if ( fabs( NPC->client->ps.velocity[1] ) < 1 )
		{
			NPC->client->ps.velocity[1] = 0;
		}
	}

	NPC_FaceEnemy( qtrue );
}

/*
-------------------------
Fighter_Idle
-------------------------
*/
void Fighter_Idle( void )
{
	/*float	distance, speed;
	vec3_t	forward;
	Fighter_MaintainHeight();
	VectorSubtract( NPC->enemy->currentOrigin, NPC->currentOrigin, forward );
	distance = VectorNormalize( forward );
	speed = 0;*/
	
	// Is he waking up?
	if (NPCInfo->localState == FSTATE_WAKEUP)
	{
		if (NPC->client->ps.torsoAnimTimer<=0)
		{
			NPCInfo->scriptFlags |= SCF_LOOK_FOR_ENEMIES;
			NPCInfo->burstCount = 0;
		}
	}
	else
	{
		NPC_SetAnim( NPC, SETANIM_BOTH, BOTH_SWIM_IDLE1, SETANIM_FLAG_OVERRIDE|SETANIM_FLAG_HOLD );
		NPC->flags |= FL_SHIELDED;

		
		NPC_BSIdle();
	}
}

/*
-------------------------
Fighter_Strafe
-------------------------
*/
void Fighter_Strafe( void )
{
	int		dir;
	vec3_t	end, right;
	trace_t	tr;

	AngleVectors( NPC->client->renderInfo.eyeAngles, NULL, right, NULL );

	// Pick a random strafe direction, then check to see if doing a strafe would be
	//	reasonable valid
	dir = ( rand() & 1 ) ? -1 : 1;
	VectorMA( NPC->currentOrigin, FIGHTER_STRAFE_DIS * dir, right, end );

	gi.trace( &tr, NPC->currentOrigin, NULL, NULL, end, NPC->s.number, MASK_SOLID, (EG2_Collision)0, 0 );

	// Close enough
	if ( tr.fraction > 0.9f )
	{
		VectorMA( NPC->client->ps.velocity, FIGHTER_STRAFE_DIS * dir, right, NPC->client->ps.velocity );

		// Add a slight upward push
		NPC->client->ps.velocity[2] += FIGHTER_UPWARD_PUSH;

		// Set the strafe start time so we can do a controlled roll
		NPC->fx_time = level.time;
		NPCInfo->standTime = level.time + 3000 + random() * 500;
	}
}

/*
-------------------------
Fighter_Steer
-------------------------
*/

#define			FIGHTER_ATTACK_FWD				0.95f
#define			FIGHTER_ATTACK_SIDE				0.20f
#define			FIGHTER_AIM_SIDE				0.60f
#define			FIGHTER_FUTURE_PRED_DIST		20.0f
#define			FIGHTER_FUTURE_SIDE_DIST		60.0f
#define			FIGHTER_ATTACK_FLANK_SLOWING	1000.0f
#define			FIGHTER_RAM_DIST				150.0f
#define			FIGHTER_MIN_STAY_VIEWABLE_TIME	20000
extern bool	VEH_StartStrafeRam(Vehicle_t *pVeh, bool Right);
void	Fighter_Steer()
{
	if (!NPC->enemy || !NPC->enemy->client)
	{
		return;
	}
	
// SETUP
//=======
	// Setup Fighter Data
	//------------------
	CVec3		FighterPos(NPC->currentOrigin);
	CVec3		FighterAngles(NPC->currentAngles);
				FighterAngles[2]	= 0;
	Vehicle_t*	FighterVeh		= NPCInfo->greetEnt->m_pVehicle;
	bool		FighterInTurbo	= (FighterVeh->m_iTurboTime>level.time);
	float		FighterSpeed		= (FighterVeh)?(VectorLength(FighterVeh->m_pParentEntity->client->ps.velocity)):(NPC->client->ps.speed);

	
	// If my vehicle is spinning out of control, just hold on, we're going to die!!!!!
	//---------------------------------------------------------------------------------
	/*if (NPC->health <=1)
	{// too damaged for combat? 
		VEH_OUTOFCONTROL; 
		if (NPC->client->ps.weapon!=WP_NONE)
		{
			NPC_ChangeWeapon(WP_NONE);
		}
		ucmd.buttons	&=~BUTTON_ATTACK;
		ucmd.buttons	&=~BUTTON_ALT_ATTACK;
		return;
	}*/	
		
		
		/*
	if ( FighterVeh && (FighterVeh->m_ulFlags & VEH_OUTOFCONTROL))
	{
		if (NPC->client->ps.weapon!=WP_NONE)
		{
			NPC_ChangeWeapon(WP_NONE);
		}
		ucmd.buttons	&=~BUTTON_ATTACK;
		ucmd.buttons	&=~BUTTON_ALT_ATTACK;
		return;
	}*/

	CVec3		FighterDirection;
				AngleVectors(FighterAngles.v, FighterDirection.v, 0, 0);

	CVec3		FighterFuturePos(FighterPos);
				FighterFuturePos.ScaleAdd(FighterDirection, FIGHTER_FUTURE_PRED_DIST);

	bool		FighterDoTurbo	= false;
	bool		FighterAccelerate	= false;
	bool		FighterAimAtTarget= true;
	float		FighterYawOffset	= 0.0f;

	
	// Setup Enemy Data
	//------------------
	CVec3		EnemyPos(NPC->enemy->currentOrigin);
	CVec3		EnemyAngles(NPC->enemy->currentAngles);
				EnemyAngles[2]	= 0;
	Vehicle_t*	EnemyVeh		= (NPC->enemy->s.m_iVehicleNum)?(g_entities[NPC->enemy->s.m_iVehicleNum].m_pVehicle):(0);
	bool		EnemyInTurbo	= (EnemyVeh && EnemyVeh->m_iTurboTime>level.time);
	float		EnemySpeed		= (EnemyVeh)?(EnemyVeh->m_pParentEntity->client->ps.speed):(NPC->enemy->resultspeed);
	bool		EnemySlideBreak	= (EnemyVeh && (EnemyVeh->m_ulFlags&VEH_SLIDEBREAKING || EnemyVeh->m_ulFlags&VEH_STRAFERAM));
	bool		EnemyDead		= (NPC->enemy->health<=0);

	bool		FighterFlank		= (NPCInfo->lastAvoidSteerSideDebouncer>level.time && EnemyVeh && EnemySpeed>10.0f);

	CVec3		EnemyDirection;
	CVec3		EnemyRight;
				AngleVectors(EnemyAngles.v, EnemyDirection.v, EnemyRight.v, 0);

	CVec3		EnemyFuturePos(EnemyPos);
				EnemyFuturePos.ScaleAdd(EnemyDirection, FIGHTER_FUTURE_PRED_DIST);

	ESide		EnemySide		= FighterPos.LRTest(EnemyPos, EnemyFuturePos);
	CVec3		EnemyFlankPos(EnemyFuturePos);
				EnemyFlankPos.ScaleAdd(EnemyRight, (EnemySide==Side_Right)?(FIGHTER_FUTURE_SIDE_DIST):(-FIGHTER_FUTURE_SIDE_DIST));

	// Debug Draw Enemy Data
	//-----------------------
	if (false)
	{
		CG_DrawEdge(EnemyPos.v,			EnemyFuturePos.v, EDGE_IMPACT_SAFE);
		CG_DrawEdge(EnemyFuturePos.v,	EnemyFlankPos.v, EDGE_IMPACT_SAFE);
	}

	
	// Setup Move And Aim Directions
	//-------------------------------
	CVec3		MoveDirection((FighterFlank)?(EnemyFlankPos):(EnemyFuturePos));
				MoveDirection	-= FighterPos;
	float		MoveDistance	= MoveDirection.SafeNorm();
	float		MoveAccuracy	= MoveDirection.Dot(FighterDirection);

	CVec3		AimDirection(EnemyPos);
				AimDirection	-= FighterPos;
	float		AimDistance		= AimDirection.SafeNorm();
	float		AimAccuracy		= AimDirection.Dot(FighterDirection);


	
	if (!FighterFlank && TIMER_Done(NPC, "FlankAttackCheck"))
	{
		TIMER_Set(NPC, "FlankAttackCheck", Q_irand(1000, 3000));
		if (MoveDistance<4000 && Q_irand(0, 1)==0)
		{
			NPCInfo->lastAvoidSteerSideDebouncer	= level.time + Q_irand(8000, 14000);
		}
	}


	// Fly By Sounds
	//---------------
	if (EnemySpeed<100.0f || (FighterDirection.Dot(EnemyDirection)*(MoveDistance/800.0f))<-0.5f)
	{
			G_SoundOnEnt( NPC, CHAN_ITEM, "sound/chars/fighter/flyby.mp3" );
	}
	

	
	
// FLY PAST BEHAVIOR
//===================
 	if (EnemySlideBreak || !TIMER_Done(NPC, "MinHoldDirectionTime"))
	{
		// O QUESTO? 
		if (TIMER_Done(NPC, "MinHoldDirectionTime"))
		{
			TIMER_Set(NPC, "MinHoldDirectionTime", 500);	// Hold For At Least 500 ms
		}
		FighterAccelerate		= true;							// Go
		FighterAimAtTarget	= true;						// Don't Alter Our Aim Direction
		ucmd.buttons		&=~BUTTON_VEH_SPEED;			// Let Normal Vehicle Controls Go
	}
	

	// CRASHA! 
	 
// FLANKING BEHAVIOR
//===================
	else if (FighterFlank)
	{
  		FighterAccelerate	= true;
		FighterDoTurbo	= (MoveDistance>2500 || EnemyInTurbo);
		ucmd.buttons	|= BUTTON_VEH_SPEED;			// Tells PMove to use the ps.speed we calculate here, not the one from g_vehicles.c

		
		// For Flanking, We Calculate The Speed By Hand, Rather Than Using Pure Accelerate / No Accelerate Functionality
		//---------------------------------------------------------------------------------------------------------------
		// QUESTA CRASHA, CAMBIALA. 	
		NPC->client->ps.speed += EnemySpeed;
			
		// If In Slowing Distance, Scale Down The Speed As We Approach Our Move Target
		//-----------------------------------------------------------------------------
		if (MoveDistance<FIGHTER_ATTACK_FLANK_SLOWING)
		{
			NPC->client->ps.speed *= (MoveDistance/FIGHTER_ATTACK_FLANK_SLOWING);
			NPC->client->ps.speed += EnemySpeed;

			// Match Enemy Speed
			//-------------------
			if (NPC->client->ps.speed<5.0f && EnemySpeed<5.0f)
			{
				NPC->client->ps.speed = EnemySpeed;
			}

			// Extra Slow Down When Out In Front
			//-----------------------------------
 			if  (MoveAccuracy<0.0f)
			{
				NPC->client->ps.speed *= (MoveAccuracy + 1.0f);
			}

			
			MoveDirection	*=        (MoveDistance/FIGHTER_ATTACK_FLANK_SLOWING);
			EnemyDirection	*= 1.0f - (MoveDistance/FIGHTER_ATTACK_FLANK_SLOWING);
			MoveDirection	+= EnemyDirection;
			
			if (TIMER_Done(NPC, "RamCheck"))
			{
				TIMER_Set(NPC, "RamCheck", Q_irand(1000, 3000));
				if (MoveDistance<FIGHTER_RAM_DIST && Q_irand(0, 2)==0)
				{
					//VEH_StartStrafeRam(FighterVeh, (EnemySide==Side_Left));
				}
			}
		}
	}
	
	
// NORMAL CHASE BEHAVIOR
//=======================
	else
	{
		if (!EnemyVeh && AimAccuracy>0.99f && MoveDistance<500 && !EnemyDead)
		{
			FighterAccelerate = true;
			FighterDoTurbo	= false;
		}
		else
		{
			FighterAccelerate = ((MoveDistance>500 && EnemySpeed>20.0f) || MoveDistance>1000);
			FighterDoTurbo	= (MoveDistance>3000 && EnemySpeed>20.0f);
		}
		ucmd.buttons	&=~BUTTON_VEH_SPEED;
	}
	


	// CRASHA!!
// APPLY RESULTS
//=======================
	// Decide Turbo
	//--------------
	/*if (FighterDoTurbo || FighterInTurbo)
	{
		ucmd.buttons |= BUTTON_ALT_ATTACK;
	}
	else
	{
		ucmd.buttons &=~BUTTON_ALT_ATTACK;
	}*/

	// Decide Acceleration
	//---------------------
	// CRASHA 
	ucmd.forwardmove = (FighterAccelerate)?(127):(0);



	// Decide To Shoot
	//-----------------
	ucmd.buttons	&=~BUTTON_ATTACK;
	ucmd.rightmove	= 0;
 	if (AimDistance<2000 && !EnemyDead)
	{
		// If Doing A Ram Attack
		//-----------------------
		//THIS IS ALLOWED ONLY FOR THE MOST UGLY AND BAD FIGHTER CLASS 
		// CAREFUL! THIS GET CRAZY SHOOT ENABLED!
		if (FighterYawOffset!=0)
		{
			//ucmd.buttons	&=~BUTTON_ATTACK;
			Fighter_Update(); 
		}
 		else if (AimAccuracy>FIGHTER_ATTACK_FWD)
		{
			//ucmd.buttons	|= BUTTON_ATTACK;
			Fighter_Update(); 
		} 
	}
	else if (AimAccuracy<FIGHTER_AIM_SIDE && AimAccuracy>-FIGHTER_AIM_SIDE)
	{
			if (AimAccuracy<FIGHTER_ATTACK_SIDE && AimAccuracy>-FIGHTER_ATTACK_SIDE)
			{
				    ucmd.buttons |= BUTTON_ATTACK;
					
			
					if (TIMER_Done(NPC, "RiderAltAttackCheck"))
					{
						TIMER_Set(NPC, "RiderAltAttackCheck", Q_irand(1000, 3000));
						if (Q_irand(0, 2)==0)
						{
							TIMER_Set(NPC, "RiderAltAttack", 300);
						}
					}
			
				WeaponThink(true);
				
			}
			ucmd.rightmove = (EnemySide==Side_Left)?( 127):(-127);
		}
		

	// Aim At Target
	//---------------
	if (FighterAimAtTarget)
	{
		MoveDirection.VecToAng();
		NPCInfo->desiredPitch	= AngleNormalize360(MoveDirection[PITCH]);
		NPCInfo->desiredYaw		= AngleNormalize360(MoveDirection[YAW] + FighterYawOffset);
	}
	NPC_UpdateAngles(qtrue, qtrue);
}

/*
-------------------------
Fighter_Hunt
-------------------------
*/

void Fighter_Hunt( qboolean visible, qboolean advance )
{
	float	distance, speed;
	vec3_t	forward;

	//If we're not supposed to stand still, pursue the player
	if ( NPCInfo->standTime < level.time )
	{
		// Only strafe when we can see the player
		if ( visible )
		{
			Fighter_Strafe();
			return;
		}
	}

	//If we don't want to advance, stop here
	if ( !advance && visible )
		return;

	//Only try and navigate if the player is visible
	if ( visible == qfalse )
	{
		// Move towards our goal
		NPCInfo->goalEntity = NPC->enemy;
		NPCInfo->goalRadius = 12;

		NPC_MoveToGoal(qtrue);
		return;
	}
	else
	{
		VectorSubtract( NPC->enemy->currentOrigin, NPC->currentOrigin, forward );
		distance = VectorNormalize( forward );
		// only for fighter classes not for cruisers 
		// Acceleration! this allow a fighter to flight like a swoop.
		// friction: this allow a fighter to slow speed when go over the player
		// if a fighter go over the player, it brakes and turn back for purchase. 
	}
	// If you disable that, fighter not fly and you get the bobbing movement! 
	speed = FIGHTER_FORWARD_BASE_SPEED + FIGHTER_FORWARD_MULTIPLIER * g_spskill->integer;
	VectorMA( NPC->client->ps.velocity, speed, forward, NPC->client->ps.velocity );

	//Fighter_Steer(); 
}


/*
-------------------------
NPC_Fighter_Patrol
-------------------------
*/
void NPC_Fighter_Patrol( void )
{
	Fighter_MaintainHeight();

	//If we have somewhere to go, then do that
	if (!NPC->enemy)
	{
		if ( NPC_CheckPlayerTeamStealth() )
		{
			//NPC_AngerSound();
			NPC_UpdateAngles( qtrue, qtrue );
			return;
		}

		if ( UpdateGoal() )
		{
			//start loop sound once we move
			ucmd.buttons |= BUTTON_WALKING;
			NPC_MoveToGoal( qtrue );
		}

		//randomly talk
		if (TIMER_Done(NPC,"patrolNoise"))
		{
			G_SoundOnEnt( NPC, CHAN_AUTO, va("sound/chars/fighter/misc/talk%d", Q_irand(1, 3)) );

			TIMER_Set( NPC, "patrolNoise", Q_irand( 2000, 4000 ) );
		}
	}

	NPC_UpdateAngles( qtrue, qtrue );
}


/*
-------------------------
Fighter_RangedAttack
-------------------------
*/
void Fighter_RangedAttack( qboolean visible, qboolean advance )
{
	if ( TIMER_Done( NPC, "attackDelay" ) && NPC->attackDebounceTime < level.time && visible )	// Attack?
	{
		if ( NPCInfo->burstCount > 6 )
		{
			if ( !NPC->fly_sound_debounce_time )
			{//delay closing down to give the player an opening
				NPC->fly_sound_debounce_time = level.time + Q_irand( 500, 2000 );
			}
			// YOU WANT TO SHIELD ONLY SOME CLASSES OF FIGHTER OR SPECIAL STARSHIP?
			/*else if ( NPC->fly_sound_debounce_time < level.time )
			{
				NPCInfo->localState = FSTATE_ACTIVE;
				NPC->fly_sound_debounce_time = NPCInfo->burstCount = 0;
				TIMER_Set( NPC, "attackDelay", Q_irand( 2000, 3500) );
				NPC->flags |= FL_SHIELDED;
				NPC_SetAnim( NPC, SETANIM_BOTH, BOTH_FLY_SHIELDED, SETANIM_FLAG_OVERRIDE|SETANIM_FLAG_HOLD );
				G_SoundOnEnt( NPC, CHAN_AUTO, "sound/chars/fighter/misc/fighter_shield_close" );
			}*/
		}
		else
		{
			Fighter_Fire();
		}
	}

	if ( NPCInfo->scriptFlags & SCF_CHASE_ENEMIES )
	{
		Fighter_Hunt( visible, advance );
	}
}

/*
-------------------------
Fighter_AttackDecision
-------------------------
*/
void Fighter_AttackDecision( void )
{
	// Always keep a good height off the ground
	Fighter_MaintainHeight();

	NPC->s.loopSound = G_SoundIndex( "sound/chars/fighter/fighter_hover_2_lp" );

	//randomly talk
	if ( TIMER_Done(NPC,"patrolNoise") )
	{
		if (TIMER_Done(NPC,"angerNoise"))
		{
			G_SoundOnEnt( NPC, CHAN_AUTO, va("sound/chars/fighter/misc/talk%d", Q_irand(1, 3)) );

			TIMER_Set( NPC, "patrolNoise", Q_irand( 4000, 10000 ) );
		}
	}

	// He's dead.
	if (NPC->enemy->health<1)
	{
		NPC->enemy = NULL;
		Fighter_Idle();
		return;
	}

	// If we don't have an enemy, just idle
	if ( NPC_CheckEnemyExt() == qfalse )
	{
		Fighter_Idle();
		return;
	}

	// Rate our distance to the target and visibilty
	float		distance	= (int) DistanceHorizontalSquared( NPC->currentOrigin, NPC->enemy->currentOrigin );	
	qboolean	visible		= NPC_ClearLOS( NPC->enemy );
	qboolean	advance		= (qboolean)(distance > FIGHTER_MIN_DISTANCE_SQR);

	// If we cannot see our target, move to see it
	if ( visible == qfalse )
	{
		if ( NPCInfo->scriptFlags & SCF_CHASE_ENEMIES )
		{
			Fighter_Hunt( visible, advance );
			return;
		}
	}

	NPC_FaceEnemy( qtrue );

	Fighter_RangedAttack( visible, advance );
}

qboolean NPC_CheckPlayerTeamStealth( void );



// FIGHTER DEFAULT WAS HERE 


/*
================
Fighter_use
================
*/
void Fighter_use( gentity_t *self, gentity_t *other, gentity_t *activator)
{
	G_ActivateBehavior(self,BSET_USE);

	self->flags &= ~FL_SHIELDED;
	NPC_SetAnim( self, SETANIM_BOTH, BOTH_STAND1, SETANIM_FLAG_OVERRIDE|SETANIM_FLAG_HOLD );
//	self->NPC->localState = LSTATE_WAKEUP;
	self->NPC->localState = FSTATE_ACTIVE;
}

/*
-------------------------
NPC_Fighter_Pain
-------------------------
*/
void NPC_Fighter_Pain( gentity_t *self, gentity_t *inflictor, gentity_t *other, const vec3_t point, int damage, int mod,int hitLoc ) 
{		
	NPC_Pain( self, inflictor, other, point, damage, mod );

	if ( mod == MOD_DEMP2 || mod == MOD_DEMP2_ALT )
	{
		self->NPC->burstCount = 0;
		TIMER_Set( self, "attackDelay", Q_irand( 9000, 12000) );
		self->flags |= FL_SHIELDED;
		NPC_SetAnim( self, SETANIM_BOTH, BOTH_SWIM_IDLE1, SETANIM_FLAG_OVERRIDE|SETANIM_FLAG_HOLD );
		G_SoundOnEnt( self, CHAN_AUTO, "sound/chars/fighter/misc/fighter_pain" );		

		self->NPC->localState = FSTATE_ACTIVE;
	}

	// You got hit, go after the enemy
	if (self->NPC->localState == FSTATE_ASLEEP)
	{
		G_Sound( self, G_SoundIndex("sound/chars/Fighter/misc/shieldsopen.wav"));

		//self->flags &= ~FL_SHIELDED;
		NPC_SetAnim( self, SETANIM_BOTH, BOTH_SWIM_IDLE1, SETANIM_FLAG_OVERRIDE|SETANIM_FLAG_HOLD );
		self->NPC->localState = FSTATE_WAKEUP;
	}
}


//////////////////////////////////////////// SEEKER AI
// APPLY THESE FOR FIGHTER DOGFIGHT MOVEMENT AROUND ENEMY 
extern void NPC_BSST_Patrol( void );
extern void Boba_FireDecide( void );
//extern gentity_t *CreateMissile( vec3_t org, vec3_t dir, float vel, int life, gentity_t *owner, qboolean altFire = qfalse );

void Figher_Round_Strafe( void );

#define FIGHTER_ROUND_DECAY		0.85f

#define	FIGHTER_ROUND_RANGE		1024
#define	FIGHTER_ROUND_RANGE_SQR	( FIGHTER_ROUND_RANGE * FIGHTER_ROUND_RANGE )

#define FIGHTER_ROUND_DISTANCE		512
#define FIGHTER_ROUND_DISTANCE_SQR	( FIGHTER_ROUND_DISTANCE * FIGHTER_ROUND_DISTANCE )

#define FIGHTER_ROUND_STRAFE_VEL	512
#define FIGHTER_ROUND_STRAFE_DIS	512
#define FIGHTER_ROUND_UPWARD_PUSH	32

#define FIGHTER_ROUND_FORWARD_BASE_SPEED	75
#define FIGHTER_ROUND_FORWARD_MULTIPLIER	5

#define FIGHTER_SEEK_RADIUS			2048

//------------------------------------
void Fighter_Round_MaintainHeight( void )
{	
	float	dif;

	// Update our angles regardless
	NPC_UpdateAngles( qtrue, qtrue );

	// If we have an enemy, we should try to hover at or a little below enemy eye level
	if ( NPC->enemy )
	{
		if (TIMER_Done( NPC, "heightChange" ))
		{
			TIMER_Set( NPC,"heightChange",Q_irand( 1000, 3000 ));

			// Find the height difference
			dif = (NPC->enemy->currentOrigin[2] +  Q_flrand( NPC->enemy->maxs[2]/2, NPC->enemy->maxs[2]+8 )) - NPC->currentOrigin[2]; 

			float	difFactor = 1.0f;
			if ( NPC->client->NPC_class == CLASS_FIGHTER )
			{
				if ( TIMER_Done( NPC, "flameTime" ) )
				{
					difFactor = 10.0f;
				}
			}

			// cap to prevent dramatic height shifts
			if ( fabs( dif ) > 2*difFactor )
			{
				if ( fabs( dif ) > 24*difFactor )
				{
					dif = ( dif < 0 ? -24*difFactor : 24*difFactor );
				}

				NPC->client->ps.velocity[2] = (NPC->client->ps.velocity[2]+dif)/2;
			}
			if ( NPC->client->NPC_class == CLASS_FIGHTER )
			{
				NPC->client->ps.velocity[2] *= Q_flrand( 0.85f, 3.0f );
			}
		}
	}
	else
	{
		gentity_t *goal = NULL;

		if ( NPCInfo->goalEntity )	// Is there a goal?
		{
			goal = NPCInfo->goalEntity;
		}
		else
		{
			goal = NPCInfo->lastGoalEntity;
		}
		if ( goal )
		{
			dif = goal->currentOrigin[2] - NPC->currentOrigin[2];

			if ( fabs( dif ) > 24 )
			{
				ucmd.upmove = ( ucmd.upmove < 0 ? -4 : 4 );
			}
			else
			{
				if ( NPC->client->ps.velocity[2] )
				{
					NPC->client->ps.velocity[2] *= FIGHTER_ROUND_DECAY;

					if ( fabs( NPC->client->ps.velocity[2] ) < 2 )
					{
						NPC->client->ps.velocity[2] = 0;
					}
				}
			}
		}
	}

	// Apply friction
	if ( NPC->client->ps.velocity[0] )
	{
		NPC->client->ps.velocity[0] *= FIGHTER_ROUND_DECAY;

		if ( fabs( NPC->client->ps.velocity[0] ) < 1 )
		{
			NPC->client->ps.velocity[0] = 0;
		}
	}

	if ( NPC->client->ps.velocity[1] )
	{
		NPC->client->ps.velocity[1] *= FIGHTER_ROUND_DECAY;

		if ( fabs( NPC->client->ps.velocity[1] ) < 1 )
		{
			NPC->client->ps.velocity[1] = 0;
		}
	}
}

//------------------------------------
void Fighter_Round_Strafe( void )
{
	int		side;
	vec3_t	end, right, dir;
	trace_t	tr;

	if ( random() > 0.7f || !NPC->enemy || !NPC->enemy->client )
	{
		// Do a regular style strafe
		AngleVectors( NPC->client->renderInfo.eyeAngles, NULL, right, NULL );

		// Pick a random strafe direction, then check to see if doing a strafe would be
		//	reasonably valid
		side = ( rand() & 1 ) ? -1 : 1;
		VectorMA( NPC->currentOrigin, FIGHTER_ROUND_STRAFE_DIS * side, right, end );

		gi.trace( &tr, NPC->currentOrigin, NULL, NULL, end, NPC->s.number, MASK_SOLID, (EG2_Collision)0, 0 );

		// Close enough
		if ( tr.fraction > 0.9f )
		{
			float vel = FIGHTER_ROUND_STRAFE_DIS;
			float upPush = FIGHTER_ROUND_UPWARD_PUSH;
			if ( NPC->client->NPC_class == CLASS_FIGHTER )
			{
				G_Sound( NPC, G_SoundIndex( "sound/chars/fighter/misc/strafe" ));
			}
			else
			{
				vel *= 3.0f;
				upPush *= 4.0f;
			}
			VectorMA( NPC->client->ps.velocity, vel*side, right, NPC->client->ps.velocity );
			// Add a slight upward push
			NPC->client->ps.velocity[2] += upPush;

			NPCInfo->standTime = level.time + 1000 + random() * 500;
		}
	}
	else
	{
		// Do a strafe to try and keep on the side of their enemy
		AngleVectors( NPC->enemy->client->renderInfo.eyeAngles, dir, right, NULL );

		// Pick a random side
		side = ( rand() & 1 ) ? -1 : 1;
		float	stDis = FIGHTER_ROUND_STRAFE_DIS;
		if ( NPC->client->NPC_class == CLASS_FIGHTER )
		{
			stDis *= 2.0f;
		}
		VectorMA( NPC->enemy->currentOrigin, stDis * side, right, end );

		// then add a very small bit of random in front of/behind the player action
		VectorMA( end, crandom() * 25, dir, end );

		gi.trace( &tr, NPC->currentOrigin, NULL, NULL, end, NPC->s.number, MASK_SOLID, (EG2_Collision)0, 0 );

		// Close enough
		if ( tr.fraction > 0.9f )
		{
			VectorSubtract( tr.endpos, NPC->currentOrigin, dir );
			dir[2] *= 0.25; // do less upward change
			float dis = VectorNormalize( dir );

			// Try to move the desired enemy side
			VectorMA( NPC->client->ps.velocity, dis, dir, NPC->client->ps.velocity );

			float upPush = FIGHTER_ROUND_UPWARD_PUSH;
			if ( NPC->client->NPC_class == CLASS_FIGHTER )
			{
				G_Sound( NPC, G_SoundIndex( "sound/chars/fighter/misc/strafe" ));
			}
			else
			{
				upPush *= 4.0f;
			}

			// Add a slight upward push
			NPC->client->ps.velocity[2] += upPush;

			NPCInfo->standTime = level.time + 2500 + random() * 500;
		}
	}
}

//------------------------------------
void Fighter_Round_Pain( gentity_t *self, gentity_t *inflictor, gentity_t *other, const vec3_t point, int damage, int mod,int hitLoc ) 
{
	if ( !(self->svFlags & SVF_CUSTOM_GRAVITY ))
	{//void G_Damage( gentity_t *targ, gentity_t *inflictor, gentity_t *attacker, vec3_t dir, vec3_t point, int damage, int dflags, int mod, int hitLoc=HL_NONE );
		G_Damage( self, NULL, NULL, vec3_origin, (float*)vec3_origin, 999, 0, MOD_FALLING );
	}

	SaveNPCGlobals();
	SetNPCGlobals( self );
	Fighter_Round_Strafe();
	RestoreNPCGlobals();
	NPC_Pain( self, inflictor, other, point, damage, mod );
}



//------------------------------------
void Fighter_Round_Hunt( qboolean visible, qboolean advance )
{
	float	distance, speed;
	vec3_t	forward;

	NPC_FaceEnemy( qtrue );

	// If we're not supposed to stand still, pursue the player
	if ( NPCInfo->standTime < level.time )
	{
		// Only strafe when we can see the player
		if ( visible )
		{
			Fighter_Round_Strafe();
			return;
		}
	}

	// If we don't want to advance, stop here
	if ( advance == qfalse )
	{
		return;
	}

	// Only try and navigate if the player is visible
	if ( visible == qfalse )
	{
		// Move towards our goal
		NPCInfo->goalEntity = NPC->enemy;
		NPCInfo->goalRadius = 24;

		NPC_MoveToGoal(qtrue);
		return;

	}
	else
	{
		VectorSubtract( NPC->enemy->currentOrigin, NPC->currentOrigin, forward );
		distance = VectorNormalize( forward );
	}

	speed = FIGHTER_ROUND_FORWARD_BASE_SPEED + FIGHTER_ROUND_FORWARD_MULTIPLIER * g_spskill->integer;
	VectorMA( NPC->client->ps.velocity, speed, forward, NPC->client->ps.velocity );
}

void Fighter_Round_Ranged( qboolean visible, qboolean advance )
{
	if ( NPC->client->NPC_class == CLASS_FIGHTER )
	{
		if ( NPC->count > 0 )
		{
			if ( TIMER_Done( NPC, "attackDelay" ))	// Attack?
			{
				TIMER_Set( NPC, "attackDelay", Q_irand( 250, 2500 ));
				Fighter_Fire();
				NPC->count--;
			}
		}
	}

	if ( NPCInfo->scriptFlags & SCF_CHASE_ENEMIES )
	{
		Fighter_Round_Hunt( visible, advance );
	}
} 

//------------------------------------
void Fighter_Round_Attack( void )
{
	// Always keep a good height off the ground
	Fighter_Round_MaintainHeight();

	// Rate our distance to the target, and our visibilty
	float		rounddistance	= DistanceHorizontalSquared( NPC->currentOrigin, NPC->enemy->currentOrigin );	
	qboolean	roundvisible		= NPC_ClearLOS( NPC->enemy );
	qboolean	roundadvance		= (qboolean)(rounddistance > FIGHTER_ROUND_DISTANCE_SQR);

	if ( NPC->client->NPC_class == CLASS_FIGHTER )
	{
		roundadvance = (qboolean)(rounddistance>(200.0f*200.0f));
	}

	// If we cannot see our target, move to see it
	if ( roundvisible == qfalse )
	{
		if ( NPCInfo->scriptFlags & SCF_CHASE_ENEMIES )
		{
			Fighter_Round_Hunt( roundvisible, roundadvance );
			return;
		}
	}

	Fighter_Round_Ranged( roundvisible, roundadvance );
}

//------------------------------------
void Fighter_Round_FindEnemy( void )
{
	int			numFound;
	float		dis, bestDis = FIGHTER_SEEK_RADIUS * FIGHTER_SEEK_RADIUS + 1;
	vec3_t		mins, maxs;
	gentity_t	*entityList[MAX_GENTITIES], *ent, *best = NULL;

	VectorSet( maxs, FIGHTER_SEEK_RADIUS, FIGHTER_SEEK_RADIUS, FIGHTER_SEEK_RADIUS );
	VectorScale( maxs, -1, mins );

	numFound = gi.EntitiesInBox( mins, maxs, entityList, MAX_GENTITIES );

	for ( int i = 0 ; i < numFound ; i++ ) 
	{
		ent = entityList[i];

		if ( ent->s.number == NPC->s.number || !ent->client || !ent->NPC || ent->health <= 0 || !ent->inuse )
		{
			continue;
		}

		if ( ent->client->playerTeam == NPC->client->playerTeam || ent->client->playerTeam == TEAM_NEUTRAL ) // don't attack same team or bots
		{
			continue;
		}

		// try to find the closest visible one
		if ( !NPC_ClearLOS( ent ))
		{
			continue;
		}

		dis = DistanceHorizontalSquared( NPC->currentOrigin, ent->currentOrigin );

		if ( dis <= bestDis )
		{
			bestDis = dis;
			best = ent;
		}
	}

	if ( best )
	{
		// used to offset seekers around a circle so they don't occupy the same spot.  This is not a fool-proof method.
		NPC->random = random() * 6.3f; // roughly 2pi

		NPC->enemy = best;
	}
}

//------------------------------------
void Fighter_Round_FollowPlayer( void )
{
	Fighter_Round_MaintainHeight();

	float	dis	= DistanceHorizontalSquared( NPC->currentOrigin, g_entities[0].currentOrigin );
	vec3_t	pt, dir;
	
	float	minDistSqr = FIGHTER_ROUND_DISTANCE_SQR;

	if ( NPC->client->NPC_class == CLASS_FIGHTER )
	{
		if ( TIMER_Done( NPC, "flameTime" ) )
		{
			minDistSqr = 200*200;
		}
	}

	if ( dis < minDistSqr )
	{
		// generally circle the player closely till we take an enemy..this is our target point
		if ( NPC->client->NPC_class == CLASS_FIGHTER )
		{
			pt[0] = g_entities[0].currentOrigin[0] + cos( level.time * 0.001f + NPC->random ) * 250;
			pt[1] = g_entities[0].currentOrigin[1] + sin( level.time * 0.001f + NPC->random ) * 250;
			if ( NPC->client->jetPackTime < level.time )
			{
				pt[2] = NPC->currentOrigin[2] - 64;
			}
			else
			{
				pt[2] = g_entities[0].currentOrigin[2] + 200;
			}
		}
		else
		{
			pt[0] = g_entities[0].currentOrigin[0] + cos( level.time * 0.001f + NPC->random ) * 56;
			pt[1] = g_entities[0].currentOrigin[1] + sin( level.time * 0.001f + NPC->random ) * 56;
			pt[2] = g_entities[0].currentOrigin[2] + 40;
		}

		VectorSubtract( pt, NPC->currentOrigin, dir );
		VectorMA( NPC->client->ps.velocity, 0.8f, dir, NPC->client->ps.velocity );
	}
	else
	{
		if ( NPC->client->NPC_class == CLASS_FIGHTER )
		{
			if ( TIMER_Done( NPC, "seekerhiss" ))
			{
				TIMER_Set( NPC, "seekerhiss", 1000 + random() * 1000 );
			}
		}

		// Hey come back!
		NPCInfo->goalEntity = &g_entities[0];
		NPCInfo->goalRadius = 32;
		NPC_MoveToGoal( qtrue );
		NPC->owner = &g_entities[0];
	}

	if ( NPCInfo->enemyCheckDebounceTime < level.time )
	{
		// check twice a second to find a new enemy
		Fighter_Round_FindEnemy();
		NPCInfo->enemyCheckDebounceTime = level.time + 500;
	}

	NPC_UpdateAngles( qtrue, qtrue );
}

//------------------------------------
void NPC_FighterRound_Default( void )
{
	if ( NPC->random == 0.0f )
	{
		// used to offset seekers around a circle so they don't occupy the same spot.  This is not a fool-proof method.
		NPC->random = random() * 6.3f; // roughly 2pi
	}

	if ( NPC->enemy && NPC->enemy->health && NPC->enemy->inuse )
	{
			Fighter_Round_Attack();
			if ( NPC->client->NPC_class == CLASS_FIGHTER )
			{
				Fighter_FireDecide();
			}
			return;
		
	}
	else if ( NPC->client->NPC_class == CLASS_FIGHTER )
	{
		NPC_BSST_Patrol();
		return;
	}

	// In all other cases, follow the player and look for enemies to take on
	Fighter_Round_FollowPlayer();
}



void NPC_BSFighter_Default( void )
{
	//Fighter_Update();	
	if ( NPC->targetname )
	{
		//NPC->e_UseFunc = useF_fighter_use;
	}

	if (( NPC->enemy ) && (NPCInfo->localState != LSTATE_WAKEUP))
	{
		// Don't attack if waking up or if no enemy
		//Fighter_Round_FollowPlayer(); // chase the opponent (temp behavour) 
		// Activate for remote behavoru. not activate for sentry behavour 
		Fighter_AttackDecision();
	}
	else if ( NPCInfo->scriptFlags & SCF_LOOK_FOR_ENEMIES )
	{
		NPC_Fighter_Patrol();
	}
	else
	{
		Fighter_Idle();
	}

	// FIGHTER SEEKER AI MERGED ! 
	/*if ( NPC->random == 0.0f )
	{
		// used to offset seekers around a circle so they don't occupy the same spot.  This is not a fool-proof method.
		NPC->random = random() * 6.3f; // roughly 2pi
	}*/

	if ( NPC->enemy && NPC->enemy->health && NPC->enemy->inuse )
	{
			Fighter_Round_Attack();
			//Fighter_Round_FollowPlayer();  // FUNZIONA! GIRANO ATTORNO COME SQUALI! (ma si muovono da schifo)
			if ( NPC->client->NPC_class == CLASS_FIGHTER )
			{
				Fighter_FireDecide();
			}
			return;
		
	}
	/*else if ( NPC->client->NPC_class == CLASS_FIGHTER )
	{
		NPC_BSST_Patrol();
		return;
	}*/

	// In all other cases, follow the player and look for enemies to take on
	//Fighter_Round_FollowPlayer();
	// for avoid that ovlon and allied follow player fly silly around. 


}


// ADDED FOR PLAYER PRECACHE STUFF

void NPC_Fighter_Precache(void)
{
	G_SoundIndex( "sound/chars/fighter/fighter_damaged1" );
	G_SoundIndex( "sound/chars/fighter/fighter_damaged2" );

	RegisterItem( FindItemForWeapon( WP_BLASTER ));	//precache the weapon
	RegisterItem( FindItemForWeapon( WP_BOWCASTER ));	//precache the weapon
	RegisterItem( FindItemForWeapon( WP_ROCKET_LAUNCHER ));	//precache the weapon
	RegisterItem( FindItemForWeapon( WP_REPEATER ));	//precache the weapon
	RegisterItem( FindItemForWeapon( WP_BLASTER_PISTOL ));	//precache the weapon
	RegisterItem( FindItemForWeapon( WP_FLECHETTE ));	//precache the weapon
	RegisterItem( FindItemForWeapon( WP_DEMP2 ));	//precache the weapon
	RegisterItem( FindItemForWeapon( WP_CONCUSSION ));	//precache the weapon



	G_EffectIndex( "env/med_explode2" );
//	G_EffectIndex( "smaller_chunks" );
	G_EffectIndex( "blaster/smoke_bolton" );
	G_EffectIndex( "explosions/droidexplosion1" );
}

/*

// MOVEMENT FUNCTIONS - CREATED BASED ABOUT FIGHTERNPC.CPP 
#include "g_headers.h"

//seems to be a compiler bug, it doesn't clean out the #ifdefs between dif-compiles
//or something, so the headers spew errors on these defs from the previous compile.
//this fixes that. -rww
#ifdef _JK2MP
//get rid of all the crazy defs we added for this file
#undef currentAngles
#undef currentOrigin
#undef mins
#undef maxs
#undef legsAnimTimer
#undef torsoAnimTimer
#undef bool
#undef false
#undef true

#undef sqrtf
#undef Q_flrand

#undef MOD_EXPLOSIVE
#endif

#ifdef _JK2 //SP does not have this preprocessor for game like MP does
#ifndef _JK2MP
#define _JK2MP
#endif
#endif

#ifndef _JK2MP //if single player
#ifndef QAGAME //I don't think we have a QAGAME define
#define QAGAME //but define it cause in sp we're always in the game
#endif
#endif

#ifdef QAGAME //including game headers on cgame is FORBIDDEN ^_^
#include "g_local.h"
#elif defined _JK2MP
#include "bg_public.h"
#endif

#ifndef _JK2MP
#include "g_functions.h"
#include "g_vehicles.h"
#else
#include "bg_vehicles.h"
#endif

#ifdef _JK2MP
//this is really horrible, but it works! just be sure not to use any locals or anything
//with these names (exluding bool, false, true). -rww
#define currentAngles r.currentAngles
#define currentOrigin r.currentOrigin
#define mins r.mins
#define maxs r.maxs
#define legsAnimTimer legsTimer
#define torsoAnimTimer torsoTimer
#define bool qboolean
#define false qfalse
#define true qtrue

#define sqrtf sqrt
#define Q_flrand flrand

#define MOD_EXPLOSIVE MOD_SUICIDE
#else
#define bgEntity_t gentity_t
#endif

extern float DotToSpot( vec3_t spot, vec3_t from, vec3_t fromAngles );
#ifdef QAGAME //SP or gameside MP
extern vmCvar_t	cg_thirdPersonAlpha;
extern vec3_t playerMins;
extern vec3_t playerMaxs;
extern cvar_t	*g_speederControlScheme;
extern void ChangeWeapon( gentity_t *ent, int newWeapon );
extern void PM_SetAnim(pmove_t	*pm,int setAnimParts,int anim,int setAnimFlags, int blendTime);
extern int PM_AnimLength( int index, animNumber_t anim );
extern void G_VehicleTrace( trace_t *results, const vec3_t start, const vec3_t tMins, const vec3_t tMaxs, const vec3_t end, int passEntityNum, int contentmask );
#endif

extern qboolean BG_UnrestrainedPitchRoll( playerState_t *ps, Vehicle_t *pVeh );

#ifdef _JK2MP

#include "../namespace_begin.h"

extern void BG_SetAnim(playerState_t *ps, animation_t *animations, int setAnimParts,int anim,int setAnimFlags, int blendTime);
extern int BG_GetTime(void);
#endif

extern void BG_ExternThisSoICanRecompileInDebug( Vehicle_t *pVeh, playerState_t *riderPS );

//this stuff has got to be predicted, so..
bool BG_FighterNPCUpdate(Vehicle_t *pVeh, const usercmd_t *pUcmd, vec3_t trMins, vec3_t trMaxs, float gravity,
					  void (*traceFunc)( trace_t *results, const vec3_t start, const vec3_t lmins, const vec3_t lmaxs, const vec3_t end, int passEntityNum, int contentMask ))
{
	vec3_t		bottom;
	playerState_t *parentPS;
	qboolean	isDead = qfalse;
#ifdef QAGAME //don't do this on client
	// Make sure the riders are not visible or collidable.
	//pVeh->m_pVehicleInfo->Ghost( pVeh, pVeh->m_pPilot );
#endif


#ifdef _JK2MP
	parentPS = pVeh->m_pParentEntity->playerState;
#else
	parentPS = &pVeh->m_pParentEntity->client->ps;
#endif

	if (!parentPS)
	{
		Com_Error(ERR_DROP, "NULL PS in BG_FighterUpdate (%s)", pVeh->m_pVehicleInfo->name);
		return false;
	}

	// If we have a pilot, take out gravity (it's a flying craft...).
	if ( NPC )
	{
		parentPS->gravity = 0;
#ifndef _JK2MP //don't need this flag in mp, I.. guess
		pVeh->m_pParentEntity->svFlags |= SVF_CUSTOM_GRAVITY;
#endif
	}
	else
	{
#ifndef _JK2MP //don't need this flag in mp, I.. guess
		pVeh->m_pParentEntity->svFlags &= ~SVF_CUSTOM_GRAVITY;
#else //in MP set grav back to normal gravity
		if (pVeh->m_pVehicleInfo->gravity)
		{
			parentPS->gravity = pVeh->m_pVehicleInfo->gravity;
		}
		else
		{ //it doesn't have gravity specified apparently
			parentPS->gravity = gravity;
		}
#endif
	}

#ifdef _JK2MP
	isDead = (qboolean)((parentPS->eFlags&EF_DEAD)!=0);
#else
	isDead = (parentPS->stats[STAT_HEALTH] <= 0 );
#endif

	/*
	if ( isDead || 
		(pVeh->m_pVehicleInfo->surfDestruction &&
			pVeh->m_iRemovedSurfaces ) )
	{//can't land if dead or spiralling out of control
		pVeh->m_LandTrace.fraction = 1.0f;
		pVeh->m_LandTrace.contents = pVeh->m_LandTrace.surfaceFlags = 0;
		VectorClear( pVeh->m_LandTrace.plane.normal );
		pVeh->m_LandTrace.allsolid = qfalse;
		pVeh->m_LandTrace.startsolid = qfalse;
	}
	else
	{
	*/
	//argh, no, I need to have a way to see when they impact the ground while damaged. -rww

		// Check to see if the fighter has taken off yet (if it's a certain height above ground).
/*		VectorCopy( parentPS->origin, bottom );
		bottom[2] -= pVeh->m_pVehicleInfo->landingHeight;

		traceFunc( &pVeh->m_LandTrace, parentPS->origin, trMins, trMaxs, bottom, pVeh->m_pParentEntity->s.number, (MASK_NPCSOLID&~CONTENTS_BODY) );
	//}

	return true;
}
/*
#ifdef QAGAME //ONLY in SP or on server, not cgame

// Like a think or move command, this updates various vehicle properties.
static bool Update( Vehicle_t *pVeh, const usercmd_t *pUcmd )
{
	assert(pVeh->m_pParentEntity);
	if (!BG_FighterNPCUpdate(pVeh, pUcmd, ((gentity_t *)pVeh->m_pParentEntity)->mins,
		((gentity_t *)pVeh->m_pParentEntity)->maxs, 
#ifdef _JK2MP
		g_gravity.value,
#else
		g_gravity->value,
#endif
		G_VehicleTrace))
	{
		return false;
	}

	if ( !g_vehicleInfo[VEHICLE_BASE].Update( pVeh, pUcmd ) )
	{
		return false;
	}

	return true;
}

// Board this Vehicle (get on). The first entity to board an empty vehicle becomes the Pilot.
static bool Board( Vehicle_t *pVeh, bgEntity_t *pEnt )
{
	if ( !g_vehicleInfo[VEHICLE_BASE].Board( pVeh, pEnt ) )
		return false;

	// Set the board wait time (they won't be able to do anything, including getting off, for this amount of time).
	pVeh->m_iBoarding = level.time + 1500;

	return true;
}

// Eject an entity from the vehicle.
static bool Eject( Vehicle_t *pVeh, bgEntity_t *pEnt, qboolean forceEject )
{
	if ( g_vehicleInfo[VEHICLE_BASE].Eject( pVeh, pEnt, forceEject ) )
	{
		return true;
	}
	
	return false;
}

#endif //end game-side only

//method of decrementing the given angle based on the given taking variable frame times into account
static float PredictedAngularDecrement(float scale, float timeMod, float originalAngle)
{
	float fixedBaseDec = originalAngle*0.05f;
	float r = 0.0f;

	if (fixedBaseDec < 0.0f)
	{
		fixedBaseDec = -fixedBaseDec;
	}
	
	fixedBaseDec *= (1.0f+(1.0f-scale));

	if (fixedBaseDec < 0.1f)
	{ //don't increment in incredibly small fractions, it would eat up unnecessary bandwidth.
		fixedBaseDec = 0.1f;
	}

	fixedBaseDec *= (timeMod*0.1f);
	if (originalAngle > 0.0f)
	{ //subtract
		r = (originalAngle-fixedBaseDec);
		if (r < 0.0f)
		{
			r = 0.0f;
		}
	}
	else if (originalAngle < 0.0f)
	{ //add
		r = (originalAngle+fixedBaseDec);
		if (r > 0.0f)
		{
			r = 0.0f;
		}
	}

	return r;
}

#ifdef QAGAME//only do this check on GAME side, because if it's CGAME, it's being predicted, and it's only predicted if the local client is the driver
qboolean FighterNPCIsInSpace( gentity_t *NPC )
{
	if ( NPC 
		&& NPC->client 
		&& NPC->client->inSpaceIndex
		&& NPC->client->inSpaceIndex < ENTITYNUM_WORLD )
	{
		return qtrue;
	}
	return qfalse;
}
#endif

qboolean FighterNPCOverValidLandingSurface( gentity_t *pVeh )
{
	if ( pVeh->m_LandTrace.fraction < 1.0f //ground present
		&& pVeh->m_LandTrace.plane.normal[2] >= MIN_LANDING_SLOPE )//flat enough
		//FIXME: also check for a certain surface flag ... "landing zones"?
	{
		return qtrue;
	}
	return qfalse;
}

qboolean FighterNPCIsLanded( Vehicle_t *pVeh, playerState_t *parentPS )
{
	if ( FighterNPCOverValidLandingSurface( pVeh )
		&& !parentPS->speed )//stopped
	{
		return qtrue;
	}
	return qfalse;
}

qboolean FighterNPCIsLanding( Vehicle_t *pVeh, playerState_t *parentPS )
{

	if ( FighterNPCOverValidLandingSurface( pVeh )
#ifdef QAGAME//only do this check on GAME side, because if it's CGAME, it's being predicted, and it's only predicted if the local client is the driver
		&& pVeh->m_pVehicleInfo->Inhabited( pVeh )//has to have a driver in order to be capable of landing
#endif
		&& (pVeh->m_ucmd.forwardmove < 0||pVeh->m_ucmd.upmove<0) //decelerating or holding crouch button
		&& parentPS->speed <= MIN_LANDING_SPEED )//going slow enough to start landing - was using pVeh->m_pVehicleInfo->speedIdle, but that's still too fast
	{
		return qtrue;
	}
	return qfalse;
}

qboolean FighterNPCIsLaunching( Vehicle_t *pVeh, playerState_t *parentPS )
{

	if ( FighterNPCOverValidLandingSurface( pVeh )
#ifdef QAGAME//only do this check on GAME side, because if it's CGAME, it's being predicted, and it's only predicted if the local client is the driver
		&& pVeh->m_pVehicleInfo->Inhabited( pVeh )//has to have a driver in order to be capable of landing
#endif
		&& pVeh->m_ucmd.upmove > 0 //trying to take off
		&& parentPS->speed <= 200.0f )//going slow enough to start landing - was using pVeh->m_pVehicleInfo->speedIdle, but that's still too fast
	{
		return qtrue;
	}
	return qfalse;
}

qboolean FighterNPCSuspended( Vehicle_t *pVeh, playerState_t *parentPS )
{
#ifdef QAGAME//only do this check on GAME side, because if it's CGAME, it's being predicted, and it's only predicted if the local client is the driver
	if (!pVeh->m_pPilot//empty
		&& !parentPS->speed//not moving
		&& pVeh->m_ucmd.forwardmove <= 0//not trying to go forward for whatever reason
		&& pVeh->m_pParentEntity != NULL
		&& (((gentity_t *)pVeh->m_pParentEntity)->spawnflags&2) )//SUSPENDED spawnflag is on
	{
		return qtrue;
	}
	return qfalse;
#elif defined CGAME
	return qfalse;
#endif
}

#ifdef CGAME
extern void trap_S_StartSound( vec3_t origin, int entityNum, int entchannel, sfxHandle_t sfx ); //cg_syscalls.c
extern sfxHandle_t trap_S_RegisterSound( const char *sample); //cg_syscalls.c
#endif

//MP RULE - ALL PROCESSMOVECOMMANDS FUNCTIONS MUST BE BG-COMPATIBLE!!!
//If you really need to violate this rule for SP, then use ifdefs.
//By BG-compatible, I mean no use of game-specific data - ONLY use
//stuff available in the MP bgEntity (in SP, the bgEntity is #defined
//as a gentity, but the MP-compatible access restrictions are based
//on the bgEntity structure in the MP codebase) -rww
// ProcessMoveCommands the Vehicle.
#define FIGHTER_MIN_TAKEOFF_FRACTION 0.7f
static void ProcessMoveCommands( Vehicle_t *pVeh )
{
	/************************************************************************************/
	/*	BEGIN	Here is where we move the vehicle (forward or back or whatever). BEGIN	*/
	/************************************************************************************/
/*
	//Client sets ucmds and such for speed alterations
	float speedInc, speedIdleDec, speedIdle, speedIdleAccel, speedMin, speedMax;
	bgEntity_t *parent = pVeh->m_pParentEntity;
	qboolean isLandingOrLaunching = qfalse;/*
#ifndef _JK2MP//SP
	int curTime = level.time;
#elif defined QAGAME//MP GAME
	int curTime = level.time;
#elif defined CGAME//MP CGAME
	//FIXME: pass in ucmd?  Not sure if this is reliable...
	int curTime = pm->cmd.serverTime;
#endif

#ifdef _JK2MP
	playerState_t *parentPS = parent->playerState;
#else
	playerState_t *parentPS = &parent->client->ps;
#endif

#ifdef _JK2MP
	if ( parentPS->hyperSpaceTime
		&& curTime - parentPS->hyperSpaceTime < HYPERSPACE_TIME )
	{//Going to Hyperspace
		//totally override movement
		float timeFrac = ((float)(curTime-parentPS->hyperSpaceTime))/HYPERSPACE_TIME;
		if ( timeFrac < HYPERSPACE_TELEPORT_FRAC )
		{//for first half, instantly jump to top speed!
			if ( !(parentPS->eFlags2&EF2_HYPERSPACE) )
			{//waiting to face the right direction, do nothing
				parentPS->speed = 0.0f;
			}
			else
			{
				if ( parentPS->speed < HYPERSPACE_SPEED )
				{//just started hyperspace
//MIKE: This is going to play the sound twice for the predicting client, I suggest using
//a predicted event or only doing it game-side. -rich
#ifdef QAGAME//MP GAME-side
					//G_EntitySound( ((gentity_t *)(pVeh->m_pParentEntity)), CHAN_LOCAL, pVeh->m_pVehicleInfo->soundHyper );
#elif defined CGAME//MP CGAME-side
					trap_S_StartSound( NULL, pm->ps->clientNum, CHAN_LOCAL, pVeh->m_pVehicleInfo->soundHyper );
#endif			
				}

				parentPS->speed = HYPERSPACE_SPEED;
			}
		}
		else
		{//slow from top speed to 200...
			parentPS->speed = 200.0f + ((1.0f-timeFrac)*(1.0f/HYPERSPACE_TELEPORT_FRAC)*(HYPERSPACE_SPEED-200.0f));
			//don't mess with acceleration, just pop to the high velocity
			if ( VectorLength( parentPS->velocity ) < parentPS->speed )
			{
				VectorScale( parentPS->moveDir, parentPS->speed, parentPS->velocity );
			}
		}
		return;
	}
#endif

	if ( pVeh->m_iDropTime >= curTime )
	{//no speed, just drop
		parentPS->speed = 0.0f;
		parentPS->gravity = 800;
		return;
	}

	isLandingOrLaunching = (FighterNPCIsLanding( pVeh, parentPS )||FighterNPCIsLaunching( pVeh, parentPS ));

	// If we are hitting the ground, just allow the fighter to go up and down.
	if ( isLandingOrLaunching//going slow enough to start landing
		&& (pVeh->m_ucmd.forwardmove<=0||pVeh->m_LandTrace.fraction<=FIGHTER_MIN_TAKEOFF_FRACTION) )//not trying to accelerate away already (or: you are trying to, but not high enough off the ground yet)
	{//FIXME: if start to move forward and fly over something low while still going relatively slow, you may try to land even though you don't mean to...
		//float fInvFrac = 1.0f - pVeh->m_LandTrace.fraction; 

		if ( pVeh->m_ucmd.upmove > 0 )
		{
#ifdef _JK2MP
			if ( parentPS->velocity[2] <= 0 
				&& pVeh->m_pVehicleInfo->soundTakeOff )
			{//taking off for the first time
#ifdef QAGAME//MP GAME-side
				G_EntitySound( ((gentity_t *)(pVeh->m_pParentEntity)), CHAN_AUTO, pVeh->m_pVehicleInfo->soundTakeOff );
#endif
			}
#endif
			parentPS->velocity[2] += pVeh->m_pVehicleInfo->acceleration * pVeh->m_fTimeModifier;// * ( /*fInvFrac **//* 1.5f );
		}
		else if ( pVeh->m_ucmd.upmove < 0 )
		{
			parentPS->velocity[2] -= pVeh->m_pVehicleInfo->acceleration * pVeh->m_fTimeModifier;// * ( /*fInvFrac **//* 1.8f );
		}
		else if ( pVeh->m_ucmd.forwardmove < 0 )
		{
			if ( pVeh->m_LandTrace.fraction != 0.0f ) 
			{
				parentPS->velocity[2] -= pVeh->m_pVehicleInfo->acceleration * pVeh->m_fTimeModifier;
			}

			if ( pVeh->m_LandTrace.fraction <= FIGHTER_MIN_TAKEOFF_FRACTION )
			{
				//pVeh->m_pParentEntity->client->ps.velocity[0] *= pVeh->m_LandTrace.fraction;
				//pVeh->m_pParentEntity->client->ps.velocity[1] *= pVeh->m_LandTrace.fraction;

				//remember to always base this stuff on the time modifier! otherwise, you create
				//framerate-dependancy issues and break prediction in MP -rww
				//parentPS->velocity[2] *= pVeh->m_LandTrace.fraction;
				//it's not an angle, but hey
				parentPS->velocity[2] = PredictedAngularDecrement(pVeh->m_LandTrace.fraction, pVeh->m_fTimeModifier*5.0f, parentPS->velocity[2]);
			
				parentPS->speed = 0;
			}
		}

		// Make sure they don't pitch as they near the ground.
		//pVeh->m_vOrientation[PITCH] *= 0.7f;
		pVeh->m_vOrientation[PITCH] = PredictedAngularDecrement(0.7f, pVeh->m_fTimeModifier*10.0f, pVeh->m_vOrientation[PITCH]);

		return;
	}

	if ( (pVeh->m_ucmd.upmove > 0) && pVeh->m_pVehicleInfo->turboSpeed )
	{
		if ((curTime - pVeh->m_iTurboTime)>pVeh->m_pVehicleInfo->turboRecharge)
		{
			pVeh->m_iTurboTime = (curTime + pVeh->m_pVehicleInfo->turboDuration);
			if (pVeh->m_pVehicleInfo->iTurboStartFX)
			{
				int i;
				for (i=0; i<MAX_VEHICLE_EXHAUSTS; i++)
				{
					if (pVeh->m_iExhaustTag[i]==-1)
					{
						break;
					}
					#ifndef _JK2MP//SP
						G_PlayEffect(pVeh->m_pVehicleInfo->iTurboStartFX, pVeh->m_pParentEntity->playerModel, pVeh->m_iExhaustTag[i], pVeh->m_pParentEntity->s.number, pVeh->m_pParentEntity->currentOrigin );
					#else
						//TODO: MP Play Effect?
					#endif
				}
			}
			//NOTE: turbo sound can't be part of effect if effect is played on every muzzle!
			if ( pVeh->m_pVehicleInfo->soundTurbo )
			{
#ifndef _JK2MP//SP
				G_SoundIndexOnEnt( pVeh->m_pParentEntity, CHAN_AUTO, pVeh->m_pVehicleInfo->soundTurbo );
#elif defined QAGAME//MP GAME-side
				G_EntitySound( ((gentity_t *)(pVeh->m_pParentEntity)), CHAN_AUTO, pVeh->m_pVehicleInfo->soundTurbo );
#elif defined CGAME//MP CGAME-side
				//trap_S_StartSound( NULL, pVeh->m_pParentEntity->s.number, CHAN_AUTO, pVeh->m_pVehicleInfo->soundTurbo );
#endif		
			}
		}
	}
	speedInc = pVeh->m_pVehicleInfo->acceleration * pVeh->m_fTimeModifier;
	if ( curTime < pVeh->m_iTurboTime )
	{//going turbo speed
		speedMax = pVeh->m_pVehicleInfo->turboSpeed;
		//double our acceleration
		speedInc *= 2.0f;
		//force us to move forward
		pVeh->m_ucmd.forwardmove = 127;
#ifdef _JK2MP//SP can cheat and just check m_iTurboTime directly... :)
		//add flag to let cgame know to draw the iTurboFX effect
		parentPS->eFlags |= EF_JETPACK_ACTIVE;
#endif
	}
	/*
	//FIXME: if turbotime is up and we're waiting for it to recharge, should our max speed drop while we recharge?
	else if ( (curTime - pVeh->m_iTurboTime)<3000 )
	{//still waiting for the recharge
		speedMax = pVeh->m_pVehicleInfo->speedMax*0.75;
	}
	*//*
	else
	{//normal max speed
		speedMax = pVeh->m_pVehicleInfo->speedMax;
#ifdef _JK2MP//SP can cheat and just check m_iTurboTime directly... :)
		if ( (parentPS->eFlags&EF_JETPACK_ACTIVE) )
		{//stop cgame from playing the turbo exhaust effect
			parentPS->eFlags &= ~EF_JETPACK_ACTIVE;
		}
#endif
	}
	speedIdleDec = pVeh->m_pVehicleInfo->decelIdle * pVeh->m_fTimeModifier;
	speedIdle = pVeh->m_pVehicleInfo->speedIdle;
	speedIdleAccel = pVeh->m_pVehicleInfo->accelIdle * pVeh->m_fTimeModifier;
	speedMin = pVeh->m_pVehicleInfo->speedMin;

	if ( (parentPS->brokenLimbs&(1<<SHIPSURF_DAMAGE_BACK_HEAVY)) )
	{//engine has taken heavy damage
		speedMax *= 0.8f;//at 80% speed
	}
	else if ( (parentPS->brokenLimbs&(1<<SHIPSURF_DAMAGE_BACK_LIGHT)) )
	{//engine has taken light damage
		speedMax *= 0.6f;//at 60% speed
	}

	if (pVeh->m_iRemovedSurfaces
		|| parentPS->electrifyTime>=curTime)
	{ //go out of control
		parentPS->speed += speedInc;
		//Why set forwardmove?  PMove code doesn't use it... does it?
		pVeh->m_ucmd.forwardmove = 127;
	}
#ifdef QAGAME //well, the thing is always going to be inhabited if it's being predicted!
	else if ( FighterNPCSuspended( pVeh, parentPS ) )
	{
		parentPS->speed = 0;
		pVeh->m_ucmd.forwardmove = 0;
	}
	else if ( !pVeh->m_pVehicleInfo->Inhabited( pVeh ) 
		&& parentPS->speed > 0 )
	{//pilot jumped out while we were moving forward (not landing or landed) so just keep the throttle locked
		//Why set forwardmove?  PMove code doesn't use it... does it?
		pVeh->m_ucmd.forwardmove = 127;
	}
#endif
	else if ( ( parentPS->speed || parentPS->groundEntityNum == ENTITYNUM_NONE  ||
		 pVeh->m_ucmd.forwardmove || pVeh->m_ucmd.upmove > 0 ) && pVeh->m_LandTrace.fraction >= 0.05f ) 
	{
		if ( pVeh->m_ucmd.forwardmove > 0 && speedInc )
		{
			parentPS->speed += speedInc;
			pVeh->m_ucmd.forwardmove = 127;
		}
		else if ( pVeh->m_ucmd.forwardmove < 0 
			|| pVeh->m_ucmd.upmove < 0 )
		{//decelerating or braking
			if ( pVeh->m_ucmd.upmove < 0 )
			{//braking (trying to land?), slow down faster
				if ( pVeh->m_ucmd.forwardmove )
				{//decelerator + brakes
					speedInc += pVeh->m_pVehicleInfo->braking;
					speedIdleDec += pVeh->m_pVehicleInfo->braking;
				}
				else
				{//just brakes
					speedInc = speedIdleDec = pVeh->m_pVehicleInfo->braking;
				}
			}
			if ( parentPS->speed > speedIdle )
			{
				parentPS->speed -= speedInc;
			}
			else if ( parentPS->speed > speedMin )
			{
				if ( FighterNPCOverValidLandingSurface( pVeh ) )
				{//there's ground below us and we're trying to slow down, slow down faster
					parentPS->speed -= speedInc;
				}
				else
				{
					parentPS->speed -= speedIdleDec;
					if ( parentPS->speed < MIN_LANDING_SPEED )
					{//unless you can land, don't drop below the landing speed!!!  This way you can't come to a dead stop in mid-air
						parentPS->speed = MIN_LANDING_SPEED;
					}
				}
			}
			if ( pVeh->m_pVehicleInfo->type == VH_FIGHTER )
			{
				pVeh->m_ucmd.forwardmove = 127;
			}
			else if ( speedMin >= 0 )
			{
				pVeh->m_ucmd.forwardmove = 0;
			}
		}
		//else not accel, decel or braking
		else if ( pVeh->m_pVehicleInfo->throttleSticks )
		{//we're using a throttle that sticks at current speed
			if ( parentPS->speed <= MIN_LANDING_SPEED )
			{//going less than landing speed
				if ( FighterNPCOverValidLandingSurface( pVeh ) )
				{//close to ground and not going very fast
					//slow to a stop if within landing height and not accel/decel/braking
					if ( parentPS->speed > 0 )
					{//slow down
						parentPS->speed -= speedIdleDec;
					}
					else if ( parentPS->speed < 0 )
					{//going backwards, slow down
						parentPS->speed += speedIdleDec;
					}
				}
				else
				{//not over a valid landing surf, but going too slow
					//speed up to idle speed if not over a valid landing surf and not accel/decel/braking
					if ( parentPS->speed < speedIdle )
					{
						parentPS->speed += speedIdleAccel;
						if ( parentPS->speed > speedIdle )
						{
							parentPS->speed = speedIdle;
						}
					}
				}
			}
		}
		else
		{//then speed up or slow down to idle speed
			//accelerate to cruising speed only, otherwise, just coast
			// If they've launched, apply some constant motion.
			if ( (pVeh->m_LandTrace.fraction >= 1.0f //no ground
					|| pVeh->m_LandTrace.plane.normal[2] < MIN_LANDING_SLOPE )//or can't land on ground below us
				&& speedIdle > 0 ) 
			{//not above ground and have an idle speed
				//float fSpeed = pVeh->m_pParentEntity->client->ps.speed;
				if ( parentPS->speed < speedIdle )
				{
					parentPS->speed += speedIdleAccel;
					if ( parentPS->speed > speedIdle )
					{
						parentPS->speed = speedIdle;
					}
				}
				else if ( parentPS->speed > 0 )
				{//slow down
					parentPS->speed -= speedIdleDec;

					if ( parentPS->speed < speedIdle )
					{
						parentPS->speed = speedIdle;
					}
				}
			}
			else//either close to ground or no idle speed
			{//slow to a stop if no idle speed or within landing height and not accel/decel/braking
				if ( parentPS->speed > 0 )
				{//slow down
					parentPS->speed -= speedIdleDec;
				}
				else if ( parentPS->speed < 0 )
				{//going backwards, slow down
					parentPS->speed += speedIdleDec;
				}
			}
		}
	}
	else
	{
		if ( pVeh->m_ucmd.forwardmove < 0 )
		{
			pVeh->m_ucmd.forwardmove = 0;
		}
		if ( pVeh->m_ucmd.upmove < 0 )
		{
			pVeh->m_ucmd.upmove = 0;
		}

#ifndef _JK2MP
		if ( !pVeh->m_pVehicleInfo->strafePerc || (!g_speederControlScheme->value && !pVeh->m_pParentEntity->s.number) )
		{//if in a strafe-capable vehicle, clear strafing unless using alternate control scheme
			pVeh->m_ucmd.rightmove = 0;
		}
#endif
	}

#if 1//This is working now, but there are some transitional jitters... Rich?
//STRAFING==============================================================================
	if ( pVeh->m_pVehicleInfo->strafePerc 
#ifdef QAGAME//only do this check on GAME side, because if it's CGAME, it's being predicted, and it's only predicted if the local client is the driver
		&& pVeh->m_pVehicleInfo->Inhabited( pVeh )//has to have a driver in order to be capable of landing
#endif
		&& !pVeh->m_iRemovedSurfaces
		&& parentPS->electrifyTime<curTime
		&& (pVeh->m_LandTrace.fraction >= 1.0f//no grounf
			||pVeh->m_LandTrace.plane.normal[2] < MIN_LANDING_SLOPE//can't land here
			||parentPS->speed>MIN_LANDING_SPEED)//going too fast to land
		&& pVeh->m_ucmd.rightmove )
	{//strafe
		vec3_t vAngles, vRight;
		float strafeSpeed = (pVeh->m_pVehicleInfo->strafePerc*speedMax)*5.0f;
		VectorCopy( pVeh->m_vOrientation, vAngles );
		vAngles[PITCH] = vAngles[ROLL] = 0;
		AngleVectors( vAngles, NULL, vRight, NULL );

		if ( pVeh->m_ucmd.rightmove > 0 )
		{//strafe right
			//FIXME: this will probably make it possible to cheat and
			//		go faster than max speed if you keep turning and strafing...
			if ( pVeh->m_fStrafeTime > -MAX_STRAFE_TIME )
			{//can strafe right for 2 seconds
				float curStrafeSpeed = DotProduct( parentPS->velocity, vRight );
				if ( curStrafeSpeed > 0.0f )
				{//if > 0, already strafing right
					strafeSpeed -= curStrafeSpeed;//so it doesn't add up
				}
				if ( strafeSpeed > 0 )
				{
					VectorMA( parentPS->velocity, strafeSpeed*pVeh->m_fTimeModifier, vRight, parentPS->velocity );
				}
				pVeh->m_fStrafeTime -= 50*pVeh->m_fTimeModifier;
			}
		}
		else
		{//strafe left
			if ( pVeh->m_fStrafeTime < MAX_STRAFE_TIME )
			{//can strafe left for 2 seconds
				float curStrafeSpeed = DotProduct( parentPS->velocity, vRight );
				if ( curStrafeSpeed < 0.0f )
				{//if < 0, already strafing left
					strafeSpeed += curStrafeSpeed;//so it doesn't add up
				}
				if ( strafeSpeed > 0 )
				{
					VectorMA( parentPS->velocity, -strafeSpeed*pVeh->m_fTimeModifier, vRight, parentPS->velocity );
				}
				pVeh->m_fStrafeTime += 50*pVeh->m_fTimeModifier;
			}
		}
		//strafing takes away from forward speed?  If so, strafePerc above should use speedMax
		//parentPS->speed *= (1.0f-pVeh->m_pVehicleInfo->strafePerc);
	}
	else//if ( pVeh->m_fStrafeTime )
	{
		if ( pVeh->m_fStrafeTime > 0 )
		{
			pVeh->m_fStrafeTime -= 50*pVeh->m_fTimeModifier;
			if ( pVeh->m_fStrafeTime < 0 )
			{
				pVeh->m_fStrafeTime = 0.0f;
			}
		}
		else if ( pVeh->m_fStrafeTime < 0 )
		{
			pVeh->m_fStrafeTime += 50*pVeh->m_fTimeModifier;
			if ( pVeh->m_fStrafeTime > 0 )
			{
				pVeh->m_fStrafeTime = 0.0f;
			}
		}
	}
//STRAFING==============================================================================
#endif

	if ( parentPS->speed > speedMax )
	{
		parentPS->speed = speedMax;
	}
	else if ( parentPS->speed < speedMin )
	{
		parentPS->speed = speedMin;
	}

#ifdef QAGAME//FIXME: get working in GAME and CGAME
	if ((pVeh->m_vOrientation[PITCH]*0.1f) > 10.0f)
	{ //pitched downward, increase speed more and more based on our tilt
		if ( FighterNPCIsInSpace( (gentity_t *)parent ) )
		{//in space, do nothing with speed base on pitch...
		}
		else
		{
			//really should only do this when on a planet
			float mult = pVeh->m_vOrientation[PITCH]*0.1f;
			if (mult < 1.0f)
			{
				mult = 1.0f;
			}
			parentPS->speed = PredictedAngularDecrement(mult, pVeh->m_fTimeModifier*10.0f, parentPS->speed);
		}
	}

	if (pVeh->m_iRemovedSurfaces
		|| parentPS->electrifyTime>=curTime)
	{ //going down
		if ( FighterNPCIsInSpace( (gentity_t *)parent ) )
		{//we're in a valid trigger_space brush
			//simulate randomness
			if ( !(parent->s.number&3) )
			{//even multiple of 3, don't do anything
				parentPS->gravity = 0;
			}
			else if ( !(parent->s.number&2) )
			{//even multiple of 2, go up
				parentPS->gravity = -500.0f;
				parentPS->velocity[2] = 80.0f;
			}
			else
			{//odd number, go down
				parentPS->gravity = 500.0f;
				parentPS->velocity[2] = -80.0f;
			}
		}
		else
		{//over a planet
			parentPS->gravity = 500.0f;
			parentPS->velocity[2] = -80.0f;
		}
	}
	else if ( FighterNPCSuspended( pVeh, parentPS ) )
	{
		parentPS->gravity = 0;
	}
	else if ( (!parentPS->speed||parentPS->speed < speedIdle) && pVeh->m_ucmd.upmove <= 0 )
	{//slowing down or stopped and not trying to take off
		if ( FighterNPCIsInSpace( (gentity_t *)parent ) )
		{//we're in space, stopping doesn't make us drift downward
			if ( FighterNPCOverValidLandingSurface( pVeh ) )
			{//well, there's something below us to land on, so go ahead and lower us down to it
				parentPS->gravity = (speedIdle - parentPS->speed)/4;
			}
		}
		else
		{//over a planet
			parentPS->gravity = (speedIdle - parentPS->speed)/4;
		}
	}
	else
	{
		parentPS->gravity = 0;
	}
#else//FIXME: get above checks working in GAME and CGAME
	parentPS->gravity = 0;
#endif

	/********************************************************************************/
	/*	END Here is where we move the vehicle (forward or back or whatever). END	*/
	/********************************************************************************/
/*
}

extern void BG_VehicleTurnRateForSpeed( Vehicle_t *pVeh, float speed, float *mPitchOverride, float *mYawOverride );
static void FighterNPCWingMalfunctionCheck( Vehicle_t *pVeh, playerState_t *parentPS )
{
	float mPitchOverride = 1.0f;
	float mYawOverride = 1.0f;
	BG_VehicleTurnRateForSpeed( pVeh, parentPS->speed, &mPitchOverride, &mYawOverride );
	//check right wing damage
	if ( (parentPS->brokenLimbs&(1<<SHIPSURF_DAMAGE_RIGHT_HEAVY)) )
	{//right wing has taken heavy damage
		pVeh->m_vOrientation[ROLL] += (sin( pVeh->m_ucmd.serverTime*0.001 )+1.0f)*pVeh->m_fTimeModifier*mYawOverride*50.0f;
	}
	else if ( (parentPS->brokenLimbs&(1<<SHIPSURF_DAMAGE_RIGHT_LIGHT)) )
	{//right wing has taken light damage
		pVeh->m_vOrientation[ROLL] += (sin( pVeh->m_ucmd.serverTime*0.001 )+1.0f)*pVeh->m_fTimeModifier*mYawOverride*12.5f;
	}

	//check left wing damage
	if ( (parentPS->brokenLimbs&(1<<SHIPSURF_DAMAGE_LEFT_HEAVY)) )
	{//left wing has taken heavy damage
		pVeh->m_vOrientation[ROLL] -= (sin( pVeh->m_ucmd.serverTime*0.001 )+1.0f)*pVeh->m_fTimeModifier*mYawOverride*50.0f;
	}
	else if ( (parentPS->brokenLimbs&(1<<SHIPSURF_DAMAGE_LEFT_LIGHT)) )
	{//left wing has taken light damage
		pVeh->m_vOrientation[ROLL] -= (sin( pVeh->m_ucmd.serverTime*0.001 )+1.0f)*pVeh->m_fTimeModifier*mYawOverride*12.5f;
	}

}

static void FighterNPCNoseMalfunctionCheck( Vehicle_t *pVeh, playerState_t *parentPS )
{
	float mPitchOverride = 1.0f;
	float mYawOverride = 1.0f;
	BG_VehicleTurnRateForSpeed( pVeh, parentPS->speed, &mPitchOverride, &mYawOverride );
	//check nose damage
	if ( (parentPS->brokenLimbs&(1<<SHIPSURF_DAMAGE_FRONT_HEAVY)) )
	{//nose has taken heavy damage
		//pitch up and down over time
		pVeh->m_vOrientation[PITCH] += sin( pVeh->m_ucmd.serverTime*0.001 )*pVeh->m_fTimeModifier*mPitchOverride*50.0f;
	}
	else if ( (parentPS->brokenLimbs&(1<<SHIPSURF_DAMAGE_FRONT_LIGHT)) )
	{//nose has taken heavy damage
		//pitch up and down over time
		pVeh->m_vOrientation[PITCH] += sin( pVeh->m_ucmd.serverTime*0.001 )*pVeh->m_fTimeModifier*mPitchOverride*20.0f;
	}
}

static void FighterNPCDamageRoutine( Vehicle_t *pVeh, bgEntity_t *parent, playerState_t *parentPS, playerState_t *riderPS, qboolean isDead )
{
 	if ( !pVeh->m_iRemovedSurfaces )
	{//still in one piece
		if ( pVeh->m_pParentEntity && isDead )
		{//death spiral
			pVeh->m_ucmd.upmove = 0;
			//FIXME: don't bias toward pitching down when not in space
			/*
			if ( FighterIsInSpace( pVeh->m_pParentEntity ) )
			{
			}
			else
			*//*
			if ( !(pVeh->m_pParentEntity->s.number%3) )
			{//NOT everyone should do this
				pVeh->m_vOrientation[PITCH] += pVeh->m_fTimeModifier; 
				if ( !BG_UnrestrainedPitchRoll( riderPS, pVeh ) )
				{
					if ( pVeh->m_vOrientation[PITCH] > 60.0f )  
					{
						pVeh->m_vOrientation[PITCH] = 60.0f;
					}
				}
			}
			else if ( !(pVeh->m_pParentEntity->s.number%2) )
			{
				pVeh->m_vOrientation[PITCH] -= pVeh->m_fTimeModifier; 
				if ( !BG_UnrestrainedPitchRoll( riderPS, pVeh ) )
				{
					if ( pVeh->m_vOrientation[PITCH] > -60.0f )  
					{
						pVeh->m_vOrientation[PITCH] = -60.0f;
					}
				}
			}
			if ( (pVeh->m_pParentEntity->s.number%2) )
			{
				pVeh->m_vOrientation[YAW] += pVeh->m_fTimeModifier;
				pVeh->m_vOrientation[ROLL] += pVeh->m_fTimeModifier*4.0f;
			}
			else
			{
				pVeh->m_vOrientation[YAW] -= pVeh->m_fTimeModifier;
				pVeh->m_vOrientation[ROLL] -= pVeh->m_fTimeModifier*4.0f;
			}
		}
		return;
	}

	//if we get into here we have at least one broken piece
	pVeh->m_ucmd.upmove = 0;
	
	//if you're off the ground and not suspended, pitch down
	//FIXME: not in space!
	if ( pVeh->m_LandTrace.fraction >= 0.1f )
	{
		if ( !FighterNPCSuspended( pVeh, parentPS ) )
		{
			//pVeh->m_ucmd.forwardmove = 0;
			//FIXME: don't bias towards pitching down when in space...
			if ( !(pVeh->m_pParentEntity->s.number%2) )
			{//NOT everyone should do this
				pVeh->m_vOrientation[PITCH] += pVeh->m_fTimeModifier; 
				if ( !BG_UnrestrainedPitchRoll( riderPS, pVeh ) )
				{
					if ( pVeh->m_vOrientation[PITCH] > 60.0f )  
					{
						pVeh->m_vOrientation[PITCH] = 60.0f;
					}
				}
			}
			else if ( !(pVeh->m_pParentEntity->s.number%3) )
			{
				pVeh->m_vOrientation[PITCH] -= pVeh->m_fTimeModifier; 
				if ( !BG_UnrestrainedPitchRoll( riderPS, pVeh ) )
				{
					if ( pVeh->m_vOrientation[PITCH] > -60.0f )  
					{
						pVeh->m_vOrientation[PITCH] = -60.0f;
					}
				}
			}
			//else: just keep going forward
		}
	}
#ifdef QAGAME
	if ( pVeh->m_LandTrace.fraction < 1.0f )
	{ //if you land at all when pieces of your ship are missing, then die
		gentity_t *parent = (gentity_t *)pVeh->m_pParentEntity;
		gentity_t *killer = parent;
#ifdef _JK2MP//only have this info in MP...
		if (parent->client->ps.otherKiller < ENTITYNUM_WORLD &&
			parent->client->ps.otherKillerTime > level.time)
		{
			gentity_t *potentialKiller = &g_entities[parent->client->ps.otherKiller];

			if (potentialKiller->inuse && potentialKiller->client)
			{ //he's valid I guess
				killer = potentialKiller;
			}
		}
#endif
		G_Damage(parent, killer, killer, vec3_origin, parent->client->ps.origin, 99999, DAMAGE_NO_ARMOR, MOD_SUICIDE);
	}
#endif

	if ( ((pVeh->m_iRemovedSurfaces & SHIPSURF_BROKEN_C) ||
		(pVeh->m_iRemovedSurfaces & SHIPSURF_BROKEN_D)) &&
		((pVeh->m_iRemovedSurfaces & SHIPSURF_BROKEN_E) ||
		(pVeh->m_iRemovedSurfaces & SHIPSURF_BROKEN_F)) )
	{ //wings on both side broken
		float factor = 2.0f;
		if ((pVeh->m_iRemovedSurfaces & SHIPSURF_BROKEN_E) &&
			(pVeh->m_iRemovedSurfaces & SHIPSURF_BROKEN_F) &&
			(pVeh->m_iRemovedSurfaces & SHIPSURF_BROKEN_C) &&
			(pVeh->m_iRemovedSurfaces & SHIPSURF_BROKEN_D))
		{ //all wings broken
			factor *= 2.0f;
		}

		if ( !(pVeh->m_pParentEntity->s.number%4)||!(pVeh->m_pParentEntity->s.number%5) )
		{//won't yaw, so increase roll factor
			factor *= 4.0f;
		}

		pVeh->m_vOrientation[ROLL] += (pVeh->m_fTimeModifier*factor); //do some spiralling
	}
	else if ((pVeh->m_iRemovedSurfaces & SHIPSURF_BROKEN_C) ||
		(pVeh->m_iRemovedSurfaces & SHIPSURF_BROKEN_D))
	{ //left wing broken
		float factor = 2.0f;
		if ((pVeh->m_iRemovedSurfaces & SHIPSURF_BROKEN_C) &&
			(pVeh->m_iRemovedSurfaces & SHIPSURF_BROKEN_D))
		{ //if both are broken..
			factor *= 2.0f;
		}

		if ( !(pVeh->m_pParentEntity->s.number%4)||!(pVeh->m_pParentEntity->s.number%5) )
		{//won't yaw, so increase roll factor
			factor *= 4.0f;
		}

		pVeh->m_vOrientation[ROLL] += factor*pVeh->m_fTimeModifier;
	}
	else if ((pVeh->m_iRemovedSurfaces & SHIPSURF_BROKEN_E) ||
		(pVeh->m_iRemovedSurfaces & SHIPSURF_BROKEN_F))
	{ //right wing broken
		float factor = 2.0f;
		if ((pVeh->m_iRemovedSurfaces & SHIPSURF_BROKEN_E) &&
			(pVeh->m_iRemovedSurfaces & SHIPSURF_BROKEN_F))
		{ //if both are broken..
			factor *= 2.0f;
		}

		if ( !(pVeh->m_pParentEntity->s.number%4)||!(pVeh->m_pParentEntity->s.number%5) )
		{//won't yaw, so increase roll factor
			factor *= 4.0f;
		}

		pVeh->m_vOrientation[ROLL] -= factor*pVeh->m_fTimeModifier;
	}
}

#ifdef _JK2MP
void FighterYawAdjust(Vehicle_t *pVeh, playerState_t *riderPS, playerState_t *parentPS)
{
	float angDif = AngleSubtract(pVeh->m_vOrientation[YAW], riderPS->viewangles[YAW]);

	if (parentPS && parentPS->speed)
	{
		float s = parentPS->speed;
		float maxDif = pVeh->m_pVehicleInfo->turningSpeed*0.8f; //magic number hackery

		if (s < 0.0f)
		{
			s = -s;
		}
		angDif *= s/pVeh->m_pVehicleInfo->speedMax;
		if (angDif > maxDif)
		{
			angDif = maxDif;
		}
		else if (angDif < -maxDif)
		{
			angDif = -maxDif;
		}
		pVeh->m_vOrientation[YAW] = AngleNormalize180(pVeh->m_vOrientation[YAW] - angDif*(pVeh->m_fTimeModifier*0.2f));
	}
}

void FighterPitchAdjust(Vehicle_t *pVeh, playerState_t *riderPS, playerState_t *parentPS)
{
	float angDif = AngleSubtract(pVeh->m_vOrientation[PITCH], riderPS->viewangles[PITCH]);

	if (parentPS && parentPS->speed)
	{
		float s = parentPS->speed;
		float maxDif = pVeh->m_pVehicleInfo->turningSpeed*0.8f; //magic number hackery

		if (s < 0.0f)
		{
			s = -s;
		}
		angDif *= s/pVeh->m_pVehicleInfo->speedMax;
		if (angDif > maxDif)
		{
			angDif = maxDif;
		}
		else if (angDif < -maxDif)
		{
			angDif = -maxDif;
		}
		pVeh->m_vOrientation[PITCH] = AngleNormalize360(pVeh->m_vOrientation[PITCH] - angDif*(pVeh->m_fTimeModifier*0.2f));
	}
}
#endif

//MP RULE - ALL PROCESSORIENTCOMMANDS FUNCTIONS MUST BE BG-COMPATIBLE!!!
//If you really need to violate this rule for SP, then use ifdefs.
//By BG-compatible, I mean no use of game-specific data - ONLY use
//stuff available in the MP bgEntity (in SP, the bgEntity is #defined
//as a gentity, but the MP-compatible access restrictions are based
//on the bgEntity structure in the MP codebase) -rww
// ProcessOrientCommands the Vehicle.
static void ProcessOrientCommands( Vehicle_t *pVeh )
{
	/********************************************************************************/
	/*	BEGIN	Here is where make sure the vehicle is properly oriented.	BEGIN	*/
	/********************************************************************************/
/*
	bgEntity_t *parent = pVeh->m_pParentEntity;
	playerState_t *parentPS, *riderPS;
	float angleTimeMod;
#ifdef QAGAME
	const float groundFraction = 0.1f;
#endif
	float	curRoll = 0.0f;
	qboolean isDead = qfalse;
	qboolean isLandingOrLanded = qfalse;
#ifndef _JK2MP//SP
	int curTime = level.time;
#elif defined QAGAME//MP GAME
	int curTime = level.time;
#elif defined CGAME//MP CGAME
	//FIXME: pass in ucmd?  Not sure if this is reliable...
	int curTime = pm->cmd.serverTime;
#endif

#ifdef _JK2MP
	bgEntity_t *rider = NULL;
	if (parent->s.owner != ENTITYNUM_NONE)
	{
		rider = PM_BGEntForNum(parent->s.owner); //&g_entities[parent->r.ownerNum];
	}
#else
	gentity_t *rider = parent->owner;
#endif

#ifdef _JK2MP
	if ( !rider )
#else
	if ( !rider || !rider->client )
#endif
	{
		rider = parent;
	}

#ifdef _JK2MP
	parentPS = parent->playerState;
	riderPS = rider->playerState;
	isDead = (qboolean)((parentPS->eFlags&EF_DEAD)!=0);
#else
	parentPS = &parent->client->ps;
	riderPS = &rider->client->ps;
	isDead = (parentPS->stats[STAT_HEALTH] <= 0 );
#endif

#ifdef _JK2MP
	if ( parentPS->hyperSpaceTime
		&& (curTime - parentPS->hyperSpaceTime) < HYPERSPACE_TIME )
	{//Going to Hyperspace
		VectorCopy( riderPS->viewangles, pVeh->m_vOrientation );
		VectorCopy( riderPS->viewangles, parentPS->viewangles );
		return;
	}
#endif
	
	if ( pVeh->m_iDropTime >= curTime )
	{//you can only YAW during this
		parentPS->viewangles[YAW] = pVeh->m_vOrientation[YAW] = riderPS->viewangles[YAW];
		return;
	}

	angleTimeMod = pVeh->m_fTimeModifier;

	if ( isDead || parentPS->electrifyTime>=curTime ||
		(pVeh->m_pVehicleInfo->surfDestruction &&
		pVeh->m_iRemovedSurfaces &&
		(pVeh->m_iRemovedSurfaces & SHIPSURF_BROKEN_C) &&
		(pVeh->m_iRemovedSurfaces & SHIPSURF_BROKEN_D) &&
		(pVeh->m_iRemovedSurfaces & SHIPSURF_BROKEN_E) &&
		(pVeh->m_iRemovedSurfaces & SHIPSURF_BROKEN_F)) )
	{ //do some special stuff for when all the wings are torn off
		FighterNPCDamageRoutine(pVeh, parent, parentPS, riderPS, isDead);
		pVeh->m_vOrientation[ROLL] = AngleNormalize180( pVeh->m_vOrientation[ROLL] );
		return;
	}

	if ( !BG_UnrestrainedPitchRoll( riderPS, pVeh ) )
	{
		pVeh->m_vOrientation[ROLL] = PredictedAngularDecrement(0.95f, angleTimeMod*2.0f, pVeh->m_vOrientation[ROLL]);
	}

	isLandingOrLanded = (FighterNPCIsLanding( pVeh, parentPS )||FighterNPCIsLanded( pVeh, parentPS ));

	if (!isLandingOrLanded)
	{ //don't do this stuff while landed.. I guess. I don't want ships spinning in place, looks silly.
		int m = 0;
		float aVelDif;
		float dForVel;

		FighterNPCWingMalfunctionCheck( pVeh, parentPS );

		while (m < 3)
		{
			aVelDif = pVeh->m_vFullAngleVelocity[m];

			if (aVelDif != 0.0f)
			{
				dForVel = (aVelDif*0.1f)*pVeh->m_fTimeModifier;
				if (dForVel > 1.0f || dForVel < -1.0f)
				{
					pVeh->m_vOrientation[m] += dForVel;
					pVeh->m_vOrientation[m] = AngleNormalize180(pVeh->m_vOrientation[m]);
					if (m == PITCH)
					{ //don't pitch downward into ground even more.
						if (pVeh->m_vOrientation[m] > 90.0f && (pVeh->m_vOrientation[m]-dForVel) < 90.0f)
						{
							pVeh->m_vOrientation[m] = 90.0f;
							pVeh->m_vFullAngleVelocity[m] = -pVeh->m_vFullAngleVelocity[m];
						}
					}
					if (riderPS)
					{
						riderPS->viewangles[m] = pVeh->m_vOrientation[m];
					}
					pVeh->m_vFullAngleVelocity[m] -= dForVel;
				}
				else
				{
					pVeh->m_vFullAngleVelocity[m] = 0.0f;
				}
			}

			m++;
		}
	}
	else
	{ //clear decr/incr angles once landed.
		VectorClear(pVeh->m_vFullAngleVelocity);
	}

	curRoll = pVeh->m_vOrientation[ROLL];

	// If we're landed, we shouldn't be able to do anything but take off.
	if ( isLandingOrLanded //going slow enough to start landing
		&& !pVeh->m_iRemovedSurfaces 
		&& parentPS->electrifyTime<curTime)//not spiraling out of control
	{
		if ( parentPS->speed > 0.0f )
		{//Uh... what?  Why?
			if ( pVeh->m_LandTrace.fraction < 0.3f )
			{
				pVeh->m_vOrientation[PITCH] = 0.0f;
			}
			else
			{
				pVeh->m_vOrientation[PITCH] = PredictedAngularDecrement(0.83f, angleTimeMod*10.0f, pVeh->m_vOrientation[PITCH]);
			}
		}
		if ( pVeh->m_LandTrace.fraction > 0.1f 
			|| pVeh->m_LandTrace.plane.normal[2] < MIN_LANDING_SLOPE )
		{//off the ground, at least (or not on a valid landing surf)
			// Dampen the turn rate based on the current height.
#ifdef _JK2MP
			FighterYawAdjust(pVeh, riderPS, parentPS);
#else
			pVeh->m_vOrientation[YAW] = riderPS->viewangles[YAW];//*pVeh->m_LandTrace.fraction;
#endif
		}
	}
	else if ( (pVeh->m_iRemovedSurfaces||parentPS->electrifyTime>=curTime)//spiralling out of control
		&& (!(pVeh->m_pParentEntity->s.number%4)||!(pVeh->m_pParentEntity->s.number%5)) )
	{//no yaw control
	}
	else if ( pVeh->m_pPilot && pVeh->m_pPilot->s.number < MAX_CLIENTS && parentPS->speed > 0.0f )//&& !( pVeh->m_ucmd.forwardmove > 0 && pVeh->m_LandTrace.fraction != 1.0f ) )   
	{
		if ( BG_UnrestrainedPitchRoll( riderPS, pVeh ) )
		{
			VectorCopy( riderPS->viewangles, pVeh->m_vOrientation );
			VectorCopy( riderPS->viewangles, parentPS->viewangles );
#ifdef _JK2MP
			//BG_ExternThisSoICanRecompileInDebug( pVeh, riderPS );
#endif

			curRoll = pVeh->m_vOrientation[ROLL];

			FighterNPCNoseMalfunctionCheck( pVeh, parentPS );

			//VectorCopy( pVeh->m_vOrientation, parentPS->viewangles );
		}
		else
		{
			/*
			float fTurnAmt[3];
			//PITCH
			fTurnAmt[PITCH] = riderPS->viewangles[PITCH] * 0.08f;
			//YAW
			fTurnAmt[YAW] = riderPS->viewangles[YAW] * 0.065f;
			fTurnAmt[YAW] *= fTurnAmt[YAW];
			// Dampen the turn rate based on the current height.
			if ( riderPS->viewangles[YAW] < 0 )
			{//must keep it negative because squaring a negative makes it positive
				fTurnAmt[YAW] = -fTurnAmt[YAW];
			}
			fTurnAmt[YAW] *= pVeh->m_LandTrace.fraction;
			//ROLL
			fTurnAmt[2] = 0.0f;
			*/
/*
			//Actal YAW
#ifdef _JK2MP
			FighterYawAdjust(pVeh, riderPS, parentPS);
#else
			pVeh->m_vOrientation[YAW] = riderPS->viewangles[YAW];
#endif

			// If we are not hitting the ground, allow the fighter to pitch up and down.
			if ( !FighterNPCOverValidLandingSurface( pVeh ) 
				|| parentPS->speed > MIN_LANDING_SPEED )
			//if ( ( pVeh->m_LandTrace.fraction >= 1.0f || pVeh->m_ucmd.forwardmove != 0 ) && pVeh->m_LandTrace.fraction >= 0.0f )
			{
				float fYawDelta;

#ifdef _JK2MP
				FighterPitchAdjust(pVeh, riderPS, parentPS);
#else
				pVeh->m_vOrientation[PITCH] = riderPS->viewangles[PITCH]; 
#endif

				FighterNPCNoseMalfunctionCheck( pVeh, parentPS );

				// Adjust the roll based on the turn amount and dampen it a little.
				fYawDelta = AngleSubtract(pVeh->m_vOrientation[YAW], pVeh->m_vPrevOrientation[YAW]); //pVeh->m_vOrientation[YAW] - pVeh->m_vPrevOrientation[YAW];
				if ( fYawDelta > 8.0f ) 
				{
					fYawDelta = 8.0f;
				}
				else if ( fYawDelta < -8.0f )
				{
					fYawDelta = -8.0f;
				}
				curRoll -= fYawDelta;

				curRoll = PredictedAngularDecrement(0.93f, angleTimeMod*2.0f, curRoll);
				//cap it reasonably
				//NOTE: was hardcoded to 40.0f, now using extern data
				if ( pVeh->m_pVehicleInfo->rollLimit != -1 )
				{
					if (curRoll > pVeh->m_pVehicleInfo->rollLimit )
					{
						curRoll = pVeh->m_pVehicleInfo->rollLimit;
					}
					else if (curRoll < -pVeh->m_pVehicleInfo->rollLimit)
					{
						curRoll = -pVeh->m_pVehicleInfo->rollLimit;
					}
				}
			}
		}
	}

	// If you are directly impacting the ground, even out your pitch.
	if ( isLandingOrLanded )    
	{//only if capable of landing
		if ( !isDead 
			&& parentPS->electrifyTime<curTime
			&& (!pVeh->m_pVehicleInfo->surfDestruction || !pVeh->m_iRemovedSurfaces ) )
		{//not crashing or spiralling out of control...
			if ( pVeh->m_vOrientation[PITCH] > 0 )
			{
				pVeh->m_vOrientation[PITCH] = PredictedAngularDecrement(0.2f, angleTimeMod*10.0f, pVeh->m_vOrientation[PITCH]);
			}
			else
			{
				pVeh->m_vOrientation[PITCH] = PredictedAngularDecrement(0.75f, angleTimeMod*10.0f, pVeh->m_vOrientation[PITCH]);
			}
		}
	}


/*
//NOTE: all this is redundant now since we have the FighterDamageRoutine func...
#ifdef _JK2MP //...yeah. Need to send armor across net for prediction to work.
	if ( isDead )
#else
	if ( pVeh->m_iArmor <= 0 )
#endif
	{//going to explode
		//FIXME: maybe make it erratically jerk or spin or start and stop?
#ifndef _JK2MP
		if ( g_speederControlScheme->value > 0 || !rider || rider->s.number )
#else
		if (1)
#endif
		{
			pVeh->m_ucmd.rightmove = Q_irand( -64, 64 );
		}
		else
		{
			pVeh->m_ucmd.rightmove = 0;
		}
		pVeh->m_ucmd.forwardmove = Q_irand( -32, 127 );
		pVeh->m_ucmd.upmove = Q_irand( -127, 127 );
		pVeh->m_vOrientation[YAW] += Q_flrand( -10, 10 );
		pVeh->m_vOrientation[PITCH] += pVeh->m_fTimeModifier; 
		if ( pVeh->m_vOrientation[PITCH] > 60.0f )  
		{
			pVeh->m_vOrientation[PITCH] = 60.0f;
		}
		if ( pVeh->m_LandTrace.fraction != 0.0f )
		{
			parentPS->velocity[2] -= pVeh->m_pVehicleInfo->acceleration * pVeh->m_fTimeModifier;
		}
	}
*//*
	// If no one is in this vehicle and it's up in the sky, pitch it forward as it comes tumbling down.
#ifdef QAGAME //never gonna happen on client anyway, we can't be getting predicted unless the predicting client is boarded
 	if ( !pVeh->m_pVehicleInfo->Inhabited( pVeh ) 
		&& pVeh->m_LandTrace.fraction >= groundFraction 
		&& !FighterNPCIsInSpace( (gentity_t *)parent ) 
		&& !FighterNPCSuspended( pVeh, parentPS ) )
	{
		pVeh->m_ucmd.upmove = 0;
		//pVeh->m_ucmd.forwardmove = 0;
		pVeh->m_vOrientation[PITCH] += pVeh->m_fTimeModifier; 
		if ( !BG_UnrestrainedPitchRoll( riderPS, pVeh ) )
		{
			if ( pVeh->m_vOrientation[PITCH] > 60.0f )  
			{
				pVeh->m_vOrientation[PITCH] = 60.0f;
			}
		}
	}
#endif

	if ( !pVeh->m_fStrafeTime )
	{//use that roll
		pVeh->m_vOrientation[ROLL] = curRoll;
		//NOTE: this seems really backwards...
		if ( pVeh->m_vOrientation[ROLL] )
		{ //continually adjust the yaw based on the roll..
			if ( (pVeh->m_iRemovedSurfaces||parentPS->electrifyTime>=curTime)//spiralling out of control
				&& (!(pVeh->m_pParentEntity->s.number%4)||!(pVeh->m_pParentEntity->s.number%5)) )
			{//leave YAW alone
			}
			else
			{
				if ( !BG_UnrestrainedPitchRoll( riderPS, pVeh ) )
				{
					pVeh->m_vOrientation[YAW] -= ((pVeh->m_vOrientation[ROLL])*0.05f)*pVeh->m_fTimeModifier;
				}
			}
		}
	}
	else
	{//add in strafing roll
		float strafeRoll = (pVeh->m_fStrafeTime/MAX_STRAFE_TIME)*pVeh->m_pVehicleInfo->rollLimit;//pVeh->m_pVehicleInfo->bankingSpeed*
		float strafeDif = AngleSubtract(strafeRoll, pVeh->m_vOrientation[ROLL]);
		pVeh->m_vOrientation[ROLL] += (strafeDif*0.1f)*pVeh->m_fTimeModifier;
		if ( !BG_UnrestrainedPitchRoll( riderPS, pVeh ) )
		{//cap it reasonably
			if ( pVeh->m_pVehicleInfo->rollLimit != -1 
				&& !pVeh->m_iRemovedSurfaces 
				&& parentPS->electrifyTime<curTime)
			{
				if (pVeh->m_vOrientation[ROLL] > pVeh->m_pVehicleInfo->rollLimit )
				{
					pVeh->m_vOrientation[ROLL] = pVeh->m_pVehicleInfo->rollLimit;
				}
				else if (pVeh->m_vOrientation[ROLL] < -pVeh->m_pVehicleInfo->rollLimit)
				{
					pVeh->m_vOrientation[ROLL] = -pVeh->m_pVehicleInfo->rollLimit;
				}
			}
		}
	}

	if (pVeh->m_pVehicleInfo->surfDestruction)
	{
		FighterNPCDamageRoutine(pVeh, parent, parentPS, riderPS, isDead);
	}
	pVeh->m_vOrientation[ROLL] = AngleNormalize180( pVeh->m_vOrientation[ROLL] );
	/********************************************************************************/
	/*	END	Here is where make sure the vehicle is properly oriented.	END			*/
	/********************************************************************************/
/*}

#ifdef QAGAME //ONLY in SP or on server, not cgame

extern void PM_SetAnim(pmove_t	*pm,int setAnimParts,int anim,int setAnimFlags, int blendTime);

// This function makes sure that the vehicle is properly animated.
static void AnimateVehicle( Vehicle_t *pVeh )
{
	int Anim = -1; 
	int iFlags = SETANIM_FLAG_NORMAL, iBlend = 300;
	qboolean isLanding = qfalse, isLanded = qfalse;
#ifdef _JK2MP
	playerState_t *parentPS = pVeh->m_pParentEntity->playerState;
#else
	playerState_t *parentPS = &pVeh->m_pParentEntity->client->ps;
#endif
#ifndef _JK2MP//SP
	//nothing
#elif defined QAGAME//MP GAME
	int curTime = level.time;
#elif defined CGAME//MP CGAME
	//FIXME: pass in ucmd?  Not sure if this is reliable...
	int curTime = pm->cmd.serverTime;
#endif

#ifdef _JK2MP
	if ( parentPS->hyperSpaceTime
		&& curTime - parentPS->hyperSpaceTime < HYPERSPACE_TIME )
	{//Going to Hyperspace
		//close the wings (FIXME: makes sense on X-Wing, not Shuttle?)
		if ( pVeh->m_ulFlags & VEH_WINGSOPEN )
		{
			pVeh->m_ulFlags &= ~VEH_WINGSOPEN;
			Anim = BOTH_WINGS_CLOSE;
		}
	}
	else
#endif
	{
		isLanding = FighterNPCIsLanding( pVeh, parentPS );
		isLanded = FighterNPCIsLanded( pVeh, parentPS );

		// if we're above launch height (way up in the air)... 
		if ( !isLanding && !isLanded ) 
		{
			if ( !( pVeh->m_ulFlags & VEH_WINGSOPEN ) )
			{
				pVeh->m_ulFlags |= VEH_WINGSOPEN;
				pVeh->m_ulFlags &= ~VEH_GEARSOPEN;
				Anim = BOTH_WINGS_OPEN;
			}
		}
		// otherwise we're below launch height and still taking off.
		else
		{
			if ( (pVeh->m_ucmd.forwardmove < 0 || pVeh->m_ucmd.upmove < 0||isLanded)
				&& pVeh->m_LandTrace.fraction <= 0.4f
				&& pVeh->m_LandTrace.plane.normal[2] >= MIN_LANDING_SLOPE )
			{//already landed or trying to land and close to ground
				// Open gears.
				if ( !( pVeh->m_ulFlags & VEH_GEARSOPEN ) )
				{
#ifdef _JK2MP
					if ( pVeh->m_pVehicleInfo->soundLand )
					{//just landed?
#ifdef QAGAME//MP GAME-side
						G_EntitySound( ((gentity_t *)(pVeh->m_pParentEntity)), CHAN_AUTO, pVeh->m_pVehicleInfo->soundLand );
#elif defined CGAME//MP CGAME-side
						//trap_S_StartSound( NULL, pVeh->m_pParentEntity->s.number, CHAN_AUTO, pVeh->m_pVehicleInfo->soundLand );
#endif
					}
#endif
					pVeh->m_ulFlags |= VEH_GEARSOPEN;
					Anim = BOTH_GEARS_OPEN;
				}
			}
			else
			{//trying to take off and almost halfway off the ground
				// Close gears (if they're open).
				if ( pVeh->m_ulFlags & VEH_GEARSOPEN )
				{
					pVeh->m_ulFlags &= ~VEH_GEARSOPEN;
					Anim = BOTH_GEARS_CLOSE;
					//iFlags = SETANIM_FLAG_OVERRIDE | SETANIM_FLAG_HOLD; 
				}
				// If gears are closed, and we are below launch height, close the wings.
				else
				{
					if ( pVeh->m_ulFlags & VEH_WINGSOPEN )
					{
						pVeh->m_ulFlags &= ~VEH_WINGSOPEN;
						Anim = BOTH_WINGS_CLOSE;
					}
				}
			}
		}
	}

	if ( Anim != -1 )
	{
		#ifdef _JK2MP
			BG_SetAnim(pVeh->m_pParentEntity->playerState, bgAllAnims[pVeh->m_pParentEntity->localAnimIndex].anims,
				SETANIM_BOTH, Anim, iFlags, iBlend);
		#else
			NPC_SetAnim( pVeh->m_pParentEntity, SETANIM_BOTH, Anim, iFlags, iBlend );
		#endif
	}
}

// This function makes sure that the rider's in this vehicle are properly animated.
static void AnimateRiders( Vehicle_t *pVeh )
{
}

#endif //game-only

#ifndef QAGAME
void AttachRidersGeneric( Vehicle_t *pVeh );
#endif

void G_SetFighterNPCVehicleFunctions( vehicleInfo_t *pVehInfo )
{
#ifdef QAGAME //ONLY in SP or on server, not cgame
	pVehInfo->AnimateVehicle			=		AnimateVehicle;
	pVehInfo->AnimateRiders				=		AnimateRiders;
//	pVehInfo->ValidateBoard				=		ValidateBoard;
//	pVehInfo->SetParent					=		SetParent;
//	pVehInfo->SetPilot					=		SetPilot;
//	pVehInfo->AddPassenger				=		AddPassenger;
//	pVehInfo->Animate					=		Animate;
	pVehInfo->Board						=		Board;
	pVehInfo->Eject						=		Eject;
//	pVehInfo->EjectAll					=		EjectAll;
//	pVehInfo->StartDeathDelay			=		StartDeathDelay;
//	pVehInfo->DeathUpdate				=		DeathUpdate;
//	pVehInfo->RegisterAssets			=		RegisterAssets;
//	pVehInfo->Initialize				=		Initialize;
	pVehInfo->Update					=		Update;
//	pVehInfo->UpdateRider				=		UpdateRider;
#endif //game-only
	pVehInfo->ProcessMoveCommands		=		ProcessMoveCommands;
	pVehInfo->ProcessOrientCommands		=		ProcessOrientCommands;

#ifndef QAGAME //cgame prediction attachment func
	pVehInfo->AttachRiders				=		AttachRidersGeneric;
#endif
//	pVehInfo->AttachRiders				=		AttachRiders;
//	pVehInfo->Ghost						=		Ghost;
//	pVehInfo->UnGhost					=		UnGhost;
//	pVehInfo->Inhabited					=		Inhabited;
}

// Following is only in game, not in namespace
#ifdef _JK2MP
#include "../namespace_end.h"
#endif

#ifdef QAGAME
extern void G_AllocateVehicleObject(Vehicle_t **pVeh);
#endif

#ifdef _JK2MP
#include "../namespace_begin.h"
#endif

// Create/Allocate a new Animal Vehicle (initializing it as well).
void G_CreateFighterNPC2( Vehicle_t **pVeh, const char *strType )
{
	// Allocate the Vehicle.
#ifdef _JK2MP
#ifdef QAGAME
	//these will remain on entities on the client once allocated because the pointer is
	//never stomped. on the server, however, when an ent is freed, the entity struct is
	//memset to 0, so this memory would be lost..
    G_AllocateVehicleObject(pVeh);
#else
	if (!*pVeh)
	{ //only allocate a new one if we really have to
		(*pVeh) = (Vehicle_t *) BG_Alloc( sizeof(Vehicle_t) );
	}
#endif
	memset(*pVeh, 0, sizeof(Vehicle_t));
#else
	(*pVeh) = (Vehicle_t *) gi.Malloc( sizeof(Vehicle_t), TAG_G_ALLOC, qtrue );
#endif
	(*pVeh)->m_pVehicleInfo = &g_vehicleInfo[BG_VehicleGetIndex( strType )];
}

#ifdef _JK2MP

#include "../namespace_end.h"

//get rid of all the crazy defs we added for this file
#undef currentAngles
#undef currentOrigin
#undef mins
#undef maxs
#undef legsAnimTimer
#undef torsoAnimTimer
#undef bool
#undef false
#undef true

#undef sqrtf
#undef Q_flrand

#undef MOD_EXPLOSIVE
#endif*/



