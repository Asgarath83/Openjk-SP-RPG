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

// leave this line at the top for all g_xxxx.cpp files...
#include "g_headers.h"



#include "Q3_Interface.h"

#include "g_local.h"
#include "wp_saber.h"

extern void G_NextTestAxes( void );
extern void G_ChangePlayerModel( gentity_t *ent, const char *newModel );
extern void G_InitPlayerFromCvars( gentity_t *ent );
extern void Q3_SetViewEntity(int entID, const char *name);
extern qboolean G_ClearViewEntity( gentity_t *ent );
extern void G_Knockdown( gentity_t *self, gentity_t *attacker, const vec3_t pushDir, float strength, qboolean breakSaberLock );

extern void WP_SetSaber( gentity_t *ent, int saberNum, const char *saberName );
extern void WP_RemoveSaber( gentity_t *ent, int saberNum );
extern saber_colors_t TranslateSaberColor( const char *name );
extern qboolean WP_SaberBladeUseSecondBladeStyle( saberInfo_t *saber, int bladeNum );
extern qboolean WP_UseFirstValidSaberStyle( gentity_t *ent, int *saberAnimLevel );
extern void WP_SetGun( gentity_t *ent, int GunNum, const char *GunName );
extern void WP_RemoveGun( gentity_t *ent, int GunNum );
extern void G_SetWeapon( gentity_t *self, int wp );
extern stringID_table_t WPTable[];

extern cvar_t	*g_char_model;
extern cvar_t	*g_char_skin_head;
extern cvar_t	*g_char_skin_torso;
extern cvar_t	*g_char_skin_legs;
extern cvar_t	*g_char_color_red;
extern cvar_t	*g_char_color_green;
extern cvar_t	*g_char_color_blue;
extern cvar_t	*g_saber;
extern cvar_t	*g_saber2;
extern cvar_t	*g_saber_color;
extern cvar_t	*g_saber2_color;
extern cvar_t	*g_gun;
extern cvar_t	*g_gun2;
/*
===================
Svcmd_EntityList_f
===================
*/
void	Svcmd_EntityList_f (void) {
	int			e;
	gentity_t		*check;

	check = g_entities+1;
	for (e = 1; e < globals.num_entities ; e++, check++) {
		if ( !check->inuse ) {
			continue;
		}
		gi.Printf("%3i:", e);
		switch ( check->s.eType ) {
		case ET_GENERAL:
			gi.Printf("ET_GENERAL ");
			break;
		case ET_PLAYER:
			gi.Printf("ET_PLAYER  ");
			break;
		case ET_ITEM:
			gi.Printf("ET_ITEM    ");
			break;
		case ET_MISSILE:
			gi.Printf("ET_MISSILE ");
			break;
		case ET_MOVER:
			gi.Printf("ET_MOVER   ");
			break;
		case ET_BEAM:
			gi.Printf("ET_BEAM    ");
			break;
		default:
			gi.Printf("#%i", check->s.eType);
			break;
		}

		if ( check->classname ) {
			gi.Printf("%s", check->classname);
		}
		gi.Printf("\n");
	}
}

gclient_t	*ClientForString( const char *s ) {
	gclient_t	*cl;
	int			i;
	int			idnum;

	// numeric values are just slot numbers
	if ( s[0] >= '0' && s[0] <= '9' ) {
		idnum = atoi( s );
		if ( idnum < 0 || idnum >= level.maxclients ) {
			Com_Printf( "Bad client slot: %i\n", idnum );
			return NULL;
		}

		cl = &level.clients[idnum];
		if ( cl->pers.connected == CON_DISCONNECTED ) {
			gi.Printf( "Client %i is not connected\n", idnum );
			return NULL;
		}
		return cl;
	}

	// check for a name match
	for ( i=0 ; i < level.maxclients ; i++ ) {
		cl = &level.clients[i];
		if ( cl->pers.connected == CON_DISCONNECTED ) {
			continue;
		}
		if ( !Q_stricmp( cl->pers.netname, s ) ) {
			return cl;
		}
	}

	gi.Printf( "User %s is not on the server\n", s );

	return NULL;
}

//---------------------------
extern void G_StopCinematicSkip( void );
extern void G_StartCinematicSkip( void );
extern void ExitEmplacedWeapon( gentity_t *ent );
static void Svcmd_ExitView_f( void )
{
extern cvar_t	*g_skippingcin;
	static int exitViewDebounce = 0;
	if ( exitViewDebounce > level.time )
	{
		return;
	}
	exitViewDebounce = level.time + 500;
	if ( in_camera )
	{//see if we need to exit an in-game cinematic
		if ( g_skippingcin->integer )	// already doing cinematic skip?
		{// yes...   so stop skipping...
			G_StopCinematicSkip();
		}
		else
		{// no... so start skipping...
			G_StartCinematicSkip();
		}
	}
	else if ( !G_ClearViewEntity( player ) )
	{//didn't exit control of a droid or turret
		//okay, now try exiting emplaced guns or AT-ST's
		if ( player->s.eFlags & EF_LOCKED_TO_WEAPON )
		{//get out of emplaced gun
			ExitEmplacedWeapon( player );
		}
		else if ( player->client && player->client->NPC_class == CLASS_ATST )
		{//a player trying to get out of his ATST
			GEntity_UseFunc( player->activator, player, player );
		}
	}
}

gentity_t *G_GetSelfForPlayerCmd( void )
{
	if ( g_entities[0].client->ps.viewEntity > 0 
		&& g_entities[0].client->ps.viewEntity < ENTITYNUM_WORLD 
		&& g_entities[g_entities[0].client->ps.viewEntity].client 
		&& g_entities[g_entities[0].client->ps.viewEntity].s.weapon == WP_SABER )
	{//you're controlling another NPC
		return (&g_entities[g_entities[0].client->ps.viewEntity]);
	}
	else
	{
		return (&g_entities[0]);
	}
}





