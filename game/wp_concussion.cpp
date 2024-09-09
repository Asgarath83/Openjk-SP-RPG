
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

static void WP_FireConcussionAlt( gentity_t *ent )
{//a rail-gun-like beam
	int			damage = weaponData[WP_CONCUSSION].altDamage, skip, traces = DISRUPTOR_ALT_TRACES;
	qboolean	render_impact = qtrue;
	vec3_t		start, end;
	vec3_t		muzzle2, spot, dir;
	trace_t		tr;
	gentity_t	*traceEnt, *tent;
	float		dist, shotDist, shotRange = 8192;
	qboolean	hitDodged = qfalse;

	if (ent->s.number >= MAX_CLIENTS)
	{
		vec3_t angles;
		vectoangles(forwardVec, angles);
		angles[PITCH] += ( crandom() * (CONC_NPC_SPREAD+(6-ent->NPC->currentAim)*0.25f));//was 0.5f
		angles[YAW]	  += ( crandom() * (CONC_NPC_SPREAD+(6-ent->NPC->currentAim)*0.25f));//was 0.5f
		AngleVectors(angles, forwardVec, vrightVec, up);
	}

	//Shove us backwards for half a second
	VectorMA( ent->client->ps.velocity, -200, forwardVec, ent->client->ps.velocity );
	ent->client->ps.groundEntityNum = ENTITYNUM_NONE;
	if ( (ent->client->ps.pm_flags&PMF_DUCKED) )
	{//hunkered down
		ent->client->ps.pm_time = 100;
	}
	else
	{
		ent->client->ps.pm_time = 250;
	}
	ent->client->ps.pm_flags |= PMF_TIME_KNOCKBACK|PMF_TIME_NOFRICTION;
	//FIXME: only if on ground?  So no "rocket jump"?  Or: (see next FIXME)
	//FIXME: instead, set a forced ucmd backmove instead of this sliding

	VectorCopy( muzzle, muzzle2 ); // making a backup copy

	// The trace start will originate at the eye so we can ensure that it hits the crosshair.
	if ( ent->NPC )
	{
		switch ( g_spskill->integer )
		{
		case 0:
			damage = CONC_ALT_NPC_DAMAGE_EASY;
			break;
		case 1:
			damage = CONC_ALT_NPC_DAMAGE_MEDIUM;
			break;
		case 2:
		default:
			damage = CONC_ALT_NPC_DAMAGE_HARD;
			break;
		}
	}

	// DMG 
	if ( ent->client->NPC_class ==  CLASS_VAMPIRE_ZOMBIE || 
				ent->client->NPC_class ==  CLASS_TUSKEN ||
				ent->client->NPC_class ==  CLASS_MELC ||
				ent->client->NPC_class ==  CLASS_SUMMONER )
	{
		damage *= 15;
	}
	else if ( ent->client->NPC_class == CLASS_GLIDER )
	{
		damage *= 20;
	}
	// SPIRITH DEATH: All vampires classes Zephonim \ Alliance \ Dumahim 
	else if ( ent->client->NPC_class ==  CLASS_REBORN ||
				ent->client->NPC_class ==  CLASS_ALORA ||
				ent->client->NPC_class ==  CLASS_MONMOTHA ||
				ent->client->NPC_class ==  CLASS_NOGHRI ||
				ent->client->NPC_class ==  CLASS_FLIER2 || 
				ent->client->NPC_class ==  CLASS_ZEPH ||
				ent->client->NPC_class ==  CLASS_DECEIVER ||
				ent->client->NPC_class ==  CLASS_DUMAHIM || 
				ent->client->NPC_class ==  CLASS_VAMPIRE ||
				ent->client->NPC_class ==  CLASS_KAIN2 || 
				ent->client->NPC_class ==  CLASS_UMAH || 
				ent->client->NPC_class ==  CLASS_NECROMANCER || 
				ent->client->NPC_class ==  CLASS_VAMPIRE_QUEEN ||
				ent->client->NPC_class ==  CLASS_VAMPIRE_DARK || 
				ent->client->NPC_class ==  CLASS_VAMPIRE_NECRO || 
				ent->client->NPC_class ==  CLASS_VAMPIRE_NECRO2 ||
				ent->client->NPC_class ==  CLASS_VAMPIRE_FEAR || 
				ent->client->NPC_class ==  CLASS_VAMPIRE_SOUND || 
				ent->client->NPC_class ==  CLASS_VAMPIRE_LYCAN || 
				ent->client->NPC_class ==  CLASS_VAMPIRE_WATER || 
				ent->client->NPC_class ==  CLASS_VAMPIRE_ARCHER || 
				ent->client->NPC_class ==  CLASS_HALFVAMPIRE || 
				ent->client->NPC_class == CLASS_RAZIELHIM ||  
				ent->client->NPC_class == CLASS_ESKANDOR ||  
				ent->client->NPC_class ==  CLASS_VAMPIRE_BOUNTY || 
				ent->client->NPC_class ==  CLASS_VAMPIRE_SARAFAN )
	{
		damage *= 10;
	}
	// NECROMANTIC FIRE - RAHABIM   
	else if ( ent->client->NPC_class ==  CLASS_SWAMPTROOPER ||
				ent->client->NPC_class ==  CLASS_RAHABIM || 
				ent->client->NPC_class ==  CLASS_COMMANDO || 
				ent->client->NPC_class ==  CLASS_LYCAN || 
				ent->client->NPC_class ==  CLASS_CENOBITA || 
				ent->client->NPC_class ==  CLASS_WIZARD )
	{
		damage *= 16;
	}
	else if ( ent->client->NPC_class == CLASS_FISH )
	{
		damage *= 17.5;
	}
	// SD and IMMOLATION TOGETHER - KAIN 
	else if ( ent->client->NPC_class ==  CLASS_KYLE ||
				ent->client->NPC_class ==  CLASS_KAIN || 
				ent->client->NPC_class ==  CLASS_KAIN3 ||
				ent->client->NPC_class ==  CLASS_TURELIM || 
				ent->client->NPC_class ==  CLASS_VORADOR ||
				ent->client->NPC_class ==  CLASS_JANOS ||
				ent->client->NPC_class ==  CLASS_CLAW || 
				ent->client->NPC_class ==  CLASS_GOLEM_LAVA || 
				ent->client->NPC_class ==  CLASS_VAMPIRE_FEAR2 || 
				ent->client->NPC_class ==  CLASS_VAMPIRE_BLOOD || 
				ent->client->NPC_class ==  CLASS_VAMPIRE_DEMON  )
	{
		damage *= 15;
	}
	// THE HOLY IMMOLATION:  Humans, Saraphan and Druids. 
	else if ( ent->client->NPC_class ==  CLASS_HUNTER || 
				ent->client->NPC_class ==  CLASS_SCOUT ||  
				ent->client->NPC_class ==  CLASS_ALCHEMIST || 
				ent->client->NPC_class ==  CLASS_SARAFAN || 
				ent->client->NPC_class ==  CLASS_ARCHER ||  
				ent->client->NPC_class ==  CLASS_WARRIOR || 
				ent->client->NPC_class ==  CLASS_TRANDOSHAN ||
				ent->client->NPC_class ==  CLASS_STORMTROOPER ||
				ent->client->NPC_class ==  CLASS_PRISONER ||
				ent->client->NPC_class ==  CLASS_RODIAN ||
				ent->client->NPC_class ==  CLASS_IMPWORKER ||
				ent->client->NPC_class ==  CLASS_MARK1 ||
				ent->client->NPC_class ==  CLASS_MARK2 ||
				ent->client->NPC_class ==  CLASS_R2D2 ||
				ent->client->NPC_class ==  CLASS_R5D2 ||
				ent->client->NPC_class ==  CLASS_SEEKER ||
				ent->client->NPC_class ==  CLASS_REMOTE ||
				ent->client->NPC_class ==  CLASS_DRUIDREAVER || 
				ent->client->NPC_class ==  CLASS_RAZIEL || 
				ent->client->NPC_class ==  CLASS_HOLY || 
				ent->client->NPC_class ==  CLASS_GOLEM_HOLY )
	{
		damage *= 12.5;
	}
	// HUMAN BOSSES
	else if ( ent->client->NPC_class == CLASS_JAN || ent->client->NPC_class == CLASS_GALAK )
	{
		damage *= 15;
	}
	// THE POISON IMMOLATION 
	else if ( ent->client->NPC_class ==  CLASS_SPIDERREAVER3 || 
				ent->client->NPC_class ==  CLASS_BEAST || 
				ent->client->NPC_class ==  CLASS_MINEMONSTER ||
				ent->client->NPC_class ==  CLASS_REELO || 
				ent->client->NPC_class ==  CLASS_POISON || 
				ent->client->NPC_class ==  CLASS_DEMON_POISON || 
				ent->client->NPC_class ==  CLASS_GOLEM_POISON )
	{
		damage *= 15;
	}
	// THE NECROMANTIC FOG - MIND ATK 
	else if ( ent->client->NPC_class ==  CLASS_PROBE ||
				ent->client->NPC_class ==  CLASS_POLTER || 
				ent->client->NPC_class ==  CLASS_REBEL || 
				ent->client->NPC_class ==  CLASS_MENTALIST ||
				ent->client->NPC_class ==  CLASS_AIRREAVER || 
				ent->client->NPC_class ==  CLASS_AIR || 
				ent->client->NPC_class ==  CLASS_GOLEM_AIR || 
				ent->client->NPC_class ==  CLASS_BIRD )
	{
		damage *= 2.5;
	}
	// THE NECROMANTIC SMOKE ATK
	else if ( ent->client->NPC_class ==  CLASS_BESPIN_COP || 				
				ent->client->NPC_class ==  CLASS_DARK || 			 
				ent->client->NPC_class ==  CLASS_GOLEM_DARK || 
				ent->client->NPC_class ==  CLASS_TANKREAVER || 
				ent->client->NPC_class ==  CLASS_GOLEM_SMOKE )
	{
		damage *= 5;
	}
	// THE NECROMANTIC ASTRAL ATK - Dimension  - Secondary Atk: Sonic ray 
	else if ( ent->client->NPC_class ==  CLASS_HOWLER ||
				ent->client->NPC_class ==  CLASS_SONIC || 
				ent->client->NPC_class ==  CLASS_GOLEM_SONIC || 
				ent->client->NPC_class ==  CLASS_LANDO )
	{
		damage *= 17.5;
	}
	// THE NECROMANTIC FULL IMMOLATE ATTACK - MAGMA GUARDIAN \ TURELIM  
	else if ( ent->client->NPC_class ==  CLASS_FIREREAVER || 
				ent->client->NPC_class ==  CLASS_FIRE || 
				ent->client->NPC_class ==  CLASS_GOLEM_FIRE || 
				ent->client->NPC_class ==  CLASS_HELLGOLEM || 
				ent->client->NPC_class ==  CLASS_HAZARD_TROOPER ||
				ent->client->NPC_class ==  CLASS_GRAN ||
				ent->client->NPC_class ==  CLASS_VAMPIRE_FROST || 
				ent->client->NPC_class ==  CLASS_BOBAFETT || 
				ent->client->NPC_class ==  CLASS_WAMPA || 
				ent->client->NPC_class ==  CLASS_DEMON_FIRE )
	{
		damage *= 20;
	}
	// THE NECROMANTIC MUD ATK -   
	else if ( ent->client->NPC_class ==  CLASS_SWAMP ||
				ent->client->NPC_class ==  CLASS_INTERROGATOR ||
				ent->client->NPC_class ==  CLASS_SHARK || 
				ent->client->NPC_class ==  CLASS_WATER || 
				ent->client->NPC_class ==  CLASS_GOLEM_WATER || 
				ent->client->NPC_class ==  CLASS_UGNAUGHT || 
				ent->client->NPC_class ==  CLASS_SPIDERREAVER2 || 
				ent->client->NPC_class ==  CLASS_SPIDERREAVER )
	{
		damage *= 11;
	}
	// THE NECROMANTIC NATURE ATK - Dimension  
	else if ( ent->client->NPC_class ==  CLASS_LUKE || 
				ent->client->NPC_class ==  CLASS_IVY || 
				ent->client->NPC_class ==  CLASS_DRUID )
	{
		damage *= 20;
	}
	// NECROMANTIC SHOCK ATK - Lightning forge 
	else if ( ent->client->NPC_class ==  CLASS_LIGHTREAVER || 
				ent->client->NPC_class ==  CLASS_GOLEM_METAL || 
				ent->client->NPC_class ==  CLASS_DEMON_ELECTRO || 
				ent->client->NPC_class ==  CLASS_ASSASSIN_DROID || 
				ent->client->NPC_class ==  CLASS_SABER_DROID )
	{
		damage *= 15;
	}
	// NECROMANTIC Sun  ATK - Sunlight forge 
	else if ( ent->client->NPC_class ==  CLASS_LIGHT || 
				ent->client->NPC_class ==  CLASS_GOLEM_LIGHT || 
				ent->client->NPC_class ==  CLASS_GOLEM_GOLD || 
				ent->client->NPC_class ==  CLASS_MORGANKATARN ||
				ent->client->NPC_class ==  CLASS_PROTOCOL )
	{
		damage *= 20;
	}
	// NECROMANTIC TIME ATK - Storm Forge - Primary shoot: sandstorm devil. secondary: time slam 
	else if ( ent->client->NPC_class == CLASS_ROCKETTROOPER ||
		ent->client->NPC_class == CLASS_VAMPIRE_ELF3 )
	{
		damage *= 15;
	}
	// NECROMANTIC STONE ATK 
	else if ( ent->client->NPC_class ==  CLASS_GONK ||
				ent->client->NPC_class ==  CLASS_LIZARD ||
				ent->client->NPC_class ==  CLASS_VAMPIRE_ELF || 
				ent->client->NPC_class ==  CLASS_VAMPIRE_ELF2 || 
				ent->client->NPC_class ==  CLASS_EARTHREAVER || 
				ent->client->NPC_class ==  CLASS_EARTH || 
				ent->client->NPC_class ==  CLASS_GOLEM || 
				ent->client->NPC_class ==  CLASS_DEMON_BLADE )
	{
		damage *= 20;
	}
	// NECROMANTIC STATES CRYSTAL RAY
	else if ( ent->client->NPC_class ==  CLASS_WEEQUAY ||
				ent->client->NPC_class ==  CLASS_BARTENDER ||
				ent->client->NPC_class ==  CLASS_GOLEM_ICE || 
				ent->client->NPC_class ==  CLASS_ICEREAVER || 
				ent->client->NPC_class ==  CLASS_CRYSTAL || 
				ent->client->NPC_class ==  CLASS_GALAKMECH  )
	{
		damage *= 15;
	}
	// NECROMANTIC CHAOS 
	else if ( ent->client->NPC_class ==  CLASS_PROPHET || 
				ent->client->NPC_class ==  CLASS_TAVION ||
				ent->client->NPC_class ==  CLASS_DESANN ||
				ent->client->NPC_class ==  CLASS_HYLDEN || 
				ent->client->NPC_class ==  CLASS_HYLDENLORD || 
				ent->client->NPC_class ==  CLASS_ELDERGOD || 
				ent->client->NPC_class ==  CLASS_TENTACLE || 
				ent->client->NPC_class ==  CLASS_ATST ||
				ent->client->NPC_class ==  CLASS_SAND_CREATURE ||
				ent->client->NPC_class ==  CLASS_MOUSE || 
				ent->client->NPC_class ==  CLASS_SENTRY )
	{
		damage *= 40;
	}
	// NECROMANTIC VOID 
	else if ( ent->client->NPC_class ==  CLASS_SABOTEUR || 
				ent->client->NPC_class ==  CLASS_SHADOWTROOPER || 
				ent->client->NPC_class ==  CLASS_VOID || 
				ent->client->NPC_class ==  CLASS_GOLEM_VOID || 
				ent->client->NPC_class ==  CLASS_DARKREAVER )
	{
		damage *= 20;
	}
	// NECROMANTIC DEATH FIRE (FOR ALL WRAITH, REAPERS AND GHOSTS) 
	else if ( ent->client->NPC_class ==  CLASS_ARIEL || 
				ent->client->NPC_class ==  CLASS_GHOST || 
				ent->client->NPC_class ==  CLASS_REAPER || 
				ent->client->NPC_class ==  CLASS_DREADNOUGHT || 
				ent->client->NPC_class ==  CLASS_VAMPIRE_GHOST || 
				ent->client->NPC_class ==  CLASS_JEDI ||
				ent->client->NPC_class ==  CLASS_REAVER || 
				ent->client->NPC_class ==  CLASS_NECROREAVER || 
				ent->client->NPC_class ==  CLASS_THRALL || 
				ent->client->NPC_class ==  CLASS_NECRO || 
				ent->client->NPC_class ==  CLASS_GOLEM_NECRO || 
				ent->client->NPC_class ==  CLASS_JAWA ||
				ent->client->NPC_class ==  CLASS_MURJJ || 
				ent->client->NPC_class ==  CLASS_RANCOR || 
				ent->client->NPC_class ==  CLASS_PLAYER )
	{
		damage *= 10;
	}
	
	VectorCopy( muzzle, start );
	WP_TraceSetStart( ent, start, vec3_origin, vec3_origin );

	skip = ent->s.number;

//	if ( ent->client && ent->client->ps.powerups[PW_WEAPON_OVERCHARGE] > 0 && ent->client->ps.powerups[PW_WEAPON_OVERCHARGE] > cg.time )
//	{
//		// in overcharge mode, so doing double damage
//		damage *= 2;
//	}
	
	//Make it a little easier to hit guys at long range
	vec3_t shot_mins, shot_maxs;
	VectorSet( shot_mins, -1, -1, -1 );
	VectorSet( shot_maxs, 1, 1, 1 );

	for ( int i = 0; i < traces; i++ )
	{
		VectorMA( start, shotRange, forwardVec, end );

		//NOTE: if you want to be able to hit guys in emplaced guns, use "G2_COLLIDE, 10" instead of "G2_RETURNONHIT, 0"
		//alternately, if you end up hitting an emplaced_gun that has a sitter, just redo this one trace with the "G2_COLLIDE, 10" to see if we it the sitter
		//gi.trace( &tr, start, NULL, NULL, end, skip, MASK_SHOT, G2_COLLIDE, 10 );//G2_RETURNONHIT, 0 );
		gi.trace( &tr, start, shot_mins, shot_maxs, end, skip, MASK_SHOT, G2_COLLIDE, 10 );//G2_RETURNONHIT, 0 );

		if ( tr.surfaceFlags & SURF_NOIMPACT ) 
		{
			render_impact = qfalse;
		}

		if ( tr.entityNum == ent->s.number )
		{
			// should never happen, but basically we don't want to consider a hit to ourselves?
			// Get ready for an attempt to trace through another person
			VectorCopy( tr.endpos, muzzle2 );
			VectorCopy( tr.endpos, start );
			skip = tr.entityNum;
#ifdef _DEBUG
			gi.Printf( "BAD! Concussion gun shot somehow traced back and hit the owner!\n" );			
#endif
			continue;
		}

		// always render a shot beam, doing this the old way because I don't much feel like overriding the effect.
		//NOTE: let's just draw one beam at the end
		//tent = G_TempEntity( tr.endpos, EV_CONC_ALT_SHOT );
		//tent->svFlags |= SVF_BROADCAST;

		//VectorCopy( muzzle2, tent->s.origin2 );

		if ( tr.fraction >= 1.0f )
		{
			// draw the beam but don't do anything else
			break;
		}

		traceEnt = &g_entities[tr.entityNum];

		if ( traceEnt //&& traceEnt->NPC  // CAN DODGE ONLY: Who use this, the Necromancers, the Summoners, the Rahabim, the Sarafan Lords and the Liches.  
			&& ( traceEnt->s.weapon == WP_CONCUSSION || (traceEnt->client && (traceEnt->client->NPC_class == CLASS_GLIDER||traceEnt->client->NPC_class == CLASS_FISH||traceEnt->client->NPC_class == CLASS_JAN|| traceEnt->client->NPC_class == CLASS_COMMANDO && traceEnt->s.weapon == WP_CONCUSSION || traceEnt->client->NPC_class == CLASS_MURJJ ) ) ) )
		{//FIXME: need a more reliable way to know we hit a jedi?
			hitDodged = Jedi_DodgeEvasion( traceEnt, ent, &tr, HL_NONE );
			//acts like we didn't even hit him
		}
		if ( !hitDodged )
		{
			if ( render_impact )
			{
				if (( tr.entityNum < ENTITYNUM_WORLD && traceEnt->takedamage ) 
					|| !Q_stricmp( traceEnt->classname, "misc_model_breakable" ) 
					|| traceEnt->s.eType == ET_MOVER )
				{
					// Create a simple impact type mark that doesn't last long in the world
					//G_PlayEffect( G_EffectIndex( "concussion/alt_hit" ), tr.endpos, tr.plane.normal );
					///////////////////////////////////////ALT HIT SHOT //////////////////
					if ( ent->client->NPC_class ==  CLASS_VAMPIRE_ZOMBIE || 
				ent->client->NPC_class ==  CLASS_GLIDER ||
				ent->client->NPC_class ==  CLASS_TUSKEN ||
				ent->client->NPC_class ==  CLASS_MELC ||
				ent->client->NPC_class ==  CLASS_SUMMONER )
	{
		{
			G_PlayEffect( G_EffectIndex( "concussion/alt_hit" ), tr.endpos, tr.plane.normal );
		}
	}
// SPIRITH DEATH: All vampires classes Zephonim \ Alliance \ Dumahim 
	else if ( ent->client->NPC_class ==  CLASS_REBORN ||
				ent->client->NPC_class ==  CLASS_ALORA ||
				ent->client->NPC_class ==  CLASS_MONMOTHA ||
				ent->client->NPC_class ==  CLASS_NOGHRI ||
				ent->client->NPC_class ==  CLASS_FLIER2 || 
				ent->client->NPC_class ==  CLASS_ZEPH ||
				ent->client->NPC_class ==  CLASS_DECEIVER ||
				ent->client->NPC_class ==  CLASS_DUMAHIM || 
				ent->client->NPC_class ==  CLASS_VAMPIRE ||
				ent->client->NPC_class ==  CLASS_KAIN2 || 
				ent->client->NPC_class ==  CLASS_UMAH || 
				ent->client->NPC_class ==  CLASS_NECROMANCER || 
				ent->client->NPC_class ==  CLASS_VAMPIRE_QUEEN ||
				ent->client->NPC_class ==  CLASS_VAMPIRE_DARK || 
				ent->client->NPC_class ==  CLASS_VAMPIRE_NECRO || 
				ent->client->NPC_class ==  CLASS_VAMPIRE_NECRO2 ||
				ent->client->NPC_class ==  CLASS_VAMPIRE_FEAR || 
				ent->client->NPC_class ==  CLASS_VAMPIRE_SOUND || 
				ent->client->NPC_class ==  CLASS_VAMPIRE_LYCAN || 
				ent->client->NPC_class ==  CLASS_VAMPIRE_WATER || 
				ent->client->NPC_class ==  CLASS_VAMPIRE_ARCHER || 
				ent->client->NPC_class ==  CLASS_HALFVAMPIRE || 
				ent->client->NPC_class == CLASS_RAZIELHIM ||  
				ent->client->NPC_class == CLASS_ESKANDOR ||  
				ent->client->NPC_class ==  CLASS_VAMPIRE_BOUNTY || 
				ent->client->NPC_class ==  CLASS_VAMPIRE_SARAFAN )
	{
		{
			G_PlayEffect( G_EffectIndex( "spirith_death/alt_hit" ), tr.endpos, tr.plane.normal );
		}
	}
	// NECROMANTIC FIRE - RAHABIM   
	else if ( ent->client->NPC_class ==  CLASS_SWAMPTROOPER ||
				ent->client->NPC_class ==  CLASS_RAHABIM || 
				ent->client->NPC_class ==  CLASS_COMMANDO || 
				ent->client->NPC_class ==  CLASS_LYCAN || 
				ent->client->NPC_class ==  CLASS_CENOBITA || 
				ent->client->NPC_class ==  CLASS_WIZARD ||
				ent->client->NPC_class == CLASS_FISH )
	{
		{
			G_PlayEffect( G_EffectIndex( "blackfire/alt_hit" ), tr.endpos, tr.plane.normal );
		}
	}
	// SD and IMMOLATION TOGETHER - KAIN 
	else if ( ent->client->NPC_class ==  CLASS_KYLE ||
				ent->client->NPC_class ==  CLASS_KAIN || 
				ent->client->NPC_class ==  CLASS_KAIN3 ||
				ent->client->NPC_class ==  CLASS_TURELIM || 
				ent->client->NPC_class ==  CLASS_VORADOR ||
				ent->client->NPC_class ==  CLASS_JANOS ||
				ent->client->NPC_class ==  CLASS_CLAW || 
				ent->client->NPC_class ==  CLASS_GOLEM_LAVA || 
				ent->client->NPC_class ==  CLASS_VAMPIRE_FEAR2 || 
				ent->client->NPC_class ==  CLASS_VAMPIRE_BLOOD || 
				ent->client->NPC_class ==  CLASS_VAMPIRE_DEMON  )
	{
		{
			G_PlayEffect( G_EffectIndex( "immolate/alt_hit" ), tr.endpos, tr.plane.normal );
		}
	}
	// THE HOLY IMMOLATION:  Humans, Saraphan and Druids. 
	else if ( ent->client->NPC_class ==  CLASS_HUNTER || 
				ent->client->NPC_class ==  CLASS_SCOUT ||  
				ent->client->NPC_class ==  CLASS_ALCHEMIST || 
				ent->client->NPC_class ==  CLASS_SARAFAN || 
				ent->client->NPC_class ==  CLASS_ARCHER ||  
				ent->client->NPC_class ==  CLASS_WARRIOR || 
				ent->client->NPC_class ==  CLASS_TRANDOSHAN ||
				ent->client->NPC_class ==  CLASS_STORMTROOPER ||
				ent->client->NPC_class ==  CLASS_PRISONER ||
				ent->client->NPC_class ==  CLASS_RODIAN ||
				ent->client->NPC_class ==  CLASS_IMPWORKER ||
				ent->client->NPC_class ==  CLASS_MARK1 ||
				ent->client->NPC_class ==  CLASS_MARK2 ||
				ent->client->NPC_class ==  CLASS_R2D2 ||
				ent->client->NPC_class ==  CLASS_R5D2 ||
				ent->client->NPC_class ==  CLASS_SEEKER ||
				ent->client->NPC_class ==  CLASS_REMOTE ||
				ent->client->NPC_class ==  CLASS_DRUIDREAVER || 
				ent->client->NPC_class ==  CLASS_RAZIEL || 
				ent->client->NPC_class ==  CLASS_HOLY || 
				ent->client->NPC_class ==  CLASS_GOLEM_HOLY ||
				ent->client->NPC_class == CLASS_JAN || 
				ent->client->NPC_class == CLASS_GALAK )
	{
		{
			G_PlayEffect( G_EffectIndex( "holyimmolate/alt_hit" ), tr.endpos, tr.plane.normal );
		}
	}
	// THE POISON IMMOLATION 
	else if ( ent->client->NPC_class ==  CLASS_SPIDERREAVER3 || 
				ent->client->NPC_class ==  CLASS_BEAST || 
				ent->client->NPC_class ==  CLASS_MINEMONSTER ||
				ent->client->NPC_class ==  CLASS_REELO || 
				ent->client->NPC_class ==  CLASS_POISON || 
				ent->client->NPC_class ==  CLASS_DEMON_POISON || 
				ent->client->NPC_class ==  CLASS_GOLEM_POISON )
	{
		{
			G_PlayEffect( G_EffectIndex( "necro_poison/alt_hit" ), tr.endpos, tr.plane.normal );
		}
	}
	// THE NECROMANTIC FOG - MIND ATK 
	else if ( ent->client->NPC_class ==  CLASS_PROBE ||
				ent->client->NPC_class ==  CLASS_POLTER || 
				ent->client->NPC_class ==  CLASS_REBEL || 
				ent->client->NPC_class ==  CLASS_MENTALIST ||
				ent->client->NPC_class ==  CLASS_AIRREAVER || 
				ent->client->NPC_class ==  CLASS_AIR || 
				ent->client->NPC_class ==  CLASS_GOLEM_AIR || 
				ent->client->NPC_class ==  CLASS_BIRD )
	{
		{
			G_PlayEffect( G_EffectIndex( "necro_fog/alt_hit" ), tr.endpos, tr.plane.normal );
		}
	}
	// THE NECROMANTIC SMOKE ATK
	else if ( ent->client->NPC_class ==  CLASS_BESPIN_COP || 				
				ent->client->NPC_class ==  CLASS_DARK || 			 
				ent->client->NPC_class ==  CLASS_GOLEM_DARK || 
				ent->client->NPC_class ==  CLASS_TANKREAVER || 
				ent->client->NPC_class ==  CLASS_GOLEM_SMOKE )
	{
		{
			G_PlayEffect( G_EffectIndex( "necro_smog/alt_hit" ), tr.endpos, tr.plane.normal );
		}
	}
	// THE NECROMANTIC ASTRAL ATK - Dimension  - Secondary Atk: Sonic ray 
	else if ( ent->client->NPC_class ==  CLASS_HOWLER ||
				ent->client->NPC_class ==  CLASS_SONIC || 
				ent->client->NPC_class ==  CLASS_GOLEM_SONIC || 
				ent->client->NPC_class ==  CLASS_LANDO )
	{
		{
			G_PlayEffect( G_EffectIndex( "necro_dim/alt_hit" ), tr.endpos, tr.plane.normal );
		}
	}
	// THE NECROMANTIC FULL IMMOLATE ATTACK - MAGMA GUARDIAN \ TURELIM  
	else if ( ent->client->NPC_class ==  CLASS_FIREREAVER || 
				ent->client->NPC_class ==  CLASS_FIRE || 
				ent->client->NPC_class ==  CLASS_GOLEM_FIRE || 
				ent->client->NPC_class ==  CLASS_HELLGOLEM || 
				ent->client->NPC_class ==  CLASS_HAZARD_TROOPER ||
				ent->client->NPC_class ==  CLASS_GRAN ||
				ent->client->NPC_class ==  CLASS_VAMPIRE_FROST || 
				ent->client->NPC_class ==  CLASS_BOBAFETT || 
				ent->client->NPC_class ==  CLASS_WAMPA || 
				ent->client->NPC_class ==  CLASS_DEMON_FIRE )
	{
		{
			G_PlayEffect( G_EffectIndex( "necro_conflict/alt_hit" ), tr.endpos, tr.plane.normal );
		}
	}
	// THE NECROMANTIC MUD ATK -   
	else if ( ent->client->NPC_class ==  CLASS_SWAMP ||
				ent->client->NPC_class ==  CLASS_INTERROGATOR ||
				ent->client->NPC_class ==  CLASS_SHARK || 
				ent->client->NPC_class ==  CLASS_WATER || 
				ent->client->NPC_class ==  CLASS_GOLEM_WATER || 
				ent->client->NPC_class ==  CLASS_UGNAUGHT || 
				ent->client->NPC_class ==  CLASS_SPIDERREAVER2 || 
				ent->client->NPC_class ==  CLASS_SPIDERREAVER )
	{
		{
			G_PlayEffect( G_EffectIndex( "necro_mud/alt_hit" ), tr.endpos, tr.plane.normal );
		}
	}
	// THE NECROMANTIC NATURE ATK - Dimension  
	else if ( ent->client->NPC_class ==  CLASS_LUKE || 
				ent->client->NPC_class ==  CLASS_IVY || 
				ent->client->NPC_class ==  CLASS_DRUID )
	{
		{
			G_PlayEffect( G_EffectIndex( "necro_nature/alt_hit" ), tr.endpos, tr.plane.normal );
		}
	}
	// NECROMANTIC SHOCK ATK - Lightning forge 
	else if ( ent->client->NPC_class ==  CLASS_LIGHTREAVER || 
				ent->client->NPC_class ==  CLASS_GOLEM_METAL || 
				ent->client->NPC_class ==  CLASS_DEMON_ELECTRO || 
				ent->client->NPC_class ==  CLASS_ASSASSIN_DROID || 
				ent->client->NPC_class ==  CLASS_SABER_DROID )
	{
		{
			G_PlayEffect( G_EffectIndex( "necro_shock/alt_hit" ), tr.endpos, tr.plane.normal );
		}
	}
	// NECROMANTIC Sun  ATK - Sunlight forge 
	else if ( ent->client->NPC_class ==  CLASS_LIGHT || 
				ent->client->NPC_class ==  CLASS_GOLEM_LIGHT || 
				ent->client->NPC_class ==  CLASS_GOLEM_GOLD || 
				ent->client->NPC_class ==  CLASS_MORGANKATARN ||
				ent->client->NPC_class ==  CLASS_PROTOCOL )
	{
		{
			G_PlayEffect( G_EffectIndex( "necro_energy/alt_hit" ), tr.endpos, tr.plane.normal );
		}
	}
	// NECROMANTIC TIME ATK - Storm Forge - Primary shoot: sandstorm devil. secondary: time slam 
	else if ( ent->client->NPC_class == CLASS_ROCKETTROOPER ||
		ent->client->NPC_class == CLASS_VAMPIRE_ELF3 )
{
		{
			G_PlayEffect( G_EffectIndex( "necro_time/alt_hit" ), tr.endpos, tr.plane.normal );
		}
	}
	// NECROMANTIC STONE ATK 
	else if ( ent->client->NPC_class ==  CLASS_GONK ||
				ent->client->NPC_class ==  CLASS_LIZARD ||
				ent->client->NPC_class ==  CLASS_VAMPIRE_ELF || 
				ent->client->NPC_class ==  CLASS_VAMPIRE_ELF2 || 
				ent->client->NPC_class ==  CLASS_EARTHREAVER || 
				ent->client->NPC_class ==  CLASS_EARTH || 
				ent->client->NPC_class ==  CLASS_GOLEM || 
				ent->client->NPC_class ==  CLASS_DEMON_BLADE )
	{
		{
			G_PlayEffect( G_EffectIndex( "necro_stone/alt_hit" ), tr.endpos, tr.plane.normal );
		}
	}
	// NECROMANTIC STATES CRYSTAL RAY
	else if ( ent->client->NPC_class ==  CLASS_WEEQUAY ||
				ent->client->NPC_class ==  CLASS_BARTENDER ||
				ent->client->NPC_class ==  CLASS_GOLEM_ICE || 
				ent->client->NPC_class ==  CLASS_ICEREAVER || 
				ent->client->NPC_class ==  CLASS_CRYSTAL || 
				ent->client->NPC_class ==  CLASS_GALAKMECH  )
	{
		{
			G_PlayEffect( G_EffectIndex( "necro_crystal/alt_hit" ), tr.endpos, tr.plane.normal );
		}
	}
	// NECROMANTIC CHAOS 
	else if ( ent->client->NPC_class ==  CLASS_PROPHET || 
				ent->client->NPC_class ==  CLASS_TAVION ||
				ent->client->NPC_class ==  CLASS_DESANN ||
				ent->client->NPC_class ==  CLASS_HYLDEN || 
				ent->client->NPC_class ==  CLASS_HYLDENLORD || 
				ent->client->NPC_class ==  CLASS_ELDERGOD || 
				ent->client->NPC_class ==  CLASS_TENTACLE || 
				ent->client->NPC_class ==  CLASS_ATST ||
				ent->client->NPC_class ==  CLASS_SAND_CREATURE ||
				ent->client->NPC_class ==  CLASS_MOUSE || 
				ent->client->NPC_class ==  CLASS_SENTRY )
{
		{
			G_PlayEffect( G_EffectIndex( "necro_chaos/alt_hit" ), tr.endpos, tr.plane.normal );
		}
	}
	// NECROMANTIC VOID 
	else if ( ent->client->NPC_class ==  CLASS_SABOTEUR || 
				ent->client->NPC_class ==  CLASS_SHADOWTROOPER || 
				ent->client->NPC_class ==  CLASS_VOID || 
				ent->client->NPC_class ==  CLASS_GOLEM_VOID || 
				ent->client->NPC_class ==  CLASS_DARKREAVER )
	{
		{
			G_PlayEffect( G_EffectIndex( "necro_void/alt_hit" ), tr.endpos, tr.plane.normal );
		}
	}
	// NECROMANTIC DEATH FIRE (FOR ALL WRAITH, REAPERS AND GHOSTS) 
	else if ( ent->client->NPC_class ==  CLASS_ARIEL || 
				ent->client->NPC_class ==  CLASS_GHOST || 
				ent->client->NPC_class ==  CLASS_REAPER || 
				ent->client->NPC_class ==  CLASS_DREADNOUGHT || 
				ent->client->NPC_class ==  CLASS_VAMPIRE_GHOST || 
				ent->client->NPC_class ==  CLASS_JEDI ||
				ent->client->NPC_class ==  CLASS_REAVER || 
				ent->client->NPC_class ==  CLASS_NECROREAVER || 
				ent->client->NPC_class ==  CLASS_THRALL || 
				ent->client->NPC_class ==  CLASS_NECRO || 
				ent->client->NPC_class ==  CLASS_GOLEM_NECRO || 
				ent->client->NPC_class ==  CLASS_JAWA ||
				ent->client->NPC_class ==  CLASS_MURJJ || 
				ent->client->NPC_class ==  CLASS_RANCOR || 
				ent->client->NPC_class ==  CLASS_PLAYER )
	{
		{
			G_PlayEffect( G_EffectIndex( "necro/alt_hit" ), tr.endpos, tr.plane.normal );
		}
	}
					
					
				
	
					if ( traceEnt->client && LogAccuracyHit( traceEnt, ent )) 
					{//NOTE: hitting multiple ents can still get you over 100% accuracy
						ent->client->ps.persistant[PERS_ACCURACY_HITS]++;
					} 

					// NO KNOCKBACK CLASS 
					int hitLoc = G_GetHitLocFromTrace( &tr, MOD_CONC_ALT );
					qboolean noKnockBack = (traceEnt->flags&FL_NO_KNOCKBACK);//will be set if they die, I want to know if it was on *before* they died
					( traceEnt && traceEnt->client && traceEnt->client->NPC_class == CLASS_GALAKMECH ||
						traceEnt && traceEnt->client && traceEnt->client->NPC_class == CLASS_GONK ||
						traceEnt && traceEnt->client && traceEnt->client->NPC_class == CLASS_SHADOWTROOPER ||
						traceEnt && traceEnt->client && traceEnt->client->NPC_class == CLASS_SABER_DROID ||
						traceEnt && traceEnt->client && traceEnt->client->NPC_class == CLASS_PROTOCOL ||
						traceEnt && traceEnt->client && traceEnt->client->NPC_class == CLASS_HAZARD_TROOPER ||
						traceEnt && traceEnt->client && traceEnt->client->NPC_class == CLASS_ROCKETTROOPER ||
						traceEnt && traceEnt->client && traceEnt->client->NPC_class == CLASS_HOWLER ||
						traceEnt && traceEnt->client && traceEnt->client->NPC_class == CLASS_GONK ||
						traceEnt && traceEnt->client && traceEnt->client->NPC_class == CLASS_DESANN ||
						traceEnt && traceEnt->client && traceEnt->client->NPC_class == CLASS_COMMANDO ||
						traceEnt && traceEnt->client && traceEnt->client->NPC_class == CLASS_WAMPA ||
						traceEnt && traceEnt->client && traceEnt->client->NPC_class == CLASS_ATST ||
						traceEnt && traceEnt->client && traceEnt->client->NPC_class == CLASS_GOLEM_DARK ||
						traceEnt && traceEnt->client && traceEnt->client->NPC_class == CLASS_GOLEM_SMOKE ||
						traceEnt && traceEnt->client && traceEnt->client->NPC_class == CLASS_VOID ||
						traceEnt && traceEnt->client && traceEnt->client->NPC_class == CLASS_GOLEM_VOID ||
						traceEnt && traceEnt->client && traceEnt->client->NPC_class == CLASS_GOLEM_LIGHT ||
						traceEnt && traceEnt->client && traceEnt->client->NPC_class == CLASS_GOLEM_GOLD ||
						traceEnt && traceEnt->client && traceEnt->client->NPC_class == CLASS_GOLEM_FIRE ||
						traceEnt && traceEnt->client && traceEnt->client->NPC_class == CLASS_HELLGOLEM ||
						traceEnt && traceEnt->client && traceEnt->client->NPC_class == CLASS_GOLEM_LAVA ||
						traceEnt && traceEnt->client && traceEnt->client->NPC_class == CLASS_GOLEM_METAL ||
						traceEnt && traceEnt->client && traceEnt->client->NPC_class == CLASS_CRYSTAL ||
						traceEnt && traceEnt->client && traceEnt->client->NPC_class == CLASS_EARTHREAVER ||
						traceEnt && traceEnt->client && traceEnt->client->NPC_class == CLASS_VAMPIRE_DEMON ||
						traceEnt && traceEnt->client && traceEnt->client->NPC_class == CLASS_GOLEM ||
						traceEnt && traceEnt->client && traceEnt->client->NPC_class == CLASS_GOLEM_WATER ||
						traceEnt && traceEnt->client && traceEnt->client->NPC_class == CLASS_GOLEM_ICE ||
						traceEnt && traceEnt->client && traceEnt->client->NPC_class == CLASS_GOLEM_SONIC ||
						traceEnt && traceEnt->client && traceEnt->client->NPC_class == CLASS_GOLEM_HOLY ||
						traceEnt && traceEnt->client && traceEnt->client->NPC_class == CLASS_GOLEM_NECRO ||
						traceEnt && traceEnt->client && traceEnt->client->NPC_class == CLASS_GOLEM_POISON ||
						traceEnt && traceEnt->client && traceEnt->client->NPC_class == CLASS_NECRO ||
						traceEnt && traceEnt->client && traceEnt->client->NPC_class == CLASS_MURJJ ||
						traceEnt && traceEnt->client && traceEnt->client->NPC_class == CLASS_IVY ||
						traceEnt && traceEnt->client && traceEnt->client->NPC_class == CLASS_DEMON_FIRE ||
						traceEnt && traceEnt->client && traceEnt->client->NPC_class == CLASS_DEMON_POISON ||
						traceEnt && traceEnt->client && traceEnt->client->NPC_class == CLASS_DEMON_ELECTRO ||
						traceEnt && traceEnt->client && traceEnt->client->NPC_class == CLASS_DEMON_BLADE ||
						traceEnt && traceEnt->client && traceEnt->client->NPC_class == CLASS_HYLDENLORD ||
						traceEnt && traceEnt->client && traceEnt->client->NPC_class == CLASS_ELDERGOD ||
						traceEnt && traceEnt->client && traceEnt->client->NPC_class == CLASS_TENTACLE ||
						traceEnt && traceEnt->client && traceEnt->client->NPC_class == CLASS_ARIEL ||
						traceEnt && traceEnt->client && traceEnt->client->NPC_class == CLASS_JANOS ||
						traceEnt && traceEnt->client && traceEnt->client->NPC_class == CLASS_KAIN3 );
					{//hehe
						G_Damage( traceEnt, ent, ent, forwardVec, tr.endpos, 10, DAMAGE_NO_KNOCKBACK|DAMAGE_NO_HIT_LOC, MOD_CONC_ALT, hitLoc );
						break;
					}
					G_Damage( traceEnt, ent, ent, forwardVec, tr.endpos, damage, DAMAGE_NO_KNOCKBACK|DAMAGE_NO_HIT_LOC, MOD_CONC_ALT, hitLoc );

					//do knockback and knockdown manually
					if ( traceEnt->client )
					{//only if we hit a client
						vec3_t pushDir;
						VectorCopy( forwardVec, pushDir );
						if ( pushDir[2] < 0.2f )
						{
							pushDir[2] = 0.2f;
						}//hmm, re-normalize?  nah...
						//if ( traceEnt->NPC || Q_irand(0,g_spskill->integer+1) )
						{
							if ( !noKnockBack )
							{//knock-backable
								G_Throw( traceEnt, pushDir, 200 );
								if ( traceEnt->client->NPC_class == CLASS_ROCKETTROOPER )
								{
									traceEnt->client->ps.pm_time = Q_irand( 1500, 3000 );
								}
							}
							if ( traceEnt->health > 0 )
							{//alive
								if ( G_HasKnockdownAnims( traceEnt ) )
								{//knock-downable
									G_Knockdown( traceEnt, ent, pushDir, 400, qtrue );
								}
							}
						}
					}

					if ( traceEnt->s.eType == ET_MOVER )
					{//stop the traces on any mover
						break;
					}
				}
				else 
				{
					 // we only make this mark on things that can't break or move
					
					//tent = G_TempEntity( tr.endpos, EV_CONC_ALT_MISS );
					
			if ( ent->client->NPC_class ==  CLASS_VAMPIRE_ZOMBIE || 
				ent->client->NPC_class ==  CLASS_GLIDER ||
				ent->client->NPC_class ==  CLASS_TUSKEN ||
				ent->client->NPC_class ==  CLASS_MELC ||
				ent->client->NPC_class ==  CLASS_SUMMONER )
		{
			tent = G_TempEntity( tr.endpos, EV_CONC_ALT_MISS );
		}
	// SPIRITH DEATH: All vampires classes Zephonim \ Alliance \ Dumahim 
	else if ( ent->client->NPC_class ==  CLASS_REBORN ||
				ent->client->NPC_class ==  CLASS_ALORA ||
				ent->client->NPC_class ==  CLASS_MONMOTHA ||
				ent->client->NPC_class ==  CLASS_NOGHRI ||
				ent->client->NPC_class ==  CLASS_FLIER2 || 
				ent->client->NPC_class ==  CLASS_ZEPH ||
				ent->client->NPC_class ==  CLASS_DECEIVER ||
				ent->client->NPC_class ==  CLASS_DUMAHIM || 
				ent->client->NPC_class ==  CLASS_VAMPIRE ||
				ent->client->NPC_class ==  CLASS_KAIN2 || 
				ent->client->NPC_class ==  CLASS_UMAH || 
				ent->client->NPC_class ==  CLASS_NECROMANCER || 
				ent->client->NPC_class ==  CLASS_VAMPIRE_QUEEN ||
				ent->client->NPC_class ==  CLASS_VAMPIRE_DARK || 
				ent->client->NPC_class ==  CLASS_VAMPIRE_NECRO || 
				ent->client->NPC_class ==  CLASS_VAMPIRE_NECRO2 ||
				ent->client->NPC_class ==  CLASS_VAMPIRE_FEAR || 
				ent->client->NPC_class ==  CLASS_VAMPIRE_SOUND || 
				ent->client->NPC_class ==  CLASS_VAMPIRE_LYCAN || 
				ent->client->NPC_class ==  CLASS_VAMPIRE_WATER || 
				ent->client->NPC_class ==  CLASS_VAMPIRE_ARCHER || 
				ent->client->NPC_class ==  CLASS_HALFVAMPIRE || 
				ent->client->NPC_class == CLASS_RAZIELHIM ||  
				ent->client->NPC_class == CLASS_ESKANDOR ||  
				ent->client->NPC_class ==  CLASS_VAMPIRE_BOUNTY || 
				ent->client->NPC_class ==  CLASS_VAMPIRE_SARAFAN )
	{
			tent = G_TempEntity( tr.endpos, EV_SD_ALT_MISS );
	}
	// NECROMANTIC FIRE - RAHABIM   
	else if ( ent->client->NPC_class ==  CLASS_SWAMPTROOPER ||
				ent->client->NPC_class ==  CLASS_RAHABIM || 
				ent->client->NPC_class ==  CLASS_COMMANDO || 
				ent->client->NPC_class ==  CLASS_LYCAN || 
				ent->client->NPC_class ==  CLASS_CENOBITA || 
				ent->client->NPC_class ==  CLASS_WIZARD ||
				ent->client->NPC_class == CLASS_FISH )
	{
			tent = G_TempEntity( tr.endpos, EV_BLACKFIRE_ALT_MISS );
	}
	// SD and IMMOLATION TOGETHER - KAIN 
	else if ( ent->client->NPC_class ==  CLASS_KYLE ||
				ent->client->NPC_class ==  CLASS_KAIN || 
				ent->client->NPC_class ==  CLASS_KAIN3 ||
				ent->client->NPC_class ==  CLASS_TURELIM || 
				ent->client->NPC_class ==  CLASS_VORADOR ||
				ent->client->NPC_class ==  CLASS_JANOS ||
				ent->client->NPC_class ==  CLASS_CLAW || 
				ent->client->NPC_class ==  CLASS_GOLEM_LAVA || 
				ent->client->NPC_class ==  CLASS_VAMPIRE_FEAR2 || 
				ent->client->NPC_class ==  CLASS_VAMPIRE_BLOOD || 
				ent->client->NPC_class ==  CLASS_VAMPIRE_DEMON  )
	{
			tent = G_TempEntity( tr.endpos, EV_IMMOLATION_ALT_MISS );
	}
	// THE HOLY IMMOLATION:  Humans, Saraphan and Druids. 
	else if ( ent->client->NPC_class ==  CLASS_HUNTER || 
				ent->client->NPC_class ==  CLASS_SCOUT ||  
				ent->client->NPC_class ==  CLASS_ALCHEMIST || 
				ent->client->NPC_class ==  CLASS_SARAFAN || 
				ent->client->NPC_class ==  CLASS_ARCHER ||  
				ent->client->NPC_class ==  CLASS_WARRIOR || 
				ent->client->NPC_class ==  CLASS_TRANDOSHAN ||
				ent->client->NPC_class ==  CLASS_STORMTROOPER ||
				ent->client->NPC_class ==  CLASS_PRISONER ||
				ent->client->NPC_class ==  CLASS_RODIAN ||
				ent->client->NPC_class ==  CLASS_IMPWORKER ||
				ent->client->NPC_class ==  CLASS_MARK1 ||
				ent->client->NPC_class ==  CLASS_MARK2 ||
				ent->client->NPC_class ==  CLASS_R2D2 ||
				ent->client->NPC_class ==  CLASS_R5D2 ||
				ent->client->NPC_class ==  CLASS_SEEKER ||
				ent->client->NPC_class ==  CLASS_REMOTE ||
				ent->client->NPC_class ==  CLASS_DRUIDREAVER || 
				ent->client->NPC_class ==  CLASS_RAZIEL || 
				ent->client->NPC_class ==  CLASS_HOLY || 
				ent->client->NPC_class ==  CLASS_GOLEM_HOLY ||
				ent->client->NPC_class == CLASS_JAN || 
				ent->client->NPC_class == CLASS_GALAK )
	{
			tent = G_TempEntity( tr.endpos, EV_HOLYIMMOLATION_ALT_MISS );
		}
	// THE POISON IMMOLATION 
	else if ( ent->client->NPC_class ==  CLASS_SPIDERREAVER3 || 
				ent->client->NPC_class ==  CLASS_BEAST || 
				ent->client->NPC_class ==  CLASS_MINEMONSTER ||
				ent->client->NPC_class ==  CLASS_REELO || 
				ent->client->NPC_class ==  CLASS_POISON || 
				ent->client->NPC_class ==  CLASS_DEMON_POISON || 
				ent->client->NPC_class ==  CLASS_GOLEM_POISON )
		{
			tent = G_TempEntity( tr.endpos, EV_NECROPOISON_ALT_MISS );
		}
	// THE NECROMANTIC FOG - MIND ATK 
	else if ( ent->client->NPC_class ==  CLASS_PROBE ||
				ent->client->NPC_class ==  CLASS_POLTER || 
				ent->client->NPC_class ==  CLASS_REBEL || 
				ent->client->NPC_class ==  CLASS_MENTALIST ||
				ent->client->NPC_class ==  CLASS_AIRREAVER || 
				ent->client->NPC_class ==  CLASS_AIR || 
				ent->client->NPC_class ==  CLASS_GOLEM_AIR || 
				ent->client->NPC_class ==  CLASS_BIRD )
	{
			tent = G_TempEntity( tr.endpos, EV_MIND_ALT_MISS );
		}
	// THE NECROMANTIC SMOKE ATK
	else if ( ent->client->NPC_class ==  CLASS_BESPIN_COP || 				
				ent->client->NPC_class ==  CLASS_DARK || 			 
				ent->client->NPC_class ==  CLASS_GOLEM_DARK || 
				ent->client->NPC_class ==  CLASS_TANKREAVER || 
				ent->client->NPC_class ==  CLASS_GOLEM_SMOKE )
	{
			tent = G_TempEntity( tr.endpos, EV_NECROSMOG_ALT_MISS );
	}
	// THE NECROMANTIC ASTRAL ATK - Dimension  - Secondary Atk: Sonic ray 
	else if ( ent->client->NPC_class ==  CLASS_HOWLER ||
				ent->client->NPC_class ==  CLASS_SONIC || 
				ent->client->NPC_class ==  CLASS_GOLEM_SONIC || 
				ent->client->NPC_class ==  CLASS_LANDO )
	{
			tent = G_TempEntity( tr.endpos, EV_DIM_ALT_MISS );
		}
	// THE NECROMANTIC FULL IMMOLATE ATTACK - MAGMA GUARDIAN \ TURELIM  
	else if ( ent->client->NPC_class ==  CLASS_FIREREAVER || 
				ent->client->NPC_class ==  CLASS_FIRE || 
				ent->client->NPC_class ==  CLASS_GOLEM_FIRE || 
				ent->client->NPC_class ==  CLASS_HELLGOLEM || 
				ent->client->NPC_class ==  CLASS_HAZARD_TROOPER ||
				ent->client->NPC_class ==  CLASS_GRAN ||
				ent->client->NPC_class ==  CLASS_VAMPIRE_FROST || 
				ent->client->NPC_class ==  CLASS_BOBAFETT || 
				ent->client->NPC_class ==  CLASS_WAMPA || 
				ent->client->NPC_class ==  CLASS_DEMON_FIRE )
	{
			tent = G_TempEntity( tr.endpos, EV_CONFLICT_ALT_MISS );
		}
	// THE NECROMANTIC MUD ATK -   
	else if ( ent->client->NPC_class ==  CLASS_SWAMP ||
				ent->client->NPC_class ==  CLASS_INTERROGATOR ||
				ent->client->NPC_class ==  CLASS_SHARK || 
				ent->client->NPC_class ==  CLASS_WATER || 
				ent->client->NPC_class ==  CLASS_GOLEM_WATER || 
				ent->client->NPC_class ==  CLASS_UGNAUGHT || 
				ent->client->NPC_class ==  CLASS_SPIDERREAVER2 || 
				ent->client->NPC_class ==  CLASS_SPIDERREAVER )
	{
			tent = G_TempEntity( tr.endpos, EV_NECROMUD_ALT_MISS );
		}
	// THE NECROMANTIC NATURE ATK - Dimension  
	else if ( ent->client->NPC_class ==  CLASS_LUKE || 
				ent->client->NPC_class ==  CLASS_IVY || 
				ent->client->NPC_class ==  CLASS_DRUID )
	{
			tent = G_TempEntity( tr.endpos, EV_NATURE_ALT_MISS );
		}
	// NECROMANTIC SHOCK ATK - Lightning forge 
	else if ( ent->client->NPC_class ==  CLASS_LIGHTREAVER || 
				ent->client->NPC_class ==  CLASS_GOLEM_METAL || 
				ent->client->NPC_class ==  CLASS_DEMON_ELECTRO || 
				ent->client->NPC_class ==  CLASS_ASSASSIN_DROID || 
				ent->client->NPC_class ==  CLASS_SABER_DROID )
	{
			tent = G_TempEntity( tr.endpos, EV_ELECTRO_ALT_MISS );
		}
	// NECROMANTIC Sun  ATK - Sunlight forge 
	else if ( ent->client->NPC_class ==  CLASS_LIGHT || 
				ent->client->NPC_class ==  CLASS_GOLEM_LIGHT || 
				ent->client->NPC_class ==  CLASS_GOLEM_GOLD || 
				ent->client->NPC_class ==  CLASS_MORGANKATARN ||
				ent->client->NPC_class ==  CLASS_PROTOCOL )
	{
			tent = G_TempEntity( tr.endpos, EV_ENERGY_ALT_MISS );
	}
	// NECROMANTIC TIME ATK - Storm Forge - Primary shoot: sandstorm devil. secondary: time slam 
	else if ( ent->client->NPC_class == CLASS_ROCKETTROOPER ||
		ent->client->NPC_class == CLASS_VAMPIRE_ELF3 )
		{
			tent = G_TempEntity( tr.endpos, EV_TIME_ALT_MISS );
		}
	// NECROMANTIC STONE ATK 
	else if ( ent->client->NPC_class ==  CLASS_GONK ||
				ent->client->NPC_class ==  CLASS_LIZARD ||
				ent->client->NPC_class ==  CLASS_VAMPIRE_ELF || 
				ent->client->NPC_class ==  CLASS_VAMPIRE_ELF2 || 
				ent->client->NPC_class ==  CLASS_EARTHREAVER || 
				ent->client->NPC_class ==  CLASS_EARTH || 
				ent->client->NPC_class ==  CLASS_GOLEM || 
				ent->client->NPC_class ==  CLASS_DEMON_BLADE )
	{
			tent = G_TempEntity( tr.endpos, EV_ROCK_ALT_MISS );
	}
	// NECROMANTIC STATES CRYSTAL RAY
	else if ( ent->client->NPC_class ==  CLASS_WEEQUAY ||
				ent->client->NPC_class ==  CLASS_BARTENDER ||
				ent->client->NPC_class ==  CLASS_GOLEM_ICE || 
				ent->client->NPC_class ==  CLASS_ICEREAVER || 
				ent->client->NPC_class ==  CLASS_CRYSTAL || 
				ent->client->NPC_class ==  CLASS_GALAKMECH  )
	{
			tent = G_TempEntity( tr.endpos, EV_NECROCRYSTAL_ALT_MISS );
	}
	// NECROMANTIC CHAOS 
	else if ( ent->client->NPC_class ==  CLASS_PROPHET || 
				ent->client->NPC_class ==  CLASS_TAVION ||
				ent->client->NPC_class ==  CLASS_DESANN ||
				ent->client->NPC_class ==  CLASS_HYLDEN || 
				ent->client->NPC_class ==  CLASS_HYLDENLORD || 
				ent->client->NPC_class ==  CLASS_ELDERGOD || 
				ent->client->NPC_class ==  CLASS_TENTACLE || 
				ent->client->NPC_class ==  CLASS_ATST ||
				ent->client->NPC_class ==  CLASS_SAND_CREATURE ||
				ent->client->NPC_class ==  CLASS_MOUSE || 
				ent->client->NPC_class ==  CLASS_SENTRY )
		{
			tent = G_TempEntity( tr.endpos, EV_CHAOS_ALT_MISS );
		}
	// NECROMANTIC VOID 
	else if ( ent->client->NPC_class ==  CLASS_SABOTEUR || 
				ent->client->NPC_class ==  CLASS_SHADOWTROOPER || 
				ent->client->NPC_class ==  CLASS_VOID || 
				ent->client->NPC_class ==  CLASS_GOLEM_VOID || 
				ent->client->NPC_class ==  CLASS_DARKREAVER )
	{
			tent = G_TempEntity( tr.endpos, EV_NECROVOID_ALT_MISS );
		}
	// NECROMANTIC DEATH FIRE (FOR ALL WRAITH, REAPERS AND GHOSTS) 
	else if ( ent->client->NPC_class ==  CLASS_ARIEL || 
				ent->client->NPC_class ==  CLASS_GHOST || 
				ent->client->NPC_class ==  CLASS_REAPER || 
				ent->client->NPC_class ==  CLASS_DREADNOUGHT || 
				ent->client->NPC_class ==  CLASS_VAMPIRE_GHOST || 
				ent->client->NPC_class ==  CLASS_JEDI ||
				ent->client->NPC_class ==  CLASS_REAVER || 
				ent->client->NPC_class ==  CLASS_NECROREAVER || 
				ent->client->NPC_class ==  CLASS_THRALL || 
				ent->client->NPC_class ==  CLASS_NECRO || 
				ent->client->NPC_class ==  CLASS_GOLEM_NECRO || 
				ent->client->NPC_class ==  CLASS_JAWA ||
				ent->client->NPC_class ==  CLASS_MURJJ || 
				ent->client->NPC_class ==  CLASS_RANCOR || 
				ent->client->NPC_class ==  CLASS_PLAYER )
	{
			tent = G_TempEntity( tr.endpos, EV_NECRO_ALT_MISS );
	}
					tent->svFlags |= SVF_BROADCAST;
					VectorCopy( tr.plane.normal, tent->pos1 );
					break; // hit solid, but doesn't take damage, so stop the shot...we _could_ allow it to shoot through walls, might be cool?
				}
			}
			else // not rendering impact, must be a skybox or other similar thing?
			{
				break; // don't try anymore traces
			}
		}
		// Get ready for an attempt to trace through another person
		VectorCopy( tr.endpos, muzzle2 );
		VectorCopy( tr.endpos, start );
		skip = tr.entityNum;
		hitDodged = qfalse;
	}
	//just draw one beam all the way to the end
	tent = G_TempEntity( tr.endpos, EV_CONC_ALT_SHOT );
	tent->svFlags |= SVF_BROADCAST;
	VectorCopy( muzzle, tent->s.origin2 );

	// now go along the trail and make sight events
	VectorSubtract( tr.endpos, muzzle, dir );

	shotDist = VectorNormalize( dir );

	//FIXME: if shoot *really* close to someone, the alert could be way out of their FOV
	for ( dist = 0; dist < shotDist; dist += 64 )
	{
		//FIXME: on a really long shot, this could make a LOT of alerts in one frame...
		VectorMA( muzzle, dist, dir, spot );
		AddSightEvent( ent, spot, 256, AEL_DISCOVERED, 50 );
		//FIXME: creates *way* too many effects, make it one effect somehow?
		/////////////////////////////// ALT RING ///////////////////////////////
	if ( ent->client->NPC_class ==  CLASS_VAMPIRE_ZOMBIE || 
				ent->client->NPC_class ==  CLASS_GLIDER ||
				ent->client->NPC_class ==  CLASS_TUSKEN ||
				ent->client->NPC_class ==  CLASS_MELC ||
				ent->client->NPC_class ==  CLASS_SUMMONER )
	{
		{
			G_PlayEffect( G_EffectIndex( "concussion/alt_ring" ), ent->client->renderInfo.handRPoint );
		}
	}
// SPIRITH DEATH: All vampires classes Zephonim \ Alliance \ Dumahim 
	else if ( ent->client->NPC_class ==  CLASS_REBORN ||
				ent->client->NPC_class ==  CLASS_ALORA ||
				ent->client->NPC_class ==  CLASS_MONMOTHA ||
				ent->client->NPC_class ==  CLASS_NOGHRI ||
				ent->client->NPC_class ==  CLASS_FLIER2 || 
				ent->client->NPC_class ==  CLASS_ZEPH ||
				ent->client->NPC_class ==  CLASS_DECEIVER ||
				ent->client->NPC_class ==  CLASS_DUMAHIM || 
				ent->client->NPC_class ==  CLASS_VAMPIRE ||
				ent->client->NPC_class ==  CLASS_KAIN2 || 
				ent->client->NPC_class ==  CLASS_UMAH || 
				ent->client->NPC_class ==  CLASS_NECROMANCER || 
				ent->client->NPC_class ==  CLASS_VAMPIRE_QUEEN ||
				ent->client->NPC_class ==  CLASS_VAMPIRE_DARK || 
				ent->client->NPC_class ==  CLASS_VAMPIRE_NECRO || 
				ent->client->NPC_class ==  CLASS_VAMPIRE_NECRO2 ||
				ent->client->NPC_class ==  CLASS_VAMPIRE_FEAR || 
				ent->client->NPC_class ==  CLASS_VAMPIRE_SOUND || 
				ent->client->NPC_class ==  CLASS_VAMPIRE_LYCAN || 
				ent->client->NPC_class ==  CLASS_VAMPIRE_WATER || 
				ent->client->NPC_class ==  CLASS_VAMPIRE_ARCHER || 
				ent->client->NPC_class ==  CLASS_HALFVAMPIRE || 
				ent->client->NPC_class == CLASS_RAZIELHIM ||  
				ent->client->NPC_class == CLASS_ESKANDOR ||  
				ent->client->NPC_class ==  CLASS_VAMPIRE_BOUNTY || 
				ent->client->NPC_class ==  CLASS_VAMPIRE_SARAFAN )
	{
		{
			G_PlayEffect( G_EffectIndex( "spirith_death/alt_ring" ), ent->client->renderInfo.handRPoint );
		}
	}
	// NECROMANTIC FIRE - RAHABIM   
	else if ( ent->client->NPC_class ==  CLASS_SWAMPTROOPER ||
				ent->client->NPC_class ==  CLASS_RAHABIM || 
				ent->client->NPC_class ==  CLASS_COMMANDO || 
				ent->client->NPC_class ==  CLASS_LYCAN || 
				ent->client->NPC_class ==  CLASS_CENOBITA || 
				ent->client->NPC_class ==  CLASS_WIZARD ||
				ent->client->NPC_class == CLASS_FISH )
	{
		{
			G_PlayEffect( G_EffectIndex( "blackfire/alt_ring" ), ent->client->renderInfo.handRPoint );
		}
	}
	// SD and IMMOLATION TOGETHER - KAIN 
	else if ( ent->client->NPC_class ==  CLASS_KYLE ||
				ent->client->NPC_class ==  CLASS_KAIN || 
				ent->client->NPC_class ==  CLASS_KAIN3 ||
				ent->client->NPC_class ==  CLASS_TURELIM || 
				ent->client->NPC_class ==  CLASS_VORADOR ||
				ent->client->NPC_class ==  CLASS_JANOS ||
				ent->client->NPC_class ==  CLASS_CLAW || 
				ent->client->NPC_class ==  CLASS_GOLEM_LAVA || 
				ent->client->NPC_class ==  CLASS_VAMPIRE_FEAR2 || 
				ent->client->NPC_class ==  CLASS_VAMPIRE_BLOOD || 
				ent->client->NPC_class ==  CLASS_VAMPIRE_DEMON  )
	{
		{
			G_PlayEffect( G_EffectIndex( "immolate/alt_ring" ), ent->client->renderInfo.handRPoint );
		}
	}
	// THE HOLY IMMOLATION:  Humans, Saraphan and Druids. 
	else if ( ent->client->NPC_class ==  CLASS_HUNTER || 
				ent->client->NPC_class ==  CLASS_SCOUT ||  
				ent->client->NPC_class ==  CLASS_ALCHEMIST || 
				ent->client->NPC_class ==  CLASS_SARAFAN || 
				ent->client->NPC_class ==  CLASS_ARCHER ||  
				ent->client->NPC_class ==  CLASS_WARRIOR || 
				ent->client->NPC_class ==  CLASS_TRANDOSHAN ||
				ent->client->NPC_class ==  CLASS_STORMTROOPER ||
				ent->client->NPC_class ==  CLASS_PRISONER ||
				ent->client->NPC_class ==  CLASS_RODIAN ||
				ent->client->NPC_class ==  CLASS_IMPWORKER ||
				ent->client->NPC_class ==  CLASS_MARK1 ||
				ent->client->NPC_class ==  CLASS_MARK2 ||
				ent->client->NPC_class ==  CLASS_R2D2 ||
				ent->client->NPC_class ==  CLASS_R5D2 ||
				ent->client->NPC_class ==  CLASS_SEEKER ||
				ent->client->NPC_class ==  CLASS_REMOTE ||
				ent->client->NPC_class ==  CLASS_DRUIDREAVER || 
				ent->client->NPC_class ==  CLASS_RAZIEL || 
				ent->client->NPC_class ==  CLASS_HOLY || 
				ent->client->NPC_class ==  CLASS_GOLEM_HOLY ||
				ent->client->NPC_class == CLASS_JAN || 
				ent->client->NPC_class == CLASS_GALAK )
	{
		{
			G_PlayEffect( G_EffectIndex( "holyimmolate/alt_ring" ), ent->client->renderInfo.handRPoint );
		}
	}
	// THE POISON IMMOLATION 
	else if ( ent->client->NPC_class ==  CLASS_SPIDERREAVER3 || 
				ent->client->NPC_class ==  CLASS_BEAST || 
				ent->client->NPC_class ==  CLASS_MINEMONSTER ||
				ent->client->NPC_class ==  CLASS_REELO || 
				ent->client->NPC_class ==  CLASS_POISON || 
				ent->client->NPC_class ==  CLASS_DEMON_POISON || 
				ent->client->NPC_class ==  CLASS_GOLEM_POISON )
	{
		{
			G_PlayEffect( G_EffectIndex( "necro_poison/alt_ring" ), ent->client->renderInfo.handRPoint );
		}
	}
	// THE NECROMANTIC FOG - MIND ATK 
	else if ( ent->client->NPC_class ==  CLASS_PROBE ||
				ent->client->NPC_class ==  CLASS_POLTER || 
				ent->client->NPC_class ==  CLASS_REBEL || 
				ent->client->NPC_class ==  CLASS_MENTALIST ||
				ent->client->NPC_class ==  CLASS_AIRREAVER || 
				ent->client->NPC_class ==  CLASS_AIR || 
				ent->client->NPC_class ==  CLASS_GOLEM_AIR || 
				ent->client->NPC_class ==  CLASS_BIRD )
	{
		{
			G_PlayEffect( G_EffectIndex( "necro_fog/alt_ring" ), ent->client->renderInfo.handRPoint );
		}
	}
	// THE NECROMANTIC SMOKE ATK
	else if ( ent->client->NPC_class ==  CLASS_BESPIN_COP || 				
				ent->client->NPC_class ==  CLASS_DARK || 			 
				ent->client->NPC_class ==  CLASS_GOLEM_DARK || 
				ent->client->NPC_class ==  CLASS_TANKREAVER || 
				ent->client->NPC_class ==  CLASS_GOLEM_SMOKE )
	{
		{
			G_PlayEffect( G_EffectIndex( "necro_smog/alt_ring" ), ent->client->renderInfo.handRPoint );
		}
	}
	// THE NECROMANTIC ASTRAL ATK - Dimension  - Secondary Atk: Sonic ray 
	else if ( ent->client->NPC_class ==  CLASS_HOWLER ||
				ent->client->NPC_class ==  CLASS_SONIC || 
				ent->client->NPC_class ==  CLASS_GOLEM_SONIC || 
				ent->client->NPC_class ==  CLASS_LANDO )
	{
		{
			G_PlayEffect( G_EffectIndex( "necro_dim/alt_ring" ), ent->client->renderInfo.handRPoint );
		}
	}
	// THE NECROMANTIC FULL IMMOLATE ATTACK - MAGMA GUARDIAN \ TURELIM  
	else if ( ent->client->NPC_class ==  CLASS_FIREREAVER || 
				ent->client->NPC_class ==  CLASS_FIRE || 
				ent->client->NPC_class ==  CLASS_GOLEM_FIRE || 
				ent->client->NPC_class ==  CLASS_HELLGOLEM || 
				ent->client->NPC_class ==  CLASS_HAZARD_TROOPER ||
				ent->client->NPC_class ==  CLASS_GRAN ||
				ent->client->NPC_class ==  CLASS_VAMPIRE_FROST || 
				ent->client->NPC_class ==  CLASS_BOBAFETT || 
				ent->client->NPC_class ==  CLASS_WAMPA || 
				ent->client->NPC_class ==  CLASS_DEMON_FIRE )
	{
		{
			G_PlayEffect( G_EffectIndex( "necro_conflict/alt_ring" ), ent->client->renderInfo.handRPoint );
		}
	}
	// THE NECROMANTIC MUD ATK -   
	else if ( ent->client->NPC_class ==  CLASS_SWAMP ||
				ent->client->NPC_class ==  CLASS_INTERROGATOR ||
				ent->client->NPC_class ==  CLASS_SHARK || 
				ent->client->NPC_class ==  CLASS_WATER || 
				ent->client->NPC_class ==  CLASS_GOLEM_WATER || 
				ent->client->NPC_class ==  CLASS_UGNAUGHT || 
				ent->client->NPC_class ==  CLASS_SPIDERREAVER2 || 
				ent->client->NPC_class ==  CLASS_SPIDERREAVER )
	{
		{
			G_PlayEffect( G_EffectIndex( "necro_mud/alt_ring" ), ent->client->renderInfo.handRPoint );
		}
	}
	// THE NECROMANTIC NATURE ATK - Dimension  
	else if ( ent->client->NPC_class ==  CLASS_LUKE || 
				ent->client->NPC_class ==  CLASS_IVY || 
				ent->client->NPC_class ==  CLASS_DRUID )
	{
		{
			G_PlayEffect( G_EffectIndex( "necro_nature/alt_ring" ), ent->client->renderInfo.handRPoint );
		}
	}
	// NECROMANTIC SHOCK ATK - Lightning forge 
	else if ( ent->client->NPC_class ==  CLASS_LIGHTREAVER || 
				ent->client->NPC_class ==  CLASS_GOLEM_METAL || 
				ent->client->NPC_class ==  CLASS_DEMON_ELECTRO || 
				ent->client->NPC_class ==  CLASS_ASSASSIN_DROID || 
				ent->client->NPC_class ==  CLASS_SABER_DROID )
	{
		{
			G_PlayEffect( G_EffectIndex( "necro_shock/alt_ring" ), ent->client->renderInfo.handRPoint );
		}
	}
	// NECROMANTIC Sun  ATK - Sunlight forge 
	else if ( ent->client->NPC_class ==  CLASS_LIGHT || 
				ent->client->NPC_class ==  CLASS_GOLEM_LIGHT || 
				ent->client->NPC_class ==  CLASS_GOLEM_GOLD || 
				ent->client->NPC_class ==  CLASS_MORGANKATARN ||
				ent->client->NPC_class ==  CLASS_PROTOCOL )
	{
		{
			G_PlayEffect( G_EffectIndex( "necro_energy/alt_ring" ), ent->client->renderInfo.handRPoint );
		}
	}
	// NECROMANTIC TIME ATK - Storm Forge - Primary shoot: sandstorm devil. secondary: time slam 
	else if ( ent->client->NPC_class == CLASS_ROCKETTROOPER ||
		ent->client->NPC_class == CLASS_VAMPIRE_ELF3 )
{
		{
			G_PlayEffect( G_EffectIndex( "necro_time/alt_ring" ), ent->client->renderInfo.handRPoint );
		}
	}
	// NECROMANTIC STONE ATK 
	else if ( ent->client->NPC_class ==  CLASS_GONK ||
				ent->client->NPC_class ==  CLASS_LIZARD ||
				ent->client->NPC_class ==  CLASS_VAMPIRE_ELF || 
				ent->client->NPC_class ==  CLASS_VAMPIRE_ELF2 || 
				ent->client->NPC_class ==  CLASS_EARTHREAVER || 
				ent->client->NPC_class ==  CLASS_EARTH || 
				ent->client->NPC_class ==  CLASS_GOLEM || 
				ent->client->NPC_class ==  CLASS_DEMON_BLADE )
	{
		{
			G_PlayEffect( G_EffectIndex( "necro_stone/alt_ring" ), ent->client->renderInfo.handRPoint );
		}
	}
	// NECROMANTIC STATES CRYSTAL RAY
	else if ( ent->client->NPC_class ==  CLASS_WEEQUAY ||
				ent->client->NPC_class ==  CLASS_BARTENDER ||
				ent->client->NPC_class ==  CLASS_GOLEM_ICE || 
				ent->client->NPC_class ==  CLASS_ICEREAVER || 
				ent->client->NPC_class ==  CLASS_CRYSTAL || 
				ent->client->NPC_class ==  CLASS_GALAKMECH  )
	{
		{
			G_PlayEffect( G_EffectIndex( "necro_crystal/alt_ring" ), ent->client->renderInfo.handRPoint );
		}
	}
	// NECROMANTIC CHAOS 
	else if ( ent->client->NPC_class ==  CLASS_PROPHET || 
				ent->client->NPC_class ==  CLASS_TAVION ||
				ent->client->NPC_class ==  CLASS_DESANN ||
				ent->client->NPC_class ==  CLASS_HYLDEN || 
				ent->client->NPC_class ==  CLASS_HYLDENLORD || 
				ent->client->NPC_class ==  CLASS_ELDERGOD || 
				ent->client->NPC_class ==  CLASS_TENTACLE || 
				ent->client->NPC_class ==  CLASS_ATST ||
				ent->client->NPC_class ==  CLASS_SAND_CREATURE ||
				ent->client->NPC_class ==  CLASS_MOUSE || 
				ent->client->NPC_class ==  CLASS_SENTRY )
{
		{
			G_PlayEffect( G_EffectIndex( "necro_chaos/alt_ring" ), ent->client->renderInfo.handRPoint );
		}
	}
	// NECROMANTIC VOID 
	else if ( ent->client->NPC_class ==  CLASS_SABOTEUR || 
				ent->client->NPC_class ==  CLASS_SHADOWTROOPER || 
				ent->client->NPC_class ==  CLASS_VOID || 
				ent->client->NPC_class ==  CLASS_GOLEM_VOID || 
				ent->client->NPC_class ==  CLASS_DARKREAVER )
	{
		{
			G_PlayEffect( G_EffectIndex( "necro_void/alt_ring" ), ent->client->renderInfo.handRPoint );
		}
	}
	// NECROMANTIC DEATH FIRE (FOR ALL WRAITH, REAPERS AND GHOSTS) 
	else if ( ent->client->NPC_class ==  CLASS_ARIEL || 
				ent->client->NPC_class ==  CLASS_GHOST || 
				ent->client->NPC_class ==  CLASS_REAPER || 
				ent->client->NPC_class ==  CLASS_DREADNOUGHT || 
				ent->client->NPC_class ==  CLASS_VAMPIRE_GHOST || 
				ent->client->NPC_class ==  CLASS_JEDI ||
				ent->client->NPC_class ==  CLASS_REAVER || 
				ent->client->NPC_class ==  CLASS_NECROREAVER || 
				ent->client->NPC_class ==  CLASS_THRALL || 
				ent->client->NPC_class ==  CLASS_NECRO || 
				ent->client->NPC_class ==  CLASS_GOLEM_NECRO || 
				ent->client->NPC_class ==  CLASS_JAWA ||
				ent->client->NPC_class ==  CLASS_MURJJ || 
				ent->client->NPC_class ==  CLASS_RANCOR || 
				ent->client->NPC_class ==  CLASS_PLAYER )
	{
		{
			G_PlayEffect( G_EffectIndex( "necro/alt_ring" ), ent->client->renderInfo.handRPoint );
		}
	}	

		// G_PlayEffect( G_EffectIndex( "concussion/alt_ring" ), spot, forwardVec );
	}
	//FIXME: spawn a temp ent that continuously spawns sight alerts here?  And 1 sound alert to draw their attention?
	VectorMA( start, shotDist-4, forwardVec, spot );
	AddSightEvent( ent, spot, 256, AEL_DISCOVERED, 50 );

////////////////////////// ALT MUZZLE FLASH ////////////////////////
	if ( ent->client->NPC_class ==  CLASS_VAMPIRE_ZOMBIE || 
				ent->client->NPC_class ==  CLASS_GLIDER ||
				ent->client->NPC_class ==  CLASS_TUSKEN ||
				ent->client->NPC_class ==  CLASS_MELC ||
				ent->client->NPC_class ==  CLASS_SUMMONER )
	{
		{
			G_PlayEffect( G_EffectIndex( "concussion/altmuzzle_flash" ), ent->client->renderInfo.handRPoint );
		}
	}
// SPIRITH DEATH: All vampires classes Zephonim \ Alliance \ Dumahim 
	else if ( ent->client->NPC_class ==  CLASS_REBORN ||
				ent->client->NPC_class ==  CLASS_ALORA ||
				ent->client->NPC_class ==  CLASS_MONMOTHA ||
				ent->client->NPC_class ==  CLASS_NOGHRI ||
				ent->client->NPC_class ==  CLASS_FLIER2 || 
				ent->client->NPC_class ==  CLASS_ZEPH ||
				ent->client->NPC_class ==  CLASS_DECEIVER ||
				ent->client->NPC_class ==  CLASS_DUMAHIM || 
				ent->client->NPC_class ==  CLASS_VAMPIRE ||
				ent->client->NPC_class ==  CLASS_KAIN2 || 
				ent->client->NPC_class ==  CLASS_UMAH || 
				ent->client->NPC_class ==  CLASS_NECROMANCER || 
				ent->client->NPC_class ==  CLASS_VAMPIRE_QUEEN ||
				ent->client->NPC_class ==  CLASS_VAMPIRE_DARK || 
				ent->client->NPC_class ==  CLASS_VAMPIRE_NECRO || 
				ent->client->NPC_class ==  CLASS_VAMPIRE_NECRO2 ||
				ent->client->NPC_class ==  CLASS_VAMPIRE_FEAR || 
				ent->client->NPC_class ==  CLASS_VAMPIRE_SOUND || 
				ent->client->NPC_class ==  CLASS_VAMPIRE_LYCAN || 
				ent->client->NPC_class ==  CLASS_VAMPIRE_WATER || 
				ent->client->NPC_class ==  CLASS_VAMPIRE_ARCHER || 
				ent->client->NPC_class ==  CLASS_HALFVAMPIRE || 
				ent->client->NPC_class == CLASS_RAZIELHIM ||  
				ent->client->NPC_class == CLASS_ESKANDOR ||  
				ent->client->NPC_class ==  CLASS_VAMPIRE_BOUNTY || 
				ent->client->NPC_class ==  CLASS_VAMPIRE_SARAFAN )
	{
		{
			G_PlayEffect( G_EffectIndex( "spirith_death/altmuzzle_flash" ), ent->client->renderInfo.handRPoint );
		}
	}
	// NECROMANTIC FIRE - RAHABIM   
	else if ( ent->client->NPC_class ==  CLASS_SWAMPTROOPER ||
				ent->client->NPC_class ==  CLASS_RAHABIM || 
				ent->client->NPC_class ==  CLASS_COMMANDO || 
				ent->client->NPC_class ==  CLASS_LYCAN || 
				ent->client->NPC_class ==  CLASS_CENOBITA || 
				ent->client->NPC_class ==  CLASS_WIZARD ||
				ent->client->NPC_class == CLASS_FISH )
	{
		{
			G_PlayEffect( G_EffectIndex( "blackfire/altmuzzle_flash" ), ent->client->renderInfo.handRPoint );
		}
	}
	// SD and IMMOLATION TOGETHER - KAIN 
	else if ( ent->client->NPC_class ==  CLASS_KYLE ||
				ent->client->NPC_class ==  CLASS_KAIN || 
				ent->client->NPC_class ==  CLASS_KAIN3 ||
				ent->client->NPC_class ==  CLASS_TURELIM || 
				ent->client->NPC_class ==  CLASS_VORADOR ||
				ent->client->NPC_class ==  CLASS_JANOS ||
				ent->client->NPC_class ==  CLASS_CLAW || 
				ent->client->NPC_class ==  CLASS_GOLEM_LAVA || 
				ent->client->NPC_class ==  CLASS_VAMPIRE_FEAR2 || 
				ent->client->NPC_class ==  CLASS_VAMPIRE_BLOOD || 
				ent->client->NPC_class ==  CLASS_VAMPIRE_DEMON  )
	{
		{
			G_PlayEffect( G_EffectIndex( "immolate/altmuzzle_flash" ), ent->client->renderInfo.handRPoint );
		}
	}
	// THE HOLY IMMOLATION:  Humans, Saraphan and Druids. 
	else if ( ent->client->NPC_class ==  CLASS_HUNTER || 
				ent->client->NPC_class ==  CLASS_SCOUT ||  
				ent->client->NPC_class ==  CLASS_ALCHEMIST || 
				ent->client->NPC_class ==  CLASS_SARAFAN || 
				ent->client->NPC_class ==  CLASS_ARCHER ||  
				ent->client->NPC_class ==  CLASS_WARRIOR || 
				ent->client->NPC_class ==  CLASS_TRANDOSHAN ||
				ent->client->NPC_class ==  CLASS_STORMTROOPER ||
				ent->client->NPC_class ==  CLASS_PRISONER ||
				ent->client->NPC_class ==  CLASS_RODIAN ||
				ent->client->NPC_class ==  CLASS_IMPWORKER ||
				ent->client->NPC_class ==  CLASS_MARK1 ||
				ent->client->NPC_class ==  CLASS_MARK2 ||
				ent->client->NPC_class ==  CLASS_R2D2 ||
				ent->client->NPC_class ==  CLASS_R5D2 ||
				ent->client->NPC_class ==  CLASS_SEEKER ||
				ent->client->NPC_class ==  CLASS_REMOTE ||
				ent->client->NPC_class ==  CLASS_DRUIDREAVER || 
				ent->client->NPC_class ==  CLASS_RAZIEL || 
				ent->client->NPC_class ==  CLASS_HOLY || 
				ent->client->NPC_class ==  CLASS_GOLEM_HOLY ||
				ent->client->NPC_class == CLASS_JAN || 
				ent->client->NPC_class == CLASS_GALAK )
	{
		{
			G_PlayEffect( G_EffectIndex( "holyimmolate/altmuzzle_flash" ), ent->client->renderInfo.handRPoint );
		}
	}
	// THE POISON IMMOLATION 
	else if ( ent->client->NPC_class ==  CLASS_SPIDERREAVER3 || 
				ent->client->NPC_class ==  CLASS_BEAST || 
				ent->client->NPC_class ==  CLASS_MINEMONSTER ||
				ent->client->NPC_class ==  CLASS_REELO || 
				ent->client->NPC_class ==  CLASS_POISON || 
				ent->client->NPC_class ==  CLASS_DEMON_POISON || 
				ent->client->NPC_class ==  CLASS_GOLEM_POISON )
	{
		{
			G_PlayEffect( G_EffectIndex( "necro_poison/altmuzzle_flash" ), ent->client->renderInfo.handRPoint );
		}
	}
	// THE NECROMANTIC FOG - MIND ATK 
	else if ( ent->client->NPC_class ==  CLASS_PROBE ||
				ent->client->NPC_class ==  CLASS_POLTER || 
				ent->client->NPC_class ==  CLASS_REBEL || 
				ent->client->NPC_class ==  CLASS_MENTALIST ||
				ent->client->NPC_class ==  CLASS_AIRREAVER || 
				ent->client->NPC_class ==  CLASS_AIR || 
				ent->client->NPC_class ==  CLASS_GOLEM_AIR || 
				ent->client->NPC_class ==  CLASS_BIRD )
	{
		{
			G_PlayEffect( G_EffectIndex( "necro_fog/altmuzzle_flash" ), ent->client->renderInfo.handRPoint );
		}
	}
	// THE NECROMANTIC SMOKE ATK
	else if ( ent->client->NPC_class ==  CLASS_BESPIN_COP || 				
				ent->client->NPC_class ==  CLASS_DARK || 			 
				ent->client->NPC_class ==  CLASS_GOLEM_DARK || 
				ent->client->NPC_class ==  CLASS_TANKREAVER || 
				ent->client->NPC_class ==  CLASS_GOLEM_SMOKE )
	{
		{
			G_PlayEffect( G_EffectIndex( "necro_smog/altmuzzle_flash" ), ent->client->renderInfo.handRPoint );
		}
	}
	// THE NECROMANTIC ASTRAL ATK - Dimension  - Secondary Atk: Sonic ray 
	else if ( ent->client->NPC_class ==  CLASS_HOWLER ||
				ent->client->NPC_class ==  CLASS_SONIC || 
				ent->client->NPC_class ==  CLASS_GOLEM_SONIC || 
				ent->client->NPC_class ==  CLASS_LANDO )
	{
		{
			G_PlayEffect( G_EffectIndex( "necro_dim/altmuzzle_flash" ), ent->client->renderInfo.handRPoint );
		}
	}
	// THE NECROMANTIC FULL IMMOLATE ATTACK - MAGMA GUARDIAN \ TURELIM  
	else if ( ent->client->NPC_class ==  CLASS_FIREREAVER || 
				ent->client->NPC_class ==  CLASS_FIRE || 
				ent->client->NPC_class ==  CLASS_GOLEM_FIRE || 
				ent->client->NPC_class ==  CLASS_HELLGOLEM || 
				ent->client->NPC_class ==  CLASS_HAZARD_TROOPER ||
				ent->client->NPC_class ==  CLASS_GRAN ||
				ent->client->NPC_class ==  CLASS_VAMPIRE_FROST || 
				ent->client->NPC_class ==  CLASS_BOBAFETT || 
				ent->client->NPC_class ==  CLASS_WAMPA || 
				ent->client->NPC_class ==  CLASS_DEMON_FIRE )
	{
		{
			G_PlayEffect( G_EffectIndex( "necro_conflict/altmuzzle_flash" ), ent->client->renderInfo.handRPoint );
		}
	}
	// THE NECROMANTIC MUD ATK -   
	else if ( ent->client->NPC_class ==  CLASS_SWAMP ||
				ent->client->NPC_class ==  CLASS_INTERROGATOR ||
				ent->client->NPC_class ==  CLASS_SHARK || 
				ent->client->NPC_class ==  CLASS_WATER || 
				ent->client->NPC_class ==  CLASS_GOLEM_WATER || 
				ent->client->NPC_class ==  CLASS_UGNAUGHT || 
				ent->client->NPC_class ==  CLASS_SPIDERREAVER2 || 
				ent->client->NPC_class ==  CLASS_SPIDERREAVER )
	{
		{
			G_PlayEffect( G_EffectIndex( "necro_mud/altmuzzle_flash" ), ent->client->renderInfo.handRPoint );
		}
	}
	// THE NECROMANTIC NATURE ATK - Dimension  
	else if ( ent->client->NPC_class ==  CLASS_LUKE || 
				ent->client->NPC_class ==  CLASS_IVY || 
				ent->client->NPC_class ==  CLASS_DRUID )
	{
		{
			G_PlayEffect( G_EffectIndex( "necro_nature/altmuzzle_flash" ), ent->client->renderInfo.handRPoint );
		}
	}
	// NECROMANTIC SHOCK ATK - Lightning forge 
	else if ( ent->client->NPC_class ==  CLASS_LIGHTREAVER || 
				ent->client->NPC_class ==  CLASS_GOLEM_METAL || 
				ent->client->NPC_class ==  CLASS_DEMON_ELECTRO || 
				ent->client->NPC_class ==  CLASS_ASSASSIN_DROID || 
				ent->client->NPC_class ==  CLASS_SABER_DROID )
	{
		{
			G_PlayEffect( G_EffectIndex( "necro_shock/altmuzzle_flash" ), ent->client->renderInfo.handRPoint );
		}
	}
	// NECROMANTIC Sun  ATK - Sunlight forge 
	else if ( ent->client->NPC_class ==  CLASS_LIGHT || 
				ent->client->NPC_class ==  CLASS_GOLEM_LIGHT || 
				ent->client->NPC_class ==  CLASS_GOLEM_GOLD || 
				ent->client->NPC_class ==  CLASS_MORGANKATARN ||
				ent->client->NPC_class ==  CLASS_PROTOCOL )
	{
		{
			G_PlayEffect( G_EffectIndex( "necro_energy/altmuzzle_flash" ), ent->client->renderInfo.handRPoint );
		}
	}
	// NECROMANTIC TIME ATK - Storm Forge - Primary shoot: sandstorm devil. secondary: time slam 
	else if ( ent->client->NPC_class == CLASS_ROCKETTROOPER ||
		ent->client->NPC_class == CLASS_VAMPIRE_ELF3 )
{
		{
			G_PlayEffect( G_EffectIndex( "necro_time/altmuzzle_flash" ), ent->client->renderInfo.handRPoint );
		}
	}
	// NECROMANTIC STONE ATK 
	else if ( ent->client->NPC_class ==  CLASS_GONK ||
				ent->client->NPC_class ==  CLASS_LIZARD ||
				ent->client->NPC_class ==  CLASS_VAMPIRE_ELF || 
				ent->client->NPC_class ==  CLASS_VAMPIRE_ELF2 || 
				ent->client->NPC_class ==  CLASS_EARTHREAVER || 
				ent->client->NPC_class ==  CLASS_EARTH || 
				ent->client->NPC_class ==  CLASS_GOLEM || 
				ent->client->NPC_class ==  CLASS_DEMON_BLADE )
	{
		{
			G_PlayEffect( G_EffectIndex( "necro_stone/altmuzzle_flash" ), ent->client->renderInfo.handRPoint );
		}
	}
	// NECROMANTIC STATES CRYSTAL RAY
	else if ( ent->client->NPC_class ==  CLASS_WEEQUAY ||
				ent->client->NPC_class ==  CLASS_BARTENDER ||
				ent->client->NPC_class ==  CLASS_GOLEM_ICE || 
				ent->client->NPC_class ==  CLASS_ICEREAVER || 
				ent->client->NPC_class ==  CLASS_CRYSTAL || 
				ent->client->NPC_class ==  CLASS_GALAKMECH  )
	{
		{
			G_PlayEffect( G_EffectIndex( "necro_crystal/altmuzzle_flash" ), ent->client->renderInfo.handRPoint );
		}
	}
	// NECROMANTIC CHAOS 
	else if ( ent->client->NPC_class ==  CLASS_PROPHET || 
				ent->client->NPC_class ==  CLASS_TAVION ||
				ent->client->NPC_class ==  CLASS_DESANN ||
				ent->client->NPC_class ==  CLASS_HYLDEN || 
				ent->client->NPC_class ==  CLASS_HYLDENLORD || 
				ent->client->NPC_class ==  CLASS_ELDERGOD || 
				ent->client->NPC_class ==  CLASS_TENTACLE || 
				ent->client->NPC_class ==  CLASS_ATST ||
				ent->client->NPC_class ==  CLASS_SAND_CREATURE ||
				ent->client->NPC_class ==  CLASS_MOUSE || 
				ent->client->NPC_class ==  CLASS_SENTRY )
{
		{
			G_PlayEffect( G_EffectIndex( "necro_chaos/altmuzzle_flash" ), ent->client->renderInfo.handRPoint );
		}
	}
	// NECROMANTIC VOID 
	else if ( ent->client->NPC_class ==  CLASS_SABOTEUR || 
				ent->client->NPC_class ==  CLASS_SHADOWTROOPER || 
				ent->client->NPC_class ==  CLASS_VOID || 
				ent->client->NPC_class ==  CLASS_GOLEM_VOID || 
				ent->client->NPC_class ==  CLASS_DARKREAVER )
	{
		{
			G_PlayEffect( G_EffectIndex( "necro_void/altmuzzle_flash" ), ent->client->renderInfo.handRPoint );
		}
	}
	// NECROMANTIC DEATH FIRE (FOR ALL WRAITH, REAPERS AND GHOSTS) 
	else if ( ent->client->NPC_class ==  CLASS_ARIEL || 
				ent->client->NPC_class ==  CLASS_GHOST || 
				ent->client->NPC_class ==  CLASS_REAPER || 
				ent->client->NPC_class ==  CLASS_DREADNOUGHT || 
				ent->client->NPC_class ==  CLASS_VAMPIRE_GHOST || 
				ent->client->NPC_class ==  CLASS_JEDI ||
				ent->client->NPC_class ==  CLASS_REAVER || 
				ent->client->NPC_class ==  CLASS_NECROREAVER || 
				ent->client->NPC_class ==  CLASS_THRALL || 
				ent->client->NPC_class ==  CLASS_NECRO || 
				ent->client->NPC_class ==  CLASS_GOLEM_NECRO || 
				ent->client->NPC_class ==  CLASS_JAWA ||
				ent->client->NPC_class ==  CLASS_MURJJ || 
				ent->client->NPC_class ==  CLASS_RANCOR || 
				ent->client->NPC_class ==  CLASS_PLAYER )
	{
		{
			G_PlayEffect( G_EffectIndex( "necro/altmuzzle_flash" ), ent->client->renderInfo.handRPoint );
		}
	}
	else if ( ent->client->NPC_class == CLASS_FIGHTER )
	{
		{
			G_PlayEffect( G_EffectIndex( "concussion0/altmuzzle_flash" ), ent->client->renderInfo.handRPoint );
		}
	}
	//G_PlayEffect( G_EffectIndex( "concussion/altmuzzle_flash" ), muzzle, forwardVec );
}

