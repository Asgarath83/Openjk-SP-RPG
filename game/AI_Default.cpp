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

#include "g_headers.h"

#include "Q3_Interface.h"

//#include "anims.h"
//extern int PM_AnimLength( int index, animNumber_t anim );
//extern qboolean PM_HasAnimation( gentity_t *ent, int animation );
//extern int PM_AnimLength( int index, animNumber_t anim );
//#define	MAX_IDLE_ANIMS	8
extern int g_crosshairEntNum;

/*
void NPC_LostEnemyDecideChase(void)

  We lost our enemy and want to drop him but see if we should chase him if we are in the proper bState
*/

void NPC_LostEnemyDecideChase(void)
{
	switch( NPCInfo->behaviorState )
	{
	case BS_HUNT_AND_KILL:
		//We were chasing him and lost him, so try to find him
		if ( NPC->enemy == NPCInfo->goalEntity && NPC->enemy->lastWaypoint != WAYPOINT_NONE )
		{//Remember his last valid Wp, then check it out
			//FIXME: Should we only do this if there's no other enemies or we've got LOCKED_ENEMY on?
			NPC_BSSearchStart( NPC->enemy->lastWaypoint, BS_SEARCH );
		}
		//If he's not our goalEntity, we're running somewhere else, so lose him
		break;
	default:
		break;
	}
	G_ClearEnemy( NPC );
}
/*
-------------------------
NPC_StandIdle
-------------------------
*/

void NPC_StandIdle( void )
{
/*
	//Must be done with any other animations
	if ( NPC->client->ps.legsAnimTimer != 0 )
		return;

	//Not ready to do another one
	if ( TIMER_Done( NPC, "idleAnim" ) == false )
		return;

	int anim = NPC->client->ps.legsAnim;

	if ( anim != BOTH_STAND1 && anim != BOTH_STAND2 )
		return;

	//FIXME: Account for STAND1 or STAND2 here and set the base anim accordingly
	int	baseSeq = ( anim == BOTH_STAND1 ) ? BOTH_STAND1_RANDOM1 : BOTH_STAND2_RANDOM1;

	//Must have at least one random idle animation
	//NOTENOTE: This relies on proper ordering of animations, which SHOULD be okay
	if ( PM_HasAnimation( NPC, baseSeq ) == false )
		return;

	int	newIdle = Q_irand( 0, MAX_IDLE_ANIMS-1 );

	//FIXME: Technically this could never complete.. but that's not really too likely
	while( 1 )
	{
		if ( PM_HasAnimation( NPC, baseSeq + newIdle ) )
			break;

		newIdle = Q_irand( 0, MAX_IDLE_ANIMS );
	}
	
	//Start that animation going
	NPC_SetAnim( NPC, SETANIM_BOTH, baseSeq + newIdle, SETANIM_FLAG_OVERRIDE|SETANIM_FLAG_HOLD );
	
	int newTime = PM_AnimLength( NPC->client->clientInfo.animFileIndex, (animNumber_t) (baseSeq + newIdle) );

	//Don't do this again for a random amount of time
	TIMER_Set( NPC, "idleAnim", newTime + Q_irand( 2000, 10000 ) );
*/
}

qboolean NPC_StandTrackAndShoot (gentity_t *NPC, qboolean canDuck)
{
	qboolean	attack_ok = qfalse;
	qboolean	duck_ok = qfalse;
	qboolean	faced = qfalse;
	float		attack_scale = 1.0;

	//First see if we're hurt bad- if so, duck
	//FIXME: if even when ducked, we can shoot someone, we should.
	//Maybe is can be shot even when ducked, we should run away to the nearest cover?
	if ( canDuck )
	{
		if ( NPC->health < 20 )
		{
		//	if( NPC->svFlags&SVF_HEALING || random() )
			if( random() )
			{
				duck_ok = qtrue;
			}
		}
		else if ( NPC->health < 40 )
		{
//			if ( NPC->svFlags&SVF_HEALING )
//			{//Medic is on the way, get down!
//				duck_ok = qtrue;
//			}
			// no more borg
///			if ( NPC->client->playerTeam!= TEAM_BORG )
//			{//Borg don't care if they're about to die
				//attack_scale will be a max of .66
//				attack_scale = NPC->health/60;
//			}
		}
	}

	//NPC_CheckEnemy( qtrue, qfalse );

	if ( !duck_ok )
	{//made this whole part a function call
		attack_ok = NPC_CheckCanAttack( attack_scale, qtrue );
		faced = qtrue;
	}

	if ( canDuck && (duck_ok || (!attack_ok && client->fireDelay == 0)) && ucmd.upmove != -127 )
	{//if we didn't attack check to duck if we're not already
		if( !duck_ok )
		{
			if ( NPC->enemy->client )
			{
				if ( NPC->enemy->enemy == NPC )
				{
					if ( NPC->enemy->client->buttons & BUTTON_ATTACK )
					{//FIXME: determine if enemy fire angles would hit me or get close
						if ( NPC_CheckDefend( 1.0 ) )//FIXME: Check self-preservation?  Health?
						{
							duck_ok = qtrue;
						}
					}
				}
			}
		}

		if ( duck_ok )
		{//duck and don't shoot
			attack_ok = qfalse;
			ucmd.upmove = -127;
			NPCInfo->duckDebounceTime = level.time + 1000;//duck for a full second
		}
	}

	return faced;
}


void NPC_BSIdle( void ) 
{
	//FIXME if there is no nav data, we need to do something else
	// if we're stuck, try to move around it
	if ( UpdateGoal() )
	{
		NPC_MoveToGoal( qtrue );
	}

	if ( ( ucmd.forwardmove == 0 ) && ( ucmd.rightmove == 0 ) && ( ucmd.upmove == 0 ) )
	{
//		NPC_StandIdle();
	}

	NPC_UpdateAngles( qtrue, qtrue );
	ucmd.buttons |= BUTTON_WALKING;
}

void NPC_BSRun (void)
{
	//FIXME if there is no nav data, we need to do something else
	// if we're stuck, try to move around it
	if ( UpdateGoal() )
	{
		NPC_MoveToGoal( qtrue );
	}

	NPC_UpdateAngles( qtrue, qtrue );
}

void NPC_BSStandGuard (void)
{
	//FIXME: Use Snapshot info
	if ( NPC->enemy == NULL )
	{//Possible to pick one up by being shot
		if( random() < 0.5 )
		{
			if(NPC->client->enemyTeam)
			{
				gentity_t *newenemy = NPC_PickEnemy(NPC, NPC->client->enemyTeam, (NPC->cantHitEnemyCounter < 10), (NPC->client->enemyTeam == TEAM_PLAYER), qtrue);
				//only checks for vis if couldn't hit last enemy
				if(newenemy)
				{
					G_SetEnemy( NPC, newenemy );
				}
			}
		}
	}

	if ( NPC->enemy != NULL )
	{
		if( NPCInfo->tempBehavior == BS_STAND_GUARD )
		{
			NPCInfo->tempBehavior = BS_DEFAULT;
		}
		
		if( NPCInfo->behaviorState == BS_STAND_GUARD )
		{
			NPCInfo->behaviorState = BS_STAND_AND_SHOOT;
		}
	}

	NPC_UpdateAngles( qtrue, qtrue );
}

/*
-------------------------
NPC_BSHuntAndKill
-------------------------
*/

void NPC_BSHuntAndKill( void )
{
	qboolean	turned = qfalse;
	vec3_t		vec;
	float		enemyDist;
	visibility_t	oEVis;
	int			curAnim;

	NPC_CheckEnemy( NPCInfo->tempBehavior != BS_HUNT_AND_KILL, qfalse );//don't find new enemy if this is tempbehav

	if ( NPC->enemy )
	{
		oEVis = enemyVisibility = NPC_CheckVisibility ( NPC->enemy, CHECK_FOV|CHECK_SHOOT );//CHECK_360|//CHECK_PVS|
		if(enemyVisibility > VIS_PVS)
		{
			if ( !NPC_EnemyTooFar( NPC->enemy, 0, qtrue ) )
			{//Enemy is close enough to shoot - FIXME: this next func does this also, but need to know here for info on whether ot not to turn later
				NPC_CheckCanAttack( 1.0, qfalse );
				turned = qtrue;
			}
		}

		curAnim = NPC->client->ps.legsAnim;
		if(curAnim != BOTH_ATTACK1 && curAnim != BOTH_ATTACK2 && curAnim != BOTH_ATTACK3 && curAnim != BOTH_MELEE1 && curAnim != BOTH_MELEE2 )
		{//Don't move toward enemy if we're in a full-body attack anim
			//FIXME, use IdealDistance to determin if we need to close distance
			VectorSubtract(NPC->enemy->currentOrigin, NPC->currentOrigin, vec);
			enemyDist = VectorLength(vec);
			if( enemyDist > 48 && ((enemyDist*1.5)*(enemyDist*1.5) >= NPC_MaxDistSquaredForWeapon() ||
				oEVis != VIS_SHOOT ||
				//!(ucmd.buttons & BUTTON_ATTACK) || 
				enemyDist > IdealDistance(NPC)*3 ) )
			{//We should close in?
				NPCInfo->goalEntity = NPC->enemy;

				NPC_MoveToGoal( qtrue );
			}
			else if(enemyDist < IdealDistance(NPC))
			{//We should back off?
				//if(ucmd.buttons & BUTTON_ATTACK)
				{
					NPCInfo->goalEntity = NPC->enemy;
					NPCInfo->goalRadius = 12;
					NPC_MoveToGoal( qtrue );

					ucmd.forwardmove *= -1;
					ucmd.rightmove *= -1;
					VectorScale( NPC->client->ps.moveDir, -1, NPC->client->ps.moveDir );

					ucmd.buttons |= BUTTON_WALKING;
				}
			}//otherwise, stay where we are
		}
	}
	else 
	{//ok, stand guard until we find an enemy
		if( NPCInfo->tempBehavior == BS_HUNT_AND_KILL )
		{
			NPCInfo->tempBehavior = BS_DEFAULT;
		}
		else
		{
			NPCInfo->tempBehavior = BS_STAND_GUARD;
			NPC_BSStandGuard();
		}
		return;
	}

	if(!turned)
	{
		NPC_UpdateAngles(qtrue, qtrue);
	}
}