// SET PLAYER LEVEL
void Svcmd_SetPlayerLevel_f ( void )
{
	if ( !&g_entities[0] || !g_entities[0].client )
	{
		return;
	}
	if ( !g_cheats->integer ) 
	{
		gi.SendServerCommand( 0, "print \"Cheats are not enabled on this server.\n\"");
		return;
	}
	const char *newVal = gi.argv(1);
	if ( !VALIDSTRING( newVal ) )
	{
//		gi.Printf( "Current Playerlevel is %d\n", g_entities[0].client->ls.PlayerLevel[NUM_LEVELS] );
		//gi.Printf( "Usage:  PlayerLevel <level> (1 - 100)\n" );
		return;
	}
	int val = atoi(newVal);
	if ( val > LEVEL_NONE )
	{
		//g_entities[0].client->ls.PlayerLevel != LEVEL_NONE );
	}
	
	g_entities[0].client->ls.PlayerLevel[NUM_LEVELS] = val;
	if ( g_entities[0].client->ls.PlayerLevel[NUM_LEVELS] < LEVEL_NONE )
	{
		g_entities[0].client->ls.PlayerLevel[NUM_LEVELS] = LEVEL_NONE;
	}
	else if ( g_entities[0].client->ls.PlayerLevel[NUM_LEVELS] > LEVEL120 )
	{
		g_entities[0].client->ls.PlayerLevel[NUM_LEVELS] = LEVEL120;
	}
	// NOW CAP AND SET THE PLAYER HEALTH in relation of the amount of level. 
	
	int		max;
	const char	*cmd;
	cmd = gi.argv(1);
	if ( cmd && atoi( cmd ) )
	{
		max = atoi( cmd );
	}
	else
	{
		//max = 999;
		max = 100;
	}
	g_entities[0].health = g_entities[0].max_health = max;
	switch ( g_entities[0].client->ls.PlayerLevel[NUM_LEVELS] )
	{
		
		case LEVEL_NONE:  
		{
			g_entities[0].health = g_entities[0].max_health = 100;
			g_entities[0].client->ps.stats[STAT_HEALTH] = g_entities[0].client->ps.stats[STAT_MAX_HEALTH] = 100;
			g_entities[0].client->ps.forcePowerMax = 100; 
			g_entities[0].client->ps.forcePowerRegenRate = 150; 
			g_entities[0].client->ps.forcePowerRegenAmount = 1; 
		}
		break; 
		case LEVEL1:  
		{
			g_entities[0].health = g_entities[0].max_health = 110;
			g_entities[0].client->ps.stats[STAT_HEALTH] = g_entities[0].client->ps.stats[STAT_MAX_HEALTH] = 110;
			g_entities[0].client->ps.forcePowerMax = 102; 
			g_entities[0].client->ps.forcePowerRegenRate = 150; 
			g_entities[0].client->ps.forcePowerRegenAmount = 1; 
		}
		break; 
		case LEVEL2:  
		{
			g_entities[0].health = g_entities[0].max_health = 120;
			g_entities[0].client->ps.stats[STAT_HEALTH] = g_entities[0].client->ps.stats[STAT_MAX_HEALTH] = 120;
			g_entities[0].client->ps.forcePowerMax = 104; 
			g_entities[0].client->ps.forcePowerRegenRate = 150; 
			g_entities[0].client->ps.forcePowerRegenAmount = 1; 
		}
		break; 
		case LEVEL3:  
		{
			g_entities[0].health = g_entities[0].max_health = 130;
			g_entities[0].client->ps.stats[STAT_HEALTH] = g_entities[0].client->ps.stats[STAT_MAX_HEALTH] = 130;
			g_entities[0].client->ps.forcePowerMax = 106; 
			g_entities[0].client->ps.forcePowerRegenRate = 150; 
			g_entities[0].client->ps.forcePowerRegenAmount = 1; 
		}
		break; 
		case LEVEL4:  
		{
			g_entities[0].health = g_entities[0].max_health = 140;
			g_entities[0].client->ps.stats[STAT_HEALTH] = g_entities[0].client->ps.stats[STAT_MAX_HEALTH] = 140;
			g_entities[0].client->ps.forcePowerMax = 108; 
			g_entities[0].client->ps.forcePowerRegenRate = 150; 
			g_entities[0].client->ps.forcePowerRegenAmount = 1; 
		}
		break; 
		case LEVEL5:  
		{
			g_entities[0].health = g_entities[0].max_health = 150;
			g_entities[0].client->ps.stats[STAT_HEALTH] = g_entities[0].client->ps.stats[STAT_MAX_HEALTH] = 150;
			g_entities[0].client->ps.forcePowerMax = 110; 
			g_entities[0].client->ps.forcePowerRegenRate = 100; 
			g_entities[0].client->ps.forcePowerRegenAmount = 1; 
		}
		break; 
		case LEVEL6:  
		{
			g_entities[0].health = g_entities[0].max_health = 160;
			g_entities[0].client->ps.stats[STAT_HEALTH] = g_entities[0].client->ps.stats[STAT_MAX_HEALTH] = 160;
			g_entities[0].client->ps.forcePowerMax = 112; 
			g_entities[0].client->ps.forcePowerRegenRate = 100; 
			g_entities[0].client->ps.forcePowerRegenAmount = 1; 
		}
		break; 
		case LEVEL7:  
		{
			g_entities[0].health = g_entities[0].max_health = 170;
			g_entities[0].client->ps.stats[STAT_HEALTH] = g_entities[0].client->ps.stats[STAT_MAX_HEALTH] = 170;
			g_entities[0].client->ps.forcePowerMax = 114; 
			g_entities[0].client->ps.forcePowerRegenRate = 100; 
			g_entities[0].client->ps.forcePowerRegenAmount = 1; 
		}
		break; 
		case LEVEL8:  
		{
			g_entities[0].health = g_entities[0].max_health = 180;
			g_entities[0].client->ps.stats[STAT_HEALTH] = g_entities[0].client->ps.stats[STAT_MAX_HEALTH] = 180;
			g_entities[0].client->ps.forcePowerMax = 116; 
			g_entities[0].client->ps.forcePowerRegenRate = 100; 
			g_entities[0].client->ps.forcePowerRegenAmount = 1; 
		}
		break; 
		case LEVEL9:  
		{
			g_entities[0].health = g_entities[0].max_health = 190;
			g_entities[0].client->ps.stats[STAT_HEALTH] = g_entities[0].client->ps.stats[STAT_MAX_HEALTH] = 190;
			g_entities[0].client->ps.forcePowerMax = 118; 
			g_entities[0].client->ps.forcePowerRegenRate = 100; 
			g_entities[0].client->ps.forcePowerRegenAmount = 1; 
		}
		break; 
		case LEVEL10:  
		{
			g_entities[0].health = g_entities[0].max_health = 200;
			g_entities[0].client->ps.stats[STAT_HEALTH] = g_entities[0].client->ps.stats[STAT_MAX_HEALTH] = 200;
			g_entities[0].client->ps.forcePowerMax = 120; 
			g_entities[0].client->ps.forcePowerRegenRate = 100; 
			g_entities[0].client->ps.forcePowerRegenAmount = 2; 
		}
		break; 
		case LEVEL11:  
		{
			g_entities[0].health = g_entities[0].max_health = 210;
			g_entities[0].client->ps.stats[STAT_HEALTH] = g_entities[0].client->ps.stats[STAT_MAX_HEALTH] = 210;
			g_entities[0].client->ps.forcePowerMax = 121; 
			g_entities[0].client->ps.forcePowerRegenRate = 100; 
			g_entities[0].client->ps.forcePowerRegenAmount = 2; 
		}
		break; 
		case LEVEL12:  
		{
			g_entities[0].health = g_entities[0].max_health = 220;
			g_entities[0].client->ps.stats[STAT_HEALTH] = g_entities[0].client->ps.stats[STAT_MAX_HEALTH] = 220;
			g_entities[0].client->ps.forcePowerMax = 122; 
			g_entities[0].client->ps.forcePowerRegenRate = 100; 
			g_entities[0].client->ps.forcePowerRegenAmount = 2; 
		}
		break; 
		case LEVEL13:  
		{
			g_entities[0].health = g_entities[0].max_health = 230;
			g_entities[0].client->ps.stats[STAT_HEALTH] = g_entities[0].client->ps.stats[STAT_MAX_HEALTH] = 230;
			g_entities[0].client->ps.forcePowerMax = 123; 
			g_entities[0].client->ps.forcePowerRegenRate = 100; 
			g_entities[0].client->ps.forcePowerRegenAmount = 2; 
		}
		break; 
		case LEVEL14:  
		{
			g_entities[0].health = g_entities[0].max_health = 240;
			g_entities[0].client->ps.stats[STAT_HEALTH] = g_entities[0].client->ps.stats[STAT_MAX_HEALTH] = 240;
			g_entities[0].client->ps.forcePowerMax = 124; 
			g_entities[0].client->ps.forcePowerRegenRate = 100; 
			g_entities[0].client->ps.forcePowerRegenAmount = 2; 
		}
		break; 
		case LEVEL15:  
		{
			g_entities[0].health = g_entities[0].max_health = 250;
			g_entities[0].client->ps.stats[STAT_HEALTH] = g_entities[0].client->ps.stats[STAT_MAX_HEALTH] = 250;
			g_entities[0].client->ps.forcePowerMax = 125; 
			g_entities[0].client->ps.forcePowerRegenRate = 100; 
			g_entities[0].client->ps.forcePowerRegenAmount = 2; 
		}
		break; 
		case LEVEL16:  
		{
			g_entities[0].health = g_entities[0].max_health = 260;
			g_entities[0].client->ps.stats[STAT_HEALTH] = g_entities[0].client->ps.stats[STAT_MAX_HEALTH] = 260;
			g_entities[0].client->ps.forcePowerMax = 126; 
			g_entities[0].client->ps.forcePowerRegenRate = 100; 
			g_entities[0].client->ps.forcePowerRegenAmount = 2; 
		}
		break; 
		case LEVEL17:  
		{
			g_entities[0].health = g_entities[0].max_health = 270;
			g_entities[0].client->ps.stats[STAT_HEALTH] = g_entities[0].client->ps.stats[STAT_MAX_HEALTH] = 270;
			g_entities[0].client->ps.forcePowerMax = 127; 
			g_entities[0].client->ps.forcePowerRegenRate = 100; 
			g_entities[0].client->ps.forcePowerRegenAmount = 2; 
		}
		break; 
		case LEVEL18:  
		{
			g_entities[0].health = g_entities[0].max_health = 280;
			g_entities[0].client->ps.stats[STAT_HEALTH] = g_entities[0].client->ps.stats[STAT_MAX_HEALTH] = 280;
			g_entities[0].client->ps.forcePowerMax = 128; 
			g_entities[0].client->ps.forcePowerRegenRate = 100; 
			g_entities[0].client->ps.forcePowerRegenAmount = 2; 
		}
		break; 
		case LEVEL19:  
		{
			g_entities[0].health = g_entities[0].max_health = 290;
			g_entities[0].client->ps.stats[STAT_HEALTH] = g_entities[0].client->ps.stats[STAT_MAX_HEALTH] = 290;
			g_entities[0].client->ps.forcePowerMax = 129; 
			g_entities[0].client->ps.forcePowerRegenRate = 100; 
			g_entities[0].client->ps.forcePowerRegenAmount = 2; 
		}
		break; 
		case LEVEL20:  
		{
			g_entities[0].health = g_entities[0].max_health = 300;
			g_entities[0].client->ps.stats[STAT_HEALTH] = g_entities[0].client->ps.stats[STAT_MAX_HEALTH] = 300;
			g_entities[0].client->ps.forcePowerMax = 130; 
			g_entities[0].client->ps.forcePowerRegenRate = 100; 
			g_entities[0].client->ps.forcePowerRegenAmount = 3; 
		}
		break; 
		case LEVEL21:  
		{
			g_entities[0].health = g_entities[0].max_health = 310;
			g_entities[0].client->ps.stats[STAT_HEALTH] = g_entities[0].client->ps.stats[STAT_MAX_HEALTH] = 310;
			g_entities[0].client->ps.forcePowerMax = 131; 
			g_entities[0].client->ps.forcePowerRegenRate = 100; 
			g_entities[0].client->ps.forcePowerRegenAmount = 3; 
		}
		break; 
		case LEVEL22:  
		{
			g_entities[0].health = g_entities[0].max_health = 320;
			g_entities[0].client->ps.stats[STAT_HEALTH] = g_entities[0].client->ps.stats[STAT_MAX_HEALTH] = 320;
				g_entities[0].client->ps.forcePowerMax = 132; 
			g_entities[0].client->ps.forcePowerRegenRate = 100; 
			g_entities[0].client->ps.forcePowerRegenAmount = 3; 
		}
		break; 
		case LEVEL23:  
		{
			g_entities[0].health = g_entities[0].max_health = 330;
			g_entities[0].client->ps.stats[STAT_HEALTH] = g_entities[0].client->ps.stats[STAT_MAX_HEALTH] = 330;
				g_entities[0].client->ps.forcePowerMax = 133; 
			g_entities[0].client->ps.forcePowerRegenRate = 100; 
			g_entities[0].client->ps.forcePowerRegenAmount = 3; 
		}
		break; 
		case LEVEL24:  
		{
			g_entities[0].health = g_entities[0].max_health = 340;
			g_entities[0].client->ps.stats[STAT_HEALTH] = g_entities[0].client->ps.stats[STAT_MAX_HEALTH] = 340;
				g_entities[0].client->ps.forcePowerMax = 134; 
			g_entities[0].client->ps.forcePowerRegenRate = 100; 
			g_entities[0].client->ps.forcePowerRegenAmount = 3; 
		}
		break; 
		case LEVEL25:  
		{
			g_entities[0].health = g_entities[0].max_health = 350;
			g_entities[0].client->ps.stats[STAT_HEALTH] = g_entities[0].client->ps.stats[STAT_MAX_HEALTH] = 350;
				g_entities[0].client->ps.forcePowerMax = 135; 
			g_entities[0].client->ps.forcePowerRegenRate = 100; 
			g_entities[0].client->ps.forcePowerRegenAmount = 3; 
		}
		break; 
		case LEVEL26:  
		{
			g_entities[0].health = g_entities[0].max_health = 360;
			g_entities[0].client->ps.stats[STAT_HEALTH] = g_entities[0].client->ps.stats[STAT_MAX_HEALTH] = 360;
				g_entities[0].client->ps.forcePowerMax = 136; 
			g_entities[0].client->ps.forcePowerRegenRate = 100; 
			g_entities[0].client->ps.forcePowerRegenAmount = 3; 
		}
		break; 
		case LEVEL27:  
		{
			g_entities[0].health = g_entities[0].max_health = 370;
			g_entities[0].client->ps.stats[STAT_HEALTH] = g_entities[0].client->ps.stats[STAT_MAX_HEALTH] = 370;
			g_entities[0].client->ps.forcePowerMax = 137; 
			g_entities[0].client->ps.forcePowerRegenRate = 100; 
			g_entities[0].client->ps.forcePowerRegenAmount = 3; 
		}
		break; 
		case LEVEL28:  
		{
			g_entities[0].health = g_entities[0].max_health = 380;
			g_entities[0].client->ps.stats[STAT_HEALTH] = g_entities[0].client->ps.stats[STAT_MAX_HEALTH] = 380;
			g_entities[0].client->ps.forcePowerMax = 138; 
			g_entities[0].client->ps.forcePowerRegenRate = 100; 
			g_entities[0].client->ps.forcePowerRegenAmount = 3; 
		}
		break; 
		case LEVEL29:  
		{
			g_entities[0].health = g_entities[0].max_health = 390;
			g_entities[0].client->ps.stats[STAT_HEALTH] = g_entities[0].client->ps.stats[STAT_MAX_HEALTH] = 390;
			g_entities[0].client->ps.forcePowerMax = 139; 
			g_entities[0].client->ps.forcePowerRegenRate = 100; 
			g_entities[0].client->ps.forcePowerRegenAmount = 3; 
		}
		break; 
		case LEVEL30:  
		{
			g_entities[0].health = g_entities[0].max_health = 400;
			g_entities[0].client->ps.stats[STAT_HEALTH] = g_entities[0].client->ps.stats[STAT_MAX_HEALTH] = 400;
			g_entities[0].client->ps.forcePowerMax = 140; 
			g_entities[0].client->ps.forcePowerRegenRate = 100; 
			g_entities[0].client->ps.forcePowerRegenAmount = 4; 
		}
		break; 
		case LEVEL31:  
		{
			g_entities[0].health = g_entities[0].max_health = 410;
			g_entities[0].client->ps.stats[STAT_HEALTH] = g_entities[0].client->ps.stats[STAT_MAX_HEALTH] = 410;
			g_entities[0].client->ps.forcePowerMax = 141; 
			g_entities[0].client->ps.forcePowerRegenRate = 100; 
			g_entities[0].client->ps.forcePowerRegenAmount = 4; 
		}
		break; 
		case LEVEL32:  
		{
			g_entities[0].health = g_entities[0].max_health = 420;
			g_entities[0].client->ps.stats[STAT_HEALTH] = g_entities[0].client->ps.stats[STAT_MAX_HEALTH] = 420;
			g_entities[0].client->ps.forcePowerMax = 142; 
			g_entities[0].client->ps.forcePowerRegenRate = 100; 
			g_entities[0].client->ps.forcePowerRegenAmount = 4; 
		}
		break; 
		case LEVEL33:  
		{
			g_entities[0].health = g_entities[0].max_health = 430;
			g_entities[0].client->ps.stats[STAT_HEALTH] = g_entities[0].client->ps.stats[STAT_MAX_HEALTH] = 430;
			g_entities[0].client->ps.forcePowerMax = 143; 
			g_entities[0].client->ps.forcePowerRegenRate = 100; 
			g_entities[0].client->ps.forcePowerRegenAmount = 4; 
		}
		break; 
		case LEVEL34:  
		{
			g_entities[0].health = g_entities[0].max_health = 440;
			g_entities[0].client->ps.stats[STAT_HEALTH] = g_entities[0].client->ps.stats[STAT_MAX_HEALTH] = 440;
			g_entities[0].client->ps.forcePowerMax = 144; 
			g_entities[0].client->ps.forcePowerRegenRate = 100; 
			g_entities[0].client->ps.forcePowerRegenAmount = 4; 
		}
		break; 
		case LEVEL35:  
		{
			g_entities[0].health = g_entities[0].max_health = 450;
			g_entities[0].client->ps.stats[STAT_HEALTH] = g_entities[0].client->ps.stats[STAT_MAX_HEALTH] = 450;
			g_entities[0].client->ps.forcePowerMax = 145; 
			g_entities[0].client->ps.forcePowerRegenRate = 100; 
			g_entities[0].client->ps.forcePowerRegenAmount = 4; 
		}
		break; 
		case LEVEL36:  
		{
			g_entities[0].health = g_entities[0].max_health = 460;
			g_entities[0].client->ps.stats[STAT_HEALTH] = g_entities[0].client->ps.stats[STAT_MAX_HEALTH] = 460;
			g_entities[0].client->ps.forcePowerMax = 146; 
			g_entities[0].client->ps.forcePowerRegenRate = 100; 
			g_entities[0].client->ps.forcePowerRegenAmount = 4; 
		}
		break; 
		case LEVEL37:  
		{
			g_entities[0].health = g_entities[0].max_health = 470;
			g_entities[0].client->ps.stats[STAT_HEALTH] = g_entities[0].client->ps.stats[STAT_MAX_HEALTH] = 470;
			g_entities[0].client->ps.forcePowerMax = 147; 
			g_entities[0].client->ps.forcePowerRegenRate = 100; 
			g_entities[0].client->ps.forcePowerRegenAmount = 4; 
		}
		break; 
		case LEVEL38:  
		{
			g_entities[0].health = g_entities[0].max_health = 480;
			g_entities[0].client->ps.stats[STAT_HEALTH] = g_entities[0].client->ps.stats[STAT_MAX_HEALTH] = 480;
			g_entities[0].client->ps.forcePowerMax = 148; 
			g_entities[0].client->ps.forcePowerRegenRate = 100; 
			g_entities[0].client->ps.forcePowerRegenAmount = 4; 
		}
		break; 
		case LEVEL39:  
		{
			g_entities[0].health = g_entities[0].max_health = 490;
			g_entities[0].client->ps.stats[STAT_HEALTH] = g_entities[0].client->ps.stats[STAT_MAX_HEALTH] = 490;
			g_entities[0].client->ps.forcePowerMax = 149; 
			g_entities[0].client->ps.forcePowerRegenRate = 100; 
			g_entities[0].client->ps.forcePowerRegenAmount = 4; 
		}
		break; 
		case LEVEL40:  
		{
			g_entities[0].health = g_entities[0].max_health = 500;
			g_entities[0].client->ps.stats[STAT_HEALTH] = g_entities[0].client->ps.stats[STAT_MAX_HEALTH] = 500;
			g_entities[0].client->ps.forcePowerMax = 150; 
			g_entities[0].client->ps.forcePowerRegenRate = 100; 
			g_entities[0].client->ps.forcePowerRegenAmount = 5; 
		}
		break; 
		case LEVEL41:  
		{
			g_entities[0].health = g_entities[0].max_health = 510;
			g_entities[0].client->ps.stats[STAT_HEALTH] = g_entities[0].client->ps.stats[STAT_MAX_HEALTH] = 510;
			g_entities[0].client->ps.forcePowerMax = 151; 
			g_entities[0].client->ps.forcePowerRegenRate = 100; 
			g_entities[0].client->ps.forcePowerRegenAmount = 5; 
		}
		break; 
		case LEVEL42:  
		{
			g_entities[0].health = g_entities[0].max_health = 520;
			g_entities[0].client->ps.stats[STAT_HEALTH] = g_entities[0].client->ps.stats[STAT_MAX_HEALTH] = 520;
			g_entities[0].client->ps.forcePowerMax = 152; 
			g_entities[0].client->ps.forcePowerRegenRate = 100; 
			g_entities[0].client->ps.forcePowerRegenAmount = 5; 
		}
		break; 
		case LEVEL43:  
		{
			g_entities[0].health = g_entities[0].max_health = 530;
			g_entities[0].client->ps.stats[STAT_HEALTH] = g_entities[0].client->ps.stats[STAT_MAX_HEALTH] = 530;
			g_entities[0].client->ps.forcePowerMax = 153; 
			g_entities[0].client->ps.forcePowerRegenRate = 100; 
			g_entities[0].client->ps.forcePowerRegenAmount = 5; 
		}
		break; 
		case LEVEL44:  
		{
			g_entities[0].health = g_entities[0].max_health = 540;
			g_entities[0].client->ps.stats[STAT_HEALTH] = g_entities[0].client->ps.stats[STAT_MAX_HEALTH] = 540;
			g_entities[0].client->ps.forcePowerMax = 154; 
			g_entities[0].client->ps.forcePowerRegenRate = 100; 
			g_entities[0].client->ps.forcePowerRegenAmount = 5; 
		}
		break; 
		case LEVEL45:  
		{
			g_entities[0].health = g_entities[0].max_health = 550;
			g_entities[0].client->ps.stats[STAT_HEALTH] = g_entities[0].client->ps.stats[STAT_MAX_HEALTH] = 550;
			g_entities[0].client->ps.forcePowerMax = 155; 
			g_entities[0].client->ps.forcePowerRegenRate = 100; 
			g_entities[0].client->ps.forcePowerRegenAmount = 5; 
		}
		break; 
		case LEVEL46:  
		{
			g_entities[0].health = g_entities[0].max_health = 560;
			g_entities[0].client->ps.stats[STAT_HEALTH] = g_entities[0].client->ps.stats[STAT_MAX_HEALTH] = 560;
			g_entities[0].client->ps.forcePowerMax = 156; 
			g_entities[0].client->ps.forcePowerRegenRate = 100; 
			g_entities[0].client->ps.forcePowerRegenAmount = 5; 
		}
		break; 
		case LEVEL47:  
		{
			g_entities[0].health = g_entities[0].max_health = 570;
			g_entities[0].client->ps.stats[STAT_HEALTH] = g_entities[0].client->ps.stats[STAT_MAX_HEALTH] = 570;
			g_entities[0].client->ps.forcePowerMax = 157; 
			g_entities[0].client->ps.forcePowerRegenRate = 100; 
			g_entities[0].client->ps.forcePowerRegenAmount = 5; 
		}
		break; 
		case LEVEL48:  
		{
			g_entities[0].health = g_entities[0].max_health = 580;
			g_entities[0].client->ps.stats[STAT_HEALTH] = g_entities[0].client->ps.stats[STAT_MAX_HEALTH] = 580;
			g_entities[0].client->ps.forcePowerMax = 158; 
			g_entities[0].client->ps.forcePowerRegenRate = 100; 
			g_entities[0].client->ps.forcePowerRegenAmount = 5; 
		}
		break; 
		case LEVEL49:  
		{
			g_entities[0].health = g_entities[0].max_health = 590;
			g_entities[0].client->ps.stats[STAT_HEALTH] = g_entities[0].client->ps.stats[STAT_MAX_HEALTH] = 590;
			g_entities[0].client->ps.forcePowerMax = 159; 
			g_entities[0].client->ps.forcePowerRegenRate = 100; 
			g_entities[0].client->ps.forcePowerRegenAmount = 5; 
		}
		break; 
		case LEVEL50:  
		{
			g_entities[0].health = g_entities[0].max_health = 600;
			g_entities[0].client->ps.stats[STAT_HEALTH] = g_entities[0].client->ps.stats[STAT_MAX_HEALTH] = 600;
			g_entities[0].client->ps.forcePowerMax = 160; 
			g_entities[0].client->ps.forcePowerRegenRate = 100; 
			g_entities[0].client->ps.forcePowerRegenAmount = 6; 
		}
		break; 
		case LEVEL51:  
		{
			g_entities[0].health = g_entities[0].max_health = 610;
			g_entities[0].client->ps.stats[STAT_HEALTH] = g_entities[0].client->ps.stats[STAT_MAX_HEALTH] = 610;
			g_entities[0].client->ps.forcePowerMax = 161; 
			g_entities[0].client->ps.forcePowerRegenRate = 100; 
			g_entities[0].client->ps.forcePowerRegenAmount = 6; 
		}
		break; 
		case LEVEL52:  
		{
			g_entities[0].health = g_entities[0].max_health = 620;
			g_entities[0].client->ps.stats[STAT_HEALTH] = g_entities[0].client->ps.stats[STAT_MAX_HEALTH] = 620;
			g_entities[0].client->ps.forcePowerMax = 162; 
			g_entities[0].client->ps.forcePowerRegenRate = 100; 
			g_entities[0].client->ps.forcePowerRegenAmount = 6; 
		}
		break; 
		case LEVEL53:  
		{
			g_entities[0].health = g_entities[0].max_health = 630;
			g_entities[0].client->ps.stats[STAT_HEALTH] = g_entities[0].client->ps.stats[STAT_MAX_HEALTH] = 630;
			g_entities[0].client->ps.forcePowerMax = 163; 
			g_entities[0].client->ps.forcePowerRegenRate = 100; 
			g_entities[0].client->ps.forcePowerRegenAmount = 6; 
		
		}
		break; 
		case LEVEL54:  
		{
			g_entities[0].health = g_entities[0].max_health = 640;
			g_entities[0].client->ps.stats[STAT_HEALTH] = g_entities[0].client->ps.stats[STAT_MAX_HEALTH] = 640;
			g_entities[0].client->ps.forcePowerMax = 164; 
			g_entities[0].client->ps.forcePowerRegenRate = 100; 
			g_entities[0].client->ps.forcePowerRegenAmount = 6; 
		
		}
		break; 
		case LEVEL55:  
		{
			g_entities[0].health = g_entities[0].max_health = 650;
			g_entities[0].client->ps.stats[STAT_HEALTH] = g_entities[0].client->ps.stats[STAT_MAX_HEALTH] = 650;
			g_entities[0].client->ps.forcePowerMax = 165; 
			g_entities[0].client->ps.forcePowerRegenRate = 100; 
			g_entities[0].client->ps.forcePowerRegenAmount = 6; 
		
		}
		break; 
		case LEVEL56:  
		{
			g_entities[0].health = g_entities[0].max_health = 660;
			g_entities[0].client->ps.stats[STAT_HEALTH] = g_entities[0].client->ps.stats[STAT_MAX_HEALTH] = 660;
			g_entities[0].client->ps.forcePowerMax = 166; 
			g_entities[0].client->ps.forcePowerRegenRate = 100; 
			g_entities[0].client->ps.forcePowerRegenAmount = 6; 
		
		}
		break; 
		case LEVEL57:  
		{
			g_entities[0].health = g_entities[0].max_health = 670;
			g_entities[0].client->ps.stats[STAT_HEALTH] = g_entities[0].client->ps.stats[STAT_MAX_HEALTH] = 670;
			g_entities[0].client->ps.forcePowerMax = 167; 
			g_entities[0].client->ps.forcePowerRegenRate = 100; 
			g_entities[0].client->ps.forcePowerRegenAmount = 6; 
		}
		break; 
		case LEVEL58:  
		{
			g_entities[0].health = g_entities[0].max_health = 680;
			g_entities[0].client->ps.stats[STAT_HEALTH] = g_entities[0].client->ps.stats[STAT_MAX_HEALTH] = 680;
			g_entities[0].client->ps.forcePowerMax = 168; 
			g_entities[0].client->ps.forcePowerRegenRate = 100; 
			g_entities[0].client->ps.forcePowerRegenAmount = 6; 
		
		}
		break; 
		case LEVEL59:  
		{
			g_entities[0].health = g_entities[0].max_health = 690;
			g_entities[0].client->ps.stats[STAT_HEALTH] = g_entities[0].client->ps.stats[STAT_MAX_HEALTH] = 690;
			g_entities[0].client->ps.forcePowerMax = 169; 
			g_entities[0].client->ps.forcePowerRegenRate = 100; 
			g_entities[0].client->ps.forcePowerRegenAmount = 6; 
		
		}
		break; 
		case LEVEL60:  
		{
			g_entities[0].health = g_entities[0].max_health = 700;
			g_entities[0].client->ps.stats[STAT_HEALTH] = g_entities[0].client->ps.stats[STAT_MAX_HEALTH] = 700;
			g_entities[0].client->ps.forcePowerMax = 170; 
			g_entities[0].client->ps.forcePowerRegenRate = 150; 
			g_entities[0].client->ps.forcePowerRegenAmount = 7; 
		}
		break; 
		case LEVEL61:  
		{
			g_entities[0].health = g_entities[0].max_health = 710;
			g_entities[0].client->ps.stats[STAT_HEALTH] = g_entities[0].client->ps.stats[STAT_MAX_HEALTH] = 710;
			g_entities[0].client->ps.forcePowerMax = 171; 
			g_entities[0].client->ps.forcePowerRegenRate = 150; 
			g_entities[0].client->ps.forcePowerRegenAmount = 7; 
		
		}
		break; 
		case LEVEL62:  
		{
			g_entities[0].health = g_entities[0].max_health = 720;
			g_entities[0].client->ps.stats[STAT_HEALTH] = g_entities[0].client->ps.stats[STAT_MAX_HEALTH] = 720;
			g_entities[0].client->ps.forcePowerMax = 172; 
			g_entities[0].client->ps.forcePowerRegenRate = 150; 
			g_entities[0].client->ps.forcePowerRegenAmount = 7; 
		
		
		}
		break; 
		case LEVEL63:  
		{
			g_entities[0].health = g_entities[0].max_health = 730;
			g_entities[0].client->ps.stats[STAT_HEALTH] = g_entities[0].client->ps.stats[STAT_MAX_HEALTH] = 730;
			g_entities[0].client->ps.forcePowerMax = 173; 
			g_entities[0].client->ps.forcePowerRegenRate = 150; 
			g_entities[0].client->ps.forcePowerRegenAmount = 7; 
		
		}
		break; 
		case LEVEL64:  
		{
			g_entities[0].health = g_entities[0].max_health = 740;
			g_entities[0].client->ps.stats[STAT_HEALTH] = g_entities[0].client->ps.stats[STAT_MAX_HEALTH] = 740;
			g_entities[0].client->ps.forcePowerMax = 174; 
			g_entities[0].client->ps.forcePowerRegenRate = 150; 
			g_entities[0].client->ps.forcePowerRegenAmount = 7; 
		
		}
		break; 
		case LEVEL65:  
		{
			g_entities[0].health = g_entities[0].max_health = 750;
			g_entities[0].client->ps.stats[STAT_HEALTH] = g_entities[0].client->ps.stats[STAT_MAX_HEALTH] = 750;
			g_entities[0].client->ps.forcePowerMax = 175; 
			g_entities[0].client->ps.forcePowerRegenRate = 150; 
			g_entities[0].client->ps.forcePowerRegenAmount = 8; 
		
		}
		break; 
		case LEVEL66:  
		{
			g_entities[0].health = g_entities[0].max_health = 760;
			g_entities[0].client->ps.stats[STAT_HEALTH] = g_entities[0].client->ps.stats[STAT_MAX_HEALTH] = 760;
			g_entities[0].client->ps.forcePowerMax = 176; 
			g_entities[0].client->ps.forcePowerRegenRate = 150; 
			g_entities[0].client->ps.forcePowerRegenAmount = 8; 
		}
		break; 
		case LEVEL67:  
		{
			g_entities[0].health = g_entities[0].max_health = 770;
			g_entities[0].client->ps.stats[STAT_HEALTH] = g_entities[0].client->ps.stats[STAT_MAX_HEALTH] = 770;
			g_entities[0].client->ps.forcePowerMax = 177; 
			g_entities[0].client->ps.forcePowerRegenRate = 150; 
			g_entities[0].client->ps.forcePowerRegenAmount = 8; 
		}
		break; 
		case LEVEL68:  
		{
			g_entities[0].health = g_entities[0].max_health = 780;
			g_entities[0].client->ps.stats[STAT_HEALTH] = g_entities[0].client->ps.stats[STAT_MAX_HEALTH] = 780;
			g_entities[0].client->ps.forcePowerMax = 178; 
			g_entities[0].client->ps.forcePowerRegenRate = 150; 
			g_entities[0].client->ps.forcePowerRegenAmount = 8; 
		}
		break; 
		case LEVEL69:  
		{
			g_entities[0].health = g_entities[0].max_health = 790;
			g_entities[0].client->ps.stats[STAT_HEALTH] = g_entities[0].client->ps.stats[STAT_MAX_HEALTH] = 790;
			g_entities[0].client->ps.forcePowerMax = 179; 
			g_entities[0].client->ps.forcePowerRegenRate = 150; 
			g_entities[0].client->ps.forcePowerRegenAmount = 8; 
		}
		break; 
		case LEVEL70:  
		{
			g_entities[0].health = g_entities[0].max_health = 800;
			g_entities[0].client->ps.stats[STAT_HEALTH] = g_entities[0].client->ps.stats[STAT_MAX_HEALTH] = 800;
			g_entities[0].client->ps.forcePowerMax = 180; 
			g_entities[0].client->ps.forcePowerRegenRate = 150; 
			g_entities[0].client->ps.forcePowerRegenAmount = 9; 
		}
		break; 
		case LEVEL71:  
		{
			g_entities[0].health = g_entities[0].max_health = 810;
			g_entities[0].client->ps.stats[STAT_HEALTH] = g_entities[0].client->ps.stats[STAT_MAX_HEALTH] = 810;
			g_entities[0].client->ps.forcePowerMax = 182; 
			g_entities[0].client->ps.forcePowerRegenRate = 150; 
			g_entities[0].client->ps.forcePowerRegenAmount = 9; 
		}
		break; 
		case LEVEL72:  
		{
			g_entities[0].health = g_entities[0].max_health = 820;
			g_entities[0].client->ps.stats[STAT_HEALTH] = g_entities[0].client->ps.stats[STAT_MAX_HEALTH] = 820;
			g_entities[0].client->ps.forcePowerMax = 184; 
			g_entities[0].client->ps.forcePowerRegenRate = 150; 
			g_entities[0].client->ps.forcePowerRegenAmount = 9; 

		}
		break; 
		case LEVEL73:  
		{
			g_entities[0].health = g_entities[0].max_health = 830;
			g_entities[0].client->ps.stats[STAT_HEALTH] = g_entities[0].client->ps.stats[STAT_MAX_HEALTH] = 830;
			g_entities[0].client->ps.forcePowerMax = 186; 
			g_entities[0].client->ps.forcePowerRegenRate = 150; 
			g_entities[0].client->ps.forcePowerRegenAmount = 9; 

		}
		break; 
		case LEVEL74:  
		{
			g_entities[0].health = g_entities[0].max_health = 840;
			g_entities[0].client->ps.stats[STAT_HEALTH] = g_entities[0].client->ps.stats[STAT_MAX_HEALTH] = 840;
			g_entities[0].client->ps.forcePowerMax = 188; 
			g_entities[0].client->ps.forcePowerRegenRate = 150; 
			g_entities[0].client->ps.forcePowerRegenAmount = 9; 

		}
		break; 
		case LEVEL75:  
		{
			g_entities[0].health = g_entities[0].max_health = 850;
			g_entities[0].client->ps.stats[STAT_HEALTH] = g_entities[0].client->ps.stats[STAT_MAX_HEALTH] = 850;
			g_entities[0].client->ps.forcePowerMax = 190; 
			g_entities[0].client->ps.forcePowerRegenRate = 150; 
			g_entities[0].client->ps.forcePowerRegenAmount = 10; 

		}
		break; 
		case LEVEL76:  
		{
			g_entities[0].health = g_entities[0].max_health = 860;
			g_entities[0].client->ps.stats[STAT_HEALTH] = g_entities[0].client->ps.stats[STAT_MAX_HEALTH] = 860;
			g_entities[0].client->ps.forcePowerMax = 192; 
			g_entities[0].client->ps.forcePowerRegenRate = 150; 
			g_entities[0].client->ps.forcePowerRegenAmount = 10; 

		}
		break; 
		case LEVEL77:  
		{
			g_entities[0].health = g_entities[0].max_health = 870;
			g_entities[0].client->ps.stats[STAT_HEALTH] = g_entities[0].client->ps.stats[STAT_MAX_HEALTH] = 870;
			g_entities[0].client->ps.forcePowerMax = 194; 
			g_entities[0].client->ps.forcePowerRegenRate = 150; 
			g_entities[0].client->ps.forcePowerRegenAmount = 10; 

		}
		break; 
		case LEVEL78:  
		{
			g_entities[0].health = g_entities[0].max_health = 880;
			g_entities[0].client->ps.stats[STAT_HEALTH] = g_entities[0].client->ps.stats[STAT_MAX_HEALTH] = 880;
			g_entities[0].client->ps.forcePowerMax = 196; 
			g_entities[0].client->ps.forcePowerRegenRate = 150; 
			g_entities[0].client->ps.forcePowerRegenAmount = 10; 
		}
		break; 
		case LEVEL79:  
		{
			g_entities[0].health = g_entities[0].max_health = 890;
			g_entities[0].client->ps.stats[STAT_HEALTH] = g_entities[0].client->ps.stats[STAT_MAX_HEALTH] = 890;
				g_entities[0].client->ps.forcePowerMax = 198; 
			g_entities[0].client->ps.forcePowerRegenRate = 150; 
			g_entities[0].client->ps.forcePowerRegenAmount = 10; 
		}
		break; 
		case LEVEL80:  
		{
			g_entities[0].health = g_entities[0].max_health = 900;
			g_entities[0].client->ps.stats[STAT_HEALTH] = g_entities[0].client->ps.stats[STAT_MAX_HEALTH] = 900;
			g_entities[0].client->ps.forcePowerMax = 200; 
			g_entities[0].client->ps.forcePowerRegenRate = 150; 
			g_entities[0].client->ps.forcePowerRegenAmount = 11; 

		}
		break; 
		case LEVEL81:  
		{
			g_entities[0].health = g_entities[0].max_health = 910;
			g_entities[0].client->ps.stats[STAT_HEALTH] = g_entities[0].client->ps.stats[STAT_MAX_HEALTH] = 910;
			g_entities[0].client->ps.forcePowerMax = 220; 
			g_entities[0].client->ps.forcePowerRegenRate = 150; 
			g_entities[0].client->ps.forcePowerRegenAmount = 11; 
		}
		break; 
		case LEVEL82:  
		{
			g_entities[0].health = g_entities[0].max_health = 920;
			g_entities[0].client->ps.stats[STAT_HEALTH] = g_entities[0].client->ps.stats[STAT_MAX_HEALTH] = 920;
			g_entities[0].client->ps.forcePowerMax = 240; 
			g_entities[0].client->ps.forcePowerRegenRate = 150; 
			g_entities[0].client->ps.forcePowerRegenAmount = 11; 
		}
		break; 
		case LEVEL83:  
		{
			g_entities[0].health = g_entities[0].max_health = 930;
			g_entities[0].client->ps.stats[STAT_HEALTH] = g_entities[0].client->ps.stats[STAT_MAX_HEALTH] = 930;
			g_entities[0].client->ps.forcePowerMax = 260; 
			g_entities[0].client->ps.forcePowerRegenRate = 150; 
			g_entities[0].client->ps.forcePowerRegenAmount = 11; 
		}
		break; 
		case LEVEL84:  
		{
			g_entities[0].health = g_entities[0].max_health = 940;
			g_entities[0].client->ps.stats[STAT_HEALTH] = g_entities[0].client->ps.stats[STAT_MAX_HEALTH] = 940;
			g_entities[0].client->ps.forcePowerMax = 280; 
			g_entities[0].client->ps.forcePowerRegenRate = 150; 
			g_entities[0].client->ps.forcePowerRegenAmount = 11; 
		}
		break; 
		case LEVEL85:  
		{
			g_entities[0].health = g_entities[0].max_health = 950;
			g_entities[0].client->ps.stats[STAT_HEALTH] = g_entities[0].client->ps.stats[STAT_MAX_HEALTH] = 950;
			g_entities[0].client->ps.forcePowerMax = 300; 
			g_entities[0].client->ps.forcePowerRegenRate = 150; 
			g_entities[0].client->ps.forcePowerRegenAmount = 12; 
		}
		break; 
		case LEVEL86:  
		{
			g_entities[0].health = g_entities[0].max_health = 960;
			g_entities[0].client->ps.stats[STAT_HEALTH] = g_entities[0].client->ps.stats[STAT_MAX_HEALTH] = 960;
			g_entities[0].client->ps.forcePowerMax = 320; 
			g_entities[0].client->ps.forcePowerRegenRate = 150; 
			g_entities[0].client->ps.forcePowerRegenAmount = 12; 
		}
		break; 
		case LEVEL87:  
		{
			g_entities[0].health = g_entities[0].max_health = 970;
			g_entities[0].client->ps.stats[STAT_HEALTH] = g_entities[0].client->ps.stats[STAT_MAX_HEALTH] = 970;
			g_entities[0].client->ps.forcePowerMax = 340; 
			g_entities[0].client->ps.forcePowerRegenRate = 150; 
			g_entities[0].client->ps.forcePowerRegenAmount = 12; 
		}
		break; 
		case LEVEL88:  
		{
			g_entities[0].health = g_entities[0].max_health = 980;
			g_entities[0].client->ps.stats[STAT_HEALTH] = g_entities[0].client->ps.stats[STAT_MAX_HEALTH] = 980;
			g_entities[0].client->ps.forcePowerMax = 360; 
			g_entities[0].client->ps.forcePowerRegenRate = 150; 
			g_entities[0].client->ps.forcePowerRegenAmount = 12; 
		}
		break; 
		case LEVEL89:  
		{
			g_entities[0].health = g_entities[0].max_health = 990;
			g_entities[0].client->ps.stats[STAT_HEALTH] = g_entities[0].client->ps.stats[STAT_MAX_HEALTH] = 990;
			g_entities[0].client->ps.forcePowerMax = 380; 
			g_entities[0].client->ps.forcePowerRegenRate = 150; 
			g_entities[0].client->ps.forcePowerRegenAmount = 12; 
		}
		break; 
		case LEVEL90: 
		{
			g_entities[0].health = g_entities[0].max_health = 1000;
			g_entities[0].client->ps.stats[STAT_HEALTH] = g_entities[0].client->ps.stats[STAT_MAX_HEALTH] = 1000;
			g_entities[0].client->ps.forcePowerMax = 400; 
			g_entities[0].client->ps.forcePowerRegenRate = 150; 
			g_entities[0].client->ps.forcePowerRegenAmount = 13; 
		}
		break; 
		case LEVEL91:
		{
			g_entities[0].health = g_entities[0].max_health = 1000;
			g_entities[0].client->ps.stats[STAT_HEALTH] = g_entities[0].client->ps.stats[STAT_MAX_HEALTH] = 1000;
			g_entities[0].client->ps.forcePowerMax = 420; 
			g_entities[0].client->ps.forcePowerRegenRate = 150; 
			g_entities[0].client->ps.forcePowerRegenAmount = 13; 
		}
		break; 
		case LEVEL92:
		{
			g_entities[0].health = g_entities[0].max_health = 1000;
			g_entities[0].client->ps.stats[STAT_HEALTH] = g_entities[0].client->ps.stats[STAT_MAX_HEALTH] = 1000;
			g_entities[0].client->ps.forcePowerMax = 440; 
			g_entities[0].client->ps.forcePowerRegenRate = 150; 
			g_entities[0].client->ps.forcePowerRegenAmount = 13; 
		}
		break; 
		case LEVEL93:
		{
			g_entities[0].health = g_entities[0].max_health = 1000;
			g_entities[0].client->ps.stats[STAT_HEALTH] = g_entities[0].client->ps.stats[STAT_MAX_HEALTH] = 1000;
			g_entities[0].client->ps.forcePowerMax = 460; 
			g_entities[0].client->ps.forcePowerRegenRate = 150; 
			g_entities[0].client->ps.forcePowerRegenAmount = 13; 
		}
		break; 
		case LEVEL94:
		{
			g_entities[0].health = g_entities[0].max_health = 1000;
			g_entities[0].client->ps.stats[STAT_HEALTH] = g_entities[0].client->ps.stats[STAT_MAX_HEALTH] = 1000;
			g_entities[0].client->ps.forcePowerMax = 480; 
			g_entities[0].client->ps.forcePowerRegenRate = 150; 
			g_entities[0].client->ps.forcePowerRegenAmount = 13; 
		}
		break; 
		case LEVEL95:
		{
			g_entities[0].health = g_entities[0].max_health = 1000;
			g_entities[0].client->ps.stats[STAT_HEALTH] = g_entities[0].client->ps.stats[STAT_MAX_HEALTH] = 1000;
			g_entities[0].client->ps.forcePowerMax = 500; 
			g_entities[0].client->ps.forcePowerRegenRate = 150; 
			g_entities[0].client->ps.forcePowerRegenAmount = 14; 
		}
		break; 
		case LEVEL96:
		{
			g_entities[0].health = g_entities[0].max_health = 1000;
			g_entities[0].client->ps.stats[STAT_HEALTH] = g_entities[0].client->ps.stats[STAT_MAX_HEALTH] = 1000;
			g_entities[0].client->ps.forcePowerMax = 520; 
			g_entities[0].client->ps.forcePowerRegenRate = 150; 
			g_entities[0].client->ps.forcePowerRegenAmount = 14; 
		}
		break; 
		case LEVEL97:
		{
			g_entities[0].health = g_entities[0].max_health = 1000;
			g_entities[0].client->ps.stats[STAT_HEALTH] = g_entities[0].client->ps.stats[STAT_MAX_HEALTH] = 1000;
			g_entities[0].client->ps.forcePowerMax = 540; 
			g_entities[0].client->ps.forcePowerRegenRate = 150; 
			g_entities[0].client->ps.forcePowerRegenAmount = 14; 
		}
		break; 
		case LEVEL98:
		{
			g_entities[0].health = g_entities[0].max_health = 1000;
			g_entities[0].client->ps.stats[STAT_HEALTH] = g_entities[0].client->ps.stats[STAT_MAX_HEALTH] = 1000;
			g_entities[0].client->ps.forcePowerMax = 560; 
			g_entities[0].client->ps.forcePowerRegenRate = 150; 
			g_entities[0].client->ps.forcePowerRegenAmount = 14; 
		}
		break; 
		case LEVEL99:
		{
			g_entities[0].health = g_entities[0].max_health = 1000;
			g_entities[0].client->ps.stats[STAT_HEALTH] = g_entities[0].client->ps.stats[STAT_MAX_HEALTH] = 1000;
			g_entities[0].client->ps.forcePowerMax = 580; 
			g_entities[0].client->ps.forcePowerRegenRate = 150; 
			g_entities[0].client->ps.forcePowerRegenAmount = 14; 
		}
		break; 
		case LEVEL100: 
		{
			g_entities[0].health = g_entities[0].max_health = 1000;
			g_entities[0].client->ps.stats[STAT_HEALTH] = g_entities[0].client->ps.stats[STAT_MAX_HEALTH] = 1000;
			g_entities[0].client->ps.forcePowerMax = 600; 
			g_entities[0].client->ps.forcePowerRegenRate = 150; 
			g_entities[0].client->ps.forcePowerRegenAmount = 15; 
		}
		break; 
		case LEVEL101: 
		{
			g_entities[0].health = g_entities[0].max_health = 1000;
			g_entities[0].client->ps.stats[STAT_HEALTH] = g_entities[0].client->ps.stats[STAT_MAX_HEALTH] = 1000;
			g_entities[0].client->ps.forcePowerMax = 620; 
			g_entities[0].client->ps.forcePowerRegenRate = 150; 
			g_entities[0].client->ps.forcePowerRegenAmount = 15; 
		}
		break; 
		case LEVEL102: 
		{
			g_entities[0].health = g_entities[0].max_health = 1000;
			g_entities[0].client->ps.stats[STAT_HEALTH] = g_entities[0].client->ps.stats[STAT_MAX_HEALTH] = 1000;
			g_entities[0].client->ps.forcePowerMax = 640; 
			g_entities[0].client->ps.forcePowerRegenRate = 150; 
			g_entities[0].client->ps.forcePowerRegenAmount = 15; 
		}
		break; 
		case LEVEL103: 
		{
			g_entities[0].health = g_entities[0].max_health = 1000;
			g_entities[0].client->ps.stats[STAT_HEALTH] = g_entities[0].client->ps.stats[STAT_MAX_HEALTH] = 1000;
			g_entities[0].client->ps.forcePowerMax = 660; 
			g_entities[0].client->ps.forcePowerRegenRate = 150; 
			g_entities[0].client->ps.forcePowerRegenAmount = 15; 
		}
		break; 
		case LEVEL104: 
		{
			g_entities[0].health = g_entities[0].max_health = 1000;
			g_entities[0].client->ps.stats[STAT_HEALTH] = g_entities[0].client->ps.stats[STAT_MAX_HEALTH] = 1000;
			g_entities[0].client->ps.forcePowerMax = 680; 
			g_entities[0].client->ps.forcePowerRegenRate = 150; 
			g_entities[0].client->ps.forcePowerRegenAmount = 15; 
		}
		break; 
		case LEVEL105: 
		{
			g_entities[0].health = g_entities[0].max_health = 1000;
			g_entities[0].client->ps.stats[STAT_HEALTH] = g_entities[0].client->ps.stats[STAT_MAX_HEALTH] = 1000;
			g_entities[0].client->ps.forcePowerMax = 700; 
			g_entities[0].client->ps.forcePowerRegenRate = 150; 
			g_entities[0].client->ps.forcePowerRegenAmount = 16; 
		}
		break; 
		case LEVEL106: 
		{
			g_entities[0].health = g_entities[0].max_health = 1000;
			g_entities[0].client->ps.stats[STAT_HEALTH] = g_entities[0].client->ps.stats[STAT_MAX_HEALTH] = 1000;
			g_entities[0].client->ps.forcePowerMax = 720; 
			g_entities[0].client->ps.forcePowerRegenRate = 150; 
			g_entities[0].client->ps.forcePowerRegenAmount = 16; 
		}
		break; 
		case LEVEL107: 
		{
			g_entities[0].health = g_entities[0].max_health = 1000;
			g_entities[0].client->ps.stats[STAT_HEALTH] = g_entities[0].client->ps.stats[STAT_MAX_HEALTH] = 1000;
			g_entities[0].client->ps.forcePowerMax = 740; 
			g_entities[0].client->ps.forcePowerRegenRate = 150; 
			g_entities[0].client->ps.forcePowerRegenAmount = 16; 
		}
		break; 
		case LEVEL108: 
		{
			g_entities[0].health = g_entities[0].max_health = 1000;
			g_entities[0].client->ps.stats[STAT_HEALTH] = g_entities[0].client->ps.stats[STAT_MAX_HEALTH] = 1000;
			g_entities[0].client->ps.forcePowerMax = 760; 
			g_entities[0].client->ps.forcePowerRegenRate = 150; 
			g_entities[0].client->ps.forcePowerRegenAmount = 16; 
		}
		break; 
		case LEVEL109: 
		{
			g_entities[0].health = g_entities[0].max_health = 1000;
			g_entities[0].client->ps.stats[STAT_HEALTH] = g_entities[0].client->ps.stats[STAT_MAX_HEALTH] = 1000;
			g_entities[0].client->ps.forcePowerMax = 780; 
			g_entities[0].client->ps.forcePowerRegenRate = 150; 
			g_entities[0].client->ps.forcePowerRegenAmount = 16; 
		}
		break; 
		case LEVEL110: 
		{
			g_entities[0].health = g_entities[0].max_health = 1000;
			g_entities[0].client->ps.stats[STAT_HEALTH] = g_entities[0].client->ps.stats[STAT_MAX_HEALTH] = 1000;
			g_entities[0].client->ps.forcePowerMax = 800; 
			g_entities[0].client->ps.forcePowerRegenRate = 150; 
			g_entities[0].client->ps.forcePowerRegenAmount = 17; 
		}
		break; 
		case LEVEL111: 
		{
			g_entities[0].health = g_entities[0].max_health = 1000;
			g_entities[0].client->ps.stats[STAT_HEALTH] = g_entities[0].client->ps.stats[STAT_MAX_HEALTH] = 1000;
			g_entities[0].client->ps.forcePowerMax = 820; 
			g_entities[0].client->ps.forcePowerRegenRate = 150; 
			g_entities[0].client->ps.forcePowerRegenAmount = 17; 
		}
		break; 

		case LEVEL112: 
		{
			g_entities[0].health = g_entities[0].max_health = 1000;
			g_entities[0].client->ps.stats[STAT_HEALTH] = g_entities[0].client->ps.stats[STAT_MAX_HEALTH] = 1000;
			g_entities[0].client->ps.forcePowerMax = 840; 
			g_entities[0].client->ps.forcePowerRegenRate = 150; 
			g_entities[0].client->ps.forcePowerRegenAmount = 17; 
		}
		break; 


		case LEVEL113: 
		{
			g_entities[0].health = g_entities[0].max_health = 1000;
			g_entities[0].client->ps.stats[STAT_HEALTH] = g_entities[0].client->ps.stats[STAT_MAX_HEALTH] = 1000;
			g_entities[0].client->ps.forcePowerMax = 860; 
			g_entities[0].client->ps.forcePowerRegenRate = 150; 
			g_entities[0].client->ps.forcePowerRegenAmount = 17; 
		}
		break; 

		case LEVEL114: 
		{
			g_entities[0].health = g_entities[0].max_health = 1000;
			g_entities[0].client->ps.stats[STAT_HEALTH] = g_entities[0].client->ps.stats[STAT_MAX_HEALTH] = 1000;
			g_entities[0].client->ps.forcePowerMax = 880; 
			g_entities[0].client->ps.forcePowerRegenRate = 150; 
			g_entities[0].client->ps.forcePowerRegenAmount = 17; 
		}
		break; 
		case LEVEL115: 
		{
			g_entities[0].health = g_entities[0].max_health = 1000;
			g_entities[0].client->ps.stats[STAT_HEALTH] = g_entities[0].client->ps.stats[STAT_MAX_HEALTH] = 1000;
			g_entities[0].client->ps.forcePowerMax = 900; 
			g_entities[0].client->ps.forcePowerRegenRate = 150; 
			g_entities[0].client->ps.forcePowerRegenAmount = 18; 
		}
		break; 
		case LEVEL116: 
		{
			g_entities[0].health = g_entities[0].max_health = 1000;
			g_entities[0].client->ps.stats[STAT_HEALTH] = g_entities[0].client->ps.stats[STAT_MAX_HEALTH] = 1000;
			g_entities[0].client->ps.forcePowerMax = 920; 
			g_entities[0].client->ps.forcePowerRegenRate = 150; 
			g_entities[0].client->ps.forcePowerRegenAmount = 18; 
		}
		break; 
		case LEVEL117: 
		{
			g_entities[0].health = g_entities[0].max_health = 1000;
			g_entities[0].client->ps.stats[STAT_HEALTH] = g_entities[0].client->ps.stats[STAT_MAX_HEALTH] = 1000;
			g_entities[0].client->ps.forcePowerMax = 940; 
			g_entities[0].client->ps.forcePowerRegenRate = 150; 
			g_entities[0].client->ps.forcePowerRegenAmount = 18; 
		}
		break; 
		case LEVEL118: 
		{
			g_entities[0].health = g_entities[0].max_health = 1000;
			g_entities[0].client->ps.stats[STAT_HEALTH] = g_entities[0].client->ps.stats[STAT_MAX_HEALTH] = 1000;
			g_entities[0].client->ps.forcePowerMax = 960; 
			g_entities[0].client->ps.forcePowerRegenRate = 150; 
			g_entities[0].client->ps.forcePowerRegenAmount = 18; 
		}
		break; 
		case LEVEL119: 
		{
			g_entities[0].health = g_entities[0].max_health = 1000;
			g_entities[0].client->ps.stats[STAT_HEALTH] = g_entities[0].client->ps.stats[STAT_MAX_HEALTH] = 1000;
			g_entities[0].client->ps.forcePowerMax = 980; 
			g_entities[0].client->ps.forcePowerRegenRate = 150; 
			g_entities[0].client->ps.forcePowerRegenAmount = 19; 
		}
		break; 
		case LEVEL120: 
		{
			g_entities[0].health = g_entities[0].max_health = 1000;
			g_entities[0].client->ps.stats[STAT_HEALTH] = g_entities[0].client->ps.stats[STAT_MAX_HEALTH] = 1000;
			g_entities[0].client->ps.forcePowerMax = 1000; 
			g_entities[0].client->ps.forcePowerRegenRate = 150; 
			g_entities[0].client->ps.forcePowerRegenAmount = 20; 
		}
		break; 






	}
}
// SET PLAYER ARMOUR 
void Svcmd_SetPlayerArmor_f ( void )
{
	if ( !&g_entities[0] || !g_entities[0].client )
	{
		return;
	}
	if ( !g_cheats->integer ) 
	{
		gi.SendServerCommand( 0, "print \"Cheats are not enabled on this server.\n\"");
		return;
	}
	const char *newVal = gi.argv(1);
	if ( !VALIDSTRING( newVal ) )
	{
//		return;
	}
	int val = atoi(newVal);
	if ( val > ARMOR_NONE )
	{
		//g_entities[0].client->ls.PlayerLevel != LEVEL_NONE );
	}
	
	g_entities[0].client->ls.PlayerArmorLevel[NUM_ARMORS] = val;
	if ( g_entities[0].client->ls.PlayerArmorLevel[NUM_ARMORS] < ARMOR_NONE )
	{
		g_entities[0].client->ls.PlayerArmorLevel[NUM_ARMORS] = ARMOR_NONE;
	}
	else if ( g_entities[0].client->ls.PlayerArmorLevel[NUM_ARMORS] > ARMOR20 )
	{
		g_entities[0].client->ls.PlayerArmorLevel[NUM_ARMORS] = ARMOR20;
	}
	// NOW CAP AND SET THE PLAYER HEALTH in relation of the amount of level. 
	
	int		max;
	const char	*cmd;
	cmd = gi.argv(1);
	if ( cmd && atoi( cmd ) )
	{
		max = atoi( cmd );
	}
	else
	{
		//max = 999;
		max = 100;
	}
	g_entities[0].armor = g_entities[0].max_armor = max;
	switch ( g_entities[0].client->ls.PlayerArmorLevel[NUM_ARMORS] )
	{
		
		case ARMOR_NONE:  
		{
			g_entities[0].armor = g_entities[0].max_armor = 0;
			g_entities[0].client->ps.stats[STAT_ARMOR] = g_entities[0].client->ps.stats[STAT_MAX_ARMOR] = 0;
		}
		break; 
		// normal physical armors. each race have 10 armors type. 
		case ARMOR1:
		{
			g_entities[0].armor = g_entities[0].max_armor = 100;
			g_entities[0].client->ps.stats[STAT_ARMOR] = g_entities[0].client->ps.stats[STAT_MAX_ARMOR] = 100;
		}
		break; 
		case ARMOR2:  
		{
			g_entities[0].armor = g_entities[0].max_armor = 120;
			g_entities[0].client->ps.stats[STAT_ARMOR] = g_entities[0].client->ps.stats[STAT_MAX_ARMOR] = 120;
		}
		break; 
		case ARMOR3:
		{
			g_entities[0].armor = g_entities[0].max_armor = 150;
			g_entities[0].client->ps.stats[STAT_ARMOR] = g_entities[0].client->ps.stats[STAT_MAX_ARMOR] = 150;
		}
		break; 
		case ARMOR4:  
		{
			g_entities[0].armor = g_entities[0].max_armor = 200;
			g_entities[0].client->ps.stats[STAT_ARMOR] = g_entities[0].client->ps.stats[STAT_MAX_ARMOR] = 200;
		}
		break; 
		case ARMOR5:
		{
			g_entities[0].armor = g_entities[0].max_armor = 250;
			g_entities[0].client->ps.stats[STAT_ARMOR] = g_entities[0].client->ps.stats[STAT_MAX_ARMOR] = 250;
		}
		break; 
		case ARMOR6:  
		{
			g_entities[0].armor = g_entities[0].max_armor = 300;
			g_entities[0].client->ps.stats[STAT_ARMOR] = g_entities[0].client->ps.stats[STAT_MAX_ARMOR] = 300;
		}
		break; 
		case ARMOR7:
		{
			g_entities[0].armor = g_entities[0].max_armor = 350;
			g_entities[0].client->ps.stats[STAT_ARMOR] = g_entities[0].client->ps.stats[STAT_MAX_ARMOR] = 350;
		}
		break; 
		case ARMOR8:  
		{
			g_entities[0].armor = g_entities[0].max_armor = 400;
			g_entities[0].client->ps.stats[STAT_ARMOR] = g_entities[0].client->ps.stats[STAT_MAX_ARMOR] = 400;
		}
		break; 
		case ARMOR9:
		{
			g_entities[0].armor = g_entities[0].max_armor = 450;
			g_entities[0].client->ps.stats[STAT_ARMOR] = g_entities[0].client->ps.stats[STAT_MAX_ARMOR] = 450;
		}
		break; 
		case ARMOR10:  
		{
			g_entities[0].armor = g_entities[0].max_armor = 500;
			g_entities[0].client->ps.stats[STAT_ARMOR] = g_entities[0].client->ps.stats[STAT_MAX_ARMOR] = 500;
		}
		break; 
		case ARMOR11:
		{
			g_entities[0].armor = g_entities[0].max_armor = 550;
			g_entities[0].client->ps.stats[STAT_ARMOR] = g_entities[0].client->ps.stats[STAT_MAX_ARMOR] = 550;
		}
		break; 
		case ARMOR12:  
		{
			g_entities[0].armor = g_entities[0].max_armor = 600;
			g_entities[0].client->ps.stats[STAT_ARMOR] = g_entities[0].client->ps.stats[STAT_MAX_ARMOR] = 600;
		}
		break; 
		case ARMOR13:
		{
			g_entities[0].armor = g_entities[0].max_armor = 650;
			g_entities[0].client->ps.stats[STAT_ARMOR] = g_entities[0].client->ps.stats[STAT_MAX_ARMOR] = 650;
		}
		break; 
		case ARMOR14:  
		{
			g_entities[0].armor = g_entities[0].max_armor = 700;
			g_entities[0].client->ps.stats[STAT_ARMOR] = g_entities[0].client->ps.stats[STAT_MAX_ARMOR] = 700;
		}
		break; 
		case ARMOR15:
		{
			g_entities[0].armor = g_entities[0].max_armor = 750;
			g_entities[0].client->ps.stats[STAT_ARMOR] = g_entities[0].client->ps.stats[STAT_MAX_ARMOR] = 750;
		}
		break; 
		case ARMOR16:  
		{
			g_entities[0].armor = g_entities[0].max_armor = 800;
			g_entities[0].client->ps.stats[STAT_ARMOR] = g_entities[0].client->ps.stats[STAT_MAX_ARMOR] = 800;
		}
		break; 
		case ARMOR17:
		{
			g_entities[0].armor = g_entities[0].max_armor = 850;
			g_entities[0].client->ps.stats[STAT_ARMOR] = g_entities[0].client->ps.stats[STAT_MAX_ARMOR] = 850;
		}
		break; 
		case ARMOR18:  
		{
			g_entities[0].armor = g_entities[0].max_armor = 900;
			g_entities[0].client->ps.stats[STAT_ARMOR] = g_entities[0].client->ps.stats[STAT_MAX_ARMOR] = 900;
		}
		break; 
		case ARMOR19:
		{
			g_entities[0].armor = g_entities[0].max_armor = 950;
			g_entities[0].client->ps.stats[STAT_ARMOR] = g_entities[0].client->ps.stats[STAT_MAX_ARMOR] = 950;
		}
		break; 
		case ARMOR20:  
		{
			g_entities[0].armor = g_entities[0].max_armor = 999;
			g_entities[0].client->ps.stats[STAT_ARMOR] = g_entities[0].client->ps.stats[STAT_MAX_ARMOR] = 999;
		}
		break; 
 	}
}