static void WP_FireConcussion( gentity_t *ent )
{//a fast rocket-like projectile
	vec3_t	start;
	int		damage	= weaponData[WP_CONCUSSION].damage;
	

	float	vel = CONC_VELOCITY;

	if (ent->s.number >= MAX_CLIENTS)
	{
		vec3_t angles;
		vectoangles(forwardVec, angles);
		angles[PITCH] += ( crandom() * (CONC_NPC_SPREAD+(6-ent->NPC->currentAim)*0.25f));//was 0.5f
		angles[YAW]	  += ( crandom() * (CONC_NPC_SPREAD+(6-ent->NPC->currentAim)*0.25f));//was 0.5f
		AngleVectors(angles, forwardVec, vrightVec, up);
	}

	//hold us still for a bit
	ent->client->ps.pm_time = 300;
	ent->client->ps.pm_flags |= PMF_TIME_KNOCKBACK;
	//add viewkick
	if ( ent->s.number < MAX_CLIENTS//player only
		&& !cg.renderingThirdPerson )//gives an advantage to being in 3rd person, but would look silly otherwise
	{//kick the view back
		cg.kick_angles[PITCH] = Q_flrand( -10, -15 );
		cg.kick_time = level.time;
	}

	VectorCopy( muzzle, start );
	WP_TraceSetStart( ent, start, vec3_origin, vec3_origin );//make sure our start point isn't on the other side of a wall

	gentity_t *missile = CreateMissile( start, forwardVec, vel, 10000, ent, qfalse );

	missile->classname = "conc_proj";
	missile->s.weapon = WP_CONCUSSION;
	missile->mass = 10;

	// Do the damages
	if ( ent->s.number != 0 )
	{
		if ( g_spskill->integer == 0 )
		{
			damage = CONC_NPC_DAMAGE_EASY;
		}
		else if ( g_spskill->integer == 1 )
		{
			damage = CONC_NPC_DAMAGE_NORMAL;
		}
		else
		{
			damage = CONC_NPC_DAMAGE_HARD;
		}
	}
	// DMG 
	if ( ent->client->NPC_class ==  CLASS_VAMPIRE_ZOMBIE || 
				ent->client->NPC_class ==  CLASS_TUSKEN ||
				ent->client->NPC_class ==  CLASS_MELC ||
				ent->client->NPC_class ==  CLASS_SUMMONER )
	{
		damage *= 1.5;
	}
	else if ( ent->client->NPC_class == CLASS_GLIDER )
	{
		damage *= 2;
	}
	// SPIRITH DEATH: All vampires classes Zephonim \ Alliance \ Dumahim 
	else if ( ent->client->NPC_class ==  CLASS_REBORN ||
				ent->client->NPC_class ==  CLASS_ALORA ||
				ent->client->NPC_class ==  CLASS_MONMOTHA ||
				ent->client->NPC_class ==  CLASS_NOGHRI ||
				ent->client->NPC_class ==  CLASS_FLIER2 || 
				ent->client->NPC_class ==  CLASS_ZEPH ||
				ent->client->NPC_class ==  CLASS_DECEIVER ||
				ent->client->NPC_class ==  CLASS_DUMAHIM || 
				ent->client->NPC_class ==  CLASS_VAMPIRE ||
				ent->client->NPC_class ==  CLASS_KAIN2 || 
				ent->client->NPC_class ==  CLASS_UMAH || 
				ent->client->NPC_class ==  CLASS_NECROMANCER || 
				ent->client->NPC_class ==  CLASS_VAMPIRE_QUEEN ||
				ent->client->NPC_class ==  CLASS_VAMPIRE_DARK || 
				ent->client->NPC_class ==  CLASS_VAMPIRE_NECRO || 
				ent->client->NPC_class ==  CLASS_VAMPIRE_NECRO2 ||
				ent->client->NPC_class ==  CLASS_VAMPIRE_FEAR || 
				ent->client->NPC_class ==  CLASS_VAMPIRE_SOUND || 
				ent->client->NPC_class ==  CLASS_VAMPIRE_LYCAN || 
				ent->client->NPC_class ==  CLASS_VAMPIRE_WATER || 
				ent->client->NPC_class ==  CLASS_VAMPIRE_ARCHER || 
				ent->client->NPC_class ==  CLASS_HALFVAMPIRE || 
				ent->client->NPC_class == CLASS_RAZIELHIM ||  
				ent->client->NPC_class == CLASS_ESKANDOR ||  
				ent->client->NPC_class ==  CLASS_VAMPIRE_BOUNTY || 
				ent->client->NPC_class ==  CLASS_VAMPIRE_SARAFAN )
	{
		damage *= 1;
	}
	// NECROMANTIC FIRE - RAHABIM   
	else if ( ent->client->NPC_class ==  CLASS_SWAMPTROOPER ||
				ent->client->NPC_class ==  CLASS_RAHABIM || 
				ent->client->NPC_class ==  CLASS_COMMANDO || 
				ent->client->NPC_class ==  CLASS_LYCAN || 
				ent->client->NPC_class ==  CLASS_CENOBITA || 
				ent->client->NPC_class ==  CLASS_WIZARD )
	{
		damage *= 1.6;
	}
	else if ( ent->client->NPC_class == CLASS_FISH )
	{
		damage *= 1.75;
	}
	// SD and IMMOLATION TOGETHER - KAIN 
	else if ( ent->client->NPC_class ==  CLASS_KYLE ||
				ent->client->NPC_class ==  CLASS_KAIN || 
				ent->client->NPC_class ==  CLASS_KAIN3 ||
				ent->client->NPC_class ==  CLASS_TURELIM || 
				ent->client->NPC_class ==  CLASS_VORADOR ||
				ent->client->NPC_class ==  CLASS_JANOS ||
				ent->client->NPC_class ==  CLASS_CLAW || 
				ent->client->NPC_class ==  CLASS_GOLEM_LAVA || 
				ent->client->NPC_class ==  CLASS_VAMPIRE_FEAR2 || 
				ent->client->NPC_class ==  CLASS_VAMPIRE_BLOOD || 
				ent->client->NPC_class ==  CLASS_VAMPIRE_DEMON  )
	{
		damage *= 1.5;
	}
	// THE HOLY IMMOLATION:  Humans, Saraphan and Druids. 
	else if ( ent->client->NPC_class ==  CLASS_HUNTER || 
				ent->client->NPC_class ==  CLASS_SCOUT ||  
				ent->client->NPC_class ==  CLASS_ALCHEMIST || 
				ent->client->NPC_class ==  CLASS_SARAFAN || 
				ent->client->NPC_class ==  CLASS_ARCHER ||  
				ent->client->NPC_class ==  CLASS_WARRIOR || 
				ent->client->NPC_class ==  CLASS_TRANDOSHAN ||
				ent->client->NPC_class ==  CLASS_STORMTROOPER ||
				ent->client->NPC_class ==  CLASS_PRISONER ||
				ent->client->NPC_class ==  CLASS_RODIAN ||
				ent->client->NPC_class ==  CLASS_IMPWORKER ||
				ent->client->NPC_class ==  CLASS_MARK1 ||
				ent->client->NPC_class ==  CLASS_MARK2 ||
				ent->client->NPC_class ==  CLASS_R2D2 ||
				ent->client->NPC_class ==  CLASS_R5D2 ||
				ent->client->NPC_class ==  CLASS_SEEKER ||
				ent->client->NPC_class ==  CLASS_REMOTE ||
				ent->client->NPC_class ==  CLASS_DRUIDREAVER || 
				ent->client->NPC_class ==  CLASS_RAZIEL || 
				ent->client->NPC_class ==  CLASS_HOLY || 
				ent->client->NPC_class ==  CLASS_GOLEM_HOLY )
	{
		damage *= 1.25;
	}
	// HUMAN BOSSES
	else if ( ent->client->NPC_class == CLASS_JAN || ent->client->NPC_class == CLASS_GALAK )
	{
		damage *= 1.5;
	}
	// THE POISON IMMOLATION 
	else if ( ent->client->NPC_class ==  CLASS_SPIDERREAVER3 || 
				ent->client->NPC_class ==  CLASS_BEAST || 
				ent->client->NPC_class ==  CLASS_MINEMONSTER ||
				ent->client->NPC_class ==  CLASS_REELO || 
				ent->client->NPC_class ==  CLASS_POISON || 
				ent->client->NPC_class ==  CLASS_DEMON_POISON || 
				ent->client->NPC_class ==  CLASS_GOLEM_POISON )
	{
		damage *= 1.5;
	}
	// THE NECROMANTIC FOG - MIND ATK 
	else if ( ent->client->NPC_class ==  CLASS_PROBE ||
				ent->client->NPC_class ==  CLASS_POLTER || 
				ent->client->NPC_class ==  CLASS_REBEL || 
				ent->client->NPC_class ==  CLASS_MENTALIST ||
				ent->client->NPC_class ==  CLASS_AIRREAVER || 
				ent->client->NPC_class ==  CLASS_AIR || 
				ent->client->NPC_class ==  CLASS_GOLEM_AIR || 
				ent->client->NPC_class ==  CLASS_BIRD )
	{
		damage *= 0.25;
	}
	// THE NECROMANTIC SMOKE ATK
	else if ( ent->client->NPC_class ==  CLASS_BESPIN_COP || 				
				ent->client->NPC_class ==  CLASS_DARK || 			 
				ent->client->NPC_class ==  CLASS_GOLEM_DARK || 
				ent->client->NPC_class ==  CLASS_TANKREAVER || 
				ent->client->NPC_class ==  CLASS_GOLEM_SMOKE )
	{
		damage *= 0.5;
	}
	// THE NECROMANTIC ASTRAL ATK - Dimension  - Secondary Atk: Sonic ray 
	else if ( ent->client->NPC_class ==  CLASS_HOWLER ||
				ent->client->NPC_class ==  CLASS_SONIC || 
				ent->client->NPC_class ==  CLASS_GOLEM_SONIC || 
				ent->client->NPC_class ==  CLASS_LANDO )
	{
		damage *= 1.75;
	}
	// THE NECROMANTIC FULL IMMOLATE ATTACK - MAGMA GUARDIAN \ TURELIM  
	else if ( ent->client->NPC_class ==  CLASS_FIREREAVER || 
				ent->client->NPC_class ==  CLASS_FIRE || 
				ent->client->NPC_class ==  CLASS_GOLEM_FIRE || 
				ent->client->NPC_class ==  CLASS_HELLGOLEM || 
				ent->client->NPC_class ==  CLASS_HAZARD_TROOPER ||
				ent->client->NPC_class ==  CLASS_GRAN ||
				ent->client->NPC_class ==  CLASS_VAMPIRE_FROST || 
				ent->client->NPC_class ==  CLASS_BOBAFETT || 
				ent->client->NPC_class ==  CLASS_WAMPA || 
				ent->client->NPC_class ==  CLASS_DEMON_FIRE )
	{
		damage *= 2;
	}
	// THE NECROMANTIC MUD ATK -   
	else if ( ent->client->NPC_class ==  CLASS_SWAMP ||
				ent->client->NPC_class ==  CLASS_INTERROGATOR ||
				ent->client->NPC_class ==  CLASS_SHARK || 
				ent->client->NPC_class ==  CLASS_WATER || 
				ent->client->NPC_class ==  CLASS_GOLEM_WATER || 
				ent->client->NPC_class ==  CLASS_UGNAUGHT || 
				ent->client->NPC_class ==  CLASS_SPIDERREAVER2 || 
				ent->client->NPC_class ==  CLASS_SPIDERREAVER )
	{
		damage *= 1.1;
	}
	// THE NECROMANTIC NATURE ATK - Dimension  
	else if ( ent->client->NPC_class ==  CLASS_LUKE || 
				ent->client->NPC_class ==  CLASS_IVY || 
				ent->client->NPC_class ==  CLASS_DRUID )
	{
		damage *= 2;
	}
	// NECROMANTIC SHOCK ATK - Lightning forge 
	else if ( ent->client->NPC_class ==  CLASS_LIGHTREAVER || 
				ent->client->NPC_class ==  CLASS_GOLEM_METAL || 
				ent->client->NPC_class ==  CLASS_DEMON_ELECTRO || 
				ent->client->NPC_class ==  CLASS_ASSASSIN_DROID || 
				ent->client->NPC_class ==  CLASS_SABER_DROID )
	{
		damage *= 1.5;
	}
	// NECROMANTIC Sun  ATK - Sunlight forge 
	else if ( ent->client->NPC_class ==  CLASS_LIGHT || 
				ent->client->NPC_class ==  CLASS_GOLEM_LIGHT || 
				ent->client->NPC_class ==  CLASS_GOLEM_GOLD || 
				ent->client->NPC_class ==  CLASS_MORGANKATARN ||
				ent->client->NPC_class ==  CLASS_PROTOCOL )
	{
		damage *= 2;
	}
	// NECROMANTIC TIME ATK - Storm Forge - Primary shoot: sandstorm devil. secondary: time slam 
	else if ( ent->client->NPC_class == CLASS_ROCKETTROOPER ||
		ent->client->NPC_class == CLASS_VAMPIRE_ELF3 )
	{
		damage *= 1.5;
	}
	// NECROMANTIC STONE ATK 
	else if ( ent->client->NPC_class ==  CLASS_GONK ||
				ent->client->NPC_class ==  CLASS_LIZARD ||
				ent->client->NPC_class ==  CLASS_VAMPIRE_ELF || 
				ent->client->NPC_class ==  CLASS_VAMPIRE_ELF2 || 
				ent->client->NPC_class ==  CLASS_EARTHREAVER || 
				ent->client->NPC_class ==  CLASS_EARTH || 
				ent->client->NPC_class ==  CLASS_GOLEM || 
				ent->client->NPC_class ==  CLASS_DEMON_BLADE )
	{
		damage *= 2;
	}
	// NECROMANTIC STATES CRYSTAL RAY
	else if ( ent->client->NPC_class ==  CLASS_WEEQUAY ||
				ent->client->NPC_class ==  CLASS_BARTENDER ||
				ent->client->NPC_class ==  CLASS_GOLEM_ICE || 
				ent->client->NPC_class ==  CLASS_ICEREAVER || 
				ent->client->NPC_class ==  CLASS_CRYSTAL || 
				ent->client->NPC_class ==  CLASS_GALAKMECH  )
	{
		damage *= 1.5;
	}
	// NECROMANTIC CHAOS 
	else if ( ent->client->NPC_class ==  CLASS_PROPHET || 
				ent->client->NPC_class ==  CLASS_TAVION ||
				ent->client->NPC_class ==  CLASS_DESANN ||
				ent->client->NPC_class ==  CLASS_HYLDEN || 
				ent->client->NPC_class ==  CLASS_HYLDENLORD || 
				ent->client->NPC_class ==  CLASS_ELDERGOD || 
				ent->client->NPC_class ==  CLASS_TENTACLE || 
				ent->client->NPC_class ==  CLASS_ATST ||
				ent->client->NPC_class ==  CLASS_SAND_CREATURE ||
				ent->client->NPC_class ==  CLASS_MOUSE || 
				ent->client->NPC_class ==  CLASS_SENTRY )
	{
		damage *= 4;
	}
	// NECROMANTIC VOID 
	else if ( ent->client->NPC_class ==  CLASS_SABOTEUR || 
				ent->client->NPC_class ==  CLASS_SHADOWTROOPER || 
				ent->client->NPC_class ==  CLASS_VOID || 
				ent->client->NPC_class ==  CLASS_GOLEM_VOID || 
				ent->client->NPC_class ==  CLASS_DARKREAVER )
	{
		damage *= 2;
	}
	// NECROMANTIC DEATH FIRE (FOR ALL WRAITH, REAPERS AND GHOSTS) 
	else if ( ent->client->NPC_class ==  CLASS_ARIEL || 
				ent->client->NPC_class ==  CLASS_GHOST || 
				ent->client->NPC_class ==  CLASS_REAPER || 
				ent->client->NPC_class ==  CLASS_DREADNOUGHT || 
				ent->client->NPC_class ==  CLASS_VAMPIRE_GHOST || 
				ent->client->NPC_class ==  CLASS_JEDI ||
				ent->client->NPC_class ==  CLASS_REAVER || 
				ent->client->NPC_class ==  CLASS_NECROREAVER || 
				ent->client->NPC_class ==  CLASS_THRALL || 
				ent->client->NPC_class ==  CLASS_NECRO || 
				ent->client->NPC_class ==  CLASS_GOLEM_NECRO || 
				ent->client->NPC_class ==  CLASS_JAWA ||
				ent->client->NPC_class ==  CLASS_MURJJ || 
				ent->client->NPC_class ==  CLASS_RANCOR || 
				ent->client->NPC_class ==  CLASS_PLAYER )
	{
		damage *= 1;
	}
	/*if ( ent->client->NPC_class == CLASS_TUSKEN
		|| ent->client->NPC_class == CLASS_COMMANDO
		)
	{
		damage *= 1.5; // Elementali del Nebbia fanno 50% di danno in più!
	}
	else if ( ent->client->NPC_class == CLASS_GLIDER )
	{
		damage *= 2; // Elementali della Tempesta fanno 50% di danno doppio!!
	}
	else if ( ent->client->NPC_class == CLASS_MURJJ )
	{
		damage *= 3; // Elementali della Tempesta fanno 50% di danno doppio!!
	}*/

	// Make it easier to hit things
	VectorSet( missile->maxs, ROCKET_SIZE, ROCKET_SIZE, ROCKET_SIZE );
	VectorScale( missile->maxs, -1, missile->mins );

	missile->damage = damage;
	missile->dflags = DAMAGE_EXTRA_KNOCKBACK;

	missile->methodOfDeath = MOD_CONC;
	missile->splashMethodOfDeath = MOD_CONC;

	missile->clipmask = MASK_SHOT | CONTENTS_LIGHTSABER;
	missile->splashDamage = weaponData[WP_CONCUSSION].splashDamage;
	missile->splashRadius = weaponData[WP_CONCUSSION].splashRadius;

	// we don't want it to ever bounce
	missile->bounceCount = 0;

	if ( ent->client->NPC_class ==  CLASS_VAMPIRE_ZOMBIE || 
				ent->client->NPC_class ==  CLASS_GLIDER ||
				ent->client->NPC_class ==  CLASS_TUSKEN ||
				ent->client->NPC_class ==  CLASS_MELC ||
				ent->client->NPC_class ==  CLASS_SUMMONER )
	{
		{
			G_PlayEffect( G_EffectIndex( "concussion/muzzle_flash" ), ent->client->renderInfo.handRPoint );
		}
	}
// SPIRITH DEATH: All vampires classes Zephonim \ Alliance \ Dumahim 
	else if ( ent->client->NPC_class ==  CLASS_REBORN ||
				ent->client->NPC_class ==  CLASS_ALORA ||
				ent->client->NPC_class ==  CLASS_MONMOTHA ||
				ent->client->NPC_class ==  CLASS_NOGHRI ||
				ent->client->NPC_class ==  CLASS_FLIER2 || 
				ent->client->NPC_class ==  CLASS_ZEPH ||
				ent->client->NPC_class ==  CLASS_DECEIVER ||
				ent->client->NPC_class ==  CLASS_DUMAHIM || 
				ent->client->NPC_class ==  CLASS_VAMPIRE ||
				ent->client->NPC_class ==  CLASS_KAIN2 || 
				ent->client->NPC_class ==  CLASS_UMAH || 
				ent->client->NPC_class ==  CLASS_NECROMANCER || 
				ent->client->NPC_class ==  CLASS_VAMPIRE_QUEEN ||
				ent->client->NPC_class ==  CLASS_VAMPIRE_DARK || 
				ent->client->NPC_class ==  CLASS_VAMPIRE_NECRO || 
				ent->client->NPC_class ==  CLASS_VAMPIRE_NECRO2 ||
				ent->client->NPC_class ==  CLASS_VAMPIRE_FEAR || 
				ent->client->NPC_class ==  CLASS_VAMPIRE_SOUND || 
				ent->client->NPC_class ==  CLASS_VAMPIRE_LYCAN || 
				ent->client->NPC_class ==  CLASS_VAMPIRE_WATER || 
				ent->client->NPC_class ==  CLASS_VAMPIRE_ARCHER || 
				ent->client->NPC_class ==  CLASS_HALFVAMPIRE || 
				ent->client->NPC_class == CLASS_RAZIELHIM ||  
				ent->client->NPC_class == CLASS_ESKANDOR ||  
				ent->client->NPC_class ==  CLASS_VAMPIRE_BOUNTY || 
				ent->client->NPC_class ==  CLASS_VAMPIRE_SARAFAN )
	{
		{
			G_PlayEffect( G_EffectIndex( "spirith_death/muzzle_flash" ), ent->client->renderInfo.handRPoint );
		}
	}
	// NECROMANTIC FIRE - RAHABIM   
	else if ( ent->client->NPC_class ==  CLASS_SWAMPTROOPER ||
				ent->client->NPC_class ==  CLASS_RAHABIM || 
				ent->client->NPC_class ==  CLASS_COMMANDO || 
				ent->client->NPC_class ==  CLASS_LYCAN || 
				ent->client->NPC_class ==  CLASS_CENOBITA || 
				ent->client->NPC_class ==  CLASS_WIZARD ||
				ent->client->NPC_class == CLASS_FISH )
	{
		{
			G_PlayEffect( G_EffectIndex( "blackfire/muzzle_flash" ), ent->client->renderInfo.handRPoint );
		}
	}
	// SD and IMMOLATION TOGETHER - KAIN 
	else if ( ent->client->NPC_class ==  CLASS_KYLE ||
				ent->client->NPC_class ==  CLASS_KAIN || 
				ent->client->NPC_class ==  CLASS_KAIN3 ||
				ent->client->NPC_class ==  CLASS_TURELIM || 
				ent->client->NPC_class ==  CLASS_VORADOR ||
				ent->client->NPC_class ==  CLASS_JANOS ||
				ent->client->NPC_class ==  CLASS_CLAW || 
				ent->client->NPC_class ==  CLASS_GOLEM_LAVA || 
				ent->client->NPC_class ==  CLASS_VAMPIRE_FEAR2 || 
				ent->client->NPC_class ==  CLASS_VAMPIRE_BLOOD || 
				ent->client->NPC_class ==  CLASS_VAMPIRE_DEMON  )
	{
		{
			G_PlayEffect( G_EffectIndex( "immolate/muzzle_flash" ), ent->client->renderInfo.handRPoint );
		}
	}
	// THE HOLY IMMOLATION:  Humans, Saraphan and Druids. 
	else if ( ent->client->NPC_class ==  CLASS_HUNTER || 
				ent->client->NPC_class ==  CLASS_SCOUT ||  
				ent->client->NPC_class ==  CLASS_ALCHEMIST || 
				ent->client->NPC_class ==  CLASS_SARAFAN || 
				ent->client->NPC_class ==  CLASS_ARCHER ||  
				ent->client->NPC_class ==  CLASS_WARRIOR || 
				ent->client->NPC_class ==  CLASS_TRANDOSHAN ||
				ent->client->NPC_class ==  CLASS_STORMTROOPER ||
				ent->client->NPC_class ==  CLASS_PRISONER ||
				ent->client->NPC_class ==  CLASS_RODIAN ||
				ent->client->NPC_class ==  CLASS_IMPWORKER ||
				ent->client->NPC_class ==  CLASS_MARK1 ||
				ent->client->NPC_class ==  CLASS_MARK2 ||
				ent->client->NPC_class ==  CLASS_R2D2 ||
				ent->client->NPC_class ==  CLASS_R5D2 ||
				ent->client->NPC_class ==  CLASS_SEEKER ||
				ent->client->NPC_class ==  CLASS_REMOTE ||
				ent->client->NPC_class ==  CLASS_DRUIDREAVER || 
				ent->client->NPC_class ==  CLASS_RAZIEL || 
				ent->client->NPC_class ==  CLASS_HOLY || 
				ent->client->NPC_class ==  CLASS_GOLEM_HOLY ||
				ent->client->NPC_class == CLASS_JAN || 
				ent->client->NPC_class == CLASS_GALAK )
	{
		{
			G_PlayEffect( G_EffectIndex( "holyimmolate/muzzle_flash" ), ent->client->renderInfo.handRPoint );
		}
	}
	// THE POISON IMMOLATION 
	else if ( ent->client->NPC_class ==  CLASS_SPIDERREAVER3 || 
				ent->client->NPC_class ==  CLASS_BEAST || 
				ent->client->NPC_class ==  CLASS_MINEMONSTER ||
				ent->client->NPC_class ==  CLASS_REELO || 
				ent->client->NPC_class ==  CLASS_POISON || 
				ent->client->NPC_class ==  CLASS_DEMON_POISON || 
				ent->client->NPC_class ==  CLASS_GOLEM_POISON )
	{
		{
			G_PlayEffect( G_EffectIndex( "necro_poison/muzzle_flash" ), ent->client->renderInfo.handRPoint );
		}
	}
	// THE NECROMANTIC FOG - MIND ATK 
	else if ( ent->client->NPC_class ==  CLASS_PROBE ||
				ent->client->NPC_class ==  CLASS_POLTER || 
				ent->client->NPC_class ==  CLASS_REBEL || 
				ent->client->NPC_class ==  CLASS_MENTALIST ||
				ent->client->NPC_class ==  CLASS_AIRREAVER || 
				ent->client->NPC_class ==  CLASS_AIR || 
				ent->client->NPC_class ==  CLASS_GOLEM_AIR || 
				ent->client->NPC_class ==  CLASS_BIRD )
	{
		{
			G_PlayEffect( G_EffectIndex( "necro_fog/muzzle_flash" ), ent->client->renderInfo.handRPoint );
		}
	}
	// THE NECROMANTIC SMOKE ATK
	else if ( ent->client->NPC_class ==  CLASS_BESPIN_COP || 				
				ent->client->NPC_class ==  CLASS_DARK || 			 
				ent->client->NPC_class ==  CLASS_GOLEM_DARK || 
				ent->client->NPC_class ==  CLASS_TANKREAVER || 
				ent->client->NPC_class ==  CLASS_GOLEM_SMOKE )
	{
		{
			G_PlayEffect( G_EffectIndex( "necro_smog/muzzle_flash" ), ent->client->renderInfo.handRPoint );
		}
	}
	// THE NECROMANTIC ASTRAL ATK - Dimension  - Secondary Atk: Sonic ray 
	else if ( ent->client->NPC_class ==  CLASS_HOWLER ||
				ent->client->NPC_class ==  CLASS_SONIC || 
				ent->client->NPC_class ==  CLASS_GOLEM_SONIC || 
				ent->client->NPC_class ==  CLASS_LANDO )
	{
		{
			G_PlayEffect( G_EffectIndex( "necro_dim/muzzle_flash" ), ent->client->renderInfo.handRPoint );
		}
	}
	// THE NECROMANTIC FULL IMMOLATE ATTACK - MAGMA GUARDIAN \ TURELIM  
	else if ( ent->client->NPC_class ==  CLASS_FIREREAVER || 
				ent->client->NPC_class ==  CLASS_FIRE || 
				ent->client->NPC_class ==  CLASS_GOLEM_FIRE || 
				ent->client->NPC_class ==  CLASS_HELLGOLEM || 
				ent->client->NPC_class ==  CLASS_HAZARD_TROOPER ||
				ent->client->NPC_class ==  CLASS_GRAN ||
				ent->client->NPC_class ==  CLASS_VAMPIRE_FROST || 
				ent->client->NPC_class ==  CLASS_BOBAFETT || 
				ent->client->NPC_class ==  CLASS_WAMPA || 
				ent->client->NPC_class ==  CLASS_DEMON_FIRE )
	{
		{
			G_PlayEffect( G_EffectIndex( "necro_conflict/muzzle_flash" ), ent->client->renderInfo.handRPoint );
		}
	}
	// THE NECROMANTIC MUD ATK -   
	else if ( ent->client->NPC_class ==  CLASS_SWAMP ||
				ent->client->NPC_class ==  CLASS_INTERROGATOR ||
				ent->client->NPC_class ==  CLASS_SHARK || 
				ent->client->NPC_class ==  CLASS_WATER || 
				ent->client->NPC_class ==  CLASS_GOLEM_WATER || 
				ent->client->NPC_class ==  CLASS_UGNAUGHT || 
				ent->client->NPC_class ==  CLASS_SPIDERREAVER2 || 
				ent->client->NPC_class ==  CLASS_SPIDERREAVER )
	{
		{
			G_PlayEffect( G_EffectIndex( "necro_mud/muzzle_flash" ), ent->client->renderInfo.handRPoint );
		}
	}
	// THE NECROMANTIC NATURE ATK - Dimension  
	else if ( ent->client->NPC_class ==  CLASS_LUKE || 
				ent->client->NPC_class ==  CLASS_IVY || 
				ent->client->NPC_class ==  CLASS_DRUID )
	{
		{
			G_PlayEffect( G_EffectIndex( "necro_nature/muzzle_flash" ), ent->client->renderInfo.handRPoint );
		}
	}
	// NECROMANTIC SHOCK ATK - Lightning forge 
	else if ( ent->client->NPC_class ==  CLASS_LIGHTREAVER || 
				ent->client->NPC_class ==  CLASS_GOLEM_METAL || 
				ent->client->NPC_class ==  CLASS_DEMON_ELECTRO || 
				ent->client->NPC_class ==  CLASS_ASSASSIN_DROID || 
				ent->client->NPC_class ==  CLASS_SABER_DROID )
	{
		{
			G_PlayEffect( G_EffectIndex( "necro_shock/muzzle_flash" ), ent->client->renderInfo.handRPoint );
		}
	}
	// NECROMANTIC Sun  ATK - Sunlight forge 
	else if ( ent->client->NPC_class ==  CLASS_LIGHT || 
				ent->client->NPC_class ==  CLASS_GOLEM_LIGHT || 
				ent->client->NPC_class ==  CLASS_GOLEM_GOLD || 
				ent->client->NPC_class ==  CLASS_MORGANKATARN ||
				ent->client->NPC_class ==  CLASS_PROTOCOL )
	{
		{
			G_PlayEffect( G_EffectIndex( "necro_energy/muzzle_flash" ), ent->client->renderInfo.handRPoint );
		}
	}
	// NECROMANTIC TIME ATK - Storm Forge - Primary shoot: sandstorm devil. secondary: time slam 
	else if ( ent->client->NPC_class == CLASS_ROCKETTROOPER ||
		ent->client->NPC_class == CLASS_VAMPIRE_ELF3 )
{
		{
			G_PlayEffect( G_EffectIndex( "necro_time/muzzle_flash" ), ent->client->renderInfo.handRPoint );
		}
	}
	// NECROMANTIC STONE ATK 
	else if ( ent->client->NPC_class ==  CLASS_GONK ||
				ent->client->NPC_class ==  CLASS_LIZARD ||
				ent->client->NPC_class ==  CLASS_VAMPIRE_ELF || 
				ent->client->NPC_class ==  CLASS_VAMPIRE_ELF2 || 
				ent->client->NPC_class ==  CLASS_EARTHREAVER || 
				ent->client->NPC_class ==  CLASS_EARTH || 
				ent->client->NPC_class ==  CLASS_GOLEM || 
				ent->client->NPC_class ==  CLASS_DEMON_BLADE )
	{
		{
			G_PlayEffect( G_EffectIndex( "necro_stone/muzzle_flash" ), ent->client->renderInfo.handRPoint );
		}
	}
	// NECROMANTIC STATES CRYSTAL RAY
	else if ( ent->client->NPC_class ==  CLASS_WEEQUAY ||
				ent->client->NPC_class ==  CLASS_BARTENDER ||
				ent->client->NPC_class ==  CLASS_GOLEM_ICE || 
				ent->client->NPC_class ==  CLASS_ICEREAVER || 
				ent->client->NPC_class ==  CLASS_CRYSTAL || 
				ent->client->NPC_class ==  CLASS_GALAKMECH  )
	{
		{
			G_PlayEffect( G_EffectIndex( "necro_crystal/muzzle_flash" ), ent->client->renderInfo.handRPoint );
		}
	}
	// NECROMANTIC CHAOS 
	else if ( ent->client->NPC_class ==  CLASS_PROPHET || 
				ent->client->NPC_class ==  CLASS_TAVION ||
				ent->client->NPC_class ==  CLASS_DESANN ||
				ent->client->NPC_class ==  CLASS_HYLDEN || 
				ent->client->NPC_class ==  CLASS_HYLDENLORD || 
				ent->client->NPC_class ==  CLASS_ELDERGOD || 
				ent->client->NPC_class ==  CLASS_TENTACLE || 
				ent->client->NPC_class ==  CLASS_ATST ||
				ent->client->NPC_class ==  CLASS_SAND_CREATURE ||
				ent->client->NPC_class ==  CLASS_MOUSE || 
				ent->client->NPC_class ==  CLASS_SENTRY )
{
		{
			G_PlayEffect( G_EffectIndex( "necro_chaos/muzzle_flash" ), ent->client->renderInfo.handRPoint );
		}
	}
	// NECROMANTIC VOID 
	else if ( ent->client->NPC_class ==  CLASS_SABOTEUR || 
				ent->client->NPC_class ==  CLASS_SHADOWTROOPER || 
				ent->client->NPC_class ==  CLASS_VOID || 
				ent->client->NPC_class ==  CLASS_GOLEM_VOID || 
				ent->client->NPC_class ==  CLASS_DARKREAVER )
	{
		{
			G_PlayEffect( G_EffectIndex( "necro_void/muzzle_flash" ), ent->client->renderInfo.handRPoint );
		}
	}
	// NECROMANTIC DEATH FIRE (FOR ALL WRAITH, REAPERS AND GHOSTS) 
	else if ( ent->client->NPC_class ==  CLASS_ARIEL || 
				ent->client->NPC_class ==  CLASS_GHOST || 
				ent->client->NPC_class ==  CLASS_REAPER || 
				ent->client->NPC_class ==  CLASS_DREADNOUGHT || 
				ent->client->NPC_class ==  CLASS_VAMPIRE_GHOST || 
				ent->client->NPC_class ==  CLASS_JEDI ||
				ent->client->NPC_class ==  CLASS_REAVER || 
				ent->client->NPC_class ==  CLASS_NECROREAVER || 
				ent->client->NPC_class ==  CLASS_THRALL || 
				ent->client->NPC_class ==  CLASS_NECRO || 
				ent->client->NPC_class ==  CLASS_GOLEM_NECRO || 
				ent->client->NPC_class ==  CLASS_JAWA ||
				ent->client->NPC_class ==  CLASS_MURJJ || 
				ent->client->NPC_class ==  CLASS_RANCOR || 
				ent->client->NPC_class ==  CLASS_PLAYER )
	{
		{
			G_PlayEffect( G_EffectIndex( "necro/muzzle_flash" ), ent->client->renderInfo.handRPoint );
		}
	}
	else if ( ent->client->NPC_class == CLASS_FIGHTER )
	{
		{
			G_PlayEffect( G_EffectIndex( "concussion0/muzzle_flash" ), ent->client->renderInfo.handRPoint );
		}
	}
}

void WP_Concussion( gentity_t *ent, qboolean alt_fire )
{
	if(alt_fire)
	{
		WP_FireConcussionAlt(ent);
	}
	else
	{
		WP_FireConcussion(ent);
	}





}