void NPC_BSStandAndShoot (void)
{
	//FIXME:
	//When our numbers outnumber enemies 3 to 1, or only one of them,
	//go into hunt and kill mode

	//FIXME:
	//When they're all dead, go to some script or wander off to sickbay?
	
	if(NPC->client->playerTeam && NPC->client->enemyTeam)
	{
		//FIXME: don't realize this right away- or else enemies show up and we're standing around
		/*
		if( teamNumbers[NPC->enemyTeam] == 0 )
		{//ok, stand guard until we find another enemy
			//reset our rush counter
			teamCounter[NPC->playerTeam] = 0;
			NPCInfo->tempBehavior = BS_STAND_GUARD;
			NPC_BSStandGuard();
			return;
		}*/
		/*
		//FIXME: whether to do this or not should be settable
		else if( NPC->playerTeam != TEAM_BORG )//Borg don't rush
		{
		//FIXME: In case reinforcements show up, we should wait a few seconds
		//and keep checking before rushing!
		//Also: what if not everyone on our team is going after playerTeam?
		//Also: our team count includes medics!
			if(NPC->health > 25)
			{//Can we rush the enemy?
				if(teamNumbers[NPC->enemyTeam] == 1 ||
					teamNumbers[NPC->playerTeam] >= teamNumbers[NPC->enemyTeam]*3)
				{//Only one of them or we outnumber 3 to 1
					if(teamStrength[NPC->playerTeam] >= 75 ||
						(teamStrength[NPC->playerTeam] >= 50 && teamStrength[NPC->playerTeam] > teamStrength[NPC->enemyTeam]))
					{//Our team is strong enough to rush
						teamCounter[NPC->playerTeam]++;
						if(teamNumbers[NPC->playerTeam] * 17 <= teamCounter[NPC->playerTeam])
						{//ok, we waited 1.7 think cycles on average and everyone is go, let's do it!
							//FIXME: Should we do this to everyone on our team?
							NPCInfo->behaviorState = BS_HUNT_AND_KILL;
							//FIXME: if the tide changes, we should retreat!
							//FIXME: when do we reset the counter?
							NPC_BSHuntAndKill ();
							return;
						}
					}
					else//Oops!  Something's wrong, reset the counter to rush
						teamCounter[NPC->playerTeam] = 0;
				}
				else//Oops!  Something's wrong, reset the counter to rush
					teamCounter[NPC->playerTeam] = 0;
			}
		}
		*/
	}

	NPC_CheckEnemy(qtrue, qfalse);
	
	if(NPCInfo->duckDebounceTime > level.time && NPC->client->ps.weapon != WP_SABER )
	{
		ucmd.upmove = -127;
		if(NPC->enemy)
		{
			NPC_CheckCanAttack(1.0, qtrue);
		}
		return;		
	}

	if(NPC->enemy)
	{
		if(!NPC_StandTrackAndShoot( NPC, qtrue ))
		{//That func didn't update our angles
			NPCInfo->desiredYaw = NPC->client->ps.viewangles[YAW];
			NPCInfo->desiredPitch = NPC->client->ps.viewangles[PITCH];
			NPC_UpdateAngles(qtrue, qtrue);
		}
	}
	else
	{
		NPCInfo->desiredYaw = NPC->client->ps.viewangles[YAW];
		NPCInfo->desiredPitch = NPC->client->ps.viewangles[PITCH];
		NPC_UpdateAngles(qtrue, qtrue);
//		NPC_BSIdle();//only moves if we have a goal
	}
}

void NPC_BSRunAndShoot (void)
{
	/*if(NPC->playerTeam && NPC->enemyTeam)
	{
		//FIXME: don't realize this right away- or else enemies show up and we're standing around
		if( teamNumbers[NPC->enemyTeam] == 0 )
		{//ok, stand guard until we find another enemy
			//reset our rush counter
			teamCounter[NPC->playerTeam] = 0;
			NPCInfo->tempBehavior = BS_STAND_GUARD;
			NPC_BSStandGuard();
			return;
		}
	}*/

	//NOTE: are we sure we want ALL run and shoot people to move this way?
	//Shouldn't it check to see if we have an enemy and our enemy is our goal?!
	//Moved that check into NPC_MoveToGoal
	//NPCInfo->combatMove = qtrue;

	NPC_CheckEnemy( qtrue, qfalse );
	
	if ( NPCInfo->duckDebounceTime > level.time ) // && NPCInfo->hidingGoal )
	{
		ucmd.upmove = -127;
		if ( NPC->enemy )
		{
			NPC_CheckCanAttack( 1.0, qfalse );
		}
		return;		
	}

	if ( NPC->enemy )
	{
		int monitor = NPC->cantHitEnemyCounter;
		NPC_StandTrackAndShoot( NPC, qfalse );//(NPCInfo->hidingGoal != NULL) );

		if ( !(ucmd.buttons & BUTTON_ATTACK) && ucmd.upmove >= 0 && NPC->cantHitEnemyCounter > monitor )
		{//not crouching and not firing
			vec3_t	vec;

			VectorSubtract( NPC->enemy->currentOrigin, NPC->currentOrigin, vec );
			vec[2] = 0;
			if ( VectorLength( vec ) > 128 || NPC->cantHitEnemyCounter >= 10 )
			{//run at enemy if too far away
				//The cantHitEnemyCounter getting high has other repercussions
				//100 (10 seconds) will make you try to pick a new enemy... 
				//But we're chasing, so we clamp it at 50 here
				if ( NPC->cantHitEnemyCounter > 60 )
				{
					NPC->cantHitEnemyCounter = 60;
				}
				
				if ( NPC->cantHitEnemyCounter >= (NPCInfo->stats.aggression+1) * 10 )
				{
					NPC_LostEnemyDecideChase();
				}

				//chase and face
				ucmd.angles[YAW] = 0;
				ucmd.angles[PITCH] = 0;
				NPCInfo->goalEntity = NPC->enemy;
				NPCInfo->goalRadius = 12;
				NPC_MoveToGoal( qtrue );
				NPC_UpdateAngles(qtrue, qtrue);
			}
			else
			{
				//FIXME: this could happen if they're just on the other side
				//of a thin wall or something else blocking out shot.  That
				//would make us just stand there and not go around it...
				//but maybe it's okay- might look like we're waiting for
				//him to come out...?  
				//Current solution: runs around if cantHitEnemyCounter gets
				//to 10 (1 second).  
			}
		}
		else
		{//Clear the can't hit enemy counter here
			NPC->cantHitEnemyCounter = 0;
		}
	}
	else
	{
		if ( NPCInfo->tempBehavior == BS_HUNT_AND_KILL )
		{//lost him, go back to what we were doing before
			NPCInfo->tempBehavior = BS_DEFAULT;
			return;
		}

//		NPC_BSRun();//only moves if we have a goal
	}
}

//Simply turn until facing desired angles
void NPC_BSFace (void)
{
	//FIXME: once you stop sending turning info, they reset to whatever their delta_angles was last????
	//Once this is over, it snaps back to what it was facing before- WHY???
	if( NPC_UpdateAngles ( qtrue, qtrue ) )
	{
		Q3_TaskIDComplete( NPC, TID_BSTATE );
		
		NPCInfo->desiredYaw = client->ps.viewangles[YAW];
		NPCInfo->desiredPitch = client->ps.viewangles[PITCH];

		NPCInfo->aimTime = 0;//ok to turn normally now
	}
}

void NPC_BSPointShoot (qboolean shoot)
{//FIXME: doesn't check for clear shot...
	vec3_t	muzzle, dir, angles, org;

	if ( !NPC->enemy || !NPC->enemy->inuse || (NPC->enemy->NPC && NPC->enemy->health <= 0) )
	{//FIXME: should still keep shooting for a second or two after they actually die...
		Q3_TaskIDComplete( NPC, TID_BSTATE );
		goto finished;
		return;
	}

	CalcEntitySpot(NPC, SPOT_WEAPON, muzzle);
	CalcEntitySpot(NPC->enemy, SPOT_HEAD, org);//Was spot_org
	//Head is a little high, so let's aim for the chest:
	if ( NPC->enemy->client )
	{
		org[2] -= 12;//NOTE: is this enough?
	}

	VectorSubtract(org, muzzle, dir);
	vectoangles(dir, angles);

	switch( NPC->client->ps.weapon )
	{
	case WP_NONE:
//	case WP_TRICORDER:
	case WP_MELEE:
	case WP_TUSKEN_STAFF:
	case WP_SABER:
		//don't do any pitch change if not holding a firing weapon
		break;
	default:
		NPCInfo->desiredPitch = NPCInfo->lockedDesiredPitch = AngleNormalize360(angles[PITCH]);
		break;
	}

	NPCInfo->desiredYaw = NPCInfo->lockedDesiredYaw = AngleNormalize360(angles[YAW]);

	if ( NPC_UpdateAngles ( qtrue, qtrue ) )
	{//FIXME: if angles clamped, this may never work!
		//NPCInfo->shotTime = NPC->attackDebounceTime = 0;

		if ( shoot )
		{//FIXME: needs to hold this down if using a weapon that requires it, like phaser...
			ucmd.buttons |= BUTTON_ATTACK;
		}
		
		if ( !shoot || !(NPC->svFlags & SVF_LOCKEDENEMY) )
		{//If locked_enemy is on, dont complete until it is destroyed...
			Q3_TaskIDComplete( NPC, TID_BSTATE );
			goto finished;
		}
	}
	else if ( shoot && (NPC->svFlags & SVF_LOCKEDENEMY) )
	{//shooting them till their dead, not aiming right at them yet...
		/*
		qboolean movingTarget = qfalse;

		if ( NPC->enemy->client )
		{
			if ( VectorLengthSquared( NPC->enemy->client->ps.velocity ) )
			{
				movingTarget = qtrue;
			}
		}
		else if ( VectorLengthSquared( NPC->enemy->s.pos.trDelta ) )
		{
			movingTarget = qtrue;
		}

		if (movingTarget )
		*/
		{
			float	dist = VectorLength( dir );
			float	yawMiss, yawMissAllow = NPC->enemy->maxs[0];
			float	pitchMiss, pitchMissAllow = (NPC->enemy->maxs[2] - NPC->enemy->mins[2])/2;
			
			if ( yawMissAllow < 8.0f )
			{
				yawMissAllow = 8.0f;
			}

			if ( pitchMissAllow < 8.0f )
			{
				pitchMissAllow = 8.0f;
			}

			yawMiss = tan(DEG2RAD(AngleDelta ( NPC->client->ps.viewangles[YAW], NPCInfo->desiredYaw ))) * dist;
			pitchMiss = tan(DEG2RAD(AngleDelta ( NPC->client->ps.viewangles[PITCH], NPCInfo->desiredPitch))) * dist;

			if ( yawMissAllow >= yawMiss && pitchMissAllow > pitchMiss )
			{
				ucmd.buttons |= BUTTON_ATTACK;
			}
		}
	}
	
	return;
		
finished:
	NPCInfo->desiredYaw = client->ps.viewangles[YAW];
	NPCInfo->desiredPitch = client->ps.viewangles[PITCH];

	NPCInfo->aimTime = 0;//ok to turn normally now
}