static void Svcmd_Saber_f()
{
	const char *saber = gi.argv(1);
	const char *saber2 = gi.argv(2);

	if ( !g_entities[0].client || !saber || !saber[0] )
	{
		return;
	}

	gi.cvar_set( "g_saber", saber );
	WP_SetSaber( &g_entities[0], 0, saber );
	if ( saber2 && saber2[0] && !(g_entities[0].client->ps.saber[0].saberFlags&SFL_TWO_HANDED) )
	{//want to use a second saber and first one is not twoHanded
		gi.cvar_set( "g_saber2", saber2 );
		WP_SetSaber( &g_entities[0], 1, saber2 );
	}
	else
	{
		gi.cvar_set( "g_saber2", "" );
		WP_RemoveSaber( &g_entities[0], 1 );
	}
}

static void Svcmd_SaberBlade_f()
{
	if ( gi.argc() < 2 )
	{
		gi.Printf( "USAGE: saberblade <sabernum> <bladenum> [0 = off, 1 = on, no arg = toggle]\n" );
		return;
	}
	if ( &g_entities[0] == NULL || &g_entities[0].client == NULL )
	{
		return;
	}
	int sabernum = atoi(gi.argv(1)) - 1;
	if ( sabernum < 0 || sabernum > 1 )
	{
		return;
	}
	if ( sabernum > 0 && !g_entities[0].client->ps.dualSabers )
	{
		return;
	}
	//FIXME: what if don't even have a single saber at all?
	int bladenum = atoi(gi.argv(2)) - 1;
	if ( bladenum < 0 || bladenum >= g_entities[0].client->ps.saber[sabernum].numBlades )
	{
		return;
	}
	qboolean turnOn;
	if ( gi.argc() > 2 )
	{//explicit
		turnOn = (atoi(gi.argv(3))!=0);
	}
	else
	{//toggle
		turnOn = (g_entities[0].client->ps.saber[sabernum].blade[bladenum].active==qfalse);
	}

	g_entities[0].client->ps.SaberBladeActivate( sabernum, bladenum, turnOn );
}

static void Svcmd_SaberColor_f()
{//FIXME: just list the colors, each additional listing sets that blade
	int saberNum = atoi(gi.argv(1));
	const char *color[MAX_BLADES];
	int bladeNum;

	for ( bladeNum = 0; bladeNum < MAX_BLADES; bladeNum++ )
	{
		color[bladeNum] = gi.argv(2+bladeNum);
	}

	if ( !VALIDSTRING( color ) || saberNum < 1 || saberNum > 2 )
	{
		gi.Printf( "Usage:  saberColor <saberNum> <blade1 color> <blade2 color> ... <blade8 color> \n" );
		gi.Printf( "valid saberNums:  1 or 2\n" );
		gi.Printf( "valid colors:  red, orange, yellow, green, blue, and purple\n" );

		return;
	}
	saberNum--;
	
	gentity_t *self = G_GetSelfForPlayerCmd();

	for ( bladeNum = 0; bladeNum < MAX_BLADES; bladeNum++ )
	{
		if ( !color[bladeNum] || !color[bladeNum][0] )
		{
			break;
		}
		else
		{
			self->client->ps.saber[saberNum].blade[bladeNum].color = TranslateSaberColor( color[bladeNum] );
		}
	}

	if ( saberNum == 0 )
	{
		gi.cvar_set( "g_saber_color", color[0] );
	}
	else if ( saberNum == 1 )
	{
		gi.cvar_set( "g_saber2_color", color[0] );
	}
}