/*
void NPC_BSMove(void)
Move in a direction, face another
*/
void NPC_BSMove(void)
{
	gentity_t	*goal = NULL;

	NPC_CheckEnemy(qtrue, qfalse);
	if(NPC->enemy)
	{
		NPC_CheckCanAttack(1.0, qfalse);
	}
	else
	{
		NPC_UpdateAngles(qtrue, qtrue);
	}

	goal = UpdateGoal();
	if(goal)
	{
//		NPCInfo->moveToGoalMod = 1.0;

		NPC_SlideMoveToGoal();
	}
}

/*
void NPC_BSShoot(void)
Move in a direction, face another
*/

void NPC_BSShoot(void)
{
//	NPC_BSMove();

	enemyVisibility = VIS_SHOOT;

	if ( client->ps.weaponstate != WEAPON_READY && client->ps.weaponstate != WEAPON_FIRING ) 
	{
		client->ps.weaponstate = WEAPON_READY;
	}

	WeaponThink(qtrue);
}

/*
void NPC_BSPatrol( void ) 

  Same as idle, but you look for enemies every "vigilance"
  using your angles, HFOV, VFOV and visrange, and listen for sounds within earshot...
*/
void NPC_BSPatrol( void ) 
{
	//int	alertEventNum;

	if(level.time > NPCInfo->enemyCheckDebounceTime)
	{
		NPCInfo->enemyCheckDebounceTime = level.time + (NPCInfo->stats.vigilance * 1000);
		NPC_CheckEnemy(qtrue, qfalse);
		if(NPC->enemy)
		{//FIXME: do anger script
			NPCInfo->behaviorState = BS_HUNT_AND_KILL;
			//NPC_AngerSound();
			return;
		}
	}

	//FIXME: Implement generic sound alerts
	/*
	alertEventNum = NPC_CheckAlertEvents( qtrue, qtrue );
	if( alertEventNum != -1 )
	{//If we heard something, see if we should check it out
		if ( NPC_CheckInvestigate( alertEventNum ) )
		{
			return;
		}
	}
	*/

	NPCInfo->investigateSoundDebounceTime = 0;
	//FIXME if there is no nav data, we need to do something else
	// if we're stuck, try to move around it
	if ( UpdateGoal() )
	{
		NPC_MoveToGoal( qtrue );
	}

	NPC_UpdateAngles( qtrue, qtrue );

	ucmd.buttons |= BUTTON_WALKING;
}

/*
void NPC_BSDefault(void)
	uses various scriptflags to determine how an npc should behave
*/
extern void NPC_CheckGetNewWeapon( void );
extern void NPC_BSST_Attack( void );

void NPC_BSDefault( void ) 
{
//	vec3_t		enemyDir;
//	float		enemyDist;
//	float		shootDist;
//	qboolean	enemyFOV = qfalse;
//	qboolean	enemyShotFOV = qfalse;
//	qboolean	enemyPVS = qfalse;
//	vec3_t		enemyHead;
//	vec3_t		muzzle;
//	qboolean	enemyLOS = qfalse;
//	qboolean	enemyCS = qfalse;
	qboolean	move = qtrue;
//	qboolean	shoot = qfalse;

	
	if( NPCInfo->scriptFlags & SCF_FIRE_WEAPON )
	{
		WeaponThink( qtrue );
	}

	if ( NPCInfo->scriptFlags & SCF_FORCED_MARCH )
	{//being forced to walk
		if( NPC->client->ps.torsoAnim != TORSO_SURRENDER_START )
		{
			NPC_SetAnim( NPC, SETANIM_TORSO, TORSO_SURRENDER_START, SETANIM_FLAG_HOLD );
		}
	}
	//look for a new enemy if don't have one and are allowed to look, validate current enemy if have one
	NPC_CheckEnemy( (NPCInfo->scriptFlags&SCF_LOOK_FOR_ENEMIES), qfalse );
	if ( !NPC->enemy )
	{//still don't have an enemy
		if ( !(NPCInfo->scriptFlags&SCF_IGNORE_ALERTS) )
		{//check for alert events
			//FIXME: Check Alert events, see if we should investigate or just look at it
			int alertEvent = NPC_CheckAlertEvents( qtrue, qtrue, -1, qtrue, AEL_DISCOVERED );

			//There is an event to look at
			if ( alertEvent >= 0 )//&& level.alertEvents[alertEvent].ID != NPCInfo->lastAlertID )
			{//heard/saw something
				if ( level.alertEvents[alertEvent].level >= AEL_DISCOVERED && (NPCInfo->scriptFlags&SCF_LOOK_FOR_ENEMIES) )
				{//was a big event
					if ( level.alertEvents[alertEvent].owner 
						&& level.alertEvents[alertEvent].owner != NPC
						&& level.alertEvents[alertEvent].owner->client 
						&& level.alertEvents[alertEvent].owner->health >= 0 
						&& level.alertEvents[alertEvent].owner->client->playerTeam == NPC->client->enemyTeam )
					{//an enemy
						G_SetEnemy( NPC, level.alertEvents[alertEvent].owner );
					}
				}
				else
				{//FIXME: investigate lesser events
				}
			}
			//FIXME: also check our allies' condition?
		}
	}

	if ( NPC->enemy && !(NPCInfo->scriptFlags&SCF_FORCED_MARCH) )
	{
		// just use the stormtrooper attack AI...
		NPC_CheckGetNewWeapon();
		if ( NPC->client->leader 
			&& NPCInfo->goalEntity == NPC->client->leader 
			&& !Q3_TaskIDPending( NPC, TID_MOVE_NAV ) )
		{
			NPC_ClearGoal();
		}
		NPC_BSST_Attack();
		return;
/*
		//have an enemy
		//FIXME: if one of these fails, meaning we can't shoot, do we really need to do the rest?
		VectorSubtract( NPC->enemy->currentOrigin, NPC->currentOrigin, enemyDir );
		enemyDist = VectorNormalize( enemyDir );
		enemyDist *= enemyDist;
		shootDist = NPC_MaxDistSquaredForWeapon();

		enemyFOV = InFOV( NPC->enemy, NPC, NPCInfo->stats.hfov, NPCInfo->stats.vfov );
		enemyShotFOV = InFOV( NPC->enemy, NPC, 20, 20 );
		enemyPVS = gi.inPVS( NPC->enemy->currentOrigin, NPC->currentOrigin );

		if ( enemyPVS )
		{//in the pvs
			trace_t	tr;

			CalcEntitySpot( NPC->enemy, SPOT_HEAD, enemyHead );
			enemyHead[2] -= Q_flrand( 0.0f, NPC->enemy->maxs[2]*0.5f );
			CalcEntitySpot( NPC, SPOT_WEAPON, muzzle );
			enemyLOS = NPC_ClearLOS( muzzle, enemyHead );

			gi.trace ( &tr, muzzle, vec3_origin, vec3_origin, enemyHead, NPC->s.number, MASK_SHOT );
			enemyCS = NPC_EvaluateShot( tr.entityNum, qtrue );
		}
		else
		{//skip thr 2 traces since they would have to fail
			enemyLOS = qfalse;
			enemyCS = qfalse;
		}
		
		if ( enemyCS && enemyShotFOV )
		{//can hit enemy if we want
			NPC->cantHitEnemyCounter = 0;
		}
		else
		{//can't hit
			NPC->cantHitEnemyCounter++;
		}

		if ( enemyCS && enemyShotFOV && enemyDist < shootDist )
		{//can shoot
			shoot = qtrue;
			if ( NPCInfo->goalEntity == NPC->enemy )
			{//my goal is my enemy and I have a clear shot, no need to chase right now
				move = qfalse;
			}
		}
		else
		{//don't shoot yet, keep chasing
			shoot = qfalse;
			move = qtrue;
		}

		//shoot decision
		if ( !(NPCInfo->scriptFlags&SCF_DONT_FIRE) )
		{//try to shoot
			if ( NPC->enemy )
			{
				if ( shoot )
				{
					if( !(NPCInfo->scriptFlags & SCF_FIRE_WEAPON) ) // we've already fired, no need to do it again here
					{
						WeaponThink( qtrue );
					}
				}
			}
		}

		//chase decision
		if ( NPCInfo->scriptFlags & SCF_CHASE_ENEMIES )
		{//go after him
			NPCInfo->goalEntity = NPC->enemy;
			//FIXME: don't need to chase when have a clear shot and in range?
			if ( !enemyCS && NPC->cantHitEnemyCounter > 60 )
			{//haven't been able to shoot enemy for about 6 seconds, need to do something
				//FIXME: combat points?  Just chase?
				if ( enemyPVS )
				{//in my PVS, just pick a combat point
					//FIXME: implement
				}
				else
				{//just chase him
				}
			}
			//FIXME: in normal behavior, should we use combat Points?  Do we care?  Is anyone actually going to ever use this AI?
		}
		else if ( NPC->cantHitEnemyCounter > 60 )
		{//pick a new one
			NPC_CheckEnemy( qtrue, qfalse );
		}
		
		if ( enemyPVS && enemyLOS )//&& !enemyShotFOV )
		{//have a clear LOS to him//, but not looking at him
			//Find the desired angles
			vec3_t	angles;

			GetAnglesForDirection( muzzle, enemyHead, angles );

			NPCInfo->desiredYaw		= AngleNormalize180( angles[YAW] );
			NPCInfo->desiredPitch	= AngleNormalize180( angles[PITCH] );
		}
		*/
	}

	if ( UpdateGoal() )
	{//have a goal
		if ( !NPC->enemy 
			&& NPC->client->leader 
			&& NPCInfo->goalEntity == NPC->client->leader 
			&& !Q3_TaskIDPending( NPC, TID_MOVE_NAV ) )
		{
			NPC_BSFollowLeader();
		}
		else
		{
			//set angles
			if ( (NPCInfo->scriptFlags & SCF_FACE_MOVE_DIR) || NPCInfo->goalEntity != NPC->enemy )
			{//face direction of movement, NOTE: default behavior when not chasing enemy
				NPCInfo->combatMove = qfalse;
			}
			else
			{//face goal.. FIXME: what if have a navgoal but want to face enemy while moving?  Will this do that?
				vec3_t	dir, angles;

				NPCInfo->combatMove = qfalse;

				VectorSubtract( NPCInfo->goalEntity->currentOrigin, NPC->currentOrigin, dir );
				vectoangles( dir, angles );
				NPCInfo->desiredYaw = angles[YAW];
				if ( NPCInfo->goalEntity == NPC->enemy )
				{
					NPCInfo->desiredPitch = angles[PITCH];
				}
			}

			//set movement
			//override default walk/run behavior
			//NOTE: redundant, done in NPC_ApplyScriptFlags
			if ( NPCInfo->scriptFlags & SCF_RUNNING )
			{
				ucmd.buttons &= ~BUTTON_WALKING;
			}
			else if ( NPCInfo->scriptFlags & SCF_WALKING )
			{
				ucmd.buttons |= BUTTON_WALKING;
			}
			else if ( NPCInfo->goalEntity == NPC->enemy )
			{
				ucmd.buttons &= ~BUTTON_WALKING;
			}
			else
			{
				ucmd.buttons |= BUTTON_WALKING;
			}

			if ( NPCInfo->scriptFlags & SCF_FORCED_MARCH )
			{//being forced to walk
				if ( g_crosshairEntNum != NPC->s.number )
				{//don't walk if player isn't aiming at me
					move = qfalse;
				}
			}

			if ( move )
			{
				//move toward goal
				NPC_MoveToGoal( qtrue );
			}
		}
	}
	else if ( !NPC->enemy && NPC->client->leader )
	{
		NPC_BSFollowLeader();
	}

	//update angles
	NPC_UpdateAngles( qtrue, qtrue );
}