static void Svcmd_Gun_f()
{



	const char *gun = gi.argv(1);
	const char *gun2 = gi.argv(2);

	if ( !g_entities[0].client || !gun || !gun[0] )
	{
		return;
	}

	gi.cvar_set( "g_gun", gun );
	WP_SetGun( &g_entities[0], 0, gun );
	// Only one for moment for test! 
	if ( gun2 && gun2[0] && !(g_entities[0].client->gs.gun[0].gunFlags3&GFL_TWOGUNS) )
	{//want to use a second saber and first one is not twoHanded
		gi.cvar_set( "g_gun2", gun2 );
		WP_SetGun( &g_entities[0], 1, gun2 );
	}
	else
	{
		gi.cvar_set( "g_gun2", "" );
		WP_RemoveGun( &g_entities[0], 1 );
	}
}



static void Svcmd_ForceJump_f( void )
{
	if ( !&g_entities[0] || !g_entities[0].client )
	{
		return;
	}
	if ( !g_cheats->integer ) 
	{
		gi.SendServerCommand( 0, "print \"Cheats are not enabled on this server.\n\"");
		return;
	}
	const char *newVal = gi.argv(1);
	if ( !VALIDSTRING( newVal ) )
	{
		gi.Printf( "Current forceJump level is %d\n", g_entities[0].client->ps.forcePowerLevel[FP_LEVITATION] );
		gi.Printf( "Usage:  setForceJump <level> (1 - 3)\n" );
		return;
	}
	int val = atoi(newVal);
	if ( val > FORCE_LEVEL_0 )
	{
		g_entities[0].client->ps.forcePowersKnown |= ( 1 << FP_LEVITATION );
	}
	else
	{
		g_entities[0].client->ps.forcePowersKnown &= ~( 1 << FP_LEVITATION );
	}
	g_entities[0].client->ps.forcePowerLevel[FP_LEVITATION] = val;
	if ( g_entities[0].client->ps.forcePowerLevel[FP_LEVITATION] < FORCE_LEVEL_0 )
	{
		g_entities[0].client->ps.forcePowerLevel[FP_LEVITATION] = FORCE_LEVEL_0;
	}
	else if ( g_entities[0].client->ps.forcePowerLevel[FP_LEVITATION] > FORCE_LEVEL_3 )
	{
		g_entities[0].client->ps.forcePowerLevel[FP_LEVITATION] = FORCE_LEVEL_3;
	}
}

static void Svcmd_SaberThrow_f( void )
{
	if ( !&g_entities[0] || !g_entities[0].client )
	{
		return;
	}
	if ( !g_cheats->integer ) 
	{
		gi.SendServerCommand( 0, "print \"Cheats are not enabled on this server.\n\"");
		return;
	}
	const char *newVal = gi.argv(1);
	if ( !VALIDSTRING( newVal ) )
	{
		gi.Printf( "Current saberThrow level is %d\n", g_entities[0].client->ps.forcePowerLevel[FP_SABERTHROW] );
		gi.Printf( "Usage:  setSaberThrow <level> (1 - 3)\n" );
		return;
	}
	int val = atoi(newVal);
	if ( val > FORCE_LEVEL_0 )
	{
		g_entities[0].client->ps.forcePowersKnown |= ( 1 << FP_SABERTHROW );
	}
	else
	{
		g_entities[0].client->ps.forcePowersKnown &= ~( 1 << FP_SABERTHROW );
	}
	g_entities[0].client->ps.forcePowerLevel[FP_SABERTHROW] = val;
	if ( g_entities[0].client->ps.forcePowerLevel[FP_SABERTHROW] < FORCE_LEVEL_0 )
	{
		g_entities[0].client->ps.forcePowerLevel[FP_SABERTHROW] = FORCE_LEVEL_0;
	}
	else if ( g_entities[0].client->ps.forcePowerLevel[FP_SABERTHROW] > FORCE_LEVEL_3 )
	{
		g_entities[0].client->ps.forcePowerLevel[FP_SABERTHROW] = FORCE_LEVEL_3;
	}
}

static void Svcmd_ForceHeal_f( void )
{
	if ( !&g_entities[0] || !g_entities[0].client )
	{
		return;
	}
	if ( !g_cheats->integer ) 
	{
		gi.SendServerCommand( 0, "print \"Cheats are not enabled on this server.\n\"");
		return;
	}
	const char *newVal = gi.argv(1);
	if ( !VALIDSTRING( newVal ) )
	{
		gi.Printf( "Current forceHeal level is %d\n", g_entities[0].client->ps.forcePowerLevel[FP_HEAL] );
		gi.Printf( "Usage:  forceHeal <level> (1 - 3)\n" );
		return;
	}
	int val = atoi(newVal);
	if ( val > FORCE_LEVEL_0 )
	{
		g_entities[0].client->ps.forcePowersKnown |= ( 1 << FP_HEAL );
	}
	else
	{
		g_entities[0].client->ps.forcePowersKnown &= ~( 1 << FP_HEAL );
	}
	g_entities[0].client->ps.forcePowerLevel[FP_HEAL] = val;
	if ( g_entities[0].client->ps.forcePowerLevel[FP_HEAL] < FORCE_LEVEL_0 )
	{
		g_entities[0].client->ps.forcePowerLevel[FP_HEAL] = FORCE_LEVEL_0;
	}
	else if ( g_entities[0].client->ps.forcePowerLevel[FP_HEAL] > FORCE_LEVEL_3 )
	{
		g_entities[0].client->ps.forcePowerLevel[FP_HEAL] = FORCE_LEVEL_3;
	}
}

static void Svcmd_ForcePush_f( void )
{
	if ( !&g_entities[0] || !g_entities[0].client )
	{
		return;
	}
	if ( !g_cheats->integer ) 
	{
		gi.SendServerCommand( 0, "print \"Cheats are not enabled on this server.\n\"");
		return;
	}
	const char *newVal = gi.argv(1);
	if ( !VALIDSTRING( newVal ) )
	{
		gi.Printf( "Current forcePush level is %d\n", g_entities[0].client->ps.forcePowerLevel[FP_PUSH] );
		gi.Printf( "Usage:  forcePush <level> (1 - 3)\n" );
		return;
	}
	int val = atoi(newVal);
	if ( val > FORCE_LEVEL_0 )
	{
		g_entities[0].client->ps.forcePowersKnown |= ( 1 << FP_PUSH );
	}
	else
	{
		g_entities[0].client->ps.forcePowersKnown &= ~( 1 << FP_PUSH );
	}
	g_entities[0].client->ps.forcePowerLevel[FP_PUSH] = val;
	if ( g_entities[0].client->ps.forcePowerLevel[FP_PUSH] < FORCE_LEVEL_0 )
	{
		g_entities[0].client->ps.forcePowerLevel[FP_PUSH] = FORCE_LEVEL_0;
	}
	else if ( g_entities[0].client->ps.forcePowerLevel[FP_PUSH] > FORCE_LEVEL_3 )
	{
		g_entities[0].client->ps.forcePowerLevel[FP_PUSH] = FORCE_LEVEL_3;
	}
}

static void Svcmd_ForcePull_f( void )
{
	if ( !&g_entities[0] || !g_entities[0].client )
	{
		return;
	}
	if ( !g_cheats->integer ) 
	{
		gi.SendServerCommand( 0, "print \"Cheats are not enabled on this server.\n\"");
		return;
	}
	const char *newVal = gi.argv(1);
	if ( !VALIDSTRING( newVal ) )
	{
		gi.Printf( "Current forcePull level is %d\n", g_entities[0].client->ps.forcePowerLevel[FP_PULL] );
		gi.Printf( "Usage:  forcePull <level> (1 - 3)\n" );
		return;
	}
	int val = atoi(newVal);
	if ( val > FORCE_LEVEL_0 )
	{
		g_entities[0].client->ps.forcePowersKnown |= ( 1 << FP_PULL );
	}
	else
	{
		g_entities[0].client->ps.forcePowersKnown &= ~( 1 << FP_PULL );
	}
	g_entities[0].client->ps.forcePowerLevel[FP_PULL] = val;
	if ( g_entities[0].client->ps.forcePowerLevel[FP_PULL] < FORCE_LEVEL_0 )
	{
		g_entities[0].client->ps.forcePowerLevel[FP_PULL] = FORCE_LEVEL_0;
	}
	else if ( g_entities[0].client->ps.forcePowerLevel[FP_PULL] > FORCE_LEVEL_3 )
	{
		g_entities[0].client->ps.forcePowerLevel[FP_PULL] = FORCE_LEVEL_3;
	}
}

static void Svcmd_ForceSpeed_f( void )
{
	if ( !&g_entities[0] || !g_entities[0].client )
	{
		return;
	}
	if ( !g_cheats->integer ) 
	{
		gi.SendServerCommand( 0, "print \"Cheats are not enabled on this server.\n\"");
		return;
	}
	const char *newVal = gi.argv(1);
	if ( !VALIDSTRING( newVal ) )
	{
		gi.Printf( "Current forceSpeed level is %d\n", g_entities[0].client->ps.forcePowerLevel[FP_SPEED] );
		gi.Printf( "Usage:  forceSpeed <level> (1 - 3)\n" );
		return;
	}
	int val = atoi(newVal);
	if ( val > FORCE_LEVEL_0 )
	{
		g_entities[0].client->ps.forcePowersKnown |= ( 1 << FP_SPEED );
	}
	else
	{
		g_entities[0].client->ps.forcePowersKnown &= ~( 1 << FP_SPEED );
	}
	g_entities[0].client->ps.forcePowerLevel[FP_SPEED] = val;
	if ( g_entities[0].client->ps.forcePowerLevel[FP_SPEED] < FORCE_LEVEL_0 )
	{
		g_entities[0].client->ps.forcePowerLevel[FP_SPEED] = FORCE_LEVEL_0;
	}
	else if ( g_entities[0].client->ps.forcePowerLevel[FP_SPEED] > FORCE_LEVEL_3 )
	{
		g_entities[0].client->ps.forcePowerLevel[FP_SPEED] = FORCE_LEVEL_3;
	}
}

static void Svcmd_ForceGrip_f( void )
{
	if ( !&g_entities[0] || !g_entities[0].client )
	{
		return;
	}
	if ( !g_cheats->integer ) 
	{
		gi.SendServerCommand( 0, "print \"Cheats are not enabled on this server.\n\"");
		return;
	}
	const char *newVal = gi.argv(1);
	if ( !VALIDSTRING( newVal ) )
	{
		gi.Printf( "Current forceGrip level is %d\n", g_entities[0].client->ps.forcePowerLevel[FP_GRIP] );
		gi.Printf( "Usage:  forceGrip <level> (1 - 3)\n" );
		return;
	}
	int val = atoi(newVal);
	if ( val > FORCE_LEVEL_0 )
	{
		g_entities[0].client->ps.forcePowersKnown |= ( 1 << FP_GRIP );
	}
	else
	{
		g_entities[0].client->ps.forcePowersKnown &= ~( 1 << FP_GRIP );
	}
	g_entities[0].client->ps.forcePowerLevel[FP_GRIP] = val;
	if ( g_entities[0].client->ps.forcePowerLevel[FP_GRIP] < FORCE_LEVEL_0 )
	{
		g_entities[0].client->ps.forcePowerLevel[FP_GRIP] = FORCE_LEVEL_0;
	}
	else if ( g_entities[0].client->ps.forcePowerLevel[FP_GRIP] > FORCE_LEVEL_3 )
	{
		g_entities[0].client->ps.forcePowerLevel[FP_GRIP] = FORCE_LEVEL_3;
	}
}

static void Svcmd_ForceLightning_f( void )
{
	if ( !&g_entities[0] || !g_entities[0].client )
	{
		return;
	}
	if ( !g_cheats->integer ) 
	{
		gi.SendServerCommand( 0, "print \"Cheats are not enabled on this server.\n\"");
		return;
	}
	const char *newVal = gi.argv(1);
	if ( !VALIDSTRING( newVal ) )
	{
		gi.Printf( "Current forceLightning level is %d\n", g_entities[0].client->ps.forcePowerLevel[FP_LIGHTNING] );
		gi.Printf( "Usage:  forceLightning <level> (1 - 3)\n" );
		return;
	}
	int val = atoi(newVal);
	if ( val > FORCE_LEVEL_0 )
	{
		g_entities[0].client->ps.forcePowersKnown |= ( 1 << FP_LIGHTNING );
	}
	else
	{
		g_entities[0].client->ps.forcePowersKnown &= ~( 1 << FP_LIGHTNING );
	}
	g_entities[0].client->ps.forcePowerLevel[FP_LIGHTNING] = val;
	if ( g_entities[0].client->ps.forcePowerLevel[FP_LIGHTNING] < FORCE_LEVEL_0 )
	{
		g_entities[0].client->ps.forcePowerLevel[FP_LIGHTNING] = FORCE_LEVEL_0;
	}
	else if ( g_entities[0].client->ps.forcePowerLevel[FP_LIGHTNING] > FORCE_LEVEL_3 )
	{
		g_entities[0].client->ps.forcePowerLevel[FP_LIGHTNING] = FORCE_LEVEL_3;
	}
}

static void Svcmd_MindTrick_f( void )
{
	if ( !&g_entities[0] || !g_entities[0].client )
	{
		return;
	}
	if ( !g_cheats->integer ) 
	{
		gi.SendServerCommand( 0, "print \"Cheats are not enabled on this server.\n\"");
		return;
	}
	const char *newVal = gi.argv(1);
	if ( !VALIDSTRING( newVal ) )
	{
		gi.Printf( "Current mindTrick level is %d\n", g_entities[0].client->ps.forcePowerLevel[FP_TELEPATHY] );
		gi.Printf( "Usage:  mindTrick <level> (1 - 3)\n" );
		return;
	}
	int val = atoi(newVal);
	if ( val > FORCE_LEVEL_0 )
	{
		g_entities[0].client->ps.forcePowersKnown |= ( 1 << FP_TELEPATHY );
	}
	else
	{
		g_entities[0].client->ps.forcePowersKnown &= ~( 1 << FP_TELEPATHY );
	}
	g_entities[0].client->ps.forcePowerLevel[FP_TELEPATHY] = val;
	if ( g_entities[0].client->ps.forcePowerLevel[FP_TELEPATHY] < FORCE_LEVEL_0 )
	{
		g_entities[0].client->ps.forcePowerLevel[FP_TELEPATHY] = FORCE_LEVEL_0;
	}
	else if ( g_entities[0].client->ps.forcePowerLevel[FP_TELEPATHY] > FORCE_LEVEL_4 )
	{
		g_entities[0].client->ps.forcePowerLevel[FP_TELEPATHY] = FORCE_LEVEL_4;
	}
}

static void Svcmd_ForceStun_f( void )
{
	if ( !&g_entities[0] || !g_entities[0].client )
	{
		return;
	}
	if ( !g_cheats->integer ) 
	{
		gi.SendServerCommand( 0, "print \"Cheats are not enabled on this server.\n\"");
		return;
	}
	const char *newVal = gi.argv(1);
	if ( !VALIDSTRING( newVal ) )
	{
		gi.Printf( "Current stun  level is %d\n", g_entities[0].client->ps.forcePowerLevel[FP_STUN] );
		gi.Printf( "Usage:  forcestun <level> (1 - 3)\n" );
		return;
	}
	int val = atoi(newVal);
	if ( val > FORCE_LEVEL_0 )
	{
		g_entities[0].client->ps.forcePowersKnown |= ( 1 << FP_STUN );
	}
	else
	{
		g_entities[0].client->ps.forcePowersKnown &= ~( 1 << FP_STUN );
	}
	g_entities[0].client->ps.forcePowerLevel[FP_STUN] = val;
	if ( g_entities[0].client->ps.forcePowerLevel[FP_STUN] < FORCE_LEVEL_0 )
	{
		g_entities[0].client->ps.forcePowerLevel[FP_STUN] = FORCE_LEVEL_0;
	}
	else if ( g_entities[0].client->ps.forcePowerLevel[FP_STUN] > FORCE_LEVEL_4 )
	{
		g_entities[0].client->ps.forcePowerLevel[FP_STUN] = FORCE_LEVEL_4;
	}
}

static void Svcmd_ForceHate_f( void )
{
	if ( !&g_entities[0] || !g_entities[0].client )
	{
		return;
	}
	if ( !g_cheats->integer ) 
	{
		gi.SendServerCommand( 0, "print \"Cheats are not enabled on this server.\n\"");
		return;
	}
	const char *newVal = gi.argv(1);
	if ( !VALIDSTRING( newVal ) )
	{
		gi.Printf( "Current Inspire Hate level is %d\n", g_entities[0].client->ps.forcePowerLevel[FP_TELEPATHY] );
		gi.Printf( "Usage:  InspireHate <level> (1 - 3)\n" );
		return;
	}
	int val = atoi(newVal);
	if ( val > FORCE_LEVEL_0 )
	{
		g_entities[0].client->ps.forcePowersKnown |= ( 1 << FP_HATE );
	}
	else
	{
		g_entities[0].client->ps.forcePowersKnown &= ~( 1 << FP_HATE );
	}
	g_entities[0].client->ps.forcePowerLevel[FP_HATE] = val;
	if ( g_entities[0].client->ps.forcePowerLevel[FP_HATE] < FORCE_LEVEL_0 )
	{
		g_entities[0].client->ps.forcePowerLevel[FP_HATE] = FORCE_LEVEL_0;
	}
	else if ( g_entities[0].client->ps.forcePowerLevel[FP_HATE] > FORCE_LEVEL_4 )
	{
		g_entities[0].client->ps.forcePowerLevel[FP_HATE] = FORCE_LEVEL_4;
	}
}

static void Svcmd_ControlMind_f( void )
{
	if ( !&g_entities[0] || !g_entities[0].client )
	{
		return;
	}
	if ( !g_cheats->integer ) 
	{
		gi.SendServerCommand( 0, "print \"Cheats are not enabled on this server.\n\"");
		return;
	}
	const char *newVal = gi.argv(1);
	if ( !VALIDSTRING( newVal ) )
	{
		gi.Printf( "Current Control Mind level is %d\n", g_entities[0].client->ps.forcePowerLevel[FP_TELEPATHY] );
		gi.Printf( "Usage:  ControlMind <level> (1 - 3)\n" );
		return;
	}
	int val = atoi(newVal);
	if ( val > FORCE_LEVEL_0 )
	{
		g_entities[0].client->ps.forcePowersKnown |= ( 1 << FP_CONTROLMIND );
	}
	else
	{
		g_entities[0].client->ps.forcePowersKnown &= ~( 1 << FP_CONTROLMIND );
	}
	g_entities[0].client->ps.forcePowerLevel[FP_CONTROLMIND] = val;
	if ( g_entities[0].client->ps.forcePowerLevel[FP_CONTROLMIND] < FORCE_LEVEL_0 )
	{
		g_entities[0].client->ps.forcePowerLevel[FP_CONTROLMIND] = FORCE_LEVEL_0;
	}
	else if ( g_entities[0].client->ps.forcePowerLevel[FP_CONTROLMIND] > FORCE_LEVEL_4 )
	{
		g_entities[0].client->ps.forcePowerLevel[FP_CONTROLMIND] = FORCE_LEVEL_4;
	}
}

static void Svcmd_ForceFreeze_f( void )
{
	if ( !&g_entities[0] || !g_entities[0].client )
	{
		return;
	}
	if ( !g_cheats->integer ) 
	{
		gi.SendServerCommand( 0, "print \"Cheats are not enabled on this server.\n\"");
		return;
	}
	const char *newVal = gi.argv(1);
	if ( !VALIDSTRING( newVal ) )
	{
		gi.Printf( "Current Incapacitate level is %d\n", g_entities[0].client->ps.forcePowerLevel[FP_TELEPATHY] );
		gi.Printf( "Usage:  Incapacitate <level> (1 - 3)\n" );
		return;
	}
	int val = atoi(newVal);
	if ( val > FORCE_LEVEL_0 )
	{
		g_entities[0].client->ps.forcePowersKnown |= ( 1 << FP_FREEZE );
	}
	else
	{
		g_entities[0].client->ps.forcePowersKnown &= ~( 1 << FP_FREEZE );
	}
	g_entities[0].client->ps.forcePowerLevel[FP_FREEZE] = val;
	if ( g_entities[0].client->ps.forcePowerLevel[FP_FREEZE] < FORCE_LEVEL_0 )
	{
		g_entities[0].client->ps.forcePowerLevel[FP_FREEZE] = FORCE_LEVEL_0;
	}
	else if ( g_entities[0].client->ps.forcePowerLevel[FP_FREEZE] > FORCE_LEVEL_4 )
	{
		g_entities[0].client->ps.forcePowerLevel[FP_FREEZE] = FORCE_LEVEL_4;
	}
}

static void Svcmd_ForceHealOther_f( void )
{
	if ( !&g_entities[0] || !g_entities[0].client )
	{
		return;
	}
	if ( !g_cheats->integer ) 
	{
		gi.SendServerCommand( 0, "print \"Cheats are not enabled on this server.\n\"");
		return;
	}
	const char *newVal = gi.argv(1);
	if ( !VALIDSTRING( newVal ) )
	{
		gi.Printf( "Current HealOther level is %d\n", g_entities[0].client->ps.forcePowerLevel[FP_HEALOTHER] );
		gi.Printf( "Usage:  HealOther <level> (1 - 3)\n" );
		return;
	}
	int val = atoi(newVal);
	if ( val > FORCE_LEVEL_0 )
	{
		g_entities[0].client->ps.forcePowersKnown |= ( 1 << FP_HEALOTHER );
	}
	else
	{
		g_entities[0].client->ps.forcePowersKnown &= ~( 1 << FP_HEALOTHER );
	}
	g_entities[0].client->ps.forcePowerLevel[FP_HEALOTHER] = val;
	if ( g_entities[0].client->ps.forcePowerLevel[FP_HEALOTHER] < FORCE_LEVEL_0 )
	{
		g_entities[0].client->ps.forcePowerLevel[FP_HEALOTHER] = FORCE_LEVEL_0;
	}
	else if ( g_entities[0].client->ps.forcePowerLevel[FP_HEALOTHER] > FORCE_LEVEL_4 )
	{
		g_entities[0].client->ps.forcePowerLevel[FP_HEALOTHER] = FORCE_LEVEL_4;
	}
}

static void Svcmd_ForceWrack_f( void )
{
	if ( !&g_entities[0] || !g_entities[0].client )
	{
		return;
	}
	if ( !g_cheats->integer ) 
	{
		gi.SendServerCommand( 0, "print \"Cheats are not enabled on this server.\n\"");
		return;
	}
	const char *newVal = gi.argv(1);
	if ( !VALIDSTRING( newVal ) )
	{
		gi.Printf( "Current Spirith Wrack level is %d\n", g_entities[0].client->ps.forcePowerLevel[FP_WRACK] );
		gi.Printf( "Usage:  ForceWrack <level> (1 - 3)\n" );
		return;
	}
	int val = atoi(newVal);
	if ( val > FORCE_LEVEL_0 )
	{
		g_entities[0].client->ps.forcePowersKnown |= ( 1 << FP_WRACK );
	}
	else
	{
		g_entities[0].client->ps.forcePowersKnown &= ~( 1 << FP_WRACK );
	}
	g_entities[0].client->ps.forcePowerLevel[FP_WRACK] = val;
	if ( g_entities[0].client->ps.forcePowerLevel[FP_WRACK] < FORCE_LEVEL_0 )
	{
		g_entities[0].client->ps.forcePowerLevel[FP_WRACK] = FORCE_LEVEL_0;
	}
	else if ( g_entities[0].client->ps.forcePowerLevel[FP_WRACK] > FORCE_LEVEL_4 )
	{
		g_entities[0].client->ps.forcePowerLevel[FP_WRACK] = FORCE_LEVEL_4;
	}
}

static void Svcmd_ForceGlyph_f( void )
{
	if ( !&g_entities[0] || !g_entities[0].client )
	{
		return;
	}
	if ( !g_cheats->integer ) 
	{
		gi.SendServerCommand( 0, "print \"Cheats are not enabled on this server.\n\"");
		return;
	}
	const char *newVal = gi.argv(1);
	if ( !VALIDSTRING( newVal ) )
	{
		gi.Printf( "Current force glyph level is %d\n", g_entities[0].client->ps.forcePowerLevel[FP_FORCEGLYPH] );
		gi.Printf( "Usage:  ForceGlyph <level> (1 - 3)\n" );
		return;
	}
	int val = atoi(newVal);
	if ( val > FORCE_LEVEL_0 )
	{
		g_entities[0].client->ps.forcePowersKnown |= ( 1 << FP_FORCEGLYPH );
	}
	else
	{
		g_entities[0].client->ps.forcePowersKnown &= ~( 1 << FP_FORCEGLYPH );
	}
	g_entities[0].client->ps.forcePowerLevel[FP_FORCEGLYPH] = val;
	if ( g_entities[0].client->ps.forcePowerLevel[FP_FORCEGLYPH] < FORCE_LEVEL_0 )
	{
		g_entities[0].client->ps.forcePowerLevel[FP_FORCEGLYPH] = FORCE_LEVEL_0;
	}
	else if ( g_entities[0].client->ps.forcePowerLevel[FP_FORCEGLYPH] > FORCE_LEVEL_4 )
	{
		g_entities[0].client->ps.forcePowerLevel[FP_FORCEGLYPH] = FORCE_LEVEL_4;
	}
}

static void Svcmd_StoneGlyph_f( void )
{
	if ( !&g_entities[0] || !g_entities[0].client )
	{
		return;
	}
	if ( !g_cheats->integer ) 
	{
		gi.SendServerCommand( 0, "print \"Cheats are not enabled on this server.\n\"");
		return;
	}
	const char *newVal = gi.argv(1);
	if ( !VALIDSTRING( newVal ) )
	{
		gi.Printf( "Current Stone Glyph  level is %d\n", g_entities[0].client->ps.forcePowerLevel[FP_STONEGLYPH] );
		gi.Printf( "Usage:  stoneglyph <level> (1 - 3)\n" );
		return;
	}
	int val = atoi(newVal);
	if ( val > FORCE_LEVEL_0 )
	{
		g_entities[0].client->ps.forcePowersKnown |= ( 1 << FP_STONEGLYPH );
	}
	else
	{
		g_entities[0].client->ps.forcePowersKnown &= ~( 1 << FP_STONEGLYPH );
	}
	g_entities[0].client->ps.forcePowerLevel[FP_STONEGLYPH] = val;
	if ( g_entities[0].client->ps.forcePowerLevel[FP_STONEGLYPH] < FORCE_LEVEL_0 )
	{
		g_entities[0].client->ps.forcePowerLevel[FP_STONEGLYPH] = FORCE_LEVEL_0;
	}
	else if ( g_entities[0].client->ps.forcePowerLevel[FP_STONEGLYPH] > FORCE_LEVEL_4 )
	{
		g_entities[0].client->ps.forcePowerLevel[FP_STONEGLYPH] = FORCE_LEVEL_4;
	}
}

static void Svcmd_SoundGlyph_f( void )
{
	if ( !&g_entities[0] || !g_entities[0].client )
	{
		return;
	}
	if ( !g_cheats->integer ) 
	{
		gi.SendServerCommand( 0, "print \"Cheats are not enabled on this server.\n\"");
		return;
	}
	const char *newVal = gi.argv(1);
	if ( !VALIDSTRING( newVal ) )
	{
		gi.Printf( "Current Sound Glyph level is %d\n", g_entities[0].client->ps.forcePowerLevel[FP_SOUNDGLYPH] );
		gi.Printf( "Usage:  SoundGlyph <level> (1 - 3)\n" );
		return;
	}
	int val = atoi(newVal);
	if ( val > FORCE_LEVEL_0 )
	{
		g_entities[0].client->ps.forcePowersKnown |= ( 1 << FP_SOUNDGLYPH );
	}
	else
	{
		g_entities[0].client->ps.forcePowersKnown &= ~( 1 << FP_SOUNDGLYPH );
	}
	g_entities[0].client->ps.forcePowerLevel[FP_SOUNDGLYPH] = val;
	if ( g_entities[0].client->ps.forcePowerLevel[FP_SOUNDGLYPH] < FORCE_LEVEL_0 )
	{
		g_entities[0].client->ps.forcePowerLevel[FP_SOUNDGLYPH] = FORCE_LEVEL_0;
	}
	else if ( g_entities[0].client->ps.forcePowerLevel[FP_SOUNDGLYPH] > FORCE_LEVEL_4 )
	{
		g_entities[0].client->ps.forcePowerLevel[FP_SOUNDGLYPH] = FORCE_LEVEL_4;
	}
}

static void Svcmd_WaterGlyph_f( void )
{
	if ( !&g_entities[0] || !g_entities[0].client )
	{
		return;
	}
	if ( !g_cheats->integer ) 
	{
		gi.SendServerCommand( 0, "print \"Cheats are not enabled on this server.\n\"");
		return;
	}
	const char *newVal = gi.argv(1);
	if ( !VALIDSTRING( newVal ) )
	{
		gi.Printf( "Current Water Glyph level is %d\n", g_entities[0].client->ps.forcePowerLevel[FP_WATERGLYPH] );
		gi.Printf( "Usage:  WaterGlyph <level> (1 - 3)\n" );
		return;
	}
	int val = atoi(newVal);
	if ( val > FORCE_LEVEL_0 )
	{
		g_entities[0].client->ps.forcePowersKnown |= ( 1 << FP_WATERGLYPH );
	}
	else
	{
		g_entities[0].client->ps.forcePowersKnown &= ~( 1 << FP_WATERGLYPH );
	}
	g_entities[0].client->ps.forcePowerLevel[FP_WATERGLYPH] = val;
	if ( g_entities[0].client->ps.forcePowerLevel[FP_WATERGLYPH] < FORCE_LEVEL_0 )
	{
		g_entities[0].client->ps.forcePowerLevel[FP_WATERGLYPH] = FORCE_LEVEL_0;
	}
	else if ( g_entities[0].client->ps.forcePowerLevel[FP_WATERGLYPH] > FORCE_LEVEL_4 )
	{
		g_entities[0].client->ps.forcePowerLevel[FP_WATERGLYPH] = FORCE_LEVEL_4;
	}
}

static void Svcmd_FireGlyph_f( void )
{
	if ( !&g_entities[0] || !g_entities[0].client )
	{
		return;
	}
	if ( !g_cheats->integer ) 
	{
		gi.SendServerCommand( 0, "print \"Cheats are not enabled on this server.\n\"");
		return;
	}
	const char *newVal = gi.argv(1);
	if ( !VALIDSTRING( newVal ) )
	{
		gi.Printf( "Current Fire Glyph level is %d\n", g_entities[0].client->ps.forcePowerLevel[FP_FIREGLYPH] );
		gi.Printf( "Usage:  FireGlyph <level> (1 - 3)\n" );
		return;
	}
	int val = atoi(newVal);
	if ( val > FORCE_LEVEL_0 )
	{
		g_entities[0].client->ps.forcePowersKnown |= ( 1 << FP_FIREGLYPH );
	}
	else
	{
		g_entities[0].client->ps.forcePowersKnown &= ~( 1 << FP_FIREGLYPH );
	}
	g_entities[0].client->ps.forcePowerLevel[FP_FIREGLYPH] = val;
	if ( g_entities[0].client->ps.forcePowerLevel[FP_FIREGLYPH] < FORCE_LEVEL_0 )
	{
		g_entities[0].client->ps.forcePowerLevel[FP_FIREGLYPH] = FORCE_LEVEL_0;
	}
	else if ( g_entities[0].client->ps.forcePowerLevel[FP_FIREGLYPH] > FORCE_LEVEL_4 )
	{
		g_entities[0].client->ps.forcePowerLevel[FP_FIREGLYPH] = FORCE_LEVEL_4;
	}
}

static void Svcmd_SunGlyph_f( void )
{
	if ( !&g_entities[0] || !g_entities[0].client )
	{
		return;
	}
	if ( !g_cheats->integer ) 
	{
		gi.SendServerCommand( 0, "print \"Cheats are not enabled on this server.\n\"");
		return;
	}
	const char *newVal = gi.argv(1);
	if ( !VALIDSTRING( newVal ) )
	{
		gi.Printf( "Current Sunlight Glyph level is %d\n", g_entities[0].client->ps.forcePowerLevel[FP_SUNGLYPH] );
		gi.Printf( "Usage:  SunGlyph <level> (1 - 3)\n" );
		return;
	}
	int val = atoi(newVal);
	if ( val > FORCE_LEVEL_0 )
	{
		g_entities[0].client->ps.forcePowersKnown |= ( 1 << FP_SUNGLYPH );
	}
	else
	{
		g_entities[0].client->ps.forcePowersKnown &= ~( 1 << FP_SUNGLYPH );
	}
	g_entities[0].client->ps.forcePowerLevel[FP_SUNGLYPH] = val;
	if ( g_entities[0].client->ps.forcePowerLevel[FP_SUNGLYPH] < FORCE_LEVEL_0 )
	{
		g_entities[0].client->ps.forcePowerLevel[FP_SUNGLYPH] = FORCE_LEVEL_0;
	}
	else if ( g_entities[0].client->ps.forcePowerLevel[FP_SUNGLYPH] > FORCE_LEVEL_4 )
	{
		g_entities[0].client->ps.forcePowerLevel[FP_SUNGLYPH] = FORCE_LEVEL_4;
	}
}

static void Svcmd_WindGlyph_f( void )
{
	if ( !&g_entities[0] || !g_entities[0].client )
	{
		return;
	}
	if ( !g_cheats->integer ) 
	{
		gi.SendServerCommand( 0, "print \"Cheats are not enabled on this server.\n\"");
		return;
	}
	const char *newVal = gi.argv(1);
	if ( !VALIDSTRING( newVal ) )
	{
		gi.Printf( "Current Wind Glyph level is %d\n", g_entities[0].client->ps.forcePowerLevel[FP_WINDGLYPH] );
		gi.Printf( "Usage:  WindGlyph <level> (1 - 3)\n" );
		return;
	}
	int val = atoi(newVal);
	if ( val > FORCE_LEVEL_0 )
	{
		g_entities[0].client->ps.forcePowersKnown |= ( 1 << FP_WINDGLYPH );
	}
	else
	{
		g_entities[0].client->ps.forcePowersKnown &= ~( 1 << FP_WINDGLYPH );
	}
	g_entities[0].client->ps.forcePowerLevel[FP_WINDGLYPH] = val;
	if ( g_entities[0].client->ps.forcePowerLevel[FP_WINDGLYPH] < FORCE_LEVEL_0 )
	{
		g_entities[0].client->ps.forcePowerLevel[FP_WINDGLYPH] = FORCE_LEVEL_0;
	}
	else if ( g_entities[0].client->ps.forcePowerLevel[FP_WINDGLYPH] > FORCE_LEVEL_4 )
	{
		g_entities[0].client->ps.forcePowerLevel[FP_WINDGLYPH] = FORCE_LEVEL_4;
	}
}