//////////////////////////////////////////////////////////////////////////////////
//////////////////// SPECIAL AI COMBAT FOR SHOOTERS NPC ////////////////////////////////////
// WORKING ALPHA VERSION 

//////////////////////////////////////////////////////////////////////////////////
//////////////////// SPECIAL AI COMBAT FOR SHOOTERS NPC ////////////////////////////////////
void		NPC_Precache( void );
void		NPC_DustFallNear(const vec3_t origin, int dustcount);
void		NPC_ChangeWeapon2(int wp);
qboolean	NPC_StopKnockdown(gentity_t *self, gentity_t *pusher, const vec3_t pushDir, qboolean forceKnockdown = qfalse);
// Flight Related Functions (used Only by flying classes)
//--------------------------------------------------------
extern qboolean	NPC_Flying( gentity_t *self );
extern void		NPC_FlyStart( gentity_t *self );
extern void		NPC_FlyStop( gentity_t *self );
// Called From NPC_Pain()
//-----------------------------
void		NPC_Pain2( gentity_t *self, gentity_t *inflictor, int damage, int mod);


// Local: Other Tactics
//----------------------
void		NPC_DoAmbushWait( gentity_t *self);
void		NPC_DoSniper( gentity_t *self);

// Local: Respawning 
//-------------------
bool		NPC_Respawn();

// Called From Within AI_Jedi && AI_Seeker
//-----------------------------------------
void		NPC_Fire();
void		NPC_FireDecide();

// Local: Called From Tactics()
//----------------------------
void		NPC_TacticsSelect();
bool		NPC_CanSeeEnemy( gentity_t *self );


// Called From NPC_RunBehavior()
//-------------------------------
void		NPC_Update();		// Always Called First, Before Any Other Thinking
bool		NPC_Tactics();		// If returns true, Jedi and Seeker AI not used
bool		NPC_Flee();		// If returns true, Jedi and Seeker AI not used
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
extern qboolean heavyWeap(int);
extern qboolean blasterWeap(int);
extern int ChooseBestWeapon(gentity_t* ent);
extern int ChooseWeaponRandom(gentity_t *ent, int wpnGroup);
extern qboolean lightBlasterWeap(int);
extern qboolean heavyBlasterWeap(int);
extern qboolean meleeWeap(int);

////////////////////////////////////////////////////////////////////////////////////////
// Defines
////////////////////////////////////////////////////////////////////////////////////////
#define		NPC_SHOOTRANGEMIN			512
#define		NPC_SHOOTRANGEMED			1024 // DISTANCE FOR SWITCHING BY SABER TO SHOOTING
#define		NPC_SHOOTRANGEMAX			65536

////////////////////////////////////////////////////////////////////////////////////////
// Global Data 
////////////////////////////////////////////////////////////////////////////////////////
bool	NPCHadDeathScript = false;
bool	NPCActive = false;
vec3_t	NPCFootStepLoc;
int		NPCFootStepCount = 0;
vec3_t	AverageEnemyDirection2;
int		AverageEnemyDirectionSamples2;



////////////////////////////////////////////////////////////////////////////////////////
// NPC NEW AGILITY SYSTEM FOR AVOID ATTACKS! 
////////////////////////////////////////////////////////////////////////////////////////

enum	NpcTacticsState
{
	NPC_NONE,

	// Attack 
	//--------
	NPC_GLYPHIC,	// Replace Rifle, alternate medium distance atk 
	NPC_MAGIC,		// Replace Rifle, medium distance atk  
	NPC_ARROWS,     // Replace sniper, large distance atk archers  
	NPC_NEC_HOLY,	// replace missile, large distance atk 
	NPC_WARRIOR,	// atk for short distance > Glyphic magic 
	NPC_FENCER,      // saberist NPC! 
	NPC_BURNER,	// cannonier NPC 
	/// Waiting
	//---------
	NPC_AMBUSHWAIT,		// Goto CP & Wait
	NPC_MAX
};

void NPC_Precache( void )
{
	G_SoundIndex( "sound/chars/boba/bf_blast-off.wav" );
	G_SoundIndex( "sound/chars/boba/bf_jetpack_lp.wav" );
	G_SoundIndex( "sound/chars/boba/bf_land.wav" );
	G_SoundIndex( "sound/weapons/boba/bf_flame.mp3" );
	G_SoundIndex( "sound/player/footsteps/boot1" );
	G_SoundIndex( "sound/player/footsteps/boot2" );
	G_SoundIndex( "sound/player/footsteps/boot3" );
	G_SoundIndex( "sound/player/footsteps/boot4" );
	G_EffectIndex( "boba/jetSP" );
	G_EffectIndex( "boba/fthrw" );
	G_EffectIndex( "volumetric/black_smoke" );
	G_EffectIndex( "chunks/dustFall" );

	AverageEnemyDirectionSamples2 = 0;
	VectorClear(AverageEnemyDirection2);
	NPCHadDeathScript			= false;
	NPCActive					= true;
	NPCFootStepCount			= 0;
}