static void Svcmd_HolyGlyph_f( void )
{
	if ( !&g_entities[0] || !g_entities[0].client )
	{
		return;
	}
	if ( !g_cheats->integer ) 
	{
		gi.SendServerCommand( 0, "print \"Cheats are not enabled on this server.\n\"");
		return;
	}
	const char *newVal = gi.argv(1);
	if ( !VALIDSTRING( newVal ) )
	{
		gi.Printf( "Current HolyGlyph level is %d\n", g_entities[0].client->ps.forcePowerLevel[FP_HOLYGLYPH] );
		gi.Printf( "Usage:  HolyGlyph <level> (1 - 3)\n" );
		return;
	}
	int val = atoi(newVal);
	if ( val > FORCE_LEVEL_0 )
	{
		g_entities[0].client->ps.forcePowersKnown |= ( 1 << FP_HOLYGLYPH );
	}
	else
	{
		g_entities[0].client->ps.forcePowersKnown &= ~( 1 << FP_HOLYGLYPH );
	}
	g_entities[0].client->ps.forcePowerLevel[FP_HOLYGLYPH] = val;
	if ( g_entities[0].client->ps.forcePowerLevel[FP_HOLYGLYPH] < FORCE_LEVEL_0 )
	{
		g_entities[0].client->ps.forcePowerLevel[FP_HOLYGLYPH] = FORCE_LEVEL_0;
	}
	else if ( g_entities[0].client->ps.forcePowerLevel[FP_HOLYGLYPH] > FORCE_LEVEL_4 )
	{
		g_entities[0].client->ps.forcePowerLevel[FP_HOLYGLYPH] = FORCE_LEVEL_4;
	}
}

static void Svcmd_DarkGlyph_f( void )
{
	if ( !&g_entities[0] || !g_entities[0].client )
	{
		return;
	}
	if ( !g_cheats->integer ) 
	{
		gi.SendServerCommand( 0, "print \"Cheats are not enabled on this server.\n\"");
		return;
	}
	const char *newVal = gi.argv(1);
	if ( !VALIDSTRING( newVal ) )
	{
		gi.Printf( "Current Darkness level is %d\n", g_entities[0].client->ps.forcePowerLevel[FP_DARKGLYPH] );
		gi.Printf( "Usage:  DarkGlyph <level> (1 - 3)\n" );
		return;
	}
	int val = atoi(newVal);
	if ( val > FORCE_LEVEL_0 )
	{
		g_entities[0].client->ps.forcePowersKnown |= ( 1 << FP_DARKGLYPH );
	}
	else
	{
		g_entities[0].client->ps.forcePowersKnown &= ~( 1 << FP_DARKGLYPH );
	}
	g_entities[0].client->ps.forcePowerLevel[FP_DARKGLYPH] = val;
	if ( g_entities[0].client->ps.forcePowerLevel[FP_DARKGLYPH] < FORCE_LEVEL_0 )
	{
		g_entities[0].client->ps.forcePowerLevel[FP_DARKGLYPH] = FORCE_LEVEL_0;
	}
	else if ( g_entities[0].client->ps.forcePowerLevel[FP_DARKGLYPH] > FORCE_LEVEL_4 )
	{
		g_entities[0].client->ps.forcePowerLevel[FP_DARKGLYPH] = FORCE_LEVEL_4;
	}
}

static void Svcmd_NecroGlyph_f( void )
{
	if ( !&g_entities[0] || !g_entities[0].client )
	{
		return;
	}
	if ( !g_cheats->integer ) 
	{
		gi.SendServerCommand( 0, "print \"Cheats are not enabled on this server.\n\"");
		return;
	}
	const char *newVal = gi.argv(1);
	if ( !VALIDSTRING( newVal ) )
	{
		gi.Printf( "Current Necro level is %d\n", g_entities[0].client->ps.forcePowerLevel[FP_NECROGLYPH] );
		gi.Printf( "Usage:  NecroGlyph <level> (1 - 3)\n" );
		return;
	}
	int val = atoi(newVal);
	if ( val > FORCE_LEVEL_0 )
	{
		g_entities[0].client->ps.forcePowersKnown |= ( 1 << FP_NECROGLYPH );
	}
	else
	{
		g_entities[0].client->ps.forcePowersKnown &= ~( 1 << FP_NECROGLYPH );
	}
	g_entities[0].client->ps.forcePowerLevel[FP_NECROGLYPH] = val;
	if ( g_entities[0].client->ps.forcePowerLevel[FP_NECROGLYPH] < FORCE_LEVEL_0 )
	{
		g_entities[0].client->ps.forcePowerLevel[FP_NECROGLYPH] = FORCE_LEVEL_0;
	}
	else if ( g_entities[0].client->ps.forcePowerLevel[FP_NECROGLYPH] > FORCE_LEVEL_4 )
	{
		g_entities[0].client->ps.forcePowerLevel[FP_NECROGLYPH] = FORCE_LEVEL_4;
	}
}

static void Svcmd_SaberDefense_f( void )
{
	if ( !&g_entities[0] || !g_entities[0].client )
	{
		return;
	}
	if ( !g_cheats->integer ) 
	{
		gi.SendServerCommand( 0, "print \"Cheats are not enabled on this server.\n\"");
		return;
	}
	const char *newVal = gi.argv(1);
	if ( !VALIDSTRING( newVal ) )
	{
		gi.Printf( "Current saberDefense level is %d\n", g_entities[0].client->ps.forcePowerLevel[FP_SABER_DEFENSE] );
		gi.Printf( "Usage:  saberDefense <level> (1 - 3)\n" );
		return;
	}
	int val = atoi(newVal);
	if ( val > FORCE_LEVEL_0 )
	{
		g_entities[0].client->ps.forcePowersKnown |= ( 1 << FP_SABER_DEFENSE );
	}
	else
	{
		g_entities[0].client->ps.forcePowersKnown &= ~( 1 << FP_SABER_DEFENSE );
	}
	g_entities[0].client->ps.forcePowerLevel[FP_SABER_DEFENSE] = val;
	if ( g_entities[0].client->ps.forcePowerLevel[FP_SABER_DEFENSE] < FORCE_LEVEL_0 )
	{
		g_entities[0].client->ps.forcePowerLevel[FP_SABER_DEFENSE] = FORCE_LEVEL_0;
	}
	else if ( g_entities[0].client->ps.forcePowerLevel[FP_SABER_DEFENSE] > FORCE_LEVEL_3 )
	{
		g_entities[0].client->ps.forcePowerLevel[FP_SABER_DEFENSE] = FORCE_LEVEL_3;
	}
}

static void Svcmd_SaberOffense_f( void )
{
	if ( !&g_entities[0] || !g_entities[0].client )
	{
		return;
	}
	if ( !g_cheats->integer ) 
	{
		gi.SendServerCommand( 0, "print \"Cheats are not enabled on this server.\n\"");
		return;
	}
	const char *newVal = gi.argv(1);
	if ( !VALIDSTRING( newVal ) )
	{
		gi.Printf( "Current saberOffense level is %d\n", g_entities[0].client->ps.forcePowerLevel[FP_SABER_OFFENSE] );
		gi.Printf( "Usage:  saberOffense <level> (1 - 3)\n" );
		return;
	}
	int val = atoi(newVal);
	if ( val > FORCE_LEVEL_0 )
	{
		g_entities[0].client->ps.forcePowersKnown |= ( 1 << FP_SABER_OFFENSE );
	}
	else
	{
		g_entities[0].client->ps.forcePowersKnown &= ~( 1 << FP_SABER_OFFENSE );
	}
	g_entities[0].client->ps.forcePowerLevel[FP_SABER_OFFENSE] = val;
	if ( g_entities[0].client->ps.forcePowerLevel[FP_SABER_OFFENSE] < FORCE_LEVEL_0 )
	{
		g_entities[0].client->ps.forcePowerLevel[FP_SABER_OFFENSE] = FORCE_LEVEL_0;
	}
	else if ( g_entities[0].client->ps.forcePowerLevel[FP_SABER_OFFENSE] >= SS_NUM_SABER_STYLES )
	{
		g_entities[0].client->ps.forcePowerLevel[FP_SABER_OFFENSE] = SS_NUM_SABER_STYLES-1;
	}
}

static void Svcmd_ForceSetLevel_f( int forcePower )
{
	if ( !&g_entities[0] || !g_entities[0].client )
	{
		return;
	}
	if ( !g_cheats->integer ) 
	{
		gi.SendServerCommand( 0, "print \"Cheats are not enabled on this server.\n\"");
		return;
	}
	const char *newVal = gi.argv(1);
	if ( !VALIDSTRING( newVal ) )
	{
		gi.Printf( "Current force level is %d\n", g_entities[0].client->ps.forcePowerLevel[forcePower] );
		gi.Printf( "Usage:  force <level> (1 - 3)\n" );
		return;
	}
	int val = atoi(newVal);
	if ( val > FORCE_LEVEL_0 )
	{
		g_entities[0].client->ps.forcePowersKnown |= ( 1 << forcePower );
	}
	else
	{
		g_entities[0].client->ps.forcePowersKnown &= ~( 1 << forcePower );
	}
	g_entities[0].client->ps.forcePowerLevel[forcePower] = val;
	if ( g_entities[0].client->ps.forcePowerLevel[forcePower] < FORCE_LEVEL_0 )
	{
		g_entities[0].client->ps.forcePowerLevel[forcePower] = FORCE_LEVEL_0;
	}
	else if ( g_entities[0].client->ps.forcePowerLevel[forcePower] > FORCE_LEVEL_3 )
	{
		g_entities[0].client->ps.forcePowerLevel[forcePower] = FORCE_LEVEL_3;
	}
}

extern qboolean PM_SaberInStart( int move );
extern qboolean PM_SaberInTransition( int move );
extern qboolean PM_SaberInAttack( int move );
extern qboolean WP_SaberCanTurnOffSomeBlades( saberInfo_t *saber );
void Svcmd_SaberAttackCycle_f( void )
{
	if ( !&g_entities[0] || !g_entities[0].client )
	{
		return;
	}

	gentity_t *self = G_GetSelfForPlayerCmd();
	if ( self->s.weapon != WP_SABER )
	{// saberAttackCycle button also switches to saber
		gi.SendConsoleCommand("weapon 1" );
		return;
	}

	if ( self->client->ps.dualSabers ) 
	{//can't cycle styles with dualSabers, so just toggle second saber on/off
		if ( WP_SaberCanTurnOffSomeBlades( &self->client->ps.saber[1] ) )
		{//can turn second saber off 
			if ( self->client->ps.saber[1].ActiveManualOnly() )
			{//turn it off
				qboolean skipThisBlade;
				for ( int bladeNum = 0; bladeNum < self->client->ps.saber[1].numBlades; bladeNum++ )
				{
					skipThisBlade = qfalse;
					if ( WP_SaberBladeUseSecondBladeStyle( &self->client->ps.saber[1], bladeNum ) )
					{//check to see if we should check the secondary style's flags
						if ( (self->client->ps.saber[1].saberFlags2&SFL2_NO_MANUAL_DEACTIVATE2) )
						{
							skipThisBlade = qtrue;
						}
					}
					else
					{//use the primary style's flags
						if ( (self->client->ps.saber[1].saberFlags2&SFL2_NO_MANUAL_DEACTIVATE) )
						{
							skipThisBlade = qtrue;
						}
					}
					if ( !skipThisBlade )
					{
						self->client->ps.saber[1].BladeActivate( bladeNum, qfalse );
						G_SoundIndexOnEnt( self, CHAN_WEAPON, self->client->ps.saber[1].soundOff );
					}
				}
			}
			else if ( !self->client->ps.saber[0].ActiveManualOnly() )
			{//first one is off, too, so just turn that one on
				if ( !self->client->ps.saberInFlight )
				{//but only if it's in your hand!
					self->client->ps.saber[0].Activate();
				}
			}
			else
			{//turn on the second one
				self->client->ps.saber[1].Activate();
			}
			return;
		}
	}
	else if ( self->client->ps.saber[0].numBlades > 1 
		&& WP_SaberCanTurnOffSomeBlades( &self->client->ps.saber[0] ) )//self->client->ps.saber[0].type == SABER_STAFF )
	{//can't cycle styles with saberstaff, so just toggles saber blades on/off
		if ( self->client->ps.saberInFlight )
		{//can't turn second blade back on if it's in the air, you naughty boy!
			return;
		}
		/*
		if ( self->client->ps.saber[0].singleBladeStyle == SS_NONE )
		{//can't use just one blade?
			return;
		}
		*/
		qboolean playedSound = qfalse;
		if ( !self->client->ps.saber[0].blade[0].active )
		{//first one is not even on
			//turn only it on
			self->client->ps.SaberBladeActivate( 0, 0, qtrue );
			return;
		}

		qboolean skipThisBlade;
		for ( int bladeNum = 1; bladeNum < self->client->ps.saber[0].numBlades; bladeNum++ )
		{
			if ( !self->client->ps.saber[0].blade[bladeNum].active )
			{//extra is off, turn it on
				self->client->ps.saber[0].BladeActivate( bladeNum, qtrue );
			}
			else
			{//turn extra off
				skipThisBlade = qfalse;
				if ( WP_SaberBladeUseSecondBladeStyle( &self->client->ps.saber[1], bladeNum ) )
				{//check to see if we should check the secondary style's flags
					if ( (self->client->ps.saber[1].saberFlags2&SFL2_NO_MANUAL_DEACTIVATE2) )
					{
						skipThisBlade = qtrue;
					}
				}
				else
				{//use the primary style's flags
					if ( (self->client->ps.saber[1].saberFlags2&SFL2_NO_MANUAL_DEACTIVATE) )
					{
						skipThisBlade = qtrue;
					}
				}
				if ( !skipThisBlade )
				{
					self->client->ps.saber[0].BladeActivate( bladeNum, qfalse );
					if ( !playedSound )
					{
						G_SoundIndexOnEnt( self, CHAN_WEAPON, self->client->ps.saber[0].soundOff );
						playedSound = qtrue;
					}
				}
			}
		}
		return;
	}

	int allowedStyles = self->client->ps.saberStylesKnown;
	if ( self->client->ps.dualSabers
		&& self->client->ps.saber[0].Active()
		&& self->client->ps.saber[1].Active() )
	{
		allowedStyles |= (1<<SS_DUAL);
		for ( int styleNum = SS_NONE+1; styleNum < SS_NUM_SABER_STYLES; styleNum++ )
		{ 
			if ( styleNum == SS_TAVION
				&& ((self->client->ps.saber[0].stylesLearned&(1<<SS_TAVION))||(self->client->ps.saber[1].stylesLearned&(1<<SS_TAVION)))//was given this style by one of my sabers
				&& !(self->client->ps.saber[0].stylesForbidden&(1<<SS_TAVION))
				&& !(self->client->ps.saber[1].stylesForbidden&(1<<SS_TAVION)) )
			{//if have both sabers on, allow tavion only if one of our sabers specifically wanted to use it... (unless specifically forbidden)
			}
			else if ( styleNum == SS_DUAL
				&& !(self->client->ps.saber[0].stylesForbidden&(1<<SS_DUAL))
				&& !(self->client->ps.saber[1].stylesForbidden&(1<<SS_DUAL)) )
			{//if have both sabers on, only dual style is allowed (unless specifically forbidden)
			}
			else
			{
				allowedStyles &= ~(1<<styleNum);
			}
		}
	}

	if ( !allowedStyles )
	{
		return;
	}

	int	saberAnimLevel;
	if ( !self->s.number )
	{
		saberAnimLevel = cg.saberAnimLevelPending;
	}
	else
	{
		saberAnimLevel = self->client->ps.saberAnimLevel;
	}
	saberAnimLevel++;
	int sanityCheck = 0;
	while ( self->client->ps.saberAnimLevel != saberAnimLevel 
		&& !(allowedStyles&(1<<saberAnimLevel))
		&& sanityCheck < SS_NUM_SABER_STYLES+1 )
	{
		saberAnimLevel++;
		if ( saberAnimLevel > SS_STAFF )
		{
			saberAnimLevel = SS_FAST;
		}
		sanityCheck++;
	}

	if ( !(allowedStyles&(1<<saberAnimLevel)) )
	{
		return;
	}

	WP_UseFirstValidSaberStyle( self, &saberAnimLevel );
	if ( !self->s.number )
	{
		cg.saberAnimLevelPending = saberAnimLevel;
	}
	else
	{
		self->client->ps.saberAnimLevel = saberAnimLevel;
	}

#ifndef FINAL_BUILD
	switch ( saberAnimLevel )
	{
	case SS_FAST:
		gi.Printf( S_COLOR_BLUE"Lightsaber Combat Style: Fast\n" );
		//LIGHTSABERCOMBATSTYLE_FAST
		break;
	case SS_MEDIUM:
		gi.Printf( S_COLOR_YELLOW"Lightsaber Combat Style: Medium\n" );
		//LIGHTSABERCOMBATSTYLE_MEDIUM
		break;
	case SS_STRONG:
		gi.Printf( S_COLOR_RED"Lightsaber Combat Style: Strong\n" );
		//LIGHTSABERCOMBATSTYLE_STRONG
		break;
	case SS_DESANN:
		gi.Printf( S_COLOR_CYAN"Lightsaber Combat Style: Desann\n" );
		//LIGHTSABERCOMBATSTYLE_DESANN
		break;
	case SS_TAVION:
		gi.Printf( S_COLOR_MAGENTA"Lightsaber Combat Style: Tavion\n" );
		//LIGHTSABERCOMBATSTYLE_TAVION
		break;
	case SS_DUAL:
		gi.Printf( S_COLOR_MAGENTA"Lightsaber Combat Style: Dual\n" );
		//LIGHTSABERCOMBATSTYLE_TAVION
		break;
	case SS_STAFF:
		gi.Printf( S_COLOR_MAGENTA"Lightsaber Combat Style: Staff\n" );
		//LIGHTSABERCOMBATSTYLE_TAVION
		break;
	}
	//gi.Printf("\n");
#endif
}

qboolean G_ReleaseEntity( gentity_t *grabber )
{
	if ( grabber && grabber->client && grabber->client->ps.heldClient < ENTITYNUM_WORLD )
	{
		gentity_t *heldClient = &g_entities[grabber->client->ps.heldClient];
		grabber->client->ps.heldClient = ENTITYNUM_NONE;
		if ( heldClient && heldClient->client )
		{
			heldClient->client->ps.heldByClient = ENTITYNUM_NONE;

			heldClient->owner = NULL;
		}
		return qtrue;
	}
	return qfalse;
}

void G_GrabEntity( gentity_t *grabber, const char *target )
{
	if ( !grabber || !grabber->client )
	{
		return;
	}
	gentity_t	*heldClient = G_Find( NULL, FOFS(targetname), (char *)target );
	if ( heldClient && heldClient->client && heldClient != grabber )//don't grab yourself, it's not polite
	{//found him
		grabber->client->ps.heldClient = heldClient->s.number;
		heldClient->client->ps.heldByClient = grabber->s.number;

		heldClient->owner = grabber;
	}
}