////////////////////////////////////////////////////////////////////////////////////////
// 
////////////////////////////////////////////////////////////////////////////////////////
void	NPC_DustFallNear(const vec3_t origin, int dustcount)
{
	if (!NPCActive)
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

void NPC_ChangeWeapon2( int wp )
{
	if ( NPC->s.weapon == wp )
	{
		return;
	}
	NPC_ChangeWeapon2( wp );
	G_AddEvent( NPC, EV_GENERAL_SOUND, G_SoundIndex( "sound/weapons/change.wav" ));
}

qboolean NPC_StopKnockdown( gentity_t *self, gentity_t *pusher, const vec3_t pushDir, qboolean forceKnockdown )
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

qboolean NPC_Flying( gentity_t *self )
{
	assert(self && self->client && self->client->NPC_class==CLASS_BIRD ||
		self && self->client && self->client->NPC_class==CLASS_POLTER ||
		self && self->client && self->client->NPC_class==CLASS_RAZIELHIM ||
		self && self->client && self->client->NPC_class==CLASS_ESKANDOR ||	
		self && self->client && self->client->NPC_class==CLASS_REAPER ||
		self && self->client && self->client->NPC_class==CLASS_ARIEL ||
		self && self->client && self->client->NPC_class==CLASS_VAMPIRE_GHOST ||
		self && self->client && self->client->NPC_class==CLASS_BATFORM ||
		self && self->client && self->client->NPC_class==CLASS_BATKAIN ||
		self && self->client && self->client->NPC_class==CLASS_BATKAIN2 ||
		self && self->client && self->client->NPC_class==CLASS_BATKAIN3 ||
		self && self->client && self->client->NPC_class==CLASS_BATVORADOR ||
		self && self->client && self->client->NPC_class==CLASS_BATVAMPIRE_QUEEN ||
		self && self->client && self->client->NPC_class==CLASS_BATVAMPIRE_FROST || 
		self && self->client && self->client->NPC_class==CLASS_BATNECROMANCER ||
		self && self->client && self->client->NPC_class==CLASS_BATVAMPIRE_WATER ||
		self && self->client && self->client->NPC_class==CLASS_BATVAMPIRE_ELF ||
		self && self->client && self->client->NPC_class==CLASS_BATVAMPIRE_ELF2 ||
		self && self->client && self->client->NPC_class==CLASS_BATVAMPIRE_ELF3 ||
		self && self->client && self->client->NPC_class==CLASS_HELLBAT ||
		self && self->client && self->client->NPC_class==CLASS_BATVAMPIRE_FEAR ||
		self && self->client && self->client->NPC_class==CLASS_BATVAMPIRE_FEAR2 ||
		self && self->client && self->client->NPC_class==CLASS_JANOS ||
		self && self->client->NPC_class == CLASS_KAIN3 ||
		self && self->client->NPC_class == CLASS_AIR ||
		self && self->->client->NPC_class == CLASS_VOID ||
		self && self->->client->NPC_class == CLASS_DARK ||
		self && self->->client->NPC_class == CLASS_LIGHT ||
		self && self->->client->NPC_class == CLASS_GOLEM_AIR ||
		self && self->->client->NPC_class == CLASS_GOLEM_VOID );//self->NPC && 
	return ((qboolean)(self->client->moveType==MT_FLYSWIM));
// ONLY FOR FLYING CLASSES 
}

////////////////////////////////////////////////////////////////////////////////////////
// 
////////////////////////////////////////////////////////////////////////////////////////
bool	NPC_CanSeeEnemy( gentity_t *self )
{
	assert(self && self->NPC && self->client && self->client->NPC_class==CLASS_BOBAFETT || 
		self && self->NPC && self->client && self->client->NPC_class==CLASS_WIZARD 
		);
 	return ((level.time - self->NPC->enemyLastSeenTime)<1000);
}

////////////////////////////////////////////////////////////////////////////////////////
// 
////////////////////////////////////////////////////////////////////////////////////////
void	NPC_Pain( gentity_t *self, gentity_t *inflictor, int damage, int mod)
{
	if ( mod==MOD_SABER || mod !=MOD_SABER /*!(NPCInfo->aiFlags&NPCAI_FLAMETHROW)*/)
	{
		TIMER_Set( self, "NPC_TacticsSelect", 0);	// Hurt By The Saber, Time To Try Something New
	}
	/*if (self->NPC->aiFlags&NPCAI_FLAMETHROW)
	{
		NPC_SetAnim( self, SETANIM_TORSO, BOTH_FORCELIGHTNING_HOLD, SETANIM_FLAG_OVERRIDE|SETANIM_FLAG_HOLD );
	 	self->client->ps.torsoAnimTimer  =	level.time - TIMER_Get(self, "falmeTime");
	}*/
}

////////////////////////////////////////////////////////////////////////////////////////
// 
////////////////////////////////////////////////////////////////////////////////////////
void NPC_FlyStart( gentity_t *self )
{//switch to seeker AI for a while
	if ( TIMER_Done( self, "jetRecharge" ) 
		&& !NPC_Flying( self ) )
	{
		self->client->ps.gravity = 0;
		self->svFlags |= SVF_CUSTOM_GRAVITY;
		self->client->moveType = MT_FLYSWIM;
		//start jet effect
		self->client->jetPackTime = level.time + Q_irand( 3000, 10000 );
			//take-off sound
		G_SoundOnEnt( self, CHAN_ITEM, "sound/chars/boba/bf_blast-off.wav" );
	
		/*if ( self->client->NPC_class == CLASS_BIRD ) 
		{
			//G_SoundOnEnt( self, CHAN_ITEM, "sound/chars/boba/bf_blast-off.wav" );
		}
		else 
		{
			G_SoundOnEnt( self, CHAN_ITEM, "sound/chars/boba/bf_blast-off.wav" );
		}*/
		
		//jet loop sound
		self->s.loopSound = G_SoundIndex( "sound/chars/boba/bf_jetpack_lp.wav" );
	/*
		if (self->client->NPC_class == CLASS_BIRD )
		{
			//self->s.loopSound = G_SoundIndex( "sound/chars/boba/bf_jetpack_lp.wav" );
		}
		else 
		{
			self->s.loopSound = G_SoundIndex( "sound/chars/boba/bf_jetpack_lp.wav" );
		}*/

		
		
		
		if ( self->NPC )
		{
			self->count = Q3_INFINITE; // SEEKER shot ammo count
		}
	}
}

////////////////////////////////////////////////////////////////////////////////////////
// 
////////////////////////////////////////////////////////////////////////////////////////
void NPC_FlyStop( gentity_t *self )
{
	self->client->ps.gravity = g_gravity->value;
	self->svFlags &= ~SVF_CUSTOM_GRAVITY;
	self->client->moveType = MT_RUNJUMP;
	//Stop effect
	self->client->jetPackTime = 0;
	//stop jet loop sound
	G_SoundOnEnt( self, CHAN_ITEM, "sound/chars/boba/bf_land.wav" );
	/*	
	if ( self->client->NPC_class == CLASS_BIRD ) 
		{
			G_SoundOnEnt( self, CHAN_ITEM, "sound/chars/boba/bf_land.wav" );
		}
	else 
		{
			G_SoundOnEnt( self, CHAN_ITEM, "sound/chars/boba/bf_land.wav" );
		}*/

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
void		NPC_DoAmbushWait( gentity_t *self)
{
  NPC_TacticsSelect(); 
}

////////////////////////////////////////////////////////////////////////////////////////
// 
////////////////////////////////////////////////////////////////////////////////////////
void		NPC_DoSniper( gentity_t *self)
{
	if (TIMER_Done(NPC, "PickNewSniperPoint"))
	{
		TIMER_Set(NPC, "PickNewSniperPoint", Q_irand(15000, 25000));
 		int		SniperPoint = NPC_FindCombatPoint(NPC->currentOrigin, 0, NPC->currentOrigin, CP_SNIPE|CP_CLEAR|CP_HAS_ROUTE|CP_TRYFAR|CP_HORZ_DIST_COLL, 0, -1);
		if (SniperPoint!=-1)
		{
			NPC_SetCombatPoint(SniperPoint);
			NPC_SetMoveGoal( NPC, level.combatPoints[SniperPoint].origin, 20, qtrue, SniperPoint );
		}
	}

    if (Distance(NPC->currentOrigin, level.combatPoints[NPCInfo->combatPoint].origin)<50.0f)
	{
		NPC_FireDecide();
	}
	else 
	{
		NPC_FireDecide();
	}

	bool	IsOnAPath = !!NPC_MoveToGoal(qtrue);

	// Resolve Blocked Problems
	//--------------------------
	if (NPCInfo->aiFlags&NPCAI_BLOCKED && 
		NPC->client->moveType!=MT_FLYSWIM && 
		((level.time - NPCInfo->blockedDebounceTime)>3000)
		)
	{
		////NPC_Printf("BLOCKED: Attempting Jump");
		if (IsOnAPath)
		{
			if (!NPC_TryJump(NPCInfo->blockedTargetPosition))
			{
//				//NPC_Printf("  Failed");
			}
		}
	}

	NPC_FaceEnemy(qtrue);
	NPC_UpdateAngles( qtrue, qtrue );
}


////////////////////////////////////////////////////////////////////////////////////////
// Call This function to make Boba actually shoot his current weapon
////////////////////////////////////////////////////////////////////////////////////////
void	NPC_Fire()
{
	WeaponThink(qtrue);

	// If Actually Fired, Decide To Apply Alt Fire And Calc Next Attack Delay
	//------------------------------------------------------------------------
	if (ucmd.buttons&BUTTON_ATTACK)
	{
		// DUSTY ADD - THIS WORK WITHOUT TROUBLE! 

		if (heavyWeap(NPC->s.weapon))
		{
		if (NPC->s.weapon == WP_ROCKET_LAUNCHER)
				TIMER_Set(NPC, "nextAttackDelay", Q_irand(1000, 2000));

			// Occasionally Shoot A Homing Missile
			//-------------------------------------
			if (!Q_irand(0, 3))
			{
				ucmd.buttons &= ~BUTTON_ATTACK;
				ucmd.buttons |= BUTTON_ALT_ATTACK;
				if (NPC->s.weapon == WP_ROCKET_LAUNCHER)
					NPC->client->fireDelay = Q_irand(1000, 3000);
			}
						
		}
		else if ( NPC->s.weapon == WP_TUSKEN_RIFLE || NPC->s.weapon == WP_STUN_BATON ||
		NPC->s.weapon == WP_NOGHRI_STICK ) 
		{
			NPC->client->fireDelay = Q_irand(1000, 3000); // NO ALT FIRE FOR THESE WEAPONS
		}
		else if (NPC->s.weapon == WP_DISRUPTOR)
		{
			TIMER_Set(NPC, "nextAttackDelay", Q_irand(1000, 4000));

			// Occasionally Alt-Fire
			//-----------------------

			if (!Q_irand(0, 3))
			{
				ucmd.buttons &= ~BUTTON_ATTACK;
				ucmd.buttons |= BUTTON_ALT_ATTACK;
				NPC->client->fireDelay = Q_irand(1000, 3000);;
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


////////////////////////////////////////////////////////////////////////////////////////
// Call this function to see if Fett should fire his current weapon
////////////////////////////////////////////////////////////////////////////////////////
//helper function
qboolean isShooterClass(int className)
{
	if (NPC->client->NPC_class != CLASS_BOBAFETT || NPC->client->NPC_class != CLASS_MANDA || NPC->client->NPC_class == CLASS_COMMANDO ||
		NPC->client->NPC_class == CLASS_WIZARD )// Add other special classes )
		return qtrue;

	return qfalse;
}

void NPC_FireDecide( void )
{
	// Any Reason Not To Shoot?
	//--------------------------
	if (!NPC ||											// Only NPCs
		!NPC->client ||									// Only Clients
		 //NPC->client->NPC_class!=CLASS_BOBAFETT ||		// Only Boba
		!isShooterClass(NPC->client->NPC_class) ||		    // Only Boba 
		!NPC->enemy ||									// Only If There Is An Enemy
		NPC->s.weapon == WP_NONE ||				// Only If Using A Valid Weapon
		!TIMER_Done(NPC, "nextAttackDelay") ||			// Only If Ready To Shoot Again
		!NPC_CanSeeEnemy(NPC)		// Only If Enemy Recently Seen
		)
	{
		return;
	}

	// Now Check Weapon Specific Parameters To See If We Should Shoot Or Not
	//-----------------------------------------------------------------------
	// GOOD CODE 
	// DUSTY VARIANTE 
	if (heavyWeap(NPC->s.weapon) || blasterWeap(NPC->s.weapon) ||  
		lightBlasterWeap(NPC->s.weapon) || heavyBlasterWeap(NPC->s.weapon)||
		meleeWeap(NPC->s.weapon))
	{
	
	// HERE PUT THE SWITCH FOR CLASS RAZIEL SWORD 	
	if (Distance(NPC->currentOrigin, NPC->enemy->currentOrigin) > 400.0f)
		{
			NPC_Fire();
		}
	    else 
		{
			NPC_Fire();
		}
	}
	else
	{
		NPC_Fire();
	}
}

////////////////////////////////////////////////////////////////////////////////////////
// Tactics avaliable to Boba Fett:
// --------------------------------
//	NPC_MAGIC,			// Uses arcane magic, medium range
//	NPC_GLYPHIC,		// Uses Glyphic weapon, medium range
//	NPC_ARROWS, 		// Uses Arrows, large range of atk 
//  NPC_NEC_HOLY, 		// Uses powerful magic, far range
//  NPC_WARRIOR,		// Use melee atk. 
//	NPC_AMBUSHWAIT,		// Goto CP & Wait
//
extern void WP_DeactivateSaber( gentity_t *self, qboolean clearLength );
////////////////////////////////////////////////////////////////////////////////////////
void	NPC_TacticsSelect()
{
	// Don't Change Tactics For A Little While
	//------------------------------------------
	TIMER_Set(NPC, "NPC_TacticsSelect", Q_irand(8000, 15000));
	int		nextState = NPCInfo->localState;


	// Get Some Data That Will Help With The Selection Of The Next Tactic
	//--------------------------------------------------------------------
	bool	enemyAlive2			= (NPC->enemy->health>0);
	float	enemyDistance2		= Distance(NPC->currentOrigin, NPC->enemy->currentOrigin);
	bool	enemyCloseRange2	= (enemyDistance2<NPC_SHOOTRANGEMIN);
	bool	enemyInShootRange2	= (enemyDistance2>NPC_SHOOTRANGEMIN && enemyDistance2<NPC_SHOOTRANGEMAX);
	bool	enemyRecentlySeen2	= NPC_CanSeeEnemy(NPC);
	
	if (!enemyAlive2)
	{
		nextState = NPC_MAGIC;
	}
	else if (enemyCloseRange2)// Enemy is pretty near, so use Saber!  
	{
		if ( HaveWeapon(NPC, WP_SABER ))// If Enemy is near, and NPC have a sword in his inventory, using that sword. 
		{
			nextState = NPC_FENCER;
		}
		else if ( HaveWeapon(NPC, WEAPS_MELEE ))// If Enemy is near, and NPC have a sword in his inventory, using that sword. 
		{
			nextState = NPC_WARRIOR;
		}
				
		// If It's Been Long Enough Since Our Last Flame Blast, Try To Torch The Enemy
		//-----------------------------------------------------------------------------
		//if (TIMER_Done(NPC, "nextFlameDelay"))
		// DUSTY ADD
		else if (TIMER_Done(NPC, "nextFlameDelay") && (NPC->client->NPC_class == CLASS_BOBAFETT || NPC->client->NPC_class == CLASS_MANDA))
		{
			nextState = NPC_GLYPHIC;
		}

		// Otherwise, He's Probably Too Close, So Try To Get Clear Of Him
		//----------------------------------------------------------------
		else
		{
			nextState = NPC_MAGIC;
		}
	}
	else if (enemyInShootRange2)// Enemy is distance so use shoot atk
	{
		
	if ( HaveWeapon(NPC, WP_SABER ) || HaveWeapon(NPC, WP_MELEE ) ||
		HaveWeapon(NPC, WP_TUSKEN_STAFF ) || HaveWeapon(NPC, WP_STUN_BATON ))
		// You are a warrior and have a sword, but enemy is far and at shoot distance
		// Put away the sword and use shoot weapons!  
	{
		if ( HaveWeapon(NPC, WEAPS_BLASTER ))// Enemy is distance, you are a wizard? shoot with magic 
		{
			nextState = NPC_MAGIC;
		}
		else if ( HaveWeapon(NPC, WEAPS_HEAVY ))// Enemy is distance? You are an Hunter? use glyphic weapons 
		{
			nextState = NPC_GLYPHIC;
		}
		else if ( HaveWeapon(NPC, WEAPS_LIGHTBLASTER ))// You are an archer? use arrows!  
		{
			nextState = NPC_ARROWS;
		}
		else if ( HaveWeapon(NPC, WEAPS_HEAVYBLASTER ))// You are a Necromancer or a chieric? Use this attacks!  
		{
			nextState = NPC_NEC_HOLY;
		}	
	}	
	else 
	{ // If you have shooting weapons, use that! 
		if ( HaveWeapon(NPC, WEAPS_BLASTER ))// Enemy is distance, you are a wizard? shoot with magic 
		{
			nextState = NPC_MAGIC;
		}
		else if ( HaveWeapon(NPC, WEAPS_HEAVY ))// Enemy is distance? You are an Hunter? use glyphic weapons 
		{
			nextState = NPC_GLYPHIC;
		}
		else if ( HaveWeapon(NPC, WEAPS_LIGHTBLASTER ))// You are an archer? use arrows!  
		{
			nextState = NPC_ARROWS;
		}
		else if ( HaveWeapon(NPC, WEAPS_HEAVYBLASTER ))// You are a Necromancer or a chieric? Use this attacks!  
		{
			nextState = NPC_NEC_HOLY;
		}
		else if ( HaveWeapon(NPC, WEAPS_MELEE ))// this is really stupid but okay...  
		{
			nextState = NPC_WARRIOR;
		}
	}
		
	}
	else if (!enemyInShootRange2 && !enemyCloseRange2) // Enemy is pretty far, try distance atk
	{
		// If It's Been Long Enough Since Our Last Flame Blast, Try To Torch The Enemy
		//-----------------------------------------------------------------------------
		//if (TIMER_Done(NPC, "nextFlameDelay"))
		// DUSTY ADD
		if ( HaveWeapon(NPC, WP_SABER ) || HaveWeapon(NPC, WP_MELEE ) ||
		HaveWeapon(NPC, WP_TUSKEN_STAFF ) || HaveWeapon(NPC, WP_STUN_BATON ))
		// You are a warrior and have a sword, but enemy is far and at shoot distance
		// Put away the sword and use shoot weapons!  
	{
		if ( HaveWeapon(NPC, WEAPS_BLASTER ))// Enemy is distance, you are a wizard? shoot with magic 
		{
			nextState = NPC_MAGIC;
		}
		else if ( HaveWeapon(NPC, WEAPS_HEAVY ))// Enemy is distance? You are an Hunter? use glyphic weapons 
		{
			nextState = NPC_GLYPHIC;
		}
		else if ( HaveWeapon(NPC, WEAPS_LIGHTBLASTER ))// You are an archer? use arrows!  
		{
			nextState = NPC_ARROWS;
		}
		else if ( HaveWeapon(NPC, WEAPS_HEAVYBLASTER ))// You are a Necromancer or a chieric? Use this attacks!  
		{
			nextState = NPC_NEC_HOLY;
		}	
	}	
	else 
	{ // If you have shooting weapons, use that! 
		if ( HaveWeapon(NPC, WEAPS_BLASTER ))// Enemy is distance, you are a wizard? shoot with magic 
		{
			nextState = NPC_MAGIC;
		}
		else if ( HaveWeapon(NPC, WEAPS_HEAVY ))// Enemy is distance? You are an Hunter? use glyphic weapons 
		{
			nextState = NPC_GLYPHIC;
		}
		else if ( HaveWeapon(NPC, WEAPS_LIGHTBLASTER ))// You are an archer? use arrows!  
		{
			nextState = NPC_ARROWS;
		}
		else if ( HaveWeapon(NPC, WEAPS_HEAVYBLASTER ))// You are a Necromancer or a chieric? Use this attacks!  
		{
			nextState = NPC_NEC_HOLY;
		}
		else if ( HaveWeapon(NPC, WEAPS_MELEE ))// this is really stupid but okay...  
		{
			nextState = NPC_WARRIOR;
		}
	}
			
	
	}


	// Recently Saw The Enemy, Time For Some Good Ole Fighten!
	//---------------------------------------------------------
	else if (enemyRecentlySeen2)
	{
		// At First, Boba will prefer to use his blaster against the player, but
		//  the more times he is driven away (NPC->count), he will be less likely to
		//  choose the blaster, and more likely to go for the missile launcher
		if (!enemyCloseRange2 && NPC->client->ps.weapon == WP_SABER )
		{
		  nextState = NPC_MAGIC; 
		}
		nextState = (!enemyCloseRange2 || Q_irand(0, NPC->count)<1)?(NPC_FENCER):(NPC_MAGIC);  
		//nextState = (!enemyInShootRange || Q_irand(0, NPC->count)<1)?(NPC_MAGIC):(NPC_FENCER);


	}

	// Hmmm...  Havn't Seen The Player In A While, We Might Want To Try Something Sneaky
	//-----------------------------------------------------------------------------------
	else
	{
		bool	SnipePointsNear = false;		 // TODO
		bool	AmbushPointNear = false;		 // TODO

		if (Q_irand(0, NPC->count)>0)
		{
			int		SniperPoint = NPC_FindCombatPoint(NPC->currentOrigin, 0, NPC->currentOrigin, CP_SNIPE|CP_CLEAR|CP_HAS_ROUTE|CP_TRYFAR|CP_HORZ_DIST_COLL, 0, -1);
			if (SniperPoint!=-1)
			{
				NPC_SetCombatPoint(SniperPoint);
				NPC_SetMoveGoal( NPC, level.combatPoints[SniperPoint].origin, 20, qtrue, SniperPoint );
				TIMER_Set(NPC, "PickNewSniperPoint", Q_irand(15000, 25000));
				SnipePointsNear = true;
			}
		}

		// DUSTY ADD 
 		//if (SnipePointsNear && TIMER_Done(NPC, "NPC_NoSniperTime"))
		if (SnipePointsNear && TIMER_Done(NPC, "NPC_NoSniperTime") && HaveWeapon(NPC, WP_DISRUPTOR))
		{
			TIMER_Set(NPC, "NPC_NoSniperTime", 120000);				// Don't snipe again for a while
			TIMER_Set(NPC, "NPC_TacticsSelect", Q_irand(35000, 45000));// More patience here
			nextState = NPC_ARROWS;
		}
		else if (AmbushPointNear)
		{
			TIMER_Set(NPC, "NPC_TacticsSelect", Q_irand(15000, 25000));// More patience here
			nextState = NPC_AMBUSHWAIT;
		}
		else
		{
			nextState = (!enemyInShootRange2 || Q_irand(0, NPC->count)<1)?(NPC_GLYPHIC):(NPC_ARROWS);
		}
	}



	// The Next State Has Been Selected, Now Change Weapon If Necessary 
	//------------------------------------------------------------------
	// DUSTY ADD VERSION 
	// The Next State Has Been Selected, Now Change Weapon If Necessary
 	//------------------------------------------------------------------
	NPCInfo->localState = nextState;
	int weapon = 0;

	switch (NPCInfo->localState)
 	{
	case NPC_FENCER: 
		if (HaveWeapon(NPC, WP_SABER))
 		{
			//NPC_Printf("NEW TACTIC: Sniper");
			NPC_ChangeWeapon(WP_SABER);
 			break;
		}
		
	case NPC_MAGIC:	
		weapon = ChooseWeaponRandom(NPC, WEAPS_BLASTER);
		if (weapon)
		{
 			//NPC_Printf("NEW TACTIC: Rifle");
			NPC_ChangeWeapon(weapon);
 			break;
		}
	case NPC_NEC_HOLY:
		weapon = ChooseWeaponRandom(NPC, WEAPS_HEAVYBLASTER);
		if (weapon)
		{
 			//NPC_Printf("NEW TACTIC: Rocket Launcher");
			NPC_ChangeWeapon(weapon);
 			break;
		}
		
	case NPC_GLYPHIC: //kinda stuck at this point if doesn't have flamethrower
		//NPC_Printf("NEW TACTIC: Flame Thrower");
		weapon = ChooseWeaponRandom(NPC, WEAPS_HEAVY);
		if (weapon)
		{
 			//NPC_Printf("NEW TACTIC: Rocket Launcher");
			NPC_ChangeWeapon(weapon);
 			break;
		}
	case NPC_WARRIOR: //kinda stuck at this point if doesn't have flamethrower
		//NPC_Printf("NEW TACTIC: Flame Thrower");
		weapon = ChooseWeaponRandom(NPC, WEAPS_MELEE);
		if (weapon)
		{
 			//NPC_Printf("NEW TACTIC: Rocket Launcher");
			NPC_ChangeWeapon(weapon);
 			break;
		}
	case NPC_ARROWS: //kinda stuck at this point if doesn't have flamethrower
		//NPC_Printf("NEW TACTIC: Flame Thrower");
		weapon = ChooseWeaponRandom(NPC, WEAPS_LIGHTBLASTER);
		if (weapon)
		{
 			//NPC_Printf("NEW TACTIC: Rocket Launcher");
			NPC_ChangeWeapon(weapon);
 			break;
		}
		
	case NPC_AMBUSHWAIT:
		//NPC_Printf("NEW TACTIC: Ambush");
		weapon = ChooseWeaponRandom(NPC, WEAPS_ALL);
		if (weapon)
		{
 			//NPC_Printf("NEW TACTIC: Rocket Launcher");
			NPC_ChangeWeapon(weapon);
 			break;
		}
	}	
 }

////////////////////////////////////////////////////////////////////////////////////////
// Tactics
//
// This function is called right after Update()
// If returns true, Jedi and Seeker AI not used for movement
//
////////////////////////////////////////////////////////////////////////////////////////
bool	NPC_Tactics()
{
	if (!NPC->enemy)
	{
		return false;
	}

	// Think About Changing Tactics
	//------------------------------
	if (TIMER_Done(NPC, "NPC_TacticsSelect"))
	{
		NPC_TacticsSelect();
	}

	// These Tactics Require Seeker & Jedi Movement
	//----------------------------------------------
	if (!NPCInfo->localState ||
		 NPCInfo->localState==NPC_MAGIC || 
		 NPCInfo->localState==NPC_NEC_HOLY ||
		 NPCInfo->localState==NPC_FENCER|| 
		 NPCInfo->localState==NPC_ARROWS ||
		 NPCInfo->localState==NPC_WARRIOR || 
		 NPCInfo->localState==NPC_GLYPHIC )
	{
		return false;
	}

	// Flame Thrower - Locked In Place
	//---------------------------------
	if (NPCInfo->localState==NPC_GLYPHIC )
	{
		NPC_TacticsSelect();
	}

	// Sniper - Move Around, And Take Shots
	//--------------------------------------
	else if (NPCInfo->localState==NPC_ARROWS)
	{
		NPC_DoSniper( NPC );
	}

	// Ambush Wait
	//------------
	else if (NPCInfo->localState==NPC_AMBUSHWAIT)
	{
		NPC_DoAmbushWait( NPC );
	}


	NPC_FacePosition( NPC->enemy->currentOrigin, qtrue);
	NPC_UpdateAngles(qtrue, qtrue);

	return true;			// Do Not Use Normal Jedi Or Seeker Movement
}


////////////////////////////////////////////////////////////////////////////////////////
// 
////////////////////////////////////////////////////////////////////////////////////////
bool	NPC_Respawn()
{
	int cp = -1;

	// Try To Predict Where The Enemy Is Going
	//-----------------------------------------
	if (AverageEnemyDirectionSamples2 && NPC->behaviorSet[BSET_DEATH]==0)
	{
		vec3_t	endPos;
	 	VectorMA(NPC->enemy->currentOrigin, 1000.0f / (float)AverageEnemyDirectionSamples2, AverageEnemyDirection2, endPos);
		cp = NPC_FindCombatPoint(endPos, 0, endPos, CP_FLEE|CP_TRYFAR|CP_HORZ_DIST_COLL, 0, -1);
		//NPC_Printf("Attempting Predictive Spawn Point");
	}

	// If That Failed, Try To Go Directly To The Enemy
	//-------------------------------------------------
	if (cp==-1)
	{
		cp = NPC_FindCombatPoint(NPC->enemy->currentOrigin, 0, NPC->enemy->currentOrigin, CP_FLEE|CP_TRYFAR|CP_HORZ_DIST_COLL, 0, -1);
		//NPC_Printf("Attempting Closest Current Spawn Point");
	}

	// If We've Found One, Go There
	//------------------------------
	if (cp!=-1)
	{
		NPC_SetCombatPoint( cp );
		NPCInfo->surrenderTime = 0;
		NPC->health = NPC->max_health;
		NPC->svFlags &=~SVF_NOCLIENT;
		NPC->count ++;										// This is the number of times spawned
		G_SetOrigin(NPC, level.combatPoints[cp].origin);

		AverageEnemyDirectionSamples2 = 0;
		VectorClear(AverageEnemyDirection2);

		//NPC_Printf("Found Spawn Point (%d)", cp);
		return true;
	}

	assert(0);	// Yea, that's bad...
	//NPC_Printf("FAILED TO FIND SPAWN POINT");
	return false;
}


////////////////////////////////////////////////////////////////////////////////////////
// 
////////////////////////////////////////////////////////////////////////////////////////
void	NPC_Update()
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

			bool	wasSeen = NPC_CanSeeEnemy(NPC);

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

		if (!NPCInfo->surrenderTime)
		{
			if ((level.time - NPCInfo->enemyLastSeenTime)>20000 && TIMER_Done(NPC, "TooLongGoneRespawn"))
			{
				TIMER_Set(NPC, "TooLongGoneRespawn", 30000);	// Give him some time to get to you before trying again
				//NPC_Printf("Gone Too Long, Attempting Respawn Even Though Not Hiding");
				NPC_Respawn();
			}
		}
	}


	// Make Sure He Always Appears In The Last Area With Full Health When His Death Script Is Turned On
	//--------------------------------------------------------------------------------------------------
	if (!NPCHadDeathScript && NPC->behaviorSet[BSET_DEATH]!=0)
	{
		if (!gi.inPVS(NPC->enemy->currentOrigin, NPC->currentOrigin))
		{
			//NPC_Printf("Attempting Final Battle Spawn...");
			if (NPC_Respawn())
			{
				NPCHadDeathScript = true;
			}
			else
			{
				//NPC_Printf("Failed");
			}
		}
	}
	
	// WORK IN PROGRESS 
	// The following elementals creatures spawn theyself with a graphical looping effect
	/*if ( NPC->client->NPC_class == CLASS_DARK )
	{
		G_SoundOnEnt( NPC, CHAN_ITEM, "sound/chars/elemental/shadow.wav" );
		NPC->s.loopSound = G_SoundIndex( "sound/chars/elemental/shadow_lp.wav" );
		G_PlayEffect( G_EffectIndex( "elemental/shadow" ), NPC->playerModel, NPC->chestBolt, NPC->s.number, NPC->currentOrigin, qtrue, qtrue );
		//NPC_Printf("Failed");
		if ( NPC->health <= 0 ) 
		{// If NPC is dead, turn off effect and loop sound
			G_StopEffect( "elemental/shadow", NPC->playerModel, NPC->chestBolt, NPC->s.number );
		}
	}*/

		
			
		
		
			



	// Occasionally A Jump Turns Into A Rocket Fly
	//---------------------------------------------
	if ( NPC->client->ps.groundEntityNum == ENTITYNUM_NONE 
		&& NPC->client->ps.forceJumpZStart && NPC->client->NPC_class == CLASS_BIRD
		&& !Q_irand( 0, 10 ) ||
		// REAPERS EVER FLYING 
		NPC->client->ps.groundEntityNum == ENTITYNUM_NONE 
		&& NPC->client->ps.forceJumpZStart && NPC->client->NPC_class == CLASS_REAPER ||
		// POLTERGEIST E ARIEL 
		NPC->client->ps.groundEntityNum == ENTITYNUM_NONE 
		&& NPC->client->ps.forceJumpZStart && NPC->client->NPC_class == CLASS_POLTER
		&& !Q_irand( 0, 10 ) ||
		NPC->client->ps.groundEntityNum == ENTITYNUM_NONE 
		&& NPC->client->ps.forceJumpZStart && NPC->client->NPC_class == CLASS_ARIEL
		&& !Q_irand( 0, 10 ) ||
		// BAT FORM CLASSES 
		NPC->client->ps.groundEntityNum == ENTITYNUM_NONE 
		&& NPC->client->ps.forceJumpZStart && NPC->client->NPC_class == CLASS_BATFORM
		&& !Q_irand( 0, 10 ) ||
		NPC->client->ps.groundEntityNum == ENTITYNUM_NONE 
		&& NPC->client->ps.forceJumpZStart && NPC->client->NPC_class == CLASS_BATKAIN
		&& !Q_irand( 0, 10 ) ||
		NPC->client->ps.groundEntityNum == ENTITYNUM_NONE 
		&& NPC->client->ps.forceJumpZStart && NPC->client->NPC_class == CLASS_BATKAIN2
		&& !Q_irand( 0, 10 ) ||
		NPC->client->ps.groundEntityNum == ENTITYNUM_NONE 
		&& NPC->client->ps.forceJumpZStart && NPC->client->NPC_class == CLASS_BATKAIN3
		&& !Q_irand( 0, 10 ) ||
		NPC->client->ps.groundEntityNum == ENTITYNUM_NONE 
		&& NPC->client->ps.forceJumpZStart && NPC->client->NPC_class == CLASS_BATVORADOR
		&& !Q_irand( 0, 10 ) ||
		NPC->client->ps.groundEntityNum == ENTITYNUM_NONE 
		&& NPC->client->ps.forceJumpZStart && NPC->client->NPC_class == CLASS_BATVAMPIRE_QUEEN
		&& !Q_irand( 0, 10 ) ||
		NPC->client->ps.groundEntityNum == ENTITYNUM_NONE 
		&& NPC->client->ps.forceJumpZStart && NPC->client->NPC_class == CLASS_BATVAMPIRE_FROST
		&& !Q_irand( 0, 10 ) ||
		NPC->client->ps.groundEntityNum == ENTITYNUM_NONE 
		&& NPC->client->ps.forceJumpZStart && NPC->client->NPC_class == CLASS_BATNECROMANCER
		&& !Q_irand( 0, 10 ) ||
		NPC->client->ps.groundEntityNum == ENTITYNUM_NONE 
		&& NPC->client->ps.forceJumpZStart && NPC->client->NPC_class == CLASS_BATVAMPIRE_WATER
		&& !Q_irand( 0, 10 ) ||
		NPC->client->ps.groundEntityNum == ENTITYNUM_NONE 
		&& NPC->client->ps.forceJumpZStart && NPC->client->NPC_class == CLASS_BATVAMPIRE_ELF2
		&& !Q_irand( 0, 10 ) ||
		NPC->client->ps.groundEntityNum == ENTITYNUM_NONE 
		&& NPC->client->ps.forceJumpZStart && NPC->client->NPC_class == CLASS_BATVAMPIRE_ELF2
		&& !Q_irand( 0, 10 ) ||
		NPC->client->ps.groundEntityNum == ENTITYNUM_NONE 
		&& NPC->client->ps.forceJumpZStart && NPC->client->NPC_class == CLASS_BATVAMPIRE_ELF3
		&& !Q_irand( 0, 10 ) ||
		NPC->client->ps.groundEntityNum == ENTITYNUM_NONE 
		&& NPC->client->ps.forceJumpZStart && NPC->client->NPC_class == CLASS_HELLBAT
		&& !Q_irand( 0, 10 ) ||
		NPC->client->ps.groundEntityNum == ENTITYNUM_NONE 
		&& NPC->client->ps.forceJumpZStart && NPC->client->NPC_class == CLASS_BATVAMPIRE_FEAR
		&& !Q_irand( 0, 10 ) ||
		NPC->client->ps.groundEntityNum == ENTITYNUM_NONE 
		&& NPC->client->ps.forceJumpZStart && NPC->client->NPC_class == CLASS_BATVAMPIRE_FEAR2
		&& !Q_irand( 0, 10 ) ||
		NPC->client->ps.groundEntityNum == ENTITYNUM_NONE 
		&& NPC->client->ps.forceJumpZStart && NPC->client->NPC_class == CLASS_JANOS
		&& !Q_irand( 0, 10 ) ||
		NPC->client->ps.groundEntityNum == ENTITYNUM_NONE
		&& NPC->client->ps.forceJumpZStart && NPC->client->NPC_class == CLASS_KAIN3
		&& !Q_irand( 0, 10 ) ||
		NPC->client->ps.groundEntityNum == ENTITYNUM_NONE 
		&& NPC->client->ps.forceJumpZStart && NPC->client->NPC_class == CLASS_AIR
		&& !Q_irand( 0, 10 ) ||
		NPC->client->ps.groundEntityNum == ENTITYNUM_NONE 
		&& NPC->client->ps.forceJumpZStart && NPC->client->NPC_class == CLASS_VOID
		&& !Q_irand( 0, 10 ) ||
		NPC->client->ps.groundEntityNum == ENTITYNUM_NONE 
		&& NPC->client->ps.forceJumpZStart && NPC->client->NPC_class == CLASS_DARK
		&& !Q_irand( 0, 10 ) ||
		NPC->client->ps.groundEntityNum == ENTITYNUM_NONE 
		&& NPC->client->ps.forceJumpZStart && NPC->client->NPC_class == CLASS_LIGHT
		&& !Q_irand( 0, 10 ) ||
		NPC->client->ps.groundEntityNum == ENTITYNUM_NONE 
		&& NPC->client->ps.forceJumpZStart && NPC->client->NPC_class == CLASS_GOLEM_VOID
		&& !Q_irand( 0, 10 ) ||
		NPC->client->ps.groundEntityNum == ENTITYNUM_NONE 
		&& NPC->client->ps.forceJumpZStart && NPC->client->NPC_class == CLASS_RAZIELHIM
		&& !Q_irand( 0, 10 ) ||
		NPC->client->ps.groundEntityNum == ENTITYNUM_NONE 
		&& NPC->client->ps.forceJumpZStart && NPC->client->NPC_class == CLASS_ESKANDOR
		&& !Q_irand( 0, 10 ) ||
		NPC->client->ps.groundEntityNum == ENTITYNUM_NONE 
		&& NPC->client->ps.forceJumpZStart && NPC->client->NPC_class == CLASS_GOLEM_AIR
		&& !Q_irand( 0, 10 ) )
	{//take off
		NPC_FlyStart( NPC );
	}
	// If Hurting, Try To Run Away
	//-----------------------------
	if (!NPCInfo->surrenderTime && (NPC->health<NPC->max_health/10))
	{
		//NPC_Printf("Time To Surrender, Searching For Flee Point");


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
		else
		{
			//NPC_Printf("  Failure");
		}
	}
}




////////////////////////////////////////////////////////////////////////////////////////
// 
////////////////////////////////////////////////////////////////////////////////////////
bool	NPC_Flee()
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

			if (NPC_Respawn())
			{
				return true;
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
					NPC_DustFallNear(NPC->enemy->currentOrigin, Q_irand(1,2));
					break;

				case 1:
					//NPC_Printf("SPOOK: Footsteps");
  					testDirection[0] =  (random() * 0.5f) - 1.0f;
	 		 		testDirection[0] += (testDirection[0]>0.0f)?(0.5f):(-0.5f);
					testDirection[1] = (random() * 0.5f) - 1.0f;
					testDirection[1] += (testDirection[1]>0.0f)?(0.5f):(-0.5f);
					testDirection[2] = 1.0f;
		 	 		VectorMA(NPC->enemy->currentOrigin, 400.0f, testDirection, NPCFootStepLoc);

					NPCFootStepCount = Q_irand(3,8);
					break;
				}
			}

			if (NPCFootStepCount && TIMER_Done(NPC, "NPCFootStepFakeTimer"))
			{
				TIMER_Set(NPC, "NPCFootStepFakeTimer", Q_irand(300, 800));
				NPCFootStepCount --;
				G_SoundAtSpot(NPCFootStepLoc, G_SoundIndex(va("sound/player/footsteps/boot%d", Q_irand(1,4))), qtrue);
			}

		 	if (TIMER_Done(NPC, "ResampleEnemyDirection") && NPC->enemy->resultspeed>10.0f)
			{
				TIMER_Set(NPC, "ResampleEnemyDirection", Q_irand(500, 1000));
				AverageEnemyDirectionSamples2 ++;

				vec3_t	moveDir;
				VectorCopy(NPC->enemy->client->ps.velocity, moveDir);
				VectorNormalize(moveDir);

				VectorAdd(AverageEnemyDirection2, moveDir, AverageEnemyDirection2);
			}
		}
	}
	else
	{
		NPCInfo->surrenderTime += 100;
	}

	// Finish The Flame Thrower First...
	//-----------------------------------
	if (NPCInfo->aiFlags&NPCAI_FLAMETHROW)
	{
		NPC_TacticsSelect();
		NPC_FacePosition( NPC->enemy->currentOrigin, qtrue);
		NPC_UpdateAngles(qtrue, qtrue);
		return true;
	}

	bool	IsOnAPath = !!NPC_MoveToGoal(qtrue);
	if (!ReachedEscapePoint &&
		NPCInfo->aiFlags&NPCAI_BLOCKED && 
		NPC->client->moveType!=MT_FLYSWIM && 
		((level.time - NPCInfo->blockedDebounceTime)>1000)
		)
	{
		if (!NPC_CanSeeEnemy(NPC) && Distance(NPC->currentOrigin, level.combatPoints[NPCInfo->combatPoint].origin)<200)
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