/*
=================
ConsoleCommand
// these are added in cg_main, CG_Init so they tab-complete
=================
*/
qboolean	ConsoleCommand( void ) {
	const char	*cmd;

	cmd = gi.argv(0);

	if ( Q_stricmp (cmd, "entitylist") == 0 ) 
	{
		Svcmd_EntityList_f();
		return qtrue;
	}

	if (Q_stricmp (cmd, "game_memory") == 0) {
		Svcmd_GameMem_f();
		return qtrue;
	}

//	if (Q_stricmp (cmd, "addbot") == 0) {
//		Svcmd_AddBot_f();
//		return qtrue;
//	}

	if (Q_stricmp (cmd, "nav") == 0) 
	{
		if ( !g_cheats->integer ) 
		{
			gi.SendServerCommand( 0, "print \"Cheats are not enabled on this server.\n\"");
			return qfalse;
		}
		Svcmd_Nav_f ();
		return qtrue;
	}

	if (Q_stricmp (cmd, "npc") == 0) 
	{
		if ( !g_cheats->integer ) 
		{
			gi.SendServerCommand( 0, "print \"Cheats are not enabled on this server.\n\"");
			return qfalse;
		}
		Svcmd_NPC_f ();
		return qtrue;
	}

	if (Q_stricmp (cmd, "use") == 0) 
	{
		if ( !g_cheats->integer ) 
		{
			gi.SendServerCommand( 0, "print \"Cheats are not enabled on this server.\n\"");
			return qfalse;
		}
		Svcmd_Use_f ();
		return qtrue;
	}

	if ( Q_stricmp( cmd, "ICARUS" ) == 0 )	
	{
		if ( !g_cheats->integer ) 
		{
			gi.SendServerCommand( 0, "print \"Cheats are not enabled on this server.\n\"");
			return qfalse;
		}

		Quake3Game()->Svcmd();

		return qtrue;
	}

	// RPG SYSTEM: 1 SET PLAYER LEVEL. - affect also mana point and mana regen)
	if ( Q_stricmp( cmd, "setplayerlevel" ) == 0 )	
	{
		Svcmd_SetPlayerLevel_f();
		return qtrue;
	}
	// RPG SYSTEM 2: SET PLAYER ARMOR 
	if ( Q_stricmp( cmd, "setplayerarmor" ) == 0 )	
	{
		Svcmd_SetPlayerArmor_f();
		return qtrue;
	}


	if ( Q_stricmp( cmd, "saberColor" ) == 0 )	
	{
		if ( !g_cheats->integer ) 
		{
			gi.SendServerCommand( 0, "print \"Cheats are not enabled on this server.\n\"");
			return qfalse;
		}
		Svcmd_SaberColor_f();
		return qtrue;
	}

	if ( Q_stricmp( cmd, "saber" ) == 0 )
	{
		if ( !g_cheats->integer ) 
		{
			gi.SendServerCommand( 0, "print \"Cheats are not enabled on this server.\n\"");
			return qfalse;
		}
		Svcmd_Saber_f();
		return qtrue;
	}

	if ( Q_stricmp( cmd, "saberblade" ) == 0 )
	{
		if ( !g_cheats->integer ) 
		{
			gi.SendServerCommand( 0, "print \"Cheats are not enabled on this server.\n\"");
			return qfalse;
		}
		Svcmd_SaberBlade_f();
		return qtrue;
	}
	// Give me a Custom Gun, thanks! 
	if ( Q_stricmp( cmd, "gun" ) == 0 )
	{
		if ( !g_cheats->integer ) 
		{
			gi.SendServerCommand( 0, "print \"Cheats are not enabled on this server.\n\"");
			return qfalse;
		}
		Svcmd_Gun_f();
		return qtrue;
	}

	if ( Q_stricmp( cmd, "setForceJump" ) == 0 )	
	{
		Svcmd_ForceJump_f();
		return qtrue;
	}
	if ( Q_stricmp( cmd, "setSaberThrow" ) == 0 )	
	{
		Svcmd_SaberThrow_f();
		return qtrue;
	}
	if ( Q_stricmp( cmd, "setForceHeal" ) == 0 )	
	{
		Svcmd_ForceHeal_f();
		return qtrue;
	}
	if ( Q_stricmp( cmd, "setForcePush" ) == 0 )	
	{
		Svcmd_ForcePush_f();
		return qtrue;
	}
	if ( Q_stricmp( cmd, "setForcePull" ) == 0 )	
	{
		Svcmd_ForcePull_f();
		return qtrue;
	}
	if ( Q_stricmp( cmd, "setForceSpeed" ) == 0 )	
	{
		Svcmd_ForceSpeed_f();
		return qtrue;
	}
	if ( Q_stricmp( cmd, "setForceGrip" ) == 0 )	
	{
		Svcmd_ForceGrip_f();
		return qtrue;
	}
	if ( Q_stricmp( cmd, "setForceLightning" ) == 0 )	
	{
		Svcmd_ForceLightning_f();
		return qtrue;
	}
	if ( Q_stricmp( cmd, "setMindTrick" ) == 0 )	
	{
		Svcmd_MindTrick_f();
		return qtrue;
	}
	if ( Q_stricmp( cmd, "setForceStun" ) == 0 )	
	{
		Svcmd_ForceStun_f();
		return qtrue;
	}
	if ( Q_stricmp( cmd, "setInspireHate" ) == 0 )	
	{
		Svcmd_ForceHate_f();
		return qtrue;
	}
	if ( Q_stricmp( cmd, "setControlMind" ) == 0 )	
	{
		Svcmd_ControlMind_f();
		return qtrue;
	}
	if ( Q_stricmp( cmd, "setIncapacitate" ) == 0 )	
	{
		Svcmd_ForceFreeze_f();
		return qtrue;
	}
	if ( Q_stricmp( cmd, "setforcehealother" ) == 0 )	
	{
		Svcmd_ForceHealOther_f();
		return qtrue;
	}
	if ( Q_stricmp( cmd, "setforcewrack" ) == 0 )	
	{
		Svcmd_ForceWrack_f();
		return qtrue;
	}

	if ( Q_stricmp( cmd, "setforceglyph" ) == 0 )	
	{
		Svcmd_ForceGlyph_f();
		return qtrue;
	}
	if ( Q_stricmp( cmd, "setstoneglyph" ) == 0 )	
	{
		Svcmd_StoneGlyph_f();
		return qtrue;
	}
	if ( Q_stricmp( cmd, "setsoundglyph" ) == 0 )	
	{
		Svcmd_SoundGlyph_f();
		return qtrue;
	}
	if ( Q_stricmp( cmd, "setwaterglyph" ) == 0 )	
	{
		Svcmd_WaterGlyph_f();
		return qtrue;
	}
	if ( Q_stricmp( cmd, "setfireglyph" ) == 0 )	
	{
		Svcmd_FireGlyph_f();
		return qtrue;
	}
	if ( Q_stricmp( cmd, "setsunglyph" ) == 0 )	
	{
		Svcmd_SunGlyph_f();
		return qtrue;
	}
	if ( Q_stricmp( cmd, "setwindglyph" ) == 0 )	
	{
		Svcmd_WindGlyph_f();
		return qtrue;
	}
	if ( Q_stricmp( cmd, "setholyglyph" ) == 0 )	
	{
		Svcmd_HolyGlyph_f();
		return qtrue;
	}
	if ( Q_stricmp( cmd, "setdarkglyph" ) == 0 )	
	{
		Svcmd_DarkGlyph_f();
		return qtrue;
	}
	if ( Q_stricmp( cmd, "setnecroglyph" ) == 0 )	
	{
		Svcmd_NecroGlyph_f();
		return qtrue;
	}
	if ( Q_stricmp( cmd, "setSaberDefense" ) == 0 )	
	{
		Svcmd_SaberDefense_f();
		return qtrue;
	}
	if ( Q_stricmp( cmd, "setSaberOffense" ) == 0 )	
	{
		Svcmd_SaberOffense_f();
		return qtrue;
	}
	if ( Q_stricmp( cmd, "setForceRage" ) == 0 )	
	{
		Svcmd_ForceSetLevel_f( FP_RAGE );
		return qtrue;
	}
	if ( Q_stricmp( cmd, "setForceDrain" ) == 0 )	
	{
		Svcmd_ForceSetLevel_f( FP_DRAIN );
		return qtrue;
	}
	if ( Q_stricmp( cmd, "setForceProtect" ) == 0 )	
	{
		Svcmd_ForceSetLevel_f( FP_PROTECT );
		return qtrue;
	}
	if ( Q_stricmp( cmd, "setForceAbsorb" ) == 0 )	
	{
		Svcmd_ForceSetLevel_f( FP_ABSORB );
		return qtrue;
	}
	if ( Q_stricmp( cmd, "setForceSight" ) == 0 )	
	{
		Svcmd_ForceSetLevel_f( FP_SEE );
		return qtrue;
	}
	if ( Q_stricmp( cmd, "setForceAll" ) == 0 )	
	{
		if ( !g_cheats->integer ) 
		{
			gi.SendServerCommand( 0, "print \"Cheats are not enabled on this server.\n\"");
			return qfalse;
		}

		Svcmd_ForceJump_f();
		Svcmd_SaberThrow_f();
		Svcmd_ForceHeal_f();
		Svcmd_ForcePush_f();
		Svcmd_ForcePull_f();
		Svcmd_ForceSpeed_f();
		Svcmd_ForceGrip_f();
		Svcmd_ForceLightning_f();
		Svcmd_MindTrick_f();
		Svcmd_ForceStun_f();
		Svcmd_ForceHate_f();
		Svcmd_ControlMind_f();
		Svcmd_ForceFreeze_f();
		Svcmd_ForceHealOther_f();
		Svcmd_ForceWrack_f();
		Svcmd_ForceGlyph_f();
		Svcmd_StoneGlyph_f();
		Svcmd_SoundGlyph_f();
		Svcmd_WaterGlyph_f();
		Svcmd_FireGlyph_f();
		Svcmd_SunGlyph_f();
		Svcmd_WindGlyph_f();
		Svcmd_DarkGlyph_f();
		Svcmd_HolyGlyph_f();
		Svcmd_NecroGlyph_f();
		Svcmd_SaberDefense_f();
		Svcmd_SaberOffense_f();
		Svcmd_ForceSetLevel_f( FP_RAGE );
		Svcmd_ForceSetLevel_f( FP_DRAIN );
		Svcmd_ForceSetLevel_f( FP_PROTECT );
		Svcmd_ForceSetLevel_f( FP_ABSORB );
		Svcmd_ForceSetLevel_f( FP_SEE );
		for ( int i = SS_NONE+1; i < SS_NUM_SABER_STYLES; i++ )
		{
			g_entities[0].client->ps.saberStylesKnown |= (1<<i);
		}
		return qtrue;
	}
	if ( Q_stricmp( cmd, "saberAttackCycle" ) == 0 )	
	{
		Svcmd_SaberAttackCycle_f();
		return qtrue;
	}
	if ( Q_stricmp( cmd, "runscript" ) == 0 ) 
	{
		if ( !g_cheats->integer ) 
		{
			gi.SendServerCommand( 0, "print \"Cheats are not enabled on this server.\n\"");
			return qfalse;
		}
		const char *cmd2 = gi.argv(1);

		if ( cmd2 && cmd2[0] )
		{
			const char *cmd3 = gi.argv(2);
			if ( cmd3 && cmd3[0] )
			{
				gentity_t *found = NULL;
				if ( (found = G_Find(NULL, FOFS(targetname), cmd2 ) ) != NULL )
				{
					Quake3Game()->RunScript( found, cmd3 );
				}
				else
				{
					//can't find cmd2
					gi.Printf( S_COLOR_RED"runscript: can't find targetname %s\n", cmd2 );
				}
			}
			else
			{
				Quake3Game()->RunScript( &g_entities[0], cmd2 );
			}
		}
		else
		{
			gi.Printf( S_COLOR_RED"usage: runscript <ent targetname> scriptname\n" );
		}
		//FIXME: else warning
		return qtrue;
	}

	if ( Q_stricmp( cmd, "playerteam" ) == 0 ) 
	{
		if ( !g_cheats->integer ) 
		{
			gi.SendServerCommand( 0, "print \"Cheats are not enabled on this server.\n\"");
			return qfalse;
		}
		const char	*cmd2 = gi.argv(1);
		int		n;

		if ( !*cmd2 || !cmd2[0] )
		{
			gi.Printf( S_COLOR_RED"'playerteam' - change player team, requires a team name!\n" );
			gi.Printf( S_COLOR_RED"Valid team names are:\n");
			for ( n = (TEAM_FREE + 1); n < TEAM_NUM_TEAMS; n++ )
			{
				gi.Printf( S_COLOR_RED"%s\n", GetStringForID( TeamTable, n ) );
			}
		}
		else
		{
			team_t	team;

			team = (team_t)GetIDForString( TeamTable, cmd2 );
			if ( team == -1 )
			{
				gi.Printf( S_COLOR_RED"'playerteam' unrecognized team name %s!\n", cmd2 );
				gi.Printf( S_COLOR_RED"Valid team names are:\n");
				for ( n = TEAM_FREE; n < TEAM_NUM_TEAMS; n++ )
				{
					gi.Printf( S_COLOR_RED"%s\n", GetStringForID( TeamTable, n ) );
				}
			}
			else
			{
				g_entities[0].client->playerTeam = team;
				//FIXME: convert Imperial, Malon, Hirogen and Klingon to Scavenger?
			}
		}
		return qtrue;
	}

	if ( Q_stricmp( cmd, "control" ) == 0 )
	{
		if ( !g_cheats->integer ) 
		{
			gi.SendServerCommand( 0, "print \"Cheats are not enabled on this server.\n\"");
			return qfalse;
		}
		const char	*cmd2 = gi.argv(1);
		if ( !*cmd2 || !cmd2[0] )
		{
			if ( !G_ClearViewEntity( &g_entities[0] ) )
			{
				gi.Printf( S_COLOR_RED"control <NPC_targetname>\n", cmd2 );
			}
		}
		else
		{
			Q3_SetViewEntity( 0, cmd2 );
		}
		return qtrue;
	}

	if ( Q_stricmp( cmd, "grab" ) == 0 )
	{
		if ( !g_cheats->integer ) 
		{
			gi.SendServerCommand( 0, "print \"Cheats are not enabled on this server.\n\"");
			return qfalse;
		}
		const char	*cmd2 = gi.argv(1);
		if ( !*cmd2 || !cmd2[0] )
		{
			if ( !G_ReleaseEntity( &g_entities[0] ) )
			{
				gi.Printf( S_COLOR_RED"grab <NPC_targetname>\n", cmd2 );
			}
		}
		else
		{
			G_GrabEntity( &g_entities[0], cmd2 );
		}
		return qtrue;
	}

	if ( Q_stricmp( cmd, "knockdown" ) == 0 )
	{
		if ( !g_cheats->integer ) 
		{
			gi.SendServerCommand( 0, "print \"Cheats are not enabled on this server.\n\"");
			return qfalse;
		}
		G_Knockdown( &g_entities[0], &g_entities[0], vec3_origin, 300, qtrue );
		return qtrue;
	}

	if ( Q_stricmp( cmd, "playerModel" ) == 0 )
	{
		if ( gi.argc() == 1 )
		{
			gi.Printf( S_COLOR_RED"USAGE: playerModel <NPC Name>\n       playerModel <g2model> <skinhead> <skintorso> <skinlower>\n       playerModel player (builds player from customized menu settings)\n" );
			gi.Printf( "playerModel = %s ", va("%s %s %s %s\n", g_char_model->string, g_char_skin_head->string, g_char_skin_torso->string, g_char_skin_legs->string ) );
		}
		else if ( gi.argc() == 2 )
		{
			G_ChangePlayerModel( &g_entities[0], gi.argv(1) );
		}
		else if (  gi.argc() == 5 )
		{
			//instead of setting it directly via a command, we now store it in cvars
			//G_ChangePlayerModel( &g_entities[0], va("%s|%s|%s|%s", gi.argv(1), gi.argv(2), gi.argv(3), gi.argv(4)) );
			gi.cvar_set("g_char_model", gi.argv(1) );
			gi.cvar_set("g_char_skin_head", gi.argv(2) );
			gi.cvar_set("g_char_skin_torso", gi.argv(3) );
			gi.cvar_set("g_char_skin_legs", gi.argv(4) );
			G_InitPlayerFromCvars( &g_entities[0] );
		}
		return qtrue;
	}

	if ( Q_stricmp( cmd, "playerTint" ) == 0 )
	{
		if ( gi.argc() == 4 )
		{
			g_entities[0].client->renderInfo.customRGBA[0] = atoi(gi.argv(1));
			g_entities[0].client->renderInfo.customRGBA[1] = atoi(gi.argv(2));
			g_entities[0].client->renderInfo.customRGBA[2] = atoi(gi.argv(3));
			gi.cvar_set("g_char_color_red", gi.argv(1) );
			gi.cvar_set("g_char_color_green", gi.argv(2) );
			gi.cvar_set("g_char_color_blue", gi.argv(3) );
		}
		else
		{
			gi.Printf( S_COLOR_RED"USAGE: playerTint <red 0 - 255> <green 0 - 255> <blue 0 - 255>\n" );
			gi.Printf( "playerTint = %s\n", va("%d %d %d", g_char_color_red->integer, g_char_color_green->integer, g_char_color_blue->integer ) );
		}
		return qtrue;
	}
	if ( Q_stricmp( cmd, "nexttestaxes" ) == 0 )
	{
		G_NextTestAxes();
	}

	if ( Q_stricmp( cmd, "exitview" ) == 0 )
	{
		Svcmd_ExitView_f();
	}
	
	if (Q_stricmp (cmd, "iknowkungfu") == 0)
	{
		gi.cvar_set( "g_debugMelee", "1" );
		G_SetWeapon( &g_entities[0], WP_MELEE );
		for ( int i = FP_FIRST; i < NUM_FORCE_POWERS; i++ )
		{
			g_entities[0].client->ps.forcePowersKnown |= ( 1 << i );
			if ( i == FP_TELEPATHY )
			{
				g_entities[0].client->ps.forcePowerLevel[i] = FORCE_LEVEL_4;
			}
			else
			{
				g_entities[0].client->ps.forcePowerLevel[i] = FORCE_LEVEL_3;
			}
		}
	}

	return qfalse;
}

