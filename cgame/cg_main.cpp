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

// this line must stay at top so the whole PCH thing works...
#include "cg_headers.h"
//#include "../ui/ui_shared.h"

//#include "cg_local.h"
#include "cg_media.h"
#include "FxScheduler.h"

#include "../client/vmachine.h"
#include "cg_lights.h"

#include "../qcommon/sstring.h"
//NOTENOTE: Be sure to change the mirrored code in g_shared.h
typedef	map< sstring_t, unsigned char, less<sstring_t>, allocator< unsigned char >  >	namePrecache_m;
extern namePrecache_m	*as_preCacheMap;
extern void CG_RegisterNPCCustomSounds( clientInfo_t *ci );
extern qboolean G_AddSexToMunroString ( char *string, qboolean qDoBoth );
//extern void CG_RegisterNPCEffects( team_t team );
extern int G_ParseAnimFileSet( const char *skeletonName, const char *modelName=0);
extern void CG_DrawDataPadInventorySelect( void );

void CG_Init( int serverCommandSequence );
qboolean CG_ConsoleCommand( void );
void CG_Shutdown( void );
int CG_GetCameraPos( vec3_t camerapos );
int CG_GetCameraAng( vec3_t cameraang );
void UseItem(int itemNum);
const char *CG_DisplayBoxedText(int iBoxX, int iBoxY, int iBoxWidth, int iBoxHeight, 
								const char *psText, int iFontHandle, float fScale,
								const vec4_t v4Color);

#define NUM_CHUNKS		6
/*
Ghoul2 Insert Start
*/

void CG_ResizeG2Bolt(boltInfo_v *bolt, int newCount);
void CG_ResizeG2Surface(surfaceInfo_v *surface, int newCount);
void CG_ResizeG2Bone(boneInfo_v *bone, int newCount);
void CG_ResizeG2(CGhoul2Info_v *ghoul2, int newCount);
void CG_ResizeG2TempBone(mdxaBone_v *tempBone, int newCount);
/*
Ghoul2 Insert End
*/


void CG_LoadHudMenu(void);
int inv_icons[INV_MAX];
const char *inv_names[] =
{
"ELECTROBINOCULARS",
"BACTA CANISTER",
"SEEKER",
"LIGHT AMP GOGGLES",
"ASSAULT SENTRY",
"GOODIE KEY",
"GOODIE KEY",
"GOODIE KEY",
"GOODIE KEY",
"GOODIE KEY",
"SECURITY KEY",
"SECURITY KEY",
"SECURITY KEY",
"SECURITY KEY",
"SECURITY KEY",
};

int	force_icons[NUM_FORCE_POWERS];
int	force_icons1[NUM_FORCE_POWERS];
int	force_icons2[NUM_FORCE_POWERS];
int	force_icons3[NUM_FORCE_POWERS];
int	force_icons4[NUM_FORCE_POWERS];
int	force_icons5[NUM_FORCE_POWERS];
int	force_icons6[NUM_FORCE_POWERS];
int	force_icons7[NUM_FORCE_POWERS];
int	force_icons8[NUM_FORCE_POWERS];
int	force_icons9[NUM_FORCE_POWERS];
int	force_icons10[NUM_FORCE_POWERS];
int	force_icons11[NUM_FORCE_POWERS];
int	force_icons12[NUM_FORCE_POWERS];
int	force_icons13[NUM_FORCE_POWERS];
int	force_icons14[NUM_FORCE_POWERS];
int	force_icons15[NUM_FORCE_POWERS];
int	force_icons16[NUM_FORCE_POWERS];
int	force_icons17[NUM_FORCE_POWERS];
int	force_icons18[NUM_FORCE_POWERS];
int	force_icons19[NUM_FORCE_POWERS];
int	force_icons20[NUM_FORCE_POWERS];
int	force_icons21[NUM_FORCE_POWERS];
int	force_icons22[NUM_FORCE_POWERS];
int	force_icons23[NUM_FORCE_POWERS];
int	force_icons24[NUM_FORCE_POWERS];
int	force_icons25[NUM_FORCE_POWERS];
int	force_icons26[NUM_FORCE_POWERS];
int	force_icons27[NUM_FORCE_POWERS];
int	force_icons28[NUM_FORCE_POWERS];
int	force_icons29[NUM_FORCE_POWERS];
int	force_icons30[NUM_FORCE_POWERS];
int	force_icons31[NUM_FORCE_POWERS];
int	force_icons32[NUM_FORCE_POWERS];
int	force_icons33[NUM_FORCE_POWERS];
int	force_icons34[NUM_FORCE_POWERS];
int	force_icons35[NUM_FORCE_POWERS];
int	force_icons36[NUM_FORCE_POWERS];

void CG_DrawDataPadHUD( centity_t *cent );
void CG_DrawDataPadObjectives(const centity_t *cent );
void CG_DrawDataPadIconBackground(const int backgroundType);
void CG_DrawDataPadWeaponSelect( void );
void CG_DrawDataPadForceSelect( void );

/*
================
vmMain

This is the only way control passes into the cgame module.
This must be the very first function compiled into the .q3vm file
================
*/
#ifndef _WIN32
extern "C"
#endif
Q_EXPORT intptr_t vmMain( intptr_t command, intptr_t arg0, intptr_t arg1, intptr_t arg2, intptr_t arg3, intptr_t arg4, intptr_t arg5, intptr_t arg6, intptr_t arg7  ) {
	centity_t		*cent;

	switch ( command ) {
	case CG_INIT:
		CG_Init( arg0 );
		return 0;
	case CG_SHUTDOWN:
		CG_Shutdown();
		return 0;
	case CG_CONSOLE_COMMAND:
		return CG_ConsoleCommand();
	case CG_DRAW_ACTIVE_FRAME:
		CG_DrawActiveFrame( arg0, (stereoFrame_t) arg1 );
		return 0;
	case CG_CROSSHAIR_PLAYER:
		return CG_CrosshairPlayer();
	case CG_CAMERA_POS:
		return CG_GetCameraPos( (float*)arg0);
	case CG_CAMERA_ANG:
		return CG_GetCameraAng( (float*)arg0);
/*
Ghoul2 Insert Start
*/
	case CG_RESIZE_G2:
		CG_ResizeG2((CGhoul2Info_v *)arg0, arg1);
		return 0;
	case CG_RESIZE_G2_BOLT:
		CG_ResizeG2Bolt((boltInfo_v *)arg0, arg1);
		return 0;
	case CG_RESIZE_G2_BONE:
		CG_ResizeG2Bone((boneInfo_v *)arg0, arg1);
		return 0;
	case CG_RESIZE_G2_SURFACE:
		CG_ResizeG2Surface((surfaceInfo_v *)arg0, arg1);
		return 0;
	case CG_RESIZE_G2_TEMPBONE:
		CG_ResizeG2TempBone((mdxaBone_v *)arg0, arg1);
		return 0;

/*
Ghoul2 Insert End
*/
	case CG_DRAW_DATAPAD_HUD:
		if (cg.snap)
		{
			cent = &cg_entities[cg.snap->ps.clientNum];
			CG_DrawDataPadHUD(cent);
		}
		return 0;

	case CG_DRAW_DATAPAD_OBJECTIVES:
		if (cg.snap)
		{
			cent = &cg_entities[cg.snap->ps.clientNum];
			CG_DrawDataPadObjectives(cent);
		}
		return 0;

	case CG_DRAW_DATAPAD_WEAPONS:
		if (cg.snap)
		{
			CG_DrawDataPadIconBackground(ICON_WEAPONS);
			CG_DrawDataPadWeaponSelect();
		}
		return 0;
	case CG_DRAW_DATAPAD_INVENTORY:
		if (cg.snap)
		{
			CG_DrawDataPadIconBackground(ICON_INVENTORY);
			CG_DrawDataPadInventorySelect();
		}
		return 0;
	case CG_DRAW_DATAPAD_FORCEPOWERS:
		if (cg.snap)
		{
			CG_DrawDataPadIconBackground(ICON_FORCE);
			CG_DrawDataPadForceSelect();
		}
		return 0;
	}
	return -1;
}

/*
Ghoul2 Insert Start
*/

void CG_ResizeG2Bolt(boltInfo_v *bolt, int newCount)
{
	bolt->resize(newCount);
}

void CG_ResizeG2Surface(surfaceInfo_v *surface, int newCount)
{
	surface->resize(newCount);
}

void CG_ResizeG2Bone(boneInfo_v *bone, int newCount)
{
	bone->resize(newCount);
}

void CG_ResizeG2(CGhoul2Info_v *ghoul2, int newCount)
{
	ghoul2->resize(newCount);
}

void CG_ResizeG2TempBone(mdxaBone_v *tempBone, int newCount)
{
	tempBone->resize(newCount);
}
/*
Ghoul2 Insert End
*/

cg_t				cg;
cgs_t				cgs;
centity_t			cg_entities[MAX_GENTITIES];

centity_t			*cg_permanents[MAX_GENTITIES];
int					cg_numpermanents = 0;

weaponInfo_t		cg_weapons[MAX_WEAPONS];
itemInfo_t			cg_items[MAX_ITEMS];
gunInfo_t			cg_guns[MAX_GUNS];

typedef struct {
	qboolean		registered;		// Has the player picked it up
	qboolean		active;			// Is it the chosen inventory item
	int				count;			// Count of items.
	char			description[128];
} inventoryInfo_t;

inventoryInfo_t		cg_inventory[INV_MAX];

vmCvar_t	cg_runpitch;
vmCvar_t	cg_runroll;
vmCvar_t	cg_bobup;
vmCvar_t	cg_bobpitch;
vmCvar_t	cg_bobroll;
vmCvar_t	cg_shadows;
vmCvar_t	cg_renderToTextureFX;
vmCvar_t	cg_shadowCullDistance;
vmCvar_t	cg_footsteps;
vmCvar_t	cg_saberEntMarks;
vmCvar_t	cg_paused;
vmCvar_t	cg_drawTimer;
vmCvar_t	cg_drawFPS;
vmCvar_t	cg_drawSnapshot;
vmCvar_t	cg_drawAmmoWarning;
vmCvar_t	cg_drawCrosshair;
vmCvar_t	cg_crosshairIdentifyTarget;
//vmCvar_t	cg_dynamicCrosshair;
vmCvar_t	cg_crosshairForceHint;
vmCvar_t	cg_crosshairX;
vmCvar_t	cg_crosshairY;
vmCvar_t	cg_crosshairSize;
vmCvar_t	cg_draw2D;
vmCvar_t	cg_drawStatus;
vmCvar_t	cg_drawHUD;
vmCvar_t	cg_debugAnim;
#ifndef FINAL_BUILD
vmCvar_t	cg_debugAnimTarget;
vmCvar_t	cg_gun_frame;
#endif
vmCvar_t	cg_gun_x;
vmCvar_t	cg_gun_y;
vmCvar_t	cg_gun_z;
vmCvar_t	cg_debugSaber;
vmCvar_t	cg_debugEvents;
vmCvar_t	cg_errorDecay;
vmCvar_t	cg_addMarks;
vmCvar_t	cg_drawGun;
vmCvar_t	cg_autoswitch;
vmCvar_t	cg_simpleItems;
vmCvar_t	cg_fov;
vmCvar_t	cg_fovAspectAdjust;
vmCvar_t	cg_endcredits;
vmCvar_t	cg_updatedDataPadForcePower1;
vmCvar_t	cg_updatedDataPadForcePower2;
vmCvar_t	cg_updatedDataPadForcePower3;
vmCvar_t	cg_updatedDataPadObjective;
vmCvar_t	cg_drawBreath;
vmCvar_t	cg_roffdebug;
#ifndef FINAL_BUILD
vmCvar_t	cg_roffval1;
vmCvar_t	cg_roffval2;
vmCvar_t	cg_roffval3;
vmCvar_t	cg_roffval4;
#endif
vmCvar_t	cg_thirdPerson;
vmCvar_t	cg_thirdPersonRange;
vmCvar_t	cg_thirdPersonMaxRange;
vmCvar_t	cg_thirdPersonAngle;
vmCvar_t	cg_thirdPersonPitchOffset;
vmCvar_t	cg_thirdPersonVertOffset;
vmCvar_t	cg_thirdPersonCameraDamp;
vmCvar_t	cg_thirdPersonTargetDamp;
vmCvar_t	cg_gunAutoFirst;

vmCvar_t	cg_thirdPersonAlpha;
vmCvar_t	cg_thirdPersonAutoAlpha;
vmCvar_t	cg_thirdPersonHorzOffset;

vmCvar_t	cg_stereoSeparation;
vmCvar_t 	cg_developer;
vmCvar_t 	cg_timescale;
vmCvar_t	cg_skippingcin;

vmCvar_t	cg_pano;
vmCvar_t	cg_panoNumShots;

vmCvar_t	fx_freeze;
vmCvar_t	fx_debug;

vmCvar_t	cg_missionInfoFlashTime;
vmCvar_t	cg_hudFiles;

vmCvar_t	cg_neverHearThatDumbBeepingSoundAgain;

vmCvar_t	cg_VariantSoundCap;	// 0 = no capping, else cap to (n) max (typically just 1, but allows more)
vmCvar_t	cg_turnAnims;
vmCvar_t	cg_motionBoneComp;
vmCvar_t	cg_distributeMBCorrection;
vmCvar_t	cg_reliableAnimEvents;

vmCvar_t	cg_smoothPlayerPos;
vmCvar_t	cg_smoothPlayerPlat;
vmCvar_t	cg_smoothPlayerPlatAccel;
vmCvar_t	cg_g2Marks;
vmCvar_t	fx_expensivePhysics;
vmCvar_t	cg_debugHealthBars;

typedef struct {
	vmCvar_t	*vmCvar;
	const char	*cvarName;
	const char	*defaultString;
	int			cvarFlags;
} cvarTable_t;

static cvarTable_t cvarTable[] = {
	{ &cg_autoswitch, "cg_autoswitch", "1", CVAR_ARCHIVE },
	{ &cg_drawGun, "cg_drawGun", "1", CVAR_ARCHIVE },
	{ &cg_fov, "cg_fov", "80", CVAR_ARCHIVE },
	{ &cg_fovAspectAdjust, "cg_fovAspectAdjust", "0", CVAR_ARCHIVE },
	{ &cg_stereoSeparation, "cg_stereoSeparation", "0.4", CVAR_ARCHIVE  },
	{ &cg_shadows, "cg_shadows", "1", CVAR_ARCHIVE  },
	{ &cg_renderToTextureFX, "cg_renderToTextureFX", "1", CVAR_ARCHIVE  },
	{ &cg_shadowCullDistance, "r_shadowRange", "1000", CVAR_ARCHIVE },
	{ &cg_footsteps, "cg_footsteps", "3", CVAR_ARCHIVE  },//1 = sounds, 2 = sounds & effects, 3 = sounds, effects & marks, 4 = always
	{ &cg_saberEntMarks, "cg_saberEntMarks", "1", CVAR_ARCHIVE  },

	{ &cg_draw2D, "cg_draw2D", "1", CVAR_ARCHIVE  },
	{ &cg_drawStatus, "cg_drawStatus", "1", CVAR_ARCHIVE  },
	{ &cg_drawHUD, "cg_drawHUD", "1", 0  },
	{ &cg_drawTimer, "cg_drawTimer", "0", CVAR_ARCHIVE  },
	{ &cg_drawFPS, "cg_drawFPS", "0", CVAR_ARCHIVE  },
	{ &cg_drawSnapshot, "cg_drawSnapshot", "0", CVAR_ARCHIVE  },
	{ &cg_drawAmmoWarning, "cg_drawAmmoWarning", "1", CVAR_ARCHIVE  },
	{ &cg_drawCrosshair, "cg_drawCrosshair", "1", CVAR_ARCHIVE },
//	{ &cg_dynamicCrosshair, "cg_dynamicCrosshair", "1", CVAR_ARCHIVE },
	// NOTE : I also create this in UI_Init()
	{ &cg_crosshairIdentifyTarget, "cg_crosshairIdentifyTarget", "1", CVAR_ARCHIVE },
	{ &cg_crosshairForceHint, "cg_crosshairForceHint", "1", CVAR_ARCHIVE|CVAR_SAVEGAME|CVAR_NORESTART },
	{ &cg_endcredits, "cg_endcredits", "0", 0},
	{ &cg_updatedDataPadForcePower1, "cg_updatedDataPadForcePower1", "0", 0},
	{ &cg_updatedDataPadForcePower2, "cg_updatedDataPadForcePower2", "0", 0},
	{ &cg_updatedDataPadForcePower3, "cg_updatedDataPadForcePower3", "0", 0},
	{ &cg_updatedDataPadObjective, "cg_updatedDataPadObjective", "0", 0},

	{ &cg_crosshairSize, "cg_crosshairSize", "24", CVAR_ARCHIVE },
	{ &cg_crosshairX, "cg_crosshairX", "0", CVAR_ARCHIVE },
	{ &cg_crosshairY, "cg_crosshairY", "0", CVAR_ARCHIVE },
	{ &cg_simpleItems, "cg_simpleItems", "0", CVAR_ARCHIVE },
	// NOTE : I also create this in UI_Init()
	{ &cg_addMarks, "cg_marks", "1", CVAR_ARCHIVE },
	// NOTE : I also create these weapon sway cvars in UI_Init()
	{ &cg_runpitch, "cg_runpitch", "0.002", CVAR_ARCHIVE},
	{ &cg_runroll, "cg_runroll", "0.005", CVAR_ARCHIVE },
	{ &cg_bobup , "cg_bobup", "0.005", CVAR_ARCHIVE },
	{ &cg_bobpitch, "cg_bobpitch", "0.002", CVAR_ARCHIVE },
	{ &cg_bobroll, "cg_bobroll", "0.002", CVAR_ARCHIVE },

	{ &cg_debugAnim, "cg_debuganim", "0", CVAR_CHEAT },
#ifndef FINAL_BUILD
	{ &cg_gun_frame, "gun_frame", "0", CVAR_CHEAT },
	{ &cg_debugAnimTarget, "cg_debugAnimTarget", "0", CVAR_CHEAT },
#endif	
	{ &cg_gun_x, "cg_gunX", "0", CVAR_CHEAT },
	{ &cg_gun_y, "cg_gunY", "0", CVAR_CHEAT },
	{ &cg_gun_z, "cg_gunZ", "0", CVAR_CHEAT },
	{ &cg_debugSaber, "cg_debugsaber", "0", CVAR_CHEAT },
	{ &cg_debugEvents, "cg_debugevents", "0", CVAR_CHEAT },
	{ &cg_errorDecay, "cg_errordecay", "100", 0 },

	{ &cg_drawBreath, "cg_drawBreath", "0", CVAR_ARCHIVE },		// Added 11/07/02
	{ &cg_roffdebug, "cg_roffdebug", "0" },
#ifndef FINAL_BUILD
	{ &cg_roffval1, "cg_roffval1", "0" },
	{ &cg_roffval2, "cg_roffval2", "0" },
	{ &cg_roffval3, "cg_roffval3", "0" },
	{ &cg_roffval4, "cg_roffval4", "0" },
#endif
	{ &cg_thirdPerson, "cg_thirdPerson", "1", CVAR_SAVEGAME },
	{ &cg_thirdPersonRange, "cg_thirdPersonRange", "80", 0 },
	{ &cg_thirdPersonMaxRange, "cg_thirdPersonMaxRange", "150", 0 },
	{ &cg_thirdPersonAngle, "cg_thirdPersonAngle", "0", 0 },
	{ &cg_thirdPersonPitchOffset, "cg_thirdPersonPitchOffset", "0", 0 },
	{ &cg_thirdPersonVertOffset, "cg_thirdPersonVertOffset", "16", 0},
	{ &cg_thirdPersonCameraDamp, "cg_thirdPersonCameraDamp", "0.3", 0},
	{ &cg_thirdPersonTargetDamp, "cg_thirdPersonTargetDamp", "0.5", 0},
	
	{ &cg_thirdPersonHorzOffset, "cg_thirdPersonHorzOffset", "0", 0},
	{ &cg_thirdPersonAlpha,	"cg_thirdPersonAlpha",	"1.0", CVAR_CHEAT },
	{ &cg_thirdPersonAutoAlpha,	"cg_thirdPersonAutoAlpha",	"0", 0 },
	// NOTE: also declare this in UI_Init
	{ &cg_gunAutoFirst, "cg_gunAutoFirst", "1", CVAR_ARCHIVE },

	{ &cg_pano, "pano", "0", 0 },
	{ &cg_panoNumShots, "panoNumShots", "10", 0 },

	{ &fx_freeze, "fx_freeze", "0", 0 },
	{ &fx_debug, "fx_debug", "0", 0 },
	// the following variables are created in other parts of the system,
	// but we also reference them here

	{ &cg_paused, "cl_paused", "0", CVAR_ROM },
	{ &cg_developer, "developer", "", 0 }, 
	{ &cg_timescale, "timescale", "1", 0 }, 	
	{ &cg_skippingcin, "skippingCinematic", "0", CVAR_ROM},
	{ &cg_missionInfoFlashTime, "cg_missionInfoFlashTime", "10000", 0  },
	{ &cg_hudFiles, "cg_hudFiles", "ui/jahud.txt", CVAR_ARCHIVE},

	{ &cg_VariantSoundCap, "cg_VariantSoundCap", "0", 0 },
	{ &cg_turnAnims, "cg_turnAnims", "0", 0 },
	{ &cg_motionBoneComp, "cg_motionBoneComp", "2", 0 },
	{ &cg_distributeMBCorrection, "cg_distributeMBCorrection", "1", 0 },
	{ &cg_reliableAnimEvents, "cg_reliableAnimEvents", "1", CVAR_ARCHIVE },
	{ &cg_smoothPlayerPos, "cg_smoothPlayerPos", "0.5", 0},
	{ &cg_smoothPlayerPlat, "cg_smoothPlayerPlat", "0.75", 0},
	{ &cg_smoothPlayerPlatAccel, "cg_smoothPlayerPlatAccel", "3.25", 0},
	{ &cg_g2Marks, "cg_g2Marks", "1", CVAR_ARCHIVE },
	{ &fx_expensivePhysics, "fx_expensivePhysics", "1", CVAR_ARCHIVE },
	{ &cg_debugHealthBars,	"cg_debugHealthBars",	"0", CVAR_CHEAT },
};

static int cvarTableSize = sizeof( cvarTable ) / sizeof( cvarTable[0] );

/*
=================
CG_RegisterCvars
=================
*/
void CG_RegisterCvars( void ) {
	int			i;
	cvarTable_t	*cv;

	for ( i = 0, cv = cvarTable ; i < cvarTableSize ; i++, cv++ ) {
		cgi_Cvar_Register( cv->vmCvar, cv->cvarName,
			cv->defaultString, cv->cvarFlags );
	}
}

/*
=================
CG_UpdateCvars
=================
*/
void CG_UpdateCvars( void ) {
	int			i;
	cvarTable_t	*cv;

	for ( i = 0, cv = cvarTable ; i < cvarTableSize ; i++, cv++ ) {
		cgi_Cvar_Update( cv->vmCvar );
	}
}


int CG_CrosshairPlayer( void ) 
{
	if ( cg.time > ( cg.crosshairClientTime + 1000 ) ) 
	{
		return -1;
	}
	return cg.crosshairClientNum;
}


int CG_GetCameraPos( vec3_t camerapos ) {
	if ( in_camera) {
		VectorCopy(client_camera.origin, camerapos);
		return 1;
	}
	else if ( cg_entities[0].gent && cg_entities[0].gent->client && cg_entities[0].gent->client->ps.viewEntity > 0 && cg_entities[0].gent->client->ps.viewEntity < ENTITYNUM_WORLD )
	//else if ( cg.snap && cg.snap->ps.viewEntity > 0 && cg.snap->ps.viewEntity < ENTITYNUM_WORLD )
	{//in an entity camera view
		if ( g_entities[cg_entities[0].gent->client->ps.viewEntity].client && cg.renderingThirdPerson )
		{
			VectorCopy( g_entities[cg_entities[0].gent->client->ps.viewEntity].client->renderInfo.eyePoint, camerapos );
		}
		else
		{
			VectorCopy( g_entities[cg_entities[0].gent->client->ps.viewEntity].currentOrigin, camerapos );
		}
		//VectorCopy( cg_entities[cg_entities[0].gent->client->ps.viewEntity].lerpOrigin, camerapos );
		/*
		if ( g_entities[cg.snap->ps.viewEntity].client && cg.renderingThirdPerson )
		{
			VectorCopy( g_entities[cg.snap->ps.viewEntity].client->renderInfo.eyePoint, camerapos );
		}
		else
		{//use the g_ent because it may not have gotten over to the client yet...
			VectorCopy( g_entities[cg.snap->ps.viewEntity].currentOrigin, camerapos );
		}
		*/
		return 1;
	}
	else if ( cg.renderingThirdPerson )
	{//in third person
		//FIXME: what about hacks that render in third person regardless of this value?
		VectorCopy( cg.refdef.vieworg, camerapos );
		return 1;
	}
	else if (cg.snap && (cg.snap->ps.weapon == WP_SABER||cg.snap->ps.weapon == WP_MELEE) )//implied: !cg.renderingThirdPerson 
	{//first person saber hack
		VectorCopy( cg.refdef.vieworg, camerapos );
		return 1;
	}
	return 0;
}

int CG_GetCameraAng( vec3_t cameraang ) 
{
	if ( in_camera) 
	{
		VectorCopy(client_camera.angles, cameraang);
		return 1;
	}
	else
	{
		VectorCopy( cg.refdefViewAngles, cameraang );
		return 1;
	}
}

void CG_TargetCommand_f( void ) {
	int		targetNum;
	char	test[4];

	targetNum = CG_CrosshairPlayer();
	if (targetNum <= 0) {
		return;
	}

	cgi_Argv( 1, test, 4 );	//FIXME: this is now an exec_now command - in case we start using it... JFM
	cgi_SendConsoleCommand( va( "gc %i %i", targetNum, atoi( test ) ) );
}

void CG_Printf( const char *msg, ... ) {
	va_list		argptr;
	char		text[1024];

	va_start (argptr, msg);
	Q_vsnprintf (text, sizeof(text), msg, argptr);
	va_end (argptr);

	cgi_Printf( text );
}

void CG_Error( const char *msg, ... ) {
	va_list		argptr;
	char		text[1024];

	va_start (argptr, msg);
	Q_vsnprintf (text, sizeof(text), msg, argptr);
	va_end (argptr);

	cgi_Error( text );
}

/*
================
CG_Argv
================
*/
const char *CG_Argv( int arg ) {
	static char	buffer[MAX_STRING_CHARS];

	cgi_Argv( arg, buffer, sizeof( buffer ) );

	return buffer;
}

//========================================================================

/*
=================
CG_RegisterItemSounds

The server says this item is used on this level
=================
*/
void CG_RegisterItemSounds( int itemNum ) {
	gitem_t			*item;
	char			data[MAX_QPATH];
	const char		*s, *start;
	int				len;

	item = &bg_itemlist[ itemNum ];

	if (item->pickup_sound)
	{
		cgi_S_RegisterSound( item->pickup_sound );
	}

	// parse the space seperated precache string for other media
	s = item->sounds;
	if (!s || !s[0])
		return;

	while (*s) {
		start = s;
		while (*s && *s != ' ') {
			s++;
		}

		len = s-start;
		if (len >= MAX_QPATH || len < 5) {
			CG_Error( "PrecacheItem: %s has bad precache string", 
				item->classname);
			return;
		}
		memcpy (data, start, len);
		data[len] = 0;
		if ( *s ) {
			s++;
		}

		if ( !strcmp(data+len-3, "wav" )) {
			cgi_S_RegisterSound( data );
		}
	}
}

/*
======================
CG_LoadingString

======================
*/
void CG_LoadingString( const char *s ) {
	Q_strncpyz( cg.infoScreenText, s, sizeof( cg.infoScreenText ) );
	cgi_UpdateScreen();
}

static inline void CG_AS_Register(void)
{
	CG_LoadingString( "ambient sound sets" );

	assert(as_preCacheMap);

	//Load the ambient sets

	cgi_AS_AddPrecacheEntry( "#clear" );	// ;-)
	//FIXME: Don't ask... I had to get around a really nasty MS error in the templates with this...
	namePrecache_m::iterator	pi;
	STL_ITERATE( pi, (*as_preCacheMap) )
	{
		cgi_AS_AddPrecacheEntry( ((*pi).first).c_str() );
	}

	cgi_AS_ParseSets();
}

/*
=================
CG_RegisterSounds

called during a precache command
=================
*/
static void CG_RegisterSounds( void ) {
	int		i;
	char	name[MAX_QPATH];
	const char	*soundName;

	CG_AS_Register();

	CG_LoadingString( "general sounds" );

	//FIXME: add to cg.media?
	cgi_S_RegisterSound( "sound/player/fallsplat.wav" );

	cgs.media.selectSound = cgi_S_RegisterSound( "sound/weapons/change.wav" );
	cgs.media.selectSound2 = cgi_S_RegisterSound( "sound/interface/sub_select.wav" );
//	cgs.media.useNothingSound = cgi_S_RegisterSound( "sound/items/use_nothing.wav" );

	cgs.media.noAmmoSound = cgi_S_RegisterSound( "sound/weapons/noammo.wav" );
//	cgs.media.talkSound = 	cgi_S_RegisterSound( "sound/interface/communicator.wav" );
	cgs.media.landSound =	cgi_S_RegisterSound( "sound/player/land1.wav");
	cgs.media.rollSound =	cgi_S_RegisterSound( "sound/player/roll1.wav");
	theFxScheduler.RegisterEffect( "env/slide_dust" );

	cgs.media.overchargeFastSound	= cgi_S_RegisterSound("sound/weapons/overchargeFast.wav" );
	cgs.media.overchargeSlowSound	= cgi_S_RegisterSound("sound/weapons/overchargeSlow.wav" );
	cgs.media.overchargeLoopSound	= cgi_S_RegisterSound("sound/weapons/overchargeLoop.wav");
	cgs.media.overchargeEndSound	= cgi_S_RegisterSound("sound/weapons/overchargeEnd.wav");

	cgs.media.batteryChargeSound	= cgi_S_RegisterSound( "sound/interface/pickup_battery.wav" );

//	cgs.media.tedTextSound = cgi_S_RegisterSound( "sound/interface/tedtext.wav" );
	cgs.media.messageLitSound = cgi_S_RegisterSound( "sound/interface/update" );

	cgs.media.noforceSound = cgi_S_RegisterSound( "sound/weapons/force/noforce" );
	
	cgs.media.watrInSound = cgi_S_RegisterSound ("sound/player/watr_in.wav");
	cgs.media.watrOutSound = cgi_S_RegisterSound ("sound/player/watr_out.wav");
	cgs.media.watrUnSound = cgi_S_RegisterSound ("sound/player/watr_un.wav");

	if ( (gi.totalMapContents()&CONTENTS_LAVA) )
	{
		cgs.media.lavaInSound = cgi_S_RegisterSound ("sound/player/inlava.wav");
		cgs.media.lavaOutSound = cgi_S_RegisterSound ("sound/player/watr_out.wav");
		cgs.media.lavaUnSound = cgi_S_RegisterSound ("sound/player/muckexit.wav");
	}
	// Zoom
	cgs.media.zoomStart = cgi_S_RegisterSound( "sound/interface/zoomstart.wav" );
	cgs.media.zoomLoop = cgi_S_RegisterSound( "sound/interface/zoomloop.wav" );
	cgs.media.zoomEnd = cgi_S_RegisterSound( "sound/interface/zoomend.wav" );

	cgi_S_RegisterSound( "sound/chars/turret/startup.wav" );
	cgi_S_RegisterSound( "sound/chars/turret/shutdown.wav" );
	cgi_S_RegisterSound( "sound/chars/turret/ping.wav" );
	cgi_S_RegisterSound( "sound/chars/turret/move.wav" );
	cgi_S_RegisterSound( "sound/player/use_sentry" );
	cgi_R_RegisterModel( "models/items/psgun.glm" );
	theFxScheduler.RegisterEffect( "turret/explode" );
	theFxScheduler.RegisterEffect( "sparks/spark_exp_nosnd" );

	for (i=0 ; i<4 ; i++) {
		Com_sprintf (name, sizeof(name), "sound/player/footsteps/stone_step%i.wav", i+1);
		cgs.media.footsteps[FOOTSTEP_STONEWALK][i] = cgi_S_RegisterSound (name);
		Com_sprintf (name, sizeof(name), "sound/player/footsteps/stone_run%i.wav", i+1);
		cgs.media.footsteps[FOOTSTEP_STONERUN][i] = cgi_S_RegisterSound (name);

		Com_sprintf (name, sizeof(name), "sound/player/footsteps/metal_step%i.wav", i+1);
		cgs.media.footsteps[FOOTSTEP_METALWALK][i] = cgi_S_RegisterSound (name);
		Com_sprintf (name, sizeof(name), "sound/player/footsteps/metal_run%i.wav", i+1);
		cgs.media.footsteps[FOOTSTEP_METALRUN][i] = cgi_S_RegisterSound (name);

		Com_sprintf (name, sizeof(name), "sound/player/footsteps/pipe_step%i.wav", i+1);
		cgs.media.footsteps[FOOTSTEP_PIPEWALK][i] = cgi_S_RegisterSound (name);
		Com_sprintf (name, sizeof(name), "sound/player/footsteps/pipe_run%i.wav", i+1);
		cgs.media.footsteps[FOOTSTEP_PIPERUN][i] = cgi_S_RegisterSound (name);

		Com_sprintf (name, sizeof(name), "sound/player/footsteps/water_run%i.wav", i+1);
		cgs.media.footsteps[FOOTSTEP_SPLASH][i] = cgi_S_RegisterSound (name);

		Com_sprintf (name, sizeof(name), "sound/player/footsteps/water_walk%i.wav", i+1);
		cgs.media.footsteps[FOOTSTEP_WADE][i] = cgi_S_RegisterSound (name);

		Com_sprintf (name, sizeof(name), "sound/player/footsteps/water_wade_0%i.wav", i+1);
		cgs.media.footsteps[FOOTSTEP_SWIM][i] = cgi_S_RegisterSound (name);

		Com_sprintf (name, sizeof(name), "sound/player/footsteps/snow_step%i.wav", i+1);
		cgs.media.footsteps[FOOTSTEP_SNOWWALK][i] = cgi_S_RegisterSound (name);
		Com_sprintf (name, sizeof(name), "sound/player/footsteps/snow_run%i.wav", i+1);
		cgs.media.footsteps[FOOTSTEP_SNOWRUN][i] = cgi_S_RegisterSound (name);

		Com_sprintf (name, sizeof(name), "sound/player/footsteps/sand_walk%i.wav", i+1);
		cgs.media.footsteps[FOOTSTEP_SANDWALK][i] = cgi_S_RegisterSound (name);
		Com_sprintf (name, sizeof(name), "sound/player/footsteps/sand_run%i.wav", i+1);
		cgs.media.footsteps[FOOTSTEP_SANDRUN][i] = cgi_S_RegisterSound (name);

		Com_sprintf (name, sizeof(name), "sound/player/footsteps/grass_step%i.wav", i+1);
		cgs.media.footsteps[FOOTSTEP_GRASSWALK][i] = cgi_S_RegisterSound (name);
		Com_sprintf (name, sizeof(name), "sound/player/footsteps/grass_run%i.wav", i+1);
		cgs.media.footsteps[FOOTSTEP_GRASSRUN][i] = cgi_S_RegisterSound (name);

		Com_sprintf (name, sizeof(name), "sound/player/footsteps/dirt_step%i.wav", i+1);
		cgs.media.footsteps[FOOTSTEP_DIRTWALK][i] = cgi_S_RegisterSound (name);
		Com_sprintf (name, sizeof(name), "sound/player/footsteps/dirt_run%i.wav", i+1);
		cgs.media.footsteps[FOOTSTEP_DIRTRUN][i] = cgi_S_RegisterSound (name);

		Com_sprintf (name, sizeof(name), "sound/player/footsteps/mud_walk%i.wav", i+1);
		cgs.media.footsteps[FOOTSTEP_MUDWALK][i] = cgi_S_RegisterSound (name);
		Com_sprintf (name, sizeof(name), "sound/player/footsteps/mud_run%i.wav", i+1);
		cgs.media.footsteps[FOOTSTEP_MUDRUN][i] = cgi_S_RegisterSound (name);

		Com_sprintf (name, sizeof(name), "sound/player/footsteps/gravel_walk%i.wav", i+1);
		cgs.media.footsteps[FOOTSTEP_GRAVELWALK][i] = cgi_S_RegisterSound (name);
		Com_sprintf (name, sizeof(name), "sound/player/footsteps/gravel_run%i.wav", i+1);
		cgs.media.footsteps[FOOTSTEP_GRAVELRUN][i] = cgi_S_RegisterSound (name);

		Com_sprintf (name, sizeof(name), "sound/player/footsteps/rug_step%i.wav", i+1);
		cgs.media.footsteps[FOOTSTEP_RUGWALK][i] = cgi_S_RegisterSound (name);
		Com_sprintf (name, sizeof(name), "sound/player/footsteps/rug_run%i.wav", i+1);
		cgs.media.footsteps[FOOTSTEP_RUGRUN][i] = cgi_S_RegisterSound (name);

		Com_sprintf (name, sizeof(name), "sound/player/footsteps/wood_walk%i.wav", i+1);
		cgs.media.footsteps[FOOTSTEP_WOODWALK][i] = cgi_S_RegisterSound (name);
		Com_sprintf (name, sizeof(name), "sound/player/footsteps/wood_run%i.wav", i+1);
		cgs.media.footsteps[FOOTSTEP_WOODRUN][i] = cgi_S_RegisterSound (name);
	}

	cg.loadLCARSStage = 1;
	CG_LoadingString( "item sounds" );

	// only register the items that the server says we need
	char	items[MAX_ITEMS+1];
	//Raz: Fixed buffer overflow
	Q_strncpyz(items, CG_ConfigString(CS_ITEMS), sizeof(items));

	for ( i = 1 ; i < bg_numItems ; i++ ) {
		if ( items[ i ] == '1' )	//even with sound pooling, don't clutter it for low end machines
		{
			CG_RegisterItemSounds( i );
		}
	}

	cg.loadLCARSStage = 2;
	CG_LoadingString( "preregistered sounds" );

	for ( i = 1 ; i < MAX_SOUNDS ; i++ ) {
		soundName = CG_ConfigString( CS_SOUNDS+i );
		if ( !soundName[0] ) {
			break;
		}
		if ( soundName[0] == '*' ) {
			continue;	// custom sound
		}
		if (!(i&7)) {
			CG_LoadingString( soundName );
		}
		cgs.sound_precache[i] = cgi_S_RegisterSound( soundName );
	}
}

/*
=============================================================================

CLIENT INFO

=============================================================================
*/

qhandle_t CG_RegisterHeadSkin( const char *headModelName, const char *headSkinName, qboolean *extensions )
{
	char		hfilename[MAX_QPATH];
	qhandle_t	headSkin;

	Com_sprintf( hfilename, sizeof( hfilename ), "models/players/%s/head_%s.skin", headModelName, headSkinName );
	headSkin = cgi_R_RegisterSkin( hfilename );
	if ( headSkin < 0 ) 
	{	//have extensions
		*extensions = qtrue;
		headSkin = -headSkin;
	} 
	else 
	{
		*extensions = qfalse;	//just to be sure.
	}

	if ( !headSkin )
	{
		Com_Printf( "Failed to load skin file: %s : %s\n", headModelName, headSkinName );
	}
	return headSkin;
}

/*
==========================
CG_RegisterClientSkin
==========================
*/
qboolean	CG_RegisterClientSkin( clientInfo_t *ci,
								  const char *headModelName, const char *headSkinName,
								  const char *torsoModelName, const char *torsoSkinName,
								  const char *legsModelName, const char *legsSkinName) 
{
	char		hfilename[MAX_QPATH];
	char		tfilename[MAX_QPATH];
	char		lfilename[MAX_QPATH];

	Com_sprintf( lfilename, sizeof( lfilename ), "models/players/%s/lower_%s.skin", legsModelName, legsSkinName );
	ci->legsSkin = cgi_R_RegisterSkin( lfilename );

	if ( !ci->legsSkin )
	{
//		Com_Printf( "Failed to load skin file: %s : %s\n", legsModelName, legsSkinName );
		//return qfalse;
	}

	if(torsoModelName && torsoSkinName && torsoModelName[0] && torsoSkinName[0])
	{
		Com_sprintf( tfilename, sizeof( tfilename ), "models/players/%s/upper_%s.skin", torsoModelName, torsoSkinName );
		ci->torsoSkin = cgi_R_RegisterSkin( tfilename );

		if ( !ci->torsoSkin )
		{
			Com_Printf( "Failed to load skin file: %s : %s\n", torsoModelName, torsoSkinName );
			return qfalse;
		}
	}

	if(headModelName && headSkinName && headModelName[0] && headSkinName[0])
	{
		Com_sprintf( hfilename, sizeof( hfilename ), "models/players/%s/head_%s.skin", headModelName, headSkinName );
		ci->headSkin = cgi_R_RegisterSkin( hfilename );

		if ( !ci->headSkin )
		{
			Com_Printf( "Failed to load skin file: %s : %s\n", headModelName, headSkinName );
			return qfalse;
		}
	}

	return qtrue;
}

/*
==========================
CG_RegisterClientModelname
==========================
*/
qboolean CG_RegisterClientModelname( clientInfo_t *ci,
									const char *headModelName, const char *headSkinName,
									const char *torsoModelName, const char *torsoSkinName,
									const char *legsModelName, const char *legsSkinName ) 
{
/*
Ghoul2 Insert Start
*/
#if 1
	char		filename[MAX_QPATH];


	if ( !legsModelName || !legsModelName[0] )
	{
		return qtrue;
	}
	Com_sprintf( filename, sizeof( filename ), "models/players/%s/lower.mdr", legsModelName );
	ci->legsModel = cgi_R_RegisterModel( filename );
	if ( !ci->legsModel ) 
	{//he's not skeletal, try the old way
		Com_sprintf( filename, sizeof( filename ), "models/players/%s/lower.md3", legsModelName );
		ci->legsModel = cgi_R_RegisterModel( filename );
		if ( !ci->legsModel )
		{
			Com_Printf( S_COLOR_RED"Failed to load model file %s\n", filename );
			return qfalse;
		}
	}

	if(torsoModelName && torsoModelName[0])
	{//You are trying to set one
		Com_sprintf( filename, sizeof( filename ), "models/players/%s/upper.mdr", torsoModelName );
		ci->torsoModel = cgi_R_RegisterModel( filename );
		if ( !ci->torsoModel ) 
		{//he's not skeletal, try the old way
			Com_sprintf( filename, sizeof( filename ), "models/players/%s/upper.md3", torsoModelName );
			ci->torsoModel = cgi_R_RegisterModel( filename );
			if ( !ci->torsoModel ) 
			{
				Com_Printf( S_COLOR_RED"Failed to load model file %s\n", filename );
				return qfalse;
			}
		}
	}
	else
	{
		ci->torsoModel = 0;
	}

	if(headModelName && headModelName[0])
	{//You are trying to set one
		Com_sprintf( filename, sizeof( filename ), "models/players/%s/head.md3", headModelName );
		ci->headModel = cgi_R_RegisterModel( filename );
		if ( !ci->headModel ) 
		{
			Com_Printf( S_COLOR_RED"Failed to load model file %s\n", filename );
			return qfalse;
		}
	}
	else
	{
		ci->headModel = 0;
	}


	// if any skins failed to load, return failure
	if ( !CG_RegisterClientSkin( ci, headModelName, headSkinName, torsoModelName, torsoSkinName, legsModelName, legsSkinName ) ) 
	{
		//Com_Printf( "Failed to load skin file: %s : %s/%s : %s/%s : %s\n", headModelName, headSkinName, torsoModelName, torsoSkinName, legsModelName, legsSkinName );
		return qfalse;
	}

	//FIXME: for now, uses the legs model dir for anim cfg, but should we set this in some sort of NPCs.cfg?
	// load the animation file set
	ci->animFileIndex = G_ParseAnimFileSet(legsModelName);
	if (ci->animFileIndex<0) 
	{
		Com_Printf( S_COLOR_RED"Failed to load animation file set models/players/%s\n", legsModelName );
		return qfalse;
	}
#endif
/*
Ghoul2 Insert End
*/
	return qtrue;
}


void CG_RegisterClientRenderInfo(clientInfo_t *ci, renderInfo_t *ri)
{
	char			*slash;
	char			headModelName[MAX_QPATH];
	char			torsoModelName[MAX_QPATH];
	char			legsModelName[MAX_QPATH];
	char			headSkinName[MAX_QPATH];
	char			torsoSkinName[MAX_QPATH];
	char			legsSkinName[MAX_QPATH];

	if(!ri->legsModelName || !ri->legsModelName[0])
	{//Must have at LEAST a legs model
		return;
	}

	Q_strncpyz( legsModelName, ri->legsModelName, sizeof( legsModelName ) );
	//Legs skin
	slash = strchr( legsModelName, '/' );
	if ( !slash ) 
	{
		// modelName didn not include a skin name
		Q_strncpyz( legsSkinName, "default", sizeof( legsSkinName ) );
	} 
	else 
	{
		Q_strncpyz( legsSkinName, slash + 1, sizeof( legsSkinName ) );
		// truncate modelName
		*slash = 0;
	}

	if(ri->torsoModelName && ri->torsoModelName[0])
	{
		Q_strncpyz( torsoModelName, ri->torsoModelName, sizeof( torsoModelName ) );
		//Torso skin
		slash = strchr( torsoModelName, '/' );
		if ( !slash ) 
		{
			// modelName didn't include a skin name
			Q_strncpyz( torsoSkinName, "default", sizeof( torsoSkinName ) );
		} 
		else 
		{
			Q_strncpyz( torsoSkinName, slash + 1, sizeof( torsoSkinName ) );
			// truncate modelName
			*slash = 0;
		}
	}
	else
	{
		torsoModelName[0] = 0;
	}

	//Head
	if(ri->headModelName && ri->headModelName[0])
	{
		Q_strncpyz( headModelName, ri->headModelName, sizeof( headModelName ) );
		//Head skin
		slash = strchr( headModelName, '/' );
		if ( !slash ) 
		{
			// modelName didn not include a skin name
			Q_strncpyz( headSkinName, "default", sizeof( headSkinName ) );
		} 
		else 
		{
			Q_strncpyz( headSkinName, slash + 1, sizeof( headSkinName ) );
			// truncate modelName
			*slash = 0;
		}
	}
	else
	{
		headModelName[0] = 0;
	}

	if ( !CG_RegisterClientModelname( ci, headModelName, headSkinName, torsoModelName, torsoSkinName, legsModelName, legsSkinName) ) 
	{
		if ( !CG_RegisterClientModelname( ci, DEFAULT_HEADMODEL, "default", DEFAULT_TORSOMODEL, "default", DEFAULT_LEGSMODEL, "default" ) ) 
		{
			CG_Error( "DEFAULT_MODELS failed to register");
		}
	}
}

//-------------------------------------
// CG_RegisterEffects
// 
// Handles precaching all effect files
//	and any shader, model, or sound
//	files an effect may use.
//-------------------------------------
extern void CG_InitGlass( void );
extern void	cgi_R_WorldEffectCommand( const char *command );

extern cvar_t *g_delayedShutdown;
static void CG_RegisterEffects( void )
{
	char	*effectName;
	int		i, numFailed=0;

	// Register external effects
	for ( i = 1 ; i < MAX_FX ; i++ ) 
	{
		effectName = ( char *)CG_ConfigString( CS_EFFECTS + i );

		if ( !effectName[0] ) 
		{
			break;
		}

		if (!theFxScheduler.RegisterEffect( (const char*)effectName ))
		{
			//assert(0);
			numFailed++;
		}
	}
	if (numFailed && g_delayedShutdown->integer)
	{
		//assert(0);
		//CG_Error( "CG_RegisterEffects: %i Effects failed to load.  Please fix, or ask Aurelio.", numFailed );
	}

	// Start world effects
	for ( i = 1 ; i < MAX_WORLD_FX ; i++ ) 
	{
		effectName = ( char *)CG_ConfigString( CS_WORLD_FX + i );

		if ( !effectName[0] ) 
		{
			break;
		}

		cgi_R_WorldEffectCommand( effectName );
	}

	// Set up the glass effects mini-system.
	CG_InitGlass();

	//footstep effects
	cgs.effects.footstepMud = theFxScheduler.RegisterEffect( "materials/mud" );
	cgs.effects.footstepSand = theFxScheduler.RegisterEffect( "materials/sand" );
	cgs.effects.footstepSnow = theFxScheduler.RegisterEffect( "materials/snow" );
	cgs.effects.footstepGravel = theFxScheduler.RegisterEffect( "materials/gravel" );
	//landing effects
	cgs.effects.landingMud = theFxScheduler.RegisterEffect( "materials/mud_large" );
	cgs.effects.landingSand = theFxScheduler.RegisterEffect( "materials/sand_large" );
	cgs.effects.landingDirt = theFxScheduler.RegisterEffect( "materials/dirt_large" );
	cgs.effects.landingSnow = theFxScheduler.RegisterEffect( "materials/snow_large" );
	cgs.effects.landingGravel = theFxScheduler.RegisterEffect( "materials/gravel_large" );
	//splashes
	if ( (gi.totalMapContents()&CONTENTS_WATER) )
	{
		theFxScheduler.RegisterEffect( "env/water_impact" );
		theFxScheduler.RegisterEffect( "misc/waterbreath" );
	}
	if ( (gi.totalMapContents()&CONTENTS_LAVA) )
	{
		theFxScheduler.RegisterEffect( "env/lava_splash" );
	}
	if ( (gi.totalMapContents()&CONTENTS_SLIME) )
	{
		theFxScheduler.RegisterEffect( "env/acid_splash" );
	}
	theFxScheduler.RegisterEffect( "misc/breath" );
}

/*
void CG_RegisterClientModels (int entityNum)

Only call if clientInfo->infoValid is not true

For players and NPCs to register their models
*/
void CG_RegisterClientModels (int entityNum)
{
	gentity_t		*ent;

	if(entityNum < 0 || entityNum > ENTITYNUM_WORLD)
	{
		return;
	}

	ent = &g_entities[entityNum];

	if(!ent->client)
	{
		return;
	}

	ent->client->clientInfo.infoValid = qtrue;

	if ( ent->playerModel != -1 && ent->ghoul2.size() )
	{
		return;
	}

	CG_RegisterClientRenderInfo(&ent->client->clientInfo, &ent->client->renderInfo);

	if(entityNum < MAX_CLIENTS)
	{
		memcpy(&cgs.clientinfo[entityNum], &ent->client->clientInfo, sizeof(clientInfo_t));
	}
}

//===================================================================================


HUDMenuItem_t forceTics[] = 
{
	 { "rightHUD", "force_tic1", 0,  0,  0,  0, { 0.0f, 0.0f, 0.0f, 0.0f }, NULL_HANDLE }, 	// Top 
	 { "rightHUD", "force_tic2", 0,  0,  0,  0, { 0.0f, 0.0f, 0.0f, 0.0f }, NULL_HANDLE }, 	// Top 
	 { "rightHUD", "force_tic3", 0,  0,  0,  0, { 0.0f, 0.0f, 0.0f, 0.0f }, NULL_HANDLE }, 	// Top 
	 { "rightHUD", "force_tic4", 0,  0,  0,  0, { 0.0f, 0.0f, 0.0f, 0.0f }, NULL_HANDLE }, 	// Top 
};

HUDMenuItem_t ammoTics[] = 
{
	{ "rightHUD", "ammo_tic1", 0,  0,  0,  0, { 0.0f, 0.0f, 0.0f, 0.0f }, NULL_HANDLE }, 	// Top 
	{ "rightHUD", "ammo_tic2", 0,  0,  0,  0, { 0.0f, 0.0f, 0.0f, 0.0f }, NULL_HANDLE }, 	// Top 
	{ "rightHUD", "ammo_tic3", 0,  0,  0,  0, { 0.0f, 0.0f, 0.0f, 0.0f }, NULL_HANDLE }, 	// Top 
	{ "rightHUD", "ammo_tic4", 0,  0,  0,  0, { 0.0f, 0.0f, 0.0f, 0.0f }, NULL_HANDLE }, 	// Top 
};

HUDMenuItem_t armorTics[] = 
{
	{ "leftHUD", "armor_tic1", 0,  0,  0,  0, { 0.0f, 0.0f, 0.0f, 0.0f }, NULL_HANDLE }, 	// Top 
	{ "leftHUD", "armor_tic2", 0,  0,  0,  0, { 0.0f, 0.0f, 0.0f, 0.0f }, NULL_HANDLE }, 	
	{ "leftHUD", "armor_tic3", 0,  0,  0,  0, { 0.0f, 0.0f, 0.0f, 0.0f }, NULL_HANDLE }, 	
	{ "leftHUD", "armor_tic4", 0,  0,  0,  0, { 0.0f, 0.0f, 0.0f, 0.0f }, NULL_HANDLE }, 	
};

HUDMenuItem_t healthTics[] = 
{
	{ "leftHUD", "health_tic1", 0,  0,  0,  0, { 0.0f, 0.0f, 0.0f, 0.0f }, NULL_HANDLE }, 	// Top 
	{ "leftHUD", "health_tic2", 0,  0,  0,  0, { 0.0f, 0.0f, 0.0f, 0.0f }, NULL_HANDLE },	// 
	{ "leftHUD", "health_tic3", 0,  0,  0,  0, { 0.0f, 0.0f, 0.0f, 0.0f }, NULL_HANDLE },	// 
	{ "leftHUD", "health_tic4", 0,  0,  0,  0, { 0.0f, 0.0f, 0.0f, 0.0f }, NULL_HANDLE },	// Bottom
};


HUDMenuItem_t otherHUDBits[] = 
{
	{ "lefthud", "healthamount",			0,  0,  0,  0, { 0.0f, 0.0f, 0.0f, 0.0f }, NULL_HANDLE },	// OHB_HEALTHAMOUNT
	{ "lefthud", "armoramount",			0,  0,  0,  0, { 0.0f, 0.0f, 0.0f, 0.0f }, NULL_HANDLE },	// OHB_ARMORAMOUNT
	{ "righthud", "forceamount",			0,  0,  0,  0, { 0.0f, 0.0f, 0.0f, 0.0f }, NULL_HANDLE },	// OHB_FORCEAMOUNT 
	{ "righthud", "ammoamount",			0,  0,  0,  0, { 0.0f, 0.0f, 0.0f, 0.0f }, NULL_HANDLE },	// OHB_AMMOAMOUNT
	{ "righthud", "saberstyle_strong",	0,  0,  0,  0, { 0.0f, 0.0f, 0.0f, 0.0f }, NULL_HANDLE },	// OHB_SABERSTYLE_STRONG
	{ "righthud", "saberstyle_medium",	0,  0,  0,  0, { 0.0f, 0.0f, 0.0f, 0.0f }, NULL_HANDLE },	// OHB_SABERSTYLE_MEDIUM
	{ "righthud", "saberstyle_fast",		0,  0,  0,  0, { 0.0f, 0.0f, 0.0f, 0.0f }, NULL_HANDLE },	// OHB_SABERSTYLE_FAST
	{ "lefthud",	"scanline",				0,  0,  0,  0, { 0.0f, 0.0f, 0.0f, 0.0f }, NULL_HANDLE },	// OHB_SCANLINE_LEFT
	{ "righthud",	"scanline",				0,  0,  0,  0, { 0.0f, 0.0f, 0.0f, 0.0f }, NULL_HANDLE },	// OHB_SCANLINE_RIGHT
	{ "lefthud",	"frame",				0,  0,  0,  0, { 0.0f, 0.0f, 0.0f, 0.0f }, NULL_HANDLE },	// OHB_FRAME_LEFT
	{ "righthud",	"frame",				0,  0,  0,  0, { 0.0f, 0.0f, 0.0f, 0.0f }, NULL_HANDLE },	// OHB_FRAME_RIGHT
};

/*const char *HolocronIcons[] = {
	"gfx/mp/f_icon_lt_heal",		//FP_HEAL,
	"gfx/mp/f_icon_levitation",		//FP_LEVITATION,
	"gfx/mp/f_icon_speed",			//FP_SPEED,
	"gfx/mp/f_icon_push",			//FP_PUSH,
	"gfx/mp/f_icon_pull",			//FP_PULL,
	"gfx/mp/f_icon_lt_telepathy",	//FP_TELEPATHY,
	"gfx/mp/f_icon_dk_grip",		//FP_GRIP,
	"gfx/mp/f_icon_dk_l1",			//FP_LIGHTNING,
	"gfx/mp/f_icon_dk_rage",		//FP_RAGE,
	"gfx/mp/f_icon_lt_protect",		//FP_PROTECT,
	"gfx/mp/f_icon_lt_absorb",		//FP_ABSORB,
	"gfx/mp/f_icon_lt_healother",	//FP_TEAM_HEAL,
	"gfx/mp/f_icon_dk_forceother",	//FP_TEAM_FORCE,
	"gfx/mp/f_icon_dk_drain",		//FP_DRAIN,
	"gfx/mp/f_icon_sight",			//FP_SEE,
	"gfx/mp/f_icon_saber_attack",	//FP_SABERATTACK,
	"gfx/mp/f_icon_saber_defend",	//FP_SABERDEFEND,
	"gfx/mp/f_icon_saber_throw"		//FP_SABERTHROW
};
*/
extern void CG_NPC_Precache ( gentity_t *spawner );
qboolean NPCsPrecached = qfalse;
/*
=================
CG_PrepRefresh

Call before entering a new level, or after changing renderers
This function may execute for a couple of minutes with a slow disk.
=================
*/
void CG_CreateMiscEnts(void);
static void CG_RegisterGraphics( void ) {
	int			i;
	char		items[MAX_ITEMS+1];
	int			breakPoint = 0;
	const char		*sb_nums[11] = {
		"gfx/2d/numbers/zero",
		"gfx/2d/numbers/one",
		"gfx/2d/numbers/two",
		"gfx/2d/numbers/three",
		"gfx/2d/numbers/four",
		"gfx/2d/numbers/five",
		"gfx/2d/numbers/six",
		"gfx/2d/numbers/seven",
		"gfx/2d/numbers/eight",
		"gfx/2d/numbers/nine",
		"gfx/2d/numbers/minus",
	};

	const char		*sb_t_nums[11] = {
		"gfx/2d/numbers/t_zero",
		"gfx/2d/numbers/t_one",
		"gfx/2d/numbers/t_two",
		"gfx/2d/numbers/t_three",
		"gfx/2d/numbers/t_four",
		"gfx/2d/numbers/t_five",
		"gfx/2d/numbers/t_six",
		"gfx/2d/numbers/t_seven",
		"gfx/2d/numbers/t_eight",
		"gfx/2d/numbers/t_nine",
		"gfx/2d/numbers/t_minus",
	};

	const char		*sb_c_nums[11] = {
		"gfx/2d/numbers/c_zero",
		"gfx/2d/numbers/c_one",
		"gfx/2d/numbers/c_two",
		"gfx/2d/numbers/c_three",
		"gfx/2d/numbers/c_four",
		"gfx/2d/numbers/c_five",
		"gfx/2d/numbers/c_six",
		"gfx/2d/numbers/c_seven",
		"gfx/2d/numbers/c_eight",
		"gfx/2d/numbers/c_nine",
		"gfx/2d/numbers/t_minus", //?????
	};

	// Clean, then register...rinse...repeat...
	CG_LoadingString( "effects" );
	FX_Init();
	CG_RegisterEffects();

	// clear any references to old media
	memset( &cg.refdef, 0, sizeof( cg.refdef ) );
	cgi_R_ClearScene();

	cg.loadLCARSStage = 3;
	CG_LoadingString( cgs.mapname );

	cgi_R_LoadWorldMap( cgs.mapname );

	cg.loadLCARSStage = 4;
	CG_LoadingString( "game media shaders" );

	for ( i=0; i < 11; i++ )
	{
		cgs.media.numberShaders[i]			= cgi_R_RegisterShaderNoMip( sb_nums[i] );
		cgs.media.smallnumberShaders[i]		= cgi_R_RegisterShaderNoMip( sb_t_nums[i] );
		cgs.media.chunkyNumberShaders[i]	= cgi_R_RegisterShaderNoMip( sb_c_nums[i] );
	}

	// FIXME: conditionally do this??  Something must be wrong with inventory item caching..?
	cgi_R_RegisterModel( "models/items/remote.md3" );

	cgs.media.explosionModel				= cgi_R_RegisterModel ( "models/weaphits/explosion.md3" );
	cgs.media.surfaceExplosionShader		= cgi_R_RegisterShader( "surfaceExplosion" );

	cgs.media.halfShieldModel				= cgi_R_RegisterModel( "models/weaphits/testboom.md3" );

	cgs.media.solidWhiteShader			= cgi_R_RegisterShader( "gfx/effects/solidWhite" );
	cgs.media.refractShader				= cgi_R_RegisterShader( "effects/refraction" );

	//on players
	cgs.media.personalShieldShader		= cgi_R_RegisterShader( "gfx/misc/personalshield" );
	cgs.media.cloakedShader				= cgi_R_RegisterShader( "gfx/effects/cloakedShader" );
											cgi_R_RegisterShader( "gfx/misc/ion_shield" );

	cgs.media.boltShader				= cgi_R_RegisterShader( "gfx/misc/blueLine" );

	// FIXME: do these conditionally
	cgi_R_RegisterShader( "gfx/2d/workingCamera" );
	cgi_R_RegisterShader( "gfx/2d/brokenCamera" );
	//cgi_R_RegisterShader( "gfx/effects/irid_shield" ); // for galak, but he doesn't have his own weapon so I can't register the shader there.

	//interface
	for ( i = 0 ; i < NUM_CROSSHAIRS ; i++ ) {
		cgs.media.crosshairShader[i] = cgi_R_RegisterShaderNoMip( va("gfx/2d/crosshair%c", 'a'+i) );
	}
	cgs.media.backTileShader		= cgi_R_RegisterShader( "gfx/2d/backtile" );
//	cgs.media.noammoShader			= cgi_R_RegisterShaderNoMip( "gfx/hud/noammo");
	cgs.media.weaponIconBackground	= cgi_R_RegisterShaderNoMip( "gfx/hud/background");
	cgs.media.forceIconBackground	= cgi_R_RegisterShaderNoMip( "gfx/hud/background_f");
	cgs.media.inventoryIconBackground= cgi_R_RegisterShaderNoMip( "gfx/hud/background_i");
	cgs.media.dataPadFrame			= cgi_R_RegisterShaderNoMip( "gfx/menus/datapad");

	//gore decal shaders -rww
	cgs.media.bdecal_burnmark1		= cgi_R_RegisterShader( "gfx/damage/burnmark1" );
	cgs.media.bdecal_saberglowmark	= cgi_R_RegisterShader( "gfx/damage/saberglowmark" );

	cg.loadLCARSStage = 5;
	CG_LoadingString( "game media models" );
	
	// Chunk models
	//FIXME: jfm:? bother to conditionally load these if an ent has this material type?
	for ( i = 0; i < NUM_CHUNK_MODELS; i++ )
	{
		cgs.media.chunkModels[CHUNK_METAL2][i]	= cgi_R_RegisterModel( va( "models/chunks/metal/metal1_%i.md3", i+1 ) ); //_ /switched\ _
		cgs.media.chunkModels[CHUNK_METAL1][i]	= cgi_R_RegisterModel( va( "models/chunks/metal/metal2_%i.md3", i+1 ) ); //  \switched/
		cgs.media.chunkModels[CHUNK_ROCK1][i]	= cgi_R_RegisterModel( va( "models/chunks/rock/rock1_%i.md3", i+1 ) );
		cgs.media.chunkModels[CHUNK_ROCK2][i]	= cgi_R_RegisterModel( va( "models/chunks/rock/rock2_%i.md3", i+1 ) );
		cgs.media.chunkModels[CHUNK_ROCK3][i]	= cgi_R_RegisterModel( va( "models/chunks/rock/rock3_%i.md3", i+1 ) );
		cgs.media.chunkModels[CHUNK_CRATE1][i]	= cgi_R_RegisterModel( va( "models/chunks/crate/crate1_%i.md3", i+1 ) );
		cgs.media.chunkModels[CHUNK_CRATE2][i]	= cgi_R_RegisterModel( va( "models/chunks/crate/crate2_%i.md3", i+1 ) );
		cgs.media.chunkModels[CHUNK_WHITE_METAL][i]	= cgi_R_RegisterModel( va( "models/chunks/metal/wmetal1_%i.md3", i+1 ) );
	}

	cgs.media.chunkSound			= cgi_S_RegisterSound("sound/weapons/explosions/glasslcar");
	cgs.media.grateSound			= cgi_S_RegisterSound( "sound/effects/grate_destroy" );
	cgs.media.rockBreakSound		= cgi_S_RegisterSound("sound/effects/wall_smash");
	cgs.media.rockBounceSound[0]	= cgi_S_RegisterSound("sound/effects/stone_bounce");
	cgs.media.rockBounceSound[1]	= cgi_S_RegisterSound("sound/effects/stone_bounce2");
	cgs.media.metalBounceSound[0]	= cgi_S_RegisterSound("sound/effects/metal_bounce");
	cgs.media.metalBounceSound[1]	= cgi_S_RegisterSound("sound/effects/metal_bounce2");
	cgs.media.glassChunkSound		= cgi_S_RegisterSound("sound/weapons/explosions/glassbreak1");
	cgs.media.crateBreakSound[0]	= cgi_S_RegisterSound("sound/weapons/explosions/crateBust1" );
	cgs.media.crateBreakSound[1]	= cgi_S_RegisterSound("sound/weapons/explosions/crateBust2" );

	cgs.media.weaponbox	 = cgi_R_RegisterShaderNoMip( "gfx/interface/weapon_box");

	//Models & Shaders
	cgs.media.damageBlendBlobShader	= cgi_R_RegisterShader( "gfx/misc/borgeyeflare" );

	cg.loadLCARSStage = 6;

	cgs.media.messageLitOn = cgi_R_RegisterShaderNoMip( "gfx/hud/message_on" );
	cgs.media.messageLitOff = cgi_R_RegisterShaderNoMip( "gfx/hud/message_off" );
	cgs.media.messageObjCircle = cgi_R_RegisterShaderNoMip( "gfx/hud/objective_circle" );

	cgs.media.DPForcePowerOverlay = cgi_R_RegisterShader( "gfx/hud/force_swirl" );

	//NOTE: we should only cache this if there is a vehicle or emplaced gun somewhere on the map
	cgs.media.emplacedHealthBarShader = cgi_R_RegisterShaderNoMip( "gfx/hud/health_frame" );

	// battery charge shader when using a gonk
	cgs.media.batteryChargeShader = cgi_R_RegisterShader( "gfx/2d/battery" );
	cgi_R_RegisterShader( "gfx/2d/droid_view" );
	cgs.media.useableHint = cgi_R_RegisterShader("gfx/hud/useableHint");

	// Load up other HUD bits
	for (i=0;i<OHB_MAX;i++)
	{
		cgi_UI_GetMenuItemInfo(
			otherHUDBits[i].menuName,
			otherHUDBits[i].itemName,
			&otherHUDBits[i].xPos,
			&otherHUDBits[i].yPos,
			&otherHUDBits[i].width,
			&otherHUDBits[i].height,
			otherHUDBits[i].color,
			&otherHUDBits[i].background);
	}

	// Get all the info for each HUD piece
	for (i=0;i<MAX_HUD_TICS;i++)
	{
		cgi_UI_GetMenuItemInfo(
			healthTics[i].menuName,
			healthTics[i].itemName,
			&healthTics[i].xPos,
			&healthTics[i].yPos,
			&healthTics[i].width,
			&healthTics[i].height,
			healthTics[i].color,
			&healthTics[i].background);

		cgi_UI_GetMenuItemInfo(
			armorTics[i].menuName,
			armorTics[i].itemName,
			&armorTics[i].xPos,
			&armorTics[i].yPos,
			&armorTics[i].width,
			&armorTics[i].height,
			armorTics[i].color,
			&armorTics[i].background);

		cgi_UI_GetMenuItemInfo(
			forceTics[i].menuName,
			forceTics[i].itemName,
			&forceTics[i].xPos,
			&forceTics[i].yPos,
			&forceTics[i].width,
			&forceTics[i].height,
			forceTics[i].color,
			&forceTics[i].background);

		cgi_UI_GetMenuItemInfo(
			ammoTics[i].menuName,
			ammoTics[i].itemName,
			&ammoTics[i].xPos,
			&ammoTics[i].yPos,
			&ammoTics[i].width,
			&ammoTics[i].height,
			ammoTics[i].color,
			&ammoTics[i].background);

	}


	memset( cg_items, 0, sizeof( cg_items ) );
	memset( cg_weapons, 0, sizeof( cg_weapons ) );

	// only register the items that the server says we need
	Q_strncpyz(items, CG_ConfigString(CS_ITEMS), sizeof(items));

	for ( i = 1 ; i < bg_numItems ; i++ ) {
		if ( items[ i ] == '1' ) 
		{
			if (bg_itemlist[i].classname)
			{
				CG_LoadingString( bg_itemlist[i].classname );
				CG_RegisterItemVisuals( i );
			}
		}
		if (bg_itemlist[i].giType == IT_HOLDABLE)
		{
			if (bg_itemlist[i].giTag < INV_MAX)
			{
				inv_icons[bg_itemlist[i].giTag] = cgi_R_RegisterShaderNoMip( bg_itemlist[i].icon );
			}
		}
	}

	//precache force power icons here
/*	i = 0;
	while (i < NUM_FORCE_POWERS)
	{
		cgs.media.forcePowerIcons[i] = cgi_R_RegisterShaderNoMip(HolocronIcons[i]);
		i++;
	}
*/
	cgs.media.rageRecShader = cgi_R_RegisterShaderNoMip("gfx/mp/f_icon_ragerec");
	//etc.
	cgi_R_RegisterShader( "gfx/misc/test_crackle" );	//CG_DoGlassQuad

	// wall marks
	cgs.media.scavMarkShader				= cgi_R_RegisterShader( "gfx/damage/burnmark4" );
	cgs.media.rivetMarkShader				= cgi_R_RegisterShader( "gfx/damage/rivetmark" );

	// doing one shader just makes it look like a shell.  By using two shaders with different bulge offsets and different texture scales, it has a much more chaotic look
	cgs.media.electricBodyShader			= cgi_R_RegisterShader( "gfx/misc/electric" );
	cgs.media.electricBody2Shader			= cgi_R_RegisterShader( "gfx/misc/fullbodyelectric2" );

	cgs.media.shadowMarkShader	= cgi_R_RegisterShader( "markShadow" );
	cgs.media.wakeMarkShader	= cgi_R_RegisterShader( "wake" );
	cgs.media.fsrMarkShader	= cgi_R_RegisterShader( "footstep_r" );
	cgs.media.fslMarkShader	= cgi_R_RegisterShader( "footstep_l" );
	cgs.media.fshrMarkShader	= cgi_R_RegisterShader( "footstep_heavy_r" );
	cgs.media.fshlMarkShader	= cgi_R_RegisterShader( "footstep_heavy_l" );
	cgi_S_RegisterSound( "sound/effects/energy_crackle.wav" );

	CG_LoadingString("map brushes");
	// register the inline models
	breakPoint = cgs.numInlineModels = cgi_CM_NumInlineModels();
	assert ((size_t)cgs.numInlineModels < sizeof(cgs.inlineDrawModel)/sizeof(cgs.inlineDrawModel[0]) );
	for ( i = 1 ; i < cgs.numInlineModels ; i++ ) {
		char	name[10];
		vec3_t			mins, maxs;
		int				j;

		Com_sprintf( name, sizeof(name), "*%i", i );
		cgs.inlineDrawModel[i] = cgi_R_RegisterModel( name );

		if (!cgs.inlineDrawModel[i])
		{
			breakPoint = i;
			break;
		}

		cgi_R_ModelBounds( cgs.inlineDrawModel[i], mins, maxs );
		for ( j = 0 ; j < 3 ; j++ ) {
			cgs.inlineModelMidpoints[i][j] = mins[j] + 0.5 * ( maxs[j] - mins[j] );
		}
	}

	cg.loadLCARSStage = 7;
	CG_LoadingString("map models");
	// register all the server specified models
	for (i=1 ; i<MAX_MODELS ; i++) {
		const char		*modelName;

		modelName = CG_ConfigString( CS_MODELS+i );
		if ( !modelName[0] ) {
			break;
		}
		cgs.model_draw[i] = cgi_R_RegisterModel( modelName );
//		OutputDebugString(va("### CG_RegisterGraphics(): cgs.model_draw[%d] = \"%s\"\n",i,modelName));
	}

	cg.loadLCARSStage = 8;

/*
Ghoul2 Insert Start
*/
	CG_LoadingString("skins");
	// register all the server specified models
	for (i=1 ; i<MAX_CHARSKINS ; i++) {
		const char		*modelName;

		modelName = CG_ConfigString( CS_CHARSKINS+i );
		if ( !modelName[0] ) {
			break;
		}
		cgs.skins[i] = cgi_R_RegisterSkin( modelName );
	}

/*
Ghoul2 Insert End
*/

	for (i=0 ; i<MAX_CLIENTS ; i++) 
	{
		//feedback( va("client %i", i ) );
		CG_NewClientinfo( i );
	}

	for (i=0 ; i < ENTITYNUM_WORLD ; i++)
	{
		if(&g_entities[i])
		{
			if(g_entities[i].client)
			{
				//if(!g_entities[i].client->clientInfo.infoValid)
				//We presume this
				{
					CG_LoadingString( va("client %s", g_entities[i].client->clientInfo.name ) );
					CG_RegisterClientModels(i);
					if ( i != 0 )
					{//Client weapons already precached
						CG_RegisterWeapon( g_entities[i].client->ps.weapon );
						CG_RegisterGun ( g_entities[i].client->gs.guns );
						if ( g_entities[i].client->ps.saber[0].g2MarksShader[0] )
						{
							cgi_R_RegisterShader( g_entities[i].client->ps.saber[0].g2MarksShader );
						}
						if ( g_entities[i].client->ps.saber[0].g2MarksShader2[0] )
						{
							cgi_R_RegisterShader( g_entities[i].client->ps.saber[0].g2MarksShader2 );
						}
						if ( g_entities[i].client->ps.saber[0].g2WeaponMarkShader[0] )
						{
							cgi_R_RegisterShader( g_entities[i].client->ps.saber[0].g2WeaponMarkShader );
						}
						if ( g_entities[i].client->ps.saber[0].g2WeaponMarkShader2[0] )
						{
							cgi_R_RegisterShader( g_entities[i].client->ps.saber[0].g2WeaponMarkShader2 );
						}
						if ( g_entities[i].client->ps.saber[1].g2MarksShader[0] )
						{
							cgi_R_RegisterShader( g_entities[i].client->ps.saber[1].g2MarksShader );
						}
						if ( g_entities[i].client->ps.saber[1].g2MarksShader2[0] )
						{
							cgi_R_RegisterShader( g_entities[i].client->ps.saber[1].g2MarksShader2 );
						}
						if ( g_entities[i].client->ps.saber[1].g2WeaponMarkShader[0] )
						{
							cgi_R_RegisterShader( g_entities[i].client->ps.saber[1].g2WeaponMarkShader );
						}
						if ( g_entities[i].client->ps.saber[1].g2WeaponMarkShader2[0] )
						{
							cgi_R_RegisterShader( g_entities[i].client->ps.saber[1].g2WeaponMarkShader2 );
						}
						CG_RegisterNPCCustomSounds( &g_entities[i].client->clientInfo );
						//CG_RegisterNPCEffects( g_entities[i].client->playerTeam );
					}
				}
			}
			else if ( g_entities[i].svFlags & SVF_NPC_PRECACHE && g_entities[i].NPC_type && g_entities[i].NPC_type[0] )
			{//Precache the NPC_type
				//FIXME: make sure we didn't precache this NPC_type already
				CG_LoadingString( va("NPC %s", g_entities[i].NPC_type ) );
				/*
				if (g_entities[i].classname && g_entities[i].classname[0] && Q_stricmp( g_entities[i].classname, "NPC_Vehicle" ) == 0)
				{
					// Get The Index, And Make Sure To Register All The Skins
					int iVehIndex = BG_VehicleGetIndex( g_entities[i].NPC_type );
				}
				else
				*/
				{
					CG_NPC_Precache( &g_entities[i] );
				}
			}
		}
	}

	CG_LoadingString( "static models" );
	CG_CreateMiscEnts();

	cg.loadLCARSStage = 9;

	NPCsPrecached = qtrue;

	extern	cvar_t	*com_buildScript;

	if (com_buildScript->integer) {
		cgi_R_RegisterShader( "gfx/misc/nav_cpoint" );
		cgi_R_RegisterShader( "gfx/misc/nav_line" );
		cgi_R_RegisterShader( "gfx/misc/nav_arrow" );
		cgi_R_RegisterShader( "gfx/misc/nav_node" );
	}

	for(i = 1; i < MAX_SUB_BSP; i++)
	{
		const char		*bspName = 0;
		vec3_t			mins, maxs;
		int				j = 0;
		int				sub = 0;
		char			temp[MAX_QPATH];

		bspName = CG_ConfigString( CS_BSP_MODELS+i );
		if ( !bspName[0] ) 
		{
			break;
		}
		CG_LoadingString( "BSP instances" );

		cgi_CM_LoadMap( bspName, qtrue );
		cgs.inlineDrawModel[breakPoint] = cgi_R_RegisterModel( bspName );
		cgi_R_ModelBounds( cgs.inlineDrawModel[breakPoint], mins, maxs );
		for ( j = 0 ; j < 3 ; j++ ) 
		{
			cgs.inlineModelMidpoints[breakPoint][j] = mins[j] + 0.5 * ( maxs[j] - mins[j] );
		}
		breakPoint++;
		for(sub=1;sub<MAX_MODELS;sub++)
		{
			Com_sprintf(temp, MAX_QPATH, "*%d-%d", i, sub);
			cgs.inlineDrawModel[breakPoint] = cgi_R_RegisterModel( temp );
			if (!cgs.inlineDrawModel[breakPoint])
			{
				break;
			}
			cgi_R_ModelBounds( cgs.inlineDrawModel[breakPoint], mins, maxs );
			for ( j = 0 ; j < 3 ; j++ ) 
			{
				cgs.inlineModelMidpoints[breakPoint][j] = mins[j] + 0.5 * ( maxs[j] - mins[j] );
			}
			breakPoint++;
		}
	}

	const char	*terrainInfo;
	int			terrainID;

	for(i = 1; i < MAX_TERRAINS; i++)
	{
		terrainInfo = CG_ConfigString( CS_TERRAINS + i );
		if ( !terrainInfo[0] )
		{
			break;
		}
		CG_LoadingString("Creating terrain");

		terrainID = cgi_CM_RegisterTerrain(terrainInfo);

		cgi_RMG_Init(terrainID, terrainInfo);

		// Send off the terrainInfo to the renderer
		cgi_RE_InitRendererTerrain( terrainInfo );
	}
}

//===========================================================================

/*
=================
CG_ConfigString
=================
*/
const char *CG_ConfigString( int index ) {
	if ( index < 0 || index >= MAX_CONFIGSTRINGS ) {
		CG_Error( "CG_ConfigString: bad index: %i", index );
	}
	return cgs.gameState.stringData + cgs.gameState.stringOffsets[ index ];
}

//==================================================================

void CG_LinkCentsToGents(void)
{
	int	i;

	for(i = 0; i < MAX_GENTITIES; i++)
	{
		cg_entities[i].gent = &g_entities[i];
	}
}

/*
======================
CG_StartMusic

======================
*/
void CG_StartMusic( qboolean bForceStart ) {
	const char	*s;
	char	parm1[MAX_QPATH], parm2[MAX_QPATH];

	// start the background music
	s = (char *)CG_ConfigString( CS_MUSIC );
	COM_BeginParseSession();
	Q_strncpyz( parm1, COM_Parse( &s ), sizeof( parm1 ) );
	Q_strncpyz( parm2, COM_Parse( &s ), sizeof( parm2 ) );
	COM_EndParseSession();

	cgi_S_StartBackgroundTrack( parm1, parm2, !bForceStart );
}

/*
======================
CG_GameStateReceived

Displays the info screen while loading media
======================
*/

int iCGResetCount=0;
qboolean qbVidRestartOccured = qfalse;

//===================
qboolean gbUseTheseValuesFromLoadSave = qfalse;	// MUST default to this
int gi_cg_forcepowerSelect;
int gi_cg_inventorySelect;
//===================


static void CG_GameStateReceived( void ) {
	// clear everything
	
	extern void CG_ClearAnimEvtCache( void );
	CG_ClearAnimEvtCache();	// else sound handles wrong after vid_restart

	qbVidRestartOccured = qtrue;
	iCGResetCount++;
	if (iCGResetCount == 1)	// this will only equal 1 first time, after each vid_restart it just gets higher.
	{						//	This non-clear is so the user can vid_restart during scrolling text without losing it.
		qbVidRestartOccured = qfalse;		
	}

	if (!qbVidRestartOccured)
	{
		CG_Init_CG();
		cg.weaponSelect = WP_NONE;
		cg.forcepowerSelect = FP_HEAL;
	}

	memset( cg_weapons, 0, sizeof(cg_weapons) );
	memset( cg_items, 0, sizeof(cg_items) );
	
	CG_LinkCentsToGents();

	if (gbUseTheseValuesFromLoadSave)
	{
		gbUseTheseValuesFromLoadSave = qfalse;	// ack
		cg.forcepowerSelect = gi_cg_forcepowerSelect;
		cg.inventorySelect	= gi_cg_inventorySelect;
	}


	// get the rendering configuration from the client system
	cgi_GetGlconfig( &cgs.glconfig );

/*	cgs.charScale = cgs.glconfig.vidHeight * (1.0/480.0);
	if ( cgs.glconfig.vidWidth * 480 > cgs.glconfig.vidHeight * 640 ) {
		// wide screen
		cgs.bias = 0.5 * ( cgs.glconfig.vidWidth - ( cgs.glconfig.vidHeight * (640.0/480.0) ) );
	}
	else {
		// no wide screen
		cgs.bias = 0;
	}
*/
	// get the gamestate from the client system
	cgi_GetGameState( &cgs.gameState );

	CG_ParseServerinfo();

	// load the new map
	cgs.media.levelLoad = cgi_R_RegisterShaderNoMip( "gfx/hud/mp_levelload" );
	CG_LoadingString( "collision map" );

	cgi_CM_LoadMap( cgs.mapname, qfalse );

	CG_RegisterSounds();

	CG_RegisterGraphics();

	//jfm: moved down to preinit
//	CG_InitLocalEntities();
//	CG_InitMarkPolys();

	CG_LoadingString( "music" );
	CG_StartMusic( qfalse );

	// remove the last loading update
	cg.infoScreenText[0] = 0;

	CGCam_Init();

	CG_ClearLightStyles();

	// Okay so this doesn't exactly belong here
	cg.messageLitActive = qfalse;
	cg.forceHUDActive = qtrue;
	cg.forceHUDTotalFlashTime = 0;
	cg.forceHUDNextFlashTime = 0;

}

void CG_WriteTheEvilCGHackStuff(void)
{
	gi.AppendToSaveGame(INT_ID('F','P','S','L'), &cg.forcepowerSelect, sizeof(cg.forcepowerSelect));
	gi.AppendToSaveGame(INT_ID('I','V','S','L'), &cg.inventorySelect,  sizeof(cg.inventorySelect));

}
void CG_ReadTheEvilCGHackStuff(void)
{
	gi.ReadFromSaveGame(INT_ID('F','P','S','L'), (void *)&gi_cg_forcepowerSelect, sizeof(gi_cg_forcepowerSelect), NULL);
	gi.ReadFromSaveGame(INT_ID('I','V','S','L'), (void *)&gi_cg_inventorySelect,  sizeof(gi_cg_inventorySelect), NULL);
	gbUseTheseValuesFromLoadSave = qtrue;
}

/*
Ghoul2 Insert Start
*/

// initialise the cg_entities structure
void CG_Init_CG(void)
{
	memset( &cg, 0, sizeof(cg));
}


#define MAX_MISC_ENTS	2000

typedef struct cgMiscEntData_s
{
	char		model[MAX_QPATH];
	qhandle_t	hModel;
	vec3_t		origin;
	vec3_t		angles;
	vec3_t		scale;
	float		radius;
	float		zOffset; //some models need a z offset for culling, because of stupid wrong model origins
} cgMiscEntData_t;

static cgMiscEntData_t	MiscEnts[MAX_MISC_ENTS]; //statically allocated for now.
static int				NumMiscEnts=0;

void CG_CreateMiscEntFromGent(gentity_t *ent, const vec3_t scale, float zOff)
{ //store the model data
	if (NumMiscEnts == MAX_MISC_ENTS)
	{
		Com_Error(ERR_DROP,"Maximum misc_model_static reached (%d)\n",MAX_MISC_ENTS);
		return;
	}

	if (!ent || !ent->model || !ent->model[0])
	{
		Com_Error(ERR_DROP, "misc_model_static with no model.");
		return;
	}
	const int len = strlen(ent->model);
	if (len < 4 || Q_stricmp(&ent->model[len-4],".md3")!=0)
	{
		Com_Error(ERR_DROP, "misc_model_static model(%s) is not an md3.",ent->model);
		return;
	}
	cgMiscEntData_t	*MiscEnt = &MiscEnts[NumMiscEnts++];
	memset(MiscEnt, 0, sizeof(*MiscEnt));

	strcpy(MiscEnt->model, ent->model);
	VectorCopy(ent->s.angles,	MiscEnt->angles);
	VectorCopy(scale,			MiscEnt->scale);
	VectorCopy(ent->s.origin,	MiscEnt->origin);
	MiscEnt->zOffset = zOff;
}

#define VectorScaleVector(a,b,c)		(((c)[0]=(a)[0]*(b)[0]),((c)[1]=(a)[1]*(b)[1]),((c)[2]=(a)[2]*(b)[2]))
//call on standard model load to spawn the queued stuff
void CG_CreateMiscEnts(void)
{
	vec3_t		mins, maxs;

	int i;
	for (i=0; i < NumMiscEnts; i++)
	{
		cgMiscEntData_t	*MiscEnt = &MiscEnts[i];

		MiscEnt->hModel = cgi_R_RegisterModel(MiscEnt->model);
		if (MiscEnt->hModel == 0)
		{
			Com_Error(ERR_DROP, "misc_model_static failed to load model '%s'",MiscEnt->model);
			continue;
		}

		cgi_R_ModelBounds(MiscEnt->hModel, mins, maxs);

		VectorScaleVector(mins, MiscEnt->scale, mins);
		VectorScaleVector(maxs, MiscEnt->scale, maxs);
		MiscEnt->radius = DistanceSquared(mins, maxs);
	}
}

void CG_DrawMiscEnts(void)
{
	int			i;
	cgMiscEntData_t	*MiscEnt = MiscEnts;
	refEntity_t	refEnt; 
	vec3_t		difference;
	vec3_t		cullOrigin;

	memset (&refEnt, 0, sizeof(refEnt));
	refEnt.reType = RT_MODEL;
	refEnt.frame = 0;
	refEnt.renderfx = RF_LIGHTING_ORIGIN;
	for(i=0;i<NumMiscEnts;i++)
	{
		VectorCopy(MiscEnt->origin, cullOrigin);
		cullOrigin[2] += MiscEnt->zOffset+1.0f;

		if (gi.inPVS(cg.refdef.vieworg, cullOrigin))
		{
			VectorSubtract(MiscEnt->origin, cg.refdef.vieworg, difference);
			if (VectorLengthSquared(difference)-(MiscEnt->radius) <= 8192*8192/*RMG_distancecull.value*/)
			{ //fixme: need access to the real cull dist here
				refEnt.hModel = MiscEnt->hModel;
				AnglesToAxis( MiscEnt->angles, refEnt.axis );
				VectorCopy(MiscEnt->scale, refEnt.modelScale);
				VectorCopy(MiscEnt->origin, refEnt.origin);
				VectorCopy(cullOrigin, refEnt.lightingOrigin);
				ScaleModelAxis(&refEnt);
				cgi_R_AddRefEntityToScene(&refEnt);
			}
		}
		MiscEnt++;
	}
}

void CG_TransitionPermanent(void)
{
	centity_t	*cent = cg_entities;
	int			i;

	cg_numpermanents = 0;
	for(i=0;i<MAX_GENTITIES;i++,cent++)
	{
		if (cgi_GetDefaultState(i, &cent->currentState))
		{
			cent->nextState = &cent->currentState;
			VectorCopy (cent->currentState.origin, cent->lerpOrigin);
			VectorCopy (cent->currentState.angles, cent->lerpAngles);
			cent->currentValid = qtrue;

			cg_permanents[cg_numpermanents++] = cent;
		}
	}
}

/*
Ghoul2 Insert End
*/


/*
=================
CG_PreInit

Called when DLL loads (after subsystem restart, but before gamestate is received)
=================
*/
void CG_PreInit() {
	CG_Init_CG();

	memset( &cgs, 0, sizeof( cgs ) );
	iCGResetCount = 0;

	CG_RegisterCvars();

//moved from CG_GameStateReceived because it's loaded sooner now
	CG_InitLocalEntities();

	CG_InitMarkPolys();
}

/*
=================
CG_Init

Called after every level change or subsystem restart
=================
*/
void CG_Init( int serverCommandSequence )
{

	// New Adding 
	gentity_t	*player = &g_entities[0];
	// 
	cgs.serverCommandSequence = serverCommandSequence;

	cgi_Cvar_Set( "cg_drawHUD", "1" );

	// fonts...
	//
	cgs.media.charsetShader = cgi_R_RegisterShaderNoMip("gfx/2d/charsgrid_med");

	cgs.media.qhFontSmall = cgi_R_RegisterFont("ocr_a");
	cgs.media.qhFontMedium = cgi_R_RegisterFont("ergoec");

	cgs.media.whiteShader   = cgi_R_RegisterShader( "white" );
	cgs.media.loadTick		= cgi_R_RegisterShaderNoMip( "gfx/hud/load_tick" );
	cgs.media.loadTickCap	= cgi_R_RegisterShaderNoMip( "gfx/hud/load_tick_cap" );
	// ALL ICON REGISTERED! 
	cgs.media.heal_icon =  cgi_R_RegisterShader( "gfx/hud/f_icon_lt_heal" );
	cgs.media.heal_v_icon =  cgi_R_RegisterShader( "gfx/hud/f_icon_v_heal" );
	cgs.media.heal_v_icon2 =  cgi_R_RegisterShader( "gfx/hud/f_icon_v_heal2" );
	cgs.media.heal_d_icon =  cgi_R_RegisterShader( "gfx/hud/f_icon_d_heal" );
	cgs.media.heal_h_icon =  cgi_R_RegisterShader( "gfx/hud/f_icon_h_heal" );
	cgs.media.heal_s_icon =  cgi_R_RegisterShader( "gfx/hud/f_icon_s_heal" );
	cgs.media.healother_icon =  cgi_R_RegisterShader( "gfx/hud/f_icon_healother" );
	

	cgs.media.jump_icon =  cgi_R_RegisterShader( "gfx/hud/f_icon_levitation" );
	cgs.media.pull_icon =  cgi_R_RegisterShader( "gfx/hud/f_icon_pull" );
	cgs.media.push_icon =  cgi_R_RegisterShader( "gfx/hud/f_icon_push" );
	cgs.media.speed_icon =  cgi_R_RegisterShader( "gfx/hud/f_icon_speed" );
	cgs.media.grip_icon =  cgi_R_RegisterShader( "gfx/hud/f_icon_dk_grip" );
	cgs.media.saber_attack =  cgi_R_RegisterShader( "gfx/hud/f_icon_saber_attack" );
	cgs.media.saber_throw =  cgi_R_RegisterShader( "gfx/hud/f_icon_saber_throw" );
	cgs.media.saber_defend = cgi_R_RegisterShader( "gfx/hud/f_icon_saber_defend" );
	cgs.media.icon_rage =  cgi_R_RegisterShader( "gfx/hud/f_icon_dk_rage" );
	cgs.media.icon_berserk = cgi_R_RegisterShader( "gfx/hud/f_icon_dk_berserk" );
	cgs.media.icon_trance = cgi_R_RegisterShader( "gfx/hud/f_icon_dk_trance" );
	cgs.media.icon_fear =  cgi_R_RegisterShader( "gfx/hud/f_icon_dk_fear" ); 
	cgs.media.icon_runeshield = cgi_R_RegisterShader( "gfx/hud/f_icon_dk_runeshield" );
	cgs.media.icon_tkshield = cgi_R_RegisterShader( "gfx/hud/f_icon_dk_tkshield" );
	cgs.media.icon_eldershield = cgi_R_RegisterShader( "gfx/hud/f_icon_dk_eldershield" );
	cgs.media.icon_poisonshield = cgi_R_RegisterShader( "gfx/hud/f_icon_dk_poisonshield" );
	cgs.media.icon_frostarmor = cgi_R_RegisterShader( "gfx/hud/f_icon_dk_frostarmor" );
	cgs.media.icon_crystalarmor = cgi_R_RegisterShader( "gfx/hud/f_icon_dk_crystalarmor" );
	cgs.media.icon_windshield = cgi_R_RegisterShader( "gfx/hud/f_icon_dk_windshield" );
	cgs.media.icon_stormshield = cgi_R_RegisterShader( "gfx/hud/f_icon_dk_stormshield" );
	cgs.media.icon_shockshield = cgi_R_RegisterShader( "gfx/hud_f_icon_dk_shockshield" ); 
	cgs.media.icon_cruel = cgi_R_RegisterShader( "gfx/hud/f_icon_dk_cruel" );
	cgs.media.icon_deathfear = cgi_R_RegisterShader( "gfx/hud/f_icon_dk_deathfear" );
	cgs.media.icon_firerage = cgi_R_RegisterShader( "gfx/hud_f_icon_fire_rage" ); 
	
	// REPELS 
	cgs.media.icon_protect = cgi_R_RegisterShader( "gfx/hud/f_icon_lt_protect" );
	cgs.media.icon_protect2 = cgi_R_RegisterShader( "gfx/hud/f_icon_lt_protect2" );
	cgs.media.icon_absorb = cgi_R_RegisterShader( "gfx/hud/f_icon_lt_absorb" );
	// DRAINS 
	cgs.media.icon_bloodgout = cgi_R_RegisterShader( "gfx/hud/f_icon_dk_drain" );
	cgs.media.icon_bloodshower = cgi_R_RegisterShader( "gfx/hud/f_icon_dk_drain2" );
	cgs.media.icon_souldrain = cgi_R_RegisterShader( "gfx/hud/f_icon_dk_souldrain" );
	cgs.media.icon_necrodrain = cgi_R_RegisterShader( "gfx/hud/f_icon_dk_necrodrain" );
	cgs.media.icon_hyldendrain = cgi_R_RegisterShader( "gfx/hud/f_icon_dk_hyldendrain" );
	cgs.media.icon_demondrain = cgi_R_RegisterShader( "gfx/hud/f_icon_dk_demondrain" );
	// special freezing skills
	cgs.media.icon_frostcoffin = cgi_R_RegisterShader( "gfx/hud/f_icon_frostcoffin" );
	cgs.media.icon_webbed = cgi_R_RegisterShader( "gfx/hud/f_icon_webbed" );
	cgs.media.icon_stoned = cgi_R_RegisterShader( "gfx/hud/f_icon_stoned" );
	cgs.media.icon_hollystasis = cgi_R_RegisterShader( "gfx/hud/f_icon_hollystasis" );
	cgs.media.icon_ivytrap = cgi_R_RegisterShader( "gfx/hud/f_icon_ivytrap" );
	cgs.media.icon_gravitytrap = cgi_R_RegisterShader( "gfx/hud/f_icon_gravitytrap" );


	// VAMPIRES DARK POWERS 
		cgs.media.icon_light = cgi_R_RegisterShader( "gfx/hud/f_icon_sight" );
		cgs.media.icon_revelation = cgi_R_RegisterShader( "gfx/hud/f_icon_sight2" );
		cgs.media.icon_echo = cgi_R_RegisterShader( "gfx/hud/f_icon_sight3" );
		cgs.media.icon_humanrevel = cgi_R_RegisterShader( "gfx/hud/f_icon_sight4" );
		cgs.media.icon_hyldenrevel = cgi_R_RegisterShader( "gfx/hud/f_icon_sight5" );
		cgs.media.icon_elderrevel = cgi_R_RegisterShader( "gfx/hud/f_icon_sight6" );
		cgs.media.icon_echo2 = cgi_R_RegisterShader( "gfx/hud/f_icon_echo" );
		cgs.media.icon_stun = cgi_R_RegisterShader( "gfx/hud/f_icon_stun" );
	cgs.media.icon_hate = cgi_R_RegisterShader( "gfx/hud/f_icon_hate" );
	cgs.media.icon_controlmind = cgi_R_RegisterShader( "gfx/hud/f_icon_controlmind" );
	cgs.media.icon_incapacitate = cgi_R_RegisterShader( "gfx/hud/f_icon_incapacitate" );
	cgs.media.icon_feartrick = cgi_R_RegisterShader ( "gfx/hud/f_icon_feartrick" ); 
	cgs.media.icon_proxy = cgi_R_RegisterShader ( "gfx/hud/f_icon_proxy" ); 
	cgs.media.icon_fear2 = cgi_R_RegisterShader( "gfx/hud/f_icon_fear" );
	cgs.media.icon_wrack = cgi_R_RegisterShader( "gfx/hud/f_icon_wrack" );
	
	// SARAFAN LIGHT POWERS 
		cgs.media.icon_holystun = cgi_R_RegisterShader( "gfx/hud/f_icon_holystun" );
	cgs.media.icon_hate = cgi_R_RegisterShader( "gfx/hud/f_icon_holyhate" );
	cgs.media.icon_controlmind = cgi_R_RegisterShader( "gfx/hud/f_icon_holycontrolmind" );
	cgs.media.icon_incapacitate = cgi_R_RegisterShader( "gfx/hud/f_icon_holyincapacitate" );
	cgs.media.icon_wrack = cgi_R_RegisterShader( "gfx/hud/f_icon_holywrack" );

	// FORMS  
	cgs.media.icon_batform =  cgi_R_RegisterShader( "gfx/hud/f_icon_batform" ); 
	cgs.media.icon_birdform = cgi_R_RegisterShader( "gfx/hud/f_icon_birdform" );
	cgs.media.icon_chyroptera = cgi_R_RegisterShader( "gfx/hud/f_icon_chyroptera" );
	cgs.media.icon_mistform = cgi_R_RegisterShader( "gfx/hud/f_icon_mistform" );
	cgs.media.icon_mistform2 = cgi_R_RegisterShader( "gfx/hud/f_icon_mistform2" );
	cgs.media.icon_mistform3 = cgi_R_RegisterShader( "gfx/hud/f_icon_mistform3" );
	cgs.media.icon_wolfform = cgi_R_RegisterShader( "gfx/hud/f_icon_wolfform" );
	cgs.media.icon_wolfform2 = cgi_R_RegisterShader( "gfx/hud/f_icon_wolfform2" );
	cgs.media.icon_hellwolf = cgi_R_RegisterShader( "gfx/hud_f_icon_wolfform3" ); 
	cgs.media.icon_lionform = cgi_R_RegisterShader( "gfx/hud/f_icon_lionform" );
	cgs.media.icon_battovampire = cgi_R_RegisterShader( "gfx/hud/f_icon_battovampire" );
	cgs.media.icon_misttovampire = cgi_R_RegisterShader( "gfx/hud_f_icon_misttovampire" ); 
	cgs.media.icon_wolftovampire = cgi_R_RegisterShader( "gfx/hud_f_icon_wolftovampire" ); 

	
	// GLYPH POWERS 
	cgs.media.icon_forceglyph = cgi_R_RegisterShader( "gfx/hud/f_icon_forceglyph" );
	cgs.media.icon_stoneglyph = cgi_R_RegisterShader( "gfx/hud/f_icon_stoneglyph" );
	cgs.media.icon_soundglyph = cgi_R_RegisterShader( "gfx/hud/f_icon_soundglyph" );
	cgs.media.icon_waterglyph = cgi_R_RegisterShader( "gfx/hud/f_icon_waterglyph" );
	cgs.media.icon_fireglyph = cgi_R_RegisterShader( "gfx/hud/f_icon_fireglyph" );
	cgs.media.icon_sunglyph = cgi_R_RegisterShader( "gfx/hud/f_icon_sunglyph" );
	// THE REAVERS 
	cgs.media.icon_darkreaver =  cgi_R_RegisterShader( "gfx/hud/f_icon_darkreaver" );
	cgs.media.icon_smogreaver =  cgi_R_RegisterShader( "gfx/hud/f_icon_smogreaver" );
	cgs.media.icon_voidreaver =  cgi_R_RegisterShader( "gfx/hud/f_icon_voidreaver" );
	cgs.media.icon_lightreaver =  cgi_R_RegisterShader( "gfx/hud/f_icon_lightreaver" );
	cgs.media.icon_lightningreaver =  cgi_R_RegisterShader( "gfx/hud/f_icon_lighningreaver" );
	cgs.media.icon_sunreaver =  cgi_R_RegisterShader( "gfx/hud/f_icon_sunreaver" );
	cgs.media.icon_firereaver =  cgi_R_RegisterShader( "gfx/hud/f_icon_firereaver" );
	cgs.media.icon_warmreaver =  cgi_R_RegisterShader( "gfx/hud/f_icon_warmreaver" );
	cgs.media.icon_magmareaver =  cgi_R_RegisterShader( "gfx/hud/f_icon_magmareaver" );
	cgs.media.icon_airreaver =  cgi_R_RegisterShader( "gfx/hud/f_icon_airreaver" );
	cgs.media.icon_fogreaver =  cgi_R_RegisterShader( "gfx/hud/f_icon_fogreaver" );
	cgs.media.icon_stormreaver =  cgi_R_RegisterShader( "gfx/hud/f_icon_stormreaver" );
	cgs.media.icon_waterreaver =  cgi_R_RegisterShader( "gfx/hud/f_icon_waterreaver" );
	cgs.media.icon_icereaver =  cgi_R_RegisterShader( "gfx/hud/f_icon_icereaver" );
	cgs.media.icon_crystalreaver =  cgi_R_RegisterShader( "gfx/hud/f_icon_crystalreaver" );
	cgs.media.icon_earthreaver =  cgi_R_RegisterShader( "gfx/hud/f_icon_earthreaver" );
	cgs.media.icon_mudreaver =  cgi_R_RegisterShader( "gfx/hud/f_icon_mudreaver" );
	cgs.media.icon_stonereaver =  cgi_R_RegisterShader( "gfx/hud/f_icon_stonereaver" );
	cgs.media.icon_dustreaver =  cgi_R_RegisterShader( "gfx/hud/f_icon_dustreaver" );
	cgs.media.icon_poisonreaver =  cgi_R_RegisterShader( "gfx/hud/f_icon_poisonreaver" );
	cgs.media.icon_deathreaver =  cgi_R_RegisterShader( "gfx/hud/f_icon_deathreaver" );
	cgs.media.icon_spectralreaver =  cgi_R_RegisterShader( "gfx/hud/f_icon_spectralreaver" );
	cgs.media.icon_spiritreaver =  cgi_R_RegisterShader( "gfx/hud/f_icon_spiritreaver" );
	cgs.media.icon_holyreaver =  cgi_R_RegisterShader( "gfx/hud/f_icon_holyreaver" );
	cgs.media.icon_lifereaver =  cgi_R_RegisterShader( "gfx/hud/f_icon_lifereaver" );
	cgs.media.icon_soundreaver =  cgi_R_RegisterShader( "gfx/hud/f_icon_soundreaver" );
	cgs.media.icon_astralreaver =  cgi_R_RegisterShader( "gfx/hud/f_icon_astralreaver" );
	// THE PILLARS 
	cgs.media.icon_mindreaver =  cgi_R_RegisterShader( "gfx/hud/f_icon_mindreaver" );
	cgs.media.icon_dimensionreaver =  cgi_R_RegisterShader( "gfx/hud/f_icon_dimensionreaver" );
	cgs.media.icon_conflictreaver =  cgi_R_RegisterShader( "gfx/hud/f_icon_conflictreaver" );
	cgs.media.icon_naturereaver =  cgi_R_RegisterShader( "gfx/hud/f_icon_naturereaver" );
	cgs.media.icon_energyreaver =  cgi_R_RegisterShader( "gfx/hud/f_icon_energyreaver" );
	cgs.media.icon_timereaver =  cgi_R_RegisterShader( "gfx/hud/f_icon_timereaver" );
	cgs.media.icon_statesreaver =  cgi_R_RegisterShader( "gfx/hud/f_icon_statesreaver" );
	cgs.media.icon_deathreaver2 =  cgi_R_RegisterShader( "gfx/hud/f_icon_deathreaver2" );
	cgs.media.icon_soulreaver =  cgi_R_RegisterShader( "gfx/hud/f_icon_soulreaver" );
	cgs.media.icon_bloodreaver =  cgi_R_RegisterShader( "gfx/hud/f_icon_bloodreaver" );
	cgs.media.icon_balancereaver =  cgi_R_RegisterShader( "gfx/hud/f_icon_balancereaver" );
	// ARCANE ATTACKS 
	cgs.media.icon_energybolt =  cgi_R_RegisterShader( "gfx/hud/f_icon_dk_lt1" );
	cgs.media.icon_lightning =  cgi_R_RegisterShader( "gfx/hud/f_icon_dk_lt2" );
	cgs.media.icon_lightning2 =  cgi_R_RegisterShader( "gfx/hud/f_icon_dk_lt3" );
	cgs.media.icon_web =  cgi_R_RegisterShader( "gfx/hud/f_icon_web" );
	cgs.media.icon_ghost =  cgi_R_RegisterShader( "gfx/hud/f_icon_ghost" );
	cgs.media.icon_life =  cgi_R_RegisterShader( "gfx/hud/f_icon_life" );
	cgs.media.icon_hollyfire =  cgi_R_RegisterShader( "gfx/hud/f_icon_hollyfire" );
	cgs.media.icon_necro =  cgi_R_RegisterShader( "gfx/hud/f_icon_necro" );
	cgs.media.icon_psychic =  cgi_R_RegisterShader( "gfx/hud/f_icon_psychic" );
	cgs.media.icon_crystal =  cgi_R_RegisterShader( "gfx/hud/f_icon_crystal" );
	cgs.media.icon_stone =  cgi_R_RegisterShader( "gfx/hud/f_icon_stone" );
	cgs.media.icon_water =  cgi_R_RegisterShader( "gfx/hud/f_icon_water" );
	cgs.media.icon_fire =  cgi_R_RegisterShader( "gfx/hud/f_icon_fire" );
	cgs.media.icon_skull =  cgi_R_RegisterShader( "gfx/hud/f_icon_skull" );
	cgs.media.icon_lava =  cgi_R_RegisterShader( "gfx/hud/f_icon_lava" );
	cgs.media.icon_poison =  cgi_R_RegisterShader( "gfx/hud/f_icon_poison" );
	cgs.media.icon_hell =  cgi_R_RegisterShader( "gfx/hud/f_icon_hell" );
	cgs.media.icon_hylden =  cgi_R_RegisterShader( "gfx/hud/f_icon_hylden" );
	cgs.media.icon_greenlightning =  cgi_R_RegisterShader( "gfx/hud/f_icon_greenlightning" );
	cgs.media.icon_yellowlightning =  cgi_R_RegisterShader( "gfx/hud/f_icon_yellowlightning" );
	cgs.media.icon_wind =  cgi_R_RegisterShader( "gfx/hud/f_icon_wind" );
	cgs.media.icon_stormwind =  cgi_R_RegisterShader( "gfx/hud/f_icon_wind2" );
	cgs.media.icon_eterform =  cgi_R_RegisterShader( "gfx/hud/f_icon_eterform2" );
	cgs.media.icon_mistoffear =  cgi_R_RegisterShader( "gfx/hud/f_icon_fogoffear2" );
	cgs.media.icon_burst =  cgi_R_RegisterShader( "gfx/hud/f_icon_burst" );
	cgs.media.icon_ice =  cgi_R_RegisterShader( "gfx/hud/f_icon_ice" );
	cgs.media.icon_dark =  cgi_R_RegisterShader( "gfx/hud/f_icon_dark" );
	cgs.media.icon_void =  cgi_R_RegisterShader( "gfx/hud/f_icon_void" );
	cgs.media.icon_smog =  cgi_R_RegisterShader( "gfx/hud/f_icon_smog" );
	cgs.media.icon_sonic =  cgi_R_RegisterShader( "gfx/hud/f_icon_sonic" );
	cgs.media.icon_acid =  cgi_R_RegisterShader( "gfx/hud/f_icon_acid" );
	cgs.media.icon_astral =  cgi_R_RegisterShader( "gfx/hud/f_icon_astral" );
	cgs.media.icon_fireball =  cgi_R_RegisterShader( "gfx/hud/f_icon_fireball" );
	cgs.media.icon_holly =  cgi_R_RegisterShader( "gfx/hud/f_icon_holly" );
	
// LOAD STATIC ICONS 
	// CLASS_REAVER, CLASS_JEDI , 
	// Tiziel Basic 
      const char	*force_icon_files[NUM_FORCE_POWERS] = // DEFAULT MIETITORI 
	  {//icons matching enums forcePowers_t
		"gfx/mp/f_icon_lt_heal",		//FP_HEAL,
		"gfx/mp/f_icon_levitation",		//FP_LEVITATION,
		"gfx/mp/f_icon_speed",			//FP_SPEED,
		"gfx/mp/f_icon_push",			//FP_PUSH,
		"gfx/mp/f_icon_pull",			//FP_PULL,
		"gfx/mp/f_icon_lt_telepathy",	//FP_TELEPATHY,
		"gfx/mp/f_icon_dk_grip",		//FP_GRIP,
		"gfx/mp/f_icon_dk_lt3",			//FP_LIGHTNING,
		"gfx/mp/f_icon_saber_throw",	//FP_SABERTHROW
		"gfx/mp/f_icon_saber_defend",	//FP_SABERDEFEND,
		"gfx/mp/f_icon_saber_attack",	//FP_SABERATTACK,
		"gfx/mp/f_icon_dk_trance",		//FP_RAGE,
		"gfx/mp/f_icon_lt_protect",		//FP_PROTECT,
		"gfx/mp/f_icon_lt_absorb",		//FP_ABSORB,
		"gfx/mp/f_icon_dk_souldrain",		//FP_DRAIN,
		"gfx/mp/f_icon_sight2",			//FP_SEE,
		"gfx/mp/f_icon_stun",			//FP_STUN 
		"gfx/mp/f_icon_hate",			//FP_HATE
		"gfx/mp/f_icon_controlmind",	//FP_CONTROLMIND
		"gfx/mp/f_icon_frostcoffin",	//FP_INCAPACITATE 
		"gfx/mp/f_icon_healother",			//FP_HEALOTHER 
		"gfx/mp/f_icon_wrack",			//FP_WRACK 
		"gfx/mp/f_icon_forceglyph",		//FP_FORCEGLYPH
		"gfx/mp/f_icon_earthreaver",				//FP_STONEGLYPH 
		"gfx/mp/f_icon_soundglyph",			//FP_SOUNDGLYPH
		"gfx/mp/f_icon_waterreaver",			//FP_WATERGLYPH
		"gfx/mp/f_icon_firereaver",			//FP_FIREGLYPH 
		"gfx/mp/f_icon_lightreaver",					//FP_SUNGLYPH 
		"gfx/mp/f_icon_airreaver",					//FP_WINDGLYPH
		"gfx/mp/f_icon_materialreaver",				//FP_HOLYGLYPH 
		"gfx/mp/f_icon_darkreaver",					//FP_DARKGLYPH 
		"gfx/mp/f_icon_spectralreaver",					//FP_NECROGLYPH
	};
	 // CLASS SHIFTER 
	const char	*force_icon_files1[NUM_FORCE_POWERS] =  // SABOTEUR, SHIFTER
	{//icons matching enums forcePowers_t
		"gfx/mp/f_icon_lt_heal",		//FP_HEAL,
		"gfx/mp/f_icon_levitation",		//FP_LEVITATION,
		"gfx/mp/f_icon_speed",			//FP_SPEED,
		"gfx/mp/f_icon_push",			//FP_PUSH,
		"gfx/mp/f_icon_pull",			//FP_PULL,
		"gfx/mp/f_icon_proxy",	//FP_TELEPATHY,
		"gfx/mp/f_icon_dk_grip",		//FP_GRIP,
		"gfx/mp/f_icon_dark",			//FP_LIGHTNING,
		"gfx/mp/f_icon_saber_throw",	//FP_SABERTHROW
		"gfx/mp/f_icon_saber_defend",	//FP_SABERDEFEND,
		"gfx/mp/f_icon_saber_attack",	//FP_SABERATTACK,
		"gfx/mp/f_icon_dk_cruel",		//FP_RAGE,
		"gfx/mp/f_icon_lt_protect",		//FP_PROTECT,
		"gfx/mp/f_icon_lt_absorb",		//FP_ABSORB,
		"gfx/mp/f_icon_dk_souldrain",		//FP_DRAIN,
		"gfx/mp/f_icon_sight2",			//FP_SEE,
		"gfx/mp/f_icon_feartrick",			//FP_STUN 
		"gfx/mp/f_icon_hate",			//FP_HATE
		"gfx/mp/f_icon_controlmind",	//FP_CONTROLMIND
		"gfx/mp/f_icon_incapacitate",	//FP_INCAPACITATE 
		"gfx/mp/f_icon_healother",			//FP_HEALOTHER 
		"gfx/mp/f_icon_wrack",			//FP_WRACK 
		"gfx/mp/f_icon_forceglyph",		//FP_FORCEGLYPH
		"gfx/mp/f_icon_earthreaver",		//FP_STONEGLYPH 
		"gfx/mp/f_icon_soundglyph",		//FP_SOUNDGLYPH
		"gfx/mp/f_icon_waterreaver",	//FP_WATERGLYPH
		"gfx/mp/f_icon_firereaver",	//FP_FIREGLYPH 
		"gfx/mp/f_icon_lightreaver",			//FP_SUNGLYPH 
		"gfx/mp/f_icon_airreaver",			//FP_WINDGLYPH
		"gfx/mp/f_icon_materialreaver",			//FP_HOLYGLYPH 
		"gfx/mp/f_icon_darkreaver",			//FP_DARKGLYPH 
		"gfx/mp/f_icon_spectralreaver",			//FP_NECROGLYPH
	};

	 // CLASS_TANKREAVER with smog reaver 
	const char	*force_icon_files2[NUM_FORCE_POWERS] =  // BLEED - BESPINCOP 
	{//icons matching enums forcePowers_t
		"gfx/mp/f_icon_lt_heal",		//FP_HEAL,
		"gfx/mp/f_icon_levitation",		//FP_LEVITATION,
		"gfx/mp/f_icon_speed",			//FP_SPEED,
		"gfx/mp/f_icon_push",			//FP_PUSH,
		"gfx/mp/f_icon_pull",			//FP_PULL,
		"gfx/mp/f_icon_lt_telepathy",	//FP_TELEPATHY,
		"gfx/mp/f_icon_dk_grip",		//FP_GRIP,
		"gfx/mp/f_icon_smog",			//FP_LIGHTNING,
		"gfx/mp/f_icon_saber_throw",	//FP_SABERTHROW
		"gfx/mp/f_icon_saber_defend",	//FP_SABERDEFEND,
		"gfx/mp/f_icon_saber_attack",	//FP_SABERATTACK,
		"gfx/mp/f_icon_dk_trance",		//FP_RAGE,
		"gfx/mp/f_icon_lt_protect",		//FP_PROTECT,
		"gfx/mp/f_icon_lt_absorb",		//FP_ABSORB,
		"gfx/mp/f_icon_dk_souldrain",		//FP_DRAIN,
		"gfx/mp/f_icon_sight2",			//FP_SEE,
		"gfx/mp/f_icon_stun",			//FP_STUN 
		"gfx/mp/f_icon_hate",			//FP_HATE
		"gfx/mp/f_icon_controlmind",	//FP_CONTROLMIND
		"gfx/mp/f_icon_incapacitate",	//FP_INCAPACITATE 
		"gfx/mp/f_icon_healother",			//FP_HEALOTHER 
		"gfx/mp/f_icon_wrack",			//FP_WRACK 
		"gfx/mp/f_icon_forceglyph",		//FP_FORCEGLYPH
		"gfx/mp/f_icon_earthreaver",		//FP_STONEGLYPH 
		"gfx/mp/f_icon_soundglyph",		//FP_SOUNDGLYPH
		"gfx/mp/f_icon_waterglyph",	//FP_WATERGLYPH
		"gfx/mp/f_icon_firereaver",	//FP_FIREGLYPH 
		"gfx/mp/f_icon_lightreaver",			//FP_SUNGLYPH 
		"gfx/mp/f_icon_airreaver",			//FP_WINDGLYPH
		"gfx/mp/f_icon_materialreaver",			//FP_HOLYGLYPH 
		"gfx/mp/f_icon_smogreaver",			//FP_DARKGLYPH 
		"gfx/mp/f_icon_spectralreaver",			//FP_NECROGLYPH
	};
	// CLASS_SHADOWTROOPER: shifer with Void Reaver 
	const char	*force_icon_files3[NUM_FORCE_POWERS] =  // SHIFTER - SHADOWTROOPER
	{//icons matching enums forcePowers_t
		"gfx/mp/f_icon_d_heal",		//FP_HEAL,
		"gfx/mp/f_icon_levitation",		//FP_LEVITATION,
		"gfx/mp/f_icon_speed",			//FP_SPEED,
		"gfx/mp/f_icon_push",			//FP_PUSH,
		"gfx/mp/f_icon_pull",			//FP_PULL,
		"gfx/mp/f_icon_proxy",	//FP_TELEPATHY,
		"gfx/mp/f_icon_dk_grip",		//FP_GRIP,
		"gfx/mp/f_icon_void",			//FP_LIGHTNING,
		"gfx/mp/f_icon_saber_throw",	//FP_SABERTHROW
		"gfx/mp/f_icon_saber_defend",	//FP_SABERDEFEND,
		"gfx/mp/f_icon_saber_attack",	//FP_SABERATTACK,
		"gfx/mp/f_icon_dk_cruel",		//FP_RAGE, // CRUDELTA' EMERSA 
		"gfx/mp/f_icon_lt_protect",		//FP_PROTECT,
		"gfx/mp/f_icon_lt_absorb",		//FP_ABSORB,
		"gfx/mp/f_icon_dk_necrodrain",		//FP_DRAIN,
		"gfx/mp/f_icon_sight2",			//FP_SEE,
		"gfx/mp/f_icon_feartrick",			//FP_STUN 
		"gfx/mp/f_icon_hate",			//FP_HATE
		"gfx/mp/f_icon_controlmind",	//FP_CONTROLMIND
		"gfx/mp/f_icon_incapacitate",	//FP_INCAPACITATE 
		"gfx/mp/f_icon_healother",			//FP_HEALOTHER 
		"gfx/mp/f_icon_wrack",			//FP_WRACK 
		"gfx/mp/f_icon_forceglyph",		//FP_FORCEGLYPH
		"gfx/mp/f_icon_stoneglyph",		//FP_STONEGLYPH 
		"gfx/mp/f_icon_soundglyph",		//FP_SOUNDGLYPH
		"gfx/mp/f_icon_waterreaver",	//FP_WATERGLYPH
		"gfx/mp/f_icon_firereaver",	//FP_FIREGLYPH 
		"gfx/mp/f_icon_lightreaver",			//FP_SUNGLYPH 
		"gfx/mp/f_icon_airreaver",			//FP_WINDGLYPH
		"gfx/mp/f_icon_materialreaver",			//FP_HOLYGLYPH 
		"gfx/mp/f_icon_voidreaver",			//FP_DARKGLYPH 
		"gfx/mp/f_icon_spectralreaver",			//FP_NECROGLYPH
	};

	//CLASS_LIGHTREAVER with lightning reaver 
	const char	*force_icon_files4[NUM_FORCE_POWERS] =  // TIZIEL AND SYL - 
	{//icons matching enums forcePowers_t
		"gfx/mp/f_icon_lt_heal",		//FP_HEAL,
		"gfx/mp/f_icon_levitation",		//FP_LEVITATION,
		"gfx/mp/f_icon_speed",			//FP_SPEED,
		"gfx/mp/f_icon_push",			//FP_PUSH,
		"gfx/mp/f_icon_pull",			//FP_PULL,
		"gfx/mp/f_icon_lt_telepathy",	//FP_TELEPATHY,
		"gfx/mp/f_icon_dk_grip",		//FP_GRIP,
		"gfx/mp/f_icon_yellowlightning",			//FP_LIGHTNING,
		"gfx/mp/f_icon_saber_throw",	//FP_SABERTHROW
		"gfx/mp/f_icon_saber_defend",	//FP_SABERDEFEND,
		"gfx/mp/f_icon_saber_attack",	//FP_SABERATTACK,
		"gfx/mp/f_icon_dk_shockshield",		//FP_RAGE,
		"gfx/mp/f_icon_lt_protect",		//FP_PROTECT,
		"gfx/mp/f_icon_lt_absorb",		//FP_ABSORB,
		"gfx/mp/f_icon_dk_souldrain",		//FP_DRAIN,
		"gfx/mp/f_icon_sight2",			//FP_SEE,
		"gfx/mp/f_icon_stun",			//FP_STUN 
		"gfx/mp/f_icon_hate",			//FP_HATE
		"gfx/mp/f_icon_controlmind",	//FP_CONTROLMIND
		"gfx/mp/f_icon_thundernet",	//FP_INCAPACITATE 
		"gfx/mp/f_icon_healother",			//FP_HEALOTHER 
		"gfx/mp/f_icon_wrack",			//FP_WRACK 
		"gfx/mp/f_icon_forceglyph",		//FP_FORCEGLYPH
		"gfx/mp/f_icon_stoneglyph",		//FP_STONEGLYPH 
		"gfx/mp/f_icon_soundglyph",		//FP_SOUNDGLYPH
		"gfx/mp/f_icon_waterreaver",	//FP_WATERGLYPH
		"gfx/mp/f_icon_firereaver",	//FP_FIREGLYPH 
		"gfx/mp/f_icon_lightningreaver",			//FP_SUNGLYPH 
		"gfx/mp/f_icon_airreaver",			//FP_WINDGLYPH
		"gfx/mp/f_icon_materialreaver",			//FP_HOLYGLYPH 
		"gfx/mp/f_icon_darkreaver",			//FP_DARKGLYPH 
		"gfx/mp/f_icon_spectralreaver",			//FP_NECROGLYPH
	};

	// CLASS_MORGANKATARN - Sun Reaver for Tiziel and Sylfean 
	const char	*force_icon_files5[NUM_FORCE_POWERS] =  // TIZIEL AND SYL - ASSASSINDROID
	{//icons matching enums forcePowers_t
		"gfx/mp/f_icon_lt_heal",		//FP_HEAL,
		"gfx/mp/f_icon_levitation",		//FP_LEVITATION,
		"gfx/mp/f_icon_speed",			//FP_SPEED,
		"gfx/mp/f_icon_push",			//FP_PUSH,
		"gfx/mp/f_icon_pull",			//FP_PULL,
		"gfx/mp/f_icon_lt_telepathy",	//FP_TELEPATHY,
		"gfx/mp/f_icon_dk_grip",		//FP_GRIP,
		"gfx/mp/f_icon_burst",			//FP_LIGHTNING,
		"gfx/mp/f_icon_saber_throw",	//FP_SABERTHROW
		"gfx/mp/f_icon_saber_defend",	//FP_SABERDEFEND,
		"gfx/mp/f_icon_saber_attack",	//FP_SABERATTACK,
		"gfx/mp/f_icon_dk_eldershield",		//FP_RAGE,
		"gfx/mp/f_icon_lt_protect",		//FP_PROTECT,
		"gfx/mp/f_icon_lt_absorb",		//FP_ABSORB,
		"gfx/mp/f_icon_dk_demondrain",		//FP_DRAIN,
		"gfx/mp/f_icon_sight2",			//FP_SEE,
		"gfx/mp/f_icon_stun",			//FP_STUN 
		"gfx/mp/f_icon_hate",			//FP_HATE
		"gfx/mp/f_icon_controlmind",	//FP_CONTROLMIND
		"gfx/mp/f_icon_thundernet",	//FP_INCAPACITATE 
		"gfx/mp/f_icon_healother",			//FP_HEALOTHER 
		"gfx/mp/f_icon_wrack",			//FP_WRACK 
		"gfx/mp/f_icon_forceglyph",		//FP_FORCEGLYPH
		"gfx/mp/f_icon_stoneglyph",		//FP_STONEGLYPH 
		"gfx/mp/f_icon_soundglyph",		//FP_SOUNDGLYPH
		"gfx/mp/f_icon_waterreaver",	//FP_WATERGLYPH
		"gfx/mp/f_icon_firereaver",	//FP_FIREGLYPH 
		"gfx/mp/f_icon_sunreaver",			//FP_SUNGLYPH 
		"gfx/mp/f_icon_airreaver",			//FP_WINDGLYPH
		"gfx/mp/f_icon_materialreaver",			//FP_HOLYGLYPH 
		"gfx/mp/f_icon_darkreaver",			//FP_DARKGLYPH 
		"gfx/mp/f_icon_spectralreaver",			//FP_NECROGLYPH
	};
	
	// Class Kaneel with Warm Reaver - Qua acquisisce anche la percezione echo
	const char	*force_icon_files6[NUM_FORCE_POWERS] = // HAZARDTROOPER, KANEEL 
	{//icons matching enums forcePowers_t
		"gfx/mp/f_icon_lt_heal",		//FP_HEAL,
		"gfx/mp/f_icon_levitation",		//FP_LEVITATION,
		"gfx/mp/f_icon_speed",			//FP_SPEED,
		"gfx/mp/f_icon_push",			//FP_PUSH,
		"gfx/mp/f_icon_pull",			//FP_PULL,
		"gfx/mp/f_icon_lt_telepathy",	//FP_TELEPATHY,
		"gfx/mp/f_icon_dk_grip",		//FP_GRIP,
		"gfx/mp/f_icon_fire",			//FP_LIGHTNING, // change to fire atk 
		"gfx/mp/f_icon_saber_throw",	//FP_SABERTHROW
		"gfx/mp/f_icon_saber_defend",	//FP_SABERDEFEND,
		"gfx/mp/f_icon_saber_attack",	//FP_SABERATTACK,
		"gfx/mp/f_icon_fire_rage",		//FP_RAGE,
		"gfx/mp/f_icon_lt_protect",		//FP_PROTECT,
		"gfx/mp/f_icon_lt_absorb",		//FP_ABSORB,
		"gfx/mp/f_icon_dk_souldrain",		//FP_DRAIN,
		"gfx/mp/f_icon_sight3",			//FP_SEE,
		"gfx/mp/f_icon_stun",			//FP_STUN 
		"gfx/mp/f_icon_hate",			//FP_HATE
		"gfx/mp/f_icon_controlmind",	//FP_CONTROLMIND
		"gfx/mp/f_icon_incapacitate",	//FP_INCAPACITATE 
		"gfx/mp/f_icon_healother",			//FP_HEALOTHER 
		"gfx/mp/f_icon_wrack",			//FP_WRACK 
		"gfx/mp/f_icon_forceglyph",		//FP_FORCEGLYPH
		"gfx/mp/f_icon_earthreaver",		//FP_STONEGLYPH 
		"gfx/mp/f_icon_soundglyph",		//FP_SOUNDGLYPH
		"gfx/mp/f_icon_waterreaver",	//FP_WATERGLYPH
		"gfx/mp/f_icon_warmreaver",	//FP_FIREGLYPH 
		"gfx/mp/f_icon_lightreaver",			//FP_SUNGLYPH 
		"gfx/mp/f_icon_airreaver",			//FP_WINDGLYPH
		"gfx/mp/f_icon_materialreaver",			//FP_HOLYGLYPH 
		"gfx/mp/f_icon_darkreaver",			//FP_DARKGLYPH 
		"gfx/mp/f_icon_spectralreaver",			//FP_NECROGLYPH
	};
	// CLASS_CLAW : Kaneel with Magma reaver 
	const char	*force_icon_files7[NUM_FORCE_POWERS] = // CLAW - KANEEL MAGMA 
	{//icons matching enums forcePowers_t
		"gfx/mp/f_icon_v_heal2",		//FP_HEAL,
		"gfx/mp/f_icon_levitation",		//FP_LEVITATION,
		"gfx/mp/f_icon_speed",			//FP_SPEED,
		"gfx/mp/f_icon_push",			//FP_PUSH,
		"gfx/mp/f_icon_pull",			//FP_PULL,
		"gfx/mp/f_icon_lt_telepathy",	//FP_TELEPATHY,
		"gfx/mp/f_icon_dk_grip",		//FP_GRIP,
		"gfx/mp/f_icon_lava",			//FP_LIGHTNING,
		"gfx/mp/f_icon_saber_throw",	//FP_SABERTHROW
		"gfx/mp/f_icon_saber_defend",	//FP_SABERDEFEND,
		"gfx/mp/f_icon_saber_attack",	//FP_SABERATTACK,
		"gfx/mp/f_icon_fire_rage",		//FP_RAGE,
		"gfx/mp/f_icon_lt_protect",		//FP_PROTECT,
		"gfx/mp/f_icon_lt_absorb",		//FP_ABSORB,
		"gfx/mp/f_icon_dk_drain2",		//FP_DRAIN,
		"gfx/mp/f_icon_sight3",			//FP_SEE,
		"gfx/mp/f_icon_stun",			//FP_STUN 
		"gfx/mp/f_icon_hate",			//FP_HATE
		"gfx/mp/f_icon_controlmind",	//FP_CONTROLMIND
		"gfx/mp/f_icon_incapacitate",	//FP_INCAPACITATE 
		"gfx/mp/f_icon_healother",			//FP_HEALOTHER 
		"gfx/mp/f_icon_wrack",			//FP_WRACK 
		"gfx/mp/f_icon_forceglyph",		//FP_FORCEGLYPH
		"gfx/mp/f_icon_earthreaver",		//FP_STONEGLYPH 
		"gfx/mp/f_icon_soundglyph",		//FP_SOUNDGLYPH
		"gfx/mp/f_icon_waterreaver",	//FP_WATERGLYPH
		"gfx/mp/f_icon_magmareaver",	//FP_FIREGLYPH 
		"gfx/mp/f_icon_lightreaver",			//FP_SUNGLYPH 
		"gfx/mp/f_icon_airreaver",			//FP_WINDGLYPH
		"gfx/mp/f_icon_materialreaver",			//FP_HOLYGLYPH 
		"gfx/mp/f_icon_smogreaver",			//FP_DARKGLYPH 
		"gfx/mp/f_icon_spectralreaver",			//FP_NECROGLYPH
	};
	// CLASS_REBEL NAERYAN BASIC CON ATK DI VENTO BASIC 
	const char	*force_icon_files8[NUM_FORCE_POWERS] = // REBEL - NAERYAN 
	{//icons matching enums forcePowers_t
		"gfx/mp/f_icon_lt_heal",		//FP_HEAL,
		"gfx/mp/f_icon_levitation",		//FP_LEVITATION,
		"gfx/mp/f_icon_speed",			//FP_SPEED,
		"gfx/mp/f_icon_push",			//FP_PUSH,
		"gfx/mp/f_icon_pull",			//FP_PULL,
		"gfx/mp/f_icon_lt_telepathy",	//FP_TELEPATHY,
		"gfx/mp/f_icon_dk_grip",		//FP_GRIP,
		"gfx/mp/f_icon_wind",			//FP_LIGHTNING,
		"gfx/mp/f_icon_saber_throw",	//FP_SABERTHROW
		"gfx/mp/f_icon_saber_defend",	//FP_SABERDEFEND,
		"gfx/mp/f_icon_saber_attack",	//FP_SABERATTACK,
		"gfx/mp/f_icon_dk_windshield",		//FP_RAGE,
		"gfx/mp/f_icon_lt_protect",		//FP_PROTECT,
		"gfx/mp/f_icon_lt_absorb",		//FP_ABSORB,
		"gfx/mp/f_icon_dk_souldrain",		//FP_DRAIN,
		"gfx/mp/f_icon_sight2",			//FP_SEE,
		"gfx/mp/f_icon_eterform",			//FP_STUN 
		"gfx/mp/f_icon_hate",			//FP_HATE
		"gfx/mp/f_icon_controlmind",	//FP_CONTROLMIND
		"gfx/mp/f_icon_incapacitate",	//FP_INCAPACITATE 
		"gfx/mp/f_icon_healother",			//FP_HEALOTHER 
		"gfx/mp/f_icon_wrack",			//FP_WRACK 
		"gfx/mp/f_icon_forceglyph",		//FP_FORCEGLYPH
		"gfx/mp/f_icon_earthreaver",		//FP_STONEGLYPH 
		"gfx/mp/f_icon_soundglyph",		//FP_SOUNDGLYPH
		"gfx/mp/f_icon_waterreaver",	//FP_WATERGLYPH
		"gfx/mp/f_icon_firereaver",	//FP_FIREGLYPH 
		"gfx/mp/f_icon_lightreaver",			//FP_SUNGLYPH 
		"gfx/mp/f_icon_airreaver",			//FP_WINDGLYPH
		"gfx/mp/f_icon_materialreaver",			//FP_HOLYGLYPH 
		"gfx/mp/f_icon_darkreaver",			//FP_DARKGLYPH 
		"gfx/mp/f_icon_spectralreaver",			//FP_NECROGLYPH
	};

	// Naeryan with Fog Reaver 
	const char	*force_icon_files9[NUM_FORCE_POWERS] = // REBEL - NAERYAN 
	{//icons matching enums forcePowers_t
		"gfx/mp/f_icon_lt_heal",		//FP_HEAL,
		"gfx/mp/f_icon_levitation",		//FP_LEVITATION,
		"gfx/mp/f_icon_speed",			//FP_SPEED,
		"gfx/mp/f_icon_push",			//FP_PUSH,
		"gfx/mp/f_icon_pull",			//FP_PULL,
		"gfx/mp/f_icon_lt_telepathy",	//FP_TELEPATHY,
		"gfx/mp/f_icon_dk_grip",		//FP_GRIP,
		"gfx/mp/f_icon_wind",			//FP_LIGHTNING,
		"gfx/mp/f_icon_saber_throw",	//FP_SABERTHROW
		"gfx/mp/f_icon_saber_defend",	//FP_SABERDEFEND,
		"gfx/mp/f_icon_saber_attack",	//FP_SABERATTACK,
		"gfx/mp/f_icon_dk_windshield",		//FP_RAGE,
		"gfx/mp/f_icon_lt_protect",		//FP_PROTECT,
		"gfx/mp/f_icon_lt_absorb",		//FP_ABSORB,
		"gfx/mp/f_icon_dk_souldrain",		//FP_DRAIN,
		"gfx/mp/f_icon_sight2",			//FP_SEE,
		"gfx/mp/f_icon_eterform",			//FP_STUN 
		"gfx/mp/f_icon_hate",			//FP_HATE
		"gfx/mp/f_icon_controlmind",	//FP_CONTROLMIND
		"gfx/mp/f_icon_incapacitate",	//FP_INCAPACITATE 
		"gfx/mp/f_icon_healother",			//FP_HEALOTHER 
		"gfx/mp/f_icon_wrack",			//FP_WRACK 
		"gfx/mp/f_icon_forceglyph",		//FP_FORCEGLYPH
		"gfx/mp/f_icon_earthreaver",		//FP_STONEGLYPH 
		"gfx/mp/f_icon_soundglyph",		//FP_SOUNDGLYPH
		"gfx/mp/f_icon_waterreaver",	//FP_WATERGLYPH
		"gfx/mp/f_icon_firereaver",	//FP_FIREGLYPH 
		"gfx/mp/f_icon_lightreaver",			//FP_SUNGLYPH 
		"gfx/mp/f_icon_fogreaver",			//FP_WINDGLYPH
		"gfx/mp/f_icon_materialreaver",			//FP_HOLYGLYPH 
		"gfx/mp/f_icon_darkreaver",			//FP_DARKGLYPH 
		"gfx/mp/f_icon_spectralreaver",			//FP_NECROGLYPH
	};

	// Naeryan al massimo del suo potere CLASS_ROCKETTROOPER, storm reaver 
		const char	*force_icon_files10[NUM_FORCE_POWERS] = // ROCKETTROOPER - NEBBIA E TEMPESTA
	{//icons matching enums forcePowers_t
		"gfx/mp/f_icon_lt_heal",		//FP_HEAL,
		"gfx/mp/f_icon_levitation",		//FP_LEVITATION,
		"gfx/mp/f_icon_speed",			//FP_SPEED,
		"gfx/mp/f_icon_push",			//FP_PUSH,
		"gfx/mp/f_icon_pull",			//FP_PULL,
		"gfx/mp/f_icon_lt_telepathy",	//FP_TELEPATHY,
		"gfx/mp/f_icon_dk_grip",		//FP_GRIP,
		"gfx/mp/f_icon_wind2",			//FP_LIGHTNING,
		"gfx/mp/f_icon_saber_throw",	//FP_SABERTHROW
		"gfx/mp/f_icon_saber_defend",	//FP_SABERDEFEND,
		"gfx/mp/f_icon_saber_attack",	//FP_SABERATTACK,
		"gfx/mp/f_icon_dk_stormshield",		//FP_RAGE,
		"gfx/mp/f_icon_lt_protect",		//FP_PROTECT,
		"gfx/mp/f_icon_lt_absorb",		//FP_ABSORB,
		"gfx/mp/f_icon_dk_souldrain",		//FP_DRAIN,
		"gfx/mp/f_icon_sight2",			//FP_SEE,
		"gfx/mp/f_icon_eterform",			//FP_STUN 
		"gfx/mp/f_icon_hate",			//FP_HATE
		"gfx/mp/f_icon_controlmind",	//FP_CONTROLMIND
		"gfx/mp/f_icon_incapacitate",	//FP_INCAPACITATE 
		"gfx/mp/f_icon_healother",			//FP_HEALOTHER 
		"gfx/mp/f_icon_wrack",			//FP_WRACK 
		"gfx/mp/f_icon_forceglyph",		//FP_FORCEGLYPH
		"gfx/mp/f_icon_earthreaver",		//FP_STONEGLYPH 
		"gfx/mp/f_icon_soundglyph",		//FP_SOUNDGLYPH
		"gfx/mp/f_icon_waterreaver",	//FP_WATERGLYPH
		"gfx/mp/f_icon_firereaver",	//FP_FIREGLYPH 
		"gfx/mp/f_icon_lightreaver",			//FP_SUNGLYPH 
		"gfx/mp/f_icon_stormreaver",			//FP_WINDGLYPH
		"gfx/mp/f_icon_materialreaver",			//FP_HOLYGLYPH 
		"gfx/mp/f_icon_darkreaver",			//FP_DARKGLYPH 
		"gfx/mp/f_icon_spectralreaver",			//FP_NECROGLYPH
	};
	// CLASS SEED - matrice sonica 
	const char	*force_icon_files11[NUM_FORCE_POWERS] = // REBORN, ALORA // CDS VAMIRES
	{//icons matching enums forcePowers_t
		"gfx/mp/f_icon_v_heal",		//FP_HEAL,
		"gfx/mp/f_icon_levitation",		//FP_LEVITATION,
		"gfx/mp/f_icon_speed2",			//FP_SPEED,
		"gfx/mp/f_icon_push",			//FP_PUSH,
		"gfx/mp/f_icon_pull",			//FP_PULL,
		"gfx/mp/f_icon_wolfform",			//FP_TELEPATHY,
		"gfx/mp/f_icon_dk_grip",		//FP_GRIP,
		"gfx/mp/f_icon_sonic",			//FP_LIGHTNING,
		"gfx/mp/f_icon_saber_throw",	//FP_SABERTHROW
		"gfx/mp/f_icon_saber_defend",	//FP_SABERDEFEND,
		"gfx/mp/f_icon_saber_attack",	//FP_SABERATTACK,
		"gfx/mp/f_icon_dk_rage",		//FP_RAGE,
		"gfx/mp/f_icon_lt_protect",		//FP_PROTECT,
		"gfx/mp/f_icon_lt_absorb",		//FP_ABSORB,
		"gfx/mp/f_icon_dk_drain",		//FP_DRAIN,- BLOOD SHOWER 
		"gfx/mp/f_icon_sight",			//FP_SEE,
		"gfx/mp/f_icon_stun",			//FP_STUN 
		"gfx/mp/f_icon_hate",			//FP_HATE
		"gfx/mp/f_icon_controlmind",	//FP_CONTROLMIND
		"gfx/mp/f_icon_incapacitate",	//FP_INCAPACITATE 
		"gfx/mp/f_icon_batform",			//FP_HEALOTHER 
		"gfx/mp/f_icon_wrack",			//FP_WRACK 
		"gfx/mp/f_icon_mistform",		//FP_FORCEGLYPH
		"gfx/mp/f_icon_timereaver",		//FP_STONEGLYPH 
		"gfx/mp/f_icon_dimensionreaver",		//FP_SOUNDGLYPH
		"gfx/mp/f_icon_statesreaver",	//FP_WATERGLYPH
		"gfx/mp/f_icon_conflictreaver",	//FP_FIREGLYPH 
		"gfx/mp/f_icon_energyreaver",			//FP_SUNGLYPH 
		"gfx/mp/f_icon_mindreaver",			//FP_WINDGLYPH
		"gfx/mp/f_icon_soulreaver",			//FP_HOLYGLYPH <- SOUL WILL NEVER USE THIS POWER!!
		"gfx/mp/f_icon_naturereaver",			//FP_DARKGLYPH 
		"gfx/mp/f_icon_bloodreaver",			//FP_NECROGLYPH// NON DARLO A KAIN
	};
	
	// Rekius Configurazione Basic con mietitrice d'acqua e vento glaciale
	// CLASS_BARTENDER // CURA SBAGLIATA PER CLASS REKIUS! 
	// Rekius CLASS_ICEREAVER: mietitrice di Ghiaccio 	
	const char	*force_icon_files12[NUM_FORCE_POWERS] = // REKIUS  
	{//icons matching enums forcePowers_t
		"gfx/mp/f_icon_lt_heal",		//FP_HEAL,
		"gfx/mp/f_icon_levitation",		//FP_LEVITATION,
		"gfx/mp/f_icon_speed",			//FP_SPEED,
		"gfx/mp/f_icon_push",			//FP_PUSH,
		"gfx/mp/f_icon_pull",			//FP_PULL,
		"gfx/mp/f_icon_lt_telepathy",	//FP_TELEPATHY,
		"gfx/mp/f_icon_dk_grip",		//FP_GRIP,
		"gfx/mp/f_icon_ice",			//FP_LIGHTNING,
		"gfx/mp/f_icon_saber_throw",	//FP_SABERTHROW
		"gfx/mp/f_icon_saber_defend",	//FP_SABERDEFEND,
		"gfx/mp/f_icon_saber_attack",	//FP_SABERATTACK,
		"gfx/mp/f_icon_dk_frostarmor",		//FP_RAGE,
		"gfx/mp/f_icon_lt_protect",		//FP_PROTECT,
		"gfx/mp/f_icon_lt_absorb",		//FP_ABSORB,
		"gfx/mp/f_icon_dk_necrodrain",		//FP_DRAIN,
		"gfx/mp/f_icon_sight2",			//FP_SEE,
		"gfx/mp/f_icon_stun",			//FP_STUN 
		"gfx/mp/f_icon_hate",			//FP_HATE
		"gfx/mp/f_icon_controlmind",	//FP_CONTROLMIND
		"gfx/mp/f_icon_frostcoffin",	//FP_INCAPACITATE 
		"gfx/mp/f_icon_healother",			//FP_HEALOTHER 
		"gfx/mp/f_icon_wrack",			//FP_WRACK 
		"gfx/mp/f_icon_forceglyph",		//FP_FORCEGLYPH
		"gfx/mp/f_icon_earthreaver",		//FP_STONEGLYPH 
		"gfx/mp/f_icon_soundglyph",		//FP_SOUNDGLYPH
		"gfx/mp/f_icon_icereaver",	//FP_WATERGLYPH
		"gfx/mp/f_icon_firereaver",	//FP_FIREGLYPH 
		"gfx/mp/f_icon_lightreaver",			//FP_SUNGLYPH 
		"gfx/mp/f_icon_airreaver",			//FP_WINDGLYPH
		"gfx/mp/f_icon_materialreaver",			//FP_HOLYGLYPH 
		"gfx/mp/f_icon_darkreaver",			//FP_DARKGLYPH 
		"gfx/mp/f_icon_spectralreaver",			//FP_NECROGLYPH
	};
	// Rekius CLASS_CRYSTAL // Mietitrice di Cristallo 
	const char	*force_icon_files13[NUM_FORCE_POWERS] = // REKIUS - WEEQUAY 
	{//icons matching enums forcePowers_t
		"gfx/mp/f_icon_lt_heal",		//FP_HEAL,
		"gfx/mp/f_icon_levitation",		//FP_LEVITATION,
		"gfx/mp/f_icon_speed",			//FP_SPEED,
		"gfx/mp/f_icon_push",			//FP_PUSH,
		"gfx/mp/f_icon_pull",			//FP_PULL,
		"gfx/mp/f_icon_lt_telepathy",	//FP_TELEPATHY,
		"gfx/mp/f_icon_dk_grip",		//FP_GRIP,
		"gfx/mp/f_icon_crystal",			//FP_LIGHTNING,
		"gfx/mp/f_icon_saber_throw",	//FP_SABERTHROW
		"gfx/mp/f_icon_saber_defend",	//FP_SABERDEFEND,
		"gfx/mp/f_icon_saber_attack",	//FP_SABERATTACK,
		"gfx/mp/f_icon_dk_crystalarmor",		//FP_RAGE,
		"gfx/mp/f_icon_lt_protect",		//FP_PROTECT,
		"gfx/mp/f_icon_lt_absorb",		//FP_ABSORB,
		"gfx/mp/f_icon_dk_necrodrain",		//FP_DRAIN,
		"gfx/mp/f_icon_sight2",			//FP_SEE,
		"gfx/mp/f_icon_stun",			//FP_STUN 
		"gfx/mp/f_icon_hate",			//FP_HATE
		"gfx/mp/f_icon_controlmind",	//FP_CONTROLMIND
		"gfx/mp/f_icon_frostcoffin",	//FP_INCAPACITATE 
		"gfx/mp/f_icon_healother",			//FP_HEALOTHER 
		"gfx/mp/f_icon_wrack",			//FP_WRACK 
		"gfx/mp/f_icon_forceglyph",		//FP_FORCEGLYPH
		"gfx/mp/f_icon_earthreaver",		//FP_STONEGLYPH 
		"gfx/mp/f_icon_soundglyph",		//FP_SOUNDGLYPH
		"gfx/mp/f_icon_crystalreaver",	//FP_WATERGLYPH
		"gfx/mp/f_icon_firereaver",	//FP_FIREGLYPH 
		"gfx/mp/f_icon_lightreaver",			//FP_SUNGLYPH 
		"gfx/mp/f_icon_airreaver",			//FP_WINDGLYPH
		"gfx/mp/f_icon_materialreaver",			//FP_HOLYGLYPH 
		"gfx/mp/f_icon_darkreaver",			//FP_DARKGLYPH 
		"gfx/mp/f_icon_spectralreaver",			//FP_NECROGLYPH
	};

	// CLASS_SPIDERREAVER Default con mietitirce di Terra e ragnatele 
	const char	*force_icon_files14[NUM_FORCE_POWERS] = // XADO BASIC CLASS
	{//icons matching enums forcePowers_t
		"gfx/mp/f_icon_lt_heal",		//FP_HEAL,
		"gfx/mp/f_icon_levitation",		//FP_LEVITATION,
		"gfx/mp/f_icon_speed",			//FP_SPEED,
		"gfx/mp/f_icon_push",			//FP_PUSH,
		"gfx/mp/f_icon_pull",			//FP_PULL,
		"gfx/mp/f_icon_lt_telepathy",	//FP_TELEPATHY,
		"gfx/mp/f_icon_dk_grip",		//FP_GRIP,
		"gfx/mp/f_icon_web",			//FP_LIGHTNING,
		"gfx/mp/f_icon_saber_throw",	//FP_SABERTHROW
		"gfx/mp/f_icon_saber_defend",	//FP_SABERDEFEND,
		"gfx/mp/f_icon_saber_attack",	//FP_SABERATTACK,
		"gfx/mp/f_icon_dk_trance",		//FP_RAGE,
		"gfx/mp/f_icon_lt_protect",		//FP_PROTECT,
		"gfx/mp/f_icon_lt_absorb",		//FP_ABSORB,
		"gfx/mp/f_icon_dk_souldrain",		//FP_DRAIN, // ASSENTE 
		"gfx/mp/f_icon_sight2",			//FP_SEE,
		"gfx/mp/f_icon_stun",			//FP_STUN 
		"gfx/mp/f_icon_hate",			//FP_HATE
		"gfx/mp/f_icon_controlmind",	//FP_CONTROLMIND
		"gfx/mp/f_icon_webbed",		//FP_INCAPACITATE 
		"gfx/mp/f_icon_healother",			//FP_HEALOTHER 
		"gfx/mp/f_icon_wrack",			//FP_WRACK 
		"gfx/mp/f_icon_forceglyph",		//FP_FORCEGLYPH
		"gfx/mp/f_icon_earthreaver",		//FP_STONEGLYPH 
		"gfx/mp/f_icon_soundglyph",		//FP_SOUNDGLYPH
		"gfx/mp/f_icon_waterreaver",	//FP_WATERGLYPH
		"gfx/mp/f_icon_firereaver",	//FP_FIREGLYPH 
		"gfx/mp/f_icon_lightreaver",			//FP_SUNGLYPH 
		"gfx/mp/f_icon_airreaver",			//FP_WINDGLYPH
		"gfx/mp/f_icon_materialreaver",			//FP_HOLYGLYPH 
		"gfx/mp/f_icon_darkreaver",			//FP_DARKGLYPH 
		"gfx/mp/f_icon_spectralreaver",			//FP_NECROGLYPH
	};
	// CLASS_SPIDERREAVER2 con mietitrice di polvere e fango - Usata anche da Anteo a media potenza
	const char	*force_icon_files15[NUM_FORCE_POWERS] = // UGNAUGHT - THE DUST AND MUD REAVER  - FIRST XADO CLASS
	{//icons matching enums forcePowers_t
		"gfx/mp/f_icon_lt_heal",		//FP_HEAL,
		"gfx/mp/f_icon_levitation",		//FP_LEVITATION,
		"gfx/mp/f_icon_speed",			//FP_SPEED,
		"gfx/mp/f_icon_push",			//FP_PUSH,
		"gfx/mp/f_icon_pull",			//FP_PULL,
		"gfx/mp/f_icon_lt_telepathy",	//FP_TELEPATHY,
		"gfx/mp/f_icon_dk_grip",		//FP_GRIP,
		"gfx/mp/f_icon_web",			//FP_LIGHTNING,
		"gfx/mp/f_icon_saber_throw",	//FP_SABERTHROW
		"gfx/mp/f_icon_saber_defend",	//FP_SABERDEFEND,
		"gfx/mp/f_icon_saber_attack",	//FP_SABERATTACK,
		"gfx/mp/f_icon_dk_trance",		//FP_RAGE,
		"gfx/mp/f_icon_lt_protect",		//FP_PROTECT,
		"gfx/mp/f_icon_lt_absorb",		//FP_ABSORB,
		"gfx/mp/f_icon_dk_souldrain",		//FP_DRAIN, FX MISSING 
		"gfx/mp/f_icon_sight2",			//FP_SEE,
		"gfx/mp/f_icon_stun",			//FP_STUN 
		"gfx/mp/f_icon_hate",			//FP_HATE
		"gfx/mp/f_icon_controlmind",	//FP_CONTROLMIND
		"gfx/mp/f_icon_webbed",	//FP_INCAPACITATE 
		"gfx/mp/f_icon_healother",			//FP_HEALOTHER 
		"gfx/mp/f_icon_wrack",			//FP_WRACK 
		"gfx/mp/f_icon_forceglyph",		//FP_FORCEGLYPH
		"gfx/mp/f_icon_dustreaver",		//FP_STONEGLYPH 
		"gfx/mp/f_icon_soundglyph",		//FP_SOUNDGLYPH
		"gfx/mp/f_icon_waterreaver",	//FP_WATERGLYPH
		"gfx/mp/f_icon_firereaver",	//FP_FIREGLYPH 
		"gfx/mp/f_icon_lightreaver",			//FP_SUNGLYPH 
		"gfx/mp/f_icon_airreaver",			//FP_WINDGLYPH
		"gfx/mp/f_icon_materialreaver",			//FP_HOLYGLYPH 
		"gfx/mp/f_icon_darkreaver",			//FP_DARKGLYPH 
		"gfx/mp/f_icon_mudreaver",			//FP_NECROGLYPH
	};
	// CLASS_EARTHREAVER mietitrice di pietra 
	const char	*force_icon_files16[NUM_FORCE_POWERS] = // LIZARD - TERRA E PIETRA ANTEO CLASS 
	{//icons matching enums forcePowers_t
		"gfx/mp/f_icon_lt_heal",		//FP_HEAL,
		"gfx/mp/f_icon_levitation",		//FP_LEVITATION,
		"gfx/mp/f_icon_speed",			//FP_SPEED,
		"gfx/mp/f_icon_push",			//FP_PUSH,
		"gfx/mp/f_icon_pull",			//FP_PULL,
		"gfx/mp/f_icon_lt_telepathy",	//FP_TELEPATHY,
		"gfx/mp/f_icon_dk_grip",		//FP_GRIP,
		"gfx/mp/f_icon_stone",			//FP_LIGHTNING,
		"gfx/mp/f_icon_saber_throw",	//FP_SABERTHROW
		"gfx/mp/f_icon_saber_defend",	//FP_SABERDEFEND,
		"gfx/mp/f_icon_saber_attack",	//FP_SABERATTACK,
		"gfx/mp/f_icon_dk_trance",		//FP_RAGE,
		"gfx/mp/f_icon_lt_protect",		//FP_PROTECT,
		"gfx/mp/f_icon_lt_absorb",		//FP_ABSORB,
		"gfx/mp/f_icon_dk_souldrain",		//FP_DRAIN,
		"gfx/mp/f_icon_sight2",			//FP_SEE,
		"gfx/mp/f_icon_stun",			//FP_STUN 
		"gfx/mp/f_icon_hate",			//FP_HATE
		"gfx/mp/f_icon_controlmind",	//FP_CONTROLMIND
		"gfx/mp/f_icon_stoned",	//FP_INCAPACITATE 
		"gfx/mp/f_icon_healother",			//FP_HEALOTHER 
		"gfx/mp/f_icon_wrack",			//FP_WRACK 
		"gfx/mp/f_icon_forceglyph",		//FP_FORCEGLYPH
		"gfx/mp/f_icon_stonereaver",		//FP_STONEGLYPH 
		"gfx/mp/f_icon_soundglyph",		//FP_SOUNDGLYPH
		"gfx/mp/f_icon_waterreaver",	//FP_WATERGLYPH
		"gfx/mp/f_icon_firereaver",	//FP_FIREGLYPH 
		"gfx/mp/f_icon_lightreaver",			//FP_SUNGLYPH 
		"gfx/mp/f_icon_airreaver",			//FP_WINDGLYPH
		"gfx/mp/f_icon_materialreaver",			//FP_HOLYGLYPH 
		"gfx/mp/f_icon_darkreaver",			//FP_DARKGLYPH 
		"gfx/mp/f_icon_spectralreaver",			//FP_NECROGLYPH
	};
	// For CLASS_SPIDERREAVER3  and CLASS_REELO: the Poison Reaver 
	const char	*force_icon_files17[NUM_FORCE_POWERS] = // REELO - THE DUST AND POISON REAVER  - SECOND XADO CLASS
	{//icons matching enums forcePowers_t
		"gfx/mp/f_icon_lt_heal",		//FP_HEAL,
		"gfx/mp/f_icon_levitation",		//FP_LEVITATION,
		"gfx/mp/f_icon_speed",			//FP_SPEED,
		"gfx/mp/f_icon_push",			//FP_PUSH,
		"gfx/mp/f_icon_pull",			//FP_PULL,
		"gfx/mp/f_icon_lt_telepathy",	//FP_TELEPATHY,
		"gfx/mp/f_icon_dk_grip",		//FP_GRIP,
		"gfx/mp/f_icon_web",			//FP_LIGHTNING,
		"gfx/mp/f_icon_saber_throw",	//FP_SABERTHROW
		"gfx/mp/f_icon_saber_defend",	//FP_SABERDEFEND,
		"gfx/mp/f_icon_saber_attack",	//FP_SABERATTACK,
		"gfx/mp/f_icon_dk_trance",		//FP_RAGE,
		"gfx/mp/f_icon_lt_protect",		//FP_PROTECT,
		"gfx/mp/f_icon_lt_absorb",		//FP_ABSORB,
		"gfx/mp/f_icon_dk_souldrain",		//FP_DRAIN, // MISSING FX 
		"gfx/mp/f_icon_sight2",			//FP_SEE,
		"gfx/mp/f_icon_stun",			//FP_STUN 
		"gfx/mp/f_icon_hate",			//FP_HATE
		"gfx/mp/f_icon_controlmind",	//FP_CONTROLMIND
		"gfx/mp/f_icon_webbed",	//FP_INCAPACITATE 
		"gfx/mp/f_icon_healother",			//FP_HEALOTHER 
		"gfx/mp/f_icon_wrack",			//FP_WRACK 
		"gfx/mp/f_icon_forceglyph",		//FP_FORCEGLYPH
		"gfx/mp/f_icon_dustreaver",		//FP_STONEGLYPH 
		"gfx/mp/f_icon_soundglyph",		//FP_SOUNDGLYPH
		"gfx/mp/f_icon_waterreaver",	//FP_WATERGLYPH
		"gfx/mp/f_icon_firereaver",	//FP_FIREGLYPH 
		"gfx/mp/f_icon_lightreaver",			//FP_SUNGLYPH 
		"gfx/mp/f_icon_airreaver",			//FP_WINDGLYPH
		"gfx/mp/f_icon_materialreaver",			//FP_HOLYGLYPH 
		"gfx/mp/f_icon_darkreaver",			//FP_DARKGLYPH 
		"gfx/mp/f_icon_poisonreaver",			//FP_NECROGLYPH
	};
	// Archimond Basic: dark reaver and reverb CLASS_NECROREAVER 
	const char	*force_icon_files18[NUM_FORCE_POWERS] = // COMMANDO, MURJJ - MORTE, ARCHIMOND. 
	{//icons matching enums forcePowers_t
		"gfx/mp/f_icon_lt_heal",		//FP_HEAL,
		"gfx/mp/f_icon_levitation",		//FP_LEVITATION,
		"gfx/mp/f_icon_speed",			//FP_SPEED,
		"gfx/mp/f_icon_push",			//FP_PUSH,
		"gfx/mp/f_icon_pull",			//FP_PULL,
		"gfx/mp/f_icon_lt_telepathy",	//FP_TELEPATHY,
		"gfx/mp/f_icon_dk_grip",		//FP_GRIP,
		"gfx/mp/f_icon_ghost",			//FP_LIGHTNING,
		"gfx/mp/f_icon_saber_throw",	//FP_SABERTHROW
		"gfx/mp/f_icon_saber_defend",	//FP_SABERDEFEND,
		"gfx/mp/f_icon_saber_attack",	//FP_SABERATTACK,
		"gfx/mp/f_icon_dk_necrofear",		//FP_RAGE,
		"gfx/mp/f_icon_lt_protect",		//FP_PROTECT,
		"gfx/mp/f_icon_lt_absorb",		//FP_ABSORB,
		"gfx/mp/f_icon_dk_souldrain",		//FP_DRAIN,
		"gfx/mp/f_icon_sight2",			//FP_SEE,
		"gfx/mp/f_icon_stun",			//FP_STUN 
		"gfx/mp/f_icon_hate",			//FP_HATE
		"gfx/mp/f_icon_controlmind",	//FP_CONTROLMIND
		"gfx/mp/f_icon_incapacitate",	//FP_INCAPACITATE 
		"gfx/mp/f_icon_healother",			//FP_HEALOTHER 
		"gfx/mp/f_icon_wrack",			//FP_WRACK 
		"gfx/mp/f_icon_forceglyph",		//FP_FORCEGLYPH
		"gfx/mp/f_icon_earthreaver",		//FP_STONEGLYPH 
		"gfx/mp/f_icon_soundglyph",		//FP_SOUNDGLYPH
		"gfx/mp/f_icon_waterreaver",	//FP_WATERGLYPH
		"gfx/mp/f_icon_firereaver",	//FP_FIREGLYPH 
		"gfx/mp/f_icon_lightreaver",			//FP_SUNGLYPH 
		"gfx/mp/f_icon_airreaver",			//FP_WINDGLYPH
		"gfx/mp/f_icon_materialreaver",			//FP_HOLYGLYPH 
		"gfx/mp/f_icon_darkreaver",			//FP_DARKGLYPH 
		"gfx/mp/f_icon_deathreaver",			//FP_NECROGLYPH
	};
	// ARCHIMOND WITH DEATH REAVER FULL POWER CLASS_MURJJ 
	const char	*force_icon_files19[NUM_FORCE_POWERS] = // COMMANDO, MURJJ - MORTE, ARCHIMOND. 
	{//icons matching enums forcePowers_t
		"gfx/mp/f_icon_lt_heal",		//FP_HEAL,
		"gfx/mp/f_icon_levitation",		//FP_LEVITATION,
		"gfx/mp/f_icon_speed",			//FP_SPEED,
		"gfx/mp/f_icon_push",			//FP_PUSH,
		"gfx/mp/f_icon_pull",			//FP_PULL,
		"gfx/mp/f_icon_lt_telepathy",	//FP_TELEPATHY,
		"gfx/mp/f_icon_dk_grip",		//FP_GRIP,
		"gfx/mp/f_icon_skull",			//FP_LIGHTNING,
		"gfx/mp/f_icon_saber_throw",	//FP_SABERTHROW
		"gfx/mp/f_icon_saber_defend",	//FP_SABERDEFEND,
		"gfx/mp/f_icon_saber_attack",	//FP_SABERATTACK,
		"gfx/mp/f_icon_dk_necrofear",		//FP_RAGE,
		"gfx/mp/f_icon_lt_protect",		//FP_PROTECT,
		"gfx/mp/f_icon_lt_absorb",		//FP_ABSORB,
		"gfx/mp/f_icon_dk_necrodrain",		//FP_DRAIN,
		"gfx/mp/f_icon_sight2",			//FP_SEE,
		"gfx/mp/f_icon_stun",			//FP_STUN 
		"gfx/mp/f_icon_hate",			//FP_HATE
		"gfx/mp/f_icon_controlmind",	//FP_CONTROLMIND
		"gfx/mp/f_icon_incapacitate",	//FP_INCAPACITATE 
		"gfx/mp/f_icon_healother",			//FP_HEALOTHER 
		"gfx/mp/f_icon_wrack",			//FP_WRACK 
		"gfx/mp/f_icon_forceglyph",		//FP_FORCEGLYPH
		"gfx/mp/f_icon_earthreaver",		//FP_STONEGLYPH 
		"gfx/mp/f_icon_soundglyph",		//FP_SOUNDGLYPH
		"gfx/mp/f_icon_waterreaver",	//FP_WATERGLYPH
		"gfx/mp/f_icon_firereaver",	//FP_FIREGLYPH 
		"gfx/mp/f_icon_lightreaver",			//FP_SUNGLYPH 
		"gfx/mp/f_icon_airreaver",			//FP_WINDGLYPH
		"gfx/mp/f_icon_materialreaver",			//FP_HOLYGLYPH 
		"gfx/mp/f_icon_darkreaver",			//FP_DARKGLYPH 
		"gfx/mp/f_icon_deathreaver",			//FP_NECROGLYPH
	};
	// RAZIEL AND RESPEN CLASS // RESPEN HA LA TRANCE, RAZIEL LA HOLLYSTASYS
	// CLASS_RAZIEL \ CLASS_MENTALIST 
	 const char	*force_icon_files20[NUM_FORCE_POWERS] = 
	  {//icons matching enums forcePowers_t
		"gfx/mp/f_icon_lt_heal",		//FP_HEAL,
		"gfx/mp/f_icon_levitation",		//FP_LEVITATION,
		"gfx/mp/f_icon_speed",			//FP_SPEED,
		"gfx/mp/f_icon_push",			//FP_PUSH,
		"gfx/mp/f_icon_pull",			//FP_PULL,
		"gfx/mp/f_icon_lt_telepathy",	//FP_TELEPATHY,
		"gfx/mp/f_icon_dk_grip",		//FP_GRIP,
		"gfx/mp/f_icon_dk_psychic",			//FP_LIGHTNING,
		"gfx/mp/f_icon_saber_throw",	//FP_SABERTHROW
		"gfx/mp/f_icon_saber_defend",	//FP_SABERDEFEND,
		"gfx/mp/f_icon_saber_attack",	//FP_SABERATTACK,
		"gfx/mp/f_icon_dk_trance",		//FP_RAGE, // RAZIEL DEVE AVER LA TRANCE NON IL BERSERK COME POTERE DI FURIA! (l'ha insegnato a respen che l'ha insegnato a bleed ) 
		"gfx/mp/f_icon_lt_protect",		//FP_PROTECT,
		"gfx/mp/f_icon_lt_absorb",		//FP_ABSORB,
		"gfx/mp/f_icon_dk_souldrain",		//FP_DRAIN,
		"gfx/mp/f_icon_sight2",			//FP_SEE,
		"gfx/mp/f_icon_stun",			//FP_STUN 
		"gfx/mp/f_icon_hate",			//FP_HATE
		"gfx/mp/f_icon_controlmind",	//FP_CONTROLMIND
		"gfx/mp/f_icon_hollystasis",	//FP_INCAPACITATE 
		"gfx/mp/f_icon_healother",			//FP_HEALOTHER 
		"gfx/mp/f_icon_wrack",			//FP_WRACK 
		"gfx/mp/f_icon_forceglyph",		//FP_FORCEGLYPH
		"gfx/mp/f_icon_stoneglyph",				//FP_STONEGLYPH 
		"gfx/mp/f_icon_soundglyph",			//FP_SOUNDGLYPH
		"gfx/mp/f_icon_waterglyph",			//FP_WATERGLYPH
		"gfx/mp/f_icon_fireglyph",			//FP_FIREGLYPH 
		"gfx/mp/f_icon_sunglyph",					//FP_SUNGLYPH 
		"gfx/mp/f_icon_airreaver",					//FP_WINDGLYPH
		"gfx/mp/f_icon_spiritreaver",				//FP_HOLYGLYPH 
		"gfx/mp/f_icon_darkreaver",					//FP_DARKGLYPH 
		"gfx/mp/f_icon_spectralreaver",					//FP_NECROGLYPH
	};

	 // Raziel Holy reaver Class 		
	const char	*force_icon_files21[NUM_FORCE_POWERS] = // STORMTROOPER - HOLY RAZIEL - RESPEN
	{//icons matching enums forcePowers_t
		"gfx/mp/f_icon_lt_heal",		//FP_HEAL,
		"gfx/mp/f_icon_levitation",		//FP_LEVITATION,
		"gfx/mp/f_icon_speed",			//FP_SPEED,
		"gfx/mp/f_icon_push",			//FP_PUSH,
		"gfx/mp/f_icon_pull",			//FP_PULL,
		"gfx/mp/f_icon_lt_telepathy",	//FP_TELEPATHY,
		"gfx/mp/f_icon_dk_grip",		//FP_GRIP,
		"gfx/mp/f_icon_holly",			//FP_LIGHTNING,
		"gfx/mp/f_icon_saber_throw",	//FP_SABERTHROW
		"gfx/mp/f_icon_saber_defend",	//FP_SABERDEFEND,
		"gfx/mp/f_icon_saber_attack",	//FP_SABERATTACK,
		"gfx/mp/f_icon_dk_trance",		//FP_RAGE,
		"gfx/mp/f_icon_lt_protect",		//FP_PROTECT,
		"gfx/mp/f_icon_lt_absorb",		//FP_ABSORB,
		"gfx/mp/f_icon_dk_souldrain",		//FP_DRAIN,
		"gfx/mp/f_icon_sight2",			//FP_SEE,
		"gfx/mp/f_icon_stun",			//FP_STUN 
		"gfx/mp/f_icon_hate",			//FP_HATE
		"gfx/mp/f_icon_controlmind",	//FP_CONTROLMIND
		"gfx/mp/f_icon_hollystasis",	//FP_INCAPACITATE 
		"gfx/mp/f_icon_healother",			//FP_HEALOTHER 
		"gfx/mp/f_icon_wrack",			//FP_WRACK 
		"gfx/mp/f_icon_forceglyph",		//FP_FORCEGLYPH
		"gfx/mp/f_icon_earthreaver",		//FP_STONEGLYPH 
		"gfx/mp/f_icon_soundglyph",		//FP_SOUNDGLYPH
		"gfx/mp/f_icon_waterreaver",	//FP_WATERGLYPH
		"gfx/mp/f_icon_firereaver",	//FP_FIREGLYPH 
		"gfx/mp/f_icon_lightreaver",			//FP_SUNGLYPH 
		"gfx/mp/f_icon_airreaver",			//FP_WINDGLYPH
		"gfx/mp/f_icon_holyreaver",			//FP_HOLYGLYPH 
		"gfx/mp/f_icon_darkreaver",			//FP_DARKGLYPH 
		"gfx/mp/f_icon_spectralreaver",			//FP_NECROGLYPH
	};
// CLASS_LUKE: Raziel with Life Reaver 
	const char	*force_icon_files22[NUM_FORCE_POWERS] = // LUKE CLASS - RAZIEL VITA - DRUIDI SARAFAN
	{//icons matching enums forcePowers_t
		"gfx/mp/f_icon_lt_heal",		//FP_HEAL,
		"gfx/mp/f_icon_levitation",		//FP_LEVITATION,
		"gfx/mp/f_icon_speed",			//FP_SPEED,
		"gfx/mp/f_icon_push",			//FP_PUSH,
		"gfx/mp/f_icon_pull",			//FP_PULL,
		"gfx/mp/f_icon_lt_telepathy",	//FP_TELEPATHY,
		"gfx/mp/f_icon_dk_grip",		//FP_GRIP,
		"gfx/mp/f_icon_life",			//FP_LIGHTNING,
		"gfx/mp/f_icon_saber_throw",	//FP_SABERTHROW
		"gfx/mp/f_icon_saber_defend",	//FP_SABERDEFEND,
		"gfx/mp/f_icon_saber_attack",	//FP_SABERATTACK,
		"gfx/mp/f_icon_dk_trance",		//FP_RAGE,
		"gfx/mp/f_icon_lt_protect",		//FP_PROTECT,
		"gfx/mp/f_icon_lt_absorb",		//FP_ABSORB,
		"gfx/mp/f_icon_dk_souldrain",		//FP_DRAIN,
		"gfx/mp/f_icon_sight2",			//FP_SEE,
		"gfx/mp/f_icon_stun",			//FP_STUN 
		"gfx/mp/f_icon_hate",			//FP_HATE
		"gfx/mp/f_icon_controlmind",	//FP_CONTROLMIND
		"gfx/mp/f_icon_hollystasis",	//FP_INCAPACITATE 
		"gfx/mp/f_icon_healother",			//FP_HEALOTHER 
		"gfx/mp/f_icon_wrack",			//FP_WRACK 
		"gfx/mp/f_icon_forceglyph",		//FP_FORCEGLYPH
		"gfx/mp/f_icon_earthreaver",		//FP_STONEGLYPH 
		"gfx/mp/f_icon_soundglyph",		//FP_SOUNDGLYPH
		"gfx/mp/f_icon_waterreaver",	//FP_WATERGLYPH
		"gfx/mp/f_icon_firereaver",	//FP_FIREGLYPH 
		"gfx/mp/f_icon_lightreaver",			//FP_SUNGLYPH 
		"gfx/mp/f_icon_airreaver",			//FP_WINDGLYPH
		"gfx/mp/f_icon_lifereaver",			//FP_HOLYGLYPH 
		"gfx/mp/f_icon_darkreaver",			//FP_DARKGLYPH 
		"gfx/mp/f_icon_spectralreaver",			//FP_NECROGLYPH
	};
	// Asgarath CLASS_DRUIDREAVER: ASGARATH WITH SONIC REAVER 
	const char	*force_icon_files23[NUM_FORCE_POWERS] = // TRANDOSHAN, LANDO - SUONO E ASTRALE 
	{//icons matching enums forcePowers_t
		"gfx/mp/f_icon_lt_heal",		//FP_HEAL,
		"gfx/mp/f_icon_levitation",		//FP_LEVITATION,
		"gfx/mp/f_icon_speed",			//FP_SPEED,
		"gfx/mp/f_icon_push",			//FP_PUSH,
		"gfx/mp/f_icon_pull",			//FP_PULL,
		"gfx/mp/f_icon_lt_telepathy",	//FP_TELEPATHY,
		"gfx/mp/f_icon_dk_grip",		//FP_GRIP,
		"gfx/mp/f_icon_hollyfire",			//FP_LIGHTNING,
		"gfx/mp/f_icon_saber_throw",	//FP_SABERTHROW
		"gfx/mp/f_icon_saber_defend",	//FP_SABERDEFEND,
		"gfx/mp/f_icon_saber_attack",	//FP_SABERATTACK,
		"gfx/mp/f_icon_dk_runeshield",		//FP_RAGE,
		"gfx/mp/f_icon_lt_protect",		//FP_PROTECT,
		"gfx/mp/f_icon_lt_absorb",		//FP_ABSORB,
		"gfx/mp/f_icon_dk_souldrain",		//FP_DRAIN,
		"gfx/mp/f_icon_sight2",			//FP_SEE,
		"gfx/mp/f_icon_stun",			//FP_STUN 
		"gfx/mp/f_icon_hate",			//FP_HATE
		"gfx/mp/f_icon_controlmind",	//FP_CONTROLMIND
		"gfx/mp/f_icon_ivytrap",		//FP_INCAPACITATE 
		"gfx/mp/f_icon_healother",			//FP_HEALOTHER 
		"gfx/mp/f_icon_wrack",			//FP_WRACK 
		"gfx/mp/f_icon_forceglyph",		//FP_FORCEGLYPH
		"gfx/mp/f_icon_earthreaver",		//FP_STONEGLYPH 
		"gfx/mp/f_icon_soundreaver",		//FP_SOUNDGLYPH
		"gfx/mp/f_icon_waterreaver",	//FP_WATERGLYPH
		"gfx/mp/f_icon_firereaver",	//FP_FIREGLYPH 
		"gfx/mp/f_icon_lightreaver",			//FP_SUNGLYPH 
		"gfx/mp/f_icon_airreaver",			//FP_WINDGLYPH
		"gfx/mp/f_icon_materialreaver",			//FP_HOLYGLYPH 
		"gfx/mp/f_icon_darkreaver",			//FP_DARKGLYPH 
		"gfx/mp/f_icon_spectralreaver",			//FP_NECROGLYPH
	};
	// CLASS_LANDO ASGARATH WITH ASTRAL REAVER 
	const char	*force_icon_files24[NUM_FORCE_POWERS] = // ASGARATH ASTRALE 
	{//icons matching enums forcePowers_t
		"gfx/mp/f_icon_lt_heal",		//FP_HEAL, // <- la cura di asghy è da cambiare anche nell'FX
		"gfx/mp/f_icon_levitation",		//FP_LEVITATION,
		"gfx/mp/f_icon_speed",			//FP_SPEED,
		"gfx/mp/f_icon_push",			//FP_PUSH,
		"gfx/mp/f_icon_pull",			//FP_PULL,
		"gfx/mp/f_icon_lt_telepathy",	//FP_TELEPATHY,
		"gfx/mp/f_icon_dk_grip",		//FP_GRIP,
		"gfx/mp/f_icon_astral",			//FP_LIGHTNING,
		"gfx/mp/f_icon_saber_throw",	//FP_SABERTHROW
		"gfx/mp/f_icon_saber_defend",	//FP_SABERDEFEND,
		"gfx/mp/f_icon_saber_attack",	//FP_SABERATTACK,
		"gfx/mp/f_icon_dk_runeshield",		//FP_RAGE,
		"gfx/mp/f_icon_lt_protect",		//FP_PROTECT,
		"gfx/mp/f_icon_lt_absorb",		//FP_ABSORB,
		"gfx/mp/f_icon_dk_necrodrain",		//FP_DRAIN,
		"gfx/mp/f_icon_sight2",			//FP_SEE,
		"gfx/mp/f_icon_stun",			//FP_STUN 
		"gfx/mp/f_icon_hate",			//FP_HATE
		"gfx/mp/f_icon_controlmind",	//FP_CONTROLMIND
		"gfx/mp/f_icon_gravitytrap",	//FP_INCAPACITATE 
		"gfx/mp/f_icon_healother",			//FP_HEALOTHER 
		"gfx/mp/f_icon_wrack",			//FP_WRACK 
		"gfx/mp/f_icon_forceglyph",		//FP_FORCEGLYPH
		"gfx/mp/f_icon_earthreaver",		//FP_STONEGLYPH 
		"gfx/mp/f_icon_astralreaver",		//FP_SOUNDGLYPH
		"gfx/mp/f_icon_waterreaver",	//FP_WATERGLYPH
		"gfx/mp/f_icon_firereaver",	//FP_FIREGLYPH 
		"gfx/mp/f_icon_lightreaver",			//FP_SUNGLYPH 
		"gfx/mp/f_icon_airreaver",			//FP_WINDGLYPH
		"gfx/mp/f_icon_materialreaver",			//FP_HOLYGLYPH 
		"gfx/mp/f_icon_darkreaver",			//FP_DARKGLYPH 
		"gfx/mp/f_icon_spectralreaver",			//FP_NECROGLYPH
	};
	
	
	/////////////// VAMPIRI //////////////////////////////////////////
	// CLASS_VAMPIRE \ REBORN \ DEFAULT CLASS VAMPIRE // CLASS_KAIN, CLASS_KAIN2
	
	// NON BOSS 
	// RAZIELHIM
	// KAIRON
	// REBORN
	// VAMPIRE:   
	// CLASS_KAIN2
	// CLASS_UMAH:  
	// CLASS_VAMPIRE_DARK:  
	// CLASS_VAMPIRE_NECRO:   
    // CLASS_VAMPIRE_SARAFAN
	// CLASS_SYFLFEAN 
	// CLASS_VAMPIRE_ELF
	// CLASS_MONMOTHA
	// CLASS_ALORA
	// CLASS_VAMPIRE_WATER
	// CLASS_VAMPIRE_LYCAN 
	// CLASS_VAMPIRE_SOUND 
	// CLASS_VAMPIRE_NECRO
	const char	*force_icon_files25[NUM_FORCE_POWERS] = // REBORN, ALORA // CDS VAMIRES
	{//icons matching enums forcePowers_t
		"gfx/mp/f_icon_v_heal",		//FP_HEAL,
		"gfx/mp/f_icon_levitation",		//FP_LEVITATION,
		"gfx/mp/f_icon_speed2",			//FP_SPEED,
		"gfx/mp/f_icon_push",			//FP_PUSH,
		"gfx/mp/f_icon_pull",			//FP_PULL,
		"gfx/mp/f_icon_wolfform",			//FP_TELEPATHY,
		"gfx/mp/f_icon_dk_grip",		//FP_GRIP,
		"gfx/mp/f_icon_dk_lt1",			//FP_LIGHTNING,
		"gfx/mp/f_icon_saber_throw",	//FP_SABERTHROW
		"gfx/mp/f_icon_saber_defend",	//FP_SABERDEFEND,
		"gfx/mp/f_icon_saber_attack",	//FP_SABERATTACK,
		"gfx/mp/f_icon_dk_rage",		//FP_RAGE,
		"gfx/mp/f_icon_lt_protect",		//FP_PROTECT,
		"gfx/mp/f_icon_lt_absorb",		//FP_ABSORB,
		"gfx/mp/f_icon_dk_drain",		//FP_DRAIN,- BLOOD SHOWER 
		"gfx/mp/f_icon_sight",			//FP_SEE,
		"gfx/mp/f_icon_stun",			//FP_STUN 
		"gfx/mp/f_icon_hate",			//FP_HATE
		"gfx/mp/f_icon_controlmind",	//FP_CONTROLMIND
		"gfx/mp/f_icon_incapacitate",	//FP_INCAPACITATE 
		"gfx/mp/f_icon_batform",			//FP_HEALOTHER 
		"gfx/mp/f_icon_wrack",			//FP_WRACK 
		"gfx/mp/f_icon_mistform",		//FP_FORCEGLYPH
		"gfx/mp/f_icon_timereaver",		//FP_STONEGLYPH 
		"gfx/mp/f_icon_dimensionreaver",		//FP_SOUNDGLYPH
		"gfx/mp/f_icon_statesreaver",	//FP_WATERGLYPH
		"gfx/mp/f_icon_conflictreaver",	//FP_FIREGLYPH 
		"gfx/mp/f_icon_energyreaver",			//FP_SUNGLYPH 
		"gfx/mp/f_icon_mindreaver",			//FP_WINDGLYPH
		"gfx/mp/f_icon_soulreaver",			//FP_HOLYGLYPH <- SOUL WILL NEVER USE THIS POWER!!
		"gfx/mp/f_icon_naturereaver",			//FP_DARKGLYPH 
		"gfx/mp/f_icon_bloodreaver",			//FP_NECROGLYPH// NON DARLO A KAIN
	};
	
	// Specific vampire classes HUD 
	// CLASS_VAMPIRE_FEAR
	// CLASS_VAMPIRE_FEAR2 Knight of Fear 
	const char	*force_icon_files26[NUM_FORCE_POWERS] = // WAMPA CLASS // VAMPIRI OSCURI - SOUL, SAM, PHOBOS
	{//icons matching enums forcePowers_t
		"gfx/mp/f_icon_v_heal",		//FP_HEAL,
		"gfx/mp/f_icon_levitation",		//FP_LEVITATION,
		"gfx/mp/f_icon_speed2",			//FP_SPEED,
		"gfx/mp/f_icon_push",			//FP_PUSH,
		"gfx/mp/f_icon_pull",			//FP_PULL,
		"gfx/mp/f_icon_wolfform2",	//FP_TELEPATHY,
		"gfx/mp/f_icon_dk_grip",		//FP_GRIP,
		"gfx/mp/f_icon_hell",			//FP_LIGHTNING,
		"gfx/mp/f_icon_saber_throw",	//FP_SABERTHROW
		"gfx/mp/f_icon_saber_defend",	//FP_SABERDEFEND,
		"gfx/mp/f_icon_saber_attack",	//FP_SABERATTACK,
		"gfx/mp/f_icon_dk_fear",		//FP_RAGE,
		"gfx/mp/f_icon_lt_protect2",		//FP_PROTECT,
		"gfx/mp/f_icon_lt_absorb",		//FP_ABSORB,
		"gfx/mp/f_icon_dk_demondrain",		//FP_DRAIN,
		"gfx/mp/f_icon_sight",			//FP_SEE,
		"gfx/mp/f_icon_feartrick",			//FP_STUN 
		"gfx/mp/f_icon_hate",			//FP_HATE
		"gfx/mp/f_icon_controlmind",	//FP_CONTROLMIND
		"gfx/mp/f_icon_incapacitate",	//FP_INCAPACITATE 
		"gfx/mp/f_icon_batform",			//FP_HEALOTHER 
		"gfx/mp/f_icon_wrack",			//FP_WRACK 
		"gfx/mp/f_icon_mistform2",		//FP_FORCEGLYPH
		"gfx/mp/f_icon_timereaver",		//FP_STONEGLYPH 
		"gfx/mp/f_icon_dimensionreaver",		//FP_SOUNDGLYPH
		"gfx/mp/f_icon_statesreaver",	//FP_WATERGLYPH
		"gfx/mp/f_icon_conflictreaver",	//FP_FIREGLYPH 
		"gfx/mp/f_icon_energyreaver",			//FP_SUNGLYPH 
		"gfx/mp/f_icon_fear",			//FP_WINDGLYPH - LVL 3 NON FUNZIONA! 
		"gfx/mp/f_icon_soulreaver",			//FP_HOLYGLYPH 
		"gfx/mp/f_icon_naturereaver",			//FP_DARKGLYPH 
		"gfx/mp/f_icon_deathreaver2",			//FP_NECROGLYPH
	};

	// CLASS_KAIN3 \ CLASS_KYLE \ CLASS_NECROMANCER
	// CLASS_VAMPIRE_QUEEN
	// CLASS_VORADOR
	// CLASS_JANOS
	// CLASS_VAMPIRE_NECRO2
	// CLASS HEARTLESS 

	const char	*force_icon_files27[NUM_FORCE_POWERS] = // KYLE CLASS - VAMPIRI KAIN
	{//icons matching enums forcePowers_t
		"gfx/mp/f_icon_v_heal",		//FP_HEAL,
		"gfx/mp/f_icon_levitation",		//FP_LEVITATION,
		"gfx/mp/f_icon_speed2",			//FP_SPEED,
		"gfx/mp/f_icon_push",			//FP_PUSH,
		"gfx/mp/f_icon_pull",			//FP_PULL,
		"gfx/mp/f_icon_wolfform",	//FP_TELEPATHY,
		"gfx/mp/f_icon_dk_grip",		//FP_GRIP,
		"gfx/mp/f_icon_dk_lt2",			//FP_LIGHTNING,
		"gfx/mp/f_icon_saber_throw",	//FP_SABERTHROW
		"gfx/mp/f_icon_saber_defend",	//FP_SABERDEFEND,
		"gfx/mp/f_icon_saber_attack",	//FP_SABERATTACK,
		"gfx/mp/f_icon_dk_rage",		//FP_RAGE,
		"gfx/mp/f_icon_lt_protect",		//FP_PROTECT,
		"gfx/mp/f_icon_lt_absorb",		//FP_ABSORB,
		"gfx/mp/f_icon_dk_drain2",		//FP_DRAIN,- BLOOD SHOWER 
		"gfx/mp/f_icon_sight",			//FP_SEE,
		"gfx/mp/f_icon_stun",			//FP_STUN 
		"gfx/mp/f_icon_hate",			//FP_HATE
		"gfx/mp/f_icon_controlmind",	//FP_CONTROLMIND
		"gfx/mp/f_icon_incapacitate",	//FP_INCAPACITATE 
		"gfx/mp/f_icon_batform",			//FP_HEALOTHER 
		"gfx/mp/f_icon_wrack",			//FP_WRACK 
		"gfx/mp/f_icon_mistform",		//FP_FORCEGLYPH
		"gfx/mp/f_icon_timereaver",		//FP_STONEGLYPH 
		"gfx/mp/f_icon_dimensionreaver",		//FP_SOUNDGLYPH
		"gfx/mp/f_icon_statesreaver",	//FP_WATERGLYPH
		"gfx/mp/f_icon_conflictreaver",	//FP_FIREGLYPH 
		"gfx/mp/f_icon_energyreaver",			//FP_SUNGLYPH 
		"gfx/mp/f_icon_mindreaver",			//FP_WINDGLYPH
		"gfx/mp/f_icon_balancereaver",			//FP_HOLYGLYPH <- MANCA LA SCRITTA SOUL WILL NEVER USE THIS POWER!!
		"gfx/mp/f_icon_naturereaver",			//FP_DARKGLYPH 
		"gfx/mp/f_icon_deathreaver2",			//FP_NECROGLYPH// NON DARLO A KAIN
	};
	// Rahabim and Water reaver // VA SETTATA COME I MIETITORI 
	const char	*force_icon_files28[NUM_FORCE_POWERS] = 
	{//icons matching enums forcePowers_t
		"gfx/mp/f_icon_lt_heal",		//FP_HEAL, // 
		"gfx/mp/f_icon_levitation",		//FP_LEVITATION,
		"gfx/mp/f_icon_speed",			//FP_SPEED,
		"gfx/mp/f_icon_push",			//FP_PUSH,
		"gfx/mp/f_icon_pull",			//FP_PULL,
		"gfx/mp/f_icon_lt_telepathy",			//FP_TELEPATHY,
		"gfx/mp/f_icon_dk_grip",		//FP_GRIP,
		"gfx/mp/f_icon_water",			//FP_LIGHTNING,
		"gfx/mp/f_icon_saber_throw",	//FP_SABERTHROW
		"gfx/mp/f_icon_saber_defend",	//FP_SABERDEFEND,
		"gfx/mp/f_icon_saber_attack",	//FP_SABERATTACK,
		"gfx/mp/f_icon_dk_berserk",		//FP_RAGE,
		"gfx/mp/f_icon_lt_protect",		//FP_PROTECT,
		"gfx/mp/f_icon_lt_absorb",		//FP_ABSORB,
		"gfx/mp/f_icon_dk_souldrain",		//FP_DRAIN,- BLOOD SHOWER 
		"gfx/mp/f_icon_sight2",			//FP_SEE,
		"gfx/mp/f_icon_stun",			//FP_STUN 
		"gfx/mp/f_icon_hate",			//FP_HATE
		"gfx/mp/f_icon_controlmind",	//FP_CONTROLMIND
		"gfx/mp/f_icon_frostcoffin",	//FP_INCAPACITATE 
		"gfx/mp/f_icon_healother",			//FP_HEALOTHER 
		"gfx/mp/f_icon_wrack",			//FP_WRACK 
		"gfx/mp/f_icon_forceglyph",		//FP_FORCEGLYPH
		"gfx/mp/f_icon_earthreaver",		//FP_STONEGLYPH 
		"gfx/mp/f_icon_soundglyph",		//FP_SOUNDGLYPH
		"gfx/mp/f_icon_waterreaver",	//FP_WATERGLYPH
		"gfx/mp/f_icon_firereaver",	//FP_FIREGLYPH 
		"gfx/mp/f_icon_lightreaver",			//FP_SUNGLYPH 
		"gfx/mp/f_icon_airreaver",			//FP_WINDGLYPH
		"gfx/mp/f_icon_materialreaver",			//FP_HOLYGLYPH <- SOUL WILL NEVER USE THIS POWER!!
		"gfx/mp/f_icon_darkreaver",			//FP_DARKGLYPH 
		"gfx/mp/f_icon_spectralreaver",			//FP_NECROGLYPH// NON DARLO A KAIN
	};
	// CLASS_VAMPIRE_ZOMBIE
	const char	*force_icon_files29[NUM_FORCE_POWERS] = // TUSKEN - UNDEAD VAMPIRES 
	{//icons matching enums forcePowers_t
		"gfx/mp/f_icon_v_heal",		//FP_HEAL,
		"gfx/mp/f_icon_levitation",		//FP_LEVITATION,
		"gfx/mp/f_icon_speed2",			//FP_SPEED,
		"gfx/mp/f_icon_push",			//FP_PUSH,
		"gfx/mp/f_icon_pull",			//FP_PULL,
		"gfx/mp/f_icon_wolfform",	//FP_TELEPATHY,
		"gfx/mp/f_icon_dk_grip",		//FP_GRIP,
		"gfx/mp/f_icon_necro",			//FP_LIGHTNING,
		"gfx/mp/f_icon_saber_throw",	//FP_SABERTHROW
		"gfx/mp/f_icon_saber_defend",	//FP_SABERDEFEND,
		"gfx/mp/f_icon_saber_attack",	//FP_SABERATTACK,
		"gfx/mp/f_icon_dk_rage",		//FP_RAGE,
		"gfx/mp/f_icon_lt_protect",		//FP_PROTECT,
		"gfx/mp/f_icon_lt_absorb",		//FP_ABSORB,
		"gfx/mp/f_icon_dk_drain",		//FP_DRAIN,
		"gfx/mp/f_icon_sight",			//FP_SEE,
		"gfx/mp/f_icon_stun",			//FP_STUN 
		"gfx/mp/f_icon_hate",			//FP_HATE
		"gfx/mp/f_icon_controlmind",	//FP_CONTROLMIND
		"gfx/mp/f_icon_incapacitate",	//FP_INCAPACITATE 
		"gfx/mp/f_icon_batform",			//FP_HEALOTHER 
		"gfx/mp/f_icon_wrack",			//FP_WRACK 
		"gfx/mp/f_icon_mistform",		//FP_FORCEGLYPH
		"gfx/mp/f_icon_timereaver",		//FP_STONEGLYPH 
		"gfx/mp/f_icon_dimensionreaver",		//FP_SOUNDGLYPH
		"gfx/mp/f_icon_statesreaver",	//FP_WATERGLYPH
		"gfx/mp/f_icon_conflictreaver",	//FP_FIREGLYPH 
		"gfx/mp/f_icon_energyreaver",			//FP_SUNGLYPH 
		"gfx/mp/f_icon_mindreaver",			//FP_WINDGLYPH
		"gfx/mp/f_icon_soulreaver",			//FP_HOLYGLYPH 
		"gfx/mp/f_icon_naturereaver",			//FP_DARKGLYPH 
		"gfx/mp/f_icon_bloodreaver",			//FP_NECROGLYPH
	};
	
	// OSIL AND SAMAH'EL KHAN 
	const char	*force_icon_files30[NUM_FORCE_POWERS] = // WAMPA CLASS // VAMPIRI OSCURI - SOUL, SAM, PHOBOS
	{//icons matching enums forcePowers_t
		"gfx/mp/f_icon_v_heal",		//FP_HEAL,
		"gfx/mp/f_icon_levitation",		//FP_LEVITATION,
		"gfx/mp/f_icon_speed2",			//FP_SPEED,
		"gfx/mp/f_icon_push",			//FP_PUSH,
		"gfx/mp/f_icon_pull",			//FP_PULL,
		"gfx/mp/f_icon_wolfform3",	//FP_TELEPATHY,
		"gfx/mp/f_icon_dk_grip",		//FP_GRIP,
		"gfx/mp/f_icon_hell",			//FP_LIGHTNING,
		"gfx/mp/f_icon_saber_throw",	//FP_SABERTHROW
		"gfx/mp/f_icon_saber_defend",	//FP_SABERDEFEND,
		"gfx/mp/f_icon_saber_attack",	//FP_SABERATTACK,
		"gfx/mp/f_icon_dk_rage",		//FP_RAGE,
		"gfx/mp/f_icon_lt_protect",		//FP_PROTECT,
		"gfx/mp/f_icon_lt_absorb",		//FP_ABSORB,
		"gfx/mp/f_icon_dk_demondrain",		//FP_DRAIN,
		"gfx/mp/f_icon_sight",			//FP_SEE,
		"gfx/mp/f_icon_stun",			//FP_STUN 
		"gfx/mp/f_icon_hate",			//FP_HATE
		"gfx/mp/f_icon_controlmind",	//FP_CONTROLMIND
		"gfx/mp/f_icon_incapacitate",	//FP_INCAPACITATE 
		"gfx/mp/f_icon_birdform",			//FP_HEALOTHER 
		"gfx/mp/f_icon_wrack",			//FP_WRACK 
		"gfx/mp/f_icon_mistform3",		//FP_FORCEGLYPH
		"gfx/mp/f_icon_timereaver",		//FP_STONEGLYPH 
		"gfx/mp/f_icon_dimensionreaver",		//FP_SOUNDGLYPH
		"gfx/mp/f_icon_statesreaver",	//FP_WATERGLYPH
		"gfx/mp/f_icon_conflictreaver",	//FP_FIREGLYPH 
		"gfx/mp/f_icon_energyreaver",			//FP_SUNGLYPH 
		"gfx/mp/f_icon_mindreaver",			//FP_WINDGLYPH
		"gfx/mp/f_icon_soulreaver",			//FP_HOLYGLYPH 
		"gfx/mp/f_icon_naturereaver",			//FP_DARKGLYPH 
		"gfx/mp/f_icon_deathreaver2",			//FP_NECROGLYPH
	};

	// FORMS MATRIX 
	const char	*force_icon_files31[NUM_FORCE_POWERS] = // REBORN, ALORA // CDS VAMIRES
	{//icons matching enums forcePowers_t
		"gfx/mp/f_icon_v_heal",		//FP_HEAL, LOCKED
		"gfx/mp/f_icon_levitation",		//FP_LEVITATION, LOCKED
		"gfx/mp/f_icon_speed2",			//FP_SPEED, LOCKED
		"gfx/mp/f_icon_push",			//FP_PUSH, LOCKED
		"gfx/mp/f_icon_pull",			//FP_PULL, LOCKED
		"gfx/mp/f_icon_wolftovampire",			//FP_TELEPATHY, REVERT WOLF FORM
		"gfx/mp/f_icon_dk_grip",		//FP_GRIP, LOCKED 
		"gfx/mp/f_icon_dk_lt1",			//FP_LIGHTNING, LOCKED
		"gfx/mp/f_icon_saber_throw",	//FP_SABERTHROW 
		"gfx/mp/f_icon_saber_defend",	//FP_SABERDEFEND,
		"gfx/mp/f_icon_saber_attack",	//FP_SABERATTACK,
		"gfx/mp/f_icon_dk_rage",		//FP_RAGE,LOCKED
		"gfx/mp/f_icon_lt_protect",		//FP_PROTECT, LOCKED
		"gfx/mp/f_icon_lt_absorb",		//FP_ABSORB, LOCKED
		"gfx/mp/f_icon_dk_drain",		//FP_DRAIN,- BLOOD SHOWER 
		"gfx/mp/f_icon_echo",			//FP_SEE, - ECHO SUONI NON FUNZIONANO 
		"gfx/mp/f_icon_stun",			//FP_STUN - LOCKED
		"gfx/mp/f_icon_hate",			//FP_HATE- LOCKED
		"gfx/mp/f_icon_controlmind",	//FP_CONTROLMIND - LOCKED
		"gfx/mp/f_icon_incapacitate",	//FP_INCAPACITATE - LOCKED
		"gfx/mp/f_icon_battovampire",			//FP_HEALOTHER - REVERT BAT FORM 
		"gfx/mp/f_icon_wrack",			//FP_WRACK - LOCKED 
		"gfx/mp/f_icon_misttovampire",		//FP_FORCEGLYPH - REVERT TO MIST FORM (GLIFO DI FORZA FUNZIONA ANCORA! ) 
		"gfx/mp/f_icon_timereaver",		//FP_STONEGLYPH  - LOCKED
		"gfx/mp/f_icon_dimensionreaver",		//FP_SOUNDGLYPH - LOCKED 
		"gfx/mp/f_icon_statesreaver",	//FP_WATERGLYPH - LOCKED 
		"gfx/mp/f_icon_conflictreaver",	//FP_FIREGLYPH - LOCKED 
		"gfx/mp/f_icon_energyreaver",			//FP_SUNGLYPH - LOCKED
		"gfx/mp/f_icon_mindreaver",			//FP_WINDGLYPH - LOCKED
		"gfx/mp/f_icon_soulreaver",			//FP_HOLYGLYPH - LOCKED
		"gfx/mp/f_icon_naturereaver",			//FP_DARKGLYPH - LOCKED 
		"gfx/mp/f_icon_bloodreaver",			//FP_NECROGLYPH// NON DARLO A KAIN
	};

	// CLASS DEMON POISON  (MAYBE FOR SHADE) 
	const char	*force_icon_files32[NUM_FORCE_POWERS] = // MINEMONSTER - THE POISON CLASS
	{//icons matching enums forcePowers_t
		"gfx/mp/f_icon_d_heal",		//FP_HEAL,
		"gfx/mp/f_icon_levitation",		//FP_LEVITATION,
		"gfx/mp/f_icon_speed",			//FP_SPEED,
		"gfx/mp/f_icon_push",			//FP_PUSH,
		"gfx/mp/f_icon_pull",			//FP_PULL,
		"gfx/mp/f_icon_lt_telepathy",	//FP_TELEPATHY,
		"gfx/mp/f_icon_dk_grip",		//FP_GRIP,
		"gfx/mp/f_icon_poison",			//FP_LIGHTNING,
		"gfx/mp/f_icon_saber_throw",	//FP_SABERTHROW
		"gfx/mp/f_icon_saber_defend",	//FP_SABERDEFEND,
		"gfx/mp/f_icon_saber_attack",	//FP_SABERATTACK,
		"gfx/mp/f_icon_dk_poisonshield",		//FP_RAGE,
		"gfx/mp/f_icon_lt_protect",		//FP_PROTECT,
		"gfx/mp/f_icon_lt_absorb",		//FP_ABSORB,
		"gfx/mp/f_icon_dk_demondrain",		//FP_DRAIN,
		"gfx/mp/f_icon_echo",			//FP_SEE,
		"gfx/mp/f_icon_stun",			//FP_STUN 
		"gfx/mp/f_icon_hate",			//FP_HATE
		"gfx/mp/f_icon_controlmind",	//FP_CONTROLMIND
		"gfx/mp/f_icon_incapacitate",	//FP_INCAPACITATE 
		"gfx/mp/f_icon_feartrick",			//FP_HEALOTHER 
		"gfx/mp/f_icon_wrack",			//FP_WRACK 
		"gfx/mp/f_icon_forceglyph",		//FP_FORCEGLYPH
		"gfx/mp/f_icon_stoneglyph",		//FP_STONEGLYPH 
		"gfx/mp/f_icon_soundglyph",		//FP_SOUNDGLYPH
		"gfx/mp/f_icon_waterglyph",	//FP_WATERGLYPH
		"gfx/mp/f_icon_fireglyph",	//FP_FIREGLYPH 
		"gfx/mp/f_icon_sunglyph",			//FP_SUNGLYPH 
		"gfx/mp/f_icon_airreaver",			//FP_WINDGLYPH
		"gfx/mp/f_icon_materialreaver",			//FP_HOLYGLYPH 
		"gfx/mp/f_icon_darkreaver",			//FP_DARKGLYPH 
		"gfx/mp/f_icon_poisonreaver",			//FP_NECROGLYPH
	};

	// HYLDEN CLASS MATRIX  
	// CLASS_HYLDEN \ CLASS_TAVION  
	const char	*force_icon_files33[NUM_FORCE_POWERS] = // DARWIN-LIQUID? - SWAMP 
	{//icons matching enums forcePowers_t
		"gfx/mp/f_icon_h_heal",		//FP_HEAL,
		"gfx/mp/f_icon_levitation",		//FP_LEVITATION,
		"gfx/mp/f_icon_speed",			//FP_SPEED,
		"gfx/mp/f_icon_push",			//FP_PUSH,
		"gfx/mp/f_icon_pull",			//FP_PULL,
		"gfx/mp/f_icon_lt_telepathy",	//FP_TELEPATHY,
		"gfx/mp/f_icon_dk_grip",		//FP_GRIP,
		"gfx/mp/f_icon_dk_hylden",			//FP_LIGHTNING,
		"gfx/mp/f_icon_saber_throw",	//FP_SABERTHROW
		"gfx/mp/f_icon_saber_defend",	//FP_SABERDEFEND,
		"gfx/mp/f_icon_saber_attack",	//FP_SABERATTACK,
		"gfx/mp/f_icon_dk_tkshield",		//FP_RAGE,
		"gfx/mp/f_icon_lt_protect",		//FP_PROTECT,
		"gfx/mp/f_icon_lt_absorb",		//FP_ABSORB,
		"gfx/mp/f_icon_dk_hyldendrain",		//FP_DRAIN,- BLOOD SHOWER 
		"gfx/mp/f_icon_sight5",			//FP_SEE,
		"gfx/mp/f_icon_stun",			//FP_STUN 
		"gfx/mp/f_icon_hate",			//FP_HATE
		"gfx/mp/f_icon_controlmind",	//FP_CONTROLMIND
		"gfx/mp/f_icon_incapacitate",	//FP_INCAPACITATE 
		"gfx/mp/f_icon_healother",			//FP_HEALOTHER 
		"gfx/mp/f_icon_wrack",			//FP_WRACK 
		"gfx/mp/f_icon_forceglyph",		//FP_FORCEGLYPH
		"gfx/mp/f_icon_stoneglyph",		//FP_STONEGLYPH 
		"gfx/mp/f_icon_soundglyph",		//FP_SOUNDGLYPH
		"gfx/mp/f_icon_waterglyph",	//FP_WATERGLYPH
		"gfx/mp/f_icon_fireglyph",	//FP_FIREGLYPH 
		"gfx/mp/f_icon_sunglyph",			//FP_SUNGLYPH 
		"gfx/mp/f_icon_windglyph",			//FP_WINDGLYPH
		"gfx/mp/f_icon_holyglyph",			//FP_HOLYGLYPH 
		"gfx/mp/f_icon_darkglyph",			//FP_DARKGLYPH 
		"gfx/mp/f_icon_necroglyph",			//FP_NECROGLYPH
	};

	// Xari
	const char	*force_icon_files34[NUM_FORCE_POWERS] = // NOGHRI FOR XARI
	{//icons matching enums forcePowers_t
		"gfx/mp/f_icon_v_heal",		//FP_HEAL,
		"gfx/mp/f_icon_levitation",		//FP_LEVITATION,
		"gfx/mp/f_icon_speed2",			//FP_SPEED,
		"gfx/mp/f_icon_push",			//FP_PUSH,
		"gfx/mp/f_icon_pull",			//FP_PULL,
		"gfx/mp/f_icon_wolfform",	//FP_TELEPATHY,
		"gfx/mp/f_icon_dk_grip",		//FP_GRIP,
		"gfx/mp/f_icon_web",			//FP_LIGHTNING,
		"gfx/mp/f_icon_saber_throw",	//FP_SABERTHROW
		"gfx/mp/f_icon_saber_defend",	//FP_SABERDEFEND,
		"gfx/mp/f_icon_saber_attack",	//FP_SABERATTACK,
		"gfx/mp/f_icon_dk_rage",		//FP_RAGE,
		"gfx/mp/f_icon_lt_protect",		//FP_PROTECT,
		"gfx/mp/f_icon_lt_absorb",		//FP_ABSORB,
		"gfx/mp/f_icon_dk_drain",		//FP_DRAIN,
		"gfx/mp/f_icon_sight",			//FP_SEE,
		"gfx/mp/f_icon_stun",			//FP_STUN 
		"gfx/mp/f_icon_hate",			//FP_HATE
		"gfx/mp/f_icon_controlmind",	//FP_CONTROLMIND
		"gfx/mp/f_icon_webbed",	//FP_INCAPACITATE 
		"gfx/mp/f_icon_batform",			//FP_HEALOTHER 
		"gfx/mp/f_icon_wrack",			//FP_WRACK 
		"gfx/mp/f_icon_mistform",		//FP_FORCEGLYPH
		"gfx/mp/f_icon_timereaver",		//FP_STONEGLYPH 
		"gfx/mp/f_icon_dimensionreaver",		//FP_SOUNDGLYPH
		"gfx/mp/f_icon_statesreaver",	//FP_WATERGLYPH
		"gfx/mp/f_icon_conflictreaver",	//FP_FIREGLYPH 
		"gfx/mp/f_icon_energyreaver",			//FP_SUNGLYPH 
		"gfx/mp/f_icon_mindreaver",			//FP_WINDGLYPH
		"gfx/mp/f_icon_soulreaver",			//FP_HOLYGLYPH 
		"gfx/mp/f_icon_naturereaver",			//FP_DARKGLYPH 
		"gfx/mp/f_icon_bloodreaver",			//FP_NECROGLYPH
	};

	// Emia - Sarafans	
	// CLASS_JAN? 
	const char	*force_icon_files35[NUM_FORCE_POWERS] = // JAN - SARAFAN LORD CLASS 
	{//icons matching enums forcePowers_t
		"gfx/mp/f_icon_s_heal",		//FP_HEAL,
		"gfx/mp/f_icon_levitation",		//FP_LEVITATION,
		"gfx/mp/f_icon_speed",			//FP_SPEED,
		"gfx/mp/f_icon_push",			//FP_PUSH,
		"gfx/mp/f_icon_pull",			//FP_PULL,
		"gfx/mp/f_icon_lt_telepathy",	//FP_TELEPATHY,
		"gfx/mp/f_icon_dk_grip",		//FP_GRIP,
		"gfx/mp/f_icon_holly",			//FP_LIGHTNING, // Palle di fuoco come i cacciatori XD
		"gfx/mp/f_icon_saber_throw",	//FP_SABERTHROW
		"gfx/mp/f_icon_saber_defend",	//FP_SABERDEFEND,
		"gfx/mp/f_icon_saber_attack",	//FP_SABERATTACK,
		"gfx/mp/f_icon_dk_berserk",		//FP_RAGE,
		"gfx/mp/f_icon_lt_protect",		//FP_PROTECT,
		"gfx/mp/f_icon_lt_absorb",		//FP_ABSORB,
		"gfx/mp/f_icon_dk_necrodrain",		//FP_DRAIN,
		"gfx/mp/f_icon_sight4",			//FP_SEE,
		"gfx/mp/f_icon_holystun",			//FP_STUN 
		"gfx/mp/f_icon_holyhate",			//FP_HATE
		"gfx/mp/f_icon_holycontrolmind",	//FP_CONTROLMIND
		"gfx/mp/f_icon_holyincapacitate",	//FP_INCAPACITATE // Cambia icona no BOLAS 
		"gfx/mp/f_icon_healother",			//FP_HEALOTHER 
		"gfx/mp/f_icon_holywrack",			//FP_WRACK 
		"gfx/mp/f_icon_forceglyph",		//FP_FORCEGLYPH
		"gfx/mp/f_icon_stoneglyph",		//FP_STONEGLYPH 
		"gfx/mp/f_icon_soundglyph",		//FP_SOUNDGLYPH
		"gfx/mp/f_icon_waterglyph",	//FP_WATERGLYPH
		"gfx/mp/f_icon_fireglyph",	//FP_FIREGLYPH 
		"gfx/mp/f_icon_sunglyph",			//FP_SUNGLYPH 
		"gfx/mp/f_icon_windglyph",			//FP_WINDGLYPH
		"gfx/mp/f_icon_holyglyph",			//FP_HOLYGLYPH 
		"gfx/mp/f_icon_darkglyph",			//FP_DARKGLYPH 
		"gfx/mp/f_icon_necroglyph",			//FP_NECROGLYPH
	};

	// Elf Vampire evolved 
	// Elf Vampire 2 AND 3
	const char	*force_icon_files36[NUM_FORCE_POWERS] = // MON MOTHA // CDS VAMIRES
	{//icons matching enums forcePowers_t
		"gfx/mp/f_icon_v_heal",		//FP_HEAL,
		"gfx/mp/f_icon_levitation",		//FP_LEVITATION,
		"gfx/mp/f_icon_speed2",			//FP_SPEED,NEW ICON - VELOCITA' ARMOR
		"gfx/mp/f_icon_push",			//FP_PUSH,
		"gfx/mp/f_icon_pull",			//FP_PULL,
		"gfx/mp/f_icon_lionform",	//FP_TELEPATHY,
		"gfx/mp/f_icon_dk_grip",		//FP_GRIP,
		"gfx/mp/f_icon_dk_lt1",			//FP_LIGHTNING,
		"gfx/mp/f_icon_saber_throw",	//FP_SABERTHROW
		"gfx/mp/f_icon_saber_defend",	//FP_SABERDEFEND,
		"gfx/mp/f_icon_saber_attack",	//FP_SABERATTACK,
		"gfx/mp/f_icon_dk_rage",		//FP_RAGE,
		"gfx/mp/f_icon_lt_protect",		//FP_PROTECT,
		"gfx/mp/f_icon_lt_absorb",		//FP_ABSORB,
		"gfx/mp/f_icon_dk_drain",		//FP_DRAIN,- BLOOD SHOWER 
		"gfx/mp/f_icon_sight",			//FP_SEE,
		"gfx/mp/f_icon_stun",			//FP_STUN 
		"gfx/mp/f_icon_hate",			//FP_HATE
		"gfx/mp/f_icon_controlmind",	//FP_CONTROLMIND
		"gfx/mp/f_icon_incapacitate",	//FP_INCAPACITATE 
		"gfx/mp/f_icon_chyroptera",			//FP_HEALOTHER 
		"gfx/mp/f_icon_wrack",			//FP_WRACK 
		"gfx/mp/f_icon_mistform",		//FP_FORCEGLYPH
		"gfx/mp/f_icon_timereaver",		//FP_STONEGLYPH 
		"gfx/mp/f_icon_dimensionreaver",		//FP_SOUNDGLYPH
		"gfx/mp/f_icon_statesreaver",	//FP_WATERGLYPH
		"gfx/mp/f_icon_conflictreaver",	//FP_FIREGLYPH 
		"gfx/mp/f_icon_energyreaver",			//FP_SUNGLYPH 
		"gfx/mp/f_icon_mindreaver",			//FP_WINDGLYPH
		"gfx/mp/f_icon_soulreaver",			//FP_HOLYGLYPH <- SOUL WILL NEVER USE THIS POWER!!
		"gfx/mp/f_icon_naturereaver",			//FP_DARKGLYPH 
		"gfx/mp/f_icon_bloodreaver",			//FP_NECROGLYPH// NON DARLO A KAIN
	};



	// Precache inventory icons
		
	for ( int i=0;i<NUM_FORCE_POWERS;i++) // int i=0;i<NUM_FORCE_POWERS;i++
	{				
		if (force_icon_files[i])
		{
			force_icons[i] = cgi_R_RegisterShaderNoMip( force_icon_files[i] ); // DEFAULT MIETITORI - CLASS_REAVER, CLASS_JEDI, For Rekius, Tiziel, Bleed and Others 
			force_icons1[i] = cgi_R_RegisterShaderNoMip( force_icon_files1[i] ); // TENEBRE - CLASS_DARKREAVER, CLASS_SABOTEUR. 
			force_icons2[i] = cgi_R_RegisterShaderNoMip( force_icon_files2[i] ); // FUMO - CLASS_TANKREAVER, CLASS_BESPIN_COP
			force_icons3[i] = cgi_R_RegisterShaderNoMip( force_icon_files3[i] ); // VUOTO - CLASS_SHADOWTROOPER 
			force_icons4[i] = cgi_R_RegisterShaderNoMip( force_icon_files4[i] ); // FULMINE - CLASS_LIGHTREAVER 
			force_icons5[i] = cgi_R_RegisterShaderNoMip( force_icon_files5[i] ); // RADIANZA - CLASS_MORGANKATARN 
			force_icons6[i] = cgi_R_RegisterShaderNoMip( force_icon_files6[i] ); // CALORE - CLASS_FIREREAVER \ HAZARDTROOPER
			force_icons7[i] = cgi_R_RegisterShaderNoMip( force_icon_files7[i] ); // MAGMA - CLASS_CLAW 
			force_icons8[i] = cgi_R_RegisterShaderNoMip( force_icon_files8[i] ); // AIR \ NAERYAN BASIC CLASS_REBEL 
			force_icons9[i] = cgi_R_RegisterShaderNoMip( force_icon_files9[i] ); // NEBBIA CLASS_AIRREAVER 
			force_icons10[i] = cgi_R_RegisterShaderNoMip( force_icon_files10[i] ); // TEMPESTA CLASS_ROCKETTROOOPER 
			force_icons11[i] = cgi_R_RegisterShaderNoMip( force_icon_files11[i] ); // SEED , SONIC VAMPIRE
			force_icons12[i] = cgi_R_RegisterShaderNoMip( force_icon_files12[i] ); // GHIACCIO CLASS_ICEREAVER 
			force_icons13[i] = cgi_R_RegisterShaderNoMip( force_icon_files13[i] ); // CRISTALLO CLASS_CRYSTAL  
			force_icons14[i] = cgi_R_RegisterShaderNoMip( force_icon_files14[i] ); // CLASS_SPIDERREAVER EARTH REAVER  
			force_icons15[i] = cgi_R_RegisterShaderNoMip( force_icon_files15[i] ); // CLASS_SPIDERREAVER2 FANGO E POLVERE / CLASS_UGNAUGHT
			force_icons16[i] = cgi_R_RegisterShaderNoMip( force_icon_files16[i] ); // CLASS_EARTHREAVER: Stone reaver 
			force_icons17[i] = cgi_R_RegisterShaderNoMip( force_icon_files17[i] ); // CLASS XADO3 and REELO: Poison Reaver 
			force_icons18[i] = cgi_R_RegisterShaderNoMip( force_icon_files18[i] ); // CLASS_NECROREAVER : dark reaver and reverb  
			force_icons19[i] = cgi_R_RegisterShaderNoMip( force_icon_files19[i] ); // CLASS_MURJJ: archimond con death reaver
			force_icons20[i] = cgi_R_RegisterShaderNoMip( force_icon_files20[i] ); // CLASS_RAZIEL \ CLASS_MENTALIST 
			force_icons21[i] = cgi_R_RegisterShaderNoMip( force_icon_files21[i] ); // CLASS_HOLY / Raziel with holy reaver
			force_icons22[i] = cgi_R_RegisterShaderNoMip( force_icon_files22[i] ); // CLASS_LUKE // Raziel with Life reaver 
			force_icons23[i] = cgi_R_RegisterShaderNoMip( force_icon_files23[i] ); // CLASS_DRUIDREAVER // SOUND REAVER  
			force_icons24[i] = cgi_R_RegisterShaderNoMip( force_icon_files24[i] ); // CLASS_LANDO // Astral reaver 
			force_icons25[i] = cgi_R_RegisterShaderNoMip( force_icon_files25[i] ); // VAMPIRI CDS STANDARD CLASS FOR ALL CHARACTERS 
			force_icons26[i] = cgi_R_RegisterShaderNoMip( force_icon_files26[i] ); // VAMPIRI CDS - CLASS_VAMPIRE_FEAR2 
			force_icons27[i] = cgi_R_RegisterShaderNoMip( force_icon_files27[i] ); // VAMPIRI CDS ANZIANI KAIN3, SOUL, SHEER, DEATHMASK2, VORADOR, JANOS
			force_icons28[i] = cgi_R_RegisterShaderNoMip( force_icon_files28[i] ); // DARWIN RAHABIM REAVER CLASS
			force_icons29[i] = cgi_R_RegisterShaderNoMip( force_icon_files29[i] ); // VAMPIRI CDS EZEKIEL E MELCHIAHIM
			force_icons30[i] = cgi_R_RegisterShaderNoMip( force_icon_files30[i] ); // VAMPIRI CDS OSIL e SAMAH'EL VAMPIRES DEMONS 
			force_icons31[i] = cgi_R_RegisterShaderNoMip( force_icon_files31[i] ); // VAMPIRI WOLF \ MIST \ BAT FORM MATRIX   
			force_icons32[i] = cgi_R_RegisterShaderNoMip( force_icon_files32[i] ); //  SHADE CLASS_DEMON_POISON \ MINEMONSTER CLASS  
			force_icons33[i] = cgi_R_RegisterShaderNoMip( force_icon_files33[i] ); // HYLDEN CLASS   
			force_icons34[i] = cgi_R_RegisterShaderNoMip( force_icon_files34[i] ); // CLASS_NOGHRI \ ZEPH for Xari 
			force_icons35[i] = cgi_R_RegisterShaderNoMip( force_icon_files35[i] ); // CLASS SARAFAN \\ JAN FOR EMIA  
			force_icons36[i] = cgi_R_RegisterShaderNoMip( force_icon_files36[i] ); // CLASS_VAMPIRE_ELF EVOLVED.   
	    }
	}

	CG_LoadHudMenu();      // load new hud stuff
	cgi_UI_Menu_OpenByName("loadscreen");
	//rww - Moved from CG_GameStateReceived (we don't want to clear perm ents)
	memset( cg_entities, 0, sizeof(cg_entities) );
	CG_TransitionPermanent();
	cg.loadLCARSStage		= 0;
	CG_GameStateReceived();
	CG_InitConsoleCommands();

	//
	// the game server will interpret these commands, which will be automatically
	// forwarded to the server after they are not recognized locally
	//
	cgi_AddCommand ("kill");
	cgi_AddCommand ("give");
	cgi_AddCommand ("god");
	cgi_AddCommand ("notarget");
	cgi_AddCommand ("noclip");
	cgi_AddCommand ("undying");
	cgi_AddCommand ("setviewpos");
	cgi_AddCommand ("setobjective");
	cgi_AddCommand ("viewobjective");


//ConsoleCommand in g_svcmds.cpp
	cgi_AddCommand ("entitylist");
	cgi_AddCommand ("nav");
	cgi_AddCommand ("npc");

	cgi_AddCommand ("saberColor");
	cgi_AddCommand ("saber");
	cgi_AddCommand ("saberblade");
	cgi_AddCommand ("setForceAll");

	cgi_AddCommand ("runscript");

	cgi_AddCommand ("playerteam");
	cgi_AddCommand ("playermodel");

	cgi_AddCommand ("saberAttackCycle");

	cgi_AddCommand ("use_electrobinoculars");
	cgi_AddCommand ("use_bacta");
	cgi_AddCommand ("use_seeker");
	cgi_AddCommand ("use_lightamp_goggles");
	cgi_AddCommand ("use_sentry");

	cgi_AddCommand ("force_throw");
	cgi_AddCommand ("force_pull");
	cgi_AddCommand ("force_speed");
	cgi_AddCommand ("force_heal");
	cgi_AddCommand ("force_grip");
	cgi_AddCommand ("force_distract");
	cgi_AddCommand ("force_rage");
	cgi_AddCommand ("force_protect");
	cgi_AddCommand ("force_absorb");
	cgi_AddCommand ("force_sight");
	cgi_AddCommand ("force_stun");
	cgi_AddCommand ("force_hate");
	cgi_AddCommand ("force_controlmind");
	cgi_AddCommand ("force_freeze");
	cgi_AddCommand ("force_fear");
	cgi_AddCommand ("force_wrack");
	cgi_AddCommand ("force_repel");
	cgi_AddCommand ("force_glyph");
	cgi_AddCommand ("stone_glyph");
	cgi_AddCommand ("sound_glyph");
	cgi_AddCommand ("water_glyph");
	cgi_AddCommand ("fire_glyph");
	cgi_AddCommand ("sun_glyph");

	cgi_AddCommand ("taunt");
	cgi_AddCommand ("bow");
	cgi_AddCommand ("meditate");
	cgi_AddCommand ("flourish");
	cgi_AddCommand ("gloat");

	cg.weaponPickupTextTime = 0;

	cg.missionInfoFlashTime = 0;
	cg.missionStatusShow = qfalse;

	cg.missionFailedScreen = qfalse;	// Screen hasn't been opened.
	cgi_UI_MenuCloseAll();	/*for (int i = 0; i < WP_NUM_WEAPONS; i++ ) CG_RegisterWeapon(i); }// So the loadscreen menu will turn off just after the opening snapshot
	cgi_UI_MenuCloseAll();	// So the loadscreen menu will turn off just after the opening snapshot*/
}

/*
=================
CG_Shutdown

Called before every level change or subsystem restart
=================
*/
void CG_Shutdown( void ) 
{
	in_camera = false;
	FX_Free();
}

//// DEBUG STUFF
/*
-------------------------
CG_DrawNode
-------------------------
*/
void CG_DrawNode( vec3_t origin, int type )
{
	localEntity_t	*ex;

	ex = CG_AllocLocalEntity();

	ex->leType = LE_SPRITE;
	ex->startTime = cg.time;
	ex->endTime = ex->startTime + 51;
	VectorCopy( origin, ex->refEntity.origin );
		
	ex->refEntity.customShader = cgi_R_RegisterShader( "gfx/misc/nav_node" );

	float	scale = 16.0f;

	switch ( type )
	{
	case NODE_NORMAL:
		ex->color[0] = 255;
		ex->color[1] = 255;
		ex->color[2] = 0;
		break;

	case NODE_FLOATING:
		ex->color[0] = 0;
		ex->color[1] = 255;
		ex->color[2] = 255;
		scale += 16.0f;
		break;

	case NODE_GOAL:
		ex->color[0] = 255;
		ex->color[1] = 0;
		ex->color[2] = 0;
		scale += 16.0f;
		break;

	case NODE_NAVGOAL:
		ex->color[0] = 0;
		ex->color[1] = 255;
		ex->color[2] = 0;
		break;
	}
	
	ex->radius = scale;
}

/*
-------------------------
CG_DrawRadius
-------------------------
*/

void CG_DrawRadius( vec3_t origin, unsigned int radius, int type )
{
	localEntity_t	*ex;

	ex = CG_AllocLocalEntity();

	ex->leType = LE_QUAD;
	ex->radius = radius;
	ex->startTime = cg.time;
	ex->endTime = ex->startTime + 51;
	VectorCopy( origin, ex->refEntity.origin );
		
	ex->refEntity.customShader = cgi_R_RegisterShader( "gfx/misc/nav_radius" );

	switch ( type )
	{
	case NODE_NORMAL:
		ex->color[0] = 255;
		ex->color[1] = 255;
		ex->color[2] = 0;
		break;

	case NODE_FLOATING:
		ex->color[0] = 0;
		ex->color[1] = 255;
		ex->color[2] = 255;
		break;

	case NODE_GOAL:
		ex->color[0] = 255;
		ex->color[1] = 0;
		ex->color[2] = 0;
		break;

	case NODE_NAVGOAL:
		ex->color[0] = 0;
		ex->color[1] = 255;
		ex->color[2] = 0;
		break;
	}
}

/*
-------------------------
CG_DrawEdge
-------------------------
*/

void CG_DrawEdge( vec3_t start, vec3_t end, int type )
{
	switch ( type )
	{	
		// NAVIGATION EDGES BETWEEN POINTS
		//=====================================
	case EDGE_NORMAL:
		{
			FX_AddLine( start, end, 8.0f, 4.0f, 0.0f, 0.5f, 0.5f, 51, cgi_R_RegisterShader( "gfx/misc/nav_line" ), 0 );
		}
		break;
	case EDGE_LARGE:
		{
			FX_AddLine( start, end, 8.0f, 15.0f, 0.0f, 0.5f, 0.5f, 51, cgi_R_RegisterShader( "gfx/misc/nav_line" ), 0 );
		}
		break;	
	case EDGE_BLOCKED:
		{
			vec3_t	color = { 255, 0, 0 };	// RED
			FX_AddLine( start, end, 8.0f, 4.0f, 0.0f, 0.5f, 0.5f, color, color, 51, cgi_R_RegisterShader( "gfx/misc/nav_line" ), 0 );
		}
		break;
	case EDGE_FLY:
		{
			vec3_t	color = { 0, 255, 255 };// GREEN
			FX_AddLine( start, end, 8.0f, 4.0f, 0.0f, 0.5f, 0.5f, color, color, 51, cgi_R_RegisterShader( "gfx/misc/nav_line" ), 0 );
		}
		break;
	case EDGE_JUMP:
		{
			vec3_t	color = { 0, 0, 255 };	// BLUE
			FX_AddLine( start, end, 8.0f, 4.0f, 0.0f, 0.5f, 0.5f, color, color, 51, cgi_R_RegisterShader( "gfx/misc/nav_line" ), 0 );
		}
		break;



		// EDGE NODES
		//=====================================
	case EDGE_NODE_NORMAL:
		{
			vec3_t	color = { 155, 155, 155 };
			FX_AddLine( start, end, 2.0f, 1.0f, 0.0f, 1.0f, 1.0f, color, color, 151, cgi_R_RegisterShader( "gfx/misc/whiteline2" ), 0 );
		}
		break;
	case EDGE_NODE_FLOATING:
		{
			vec3_t	color = { 155, 155, 0 };
			FX_AddLine( start, end, 2.0f, 1.0f, 0.0f, 1.0f, 1.0f, color, color, 151, cgi_R_RegisterShader( "gfx/misc/whiteline2" ), 0 );
		}
		break;
	case EDGE_NODE_GOAL:
		{
			vec3_t	color = { 0, 0, 155 };
			FX_AddLine( start, end, 2.0f, 1.0f, 0.0f, 1.0f, 1.0f, color, color, 151, cgi_R_RegisterShader( "gfx/misc/whiteline2" ), 0 );
		}
		break;
	case EDGE_NODE_COMBAT:
		{
			vec3_t	color = { 155, 0, 0 };
			FX_AddLine( start, end, 2.0f, 1.0f, 0.0f, 1.0f, 1.0f, color, color, 151, cgi_R_RegisterShader( "gfx/misc/whiteline2" ), 0 );
		}
		break;




		// NEAREST NAV 
		//=====================================
	case EDGE_NEARESTVALID:
		{
			vec3_t	color = { 155, 155, 155 };	// WHITE
			FX_AddLine( -1, start, end, 1.0f, 1.0f, 0, 1.0f, 1.0f, FX_ALPHA_LINEAR, color, color, 0, 51, cgi_R_RegisterShader( "gfx/misc/whiteline2" ), 0, 0 );
		}
		break;

	case EDGE_NEARESTINVALID:
		{
			vec3_t	color = { 155, 0, 0 };	// WHITE
			FX_AddLine( -1, start, end, 1.0f, 1.0f, 0, 1.0f, 1.0f, FX_ALPHA_LINEAR, color, color, 0, 51, cgi_R_RegisterShader( "gfx/misc/whiteline2" ), 0, 0 );
		}
		break;


		// NEAREST NAV CELLS
		//=====================================
	case EDGE_CELL:
		{
			vec3_t	color = { 155, 155, 155 };	// WHITE
			FX_AddLine( -1, start, end, 1.0f, 1.0f, 0, 1.0f, 1.0f, FX_ALPHA_LINEAR, color, color, 0, 51, cgi_R_RegisterShader( "gfx/misc/whiteline2" ), 0, 0 );
		}
		break;
	case EDGE_CELL_EMPTY:
		{	
			vec3_t	color = { 255, 0, 0 };		// RED
			FX_AddLine( -1, start, end, 1.0f, 1.0f, 0, 1.0f, 1.0f, FX_ALPHA_LINEAR, color, color, 0, 51, cgi_R_RegisterShader( "gfx/misc/whiteline2" ), 0, 0 );
		}
		break;


		// ACTOR PATHS
		//=============
	case EDGE_PATH:
		{
  			vec3_t	color = { 0, 0, 155 };		// WHITE
 			FX_AddLine( start, end, 5.0f, 5.0f, 0.0f, 0.5f, 0.5f, color, color, 151, cgi_R_RegisterShader( "gfx/misc/nav_arrow_new" ), 0 );
		}
		break;

	case EDGE_PATHBLOCKED:
		{
 			vec3_t	color = { 255, 0, 0 };		// RED
			FX_AddLine( start, end, 5.0f, 5.0f, 0.0f, 0.5f, 0.5f, color, color, 151, cgi_R_RegisterShader( "gfx/misc/nav_arrow_new" ), 0 );
			break;
		}

	case EDGE_FOLLOWPOS:
		{
			vec3_t	color = { 0, 255, 0 };		// GREEN
			FX_AddLine( start, end, 5.0f, 5.0f, 0.0f, 0.5f, 0.5f, color, color, 151, cgi_R_RegisterShader( "gfx/misc/nav_arrow_new" ), 0 );
			break;
		}


		// STEERING
		//=====================================
	case EDGE_IMPACT_SAFE:
		{
			vec3_t	color = { 155, 155, 155 };	// WHITE
			FX_AddLine( start, end, 2.0f, 1.0f, 0.0f, 1.0f, 1.0f, color, color, 151, cgi_R_RegisterShader( "gfx/misc/whiteline2" ), 0 );
		}
		break;
	case EDGE_IMPACT_POSSIBLE:
		{
			vec3_t	color = { 255, 0, 0 };		// RED
			FX_AddLine( start, end, 2.0f, 1.0f, 0.0f, 1.0f, 1.0f, color, color, 151, cgi_R_RegisterShader( "gfx/misc/whiteline2" ), 0 );
		}
		break;
	case EDGE_VELOCITY:
		{
			vec3_t	color = { 0, 255, 0 };		// GREEN
			FX_AddLine( start, end, 4.0f, 1.0f, 0.0f, 1.0f, 1.0f, color, color, 151, cgi_R_RegisterShader( "gfx/misc/whiteline2" ), 0 );
		}
		break;
	case EDGE_THRUST:
		{
			vec3_t	color = { 0, 0, 255 };		// BLUE
			FX_AddLine( start, end, 3.0f, 1.0f, 0.0f, 1.0f, 1.0f, color, color, 151, cgi_R_RegisterShader( "gfx/misc/whiteline2" ), 0 );
		}
		break;


		// MISC Colored Lines
		//=====================================
	case EDGE_WHITE_ONESECOND:
		{
			vec3_t	color = { 155, 155, 155 };	// WHITE
			FX_AddLine( start, end, 3.0f, 1.0f, 0.0f, 1.0f, 1.0f, color, color, 1051, cgi_R_RegisterShader( "gfx/misc/whiteline2" ), 0 );
		}
		break;
	case EDGE_WHITE_TWOSECOND:
		{
			vec3_t	color = { 155, 155, 155 };	// WHITE
			FX_AddLine( start, end, 3.0f, 1.0f, 0.0f, 1.0f, 1.0f, color, color, 1051, cgi_R_RegisterShader( "gfx/misc/whiteline2" ), 0 );
		}
		break;
	case EDGE_RED_ONESECOND:
		{
			vec3_t	color = { 255, 0, 0 };		// RED
			FX_AddLine( start, end, 3.0f, 1.0f, 0.0f, 1.0f, 1.0f, color, color, 2051, cgi_R_RegisterShader( "gfx/misc/whiteline2" ), 0 );
		}
		break;
	case EDGE_RED_TWOSECOND:
		{
			vec3_t	color = { 255, 0, 0 };		// RED
			FX_AddLine( start, end, 3.0f, 1.0f, 0.0f, 1.0f, 1.0f, color, color, 2051, cgi_R_RegisterShader( "gfx/misc/whiteline2" ), 0 );
		}
		break;

	default:
		break;
	}
}

/*
-------------------------
CG_DrawCombatPoint
-------------------------
*/

void CG_DrawCombatPoint( vec3_t origin, int type )
{
	localEntity_t	*ex;

	ex = CG_AllocLocalEntity();

	ex->leType = LE_SPRITE;
	ex->startTime = cg.time;
	ex->radius = 8;
	ex->endTime = ex->startTime + 51;
	VectorCopy( origin, ex->refEntity.origin );
		
	ex->refEntity.customShader = cgi_R_RegisterShader( "gfx/misc/nav_cpoint" );

	ex->color[0] = 255;
	ex->color[1] = 0;
	ex->color[2] = 255;

/*
	switch( type )
	{
	case 0:	//FIXME: To shut up the compiler warning (more will be added here later of course)
	default:
		FX_AddSprite( origin, NULL, NULL, 8.0f, 0.0f, 1.0f, 1.0f, color, color, 0.0f, 0.0f, 51, cgi_R_RegisterShader( "gfx/misc/nav_cpoint" ) );
		break;
	}
*/
}

/*
-------------------------
CG_DrawAlert
-------------------------
*/

void CG_DrawAlert( vec3_t origin, float rating )
{
	vec3_t	drawPos;

	VectorCopy( origin, drawPos );
	drawPos[2] += 48;

	vec3_t	startRGB;

	//Fades from green at 0, to red at 1
	startRGB[0] = rating;
	startRGB[1] = 1 - rating;
	startRGB[2] = 0;

	FX_AddSprite( drawPos, NULL, NULL, 16, 0.0f, 1.0f, 1.0f, startRGB, startRGB, 0, 0, 50, cgs.media.whiteShader );
}


#define MAX_MENUDEFFILE				4096

//
// ==============================
// new hud stuff ( mission pack )
// ==============================
//
qboolean CG_Asset_Parse(const char **p) 
{
	const char *token;
	const char *tempStr;
	int pointSize;

	token = COM_ParseExt(p, qtrue);

	if (!token)
	{
		return qfalse;
	}

	if (Q_stricmp(token, "{") != 0) 
	{
		return qfalse;
	}
    
	while ( 1 ) 
	{
		token = COM_ParseExt(p, qtrue);
		if (!token)
		{
			return qfalse;
		}

		if (Q_stricmp(token, "}") == 0) 
		{
			return qtrue;
		}

		// font
		if (Q_stricmp(token, "font") == 0) 
		{
/*
			int pointSize;

			cgi_UI_Parse_String(tempStr);
			cgi_UI_Parse_Int(&pointSize);

			if (!tempStr || !pointSize) 
			{
				return qfalse;
			}
*/
//			cgDC.registerFont(tempStr, pointSize, &cgDC.Assets.textFont);
			continue;
		}

		// smallFont
		if (Q_stricmp(token, "smallFont") == 0) 
		{
			if (!COM_ParseString(p, &tempStr) || !COM_ParseInt(p, &pointSize)) 
			{
				return qfalse;
			}
//			cgDC.registerFont(tempStr, pointSize, &cgDC.Assets.smallFont);
			continue;
		}

		// smallFont - because the HUD file needs it for MP.
		if (Q_stricmp(token, "small2Font") == 0) 
		{
			if (!COM_ParseString(p, &tempStr) || !COM_ParseInt(p, &pointSize)) 
			{
				return qfalse;
			}
//			cgDC.registerFont(tempStr, pointSize, &cgDC.Assets.smallFont);
			continue;
		}

		// font
		if (Q_stricmp(token, "bigfont") == 0) 
		{
			int pointSize;
			if (!COM_ParseString(p, &tempStr) || !COM_ParseInt(p, &pointSize)) 
			{
				return qfalse;
			}
//			cgDC.registerFont(tempStr, pointSize, &cgDC.Assets.bigFont);
			continue;
		}

		// gradientbar
		if (Q_stricmp(token, "gradientbar") == 0) 
		{
			if (!COM_ParseString(p, &tempStr)) 
			{
				return qfalse;
			}
//			cgDC.Assets.gradientBar = trap_R_RegisterShaderNoMip(tempStr);
			continue;
		}

		// enterMenuSound
		if (Q_stricmp(token, "menuEnterSound") == 0) 
		{
			if (!COM_ParseString(p, &tempStr)) 
			{
				return qfalse;
			}
//			cgDC.Assets.menuEnterSound = trap_S_RegisterSound( tempStr );
			continue;
		}

		// exitMenuSound
		if (Q_stricmp(token, "menuExitSound") == 0) 
		{
			if (!COM_ParseString(p, &tempStr)) 
			{
				return qfalse;
			}
//			cgDC.Assets.menuExitSound = trap_S_RegisterSound( tempStr );
			continue;
		}

		// itemFocusSound
		if (Q_stricmp(token, "itemFocusSound") == 0) 
		{
			if (!COM_ParseString(p, &tempStr)) 
			{
				return qfalse;
			}
//			cgDC.Assets.itemFocusSound = trap_S_RegisterSound( tempStr );
			continue;
		}

		// menuBuzzSound
		if (Q_stricmp(token, "menuBuzzSound") == 0) 
		{
			if (!COM_ParseString(p, &tempStr)) 
			{
				return qfalse;
			}
//			cgDC.Assets.menuBuzzSound = trap_S_RegisterSound( tempStr );
			continue;
		}

		if (Q_stricmp(token, "cursor") == 0) 
		{
//			if (!COM_ParseString(p, &cgDC.Assets.cursorStr)) 
//			{
//				return qfalse;
//			}
//			cgDC.Assets.cursor = trap_R_RegisterShaderNoMip( cgDC.Assets.cursorStr);
			continue;
		}

		if (Q_stricmp(token, "fadeClamp") == 0) 
		{
//			if (!COM_ParseFloat(p, &cgDC.Assets.fadeClamp)) 
//			{
//				return qfalse;
//			}
			continue;
		}

		if (Q_stricmp(token, "fadeCycle") == 0) 
		{
//			if (!COM_ParseInt(p, &cgDC.Assets.fadeCycle)) 
//			{
//				return qfalse;
//			}
			continue;
		}

		if (Q_stricmp(token, "fadeAmount") == 0) 
		{
//			if (!COM_ParseFloat(p, &cgDC.Assets.fadeAmount)) 
//			{
//				return qfalse;
//			}
			continue;
		}

		if (Q_stricmp(token, "shadowX") == 0) 
		{
//			if (!COM_ParseFloat(p, &cgDC.Assets.shadowX)) 
//			{
//				return qfalse;
//			}
			continue;
		}

		if (Q_stricmp(token, "shadowY") == 0) 
		{
//			if (!COM_ParseFloat(p, &cgDC.Assets.shadowY)) 
//			{
//				return qfalse;
//			}
			continue;
		}

		if (Q_stricmp(token, "shadowColor") == 0) 
		{
			/*
			if (!PC_Color_Parse(handle, &cgDC.Assets.shadowColor)) 
			{
				return qfalse;
			}
			cgDC.Assets.shadowFadeClamp = cgDC.Assets.shadowColor[3];
			*/
			continue;
		}
	}
	return qfalse; // bk001204 - why not?
}

void cgi_UI_EndParseSession(char *buf);

/*
=================
CG_ParseMenu();
=================
*/
void CG_ParseMenu(const char *menuFile) 
{
	char			*token;
	int				result;
	char			*buf,*p;

	//Com_Printf("Parsing menu file:%s\n", menuFile);

	result = cgi_UI_StartParseSession((char *) menuFile,&buf);

	if (!result)
	{
		Com_Printf("Unable to load hud menu file:%s. Using default ui/testhud.menu.\n", menuFile);
		result = cgi_UI_StartParseSession("ui/testhud.menu",&buf);
		if (!result)
		{
			Com_Printf("Unable to load default ui/testhud.menu.\n");
			cgi_UI_EndParseSession (buf);
			return;
		}
	}

	p = buf;
	while ( 1 ) 
	{
		cgi_UI_ParseExt(&token);

		if (!*token)	// All done?
		{
			break;
		}

		//if ( Q_stricmp( token, "{" ) ) {
		//	Com_Printf( "Missing { in menu file\n" );
		//	break;
		//}

		//if ( menuCount == MAX_MENUS ) {
		//	Com_Printf( "Too many menus!\n" );
		//	break;
		//}

//		if ( *token == '}' ) 
//		{
//			break;
//		}

		if (Q_stricmp(token, "assetGlobalDef") == 0) 
		{
			/*
			if (CG_Asset_Parse(handle)) 
			{
				continue;
			} 
			else 
			{
				break;
			}
			*/
		}


		if (Q_stricmp(token, "menudef") == 0) 
		{
			// start a new menu
			cgi_UI_Menu_New(p);
		}
	}

	cgi_UI_EndParseSession(buf);

}

/*
=================
CG_Load_Menu();

=================
*/
qboolean CG_Load_Menu( const char **p) 
{
	const char *token;

	token = COM_ParseExt(p, qtrue);

	if (token[0] != '{') 
	{
		return qfalse;
	}

	while ( 1 ) 
	{

		token = COM_ParseExt(p, qtrue);
    
		if (Q_stricmp(token, "}") == 0) 
		{
			return qtrue;
		}

		if ( !token || token[0] == 0 ) 
		{
			return qfalse;
		}

		CG_ParseMenu(token); 
	}
	return qfalse;
}

/*
=================
CG_LoadMenus();

=================
*/
void CG_LoadMenus(const char *menuFile) 
{
	const char	*token;
	const char	*p;
	int	len, start;
	fileHandle_t	f;
	char buf[MAX_MENUDEFFILE];

	start = cgi_Milliseconds();

	len = cgi_FS_FOpenFile( menuFile, &f, FS_READ );
	if ( !f ) 
	{
		CG_Printf( "hud menu file not found: %s, using default\n", menuFile );
		len = cgi_FS_FOpenFile( "ui/jahud.txt", &f, FS_READ );
		if (!f) 
		{
			cgi_Error( S_COLOR_RED "default menu file not found: ui/hud.txt, unable to continue!\n" );
		}
	}

	if ( len >= MAX_MENUDEFFILE ) 
	{
		cgi_FS_FCloseFile( f );
		cgi_Error( va( S_COLOR_RED "menu file too large: %s is %i, max allowed is %i", menuFile, len, MAX_MENUDEFFILE ) );
		return;
	}

	cgi_FS_Read( buf, len, f );
	buf[len] = 0;
	cgi_FS_FCloseFile( f );
	
//	COM_Compress(buf);

//	cgi_UI_Menu_Reset();

	p = buf;

	COM_BeginParseSession();
	while ( 1 ) 
	{
		token = COM_ParseExt( &p, qtrue );
		if( !token || token[0] == 0 || token[0] == '}') 
		{
			break;
		}

		if ( Q_stricmp( token, "}" ) == 0 ) 
		{
			break;
		}

		if (Q_stricmp(token, "loadmenu") == 0) 
		{
			int menuLoad = CG_Load_Menu(&p);
			if (menuLoad) 
			{
				continue;
			} 
			else 
			{
				break;
			}
		}
	}
	COM_EndParseSession();

	//Com_Printf("UI menu load time = %d milli seconds\n", cgi_Milliseconds() - start);
}

/*
=================
CG_LoadHudMenu();

=================
*/
void CG_LoadHudMenu(void) 
{
	const char *hudSet;
/*
	cgDC.registerShaderNoMip = &trap_R_RegisterShaderNoMip;
	cgDC.setColor = &trap_R_SetColor;
	cgDC.drawHandlePic = &CG_DrawPic;
	cgDC.drawStretchPic = &trap_R_DrawStretchPic;
	cgDC.drawText = &CG_Text_Paint;
	cgDC.textWidth = &CG_Text_Width;
	cgDC.textHeight = &CG_Text_Height;
	cgDC.registerModel = &trap_R_RegisterModel;
	cgDC.modelBounds = &trap_R_ModelBounds;
	cgDC.fillRect = &CG_FillRect;
	cgDC.drawRect = &CG_DrawRect;   
	cgDC.drawSides = &CG_DrawSides;
	cgDC.drawTopBottom = &CG_DrawTopBottom;
	cgDC.clearScene = &trap_R_ClearScene;
	cgDC.addRefEntityToScene = &trap_R_AddRefEntityToScene;
	cgDC.renderScene = &trap_R_RenderScene;
	cgDC.registerFont = &trap_R_RegisterFont;
	cgDC.ownerDrawItem = &CG_OwnerDraw;
	cgDC.getValue = &CG_GetValue;
	cgDC.ownerDrawVisible = &CG_OwnerDrawVisible;
	cgDC.runScript = &CG_RunMenuScript;
	cgDC.getTeamColor = &CG_GetTeamColor;
	cgDC.setCVar = trap_Cvar_Set;
	cgDC.getCVarString = trap_Cvar_VariableStringBuffer;
	cgDC.getCVarValue = CG_Cvar_Get;
	cgDC.drawTextWithCursor = &CG_Text_PaintWithCursor;
	cgDC.startLocalSound = &trap_S_StartLocalSound;
	cgDC.ownerDrawHandleKey = &CG_OwnerDrawHandleKey;
	cgDC.feederCount = &CG_FeederCount;
	cgDC.feederItemImage = &CG_FeederItemImage;
	cgDC.feederItemText = &CG_FeederItemText;
	cgDC.feederSelection = &CG_FeederSelection;
	cgDC.Error = &Com_Error; 
	cgDC.Print = &Com_Printf; 
	cgDC.ownerDrawWidth = &CG_OwnerDrawWidth;
	cgDC.registerSound = &trap_S_RegisterSound;
	cgDC.startBackgroundTrack = &trap_S_StartBackgroundTrack;
	cgDC.stopBackgroundTrack = &trap_S_StopBackgroundTrack;
	cgDC.playCinematic = &CG_PlayCinematic;
	cgDC.stopCinematic = &CG_StopCinematic;
	cgDC.drawCinematic = &CG_DrawCinematic;
	cgDC.runCinematicFrame = &CG_RunCinematicFrame;
*/	
//	Init_Display(&cgDC);

//	cgi_UI_String_Init();

//	cgi_UI_Menu_Reset();
	
	hudSet = cg_hudFiles.string;
	if (hudSet[0] == '\0') 
	{
		hudSet = "ui/jahud.txt";
	}

	CG_LoadMenus(hudSet);
}


/*
==============================================================================

INVENTORY SELECTION

==============================================================================
*/

/*
===============
CG_InventorySelectable
===============
*/
static inline qboolean CG_InventorySelectable( int index) 
{
	if (cg.snap->ps.inventory[index])	// Is there any in the inventory?
	{
		return qtrue;
	}

	return qfalse;
}


/*
===============
SetInventoryTime
===============
*/
static inline void SetInventoryTime(void)
{
	if (((cg.weaponSelectTime + WEAPON_SELECT_TIME) > cg.time) ||	// The Weapon HUD was currently active to just swap it out with Force HUD
		((cg.forcepowerSelectTime + WEAPON_SELECT_TIME) > cg.time))	// The Force HUD was currently active to just swap it out with Force HUD
	{
		cg.weaponSelectTime = 0;
		cg.forcepowerSelectTime = 0;
		cg.inventorySelectTime = cg.time + 130.0f;
	}
	else
	{
		cg.inventorySelectTime = cg.time;
	}
}

/*
===============
CG_DPPrevInventory_f
===============
*/
void CG_DPPrevInventory_f( void ) 
{
	int		i;

	if ( !cg.snap ) 
	{
		return;
	}

	const int original = cg.DataPadInventorySelect;

	for ( i = 0 ; i < INV_MAX ; i++ ) 
	{
		cg.DataPadInventorySelect--;

		if ((cg.DataPadInventorySelect < INV_ELECTROBINOCULARS) || (cg.DataPadInventorySelect >= INV_MAX))
		{ 
			cg.DataPadInventorySelect = (INV_MAX - 1); 
		}
		
		if ( CG_InventorySelectable( cg.DataPadInventorySelect ) )
		{	
			return;
		}
	}

	cg.DataPadInventorySelect = original;
}
/*
===============
CG_DPNextInventory_f
===============
*/
void CG_DPNextInventory_f( void ) 
{
	int		i;

	if ( !cg.snap ) 
	{
		return;
	}

	const int original = cg.DataPadInventorySelect;

	for ( i = 0 ; i < INV_MAX ; i++ ) 
	{
		cg.DataPadInventorySelect++;

		if ((cg.DataPadInventorySelect < INV_ELECTROBINOCULARS) || (cg.DataPadInventorySelect >= INV_MAX))
		{ 
			cg.DataPadInventorySelect = INV_ELECTROBINOCULARS; 
		}

		if ( CG_InventorySelectable( cg.DataPadInventorySelect ) && (inv_icons[cg.DataPadInventorySelect])) 
		{	
			return;
		}
	}

	cg.DataPadInventorySelect = original;
}

/*
===============
CG_NextInventory_f
===============
*/
void CG_NextInventory_f( void ) 
{
	int		i;
	float	*color;

	if ( !cg.snap ) 
	{
		return;
	}

	// The first time it's been hit so just show inventory but don't advance in inventory.
	color = CG_FadeColor( cg.inventorySelectTime, WEAPON_SELECT_TIME );	
	if ( !color )	
	{
		SetInventoryTime();
		return;
	}

	const int original = cg.inventorySelect;

	for ( i = 0 ; i < INV_MAX ; i++ ) 
	{
		cg.inventorySelect++;

		if ((cg.inventorySelect < INV_ELECTROBINOCULARS) || (cg.inventorySelect >= INV_MAX))
		{ 
			cg.inventorySelect = INV_ELECTROBINOCULARS; 
		}

		if ( CG_InventorySelectable( cg.inventorySelect ) && (inv_icons[cg.inventorySelect])) 
		{	
			cgi_S_StartSound (NULL, 0, CHAN_AUTO, cgs.media.selectSound2 );
			SetInventoryTime();
			return;
		}
	}

	cg.inventorySelect = original;
}

/*
===============
CG_UseInventory_f
===============
*/
/*
this func was moved to Cmd_UseInventory_f in g_cmds.cpp
*/

/*
===============
CG_PrevInventory_f
===============
*/
void CG_PrevInventory_f( void ) 
{
	int		i;
	float	*color;

	if ( !cg.snap ) 
	{
		return;
	}

	// The first time it's been hit so just show inventory but don't advance in inventory.
	color = CG_FadeColor( cg.inventorySelectTime, WEAPON_SELECT_TIME );	
	if ( !color )	
	{
		SetInventoryTime();
		return;
	}

	const int original = cg.inventorySelect;

	for ( i = 0 ; i < INV_MAX ; i++ ) 
	{
		cg.inventorySelect--;

		if ((cg.inventorySelect < INV_ELECTROBINOCULARS) || (cg.inventorySelect >= INV_MAX))
		{ 
			cg.inventorySelect = (INV_MAX - 1); 
		}
		
		if ( CG_InventorySelectable( cg.inventorySelect ) && (inv_icons[cg.inventorySelect])) 
		{	
			cgi_S_StartSound (NULL, 0, CHAN_AUTO, cgs.media.selectSound2 );
			SetInventoryTime();
			return;
		}
	}

	cg.inventorySelect = original;
}


/*
===================
FindInventoryItemTag
===================
*/
gitem_t *FindInventoryItemTag(int tag)
{
	int		i;

	for ( i = 1 ; i < bg_numItems ; i++ ) 
	{
		if ( bg_itemlist[i].giTag == tag && bg_itemlist[i].giType == IT_HOLDABLE ) // I guess giTag's aren't unique amongst items..must also make sure it's a holdable
		{
			return &bg_itemlist[i];
		}
	}

	return (0);
}




/*
===================
CG_DrawInventorySelect
===================
*/
void CG_DrawInventorySelect( void ) 
{
	int				i;
	int				holdCount,iconCnt;
	int				sideLeftIconCnt,sideRightIconCnt;
	int				count;
	int				holdX;
	int				height;
//	int				tag;
	float			addX;
	vec4_t			textColor = { .312f, .75f, .621f, 1.0f };
	char			text[1024]={0};

	// don't display if dead
	if ( cg.predicted_player_state.stats[STAT_HEALTH] <= 0 || ( cg.snap->ps.viewEntity > 0 && cg.snap->ps.viewEntity < ENTITYNUM_WORLD )) 
	{
		return;
	}

	if ((cg.inventorySelectTime+WEAPON_SELECT_TIME)<cg.time)	// Time is up for the HUD to display
	{
		return;
	}

	int x2,y2,w2,h2;
	if (!cgi_UI_GetMenuInfo("inventoryselecthud",&x2,&y2,&w2,&h2))
	{
		return;
	}

	cg.iconSelectTime = cg.inventorySelectTime;

	// showing weapon select clears pickup item display, but not the blend blob
	cg.itemPickupTime = 0;

	// count the number of items owned
	count = 0;
	for ( i = 0 ; i < INV_MAX ; i++ ) 
	{
		if (CG_InventorySelectable(i) && inv_icons[i]) 
		{
			count++;
		}
	}

	if (!count)
	{
		cgi_SP_GetStringTextString("SP_INGAME_EMPTY_INV",text, sizeof(text) );
		int w = cgi_R_Font_StrLenPixels( text, cgs.media.qhFontSmall, 1.0f );	
		int x = ( SCREEN_WIDTH - w ) / 2;
		CG_DrawProportionalString(x, y2 + 22, text, CG_CENTER | CG_SMALLFONT, colorTable[CT_ICON_BLUE]);
		return;
	}

	const int sideMax = 3;	// Max number of icons on the side

	// Calculate how many icons will appear to either side of the center one
	holdCount = count - 1;	// -1 for the center icon
	if (holdCount == 0)			// No icons to either side
	{
		sideLeftIconCnt = 0;
		sideRightIconCnt = 0;
	}
	else if (count > (2*sideMax))	// Go to the max on each side
	{
		sideLeftIconCnt = sideMax;
		sideRightIconCnt = sideMax;
	}
	else							// Less than max, so do the calc
	{
		sideLeftIconCnt = holdCount/2;
		sideRightIconCnt = holdCount - sideLeftIconCnt;
	}

	i = cg.inventorySelect - 1;
	if (i<0)
	{
		i = INV_MAX-1;
	}

	const int smallIconSize = 40;
	const int bigIconSize = 80;
	const int pad = 16;

	const int x = 320;
	const int y = 410;

	// Left side ICONS
	// Work backwards from current icon
	holdX = x - ((bigIconSize/2) + pad + smallIconSize);
	height = smallIconSize * cg.iconHUDPercent;
	addX = (float) smallIconSize * .75;

	for (iconCnt=0;iconCnt<sideLeftIconCnt;i--)
	{
		if (i<0)
		{
			i = INV_MAX-1;
		}

		if ((!CG_InventorySelectable(i)) || (!inv_icons[i]))
		{
			continue;
		}

		++iconCnt;					// Good icon

		if (inv_icons[i])
		{
			cgi_R_SetColor(NULL);
			CG_DrawPic( holdX, y+10, smallIconSize, smallIconSize, inv_icons[i] );

			cgi_R_SetColor(colorTable[CT_ICON_BLUE]);
			CG_DrawNumField (holdX + addX, y + smallIconSize, 2, cg.snap->ps.inventory[i], 6, 12, 
				NUM_FONT_SMALL,qfalse);

			holdX -= (smallIconSize+pad);
		}
	}

	// Current Center Icon
	height = bigIconSize * cg.iconHUDPercent;
	if (inv_icons[cg.inventorySelect])
	{
		cgi_R_SetColor(NULL);
		CG_DrawPic( x-(bigIconSize/2), (y-((bigIconSize-smallIconSize)/2))+10, bigIconSize, bigIconSize, inv_icons[cg.inventorySelect] );
		addX = (float) bigIconSize * .75;
		cgi_R_SetColor(colorTable[CT_ICON_BLUE]);
		CG_DrawNumField ((x-(bigIconSize/2)) + addX, y, 2, cg.snap->ps.inventory[cg.inventorySelect], 6, 12, 
			NUM_FONT_SMALL,qfalse);

		if (inv_names[cg.inventorySelect])
		{
			// FIXME: This is ONLY a temp solution, the icon stuff, etc, should all just use items.dat for everything
			gitem_t *item = FindInventoryItemTag( cg.inventorySelect );

			if ( item && item->classname && item->classname[0] )
			{
				char itemName[256], data[1024]; // FIXME: do these really need to be this large??  does it matter?

				sprintf( itemName, "SP_INGAME_%s",	item->classname );
			
				if ( cgi_SP_GetStringTextString( itemName, data, sizeof( data )))
				{
					int w = cgi_R_Font_StrLenPixels( data, cgs.media.qhFontSmall, 1.0f );	
					int x = ( SCREEN_WIDTH - w ) / 2;

					cgi_R_Font_DrawString( x, (SCREEN_HEIGHT - 24), data, textColor, cgs.media.qhFontSmall, -1, 1.0f);
				}
				// ADDED FOR WEAPON CODE 64
				else
				{
					Com_sprintf( itemName, sizeof(itemName), "SPMOD_INGAME_%s",	item->classname );
					if ( cgi_SP_GetStringTextString( itemName, data, sizeof( data )))
					{
						int w = cgi_R_Font_StrLenPixels( data, cgs.media.qhFontSmall, 1.0f );
						int x = ( SCREEN_WIDTH - w ) / 2;

						cgi_R_Font_DrawString( x, (SCREEN_HEIGHT - 24), data, textColor, cgs.media.qhFontSmall, -1, 1.0f);
					}
				}
			}
		}
	}

	i = cg.inventorySelect + 1;
	if (i> INV_MAX-1)
	{
		i = 0;
	}

	// Right side ICONS
	// Work forwards from current icon
	holdX = x + (bigIconSize/2) + pad;
	height = smallIconSize * cg.iconHUDPercent;
	addX = (float) smallIconSize * .75;
	for (iconCnt=0;iconCnt<sideRightIconCnt;i++)
	{
		if (i> INV_MAX-1)
		{
			i = 0;
		}

		if ((!CG_InventorySelectable(i)) || (!inv_icons[i]))
		{
			continue;
		}

		++iconCnt;					// Good icon

		if (inv_icons[i])
		{
			cgi_R_SetColor(NULL);
			CG_DrawPic( holdX, y+10, smallIconSize, smallIconSize, inv_icons[i] );

			cgi_R_SetColor(colorTable[CT_ICON_BLUE]);
			CG_DrawNumField (holdX + addX, y + smallIconSize, 2, cg.snap->ps.inventory[i], 6, 12, 
				NUM_FONT_SMALL,qfalse);

			holdX += (smallIconSize+pad);
		}
	}
}

int cgi_UI_GetItemText(char *menuFile,char *itemName, char *text);

const char *inventoryDesc[15] = 
{
"NEURO_SAAV_DESC",
"BACTA_DESC",
"INQUISITOR_DESC",
"LA_GOGGLES_DESC",
"PORTABLE_SENTRY_DESC",
"GOODIE_KEY_DESC",
"SECURITY_KEY_DP_DESC",
};


/*
===================
CG_DrawDataPadInventorySelect
===================
*/
void CG_DrawDataPadInventorySelect( void ) 
{
	int				i;
	int				holdCount,iconCnt;
	int				sideLeftIconCnt,sideRightIconCnt;
	int				count;
	int				holdX;
	int				height;
	float			addX;
	char			text[1024]={0};
	vec4_t			textColor = { .312f, .75f, .621f, 1.0f };


	// count the number of items owned
	count = 0;
	for ( i = 0 ; i < INV_MAX ; i++ ) 
	{
		if (CG_InventorySelectable(i) && inv_icons[i]) 
		{
			count++;
		}
	}


	if (!count)
	{
		cgi_SP_GetStringTextString("SP_INGAME_EMPTY_INV",text, sizeof(text) );
		int w = cgi_R_Font_StrLenPixels( text, cgs.media.qhFontSmall, 1.0f );	
		int x = ( SCREEN_WIDTH - w ) / 2;
		CG_DrawProportionalString(x, 300 + 22, text, CG_CENTER | CG_SMALLFONT, colorTable[CT_ICON_BLUE]);
		return;
	}

	const int sideMax = 3;	// Max number of icons on the side

	// Calculate how many icons will appear to either side of the center one
	holdCount = count - 1;	// -1 for the center icon
	if (holdCount == 0)			// No icons to either side
	{
		sideLeftIconCnt = 0;
		sideRightIconCnt = 0;
	}
	else if (count > (2*sideMax))	// Go to the max on each side
	{
		sideLeftIconCnt = sideMax;
		sideRightIconCnt = sideMax;
	}
	else							// Less than max, so do the calc
	{
		sideLeftIconCnt = holdCount/2;
		sideRightIconCnt = holdCount - sideLeftIconCnt;
	}

	i = cg.DataPadInventorySelect - 1;
	if (i<0)
	{
		i = INV_MAX-1;
	}


	const int smallIconSize = 40;
	const int bigIconSize = 80;
	const int bigPad = 64;
	const int pad = 32;

	const int centerXPos = 320;
	const int graphicYPos = 340;


	// Left side ICONS
	// Work backwards from current icon
	holdX = centerXPos - ((bigIconSize/2) + bigPad + smallIconSize);
	height = smallIconSize * cg.iconHUDPercent;
	addX = (float) smallIconSize * .75;

	for (iconCnt=0;iconCnt<sideLeftIconCnt;i--)
	{
		if (i<0)
		{
			i = INV_MAX-1;
		}

		if ((!CG_InventorySelectable(i)) || (!inv_icons[i]))
		{
			continue;
		}

		++iconCnt;					// Good icon

		if (inv_icons[i])
		{
			cgi_R_SetColor(colorTable[CT_WHITE]);
			CG_DrawPic( holdX, graphicYPos+10, smallIconSize, smallIconSize, inv_icons[i] );

			cgi_R_SetColor(colorTable[CT_ICON_BLUE]);
			CG_DrawNumField (holdX + addX, graphicYPos + smallIconSize, 2, cg.snap->ps.inventory[i], 6, 12, 
				NUM_FONT_SMALL,qfalse);

			holdX -= (smallIconSize+pad);
		}
	}

	// Current Center Icon
	height = bigIconSize * cg.iconHUDPercent;
	if (inv_icons[cg.DataPadInventorySelect])
	{
		cgi_R_SetColor(colorTable[CT_WHITE]);
		CG_DrawPic( centerXPos-(bigIconSize/2), (graphicYPos-((bigIconSize-smallIconSize)/2))+10, bigIconSize, bigIconSize, inv_icons[cg.DataPadInventorySelect] );
		addX = (float) bigIconSize * .75;
		cgi_R_SetColor(colorTable[CT_ICON_BLUE]);
		CG_DrawNumField ((centerXPos-(bigIconSize/2)) + addX, graphicYPos, 2, cg.snap->ps.inventory[cg.DataPadInventorySelect], 6, 12, 
			NUM_FONT_SMALL,qfalse);

	}

	i = cg.DataPadInventorySelect + 1;
	if (i> INV_MAX-1)
	{
		i = 0;
	}

	// Right side ICONS
	// Work forwards from current icon
	holdX = centerXPos + (bigIconSize/2) + bigPad;
	height = smallIconSize * cg.iconHUDPercent;
	addX = (float) smallIconSize * .75;
	for (iconCnt=0;iconCnt<sideRightIconCnt;i++)
	{
		if (i> INV_MAX-1)
		{
			i = 0;
		}

		if ((!CG_InventorySelectable(i)) || (!inv_icons[i]))
		{
			continue;
		}

		++iconCnt;					// Good icon

		if (inv_icons[i])
		{
			cgi_R_SetColor(colorTable[CT_WHITE]);
			CG_DrawPic( holdX, graphicYPos+10, smallIconSize, smallIconSize, inv_icons[i] );

			cgi_R_SetColor(colorTable[CT_ICON_BLUE]);
			CG_DrawNumField (holdX + addX, graphicYPos + smallIconSize, 2, cg.snap->ps.inventory[i], 6, 12, 
				NUM_FONT_SMALL,qfalse);

			holdX += (smallIconSize+pad);
		}
	}

	// draw the weapon description
	if ((cg.DataPadInventorySelect>=0) && (cg.DataPadInventorySelect<13))
	{
		//cgi_SP_GetStringTextString( va("SP_INGAME_%s",inventoryDesc[cg.DataPadInventorySelect]), text, sizeof(text) );
		if (!cgi_SP_GetStringTextString( va("SP_INGAME_%s",inventoryDesc[cg.DataPadInventorySelect]), text, sizeof(text) ))
		{
			cgi_SP_GetStringTextString( va("SPMOD_INGAME_%s",inventoryDesc[cg.DataPadInventorySelect]), text, sizeof(text) );
		}



		if (text[0])
		{
			CG_DisplayBoxedText(70,50,500,300,text,
											cgs.media.qhFontSmall,
											0.7f,
											textColor	
											);
		}
	}
}

/*
===============
SetForcePowerTime
===============
*/
void SetForcePowerTime(void)
{
	if (((cg.weaponSelectTime + WEAPON_SELECT_TIME) > cg.time) ||	// The Weapon HUD was currently active to just swap it out with Force HUD
		((cg.inventorySelectTime + WEAPON_SELECT_TIME) > cg.time))	// The Inventory HUD was currently active to just swap it out with Force HUD
	{
		cg.weaponSelectTime = 0;
		cg.inventorySelectTime = 0;
		cg.forcepowerSelectTime = cg.time + 130.0f;
	}
	else
	{
		cg.forcepowerSelectTime = cg.time;
	}
}

int showPowers[MAX_SHOWPOWERS] = 
{
	FP_ABSORB,
	FP_HEAL,
	FP_PROTECT,
	FP_TELEPATHY,

	FP_SPEED,
	FP_PUSH,
	FP_PULL,
	FP_SEE,

	FP_DRAIN,
	FP_LIGHTNING,
	FP_RAGE,
	FP_GRIP,

	FP_STUN,
	FP_HATE,
	FP_CONTROLMIND,
	FP_FREEZE,
	FP_HEALOTHER,
	FP_WRACK,
	FP_FORCEGLYPH,
	FP_STONEGLYPH,
	FP_SOUNDGLYPH,
	FP_WATERGLYPH,
	FP_FIREGLYPH,
	FP_SUNGLYPH,
	FP_WINDGLYPH,
	FP_DARKGLYPH,
	FP_HOLYGLYPH,
	FP_NECROGLYPH,
};

// ALL FORCE POWERS NAMES 
// Reavers default 
const char *showPowersName[MAX_SHOWPOWERS] = 
{
	"SP_INGAME_ABSORB2",
	"SP_INGAME_HEAL2",
	"SP_INGAME_PROTECT2",
	"SP_INGAME_MINDTRICK2",

	"SP_INGAME_SPEED2",
	"SP_INGAME_PUSH2",
	"SP_INGAME_PULL2",
	"SP_INGAME_REVEL2",

	"SP_INGAME_SOULDRAIN2",
	"SP_INGAME_THUNDERBOLT2",
	"SP_INGAME_TRANCE2",
	"SP_INGAME_GRIP2",

	"SP_INGAME_STUN2",
	"SP_INGAME_HATE2",
	"SP_INGAME_CONTROLMIND2",
	"SP_INGAME_FROSTCOFFIN2",
	"SP_INGAME_HEALOTHER2",
	"SP_INGAME_WRACK2",

	"SP_INGAME_FORCEGLYPH2",
	"SP_INGAME_EARTHREAVER2",
	"SP_INGAME_SOUNDGLYPH2",
	"SP_INGAME_WATERREAVER2",
	"SP_INGAME_FIREREAVER2",
	"SP_INGAME_LIGHTREAVER2",
	"SP_INGAME_AIRREAVER2",
	"SP_INGAME_DARKREAVER2",
	"SP_INGAME_MATREAVER2",
	"SP_INGAME_SPECTRALREAVER2",
};
// Shifter 
const char *showPowersName1[MAX_SHOWPOWERS] = 
{
	"SP_INGAME_ABSORB2",
	"SP_INGAME_HEAL2",
	"SP_INGAME_PROTECT2",
	"SP_INGAME_PROXY2",

	"SP_INGAME_SPEED2",
	"SP_INGAME_PUSH2",
	"SP_INGAME_PULL2",
	"SP_INGAME_REVEL2",

	"SP_INGAME_SOULDRAIN2",
	"SP_INGAME_DARKNESS2",
	"SP_INGAME_CRUEL2",
	"SP_INGAME_GRIP2",

	"SP_INGAME_FEARTRICK2",
	"SP_INGAME_HATE2",
	"SP_INGAME_CONTROLMIND2",
	"SP_INGAME_FREEZE2",
	"SP_INGAME_HEALOTHER2",
	"SP_INGAME_WRACK2",

	"SP_INGAME_FORCEGLYPH2",
	"SP_INGAME_EARTHREAVER2",
	"SP_INGAME_SOUNDGLYPH2",
	"SP_INGAME_WATERREAVER2",
	"SP_INGAME_FIREREAVER2",
	"SP_INGAME_LIGHTREAVER2",
	"SP_INGAME_AIRREAVER2",
	"SP_INGAME_DARKREAVER2",
	"SP_INGAME_MATREAVER2",
	"SP_INGAME_SPECTRALREAVER2",
};
// Bleed
const char *showPowersName2[MAX_SHOWPOWERS] = 
{
	"SP_INGAME_ABSORB2",
	"SP_INGAME_HEAL2",
	"SP_INGAME_PROTECT2",
	"SP_INGAME_MINDTRICK2",

	"SP_INGAME_SPEED2",
	"SP_INGAME_PUSH2",
	"SP_INGAME_PULL2",
	"SP_INGAME_REVEL2",

	"SP_INGAME_SOULDRAIN2",
	"SP_INGAME_SMOG2",
	"SP_INGAME_TRANCE2",
	"SP_INGAME_GRIP2",

	"SP_INGAME_STUN2",
	"SP_INGAME_HATE2",
	"SP_INGAME_CONTROLMIND2",
	"SP_INGAME_FREEZE2",
	"SP_INGAME_HEALOTHER2",
	"SP_INGAME_WRACK2",

	"SP_INGAME_FORCEGLYPH2",
	"SP_INGAME_EARTHREAVER2",
	"SP_INGAME_SOUNDGLYPH2",
	"SP_INGAME_WATERREAVER2",
	"SP_INGAME_FIREREAVER2",
	"SP_INGAME_LIGHTREAVER2",
	"SP_INGAME_AIRREAVER2",
	"SP_INGAME_SMOGREAVER2",
	"SP_INGAME_MATREAVER2",
	"SP_INGAME_SPECTRALREAVER2",
};
// Shifter VOid 
const char *showPowersName3[MAX_SHOWPOWERS] = 
{
	"SP_INGAME_ABSORB2",
	"SP_INGAME_HEALD2",
	"SP_INGAME_PROTECT2",
	"SP_INGAME_PROXY2",

	"SP_INGAME_SPEED2",
	"SP_INGAME_PUSH2",
	"SP_INGAME_PULL2",
	"SP_INGAME_REVEL2",

	"SP_INGAME_NECRODRAIN2",
	"SP_INGAME_VOID2",
	"SP_INGAME_CRUEL2",
	"SP_INGAME_GRIP2",

	"SP_INGAME_FEARTRICK2",
	"SP_INGAME_HATE2",
	"SP_INGAME_CONTROLMIND2",
	"SP_INGAME_FREEZE2",
	"SP_INGAME_HEALOTHER2",
	"SP_INGAME_WRACK2",

	"SP_INGAME_FORCEGLYPH2",
	"SP_INGAME_STONEGLYPH2",
	"SP_INGAME_SOUNDGLYPH2",
	"SP_INGAME_WATERREAVER2",
	"SP_INGAME_FIREREAVER2",
	"SP_INGAME_LIGHTREAVER2",
	"SP_INGAME_AIRREAVER2",
	"SP_INGAME_VOIDREAVER2",
	"SP_INGAME_MATREAVER2",
	"SP_INGAME_SPECTRALREAVER2",
};
 // Tiziel lighntning 
const char *showPowersName4[MAX_SHOWPOWERS] = 
{
	"SP_INGAME_ABSORB2",
	"SP_INGAME_HEAL2",
	"SP_INGAME_PROTECT2",
	"SP_INGAME_MINDTRICK2",

	"SP_INGAME_SPEED2",
	"SP_INGAME_PUSH2",
	"SP_INGAME_PULL2",
	"SP_INGAME_REVEL2",

	"SP_INGAME_SOULDRAIN2",
	"SP_INGAME_YELLOWLIGHTNING2",
	"SP_INGAME_THUNDERSHIELD2",
	"SP_INGAME_GRIP2",

	"SP_INGAME_STUN2",
	"SP_INGAME_HATE2",
	"SP_INGAME_CONTROLMIND2",
	"SP_INGAME_THUNDERNET2",
	"SP_INGAME_HEALOTHER2",
	"SP_INGAME_WRACK2",

	"SP_INGAME_FORCEGLYPH2",
	"SP_INGAME_STONEGLYPH2",
	"SP_INGAME_SOUNDGLYPH2",
	"SP_INGAME_WATERREAVER2",
	"SP_INGAME_FIREREAVER2",
	"SP_INGAME_LIGHTNINGREAVER2",
	"SP_INGAME_AIRREAVER2",
	"SP_INGAME_DARKREAVER2",
	"SP_INGAME_MATREAVER2",
	"SP_INGAME_SPECTRALREAVER2",
};
// Tiziel sun reaver
const char *showPowersName5[MAX_SHOWPOWERS] = 
{
	"SP_INGAME_ABSORB2",
	"SP_INGAME_HEAL2",
	"SP_INGAME_PROTECT2",
	"SP_INGAME_MINDTRICK2",

	"SP_INGAME_SPEED2",
	"SP_INGAME_PUSH2",
	"SP_INGAME_PULL2",
	"SP_INGAME_REVEL2",

	"SP_INGAME_DEMONDRAIN2",
	"SP_INGAME_BURST2",
	"SP_INGAME_ELDERSHIELD2",
	"SP_INGAME_GRIP2",

	"SP_INGAME_STUN2",
	"SP_INGAME_HATE2",
	"SP_INGAME_CONTROLMIND2",
	"SP_INGAME_THUNDERNET2",
	"SP_INGAME_HEALOTHER2",
	"SP_INGAME_WRACK2",

	"SP_INGAME_FORCEGLYPH2",
	"SP_INGAME_STONEGLYPH2",
	"SP_INGAME_SOUNDGLYPH2",
	"SP_INGAME_WATERREAVER2",
	"SP_INGAME_FIREREAVER2",
	"SP_INGAME_SUNREAVER2",
	"SP_INGAME_AIRREAVER2",
	"SP_INGAME_DARKREAVER2",
	"SP_INGAME_MATREAVER2",
	"SP_INGAME_SPECTRALREAVER2",
};
// Kaneel warm
const char *showPowersName6[MAX_SHOWPOWERS] = 
{
	"SP_INGAME_ABSORB2",
	"SP_INGAME_HEAL2",
	"SP_INGAME_PROTECT2",
	"SP_INGAME_MINDTRICK2",

	"SP_INGAME_SPEED2",
	"SP_INGAME_PUSH2",
	"SP_INGAME_PULL2",
	"SP_INGAME_ECHO2",

	"SP_INGAME_SOULDRAIN2",
	"SP_INGAME_FIRE2",
	"SP_INGAME_FIRE_RAGE2",
	"SP_INGAME_GRIP2",

	"SP_INGAME_STUN2",
	"SP_INGAME_HATE2",
	"SP_INGAME_CONTROLMIND2",
	"SP_INGAME_FREEZE2",
	"SP_INGAME_HEALOTHER2",
	"SP_INGAME_WRACK2",

	"SP_INGAME_FORCEGLYPH2",
	"SP_INGAME_EARTHREAVER2",
	"SP_INGAME_SOUNDGLYPH2",
	"SP_INGAME_WATERREAVER2",
	"SP_INGAME_WARMREAVER2",
	"SP_INGAME_LIGHTREAVER2",
	"SP_INGAME_AIRREAVER2",
	"SP_INGAME_DARKREAVER2",
	"SP_INGAME_MATREAVER2",
	"SP_INGAME_SPECTRALREAVER2",
};
// Kaneel Lava
const char *showPowersName7[MAX_SHOWPOWERS] = 
{
	"SP_INGAME_ABSORB2",
	"SP_INGAME_HEALFIRE2",
	"SP_INGAME_PROTECT2",
	"SP_INGAME_MINDTRICK2",

	"SP_INGAME_SPEED2",
	"SP_INGAME_PUSH2",
	"SP_INGAME_PULL2",
	"SP_INGAME_ECHO2",

	"SP_INGAME_DRAIN2",
	"SP_INGAME_LAVA2",
	"SP_INGAME_FIRE_RAGE2",
	"SP_INGAME_GRIP2",

	"SP_INGAME_STUN2",
	"SP_INGAME_HATE2",
	"SP_INGAME_CONTROLMIND2",
	"SP_INGAME_FREEZE2",
	"SP_INGAME_HEALOTHER2",
	"SP_INGAME_WRACK2",

	"SP_INGAME_FORCEGLYPH2",
	"SP_INGAME_EARTHREAVER2",
	"SP_INGAME_SOUNDGLYPH2",
	"SP_INGAME_WATERREAVER2",
	"SP_INGAME_MAGMAREAVER2",
	"SP_INGAME_LIGHTREAVER2",
	"SP_INGAME_AIRREAVER2",
	"SP_INGAME_SMOGREAVER2",
	"SP_INGAME_MATREAVER2",
	"SP_INGAME_SPECTRALREAVER2",
};
// Naeryan
const char *showPowersName8[MAX_SHOWPOWERS] = 
{
	"SP_INGAME_ABSORB2",
	"SP_INGAME_HEAL2",
	"SP_INGAME_PROTECT2",
	"SP_INGAME_MINDTRICK2",

	"SP_INGAME_SPEED2",
	"SP_INGAME_PUSH2",
	"SP_INGAME_PULL2",
	"SP_INGAME_REVEL2",

	"SP_INGAME_SOULDRAIN2",
	"SP_INGAME_WIND2",
	"SP_INGAME_WINDSHIELD2",
	"SP_INGAME_GRIP2",

	"SP_INGAME_ETERFORM2",
	"SP_INGAME_HATE2",
	"SP_INGAME_CONTROLMIND2",
	"SP_INGAME_FREEZE2",
	"SP_INGAME_HEALOTHER2",
	"SP_INGAME_WRACK2",

	"SP_INGAME_FORCEGLYPH2",
	"SP_INGAME_EARTHREAVER2",
	"SP_INGAME_SOUNDGLYPH2",
	"SP_INGAME_WATERREAVER2",
	"SP_INGAME_FIREREAVER2",
	"SP_INGAME_LIGHTREAVER2",
	"SP_INGAME_AIRREAVER2",
	"SP_INGAME_DARKREAVER2",
	"SP_INGAME_MATREAVER2",
	"SP_INGAME_SPECTRALREAVER2",
};
// Naeryan Fog 
const char *showPowersName9[MAX_SHOWPOWERS] = 
{
	"SP_INGAME_ABSORB2",
	"SP_INGAME_HEAL2",
	"SP_INGAME_PROTECT2",
	"SP_INGAME_MINDTRICK2",

	"SP_INGAME_SPEED2",
	"SP_INGAME_PUSH2",
	"SP_INGAME_PULL2",
	"SP_INGAME_REVEL2",

	"SP_INGAME_SOULDRAIN2",
	"SP_INGAME_WIND2",
	"SP_INGAME_WINDSHIELD2",
	"SP_INGAME_GRIP2",

	"SP_INGAME_ETERFORM2",
	"SP_INGAME_HATE2",
	"SP_INGAME_CONTROLMIND2",
	"SP_INGAME_FREEZE2",
	"SP_INGAME_HEALOTHER2",
	"SP_INGAME_WRACK2",

	"SP_INGAME_FORCEGLYPH2",
	"SP_INGAME_EARTHREAVER2",
	"SP_INGAME_SOUNDGLYPH2",
	"SP_INGAME_WATERREAVER2",
	"SP_INGAME_FIREREAVER2",
	"SP_INGAME_LIGHTREAVER2",
	"SP_INGAME_FOGREAVER2",
	"SP_INGAME_DARKREAVER2",
	"SP_INGAME_MATREAVER2",
	"SP_INGAME_SPECTRALREAVER2",
};
// Naeryan Storm
const char *showPowersName10[MAX_SHOWPOWERS] = 
{
	"SP_INGAME_ABSORB2",
	"SP_INGAME_HEAL2",
	"SP_INGAME_PROTECT2",
	"SP_INGAME_MINDTRICK2",

	"SP_INGAME_SPEED2",
	"SP_INGAME_PUSH2",
	"SP_INGAME_PULL2",
	"SP_INGAME_REVEL2",

	"SP_INGAME_SOULDRAIN2",
	"SP_INGAME_WIND3",
	"SP_INGAME_STORMSHIELD2",
	"SP_INGAME_GRIP2",

	"SP_INGAME_ETERFORM2",
	"SP_INGAME_HATE2",
	"SP_INGAME_CONTROLMIND2",
	"SP_INGAME_FREEZE2",
	"SP_INGAME_HEALOTHER2",
	"SP_INGAME_WRACK2",

	"SP_INGAME_FORCEGLYPH2",
	"SP_INGAME_EARTHREAVER2",
	"SP_INGAME_SOUNDGLYPH2",
	"SP_INGAME_WATERREAVER2",
	"SP_INGAME_FIREREAVER2",
	"SP_INGAME_LIGHTREAVER2",
	"SP_INGAME_STORMREAVER2",
	"SP_INGAME_DARKREAVER2",
	"SP_INGAME_MATREAVER2",
	"SP_INGAME_SPECTRALREAVER2",
};
// Seed
const char *showPowersName11[MAX_SHOWPOWERS] = 
{
	"SP_INGAME_ABSORB2",
	"SP_INGAME_HEALV2",
	"SP_INGAME_PROTECT2",
	"SP_INGAME_WOLFFORM2",

	"SP_INGAME_SPEED",
	"SP_INGAME_PUSH2",
	"SP_INGAME_PULL2",
	"SP_INGAME_SEEING2",

	"SP_INGAME_DRAIN2",
	"SP_INGAME_SONIC2",
	"SP_INGAME_DARK_RAGE2",
	"SP_INGAME_GRIP2",

	"SP_INGAME_STUN2",
	"SP_INGAME_HATE2",
	"SP_INGAME_CONTROLMIND2",
	"SP_INGAME_FREEZE2",
	"SP_INGAME_BATFORM2",
	"SP_INGAME_WRACK2",

	"SP_INGAME_MISTFORM2",
	"SP_INGAME_TIMEREAVER2",
	"SP_INGAME_DIMENSIONREAVER2",
	"SP_INGAME_STATESREAVER2",
	"SP_INGAME_CONFLICTREAVER2",
	"SP_INGAME_ENERGYREAVER2",
	"SP_INGAME_MINDREAVER2",
	"SP_INGAME_NATUREREAVER2",
	"SP_INGAME_SOULREAVER2",
	"SP_INGAME_BLOODREAVER2",
};
// Rekius 
const char *showPowersName12[MAX_SHOWPOWERS] = 
{
	"SP_INGAME_ABSORB2",
	"SP_INGAME_HEAL2",
	"SP_INGAME_PROTECT2",
	"SP_INGAME_MINDTRICK2",

	"SP_INGAME_SPEED2",
	"SP_INGAME_PUSH2",
	"SP_INGAME_PULL2",
	"SP_INGAME_REVEL2",

	"SP_INGAME_NECRODRAIN2",
	"SP_INGAME_ICE2",
	"SP_INGAME_FROSTARMOR2",
	"SP_INGAME_GRIP2",

	"SP_INGAME_STUN2",
	"SP_INGAME_HATE2",
	"SP_INGAME_CONTROLMIND2",
	"SP_INGAME_FROSTCOFFIN2",
	"SP_INGAME_HEALOTHER2",
	"SP_INGAME_WRACK2",

	"SP_INGAME_FORCEGLYPH2",
	"SP_INGAME_EARTHREAVER2",
	"SP_INGAME_SOUNDGLYPH2",
	"SP_INGAME_ICEREAVER2",
	"SP_INGAME_FIREREAVER2",
	"SP_INGAME_LIGHTREAVER2",
	"SP_INGAME_AIRREAVER2",
	"SP_INGAME_DARKREAVER2",
	"SP_INGAME_MATREAVER2",
	"SP_INGAME_SPECTRALREAVER2",
};
// Rekius Crystal 
const char *showPowersName13[MAX_SHOWPOWERS] = 
{
	"SP_INGAME_ABSORB2",
	"SP_INGAME_HEAL2",
	"SP_INGAME_PROTECT2",
	"SP_INGAME_MINDTRICK2",

	"SP_INGAME_SPEED2",
	"SP_INGAME_PUSH2",
	"SP_INGAME_PULL2",
	"SP_INGAME_REVEL2",

	"SP_INGAME_NECRODRAIN2",
	"SP_INGAME_CRYSTAL2",
	"SP_INGAME_CRYSTALARMOR2",
	"SP_INGAME_GRIP2",

	"SP_INGAME_STUN2",
	"SP_INGAME_HATE2",
	"SP_INGAME_CONTROLMIND2",
	"SP_INGAME_FROSTCOFFIN2",
	"SP_INGAME_HEALOTHER2",
	"SP_INGAME_WRACK2",

	"SP_INGAME_FORCEGLYPH2",
	"SP_INGAME_EARTHREAVER2",
	"SP_INGAME_SOUNDGLYPH2",
	"SP_INGAME_CRYSTALREAVER2",
	"SP_INGAME_FIREREAVER2",
	"SP_INGAME_LIGHTREAVER2",
	"SP_INGAME_AIRREAVER2",
	"SP_INGAME_DARKREAVER2",
	"SP_INGAME_MATREAVER2",
	"SP_INGAME_SPECTRALREAVER2",
};
// Xado
const char *showPowersName14[MAX_SHOWPOWERS] = 
{
	"SP_INGAME_ABSORB2",
	"SP_INGAME_HEAL2",
	"SP_INGAME_PROTECT2",
	"SP_INGAME_MINDTRICK2",

	"SP_INGAME_SPEED2",
	"SP_INGAME_PUSH2",
	"SP_INGAME_PULL2",
	"SP_INGAME_REVEL2",

	"SP_INGAME_SOULDRAIN2",
	"SP_INGAME_WEB2",
	"SP_INGAME_TRANCE2",
	"SP_INGAME_GRIP2",

	"SP_INGAME_STUN2",
	"SP_INGAME_HATE2",
	"SP_INGAME_CONTROLMIND2",
	"SP_INGAME_WEBBED2",
	"SP_INGAME_HEALOTHER2",
	"SP_INGAME_WRACK2",

	"SP_INGAME_FORCEGLYPH2",
	"SP_INGAME_EARTHREAVER2",
	"SP_INGAME_SOUNDGLYPH2",
	"SP_INGAME_WATERREAVER2",
	"SP_INGAME_FIREREAVER2",
	"SP_INGAME_LIGHTREAVER2",
	"SP_INGAME_AIRREAVER2",
	"SP_INGAME_DARKREAVER2",
	"SP_INGAME_MATREAVER2",
	"SP_INGAME_SPECTRALREAVER2",
};
// Xado Dust and Mud
const char *showPowersName15[MAX_SHOWPOWERS] = 
{
	"SP_INGAME_ABSORB2",
	"SP_INGAME_HEAL2",
	"SP_INGAME_PROTECT2",
	"SP_INGAME_MINDTRICK2",

	"SP_INGAME_SPEED2",
	"SP_INGAME_PUSH2",
	"SP_INGAME_PULL2",
	"SP_INGAME_REVEL2",

	"SP_INGAME_SOULDRAIN2",
	"SP_INGAME_WEB2",
	"SP_INGAME_TRANCE2",
	"SP_INGAME_GRIP2",

	"SP_INGAME_STUN2",
	"SP_INGAME_HATE2",
	"SP_INGAME_CONTROLMIND2",
	"SP_INGAME_WEBBED2",
	"SP_INGAME_HEALOTHER2",
	"SP_INGAME_WRACK2",

	"SP_INGAME_FORCEGLYPH2",
	"SP_INGAME_DUSTREAVER2",
	"SP_INGAME_SOUNDGLYPH2",
	"SP_INGAME_WATERREAVER2",
	"SP_INGAME_FIREREAVER2",
	"SP_INGAME_LIGHTREAVER2",
	"SP_INGAME_AIRREAVER2",
	"SP_INGAME_DARKREAVER2",
	"SP_INGAME_MATREAVER2",
	"SP_INGAME_MUDREAVER2",
};

// Anteo
const char *showPowersName16[MAX_SHOWPOWERS] = 
{
	"SP_INGAME_ABSORB2",
	"SP_INGAME_HEAL2",
	"SP_INGAME_PROTECT2",
	"SP_INGAME_MINDTRICK2",

	"SP_INGAME_SPEED2",
	"SP_INGAME_PUSH2",
	"SP_INGAME_PULL2",
	"SP_INGAME_REVEL2",

	"SP_INGAME_SOULDRAIN2",
	"SP_INGAME_STONE2",
	"SP_INGAME_RAGE2",
	"SP_INGAME_GRIP2",

	"SP_INGAME_STUN2",
	"SP_INGAME_HATE2",
	"SP_INGAME_CONTROLMIND2",
	"SP_INGAME_STONED2",
	"SP_INGAME_HEALOTHER2",
	"SP_INGAME_WRACK2",

	"SP_INGAME_FORCEGLYPH2",
	"SP_INGAME_STONEREAVER2",
	"SP_INGAME_SOUNDGLYPH2",
	"SP_INGAME_WATERREAVER2",
	"SP_INGAME_FIREREAVER2",
	"SP_INGAME_LIGHTREAVER2",
	"SP_INGAME_AIRREAVER2",
	"SP_INGAME_DARKREAVER2",
	"SP_INGAME_MATREAVER2",
	"SP_INGAME_SPECTRALREAVER2",
};
// Xado Poison
const char *showPowersName17[MAX_SHOWPOWERS] = 
{
	"SP_INGAME_ABSORB2",
	"SP_INGAME_HEAL2",
	"SP_INGAME_PROTECT2",
	"SP_INGAME_MINDTRICK2",

	"SP_INGAME_SPEED2",
	"SP_INGAME_PUSH2",
	"SP_INGAME_PULL2",
	"SP_INGAME_REVEL2",

	"SP_INGAME_SOULDRAIN2",
	"SP_INGAME_WEB2",
	"SP_INGAME_TRANCE2",
	"SP_INGAME_GRIP2",

	"SP_INGAME_STUN2",
	"SP_INGAME_HATE2",
	"SP_INGAME_CONTROLMIND2",
	"SP_INGAME_WEBBED2",
	"SP_INGAME_INSPIREHATE2",
	"SP_INGAME_WRACK2",

	"SP_INGAME_FORCEGLYPH2",
	"SP_INGAME_DUSTREAVER2",
	"SP_INGAME_SOUNDGLYPH2",
	"SP_INGAME_WATERREAVER2",
	"SP_INGAME_FIREREAVER2",
	"SP_INGAME_LIGHTREAVER2",
	"SP_INGAME_AIRREAVER2",
	"SP_INGAME_DARKREAVER2",
	"SP_INGAME_MATREAVER2",
	"SP_INGAME_POISONREAVER2",
};
// Archimond
const char *showPowersName18[MAX_SHOWPOWERS] = 
{
	"SP_INGAME_ABSORB2",
	"SP_INGAME_HEAL2",
	"SP_INGAME_PROTECT2",
	"SP_INGAME_MINDTRICK2",

	"SP_INGAME_SPEED2",
	"SP_INGAME_PUSH2",
	"SP_INGAME_PULL2",
	"SP_INGAME_REVEL2",

	"SP_INGAME_SOULDRAIN2",
	"SP_INGAME_REVERB2",
	"SP_INGAME_NECROFEAR2",
	"SP_INGAME_GRIP2",

	"SP_INGAME_STUN2",
	"SP_INGAME_HATE2",
	"SP_INGAME_CONTROLMIND2",
	"SP_INGAME_FREEZE2",
	"SP_INGAME_HEALOTHER2",
	"SP_INGAME_WRACK2",

	"SP_INGAME_FORCEGLYPH2",
	"SP_INGAME_EARTHREAVER2",
	"SP_INGAME_SOUNDGLYPH2",
	"SP_INGAME_WATERREAVER2",
	"SP_INGAME_FIREREAVER2",
	"SP_INGAME_LIGHTREAVER2",
	"SP_INGAME_AIRREAVER2",
	"SP_INGAME_DARKREAVER2",
	"SP_INGAME_MATREAVER2",
	"SP_INGAME_DEATHREAVER2",
};
// Archimond 2 
const char *showPowersName19[MAX_SHOWPOWERS] = 
{
	"SP_INGAME_ABSORB2",
	"SP_INGAME_HEAL2",
	"SP_INGAME_PROTECT2",
	"SP_INGAME_MINDTRICK2",

	"SP_INGAME_SPEED2",
	"SP_INGAME_PUSH2",
	"SP_INGAME_PULL2",
	"SP_INGAME_REVEL2",

	"SP_INGAME_NECRODRAIN2",
	"SP_INGAME_SKULL2",
	"SP_INGAME_NECROFEAR2",
	"SP_INGAME_GRIP2",

	"SP_INGAME_STUN2",
	"SP_INGAME_HATE2",
	"SP_INGAME_CONTROLMIND2",
	"SP_INGAME_FREEZE2",
	"SP_INGAME_HEALOTHER2",
	"SP_INGAME_WRACK2",

	"SP_INGAME_FORCEGLYPH2",
	"SP_INGAME_EARTHREAVER2",
	"SP_INGAME_SOUNDGLYPH2",
	"SP_INGAME_WATERREAVER2",
	"SP_INGAME_FIREREAVER2",
	"SP_INGAME_LIGHTREAVER2",
	"SP_INGAME_AIRREAVER2",
	"SP_INGAME_DARKREAVER2",
	"SP_INGAME_MATREAVER2",
	"SP_INGAME_DEATHREAVER2",
};
// Respen - Raziel 
const char *showPowersName20[MAX_SHOWPOWERS] = 
{
	"SP_INGAME_ABSORB2",
	"SP_INGAME_HEAL2",
	"SP_INGAME_PROTECT2",
	"SP_INGAME_MINDTRICK2",

	"SP_INGAME_SPEED2",
	"SP_INGAME_PUSH2",
	"SP_INGAME_PULL2",
	"SP_INGAME_REVEL2",

	"SP_INGAME_SOULDRAIN2",
	"SP_INGAME_PSYCHIC2",
	"SP_INGAME_TRANCE2",
	"SP_INGAME_GRIP2",

	"SP_INGAME_STUN2",
	"SP_INGAME_HATE2",
	"SP_INGAME_CONTROLMIND2",
	"SP_INGAME_HOLLYSTASIS2",
	"SP_INGAME_HEALOTHER2",
	"SP_INGAME_WRACK2",

	"SP_INGAME_FORCEGLYPH2",
	"SP_INGAME_STONEGLYPH2",
	"SP_INGAME_SOUNDGLYPH2",
	"SP_INGAME_WATERGLYPH2",
	"SP_INGAME_FIREGLYPH2",
	"SP_INGAME_LIGHTGLYPH2",
	"SP_INGAME_AIRREAVER2",
	"SP_INGAME_DARKREAVER2",
	"SP_INGAME_SPIRITREAVER2",
	"SP_INGAME_SPECTRALREAVER2",
};
// Raziel - Holy reaver
const char *showPowersName21[MAX_SHOWPOWERS] = 
{
	"SP_INGAME_ABSORB2",
	"SP_INGAME_HEAL2",
	"SP_INGAME_PROTECT2",
	"SP_INGAME_MINDTRICK2",

	"SP_INGAME_SPEED2",
	"SP_INGAME_PUSH2",
	"SP_INGAME_PULL2",
	"SP_INGAME_REVEL2",

	"SP_INGAME_SOULDRAIN2",
	"SP_INGAME_HOLLY2",
	"SP_INGAME_TRANCE2",
	"SP_INGAME_GRIP2",

	"SP_INGAME_STUN2",
	"SP_INGAME_HATE2",
	"SP_INGAME_CONTROLMIND2",
	"SP_INGAME_HOLLYSTASIS2",
	"SP_INGAME_HEALOTHER2",
	"SP_INGAME_WRACK2",

	"SP_INGAME_FORCEGLYPH2",
	"SP_INGAME_EARTHREAVER2",
	"SP_INGAME_SOUNDGLYPH2",
	"SP_INGAME_WATERREAVER2",
	"SP_INGAME_FIREREAVER2",
	"SP_INGAME_LIGHTREAVER2",
	"SP_INGAME_AIRREAVER2",
	"SP_INGAME_DARKREAVER2",
	"SP_INGAME_HOLYREAVER2",
	"SP_INGAME_SPECTRALREAVER2",
};
// Raziel life reaver
const char *showPowersName22[MAX_SHOWPOWERS] = 
{
	"SP_INGAME_ABSORB2",
	"SP_INGAME_HEALS2",
	"SP_INGAME_PROTECT2",
	"SP_INGAME_MINDTRICK2",

	"SP_INGAME_SPEED2",
	"SP_INGAME_PUSH2",
	"SP_INGAME_PULL2",
	"SP_INGAME_REVEL2",

	"SP_INGAME_SOULDRAIN2",
	"SP_INGAME_LIFE2",
	"SP_INGAME_TRANCE2",
	"SP_INGAME_GRIP2",
	"SP_INGAME_STUN2",
	"SP_INGAME_HATE2",
	"SP_INGAME_CONTROLMIND2",
	"SP_INGAME_HOLLYSTASIS2",
	"SP_INGAME_HEALOTHER2",
	"SP_INGAME_WRACK2",

	"SP_INGAME_FORCEGLYPH2",
	"SP_INGAME_EARTHREAVER2",
	"SP_INGAME_SOUNDGLYPH2",
	"SP_INGAME_WATERREAVER2",
	"SP_INGAME_FIREREAVER2",
	"SP_INGAME_LIGHTREAVER2",
	"SP_INGAME_AIRREAVER2",
	"SP_INGAME_DARKREAVER2",
	"SP_INGAME_LIFEREAVER2",
	"SP_INGAME_SPECTRALREAVER2",
};
// Asgarath Sonic reaver
const char *showPowersName23[MAX_SHOWPOWERS] = 
{
	"SP_INGAME_ABSORB2",
	"SP_INGAME_HEAL2",
	"SP_INGAME_PROTECT2",
	"SP_INGAME_MINDTRICK2",

	"SP_INGAME_SPEED2",
	"SP_INGAME_PUSH2",
	"SP_INGAME_PULL2",
	"SP_INGAME_REVEL2",

	"SP_INGAME_SOULDRAIN2",
	"SP_INGAME_HOLLYFIRE2",
	"SP_INGAME_RUNESHIELD2",
	"SP_INGAME_GRIP2",

	"SP_INGAME_STUN2",
	"SP_INGAME_HATE2",
	"SP_INGAME_CONTROLMIND2",
	"SP_INGAME_IVYTRAP2",
	"SP_INGAME_HEALOTHER2",
	"SP_INGAME_WRACK2",

	"SP_INGAME_FORCEGLYPH2",
	"SP_INGAME_EARTHREAVER2",
	"SP_INGAME_SOUNDREAVER2",
	"SP_INGAME_WATERREAVER2",
	"SP_INGAME_FIREREAVER2",
	"SP_INGAME_LIGHTREAVER2",
	"SP_INGAME_AIRREAVER2",
	"SP_INGAME_DARKREAVER2",
	"SP_INGAME_MATREAVER2",
	"SP_INGAME_SPECTRALREAVER2",
};
// Asgarath astral reaver
const char *showPowersName24[MAX_SHOWPOWERS] = 
{
	"SP_INGAME_ABSORB2",
	"SP_INGAME_HEALS2",
	"SP_INGAME_PROTECT2",
	"SP_INGAME_MINDTRICK2",

	"SP_INGAME_SPEED2",
	"SP_INGAME_PUSH2",
	"SP_INGAME_PULL2",
	"SP_INGAME_REVEL2",

	"SP_INGAME_NECRODRAIN2",
	"SP_INGAME_ASTRAL2",
	"SP_INGAME_RUNESHIELD2",
	"SP_INGAME_GRIP2",

	"SP_INGAME_STUN2",
	"SP_INGAME_HATE2",
	"SP_INGAME_CONTROLMIND2",
	"SP_INGAME_GRAVITYTRAP2",
	"SP_INGAME_HEALOTHER2",
	"SP_INGAME_WRACK2",

	"SP_INGAME_FORCEGLYPH2",
	"SP_INGAME_EARTHREAVER2",
	"SP_INGAME_SOUNDREAVER2",
	"SP_INGAME_WATERREAVER2",
	"SP_INGAME_FIREREAVER2",
	"SP_INGAME_LIGHTREAVER2",
	"SP_INGAME_AIRREAVER2",
	"SP_INGAME_DARKREAVER2",
	"SP_INGAME_MATREAVER2",
	"SP_INGAME_SPECTRALREAVER2",
};
// vampires 
const char *showPowersName25[MAX_SHOWPOWERS] = 
{
	"SP_INGAME_ABSORB2",
	"SP_INGAME_HEALV2",
	"SP_INGAME_PROTECT2",
	"SP_INGAME_WOLFFORM2",

	"SP_INGAME_SPEED2B",
	"SP_INGAME_PUSH2",
	"SP_INGAME_PULL2",
	"SP_INGAME_SEEING2",

	"SP_INGAME_DRAIN2",
	"SP_INGAME_LIGHTNING2",
	"SP_INGAME_DARK_RAGE2",
	"SP_INGAME_GRIP2",

	"SP_INGAME_STUN2",
	"SP_INGAME_HATE2",
	"SP_INGAME_CONTROLMIND2",
	"SP_INGAME_FREEZE2",
	"SP_INGAME_BATFORM2",
	"SP_INGAME_WRACK2",

	"SP_INGAME_MISTFORM2",
	"SP_INGAME_TIMEREAVER2",
	"SP_INGAME_DIMENSIONREAVER2",
	"SP_INGAME_STATESREAVER2",
	"SP_INGAME_CONFLICTREAVER2",
	"SP_INGAME_ENERGYREAVER2",
	"SP_INGAME_MINDREAVER2",
	"SP_INGAME_NATUREREAVER2",
	"SP_INGAME_SOULREAVER2",
	"SP_INGAME_BLOODREAVER2",
};
// Vampire Phobos fear 
const char *showPowersName26[MAX_SHOWPOWERS] = 
{
	"SP_INGAME_ABSORB2",
	"SP_INGAME_HEALV2",
	"SP_INGAME_PROTECT2",
	"SP_INGAME_WOLFFORMDUAL2",

	"SP_INGAME_SPEED2B",
	"SP_INGAME_PUSH2",
	"SP_INGAME_PULL2",
	"SP_INGAME_SEEING2",

	"SP_INGAME_DEMONDRAIN2",
	"SP_INGAME_HELL2",
	"SP_INGAME_INSPIREFEAR2",
	"SP_INGAME_GRIP2",

	"SP_INGAME_FEARTRICK2",
	"SP_INGAME_HATE2",
	"SP_INGAME_CONTROLMIND2",
	"SP_INGAME_FREEZE2",
	"SP_INGAME_BATFORM2",
	"SP_INGAME_WRACK2",

	"SP_INGAME_MISTOFFEAR2",
	"SP_INGAME_TIMEREAVER2",
	"SP_INGAME_DIMENSIONREAVER2",
	"SP_INGAME_STATESREAVER2",
	"SP_INGAME_CONFLICTREAVER2",
	"SP_INGAME_ENERGYREAVER2",
	"SP_INGAME_FOGOFFEAR2",
	"SP_INGAME_NATUREREAVER2",
	"SP_INGAME_SOULREAVER2",
	"SP_INGAME_DEATHREAVERB2",
};
// Soul, Kain
const char *showPowersName27[MAX_SHOWPOWERS] = 
{
	"SP_INGAME_ABSORB2",
	"SP_INGAME_HEALV2",
	"SP_INGAME_PROTECT2",
	"SP_INGAME_WOLFFORM2",

	"SP_INGAME_SPEED2B",
	"SP_INGAME_PUSH2",
	"SP_INGAME_PULL2",
	"SP_INGAME_SEEING2",

	"SP_INGAME_DRAIN3",
	"SP_INGAME_LIGHTNING3",
	"SP_INGAME_DARK_RAGE2",
	"SP_INGAME_GRIP2",

	"SP_INGAME_STUN2",
	"SP_INGAME_HATE2",
	"SP_INGAME_CONTROLMIND2",
	"SP_INGAME_FREEZE2",
	"SP_INGAME_BATFORM2",
	"SP_INGAME_WRACK2",

	"SP_INGAME_MISTFORM2",
	"SP_INGAME_TIMEREAVER2",
	"SP_INGAME_DIMENSIONREAVER2",
	"SP_INGAME_STATESREAVER2",
	"SP_INGAME_CONFLICTREAVER2",
	"SP_INGAME_ENERGYREAVER2",
	"SP_INGAME_MINDREAVER2",
	"SP_INGAME_NATUREREAVER2",
	"SP_INGAME_BALANCEREAVER2",
	"SP_INGAME_DEATHREAVERB2",
};
// Darwin
const char *showPowersName28[MAX_SHOWPOWERS] = 
{
	"SP_INGAME_ABSORB2",
	"SP_INGAME_HEAL2",
	"SP_INGAME_PROTECT2",
	"SP_INGAME_MINDTRICK2",

	"SP_INGAME_SPEED2",
	"SP_INGAME_PUSH2",
	"SP_INGAME_PULL2",
	"SP_INGAME_REVEL2",

	"SP_INGAME_SOULDRAIN2",
	"SP_INGAME_WATER2",
	"SP_INGAME_BERSERK2",
	"SP_INGAME_GRIP2",

	"SP_INGAME_STUN2",
	"SP_INGAME_HATE2",
	"SP_INGAME_CONTROLMIND2",
	"SP_INGAME_FREEZE2",
	"SP_INGAME_HEALOTHER2",
	"SP_INGAME_WRACK2",

	"SP_INGAME_FORCEGLYPH2",
	"SP_INGAME_EARTHREAVER2",
	"SP_INGAME_SOUNDGLYPH2",
	"SP_INGAME_WATERREAVER2",
	"SP_INGAME_FIREREAVER2",
	"SP_INGAME_LIGHTREAVER2",
	"SP_INGAME_AIRREAVER2",
	"SP_INGAME_DARKREAVER2",
	"SP_INGAME_MATREAVER2",
	"SP_INGAME_SPECTRALREAVER2",
};
// Ezekiel
const char *showPowersName29[MAX_SHOWPOWERS] = 
{
	"SP_INGAME_ABSORB2",
	"SP_INGAME_HEALV2",
	"SP_INGAME_PROTECT2",
	"SP_INGAME_WOLFFORM2",

	"SP_INGAME_SPEED2B",
	"SP_INGAME_PUSH2",
	"SP_INGAME_PULL2",
	"SP_INGAME_SEEING2",

	"SP_INGAME_DRAIN2",
	"SP_INGAME_NECRO2",
	"SP_INGAME_DARK_RAGE2",
	"SP_INGAME_GRIP2",

	"SP_INGAME_STUN2",
	"SP_INGAME_HATE2",
	"SP_INGAME_CONTROLMIND2",
	"SP_INGAME_FREEZE2",
	"SP_INGAME_BATFORM2",
	"SP_INGAME_WRACK2",

	"SP_INGAME_MISTFORM2",
	"SP_INGAME_TIMEREAVER2",
	"SP_INGAME_DIMENSIONREAVER2",
	"SP_INGAME_STATESREAVER2",
	"SP_INGAME_CONFLICTREAVER2",
	"SP_INGAME_ENERGYREAVER2",
	"SP_INGAME_MINDREAVER2",
	"SP_INGAME_NATUREREAVER2",
	"SP_INGAME_SOULREAVER2",
	"SP_INGAME_BLOODREAVER2",
};
// Samah'el and Osil 
const char *showPowersName30[MAX_SHOWPOWERS] = 
{
	"SP_INGAME_ABSORB2",
	"SP_INGAME_HEALV2",
	"SP_INGAME_PROTECT2",
	"SP_INGAME_HELLWOLF2",

	"SP_INGAME_SPEED2B",
	"SP_INGAME_PUSH2",
	"SP_INGAME_PULL2",
	"SP_INGAME_SEEING2",

	"SP_INGAME_DEMONDRAIN2",
	"SP_INGAME_HELL2",
	"SP_INGAME_DARK_RAGE2",
	"SP_INGAME_GRIP2",

	"SP_INGAME_STUN2",
	"SP_INGAME_HATE2",
	"SP_INGAME_CONTROLMIND2",
	"SP_INGAME_FREEZE2",
	"SP_INGAME_BIRDFORM2",
	"SP_INGAME_WRACK2",

	"SP_INGAME_HELLMIST2",
	"SP_INGAME_TIMEREAVER2",
	"SP_INGAME_DIMENSIONREAVER2",
	"SP_INGAME_STATESREAVER2",
	"SP_INGAME_CONFLICTREAVER2",
	"SP_INGAME_ENERGYREAVER2",
	"SP_INGAME_MINDREAVER2",
	"SP_INGAME_NATUREREAVER2",
	"SP_INGAME_SOULREAVER2",
	"SP_INGAME_DEATHREAVERB2",
};

// FORMS MATRIX 
const char *showPowersName31[MAX_SHOWPOWERS] = 
{
	"SP_INGAME_ABSORB2",
	"SP_INGAME_HEALV2",
	"SP_INGAME_PROTECT2",
	"SP_INGAME_REVERTWOLF2",

	"SP_INGAME_SPEED2B",
	"SP_INGAME_PUSH2",
	"SP_INGAME_PULL2",
	"SP_INGAME_ECHOBAT2",

	"SP_INGAME_DRAIN2",
	"SP_INGAME_LIGHTNING2",
	"SP_INGAME_DARK_RAGE2",
	"SP_INGAME_GRIP2",

	"SP_INGAME_STUN2",
	"SP_INGAME_HATE2",
	"SP_INGAME_CONTROLMIND2",
	"SP_INGAME_FREEZE2",
	"SP_INGAME_REVERTBAT2",
	"SP_INGAME_WRACK2",

	"SP_INGAME_REVERTMIST2",
	"SP_INGAME_TIMEREAVER2",
	"SP_INGAME_DIMENSIONREAVER2",
	"SP_INGAME_STATESREAVER2",
	"SP_INGAME_CONFLICTREAVER2",
	"SP_INGAME_ENERGYREAVER2",
	"SP_INGAME_MINDREAVER2",
	"SP_INGAME_NATUREREAVER2",
	"SP_INGAME_SOULREAVER2",
	"SP_INGAME_BLOODREAVER2",
};


// Shade
const char *showPowersName32[MAX_SHOWPOWERS] = 
{
	"SP_INGAME_ABSORB2",
	"SP_INGAME_HEALD2",
	"SP_INGAME_PROTECT2",
	"SP_INGAME_MINDTRICK2",

	"SP_INGAME_SPEED2",
	"SP_INGAME_PUSH2",
	"SP_INGAME_PULL2",
	"SP_INGAME_ECHOBAT2",

	"SP_INGAME_DEMONDRAIN2",
	"SP_INGAME_POISON2",
	"SP_INGAME_POISONSHIELD2",
	"SP_INGAME_GRIP2",

	"SP_INGAME_STUN2",
	"SP_INGAME_HATE2",
	"SP_INGAME_CONTROLMIND2",
	"SP_INGAME_FREEZE2",
	"SP_INGAME_FEARTRICK2",
	"SP_INGAME_WRACK2",

	"SP_INGAME_FORCEGLYPH2",
	"SP_INGAME_STONEGLYPH2",
	"SP_INGAME_SOUNDGLYPH2",
	"SP_INGAME_WATERGLYPH2",
	"SP_INGAME_FIREGLYPH2",
	"SP_INGAME_LIGHTGLYPH2",
	"SP_INGAME_AIRREAVER2",
	"SP_INGAME_DARKREAVER2",
	"SP_INGAME_MATREAVER2",
	"SP_INGAME_POISONREAVER2",
};
// Seer \ Hylden
const char *showPowersName33[MAX_SHOWPOWERS] = 
{
	"SP_INGAME_ABSORB2",
	"SP_INGAME_HEALH2",
	"SP_INGAME_PROTECT2",
	"SP_INGAME_MINDTRICK2",

	"SP_INGAME_SPEED2",
	"SP_INGAME_PUSH2",
	"SP_INGAME_PULL2",
	"SP_INGAME_HYLREVEL2",

	"SP_INGAME_HYLDENDRAIN2",
	"SP_INGAME_HYLDEN2",
	"SP_INGAME_TKSHIELD2",
	"SP_INGAME_GRIP2",

	"SP_INGAME_STUN2",
	"SP_INGAME_HATE2",
	"SP_INGAME_CONTROLMIND2",
	"SP_INGAME_FREEZE2",
	"SP_INGAME_HEALOTHER2",
	"SP_INGAME_WRACK2",

	"SP_INGAME_FORCEGLYPH2",
	"SP_INGAME_STONEGLYPH2",
	"SP_INGAME_SOUNDGLYPH2",
	"SP_INGAME_WATERGLYPH2",
	"SP_INGAME_FIREGLYPH2",
	"SP_INGAME_LIGHTGLYPH2",
	"SP_INGAME_WINDGLYPH2",
	"SP_INGAME_HOLYGLYPH2",
	"SP_INGAME_DARKGLYPH2",
	"SP_INGAME_NECROGLYPH2",
};
// Xari
const char *showPowersName34[MAX_SHOWPOWERS] = 
{
		"SP_INGAME_ABSORB2",
	"SP_INGAME_HEALV2",
	"SP_INGAME_PROTECT2",
	"SP_INGAME_WOLFFORM2",

	"SP_INGAME_SPEED2B",
	"SP_INGAME_PUSH2",
	"SP_INGAME_PULL2",
	"SP_INGAME_SEEING2",

	"SP_INGAME_DRAIN2",
	"SP_INGAME_WEB2",
	"SP_INGAME_DARK_RAGE2",
	"SP_INGAME_GRIP2",

	"SP_INGAME_STUN2",
	"SP_INGAME_HATE2",
	"SP_INGAME_CONTROLMIND2",
	"SP_INGAME_WEBBED2",
	"SP_INGAME_BATFORM2",
	"SP_INGAME_WRACK2",

	"SP_INGAME_MISTFORM2",
	"SP_INGAME_TIMEREAVER2",
	"SP_INGAME_DIMENSIONREAVER2",
	"SP_INGAME_STATESREAVER2",
	"SP_INGAME_CONFLICTREAVER2",
	"SP_INGAME_ENERGYREAVER2",
	"SP_INGAME_MINDREAVER2",
	"SP_INGAME_NATUREREAVER2",
	"SP_INGAME_SOULREAVER2",
	"SP_INGAME_BLOODREAVER2",
};
// Emia 
const char *showPowersName35[MAX_SHOWPOWERS] = 
{
	"SP_INGAME_ABSORB2",
	"SP_INGAME_HEALS2",
	"SP_INGAME_PROTECT2",
	"SP_INGAME_MINDTRICK2",

	"SP_INGAME_SPEED2",
	"SP_INGAME_PUSH2",
	"SP_INGAME_PULL2",
	"SP_INGAME_SAREVEL2",

	"SP_INGAME_NECRODRAIN2",
	"SP_INGAME_HOLLY2",
	"SP_INGAME_BERSERK2",
	"SP_INGAME_GRIP2",

	"SP_INGAME_HOLYSTUN2",
	"SP_INGAME_HOLYHATE2",
	"SP_INGAME_HOLYCONTROLMIND2",
	"SP_INGAME_HOLYFREEZE2",
	"SP_INGAME_HEALOTHER3",
	"SP_INGAME_HOLYWRACK2",

	"SP_INGAME_FORCEGLYPH2",
	"SP_INGAME_STONEGLYPH2",
	"SP_INGAME_SOUNDGLYPH2",
	"SP_INGAME_WATERGLYPH2",
	"SP_INGAME_FIREGLYPH2",
	"SP_INGAME_LIGHTGLYPH2",
	"SP_INGAME_WINDGLYPH2",
	"SP_INGAME_HOLYGLYPH2",
	"SP_INGAME_DARKGLYPH2",
	"SP_INGAME_NECROGLYPH2",
};
// Elf Vampire 
const char *showPowersName36[MAX_SHOWPOWERS] = 
{
	"SP_INGAME_ABSORB2",
	"SP_INGAME_HEALV2",
	"SP_INGAME_PROTECT2",
	"SP_INGAME_LIONFORM2",

	"SP_INGAME_SPEED2B",
	"SP_INGAME_PUSH2",
	"SP_INGAME_PULL2",
	"SP_INGAME_SEEING2",

	"SP_INGAME_DRAIN2",
	"SP_INGAME_LIGHTNING2",
	"SP_INGAME_DARK_RAGE2",
	"SP_INGAME_GRIP2",

	"SP_INGAME_STUN2",
	"SP_INGAME_HATE2",
	"SP_INGAME_CONTROLMIND2",
	"SP_INGAME_FREEZE2",
	"SP_INGAME_BATFORM3",
	"SP_INGAME_WRACK2",

	"SP_INGAME_MISTFORM2",
	"SP_INGAME_TIMEREAVER2",
	"SP_INGAME_DIMENSIONREAVER2",
	"SP_INGAME_STATESREAVER2",
	"SP_INGAME_CONFLICTREAVER2",
	"SP_INGAME_ENERGYREAVER2",
	"SP_INGAME_MINDREAVER2",
	"SP_INGAME_NATUREREAVER2",
	"SP_INGAME_SOULREAVER2",
	"SP_INGAME_BLOODREAVER2",
};

////////////////// FORCE POWER NAME /////////////////////////

// Keep these with groups light side, core, and dark side
int showDataPadPowers[MAX_DPSHOWPOWERS] = 
{
	// Light side
	FP_ABSORB,
	FP_HEAL,
	FP_PROTECT,
	FP_TELEPATHY,
	FP_STUN,
	FP_WINDGLYPH, 
	FP_HOLYGLYPH,

	// Core Powers
	FP_LEVITATION,
	FP_SPEED,
	FP_PUSH,
	FP_PULL,
	FP_SABERTHROW,
	FP_SABER_DEFENSE,
	FP_SABER_OFFENSE,
	FP_SEE,
	FP_FREEZE,

	//Dark Side
	FP_DRAIN,
	FP_LIGHTNING,
	FP_RAGE,
	FP_GRIP,
	FP_HATE,
	FP_CONTROLMIND,
	FP_HEALOTHER,
	FP_WRACK,

	FP_FORCEGLYPH,
	FP_STONEGLYPH,
	FP_SOUNDGLYPH,
	FP_WATERGLYPH,
	FP_FIREGLYPH,
	FP_SUNGLYPH,
	FP_DARKGLYPH,
	FP_NECROGLYPH,
};

/*
===============
ForcePower_Valid
===============
*/
qboolean ForcePower_Valid(int index)
{
	gentity_t	*player = &g_entities[0];

	assert (MAX_SHOWPOWERS == ( sizeof(showPowers)/sizeof(showPowers[0]) ));
	assert (index < MAX_SHOWPOWERS );	//is this a valid index?
	if (player->client->ps.forcePowersKnown & (1 << showPowers[index]) && 
		player->client->ps.forcePowerLevel[showPowers[index]])	// Does he have the force power?
	{
		return qtrue;
	}

	return qfalse;
}

/*
===============
CG_NextForcePower_f
===============
*/
void CG_NextForcePower_f( void ) 
{
	int		i;

	if ( !cg.snap || in_camera ) 
	{
		return;
	}

	SetForcePowerTime();

	if ((cg.forcepowerSelectTime + WEAPON_SELECT_TIME) < cg.time)
	{
		return;
	}

	const int original = cg.forcepowerSelect;

	for ( i = 0; i < MAX_SHOWPOWERS; i++ ) 
	{
		cg.forcepowerSelect++;

		if (cg.forcepowerSelect >= MAX_SHOWPOWERS)
		{ 
			cg.forcepowerSelect = 0; 
		}

		if (ForcePower_Valid(cg.forcepowerSelect))	// Does he have the force power?
		{
			cgi_S_StartSound (NULL, 0, CHAN_AUTO, cgs.media.selectSound2 );
			return;
		}
	}

	cg.forcepowerSelect = original;
}

/*
===============
CG_PrevForcePower_f
===============
*/
void CG_PrevForcePower_f( void ) 
{
	int		i;

	if ( !cg.snap || in_camera ) 
	{
		return;
	}

	SetForcePowerTime();

	if ((cg.forcepowerSelectTime + WEAPON_SELECT_TIME) < cg.time)
	{
		return;
	}

	const int original = cg.forcepowerSelect;

	for ( i = 0; i < MAX_SHOWPOWERS; i++ ) 
	{
		cg.forcepowerSelect--;

		if (cg.forcepowerSelect < 0)
		{ 
			cg.forcepowerSelect = MAX_SHOWPOWERS - 1; 
		}

		if (ForcePower_Valid(cg.forcepowerSelect))	// Does he have the force power?
		{
			cgi_S_StartSound (NULL, 0, CHAN_AUTO, cgs.media.selectSound2 );
			return;
		}
	}


	cg.forcepowerSelect = original;
}

/*
===================
CG_DrawForceSelect
===================
*/
void CG_DrawForceSelect( void ) 
{
	int		i;
	int		count;
	int		holdX;
	int		sideLeftIconCnt,sideRightIconCnt;
	int		holdCount,iconCnt;
	char	text[1024]={0};
	int		yOffset = 0;
	// New Adding 
	gentity_t	*player = &g_entities[0];

	// don't display if dead
	if ( cg.predicted_player_state.stats[STAT_HEALTH] <= 0 || ( cg.snap->ps.viewEntity > 0 && cg.snap->ps.viewEntity < ENTITYNUM_WORLD )) 
	{
		return;
	}

	if ((cg.forcepowerSelectTime+WEAPON_SELECT_TIME)<cg.time)	// Time is up for the HUD to display
	{
		return;
	}

	// count the number of powers owned
	count = 0;

	for (i=0; i<MAX_SHOWPOWERS; ++i)
	{
		if (ForcePower_Valid(i))
		{
			count++;
		}
	}

	if (count == 0)	// If no force powers, don't display
	{
		return;
	}

/*
	int x2,y2;
	if (!cgi_UI_GetMenuInfo("forceselecthud",&x2,&y2,&w2,&h2))
	{
		return;
	}
*/
	cg.iconSelectTime = cg.forcepowerSelectTime;

	// showing weapon select clears pickup item display, but not the blend blob
	cg.itemPickupTime = 0;

	const int sideMax = 3;	// Max number of icons on the side

	// Calculate how many icons will appear to either side of the center one
	holdCount = count - 1;	// -1 for the center icon
	if (holdCount == 0)			// No icons to either side
	{
		sideLeftIconCnt = 0;
		sideRightIconCnt = 0;
	}
	else if (count > (2*sideMax))	// Go to the max on each side
	{
		sideLeftIconCnt = sideMax;
		sideRightIconCnt = sideMax;
	}
	else							// Less than max, so do the calc
	{
		sideLeftIconCnt = holdCount/2;
		sideRightIconCnt = holdCount - sideLeftIconCnt;
	}

	const int smallIconSize = 30;
	const int bigIconSize = 60;
	const int pad = 12;

	const int x = 320;
	const int y = 425;

	i = cg.forcepowerSelect - 1;
	if (i < 0)
	{
		i = MAX_SHOWPOWERS-1;
	}

	cgi_R_SetColor(NULL);
	// ORIGINAL CODE 
	// Work backwards from current icon
	holdX = x - ((bigIconSize/2) + pad + smallIconSize);
	for (iconCnt=1;iconCnt<(sideLeftIconCnt+1);i--)
	{
		if (i < 0)
		{
			i = MAX_SHOWPOWERS-1;
		}

		if (!ForcePower_Valid(i))	// Does he have this power?
		{
			continue;
		}

		++iconCnt;					// Good icon

	
	if ( player->client->NPC_class == CLASS_DARKREAVER ||
		player->client->NPC_class == CLASS_SABOTEUR ||
		player->client->NPC_class == CLASS_DARK ||
		player->client->NPC_class == CLASS_GOLEM_DARK )
	{
		if (force_icons[showPowers[i]])
		{
			CG_DrawPic( holdX, y + yOffset, smallIconSize, smallIconSize, force_icons1[showPowers[i]] ); 
			holdX -= (smallIconSize+pad);
		}
	}
	else if ( player->client->NPC_class == CLASS_TANKREAVER ||
	player->client->NPC_class == CLASS_BESPIN_COP ||
	player->client->NPC_class == CLASS_GOLEM_SMOKE )
	{
		if (force_icons[showPowers[i]])
		{
			CG_DrawPic( holdX, y + yOffset, smallIconSize, smallIconSize, force_icons2[showPowers[i]] ); 
			holdX -= (smallIconSize+pad);
		}
	}
	else if ( player->client->NPC_class == CLASS_SHADOWTROOPER ||
	player->client->NPC_class == CLASS_VOID || 
	player->client->NPC_class == CLASS_GOLEM_VOID )
	{
		if (force_icons[showPowers[i]])
		{
			CG_DrawPic( holdX, y + yOffset, smallIconSize, smallIconSize, force_icons3[showPowers[i]] ); 
			holdX -= (smallIconSize+pad);
		}
	}
	else if ( player->client->NPC_class == CLASS_LIGHTREAVER ||
		player->client->NPC_class == CLASS_SABER_DROID ||
		player->client->NPC_class == CLASS_ASSASSIN_DROID ||
		player->client->NPC_class == CLASS_LIGHT ||
		player->client->NPC_class == CLASS_GOLEM_LIGHT )
	{
		if (force_icons[showPowers[i]])
		{
			CG_DrawPic( holdX, y + yOffset, smallIconSize, smallIconSize, force_icons4[showPowers[i]] ); 
			holdX -= (smallIconSize+pad);
		}
	}
	else if ( player->client->NPC_class == CLASS_MORGANKATARN ||
		player->client->NPC_class == CLASS_GOLEM_GOLD ||
		player->client->NPC_class == CLASS_PROTOCOL ||
		player->client->NPC_class == CLASS_ATST )
	{
		if (force_icons[showPowers[i]])
		{
			CG_DrawPic( holdX, y + yOffset, smallIconSize, smallIconSize, force_icons5[showPowers[i]] ); 
			holdX -= (smallIconSize+pad);
		}
	}
	else if ( player->client->NPC_class == CLASS_FIREREAVER || 
		player->client->NPC_class == CLASS_GOLEM_FIRE || 
		player->client->NPC_class == CLASS_FIRE )
	{
		if (force_icons[showPowers[i]])
		{
			CG_DrawPic( holdX, y + yOffset, smallIconSize, smallIconSize, force_icons6[showPowers[i]] ); 
			holdX -= (smallIconSize+pad);
		}
	}
	else if ( player->client->NPC_class == CLASS_CLAW || player->client->NPC_class == CLASS_CLAW || player->client->NPC_class == CLASS_GOLEM_LAVA )
					
	{
		if (force_icons[showPowers[i]])
		{
			CG_DrawPic( holdX, y + yOffset, smallIconSize, smallIconSize, force_icons7[showPowers[i]] ); 
			holdX -= (smallIconSize+pad);
		}
	}
	else if ( player->client->NPC_class == CLASS_REBEL || 
		player->client->NPC_class == CLASS_NAERYANMIST ||
				player->client->NPC_class == CLASS_PROBE || 
				player->client->NPC_class == CLASS_AIR ||
				player->client->NPC_class == CLASS_GOLEM_AIR ||
				player->client->NPC_class == CLASS_BIRD 	)
	{
		if (force_icons[showPowers[i]])
		{
			CG_DrawPic( holdX, y + yOffset, smallIconSize, smallIconSize, force_icons8[showPowers[i]] ); 
			holdX -= (smallIconSize+pad);
		}
	}
	else if ( player->client->NPC_class == CLASS_AIRREAVER || player->client->NPC_class == CLASS_POLTER ||
		player->client->NPC_class == CLASS_NAERYANMIST2 	)
	{
		if (force_icons[showPowers[i]])
		{
			CG_DrawPic( holdX, y + yOffset, smallIconSize, smallIconSize, force_icons9[showPowers[i]] ); 
			holdX -= (smallIconSize+pad);
		}
	}
	else if ( player->client->NPC_class == CLASS_ROCKETTROOPER ||
		player->client->NPC_class == CLASS_NAERYANMIST3 ||
			 player->client->NPC_class == CLASS_GOLEM_METAL ||
			  player->client->NPC_class == CLASS_DEMON_ELECTRO ||
			   player->client->NPC_class == CLASS_BOBAFETT )
	{
		if (force_icons[showPowers[i]])
		{
			CG_DrawPic( holdX, y + yOffset, smallIconSize, smallIconSize, force_icons10[showPowers[i]] ); 
			holdX -= (smallIconSize+pad);
		}
	}
	// Sonic Class (Not sure for who use it) 
	else if ( player->client->NPC_class == CLASS_VAMPIRE_SOUND ||
		player->client->NPC_class == CLASS_SENTRY ||
		player->client->NPC_class == CLASS_HOWLER ||
		player->client->NPC_class == CLASS_SONIC ||
		player->client->NPC_class == CLASS_GOLEM_SONIC )
 	{
		if (force_icons[showPowers[i]])
		{
			CG_DrawPic( holdX, y + yOffset, smallIconSize, smallIconSize, force_icons11[showPowers[i]] ); 
			holdX -= (smallIconSize+pad);
		}
	}
	else if ( player->client->NPC_class == CLASS_BARTENDER || 
		player->client->NPC_class == CLASS_ICEREAVER ||
		player->client->NPC_class == CLASS_GOLEM_ICE )
	{
		if (force_icons[showPowers[i]])
		{
			CG_DrawPic( holdX, y + yOffset, smallIconSize, smallIconSize, force_icons12[showPowers[i]] ); 
			holdX -= (smallIconSize+pad);
		}
	}
	else if ( player->client->NPC_class == CLASS_CRYSTAL ||
		player->client->NPC_class == CLASS_GALAKMECH )
	{
		if (force_icons[showPowers[i]])
		{
			CG_DrawPic( holdX, y + yOffset, smallIconSize, smallIconSize, force_icons13[showPowers[i]] ); 
			holdX -= (smallIconSize+pad);
		}
	}
	else if ( player->client->NPC_class == CLASS_SPIDERREAVER )
	{
		if (force_icons[showPowers[i]])
		{
			CG_DrawPic( holdX, y + yOffset, smallIconSize, smallIconSize, force_icons14[showPowers[i]] ); 
			holdX -= (smallIconSize+pad);
		}
	}
	else if ( player->client->NPC_class == CLASS_SPIDERREAVER2 || player->client->NPC_class == CLASS_UGNAUGHT )
	{
		if (force_icons[showPowers[i]])
		{
			CG_DrawPic( holdX, y + yOffset, smallIconSize, smallIconSize, force_icons15[showPowers[i]] ); 
			holdX -= (smallIconSize+pad);
		}
	}
	else if ( player->client->NPC_class == CLASS_EARTHREAVER || 
				player->client->NPC_class == CLASS_LIZARD || 
				player->client->NPC_class == CLASS_EARTH || 
				player->client->NPC_class == CLASS_GONK ||
				player->client->NPC_class == CLASS_DEMON_BLADE || 
				player->client->NPC_class == CLASS_GOLEM )
	{
		if (force_icons[showPowers[i]])
		{
			CG_DrawPic( holdX, y + yOffset, smallIconSize, smallIconSize, force_icons16[showPowers[i]] ); 
			holdX -= (smallIconSize+pad);
		}
	}
	else if ( player->client->NPC_class == CLASS_SPIDERREAVER3 || player->client->NPC_class == CLASS_REELO )
	{
		if (force_icons[showPowers[i]])
		{
			CG_DrawPic( holdX, y + yOffset, smallIconSize, smallIconSize, force_icons17[showPowers[i]] ); 
			holdX -= (smallIconSize+pad);
		}
	}
	else if ( player->client->NPC_class == CLASS_NECROREAVER )
	{
		if (force_icons[showPowers[i]])
		{
			CG_DrawPic( holdX, y + yOffset, smallIconSize, smallIconSize, force_icons18[showPowers[i]] ); 
			holdX -= (smallIconSize+pad);
		}
	}
	else if ( player->client->NPC_class == CLASS_MURJJ ||
		player->client->NPC_class == CLASS_FISH || 
		player->client->NPC_class == CLASS_NECRO )
	{
		if (force_icons[showPowers[i]])
		{
			CG_DrawPic( holdX, y + yOffset, smallIconSize, smallIconSize, force_icons19[showPowers[i]] ); 
			holdX -= (smallIconSize+pad);
		}
	}
	else if ( player->client->NPC_class == CLASS_RAZIEL ||
		player->client->NPC_class == CLASS_MENTALIST )
	{
		if (force_icons[showPowers[i]])
		{
			CG_DrawPic( holdX, y + yOffset, smallIconSize, smallIconSize, force_icons20[showPowers[i]] ); 
			holdX -= (smallIconSize+pad);
		}
	}
	else if ( player->client->NPC_class == CLASS_HOLY ||
		player->client->NPC_class == CLASS_GOLEM_HOLY )
	{
		if (force_icons[showPowers[i]])
		{
			CG_DrawPic( holdX, y + yOffset, smallIconSize, smallIconSize, force_icons21[showPowers[i]] ); 
			holdX -= (smallIconSize+pad);
		}
	}
	else if ( player->client->NPC_class == CLASS_LUKE ) 
	{
		if (force_icons[showPowers[i]])
		{
			CG_DrawPic( holdX, y + yOffset, smallIconSize, smallIconSize, force_icons22[showPowers[i]] ); 
			holdX -= (smallIconSize+pad);
		}
	}
	else if ( player->client->NPC_class == CLASS_DRUIDREAVER ||
		player->client->NPC_class == CLASS_TRANDOSHAN ||
		player->client->NPC_class == CLASS_LYCAN ||
		player->client->NPC_class == CLASS_DRUID ) 
	{
		if (force_icons[showPowers[i]])
		{
			CG_DrawPic( holdX, y + yOffset, smallIconSize, smallIconSize, force_icons23[showPowers[i]] ); 
			holdX -= (smallIconSize+pad);
		}
	}
	else if ( player->client->NPC_class == CLASS_LANDO ) 
	{
		if (force_icons[showPowers[i]])
		{
			CG_DrawPic( holdX, y + yOffset, smallIconSize, smallIconSize, force_icons24[showPowers[i]] ); 
			holdX -= (smallIconSize+pad);
		}
	}
	else if ( player->client->NPC_class == CLASS_VAMPIRE ||
		player->client->NPC_class == CLASS_REBORN ||	
		player->client->NPC_class == CLASS_KAIN ||
		player->client->NPC_class == CLASS_KAIN2 ||
		player->client->NPC_class == CLASS_UMAH ||
		player->client->NPC_class == CLASS_VAMPIRE_DARK ||
		player->client->NPC_class == CLASS_VAMPIRE_NECRO ||
		player->client->NPC_class == CLASS_VAMPIRE_SARAFAN ||
		player->client->NPC_class == CLASS_HALFVAMPIRE ||
		player->client->NPC_class == CLASS_RAZIELHIM ||
		player->client->NPC_class == CLASS_MONMOTHA ||		
		player->client->NPC_class == CLASS_VAMPIRE_ELF ||
		player->client->NPC_class == CLASS_ALORA ||
		player->client->NPC_class == CLASS_VAMPIRE_WATER ||
		player->client->NPC_class == CLASS_VAMPIRE_LYCAN ||
		player->client->NPC_class == CLASS_VAMPIRE_ARCHER ||
		player->client->NPC_class == CLASS_VAMPIRE_BOUNTY ||
		player->client->NPC_class == CLASS_VAMPIRE_BLOOD ||
		player->client->NPC_class == CLASS_DUMAHIM ||
		player->client->NPC_class == CLASS_WEEQUAY ||
		player->client->NPC_class == CLASS_GRAN ||
		player->client->NPC_class == CLASS_TURELIM ) 
	{
		if (force_icons[showPowers[i]])
		{
			CG_DrawPic( holdX, y + yOffset, smallIconSize, smallIconSize, force_icons25[showPowers[i]] ); 
			holdX -= (smallIconSize+pad);
		}
	}
	else if ( player->client->NPC_class == CLASS_VAMPIRE_FEAR || player->client->NPC_class == CLASS_VAMPIRE_FEAR2 )
	{
		if (force_icons[showPowers[i]])
		{
			CG_DrawPic( holdX, y + yOffset, smallIconSize, smallIconSize, force_icons26[showPowers[i]] ); 
			holdX -= (smallIconSize+pad);
		}
	}
	else if ( player->client->NPC_class == CLASS_KAIN3 ||
		player->client->NPC_class == CLASS_KYLE ||
		player->client->NPC_class == CLASS_VAMPIRE_FROST ||
		player->client->NPC_class == CLASS_NECROMANCER ||
		player->client->NPC_class == CLASS_VAMPIRE_QUEEN ||
		player->client->NPC_class == CLASS_VORADOR ||
		player->client->NPC_class == CLASS_JANOS ||
		player->client->NPC_class == CLASS_VAMPIRE_NECRO2 )
	{
		if (force_icons[showPowers[i]])
		{
			CG_DrawPic( holdX, y + yOffset, smallIconSize, smallIconSize, force_icons27[showPowers[i]] ); 
			holdX -= (smallIconSize+pad);
		}
	}
	else if (  player->client->NPC_class == CLASS_SWAMP || player->client->NPC_class == CLASS_RAHABIM ||
						player->client->NPC_class == CLASS_SHARK || player->client->NPC_class == CLASS_SWAMPTROOPER || 
						player->client->NPC_class == CLASS_WATER || player->client->NPC_class == CLASS_GOLEM_WATER )
	{
		if (force_icons[showPowers[i]])
		{
			CG_DrawPic( holdX, y + yOffset, smallIconSize, smallIconSize, force_icons28[showPowers[i]] ); 
			holdX -= (smallIconSize+pad);
		}
	}
	else if ( player->client->NPC_class == CLASS_VAMPIRE_ZOMBIE || player->client->NPC_class == CLASS_TUSKEN || 
		player->client->NPC_class == CLASS_MELC || player->client->NPC_class == CLASS_SUMMONER || 
		player->client->NPC_class == CLASS_THRALL || 	player->client->NPC_class == CLASS_GLIDER || 
		player->client->NPC_class == CLASS_COMMANDO || player->client->NPC_class == CLASS_GOLEM_NECRO )
	{
		if (force_icons[showPowers[i]])
		{
			CG_DrawPic( holdX, y + yOffset, smallIconSize, smallIconSize, force_icons29[showPowers[i]] ); 
			holdX -= (smallIconSize+pad);
		}
	}
	else if ( player->client->NPC_class == CLASS_VAMPIRE_DEMON ||
		player->client->NPC_class == CLASS_WAMPA || 
		player->client->NPC_class == CLASS_DEMON_FIRE || 
		player->client->NPC_class == CLASS_CENOBITA || 
		player->client->NPC_class == CLASS_HELLGOLEM || 
		player->client->NPC_class == CLASS_HAZARD_TROOPER || 
		player->client->NPC_class == CLASS_IMPERIAL)
	{
		if (force_icons[showPowers[i]])
		{
			CG_DrawPic( holdX, y + yOffset, smallIconSize, smallIconSize, force_icons30[showPowers[i]] ); 
			holdX -= (smallIconSize+pad);
		}
	}
	else if ( player->client->NPC_class == CLASS_WOLFFORM ||
		 player->client->NPC_class == CLASS_WOLFKAIN ||
		 player->client->NPC_class == CLASS_WOLFKAIN2 ||
		 player->client->NPC_class == CLASS_WOLFKAIN3 ||
		 player->client->NPC_class == CLASS_WOLFVORADOR ||
		 player->client->NPC_class == CLASS_WOLFJANOS || 
		 player->client->NPC_class == CLASS_WOLFNECROMANCER ||
		 player->client->NPC_class == CLASS_WOLFVAMPIRE_QUEEN ||
		 player->client->NPC_class == CLASS_WOLFVAMPIRE_FEAR ||
		 player->client->NPC_class == CLASS_WOLFVAMPIRE_FEAR2 ||
		 player->client->NPC_class == CLASS_HELLWOLF ||
		 player->client->NPC_class == CLASS_WOLFVAMPIRE_LYCAN ||
		 player->client->NPC_class == CLASS_WOLFVAMPIRE_ELF ||
		 player->client->NPC_class == CLASS_LIONVAMPIRE_ELF ||
		 player->client->NPC_class == CLASS_LIONVAMPIRE_ELF2 || 
		 player->client->NPC_class == CLASS_WOLFHALFVAMPIRE ||
		 player->client->NPC_class == CLASS_ESKANDOR || 
		 // BAT FORM  
		 player->client->NPC_class == CLASS_BATFORM ||
		 player->client->NPC_class == CLASS_BATKAIN ||
		 player->client->NPC_class == CLASS_BATKAIN2 ||
		 player->client->NPC_class == CLASS_BATKAIN3 ||
		 player->client->NPC_class == CLASS_BATVORADOR ||
		 player->client->NPC_class == CLASS_BATVAMPIRE_FEAR ||
		 player->client->NPC_class == CLASS_BATVAMPIRE_FEAR2 || 
		 player->client->NPC_class == CLASS_BATVAMPIRE_QUEEN ||
		 player->client->NPC_class == CLASS_BATNECROMANCER ||
		 player->client->NPC_class == CLASS_BATVAMPIRE_FROST ||
		player->client->NPC_class == CLASS_HELLBAT || 
		 player->client->NPC_class == CLASS_BATVAMPIRE_WATER ||
		 player->client->NPC_class == CLASS_BATVAMPIRE_ELF ||
		 player->client->NPC_class == CLASS_BATVAMPIRE_ELF2 ||
		 player->client->NPC_class == CLASS_BATVAMPIRE_ELF3 ||
	// MIST FORM 
		player->client->NPC_class == CLASS_MISTFORM || 
		player->client->NPC_class == CLASS_MISTKAIN ||
		 player->client->NPC_class == CLASS_MISTKAIN2 ||
		 player->client->NPC_class == CLASS_MISTKAIN3 ||
		 player->client->NPC_class == CLASS_MISTVORADOR ||
		 player->client->NPC_class == CLASS_MISTJANOS || 
		 player->client->NPC_class == CLASS_MISTNECROMANCER ||
		 player->client->NPC_class == CLASS_MISTVAMPIRE_QUEEN ||
		 player->client->NPC_class == CLASS_MISTVAMPIRE_FEAR ||
		 player->client->NPC_class == CLASS_MISTVAMPIRE_FEAR2 ||
		 player->client->NPC_class == CLASS_HELLMIST ||
		 player->client->NPC_class == CLASS_MISTVAMPIRE_DARK ||
		 player->client->NPC_class == CLASS_MISTVAMPIRE_BLOOD ||
		 player->client->NPC_class == CLASS_MISTVAMPIRE_WATER ||
		 player->client->NPC_class == CLASS_MISTVAMPIRE_SARAFAN ||
		 player->client->NPC_class == CLASS_MISTVAMPIRE_ELF ||
		 player->client->NPC_class == CLASS_MISTVAMPIRE_ELF2 ||
		 player->client->NPC_class == CLASS_MISTVAMPIRE_ELF3 )
	{
		if (force_icons[showPowers[i]])
		{
			CG_DrawPic( holdX, y + yOffset, smallIconSize, smallIconSize, force_icons31[showPowers[i]] ); 
			holdX -= (smallIconSize+pad);
		}
	}
	// MINEMONSTER - Shade class?
	else if ( player->client->NPC_class == CLASS_MINEMONSTER || 
	  player->client->NPC_class == CLASS_POISON ||
	  player->client->NPC_class == CLASS_IVY || 
	  player->client->NPC_class == CLASS_DEMON_POISON ||
      player->client->NPC_class == CLASS_GOLEM_POISON || 
	  player->client->NPC_class == CLASS_RANCOR || 
	  player->client->NPC_class == CLASS_INTERROGATOR )
	{
		if (force_icons[showPowers[i]])
		{
			CG_DrawPic( holdX, y + yOffset, smallIconSize, smallIconSize, force_icons32[showPowers[i]] ); 
			holdX -= (smallIconSize+pad);
		}
	}
	// hyldens 
	else if ( player->client->NPC_class == CLASS_TAVION ||
		player->client->NPC_class == CLASS_HYLDEN ||
		player->client->NPC_class == CLASS_DESANN ||
		player->client->NPC_class == CLASS_HYLDENLORD )
 	{
		if (force_icons[showPowers[i]])
		{
			CG_DrawPic( holdX, y + yOffset, smallIconSize, smallIconSize, force_icons33[showPowers[i]] ); 
			holdX -= (smallIconSize+pad);
		}
	}
	else if ( player->client->NPC_class == CLASS_NOGHRI || player->client->NPC_class == CLASS_ZEPH ||
		player->client->NPC_class == CLASS_DECEIVER || player->client->NPC_class == CLASS_FLIER2 ) 
	{
		if (force_icons[showPowers[i]])
		{
			CG_DrawPic( holdX, y + yOffset, smallIconSize, smallIconSize, force_icons34[showPowers[i]] ); 
			holdX -= (smallIconSize+pad);
		}
	}
	else if ( player->client->NPC_class == CLASS_JAN || player->client->NPC_class == CLASS_WARRIOR ||
		player->client->NPC_class == CLASS_STORMTROOPER ||player->client->NPC_class == CLASS_SARAFAN ||
		player->client->NPC_class == CLASS_PROPHET || player->client->NPC_class == CLASS_GOLEM_HOLY ||
		
		// Unused but i place however for control mind and wrack 
		player->client->NPC_class == CLASS_PRISONER || player->client->NPC_class == CLASS_RODIAN ||
		player->client->NPC_class == CLASS_ARCHER ||player->client->NPC_class == CLASS_HUNTER ||
		player->client->NPC_class == CLASS_SCOUT || player->client->NPC_class == CLASS_ALCHEMIST ||
		player->client->NPC_class == CLASS_GALAK || player->client->NPC_class == CLASS_WIZARD ||
		player->client->NPC_class == CLASS_IMPWORKER ||player->client->NPC_class == CLASS_BEAST )
	{
		if (force_icons[showPowers[i]])
		{
			CG_DrawPic( holdX, y + yOffset, smallIconSize, smallIconSize, force_icons35[showPowers[i]] ); 
			holdX -= (smallIconSize+pad);
		}
	}
	else if ( player->client->NPC_class == CLASS_VAMPIRE_ELF2 ||
		player->client->NPC_class == CLASS_VAMPIRE_ELF3 )
 	{
		if (force_icons[showPowers[i]])
		{
			CG_DrawPic( holdX, y + yOffset, smallIconSize, smallIconSize, force_icons36[showPowers[i]] ); 
			holdX -= (smallIconSize+pad);
		}
	}
	// UNUSED 	
	// ARTIFACTS 
	else if ( player->client->NPC_class == CLASS_R2D2 ||
		player->client->NPC_class == CLASS_R5D2 ||
		player->client->NPC_class == CLASS_SEEKER ||
		player->client->NPC_class == CLASS_REMOTE )		
 	{
		if (force_icons[showPowers[i]])
		{
			CG_DrawPic( holdX, y + yOffset, smallIconSize, smallIconSize, force_icons[showPowers[i]] ); 
			holdX -= (smallIconSize+pad);
		}
	}
	else if ( player->client->NPC_class == CLASS_ELDERGOD ||
		player->client->NPC_class == CLASS_SAND_CREATURE || 
		player->client->NPC_class == CLASS_TENTACLE )
 	{
		if (force_icons[showPowers[i]])
		{
			CG_DrawPic( holdX, y + yOffset, smallIconSize, smallIconSize, force_icons[showPowers[i]] ); 
			holdX -= (smallIconSize+pad);
		}
	}
	else // REAVER, JEDI, (TIZIEL NOVIZIO), DEFAULT ICONS CLASS 
		// Ariel, Sluagh, reaper, dreadnought, ghost, java, vampire ghost , mouse 
	{
		if (force_icons[showPowers[i]])
		{
			CG_DrawPic( holdX, y + yOffset, smallIconSize, smallIconSize, force_icons[showPowers[i]] ); 
			holdX -= (smallIconSize+pad);
		}
	}	
		
		
		
		
		
		/*if (force_icons[showPowers[i]])
		{
			CG_DrawPic( holdX, y + yOffset, smallIconSize, smallIconSize, force_icons[showPowers[i]] ); 
			holdX -= (smallIconSize+pad);
		}*/
	}

	// Current Center Icon
	if ( player->client->NPC_class == CLASS_DARKREAVER ||
		player->client->NPC_class == CLASS_SABOTEUR ||
		player->client->NPC_class == CLASS_DARK ||
		player->client->NPC_class == CLASS_GOLEM_DARK )
	{
		if (force_icons[showPowers[i]])
		{
			CG_DrawPic( x-(bigIconSize/2), (y-((bigIconSize-smallIconSize)/2)) + yOffset, bigIconSize, bigIconSize, force_icons1[showPowers[cg.forcepowerSelect]] );
		}
	}
	else if ( player->client->NPC_class == CLASS_TANKREAVER ||
	player->client->NPC_class == CLASS_BESPIN_COP ||
	player->client->NPC_class == CLASS_GOLEM_SMOKE )
	{
		if (force_icons[showPowers[i]])
		{
			CG_DrawPic( x-(bigIconSize/2), (y-((bigIconSize-smallIconSize)/2)) + yOffset, bigIconSize, bigIconSize, force_icons2[showPowers[cg.forcepowerSelect]] );
	
		}
	}
	else if ( player->client->NPC_class == CLASS_SHADOWTROOPER ||
	player->client->NPC_class == CLASS_VOID || 
	player->client->NPC_class == CLASS_GOLEM_VOID )
	{
		if (force_icons[showPowers[i]])
		{
			CG_DrawPic( x-(bigIconSize/2), (y-((bigIconSize-smallIconSize)/2)) + yOffset, bigIconSize, bigIconSize, force_icons3[showPowers[cg.forcepowerSelect]] );
	
		}
	}
	else if ( player->client->NPC_class == CLASS_LIGHTREAVER ||
		player->client->NPC_class == CLASS_SABER_DROID ||
		player->client->NPC_class == CLASS_ASSASSIN_DROID ||
		player->client->NPC_class == CLASS_LIGHT ||
		player->client->NPC_class == CLASS_GOLEM_LIGHT )
	{
		if (force_icons[showPowers[i]])
		{
			CG_DrawPic( x-(bigIconSize/2), (y-((bigIconSize-smallIconSize)/2)) + yOffset, bigIconSize, bigIconSize, force_icons4[showPowers[cg.forcepowerSelect]] );
	
		}
	}
	else if ( player->client->NPC_class == CLASS_MORGANKATARN ||
		player->client->NPC_class == CLASS_GOLEM_GOLD ||
		player->client->NPC_class == CLASS_PROTOCOL ||
		player->client->NPC_class == CLASS_ATST )
	{
		if (force_icons[showPowers[i]])
		{
			CG_DrawPic( x-(bigIconSize/2), (y-((bigIconSize-smallIconSize)/2)) + yOffset, bigIconSize, bigIconSize, force_icons5[showPowers[cg.forcepowerSelect]] );
	
		}
	}
	else if ( player->client->NPC_class == CLASS_FIREREAVER || 
		player->client->NPC_class == CLASS_GOLEM_FIRE || 
		player->client->NPC_class == CLASS_FIRE )
	{
		if (force_icons[showPowers[i]])
		{
			CG_DrawPic( x-(bigIconSize/2), (y-((bigIconSize-smallIconSize)/2)) + yOffset, bigIconSize, bigIconSize, force_icons6[showPowers[cg.forcepowerSelect]] );
	
		}
	}
	else if ( player->client->NPC_class == CLASS_CLAW || player->client->NPC_class == CLASS_CLAW || player->client->NPC_class == CLASS_GOLEM_LAVA )
					
	{
		if (force_icons[showPowers[i]])
		{
			CG_DrawPic( x-(bigIconSize/2), (y-((bigIconSize-smallIconSize)/2)) + yOffset, bigIconSize, bigIconSize, force_icons7[showPowers[cg.forcepowerSelect]] );
	
		}
	}
	else if ( player->client->NPC_class == CLASS_REBEL || 
		player->client->NPC_class == CLASS_NAERYANMIST ||
				player->client->NPC_class == CLASS_PROBE || 
				player->client->NPC_class == CLASS_AIR ||
				player->client->NPC_class == CLASS_GOLEM_AIR ||
				player->client->NPC_class == CLASS_BIRD 	)
	{
		if (force_icons[showPowers[i]])
		{
			CG_DrawPic( x-(bigIconSize/2), (y-((bigIconSize-smallIconSize)/2)) + yOffset, bigIconSize, bigIconSize, force_icons8[showPowers[cg.forcepowerSelect]] );
	
		}
	}
	else if ( player->client->NPC_class == CLASS_AIRREAVER || player->client->NPC_class == CLASS_POLTER ||
		player->client->NPC_class == CLASS_NAERYANMIST2	)
	{
		if (force_icons[showPowers[i]])
		{
			CG_DrawPic( x-(bigIconSize/2), (y-((bigIconSize-smallIconSize)/2)) + yOffset, bigIconSize, bigIconSize, force_icons9[showPowers[cg.forcepowerSelect]] );
	
		}
	}
	else if ( player->client->NPC_class == CLASS_ROCKETTROOPER ||
		player->client->NPC_class == CLASS_NAERYANMIST3 ||
			 player->client->NPC_class == CLASS_GOLEM_METAL ||
			  player->client->NPC_class == CLASS_DEMON_ELECTRO ||
			   player->client->NPC_class == CLASS_BOBAFETT )
	{
		if (force_icons[showPowers[i]])
		{
			CG_DrawPic( x-(bigIconSize/2), (y-((bigIconSize-smallIconSize)/2)) + yOffset, bigIconSize, bigIconSize, force_icons10[showPowers[cg.forcepowerSelect]] );
	
		}
	}
	// Sonic Class (Not sure for who use it) 
	else if ( player->client->NPC_class == CLASS_VAMPIRE_SOUND ||
		player->client->NPC_class == CLASS_SENTRY ||
		player->client->NPC_class == CLASS_HOWLER ||
		player->client->NPC_class == CLASS_SONIC ||
		player->client->NPC_class == CLASS_GOLEM_SONIC )
 	{
		if (force_icons[showPowers[i]])
		{
			CG_DrawPic( x-(bigIconSize/2), (y-((bigIconSize-smallIconSize)/2)) + yOffset, bigIconSize, bigIconSize, force_icons11[showPowers[cg.forcepowerSelect]] );
		}
	}
	else if ( player->client->NPC_class == CLASS_BARTENDER || 
		player->client->NPC_class == CLASS_ICEREAVER ||
		player->client->NPC_class == CLASS_GOLEM_ICE )
	{
		if (force_icons[showPowers[i]])
		{
			CG_DrawPic( x-(bigIconSize/2), (y-((bigIconSize-smallIconSize)/2)) + yOffset, bigIconSize, bigIconSize, force_icons12[showPowers[cg.forcepowerSelect]] );
	
		}
	}
	else if ( player->client->NPC_class == CLASS_CRYSTAL ||
		player->client->NPC_class == CLASS_GALAKMECH )
	{
		if (force_icons[showPowers[i]])
		{
			CG_DrawPic( x-(bigIconSize/2), (y-((bigIconSize-smallIconSize)/2)) + yOffset, bigIconSize, bigIconSize, force_icons13[showPowers[cg.forcepowerSelect]] );
	
		}
	}
	else if ( player->client->NPC_class == CLASS_SPIDERREAVER )
	{
		if (force_icons[showPowers[i]])
		{
			CG_DrawPic( x-(bigIconSize/2), (y-((bigIconSize-smallIconSize)/2)) + yOffset, bigIconSize, bigIconSize, force_icons14[showPowers[cg.forcepowerSelect]] );
	
		}
	}
	else if ( player->client->NPC_class == CLASS_SPIDERREAVER2 || player->client->NPC_class == CLASS_UGNAUGHT )
	{
		if (force_icons[showPowers[i]])
		{
			CG_DrawPic( x-(bigIconSize/2), (y-((bigIconSize-smallIconSize)/2)) + yOffset, bigIconSize, bigIconSize, force_icons15[showPowers[cg.forcepowerSelect]] );
	
		}
	}
	else if ( player->client->NPC_class == CLASS_EARTHREAVER || 
				player->client->NPC_class == CLASS_LIZARD || 
				player->client->NPC_class == CLASS_EARTH || 
				player->client->NPC_class == CLASS_GONK ||
				player->client->NPC_class == CLASS_DEMON_BLADE || 
				player->client->NPC_class == CLASS_GOLEM )
	{
		if (force_icons[showPowers[i]])
		{
			CG_DrawPic( x-(bigIconSize/2), (y-((bigIconSize-smallIconSize)/2)) + yOffset, bigIconSize, bigIconSize, force_icons16[showPowers[cg.forcepowerSelect]] );
	
		}
	}
	else if ( player->client->NPC_class == CLASS_SPIDERREAVER3 || player->client->NPC_class == CLASS_REELO )
	{
		if (force_icons[showPowers[i]])
		{
			CG_DrawPic( x-(bigIconSize/2), (y-((bigIconSize-smallIconSize)/2)) + yOffset, bigIconSize, bigIconSize, force_icons17[showPowers[cg.forcepowerSelect]] );
		}
	}
	else if ( player->client->NPC_class == CLASS_NECROREAVER )
	{
		if (force_icons[showPowers[i]])
		{
			CG_DrawPic( x-(bigIconSize/2), (y-((bigIconSize-smallIconSize)/2)) + yOffset, bigIconSize, bigIconSize, force_icons18[showPowers[cg.forcepowerSelect]] );
		}
	}
	else if ( player->client->NPC_class == CLASS_MURJJ ||
		player->client->NPC_class == CLASS_FISH || 
		player->client->NPC_class == CLASS_NECRO )
	{
		if (force_icons[showPowers[i]])
		{
			CG_DrawPic( x-(bigIconSize/2), (y-((bigIconSize-smallIconSize)/2)) + yOffset, bigIconSize, bigIconSize, force_icons19[showPowers[cg.forcepowerSelect]] );
		}
	}
	else if ( player->client->NPC_class == CLASS_RAZIEL ||
		player->client->NPC_class == CLASS_MENTALIST )
	{
		if (force_icons[showPowers[i]])
		{
			CG_DrawPic( x-(bigIconSize/2), (y-((bigIconSize-smallIconSize)/2)) + yOffset, bigIconSize, bigIconSize, force_icons20[showPowers[cg.forcepowerSelect]] );
		}
	}
	else if ( player->client->NPC_class == CLASS_HOLY ||
		player->client->NPC_class == CLASS_GOLEM_HOLY )
	{
		if (force_icons[showPowers[i]])
		{
			CG_DrawPic( x-(bigIconSize/2), (y-((bigIconSize-smallIconSize)/2)) + yOffset, bigIconSize, bigIconSize, force_icons21[showPowers[cg.forcepowerSelect]] );
		}
	}
	else if ( player->client->NPC_class == CLASS_LUKE ) 
	{
		if (force_icons[showPowers[i]])
		{
			CG_DrawPic( x-(bigIconSize/2), (y-((bigIconSize-smallIconSize)/2)) + yOffset, bigIconSize, bigIconSize, force_icons22[showPowers[cg.forcepowerSelect]] );
		}
	}
	else if ( player->client->NPC_class == CLASS_DRUIDREAVER ||
		player->client->NPC_class == CLASS_TRANDOSHAN ||
		player->client->NPC_class == CLASS_LYCAN ||
		player->client->NPC_class == CLASS_DRUID ) 
	{
		if (force_icons[showPowers[i]])
		{
			CG_DrawPic( x-(bigIconSize/2), (y-((bigIconSize-smallIconSize)/2)) + yOffset, bigIconSize, bigIconSize, force_icons23[showPowers[cg.forcepowerSelect]] );
		}
	}
	else if ( player->client->NPC_class == CLASS_LANDO ) 
	{
		if (force_icons[showPowers[i]])
		{
			CG_DrawPic( x-(bigIconSize/2), (y-((bigIconSize-smallIconSize)/2)) + yOffset, bigIconSize, bigIconSize, force_icons24[showPowers[cg.forcepowerSelect]] );
		}
	}
	else if ( player->client->NPC_class == CLASS_VAMPIRE ||
		player->client->NPC_class == CLASS_REBORN ||	
		player->client->NPC_class == CLASS_KAIN ||
		player->client->NPC_class == CLASS_KAIN2 ||
		player->client->NPC_class == CLASS_UMAH ||
		player->client->NPC_class == CLASS_VAMPIRE_DARK ||
		player->client->NPC_class == CLASS_VAMPIRE_NECRO ||
		player->client->NPC_class == CLASS_VAMPIRE_SARAFAN ||
		player->client->NPC_class == CLASS_HALFVAMPIRE ||
		player->client->NPC_class == CLASS_RAZIELHIM ||
		player->client->NPC_class == CLASS_MONMOTHA ||		
		player->client->NPC_class == CLASS_VAMPIRE_ELF ||
		player->client->NPC_class == CLASS_ALORA ||
		player->client->NPC_class == CLASS_VAMPIRE_WATER ||
		player->client->NPC_class == CLASS_VAMPIRE_LYCAN ||
		player->client->NPC_class == CLASS_VAMPIRE_ARCHER ||
		player->client->NPC_class == CLASS_VAMPIRE_BOUNTY ||
		player->client->NPC_class == CLASS_VAMPIRE_BLOOD ||
		player->client->NPC_class == CLASS_DUMAHIM ||
		player->client->NPC_class == CLASS_WEEQUAY ||
		player->client->NPC_class == CLASS_GRAN ||
		player->client->NPC_class == CLASS_TURELIM ) 
	{
		if (force_icons[showPowers[i]])
		{
			CG_DrawPic( x-(bigIconSize/2), (y-((bigIconSize-smallIconSize)/2)) + yOffset, bigIconSize, bigIconSize, force_icons25[showPowers[cg.forcepowerSelect]] );
		}
	}
	else if ( player->client->NPC_class == CLASS_VAMPIRE_FEAR || player->client->NPC_class == CLASS_VAMPIRE_FEAR2 )
	{
		if (force_icons[showPowers[i]])
		{
			CG_DrawPic( x-(bigIconSize/2), (y-((bigIconSize-smallIconSize)/2)) + yOffset, bigIconSize, bigIconSize, force_icons26[showPowers[cg.forcepowerSelect]] );
		}
	}
	else if ( player->client->NPC_class == CLASS_KAIN3 ||
		player->client->NPC_class == CLASS_KYLE ||
		player->client->NPC_class == CLASS_VAMPIRE_FROST ||
		player->client->NPC_class == CLASS_NECROMANCER ||
		player->client->NPC_class == CLASS_VAMPIRE_QUEEN ||
		player->client->NPC_class == CLASS_VORADOR ||
		player->client->NPC_class == CLASS_JANOS ||
		player->client->NPC_class == CLASS_VAMPIRE_NECRO2 )
	{
		if (force_icons[showPowers[i]])
		{
			CG_DrawPic( x-(bigIconSize/2), (y-((bigIconSize-smallIconSize)/2)) + yOffset, bigIconSize, bigIconSize, force_icons27[showPowers[cg.forcepowerSelect]] );
	}
	}
	else if (  player->client->NPC_class == CLASS_SWAMP || player->client->NPC_class == CLASS_RAHABIM ||
						player->client->NPC_class == CLASS_SHARK || player->client->NPC_class == CLASS_SWAMPTROOPER || 
						player->client->NPC_class == CLASS_WATER || player->client->NPC_class == CLASS_GOLEM_WATER )
	{
		if (force_icons[showPowers[i]])
		{
			CG_DrawPic( x-(bigIconSize/2), (y-((bigIconSize-smallIconSize)/2)) + yOffset, bigIconSize, bigIconSize, force_icons28[showPowers[cg.forcepowerSelect]] );
	}
	}
	else if ( player->client->NPC_class == CLASS_VAMPIRE_ZOMBIE || player->client->NPC_class == CLASS_TUSKEN || 
		player->client->NPC_class == CLASS_MELC || player->client->NPC_class == CLASS_SUMMONER || 
		player->client->NPC_class == CLASS_THRALL || 	player->client->NPC_class == CLASS_GLIDER || 
		player->client->NPC_class == CLASS_COMMANDO || player->client->NPC_class == CLASS_GOLEM_NECRO )
	{
		if (force_icons[showPowers[i]])
		{
			CG_DrawPic( x-(bigIconSize/2), (y-((bigIconSize-smallIconSize)/2)) + yOffset, bigIconSize, bigIconSize, force_icons29[showPowers[cg.forcepowerSelect]] );
	}
	}
	else if ( player->client->NPC_class == CLASS_VAMPIRE_DEMON ||
		player->client->NPC_class == CLASS_WAMPA || 
		player->client->NPC_class == CLASS_DEMON_FIRE || 
		player->client->NPC_class == CLASS_CENOBITA || 
		player->client->NPC_class == CLASS_HELLGOLEM || 
		player->client->NPC_class == CLASS_HAZARD_TROOPER || 
		player->client->NPC_class == CLASS_IMPERIAL)
	{
		if (force_icons[showPowers[i]])
		{
			CG_DrawPic( x-(bigIconSize/2), (y-((bigIconSize-smallIconSize)/2)) + yOffset, bigIconSize, bigIconSize, force_icons30[showPowers[cg.forcepowerSelect]] );
	}
	}
	else if ( player->client->NPC_class == CLASS_WOLFFORM ||
		 player->client->NPC_class == CLASS_WOLFKAIN ||
		 player->client->NPC_class == CLASS_WOLFKAIN2 ||
		 player->client->NPC_class == CLASS_WOLFKAIN3 ||
		 player->client->NPC_class == CLASS_WOLFVORADOR ||
		 player->client->NPC_class == CLASS_WOLFJANOS || 
		 player->client->NPC_class == CLASS_WOLFNECROMANCER ||
		 player->client->NPC_class == CLASS_WOLFVAMPIRE_QUEEN ||
		 player->client->NPC_class == CLASS_WOLFVAMPIRE_FEAR ||
		 player->client->NPC_class == CLASS_WOLFVAMPIRE_FEAR2 ||
		 player->client->NPC_class == CLASS_HELLWOLF ||
		 player->client->NPC_class == CLASS_WOLFVAMPIRE_LYCAN ||
		 player->client->NPC_class == CLASS_WOLFVAMPIRE_ELF ||
		 player->client->NPC_class == CLASS_LIONVAMPIRE_ELF ||
		 player->client->NPC_class == CLASS_LIONVAMPIRE_ELF2 || 
		 player->client->NPC_class == CLASS_WOLFHALFVAMPIRE ||
		 player->client->NPC_class == CLASS_ESKANDOR || 
		 // BAT FORM  
		 player->client->NPC_class == CLASS_BATFORM ||
		 player->client->NPC_class == CLASS_BATKAIN ||
		 player->client->NPC_class == CLASS_BATKAIN2 ||
		 player->client->NPC_class == CLASS_BATKAIN3 ||
		 player->client->NPC_class == CLASS_BATVORADOR ||
		 player->client->NPC_class == CLASS_BATVAMPIRE_FEAR ||
		 player->client->NPC_class == CLASS_BATVAMPIRE_FEAR2 || 
		 player->client->NPC_class == CLASS_BATVAMPIRE_QUEEN ||
		 player->client->NPC_class == CLASS_BATNECROMANCER ||
		 player->client->NPC_class == CLASS_BATVAMPIRE_FROST ||
		player->client->NPC_class == CLASS_HELLBAT || 
		 player->client->NPC_class == CLASS_BATVAMPIRE_WATER ||
		 player->client->NPC_class == CLASS_BATVAMPIRE_ELF ||
		 player->client->NPC_class == CLASS_BATVAMPIRE_ELF2 ||
		 player->client->NPC_class == CLASS_BATVAMPIRE_ELF3 ||
	// MIST FORM 
		player->client->NPC_class == CLASS_MISTFORM || 
		player->client->NPC_class == CLASS_MISTKAIN ||
		 player->client->NPC_class == CLASS_MISTKAIN2 ||
		 player->client->NPC_class == CLASS_MISTKAIN3 ||
		 player->client->NPC_class == CLASS_MISTVORADOR ||
		 player->client->NPC_class == CLASS_MISTJANOS || 
		 player->client->NPC_class == CLASS_MISTNECROMANCER ||
		 player->client->NPC_class == CLASS_MISTVAMPIRE_QUEEN ||
		 player->client->NPC_class == CLASS_MISTVAMPIRE_FEAR ||
		 player->client->NPC_class == CLASS_MISTVAMPIRE_FEAR2 ||
		 player->client->NPC_class == CLASS_HELLMIST ||
		 player->client->NPC_class == CLASS_MISTVAMPIRE_DARK ||
		 player->client->NPC_class == CLASS_MISTVAMPIRE_BLOOD ||
		 player->client->NPC_class == CLASS_MISTVAMPIRE_WATER ||
		  player->client->NPC_class == CLASS_MISTVAMPIRE_SARAFAN ||
		 player->client->NPC_class == CLASS_MISTVAMPIRE_ELF ||
		 player->client->NPC_class == CLASS_MISTVAMPIRE_ELF2 ||
		 player->client->NPC_class == CLASS_MISTVAMPIRE_ELF3 )
	{
		if (force_icons[showPowers[i]])
		{
			CG_DrawPic( x-(bigIconSize/2), (y-((bigIconSize-smallIconSize)/2)) + yOffset, bigIconSize, bigIconSize, force_icons31[showPowers[cg.forcepowerSelect]] );
	}
	}
	// MINEMONSTER - Shade class?
	else if ( player->client->NPC_class == CLASS_MINEMONSTER || 
	  player->client->NPC_class == CLASS_POISON ||
	  player->client->NPC_class == CLASS_IVY || 
	  player->client->NPC_class == CLASS_DEMON_POISON ||
      player->client->NPC_class == CLASS_GOLEM_POISON || 
	  player->client->NPC_class == CLASS_RANCOR || 
	  player->client->NPC_class == CLASS_INTERROGATOR )
	{
		if (force_icons[showPowers[i]])
		{
			CG_DrawPic( x-(bigIconSize/2), (y-((bigIconSize-smallIconSize)/2)) + yOffset, bigIconSize, bigIconSize, force_icons32[showPowers[cg.forcepowerSelect]] );
		}
	}
	// hyldens 
	else if ( player->client->NPC_class == CLASS_TAVION ||
		player->client->NPC_class == CLASS_HYLDEN ||
		player->client->NPC_class == CLASS_DESANN ||
		player->client->NPC_class == CLASS_HYLDENLORD )
 	{
		if (force_icons[showPowers[i]])
		{
			CG_DrawPic( x-(bigIconSize/2), (y-((bigIconSize-smallIconSize)/2)) + yOffset, bigIconSize, bigIconSize, force_icons33[showPowers[cg.forcepowerSelect]] );
		}
	}
	else if ( player->client->NPC_class == CLASS_NOGHRI || player->client->NPC_class == CLASS_ZEPH ||
		player->client->NPC_class == CLASS_DECEIVER || player->client->NPC_class == CLASS_FLIER2 ) 
	{
		if (force_icons[showPowers[i]])
		{
			CG_DrawPic( x-(bigIconSize/2), (y-((bigIconSize-smallIconSize)/2)) + yOffset, bigIconSize, bigIconSize, force_icons34[showPowers[cg.forcepowerSelect]] );
		}
	}
	else if ( player->client->NPC_class == CLASS_JAN || player->client->NPC_class == CLASS_WARRIOR ||
		player->client->NPC_class == CLASS_STORMTROOPER ||player->client->NPC_class == CLASS_SARAFAN ||
		player->client->NPC_class == CLASS_PROPHET || player->client->NPC_class == CLASS_GOLEM_HOLY ||
		
		// Unused but i place however for control mind and wrack 
		player->client->NPC_class == CLASS_PRISONER || player->client->NPC_class == CLASS_RODIAN ||
		player->client->NPC_class == CLASS_ARCHER ||player->client->NPC_class == CLASS_HUNTER ||
		player->client->NPC_class == CLASS_SCOUT || player->client->NPC_class == CLASS_ALCHEMIST ||
		player->client->NPC_class == CLASS_GALAK || player->client->NPC_class == CLASS_WIZARD ||
		player->client->NPC_class == CLASS_IMPWORKER ||player->client->NPC_class == CLASS_BEAST )
	{
		if (force_icons[showPowers[i]])
		{
			CG_DrawPic( x-(bigIconSize/2), (y-((bigIconSize-smallIconSize)/2)) + yOffset, bigIconSize, bigIconSize, force_icons35[showPowers[cg.forcepowerSelect]] );
	}
	}
	else if ( player->client->NPC_class == CLASS_VAMPIRE_ELF2 ||
		player->client->NPC_class == CLASS_VAMPIRE_ELF3 )
 	{
		if (force_icons[showPowers[i]])
		{
			CG_DrawPic( x-(bigIconSize/2), (y-((bigIconSize-smallIconSize)/2)) + yOffset, bigIconSize, bigIconSize, force_icons36[showPowers[cg.forcepowerSelect]] );
		}
	}
	// UNUSED 	
	// ARTIFACTS 
	else if ( player->client->NPC_class == CLASS_R2D2 ||
		player->client->NPC_class == CLASS_R5D2 ||
		player->client->NPC_class == CLASS_SEEKER ||
		player->client->NPC_class == CLASS_REMOTE )		
 	{
		if (force_icons[showPowers[i]])
		{
			CG_DrawPic( x-(bigIconSize/2), (y-((bigIconSize-smallIconSize)/2)) + yOffset, bigIconSize, bigIconSize, force_icons[showPowers[cg.forcepowerSelect]] );
		}
	}
	else if ( player->client->NPC_class == CLASS_ELDERGOD ||
		player->client->NPC_class == CLASS_SAND_CREATURE || 
		player->client->NPC_class == CLASS_TENTACLE )
 	{
		if (force_icons[showPowers[i]])
		{
			CG_DrawPic( x-(bigIconSize/2), (y-((bigIconSize-smallIconSize)/2)) + yOffset, bigIconSize, bigIconSize, force_icons[showPowers[cg.forcepowerSelect]] );
		}
	}
	else // REAVER, JEDI, (TIZIEL NOVIZIO), DEFAULT ICONS CLASS 
		// Ariel, Sluagh, reaper, dreadnought, ghost, java, vampire ghost , mouse 
	{
		if (force_icons[showPowers[i]])
		{
			CG_DrawPic( x-(bigIconSize/2), (y-((bigIconSize-smallIconSize)/2)) + yOffset, bigIconSize, bigIconSize, force_icons[showPowers[cg.forcepowerSelect]] );
		}
	}

	

	/*
	if (force_icons[showPowers[cg.forcepowerSelect]])
	{
		CG_DrawPic( x-(bigIconSize/2), (y-((bigIconSize-smallIconSize)/2)) + yOffset, bigIconSize, bigIconSize, force_icons[showPowers[cg.forcepowerSelect]] );
	}*/


	i = cg.forcepowerSelect + 1;
	if (i>=MAX_SHOWPOWERS)
	{
		i = 0;
	}

	// Work forwards from current icon
	holdX = x + (bigIconSize/2) + pad;
	for (iconCnt=1;iconCnt<(sideRightIconCnt+1);i++)
	{
		if (i>=MAX_SHOWPOWERS)
		{
			i = 0;
		}

		if (!ForcePower_Valid(i))	// Does he have this power?
		{
			continue;
		}

		++iconCnt;					// Good icon


if ( player->client->NPC_class == CLASS_DARKREAVER ||
		player->client->NPC_class == CLASS_SABOTEUR ||
		player->client->NPC_class == CLASS_DARK ||
		player->client->NPC_class == CLASS_GOLEM_DARK )
	{
		if (force_icons[showPowers[i]])
		{
			CG_DrawPic( holdX, y + yOffset, smallIconSize, smallIconSize, force_icons1[showPowers[i]] ); 
			holdX += (smallIconSize+pad);
		}
	}
	else if ( player->client->NPC_class == CLASS_TANKREAVER ||
	player->client->NPC_class == CLASS_BESPIN_COP ||
	player->client->NPC_class == CLASS_GOLEM_SMOKE )
	{
		if (force_icons[showPowers[i]])
		{
			CG_DrawPic( holdX, y + yOffset, smallIconSize, smallIconSize, force_icons2[showPowers[i]] ); 
			holdX += (smallIconSize+pad);
		}
	}
	else if ( player->client->NPC_class == CLASS_SHADOWTROOPER ||
	player->client->NPC_class == CLASS_VOID || 
	player->client->NPC_class == CLASS_GOLEM_VOID )
	{
		if (force_icons[showPowers[i]])
		{
			CG_DrawPic( holdX, y + yOffset, smallIconSize, smallIconSize, force_icons3[showPowers[i]] ); 
			holdX += (smallIconSize+pad);
		}
	}
	else if ( player->client->NPC_class == CLASS_LIGHTREAVER ||
		player->client->NPC_class == CLASS_SABER_DROID ||
		player->client->NPC_class == CLASS_ASSASSIN_DROID ||
		player->client->NPC_class == CLASS_LIGHT ||
		player->client->NPC_class == CLASS_GOLEM_LIGHT )
	{
		if (force_icons[showPowers[i]])
		{
			CG_DrawPic( holdX, y + yOffset, smallIconSize, smallIconSize, force_icons4[showPowers[i]] ); 
			holdX += (smallIconSize+pad);
		}
	}
	else if ( player->client->NPC_class == CLASS_MORGANKATARN ||
		player->client->NPC_class == CLASS_GOLEM_GOLD ||
		player->client->NPC_class == CLASS_PROTOCOL ||
		player->client->NPC_class == CLASS_ATST )
	{
		if (force_icons[showPowers[i]])
		{
			CG_DrawPic( holdX, y + yOffset, smallIconSize, smallIconSize, force_icons5[showPowers[i]] ); 
			holdX += (smallIconSize+pad);
		}
	}
	else if ( player->client->NPC_class == CLASS_FIREREAVER || 
		player->client->NPC_class == CLASS_GOLEM_FIRE || 
		player->client->NPC_class == CLASS_FIRE )
	{
		if (force_icons[showPowers[i]])
		{
			CG_DrawPic( holdX, y + yOffset, smallIconSize, smallIconSize, force_icons6[showPowers[i]] ); 
			holdX += (smallIconSize+pad);
		}
	}
	else if ( player->client->NPC_class == CLASS_CLAW || player->client->NPC_class == CLASS_CLAW || player->client->NPC_class == CLASS_GOLEM_LAVA )
					
	{
		if (force_icons[showPowers[i]])
		{
			CG_DrawPic( holdX, y + yOffset, smallIconSize, smallIconSize, force_icons7[showPowers[i]] ); 
			holdX += (smallIconSize+pad);
		}
	}
	else if ( player->client->NPC_class == CLASS_REBEL || 
		player->client->NPC_class == CLASS_NAERYANMIST ||
				player->client->NPC_class == CLASS_PROBE || 
				player->client->NPC_class == CLASS_AIR ||
				player->client->NPC_class == CLASS_GOLEM_AIR ||
				player->client->NPC_class == CLASS_BIRD 	)
	{
		if (force_icons[showPowers[i]])
		{
			CG_DrawPic( holdX, y + yOffset, smallIconSize, smallIconSize, force_icons8[showPowers[i]] ); 
			holdX += (smallIconSize+pad);
		}
	}
	else if ( player->client->NPC_class == CLASS_AIRREAVER || player->client->NPC_class == CLASS_POLTER ||
		player->client->NPC_class == CLASS_NAERYANMIST2 )
	{
		if (force_icons[showPowers[i]])
		{
			CG_DrawPic( holdX, y + yOffset, smallIconSize, smallIconSize, force_icons9[showPowers[i]] ); 
			holdX += (smallIconSize+pad);
		}
	}
	else if ( player->client->NPC_class == CLASS_ROCKETTROOPER ||
		player->client->NPC_class == CLASS_NAERYANMIST3 ||
			 player->client->NPC_class == CLASS_GOLEM_METAL ||
			  player->client->NPC_class == CLASS_DEMON_ELECTRO ||
			   player->client->NPC_class == CLASS_BOBAFETT )
	{
		if (force_icons[showPowers[i]])
		{
			CG_DrawPic( holdX, y + yOffset, smallIconSize, smallIconSize, force_icons10[showPowers[i]] ); 
			holdX += (smallIconSize+pad);
		}
	}
	// Sonic Class (Not sure for who use it) 
	else if ( player->client->NPC_class == CLASS_VAMPIRE_SOUND ||
		player->client->NPC_class == CLASS_SENTRY ||
		player->client->NPC_class == CLASS_HOWLER ||
		player->client->NPC_class == CLASS_SONIC ||
		player->client->NPC_class == CLASS_GOLEM_SONIC )
 	{
		if (force_icons[showPowers[i]])
		{
			CG_DrawPic( holdX, y + yOffset, smallIconSize, smallIconSize, force_icons11[showPowers[i]] ); 
			holdX += (smallIconSize+pad);
		}
	}
	else if ( player->client->NPC_class == CLASS_BARTENDER || 
		player->client->NPC_class == CLASS_ICEREAVER ||
		player->client->NPC_class == CLASS_GOLEM_ICE )
	{
		if (force_icons[showPowers[i]])
		{
			CG_DrawPic( holdX, y + yOffset, smallIconSize, smallIconSize, force_icons12[showPowers[i]] ); 
			holdX += (smallIconSize+pad);
		}
	}
	else if ( player->client->NPC_class == CLASS_CRYSTAL ||
		player->client->NPC_class == CLASS_GALAKMECH )
	{
		if (force_icons[showPowers[i]])
		{
			CG_DrawPic( holdX, y + yOffset, smallIconSize, smallIconSize, force_icons13[showPowers[i]] ); 
			holdX += (smallIconSize+pad);
		}
	}
	else if ( player->client->NPC_class == CLASS_SPIDERREAVER )
	{
		if (force_icons[showPowers[i]])
		{
			CG_DrawPic( holdX, y + yOffset, smallIconSize, smallIconSize, force_icons14[showPowers[i]] ); 
			holdX += (smallIconSize+pad);
		}
	}
	else if ( player->client->NPC_class == CLASS_SPIDERREAVER2 || player->client->NPC_class == CLASS_UGNAUGHT )
	{
		if (force_icons[showPowers[i]])
		{
			CG_DrawPic( holdX, y + yOffset, smallIconSize, smallIconSize, force_icons15[showPowers[i]] ); 
			holdX += (smallIconSize+pad);
		}
	}
	else if ( player->client->NPC_class == CLASS_EARTHREAVER || 
				player->client->NPC_class == CLASS_LIZARD || 
				player->client->NPC_class == CLASS_EARTH || 
				player->client->NPC_class == CLASS_GONK ||
				player->client->NPC_class == CLASS_DEMON_BLADE || 
				player->client->NPC_class == CLASS_GOLEM )
	{
		if (force_icons[showPowers[i]])
		{
			CG_DrawPic( holdX, y + yOffset, smallIconSize, smallIconSize, force_icons16[showPowers[i]] ); 
			holdX += (smallIconSize+pad);
		}
	}
	else if ( player->client->NPC_class == CLASS_SPIDERREAVER3 || player->client->NPC_class == CLASS_REELO )
	{
		if (force_icons[showPowers[i]])
		{
			CG_DrawPic( holdX, y + yOffset, smallIconSize, smallIconSize, force_icons17[showPowers[i]] ); 
			holdX += (smallIconSize+pad);
		}
	}
	else if ( player->client->NPC_class == CLASS_NECROREAVER )
	{
		if (force_icons[showPowers[i]])
		{
			CG_DrawPic( holdX, y + yOffset, smallIconSize, smallIconSize, force_icons18[showPowers[i]] ); 
			holdX += (smallIconSize+pad);
		}
	}
	else if ( player->client->NPC_class == CLASS_MURJJ ||
		player->client->NPC_class == CLASS_FISH || 
		player->client->NPC_class == CLASS_NECRO )
	{
		if (force_icons[showPowers[i]])
		{
			CG_DrawPic( holdX, y + yOffset, smallIconSize, smallIconSize, force_icons19[showPowers[i]] ); 
			holdX += (smallIconSize+pad);
		}
	}
	else if ( player->client->NPC_class == CLASS_RAZIEL ||
		player->client->NPC_class == CLASS_MENTALIST )
	{
		if (force_icons[showPowers[i]])
		{
			CG_DrawPic( holdX, y + yOffset, smallIconSize, smallIconSize, force_icons20[showPowers[i]] ); 
			holdX += (smallIconSize+pad);
		}
	}
	else if ( player->client->NPC_class == CLASS_HOLY ||
		player->client->NPC_class == CLASS_GOLEM_HOLY )
	{
		if (force_icons[showPowers[i]])
		{
			CG_DrawPic( holdX, y + yOffset, smallIconSize, smallIconSize, force_icons21[showPowers[i]] ); 
			holdX += (smallIconSize+pad);
		}
	}
	else if ( player->client->NPC_class == CLASS_LUKE ) 
	{
		if (force_icons[showPowers[i]])
		{
			CG_DrawPic( holdX, y + yOffset, smallIconSize, smallIconSize, force_icons22[showPowers[i]] ); 
			holdX += (smallIconSize+pad);
		}
	}
	else if ( player->client->NPC_class == CLASS_DRUIDREAVER ||
		player->client->NPC_class == CLASS_TRANDOSHAN ||
		player->client->NPC_class == CLASS_LYCAN ||
		player->client->NPC_class == CLASS_DRUID ) 
	{
		if (force_icons[showPowers[i]])
		{
			CG_DrawPic( holdX, y + yOffset, smallIconSize, smallIconSize, force_icons23[showPowers[i]] ); 
			holdX += (smallIconSize+pad);
		}
	}
	else if ( player->client->NPC_class == CLASS_LANDO ) 
	{
		if (force_icons[showPowers[i]])
		{
			CG_DrawPic( holdX, y + yOffset, smallIconSize, smallIconSize, force_icons24[showPowers[i]] ); 
			holdX += (smallIconSize+pad);
		}
	}
	else if ( player->client->NPC_class == CLASS_VAMPIRE ||
		player->client->NPC_class == CLASS_REBORN ||	
		player->client->NPC_class == CLASS_KAIN ||
		player->client->NPC_class == CLASS_KAIN2 ||
		player->client->NPC_class == CLASS_UMAH ||
		player->client->NPC_class == CLASS_VAMPIRE_DARK ||
		player->client->NPC_class == CLASS_VAMPIRE_NECRO ||
		player->client->NPC_class == CLASS_VAMPIRE_SARAFAN ||
		player->client->NPC_class == CLASS_HALFVAMPIRE ||
		player->client->NPC_class == CLASS_RAZIELHIM ||
		player->client->NPC_class == CLASS_MONMOTHA ||		
		player->client->NPC_class == CLASS_VAMPIRE_ELF ||
		player->client->NPC_class == CLASS_ALORA ||
		player->client->NPC_class == CLASS_VAMPIRE_WATER ||
		player->client->NPC_class == CLASS_VAMPIRE_LYCAN ||
		player->client->NPC_class == CLASS_VAMPIRE_ARCHER ||
		player->client->NPC_class == CLASS_VAMPIRE_BOUNTY ||
		player->client->NPC_class == CLASS_VAMPIRE_BLOOD ||
		player->client->NPC_class == CLASS_DUMAHIM ||
		player->client->NPC_class == CLASS_WEEQUAY ||
		player->client->NPC_class == CLASS_GRAN ||
		player->client->NPC_class == CLASS_TURELIM ) 
	{
		if (force_icons[showPowers[i]])
		{
			CG_DrawPic( holdX, y + yOffset, smallIconSize, smallIconSize, force_icons25[showPowers[i]] ); 
			holdX += (smallIconSize+pad);
		}
	}
	else if ( player->client->NPC_class == CLASS_VAMPIRE_FEAR || player->client->NPC_class == CLASS_VAMPIRE_FEAR2 )
	{
		if (force_icons[showPowers[i]])
		{
			CG_DrawPic( holdX, y + yOffset, smallIconSize, smallIconSize, force_icons26[showPowers[i]] ); 
			holdX += (smallIconSize+pad);
		}
	}
	else if ( player->client->NPC_class == CLASS_KAIN3 ||
		player->client->NPC_class == CLASS_KYLE ||
		player->client->NPC_class == CLASS_NECROMANCER ||
		player->client->NPC_class == CLASS_VAMPIRE_QUEEN ||
		player->client->NPC_class == CLASS_VAMPIRE_FROST ||
		player->client->NPC_class == CLASS_VORADOR ||
		player->client->NPC_class == CLASS_JANOS ||
		player->client->NPC_class == CLASS_VAMPIRE_NECRO2 )
	{
		if (force_icons[showPowers[i]])
		{
			CG_DrawPic( holdX, y + yOffset, smallIconSize, smallIconSize, force_icons27[showPowers[i]] ); 
			holdX += (smallIconSize+pad);
		}
	}
	else if (  player->client->NPC_class == CLASS_SWAMP || player->client->NPC_class == CLASS_RAHABIM ||
						player->client->NPC_class == CLASS_SHARK || player->client->NPC_class == CLASS_SWAMPTROOPER || 
						player->client->NPC_class == CLASS_WATER || player->client->NPC_class == CLASS_GOLEM_WATER )
	{
		if (force_icons[showPowers[i]])
		{
			CG_DrawPic( holdX, y + yOffset, smallIconSize, smallIconSize, force_icons28[showPowers[i]] ); 
			holdX += (smallIconSize+pad);
		}
	}
	else if ( player->client->NPC_class == CLASS_VAMPIRE_ZOMBIE || player->client->NPC_class == CLASS_TUSKEN || 
		player->client->NPC_class == CLASS_MELC || player->client->NPC_class == CLASS_SUMMONER || 
		player->client->NPC_class == CLASS_THRALL || 	player->client->NPC_class == CLASS_GLIDER || 
		player->client->NPC_class == CLASS_COMMANDO || player->client->NPC_class == CLASS_GOLEM_NECRO )
	{
		if (force_icons[showPowers[i]])
		{
			CG_DrawPic( holdX, y + yOffset, smallIconSize, smallIconSize, force_icons29[showPowers[i]] ); 
			holdX += (smallIconSize+pad);
		}
	}
	else if ( player->client->NPC_class == CLASS_VAMPIRE_DEMON ||
		player->client->NPC_class == CLASS_WAMPA || 
		player->client->NPC_class == CLASS_DEMON_FIRE || 
		player->client->NPC_class == CLASS_CENOBITA || 
		player->client->NPC_class == CLASS_HELLGOLEM || 
		player->client->NPC_class == CLASS_HAZARD_TROOPER || 
		player->client->NPC_class == CLASS_IMPERIAL)
	{
		if (force_icons[showPowers[i]])
		{
			CG_DrawPic( holdX, y + yOffset, smallIconSize, smallIconSize, force_icons30[showPowers[i]] ); 
			holdX += (smallIconSize+pad);
		}
	}
	else if ( player->client->NPC_class == CLASS_WOLFFORM ||
		 player->client->NPC_class == CLASS_WOLFKAIN ||
		 player->client->NPC_class == CLASS_WOLFKAIN2 ||
		 player->client->NPC_class == CLASS_WOLFKAIN3 ||
		 player->client->NPC_class == CLASS_WOLFVORADOR ||
		 player->client->NPC_class == CLASS_WOLFJANOS || 
		 player->client->NPC_class == CLASS_WOLFNECROMANCER ||
		 player->client->NPC_class == CLASS_WOLFVAMPIRE_QUEEN ||
		 player->client->NPC_class == CLASS_WOLFVAMPIRE_FEAR ||
		 player->client->NPC_class == CLASS_WOLFVAMPIRE_FEAR2 ||
		 player->client->NPC_class == CLASS_HELLWOLF ||
		 player->client->NPC_class == CLASS_WOLFVAMPIRE_LYCAN ||
		 player->client->NPC_class == CLASS_WOLFVAMPIRE_ELF ||
		 player->client->NPC_class == CLASS_LIONVAMPIRE_ELF ||
		 player->client->NPC_class == CLASS_LIONVAMPIRE_ELF2 || 
		 player->client->NPC_class == CLASS_WOLFHALFVAMPIRE ||
		 player->client->NPC_class == CLASS_ESKANDOR || 
		 // BAT FORM  
		 player->client->NPC_class == CLASS_BATFORM ||
		 player->client->NPC_class == CLASS_BATKAIN ||
		 player->client->NPC_class == CLASS_BATKAIN2 ||
		 player->client->NPC_class == CLASS_BATKAIN3 ||
		 player->client->NPC_class == CLASS_BATVORADOR ||
		 player->client->NPC_class == CLASS_BATVAMPIRE_FEAR ||
		 player->client->NPC_class == CLASS_BATVAMPIRE_FEAR2 || 
		 player->client->NPC_class == CLASS_BATVAMPIRE_QUEEN ||
		 player->client->NPC_class == CLASS_BATNECROMANCER ||
		 player->client->NPC_class == CLASS_BATVAMPIRE_FROST ||
		player->client->NPC_class == CLASS_HELLBAT || 
		 player->client->NPC_class == CLASS_BATVAMPIRE_WATER ||
		 player->client->NPC_class == CLASS_BATVAMPIRE_ELF ||
		 player->client->NPC_class == CLASS_BATVAMPIRE_ELF2 ||
		 player->client->NPC_class == CLASS_BATVAMPIRE_ELF3 ||
	// MIST FORM 
		player->client->NPC_class == CLASS_MISTFORM || 
		player->client->NPC_class == CLASS_MISTKAIN ||
		 player->client->NPC_class == CLASS_MISTKAIN2 ||
		 player->client->NPC_class == CLASS_MISTKAIN3 ||
		 player->client->NPC_class == CLASS_MISTVORADOR ||
		 player->client->NPC_class == CLASS_MISTJANOS || 
		 player->client->NPC_class == CLASS_MISTNECROMANCER ||
		 player->client->NPC_class == CLASS_MISTVAMPIRE_QUEEN ||
		 player->client->NPC_class == CLASS_MISTVAMPIRE_FEAR ||
		 player->client->NPC_class == CLASS_MISTVAMPIRE_FEAR2 ||
		 player->client->NPC_class == CLASS_HELLMIST ||
		 player->client->NPC_class == CLASS_MISTVAMPIRE_DARK ||
		 player->client->NPC_class == CLASS_MISTVAMPIRE_BLOOD ||
		 player->client->NPC_class == CLASS_MISTVAMPIRE_WATER ||
		 player->client->NPC_class == CLASS_MISTVAMPIRE_SARAFAN ||
		 player->client->NPC_class == CLASS_MISTVAMPIRE_ELF ||
		 player->client->NPC_class == CLASS_MISTVAMPIRE_ELF2 ||
		 player->client->NPC_class == CLASS_MISTVAMPIRE_ELF3 )
	{
		if (force_icons[showPowers[i]])
		{
			CG_DrawPic( holdX, y + yOffset, smallIconSize, smallIconSize, force_icons31[showPowers[i]] ); 
			holdX += (smallIconSize+pad);
		}
	}
	// MINEMONSTER - Shade class?
	else if ( player->client->NPC_class == CLASS_MINEMONSTER || 
	  player->client->NPC_class == CLASS_POISON ||
	  player->client->NPC_class == CLASS_IVY || 
	  player->client->NPC_class == CLASS_DEMON_POISON ||
      player->client->NPC_class == CLASS_GOLEM_POISON || 
	  player->client->NPC_class == CLASS_RANCOR || 
	  player->client->NPC_class == CLASS_INTERROGATOR )
	{
		if (force_icons[showPowers[i]])
		{
			CG_DrawPic( holdX, y + yOffset, smallIconSize, smallIconSize, force_icons32[showPowers[i]] ); 
			holdX += (smallIconSize+pad);
		}
	}
	// hyldens 
	else if ( player->client->NPC_class == CLASS_TAVION ||
		player->client->NPC_class == CLASS_HYLDEN ||
		player->client->NPC_class == CLASS_DESANN ||
		player->client->NPC_class == CLASS_HYLDENLORD )
 	{
		if (force_icons[showPowers[i]])
		{
			CG_DrawPic( holdX, y + yOffset, smallIconSize, smallIconSize, force_icons33[showPowers[i]] ); 
			holdX += (smallIconSize+pad);
		}
	}
	else if ( player->client->NPC_class == CLASS_NOGHRI || player->client->NPC_class == CLASS_ZEPH ||
		player->client->NPC_class == CLASS_DECEIVER || player->client->NPC_class == CLASS_FLIER2 ) 
	{
		if (force_icons[showPowers[i]])
		{
			CG_DrawPic( holdX, y + yOffset, smallIconSize, smallIconSize, force_icons34[showPowers[i]] ); 
			holdX += (smallIconSize+pad);
		}
	}
	else if ( player->client->NPC_class == CLASS_JAN || player->client->NPC_class == CLASS_WARRIOR ||
		player->client->NPC_class == CLASS_STORMTROOPER ||player->client->NPC_class == CLASS_SARAFAN ||
		player->client->NPC_class == CLASS_PROPHET || player->client->NPC_class == CLASS_GOLEM_HOLY ||
		
		// Unused but i place however for control mind and wrack 
		player->client->NPC_class == CLASS_PRISONER || player->client->NPC_class == CLASS_RODIAN ||
		player->client->NPC_class == CLASS_ARCHER ||player->client->NPC_class == CLASS_HUNTER ||
		player->client->NPC_class == CLASS_SCOUT || player->client->NPC_class == CLASS_ALCHEMIST ||
		player->client->NPC_class == CLASS_GALAK || player->client->NPC_class == CLASS_WIZARD ||
		player->client->NPC_class == CLASS_IMPWORKER ||player->client->NPC_class == CLASS_BEAST )
	{
		if (force_icons[showPowers[i]])
		{
			CG_DrawPic( holdX, y + yOffset, smallIconSize, smallIconSize, force_icons35[showPowers[i]] ); 
			holdX += (smallIconSize+pad);
		}
	}
	else if ( player->client->NPC_class == CLASS_VAMPIRE_ELF2 ||
		player->client->NPC_class == CLASS_VAMPIRE_ELF3 )
 	{
		if (force_icons[showPowers[i]])
		{
			CG_DrawPic( holdX, y + yOffset, smallIconSize, smallIconSize, force_icons36[showPowers[i]] ); 
			holdX += (smallIconSize+pad);
		}
	}
	// UNUSED 	
	// ARTIFACTS 
	else if ( player->client->NPC_class == CLASS_R2D2 ||
		player->client->NPC_class == CLASS_R5D2 ||
		player->client->NPC_class == CLASS_SEEKER ||
		player->client->NPC_class == CLASS_REMOTE )		
 	{
		if (force_icons[showPowers[i]])
		{
			CG_DrawPic( holdX, y + yOffset, smallIconSize, smallIconSize, force_icons[showPowers[i]] ); 
			holdX += (smallIconSize+pad);
		}
	}
	else if ( player->client->NPC_class == CLASS_ELDERGOD ||
		player->client->NPC_class == CLASS_SAND_CREATURE || 
		player->client->NPC_class == CLASS_TENTACLE )
 	{
		if (force_icons[showPowers[i]])
		{
			CG_DrawPic( holdX, y + yOffset, smallIconSize, smallIconSize, force_icons[showPowers[i]] ); 
			holdX += (smallIconSize+pad);
		}
	}
	else // REAVER, JEDI, (TIZIEL NOVIZIO), DEFAULT ICONS CLASS 
		// Ariel, Sluagh, reaper, dreadnought, ghost, java, vampire ghost , mouse 
	{
		if (force_icons[showPowers[i]])
		{
			CG_DrawPic( holdX, y + yOffset, smallIconSize, smallIconSize, force_icons[showPowers[i]] ); 
			holdX += (smallIconSize+pad);
		}
	}	

	// ARRIVATO QUI 
		
		/*if (force_icons[showPowers[i]])
		{
			CG_DrawPic( holdX, y + yOffset, smallIconSize, smallIconSize, force_icons[showPowers[i]] );
			holdX += (smallIconSize+pad);
		}*/
	}


	// This only a temp solution. // DRAW FORCE POWER NAMES 
	if ( player->client->NPC_class == CLASS_DARKREAVER ||
		player->client->NPC_class == CLASS_SABOTEUR ||
		player->client->NPC_class == CLASS_DARK ||
		player->client->NPC_class == CLASS_GOLEM_DARK )
	{
		if (cgi_SP_GetStringTextString( showPowersName1[cg.forcepowerSelect], text, sizeof(text) ))
		{
			int w = cgi_R_Font_StrLenPixels(text, cgs.media.qhFontSmall, 1.0f);	
			int x = ( SCREEN_WIDTH - w ) / 2;
			cgi_R_Font_DrawString(x, (SCREEN_HEIGHT - 24) + yOffset, text, colorTable[CT_ICON_BLUE], cgs.media.qhFontSmall, -1, 1.0f);
		}
	}
	else if ( player->client->NPC_class == CLASS_TANKREAVER ||
	player->client->NPC_class == CLASS_BESPIN_COP ||
	player->client->NPC_class == CLASS_GOLEM_SMOKE )
	{
		if (cgi_SP_GetStringTextString( showPowersName2[cg.forcepowerSelect], text, sizeof(text) ))
		{
			int w = cgi_R_Font_StrLenPixels(text, cgs.media.qhFontSmall, 1.0f);	
			int x = ( SCREEN_WIDTH - w ) / 2;
			cgi_R_Font_DrawString(x, (SCREEN_HEIGHT - 24) + yOffset, text, colorTable[CT_ICON_BLUE], cgs.media.qhFontSmall, -1, 1.0f);
		}
	}
	else if ( player->client->NPC_class == CLASS_SHADOWTROOPER ||
	player->client->NPC_class == CLASS_VOID || 
	player->client->NPC_class == CLASS_GOLEM_VOID )
	{
		if (cgi_SP_GetStringTextString( showPowersName3[cg.forcepowerSelect], text, sizeof(text) ))
		{
			int w = cgi_R_Font_StrLenPixels(text, cgs.media.qhFontSmall, 1.0f);	
			int x = ( SCREEN_WIDTH - w ) / 2;
			cgi_R_Font_DrawString(x, (SCREEN_HEIGHT - 24) + yOffset, text, colorTable[CT_ICON_BLUE], cgs.media.qhFontSmall, -1, 1.0f);
		}
	}
	else if ( player->client->NPC_class == CLASS_LIGHTREAVER ||
		player->client->NPC_class == CLASS_SABER_DROID ||
		player->client->NPC_class == CLASS_ASSASSIN_DROID ||
		player->client->NPC_class == CLASS_LIGHT ||
		player->client->NPC_class == CLASS_GOLEM_LIGHT )
	{
		if (cgi_SP_GetStringTextString( showPowersName4[cg.forcepowerSelect], text, sizeof(text) ))
		{
			int w = cgi_R_Font_StrLenPixels(text, cgs.media.qhFontSmall, 1.0f);	
			int x = ( SCREEN_WIDTH - w ) / 2;
			cgi_R_Font_DrawString(x, (SCREEN_HEIGHT - 24) + yOffset, text, colorTable[CT_ICON_BLUE], cgs.media.qhFontSmall, -1, 1.0f);
		}
	}
	else if ( player->client->NPC_class == CLASS_MORGANKATARN ||
		player->client->NPC_class == CLASS_GOLEM_GOLD ||
		player->client->NPC_class == CLASS_PROTOCOL ||
		player->client->NPC_class == CLASS_ATST )
	{
		if (cgi_SP_GetStringTextString( showPowersName5[cg.forcepowerSelect], text, sizeof(text) ))
		{
			int w = cgi_R_Font_StrLenPixels(text, cgs.media.qhFontSmall, 1.0f);	
			int x = ( SCREEN_WIDTH - w ) / 2;
			cgi_R_Font_DrawString(x, (SCREEN_HEIGHT - 24) + yOffset, text, colorTable[CT_ICON_BLUE], cgs.media.qhFontSmall, -1, 1.0f);
		}
	}
	else if ( player->client->NPC_class == CLASS_FIREREAVER || 
		player->client->NPC_class == CLASS_GOLEM_FIRE || 
		player->client->NPC_class == CLASS_FIRE )
	{
		if (cgi_SP_GetStringTextString( showPowersName6[cg.forcepowerSelect], text, sizeof(text) ))
		{
			int w = cgi_R_Font_StrLenPixels(text, cgs.media.qhFontSmall, 1.0f);	
			int x = ( SCREEN_WIDTH - w ) / 2;
			cgi_R_Font_DrawString(x, (SCREEN_HEIGHT - 24) + yOffset, text, colorTable[CT_ICON_BLUE], cgs.media.qhFontSmall, -1, 1.0f);
		}
	}
	else if ( player->client->NPC_class == CLASS_CLAW || player->client->NPC_class == CLASS_CLAW || player->client->NPC_class == CLASS_GOLEM_LAVA )
					
	{
		if (cgi_SP_GetStringTextString( showPowersName7[cg.forcepowerSelect], text, sizeof(text) ))
		{
			int w = cgi_R_Font_StrLenPixels(text, cgs.media.qhFontSmall, 1.0f);	
			int x = ( SCREEN_WIDTH - w ) / 2;
			cgi_R_Font_DrawString(x, (SCREEN_HEIGHT - 24) + yOffset, text, colorTable[CT_ICON_BLUE], cgs.media.qhFontSmall, -1, 1.0f);
		}
	}
	else if ( player->client->NPC_class == CLASS_REBEL || 
		player->client->NPC_class == CLASS_NAERYANMIST ||
				player->client->NPC_class == CLASS_PROBE || 
				player->client->NPC_class == CLASS_AIR ||
				player->client->NPC_class == CLASS_GOLEM_AIR ||
				player->client->NPC_class == CLASS_BIRD 	)
	{
		if (cgi_SP_GetStringTextString( showPowersName8[cg.forcepowerSelect], text, sizeof(text) ))
		{
			int w = cgi_R_Font_StrLenPixels(text, cgs.media.qhFontSmall, 1.0f);	
			int x = ( SCREEN_WIDTH - w ) / 2;
			cgi_R_Font_DrawString(x, (SCREEN_HEIGHT - 24) + yOffset, text, colorTable[CT_ICON_BLUE], cgs.media.qhFontSmall, -1, 1.0f);
		}
	}
	else if ( player->client->NPC_class == CLASS_AIRREAVER || player->client->NPC_class == CLASS_POLTER ||
		player->client->NPC_class == CLASS_NAERYANMIST2 )
	{
		if (cgi_SP_GetStringTextString( showPowersName9[cg.forcepowerSelect], text, sizeof(text) ))
		{
			int w = cgi_R_Font_StrLenPixels(text, cgs.media.qhFontSmall, 1.0f);	
			int x = ( SCREEN_WIDTH - w ) / 2;
			cgi_R_Font_DrawString(x, (SCREEN_HEIGHT - 24) + yOffset, text, colorTable[CT_ICON_BLUE], cgs.media.qhFontSmall, -1, 1.0f);
		}
	}
	else if ( player->client->NPC_class == CLASS_ROCKETTROOPER ||
			player->client->NPC_class == CLASS_NAERYANMIST3 ||
			 player->client->NPC_class == CLASS_GOLEM_METAL ||
			  player->client->NPC_class == CLASS_DEMON_ELECTRO ||
			   player->client->NPC_class == CLASS_BOBAFETT )
	{
		if (cgi_SP_GetStringTextString( showPowersName10[cg.forcepowerSelect], text, sizeof(text) ))
		{
			int w = cgi_R_Font_StrLenPixels(text, cgs.media.qhFontSmall, 1.0f);	
			int x = ( SCREEN_WIDTH - w ) / 2;
			cgi_R_Font_DrawString(x, (SCREEN_HEIGHT - 24) + yOffset, text, colorTable[CT_ICON_BLUE], cgs.media.qhFontSmall, -1, 1.0f);
		}
	}
	// Sonic Class (Not sure for who use it) 
	else if ( player->client->NPC_class == CLASS_VAMPIRE_SOUND ||
		player->client->NPC_class == CLASS_SENTRY ||
		player->client->NPC_class == CLASS_HOWLER ||
		player->client->NPC_class == CLASS_SONIC ||
		player->client->NPC_class == CLASS_GOLEM_SONIC )
 	{
		if (cgi_SP_GetStringTextString( showPowersName11[cg.forcepowerSelect], text, sizeof(text) ))
		{
			int w = cgi_R_Font_StrLenPixels(text, cgs.media.qhFontSmall, 1.0f);	
			int x = ( SCREEN_WIDTH - w ) / 2;
			cgi_R_Font_DrawString(x, (SCREEN_HEIGHT - 24) + yOffset, text, colorTable[CT_ICON_BLUE], cgs.media.qhFontSmall, -1, 1.0f);
		}
	}
	else if ( player->client->NPC_class == CLASS_BARTENDER || 
		player->client->NPC_class == CLASS_ICEREAVER ||
		player->client->NPC_class == CLASS_GOLEM_ICE )
	{
		if (cgi_SP_GetStringTextString( showPowersName12[cg.forcepowerSelect], text, sizeof(text) ))
		{
			int w = cgi_R_Font_StrLenPixels(text, cgs.media.qhFontSmall, 1.0f);	
			int x = ( SCREEN_WIDTH - w ) / 2;
			cgi_R_Font_DrawString(x, (SCREEN_HEIGHT - 24) + yOffset, text, colorTable[CT_ICON_BLUE], cgs.media.qhFontSmall, -1, 1.0f);
		}
	}
	else if ( player->client->NPC_class == CLASS_CRYSTAL ||
		player->client->NPC_class == CLASS_GALAKMECH )
	{
		if (cgi_SP_GetStringTextString( showPowersName13[cg.forcepowerSelect], text, sizeof(text) ))
		{
			int w = cgi_R_Font_StrLenPixels(text, cgs.media.qhFontSmall, 1.0f);	
			int x = ( SCREEN_WIDTH - w ) / 2;
			cgi_R_Font_DrawString(x, (SCREEN_HEIGHT - 24) + yOffset, text, colorTable[CT_ICON_BLUE], cgs.media.qhFontSmall, -1, 1.0f);
		}
	}
	else if ( player->client->NPC_class == CLASS_SPIDERREAVER )
	{
		if (cgi_SP_GetStringTextString( showPowersName14[cg.forcepowerSelect], text, sizeof(text) ))
		{
			int w = cgi_R_Font_StrLenPixels(text, cgs.media.qhFontSmall, 1.0f);	
			int x = ( SCREEN_WIDTH - w ) / 2;
			cgi_R_Font_DrawString(x, (SCREEN_HEIGHT - 24) + yOffset, text, colorTable[CT_ICON_BLUE], cgs.media.qhFontSmall, -1, 1.0f);
		}
	}
	else if ( player->client->NPC_class == CLASS_SPIDERREAVER2 || player->client->NPC_class == CLASS_UGNAUGHT )
	{
		if (cgi_SP_GetStringTextString( showPowersName15[cg.forcepowerSelect], text, sizeof(text) ))
		{
			int w = cgi_R_Font_StrLenPixels(text, cgs.media.qhFontSmall, 1.0f);	
			int x = ( SCREEN_WIDTH - w ) / 2;
			cgi_R_Font_DrawString(x, (SCREEN_HEIGHT - 24) + yOffset, text, colorTable[CT_ICON_BLUE], cgs.media.qhFontSmall, -1, 1.0f);
		}
	}
	else if ( player->client->NPC_class == CLASS_EARTHREAVER || 
				player->client->NPC_class == CLASS_LIZARD || 
				player->client->NPC_class == CLASS_EARTH || 
				player->client->NPC_class == CLASS_GONK ||
				player->client->NPC_class == CLASS_DEMON_BLADE || 
				player->client->NPC_class == CLASS_GOLEM )
	{
		if (cgi_SP_GetStringTextString( showPowersName16[cg.forcepowerSelect], text, sizeof(text) ))
		{
			int w = cgi_R_Font_StrLenPixels(text, cgs.media.qhFontSmall, 1.0f);	
			int x = ( SCREEN_WIDTH - w ) / 2;
			cgi_R_Font_DrawString(x, (SCREEN_HEIGHT - 24) + yOffset, text, colorTable[CT_ICON_BLUE], cgs.media.qhFontSmall, -1, 1.0f);
		}
	}
	else if ( player->client->NPC_class == CLASS_SPIDERREAVER3 || player->client->NPC_class == CLASS_REELO )
	{
		if (cgi_SP_GetStringTextString( showPowersName17[cg.forcepowerSelect], text, sizeof(text) ))
		{
			int w = cgi_R_Font_StrLenPixels(text, cgs.media.qhFontSmall, 1.0f);	
			int x = ( SCREEN_WIDTH - w ) / 2;
			cgi_R_Font_DrawString(x, (SCREEN_HEIGHT - 24) + yOffset, text, colorTable[CT_ICON_BLUE], cgs.media.qhFontSmall, -1, 1.0f);
		}
	}
	else if ( player->client->NPC_class == CLASS_NECROREAVER )
	{
		if (cgi_SP_GetStringTextString( showPowersName18[cg.forcepowerSelect], text, sizeof(text) ))
		{
			int w = cgi_R_Font_StrLenPixels(text, cgs.media.qhFontSmall, 1.0f);	
			int x = ( SCREEN_WIDTH - w ) / 2;
			cgi_R_Font_DrawString(x, (SCREEN_HEIGHT - 24) + yOffset, text, colorTable[CT_ICON_BLUE], cgs.media.qhFontSmall, -1, 1.0f);
		}
	}
	else if ( player->client->NPC_class == CLASS_MURJJ ||
		player->client->NPC_class == CLASS_FISH || 
		player->client->NPC_class == CLASS_NECRO )
	{
		if (cgi_SP_GetStringTextString( showPowersName19[cg.forcepowerSelect], text, sizeof(text) ))
		{
			int w = cgi_R_Font_StrLenPixels(text, cgs.media.qhFontSmall, 1.0f);	
			int x = ( SCREEN_WIDTH - w ) / 2;
			cgi_R_Font_DrawString(x, (SCREEN_HEIGHT - 24) + yOffset, text, colorTable[CT_ICON_BLUE], cgs.media.qhFontSmall, -1, 1.0f);
		}
	}
	else if ( player->client->NPC_class == CLASS_RAZIEL ||
		player->client->NPC_class == CLASS_MENTALIST )
	{
		if (cgi_SP_GetStringTextString( showPowersName20[cg.forcepowerSelect], text, sizeof(text) ))
		{
			int w = cgi_R_Font_StrLenPixels(text, cgs.media.qhFontSmall, 1.0f);	
			int x = ( SCREEN_WIDTH - w ) / 2;
			cgi_R_Font_DrawString(x, (SCREEN_HEIGHT - 24) + yOffset, text, colorTable[CT_ICON_BLUE], cgs.media.qhFontSmall, -1, 1.0f);
		}
	}
	else if ( player->client->NPC_class == CLASS_HOLY ||
		player->client->NPC_class == CLASS_GOLEM_HOLY )
	{
		if (cgi_SP_GetStringTextString( showPowersName21[cg.forcepowerSelect], text, sizeof(text) ))
		{
			int w = cgi_R_Font_StrLenPixels(text, cgs.media.qhFontSmall, 1.0f);	
			int x = ( SCREEN_WIDTH - w ) / 2;
			cgi_R_Font_DrawString(x, (SCREEN_HEIGHT - 24) + yOffset, text, colorTable[CT_ICON_BLUE], cgs.media.qhFontSmall, -1, 1.0f);
		}
	}
	else if ( player->client->NPC_class == CLASS_LUKE ) 
	{
		if (cgi_SP_GetStringTextString( showPowersName22[cg.forcepowerSelect], text, sizeof(text) ))
		{
			int w = cgi_R_Font_StrLenPixels(text, cgs.media.qhFontSmall, 1.0f);	
			int x = ( SCREEN_WIDTH - w ) / 2;
			cgi_R_Font_DrawString(x, (SCREEN_HEIGHT - 24) + yOffset, text, colorTable[CT_ICON_BLUE], cgs.media.qhFontSmall, -1, 1.0f);
		}
	}
	else if ( player->client->NPC_class == CLASS_DRUIDREAVER ||
		player->client->NPC_class == CLASS_TRANDOSHAN ||
		player->client->NPC_class == CLASS_LYCAN ||
		player->client->NPC_class == CLASS_DRUID ) 
	{
		if (cgi_SP_GetStringTextString( showPowersName23[cg.forcepowerSelect], text, sizeof(text) ))
		{
			int w = cgi_R_Font_StrLenPixels(text, cgs.media.qhFontSmall, 1.0f);	
			int x = ( SCREEN_WIDTH - w ) / 2;
			cgi_R_Font_DrawString(x, (SCREEN_HEIGHT - 24) + yOffset, text, colorTable[CT_ICON_BLUE], cgs.media.qhFontSmall, -1, 1.0f);
		}
	}
	else if ( player->client->NPC_class == CLASS_LANDO ) 
	{
		if (cgi_SP_GetStringTextString( showPowersName24[cg.forcepowerSelect], text, sizeof(text) ))
		{
			int w = cgi_R_Font_StrLenPixels(text, cgs.media.qhFontSmall, 1.0f);	
			int x = ( SCREEN_WIDTH - w ) / 2;
			cgi_R_Font_DrawString(x, (SCREEN_HEIGHT - 24) + yOffset, text, colorTable[CT_ICON_BLUE], cgs.media.qhFontSmall, -1, 1.0f);
		}
	}
	else if ( player->client->NPC_class == CLASS_VAMPIRE ||
		player->client->NPC_class == CLASS_REBORN ||	
		player->client->NPC_class == CLASS_KAIN ||
		player->client->NPC_class == CLASS_KAIN2 ||
		player->client->NPC_class == CLASS_UMAH ||
		player->client->NPC_class == CLASS_VAMPIRE_DARK ||
		player->client->NPC_class == CLASS_VAMPIRE_NECRO ||
		player->client->NPC_class == CLASS_VAMPIRE_SARAFAN ||
		player->client->NPC_class == CLASS_HALFVAMPIRE ||
		player->client->NPC_class == CLASS_RAZIELHIM ||
		player->client->NPC_class == CLASS_MONMOTHA ||		
		player->client->NPC_class == CLASS_VAMPIRE_ELF ||
		player->client->NPC_class == CLASS_ALORA ||
		player->client->NPC_class == CLASS_VAMPIRE_WATER ||
		player->client->NPC_class == CLASS_VAMPIRE_LYCAN ||
		player->client->NPC_class == CLASS_VAMPIRE_ARCHER ||
		player->client->NPC_class == CLASS_VAMPIRE_BOUNTY ||
		player->client->NPC_class == CLASS_VAMPIRE_BLOOD ||
		player->client->NPC_class == CLASS_DUMAHIM ||
		player->client->NPC_class == CLASS_WEEQUAY ||
		player->client->NPC_class == CLASS_GRAN ||
		player->client->NPC_class == CLASS_TURELIM ) 
	{
		if (cgi_SP_GetStringTextString( showPowersName25[cg.forcepowerSelect], text, sizeof(text) ))
		{
			int w = cgi_R_Font_StrLenPixels(text, cgs.media.qhFontSmall, 1.0f);	
			int x = ( SCREEN_WIDTH - w ) / 2;
			cgi_R_Font_DrawString(x, (SCREEN_HEIGHT - 24) + yOffset, text, colorTable[CT_ICON_BLUE], cgs.media.qhFontSmall, -1, 1.0f);
		}
	}
	else if ( player->client->NPC_class == CLASS_VAMPIRE_FEAR || player->client->NPC_class == CLASS_VAMPIRE_FEAR2 )
	{
		if (cgi_SP_GetStringTextString( showPowersName26[cg.forcepowerSelect], text, sizeof(text) ))
		{
			int w = cgi_R_Font_StrLenPixels(text, cgs.media.qhFontSmall, 1.0f);	
			int x = ( SCREEN_WIDTH - w ) / 2;
			cgi_R_Font_DrawString(x, (SCREEN_HEIGHT - 24) + yOffset, text, colorTable[CT_ICON_BLUE], cgs.media.qhFontSmall, -1, 1.0f);
		}
	}
	else if ( player->client->NPC_class == CLASS_KAIN3 ||
		player->client->NPC_class == CLASS_VAMPIRE_FROST || 
		player->client->NPC_class == CLASS_KYLE ||
		player->client->NPC_class == CLASS_NECROMANCER ||
		player->client->NPC_class == CLASS_VAMPIRE_QUEEN ||
		player->client->NPC_class == CLASS_VORADOR ||
		player->client->NPC_class == CLASS_JANOS ||
		player->client->NPC_class == CLASS_VAMPIRE_NECRO2 )
	{
		if (cgi_SP_GetStringTextString( showPowersName27[cg.forcepowerSelect], text, sizeof(text) ))
		{
			int w = cgi_R_Font_StrLenPixels(text, cgs.media.qhFontSmall, 1.0f);	
			int x = ( SCREEN_WIDTH - w ) / 2;
			cgi_R_Font_DrawString(x, (SCREEN_HEIGHT - 24) + yOffset, text, colorTable[CT_ICON_BLUE], cgs.media.qhFontSmall, -1, 1.0f);
		}
	}
	else if ( player->client->NPC_class == CLASS_SWAMP || player->client->NPC_class == CLASS_RAHABIM ||
						player->client->NPC_class == CLASS_SHARK || player->client->NPC_class == CLASS_SWAMPTROOPER || 
						player->client->NPC_class == CLASS_WATER || player->client->NPC_class == CLASS_GOLEM_WATER )
	{
		if (cgi_SP_GetStringTextString( showPowersName28[cg.forcepowerSelect], text, sizeof(text) ))
		{
			int w = cgi_R_Font_StrLenPixels(text, cgs.media.qhFontSmall, 1.0f);	
			int x = ( SCREEN_WIDTH - w ) / 2;
			cgi_R_Font_DrawString(x, (SCREEN_HEIGHT - 24) + yOffset, text, colorTable[CT_ICON_BLUE], cgs.media.qhFontSmall, -1, 1.0f);
		}
	}
	else if ( player->client->NPC_class == CLASS_VAMPIRE_ZOMBIE || player->client->NPC_class == CLASS_TUSKEN || 
		player->client->NPC_class == CLASS_MELC || player->client->NPC_class == CLASS_SUMMONER || 
		player->client->NPC_class == CLASS_THRALL || 	player->client->NPC_class == CLASS_GLIDER || 
		player->client->NPC_class == CLASS_COMMANDO || player->client->NPC_class == CLASS_GOLEM_NECRO )
	{
		if (cgi_SP_GetStringTextString( showPowersName29[cg.forcepowerSelect], text, sizeof(text) ))
		{
			int w = cgi_R_Font_StrLenPixels(text, cgs.media.qhFontSmall, 1.0f);	
			int x = ( SCREEN_WIDTH - w ) / 2;
			cgi_R_Font_DrawString(x, (SCREEN_HEIGHT - 24) + yOffset, text, colorTable[CT_ICON_BLUE], cgs.media.qhFontSmall, -1, 1.0f);
		}
	}
	else if ( player->client->NPC_class == CLASS_VAMPIRE_DEMON ||
		player->client->NPC_class == CLASS_WAMPA || 
		player->client->NPC_class == CLASS_DEMON_FIRE || 
		player->client->NPC_class == CLASS_CENOBITA || 
		player->client->NPC_class == CLASS_HELLGOLEM || 
		player->client->NPC_class == CLASS_HAZARD_TROOPER || 
		player->client->NPC_class == CLASS_IMPERIAL)
	{
		if (cgi_SP_GetStringTextString( showPowersName30[cg.forcepowerSelect], text, sizeof(text) ))
		{
			int w = cgi_R_Font_StrLenPixels(text, cgs.media.qhFontSmall, 1.0f);	
			int x = ( SCREEN_WIDTH - w ) / 2;
			cgi_R_Font_DrawString(x, (SCREEN_HEIGHT - 24) + yOffset, text, colorTable[CT_ICON_BLUE], cgs.media.qhFontSmall, -1, 1.0f);
		}
	}
	else if ( player->client->NPC_class == CLASS_WOLFFORM ||
		 player->client->NPC_class == CLASS_WOLFKAIN ||
		 player->client->NPC_class == CLASS_WOLFKAIN2 ||
		 player->client->NPC_class == CLASS_WOLFKAIN3 ||
		 player->client->NPC_class == CLASS_WOLFVORADOR ||
		 player->client->NPC_class == CLASS_WOLFJANOS || 
		 player->client->NPC_class == CLASS_WOLFNECROMANCER ||
		 player->client->NPC_class == CLASS_WOLFVAMPIRE_QUEEN ||
		 player->client->NPC_class == CLASS_WOLFVAMPIRE_FEAR ||
		 player->client->NPC_class == CLASS_WOLFVAMPIRE_FEAR2 ||
		 player->client->NPC_class == CLASS_HELLWOLF ||
		 player->client->NPC_class == CLASS_WOLFVAMPIRE_LYCAN ||
		 player->client->NPC_class == CLASS_WOLFVAMPIRE_ELF ||
		 player->client->NPC_class == CLASS_LIONVAMPIRE_ELF ||
		 player->client->NPC_class == CLASS_LIONVAMPIRE_ELF2 || 
		 player->client->NPC_class == CLASS_WOLFHALFVAMPIRE ||
		 player->client->NPC_class == CLASS_ESKANDOR || 
		 // BAT FORM  
		 player->client->NPC_class == CLASS_BATFORM ||
		 player->client->NPC_class == CLASS_BATKAIN ||
		 player->client->NPC_class == CLASS_BATKAIN2 ||
		 player->client->NPC_class == CLASS_BATKAIN3 ||
		 player->client->NPC_class == CLASS_BATVORADOR ||
		 player->client->NPC_class == CLASS_BATVAMPIRE_FEAR ||
		 player->client->NPC_class == CLASS_BATVAMPIRE_FEAR2 || 
		 player->client->NPC_class == CLASS_BATVAMPIRE_QUEEN ||
		 player->client->NPC_class == CLASS_BATNECROMANCER ||
		 player->client->NPC_class == CLASS_BATVAMPIRE_FROST ||
		player->client->NPC_class == CLASS_HELLBAT || 
		 player->client->NPC_class == CLASS_BATVAMPIRE_WATER ||
		 player->client->NPC_class == CLASS_BATVAMPIRE_ELF ||
		 player->client->NPC_class == CLASS_BATVAMPIRE_ELF2 ||
		 player->client->NPC_class == CLASS_BATVAMPIRE_ELF3 ||
	// MIST FORM 
		player->client->NPC_class == CLASS_MISTFORM || 
		player->client->NPC_class == CLASS_MISTKAIN ||
		 player->client->NPC_class == CLASS_MISTKAIN2 ||
		 player->client->NPC_class == CLASS_MISTKAIN3 ||
		 player->client->NPC_class == CLASS_MISTVORADOR ||
		 player->client->NPC_class == CLASS_MISTJANOS || 
		 player->client->NPC_class == CLASS_MISTNECROMANCER ||
		 player->client->NPC_class == CLASS_MISTVAMPIRE_QUEEN ||
		 player->client->NPC_class == CLASS_MISTVAMPIRE_FEAR ||
		 player->client->NPC_class == CLASS_MISTVAMPIRE_FEAR2 ||
		 player->client->NPC_class == CLASS_HELLMIST ||
		 player->client->NPC_class == CLASS_MISTVAMPIRE_DARK ||
		 player->client->NPC_class == CLASS_MISTVAMPIRE_BLOOD ||
		 player->client->NPC_class == CLASS_MISTVAMPIRE_WATER ||
		  player->client->NPC_class == CLASS_MISTVAMPIRE_SARAFAN ||
		 player->client->NPC_class == CLASS_MISTVAMPIRE_ELF ||
		 player->client->NPC_class == CLASS_MISTVAMPIRE_ELF2 ||
		 player->client->NPC_class == CLASS_MISTVAMPIRE_ELF3 )
	{
		if (cgi_SP_GetStringTextString( showPowersName31[cg.forcepowerSelect], text, sizeof(text) ))
		{
			int w = cgi_R_Font_StrLenPixels(text, cgs.media.qhFontSmall, 1.0f);	
			int x = ( SCREEN_WIDTH - w ) / 2;
			cgi_R_Font_DrawString(x, (SCREEN_HEIGHT - 24) + yOffset, text, colorTable[CT_ICON_BLUE], cgs.media.qhFontSmall, -1, 1.0f);
		}
	}
	// MINEMONSTER - Shade class?
	else if ( player->client->NPC_class == CLASS_MINEMONSTER || 
	  player->client->NPC_class == CLASS_POISON ||
	  player->client->NPC_class == CLASS_IVY || 
	  player->client->NPC_class == CLASS_DEMON_POISON ||
      player->client->NPC_class == CLASS_GOLEM_POISON || 
	  player->client->NPC_class == CLASS_RANCOR || 
	  player->client->NPC_class == CLASS_INTERROGATOR )
	{
		if (cgi_SP_GetStringTextString( showPowersName32[cg.forcepowerSelect], text, sizeof(text) ))
		{
			int w = cgi_R_Font_StrLenPixels(text, cgs.media.qhFontSmall, 1.0f);	
			int x = ( SCREEN_WIDTH - w ) / 2;
			cgi_R_Font_DrawString(x, (SCREEN_HEIGHT - 24) + yOffset, text, colorTable[CT_ICON_BLUE], cgs.media.qhFontSmall, -1, 1.0f);
		}
	}
	// hyldens 
	else if ( player->client->NPC_class == CLASS_TAVION ||
		player->client->NPC_class == CLASS_HYLDEN ||
		player->client->NPC_class == CLASS_DESANN ||
		player->client->NPC_class == CLASS_HYLDENLORD )
 	{
		if (cgi_SP_GetStringTextString( showPowersName33[cg.forcepowerSelect], text, sizeof(text) ))
		{
			int w = cgi_R_Font_StrLenPixels(text, cgs.media.qhFontSmall, 1.0f);	
			int x = ( SCREEN_WIDTH - w ) / 2;
			cgi_R_Font_DrawString(x, (SCREEN_HEIGHT - 24) + yOffset, text, colorTable[CT_ICON_BLUE], cgs.media.qhFontSmall, -1, 1.0f);
		}
	}
	else if ( player->client->NPC_class == CLASS_NOGHRI || player->client->NPC_class == CLASS_ZEPH ||
		player->client->NPC_class == CLASS_DECEIVER || player->client->NPC_class == CLASS_FLIER2 ) 
	{
		if (cgi_SP_GetStringTextString( showPowersName34[cg.forcepowerSelect], text, sizeof(text) ))
		{
			int w = cgi_R_Font_StrLenPixels(text, cgs.media.qhFontSmall, 1.0f);	
			int x = ( SCREEN_WIDTH - w ) / 2;
			cgi_R_Font_DrawString(x, (SCREEN_HEIGHT - 24) + yOffset, text, colorTable[CT_ICON_BLUE], cgs.media.qhFontSmall, -1, 1.0f);
		}
	}
	else if ( player->client->NPC_class == CLASS_JAN || player->client->NPC_class == CLASS_WARRIOR ||
		player->client->NPC_class == CLASS_STORMTROOPER ||player->client->NPC_class == CLASS_SARAFAN ||
		player->client->NPC_class == CLASS_PROPHET || player->client->NPC_class == CLASS_GOLEM_HOLY ||
		
		// Unused but i place however for control mind and wrack 
		player->client->NPC_class == CLASS_PRISONER || player->client->NPC_class == CLASS_RODIAN ||
		player->client->NPC_class == CLASS_ARCHER ||player->client->NPC_class == CLASS_HUNTER ||
		player->client->NPC_class == CLASS_SCOUT || player->client->NPC_class == CLASS_ALCHEMIST ||
		player->client->NPC_class == CLASS_GALAK || player->client->NPC_class == CLASS_WIZARD ||
		player->client->NPC_class == CLASS_IMPWORKER ||player->client->NPC_class == CLASS_BEAST )
	{
		if (cgi_SP_GetStringTextString( showPowersName35[cg.forcepowerSelect], text, sizeof(text) ))
		{
			int w = cgi_R_Font_StrLenPixels(text, cgs.media.qhFontSmall, 1.0f);	
			int x = ( SCREEN_WIDTH - w ) / 2;
			cgi_R_Font_DrawString(x, (SCREEN_HEIGHT - 24) + yOffset, text, colorTable[CT_ICON_BLUE], cgs.media.qhFontSmall, -1, 1.0f);
		}
	}
	else if ( player->client->NPC_class == CLASS_VAMPIRE_ELF2 ||
		player->client->NPC_class == CLASS_VAMPIRE_ELF3 )
 	{
		if (cgi_SP_GetStringTextString( showPowersName36[cg.forcepowerSelect], text, sizeof(text) ))
		{
			int w = cgi_R_Font_StrLenPixels(text, cgs.media.qhFontSmall, 1.0f);	
			int x = ( SCREEN_WIDTH - w ) / 2;
			cgi_R_Font_DrawString(x, (SCREEN_HEIGHT - 24) + yOffset, text, colorTable[CT_ICON_BLUE], cgs.media.qhFontSmall, -1, 1.0f);
		}
	}
	// UNUSED 	
	// ARTIFACTS 
	else if ( player->client->NPC_class == CLASS_R2D2 ||
		player->client->NPC_class == CLASS_R5D2 ||
		player->client->NPC_class == CLASS_SEEKER ||
		player->client->NPC_class == CLASS_REMOTE )		
 	{
		if (cgi_SP_GetStringTextString( showPowersName[cg.forcepowerSelect], text, sizeof(text) ))
		{
			int w = cgi_R_Font_StrLenPixels(text, cgs.media.qhFontSmall, 1.0f);	
			int x = ( SCREEN_WIDTH - w ) / 2;
			cgi_R_Font_DrawString(x, (SCREEN_HEIGHT - 24) + yOffset, text, colorTable[CT_ICON_BLUE], cgs.media.qhFontSmall, -1, 1.0f);
		}
	}
	else if ( player->client->NPC_class == CLASS_ELDERGOD ||
		player->client->NPC_class == CLASS_SAND_CREATURE || 
		player->client->NPC_class == CLASS_TENTACLE )
 	{
		if (cgi_SP_GetStringTextString( showPowersName[cg.forcepowerSelect], text, sizeof(text) ))
		{
			int w = cgi_R_Font_StrLenPixels(text, cgs.media.qhFontSmall, 1.0f);	
			int x = ( SCREEN_WIDTH - w ) / 2;
			cgi_R_Font_DrawString(x, (SCREEN_HEIGHT - 24) + yOffset, text, colorTable[CT_ICON_BLUE], cgs.media.qhFontSmall, -1, 1.0f);
		}
	}
	else // REAVER, JEDI, (TIZIEL NOVIZIO), DEFAULT ICONS CLASS 
		// Ariel, Sluagh, reaper, dreadnought, ghost, java, vampire ghost , mouse 
	{
		if (cgi_SP_GetStringTextString( showPowersName[cg.forcepowerSelect], text, sizeof(text) ))
		{
			int w = cgi_R_Font_StrLenPixels(text, cgs.media.qhFontSmall, 1.0f);	
			int x = ( SCREEN_WIDTH - w ) / 2;
			cgi_R_Font_DrawString(x, (SCREEN_HEIGHT - 24) + yOffset, text, colorTable[CT_ICON_BLUE], cgs.media.qhFontSmall, -1, 1.0f);
		}
	}	
}

qboolean ForcePowerDataPad_Valid(int index)
{
	gentity_t	*player = &g_entities[0];

	assert (index < MAX_DPSHOWPOWERS);
	if (player->client->ps.forcePowersKnown & (1 << showDataPadPowers[index]) && 
		player->client->ps.forcePowerLevel[showDataPadPowers[index]])	// Does he have the force power?
	{
		return qtrue;
	}

	return qfalse;
}

/*
===============
CG_DPNextForcePower_f
===============
*/
void CG_DPNextForcePower_f( void ) 
{
	int		i;
	int		original;

	if ( !cg.snap ) 
	{
		return;
	}

	original = cg.DataPadforcepowerSelect;

	for ( i = 0; i<MAX_DPSHOWPOWERS; i++ ) 
	{
		cg.DataPadforcepowerSelect++;

		if (cg.DataPadforcepowerSelect >= MAX_DPSHOWPOWERS)
		{ 
			cg.DataPadforcepowerSelect = 0; 
		}

		if (ForcePowerDataPad_Valid(cg.DataPadforcepowerSelect))	// Does he have the force power?
		{
			return;
		}
	}

	cg.DataPadforcepowerSelect = original;
}

/*
===============
CG_DPPrevForcePower_f
===============
*/
void CG_DPPrevForcePower_f( void ) 
{
	int		i;
	int		original;

	if ( !cg.snap ) 
	{
		return;
	}

	original = cg.DataPadforcepowerSelect;

	for ( i = 0; i<MAX_DPSHOWPOWERS; i++ ) 
	{
		cg.DataPadforcepowerSelect--;

		if (cg.DataPadforcepowerSelect < 0)
		{ 
			cg.DataPadforcepowerSelect = MAX_DPSHOWPOWERS-1; 
		}

		if (ForcePowerDataPad_Valid(cg.DataPadforcepowerSelect))	// Does he have the force power?
		{
			return;
		}
	}


	cg.DataPadforcepowerSelect = original;
}

// DESCRIZIONI MAGIE PER IL DATAPAD 
const char *forcepowerDesc[NUM_FORCE_POWERS] = 
{
"FORCE_ABSORB_DESC",
"FORCE_HEAL_DESC",
"FORCE_PROTECT_DESC",
"FORCE_MIND_TRICK_DESC",
"FORCE_STUN_DESC",
"AIRREAVER_DESC",
"MATERIALREAVER_DESC",
"FORCE_JUMP_DESC",
"FORCE_SPEED_DESC",
"FORCE_PUSH_DESC",
"FORCE_PULL_DESC",
"FORCE_SABER_THROW_DESC",
"FORCE_SABER_DEFENSE_DESC",
"FORCE_SABER_OFFENSE_DESC",
"FORCE_REVEL_DESC",
"FORCE_FROSTCOFFIN_DESC",
"FORCE_SOULDRAIN_DESC",
"FORCE_THUNDERBOLT_DESC",
"FORCE_TRANCE_DESC",
"FORCE_GRIP_DESC",
"FORCE_HATE_DESC",
"FORCE_CONTROLMIND_DESC",
"FORCE_HEALOTHER_DESC", 
"FORCE_WRACK_DESC",
"FORCE_GLYPH_DESC",
"EARTHREAVER_DESC",
"SOUND_GLYPH_DESC",
"WATERREAVER_DESC",
"FIREREAVER_DESC",
"LIGHTREAVER_DESC",
"DARKREAVER_DESC",
"SPECTRALREAVER_DESC",
};

const char *forcepowerDesc1[NUM_FORCE_POWERS] = 
{
"FORCE_ABSORB_DESC",
"FORCE_HEAL_DESC",
"FORCE_PROTECT_DESC",
"FORCE_PROXY_DESC",
"FORCE_FEARTRICK_DESC",
"AIRREAVER_DESC",
"MATERIALREAVER_DESC",
"FORCE_JUMP_DESC",
"FORCE_SPEED_DESC",
"FORCE_PUSH_DESC",
"FORCE_PULL_DESC",
"FORCE_SABER_THROW_DESC",
"FORCE_SABER_DEFENSE_DESC",
"FORCE_SABER_OFFENSE_DESC",
"FORCE_REVEL_DESC",
"FORCE_FREEZE_DESC",
"FORCE_SOULDRAIN_DESC",
"FORCE_DARKNESS_DESC",
"FORCE_CRUEL_DESC",
"FORCE_GRIP_DESC",
"FORCE_HATE_DESC",
"FORCE_CONTROLMIND_DESC",
"FORCE_HEALOTHER_DESC", 
"FORCE_WRACK_DESC",
"FORCE_GLYPH_DESC",
"EARTHREAVER_DESC",
"SOUND_GLYPH_DESC",
"WATERREAVER_DESC",
"FIREREAVER_DESC",
"LIGHTREAVER_DESC",
"DARKREAVER_DESC",
"SPECTRALREAVER_DESC",
};

const char *forcepowerDesc2[NUM_FORCE_POWERS] = 
{
"FORCE_ABSORB_DESC",
"FORCE_HEAL_DESC",
"FORCE_PROTECT_DESC",
"FORCE_MIND_TRICK_DESC",
"FORCE_STUN_DESC",
"AIRREAVER_DESC",
"MATERIALREAVER_DESC",
"FORCE_JUMP_DESC",
"FORCE_SPEED_DESC",
"FORCE_PUSH_DESC",
"FORCE_PULL_DESC",
"FORCE_SABER_THROW_DESC",
"FORCE_SABER_DEFENSE_DESC",
"FORCE_SABER_OFFENSE_DESC",
"FORCE_REVEL_DESC",
"FORCE_FREEZE_DESC",
"FORCE_NECRODRAIN_DESC",
"FORCE_SMOG_DESC",
"FORCE_TRANCE_DESC",
"FORCE_GRIP_DESC",
"FORCE_HATE_DESC",
"FORCE_CONTROLMIND_DESC",
"FORCE_HEALOTHER_DESC", 
"FORCE_WRACK_DESC",
"FORCE_GLYPH_DESC",
"EARTHREAVER_DESC",
"SOUND_GLYPH_DESC",
"WATERREAVER_DESC",
"FIREREAVER_DESC",
"LIGHTREAVER_DESC",
"SMOGREAVER_DESC",
"SPECTRALREAVER_DESC",
};

const char *forcepowerDesc3[NUM_FORCE_POWERS] = 
{
"FORCE_ABSORB_DESC",
"FORCE_HEALD_DESC",
"FORCE_PROTECT_DESC",
"FORCE_PROXY_DESC",
"FORCE_FEARTRICK_DESC",
"AIRREAVER_DESC",
"MATERIALREAVER_DESC",
"FORCE_JUMP_DESC",
"FORCE_SPEED_DESC",
"FORCE_PUSH_DESC",
"FORCE_PULL_DESC",
"FORCE_SABER_THROW_DESC",
"FORCE_SABER_DEFENSE_DESC",
"FORCE_SABER_OFFENSE_DESC",
"FORCE_REVEL_DESC",
"FORCE_FREEZE_DESC",
"FORCE_NECRODRAIN_DESC",
"FORCE_VOID_DESC",
"FORCE_CRUEL_DESC",
"FORCE_GRIP_DESC",
"FORCE_HATE_DESC",
"FORCE_CONTROLMIND_DESC",
"FORCE_HEALOTHER_DESC", 
"FORCE_WRACK_DESC",
"FORCE_GLYPH_DESC",
"STONE_GLYPH_DESC",
"SOUND_GLYPH_DESC",
"WATERREAVER_DESC",
"FIREREAVER_DESC",
"LIGHTREAVER_DESC",
"VOIDREAVER_DESC",
"SPECTRALREAVER_DESC",
};

const char *forcepowerDesc4[NUM_FORCE_POWERS] = 
{
"FORCE_ABSORB_DESC",
"FORCE_HEAL_DESC",
"FORCE_PROTECT_DESC",
"FORCE_MIND_TRICK_DESC",
"FORCE_STUN_DESC",
"AIRREAVER_DESC",
"MATERIALREAVER_DESC",
"FORCE_JUMP_DESC",
"FORCE_SPEED_DESC",
"FORCE_PUSH_DESC",
"FORCE_PULL_DESC",
"FORCE_SABER_THROW_DESC",
"FORCE_SABER_DEFENSE_DESC",
"FORCE_SABER_OFFENSE_DESC",
"FORCE_REVEL_DESC",
"FORCE_THUNDERNET_DESC",
"FORCE_SOULDRAIN_DESC",
"FORCE_YELLOWLIGHTNING_DESC",
"FORCE_SHOCKSHIELD_DESC",
"FORCE_GRIP_DESC",
"FORCE_HATE_DESC",
"FORCE_CONTROLMIND_DESC",
"FORCE_HEALOTHER_DESC", 
"FORCE_WRACK_DESC",
"FORCE_GLYPH_DESC",
"STONE_GLYPH_DESC",
"SOUND_GLYPH_DESC",
"WATERREAVER_DESC",
"FIREREAVER_DESC",
"LIGHTNINGREAVER_DESC",
"DARKREAVER_DESC",
"SPECTRALREAVER_DESC",
};

const char *forcepowerDesc5[NUM_FORCE_POWERS] = 
{
"FORCE_ABSORB_DESC",
"FORCE_HEAL_DESC",
"FORCE_PROTECT_DESC",
"FORCE_MIND_TRICK_DESC",
"FORCE_STUN_DESC",
"AIRREAVER_DESC",
"MATERIALREAVER_DESC",
"FORCE_JUMP_DESC",
"FORCE_SPEED_DESC",
"FORCE_PUSH_DESC",
"FORCE_PULL_DESC",
"FORCE_SABER_THROW_DESC",
"FORCE_SABER_DEFENSE_DESC",
"FORCE_SABER_OFFENSE_DESC",
"FORCE_REVEL_DESC",
"FORCE_THUNDERNET_DESC",
"FORCE_DEMONDRAIN_DESC",
"FORCE_BURST_DESC",
"FORCE_ELDERSHIELD_DESC",
"FORCE_GRIP_DESC",
"FORCE_HATE_DESC",
"FORCE_CONTROLMIND_DESC",
"FORCE_HEALOTHER_DESC", 
"FORCE_WRACK_DESC",
"FORCE_GLYPH_DESC",
"STONE_GLYPH_DESC",
"SOUND_GLYPH_DESC",
"WATERREAVER_DESC",
"FIREREAVER_DESC",
"SUNREAVER_DESC",
"DARKREAVER_DESC",
"SPECTRALREAVER_DESC",
};

const char *forcepowerDesc6[NUM_FORCE_POWERS] = 
{
"FORCE_ABSORB_DESC",
"FORCE_HEAL_DESC",
"FORCE_PROTECT_DESC",
"FORCE_MIND_TRICK_DESC",
"FORCE_STUN_DESC",
"AIRREAVER_DESC",
"MATERIALREAVER_DESC",
"FORCE_JUMP_DESC",
"FORCE_SPEED_DESC",
"FORCE_PUSH_DESC",
"FORCE_PULL_DESC",
"FORCE_SABER_THROW_DESC",
"FORCE_SABER_DEFENSE_DESC",
"FORCE_SABER_OFFENSE_DESC",
"FORCE_ECHO_DESC",
"FORCE_FREEZE_DESC",
"FORCE_SOULDRAIN_DESC",
"FORCE_FIRE_DESC",
"FORCE_FIRE_RAGE_DESC",
"FORCE_GRIP_DESC",
"FORCE_HATE_DESC",
"FORCE_CONTROLMIND_DESC",
"FORCE_HEALOTHER_DESC", 
"FORCE_WRACK_DESC",
"FORCE_GLYPH_DESC",
"EARTHREAVER_DESC",
"SOUND_GLYPH_DESC",
"WATERREAVER_DESC",
"WARMREAVER_DESC",
"LIGHTREAVER_DESC",
"DARKREAVER_DESC",
"SPECTRALREAVER_DESC",
};	

const char *forcepowerDesc7[NUM_FORCE_POWERS] = 
{
"FORCE_ABSORB_DESC",
"FORCE_HEALFIRE_DESC",
"FORCE_PROTECT_DESC",
"FORCE_MIND_TRICK_DESC",
"FORCE_STUN_DESC",
"AIRREAVER_DESC",
"MATERIALREAVER_DESC",
"FORCE_JUMP_DESC",
"FORCE_SPEED_DESC",
"FORCE_PUSH_DESC",
"FORCE_PULL_DESC",
"FORCE_SABER_THROW_DESC",
"FORCE_SABER_DEFENSE_DESC",
"FORCE_SABER_OFFENSE_DESC",
"FORCE_ECHO_DESC",
"FORCE_FREEZE_DESC",
"FORCE_DRAIN_DESC",
"FORCE_LAVA_DESC",
"FORCE_FIRE_RAGE_DESC",
"FORCE_GRIP_DESC",
"FORCE_HATE_DESC",
"FORCE_CONTROLMIND_DESC",
"FORCE_HEALOTHER_DESC", 
"FORCE_WRACK_DESC",
"FORCE_GLYPH_DESC",
"EARTHREAVER_DESC",
"SOUND_GLYPH_DESC",
"WATERREAVER_DESC",
"MAGMAREAVER_DESC",
"LIGHTREAVER_DESC",
"SMOGREAVER_DESC",
"SPECTRALREAVER_DESC",
};	

const char *forcepowerDesc8[NUM_FORCE_POWERS] = 
{
"FORCE_ABSORB_DESC",
"FORCE_HEAL_DESC",
"FORCE_PROTECT_DESC",
"FORCE_MIND_TRICK_DESC",
"FORCE_ETERFORM_DESC",
"AIRREAVER_DESC",
"MATERIALREAVER_DESC",
"FORCE_JUMP_DESC",
"FORCE_SPEED_DESC",
"FORCE_PUSH_DESC",
"FORCE_PULL_DESC",
"FORCE_SABER_THROW_DESC",
"FORCE_SABER_DEFENSE_DESC",
"FORCE_SABER_OFFENSE_DESC",
"FORCE_REVEL_DESC",
"FORCE_FREEZE_DESC",
"FORCE_SOULDRAIN_DESC",
"FORCE_WIND_DESC",
"FORCE_WINDSHIELD_DESC",
"FORCE_GRIP_DESC",
"FORCE_HATE_DESC",
"FORCE_CONTROLMIND_DESC",
"FORCE_HEALOTHER_DESC", 
"FORCE_WRACK_DESC",
"FORCE_GLYPH_DESC",
"EARTHREAVER_DESC",
"SOUND_GLYPH_DESC",
"WATERREAVER_DESC",
"FIREREAVER_DESC",
"LIGHTREAVER_DESC",
"DARKREAVER_DESC",
"SPECTRALREAVER_DESC",
};


const char *forcepowerDesc9[NUM_FORCE_POWERS] = 
{
"FORCE_ABSORB_DESC",
"FORCE_HEAL_DESC",
"FORCE_PROTECT_DESC",
"FORCE_MIND_TRICK_DESC",
"FORCE_ETERFORM_DESC",
"FOGREAVER_DESC",
"MATERIALREAVER_DESC",
"FORCE_JUMP_DESC",
"FORCE_SPEED_DESC",
"FORCE_PUSH_DESC",
"FORCE_PULL_DESC",
"FORCE_SABER_THROW_DESC",
"FORCE_SABER_DEFENSE_DESC",
"FORCE_SABER_OFFENSE_DESC",
"FORCE_REVEL_DESC",
"FORCE_FREEZE_DESC",
"FORCE_SOULDRAIN_DESC",
"FORCE_WIND_DESC",
"FORCE_WINDSHIELD_DESC",
"FORCE_GRIP_DESC",
"FORCE_HATE_DESC",
"FORCE_CONTROLMIND_DESC",
"FORCE_HEALOTHER_DESC", 
"FORCE_WRACK_DESC",
"FORCE_GLYPH_DESC",
"EARTHREAVER_DESC",
"SOUND_GLYPH_DESC",
"WATERREAVER_DESC",
"FIREREAVER_DESC",
"LIGHTREAVER_DESC",
"DARKREAVER_DESC",
"SPECTRALREAVER_DESC",
};
	
const char *forcepowerDesc10[NUM_FORCE_POWERS] = 
{
"FORCE_ABSORB_DESC",
"FORCE_HEAL_DESC",
"FORCE_PROTECT_DESC",
"FORCE_MIND_TRICK_DESC",
"FORCE_ETERFORM_DESC",
"STORMREAVER_DESC",
"MATERIALREAVER_DESC",
"FORCE_JUMP_DESC",
"FORCE_SPEED_DESC",
"FORCE_PUSH_DESC",
"FORCE_PULL_DESC",
"FORCE_SABER_THROW_DESC",
"FORCE_SABER_DEFENSE_DESC",
"FORCE_SABER_OFFENSE_DESC",
"FORCE_REVEL_DESC",
"FORCE_FREEZE_DESC",
"FORCE_SOULDRAIN_DESC",
"FORCE_WIND2_DESC",
"FORCE_STORMSHIELD_DESC",
"FORCE_GRIP_DESC",
"FORCE_HATE_DESC",
"FORCE_CONTROLMIND_DESC",
"FORCE_HEALOTHER_DESC", 
"FORCE_WRACK_DESC",
"FORCE_GLYPH_DESC",
"EARTHREAVER_DESC",
"SOUND_GLYPH_DESC",
"WATERREAVER_DESC",
"FIREREAVER_DESC",
"LIGHTREAVER_DESC",
"DARKREAVER_DESC",
"SPECTRALREAVER_DESC",
};

const char *forcepowerDesc11[NUM_FORCE_POWERS] = 
{
"FORCE_ABSORB_DESC",
"FORCE_HEALV_DESC",
"FORCE_PROTECT_DESC",
"FORCE_WOLFFORM_DESC",
"FORCE_STUN_DESC",
"MINDREAVER_DESC",
"SOULREAVER_DESC",
"FORCE_JUMP_DESC",
"FORCE_SPEED2_DESC",
"FORCE_PUSH_DESC",
"FORCE_PULL_DESC",
"FORCE_SABER_THROW_DESC",
"FORCE_SABER_DEFENSE_DESC",
"FORCE_SABER_OFFENSE_DESC",
"FORCE_SEEING_DESC",
"FORCE_FREEZE_DESC",
"FORCE_DRAIN_DESC",
"FORCE_SONIC_DESC",
"FORCE_RAGE_DESC",
"FORCE_GRIP_DESC",
"FORCE_HATE_DESC",
"FORCE_CONTROLMIND_DESC",
"FORCE_BATFORM_DESC", 
"FORCE_WRACK_DESC",
"FORCE_MISTFORM_DESC",
"TIMEREAVER_DESC",
"DIMENSIONREAVER_DESC",
"STATESREAVER_DESC",
"CONFLICTREAVER_DESC",
"ENERGYREAVER_DESC",
"NATUREREAVER_DESC",
"BLOODREAVER_DESC",
};

const char *forcepowerDesc12[NUM_FORCE_POWERS] = 
{
"FORCE_ABSORB_DESC",
"FORCE_HEAL_DESC",
"FORCE_PROTECT_DESC",
"FORCE_MIND_TRICK_DESC",
"FORCE_STUN_DESC",
"AIRREAVER_DESC",
"MATERIALREAVER_DESC",
"FORCE_JUMP_DESC",
"FORCE_SPEED_DESC",
"FORCE_PUSH_DESC",
"FORCE_PULL_DESC",
"FORCE_SABER_THROW_DESC",
"FORCE_SABER_DEFENSE_DESC",
"FORCE_SABER_OFFENSE_DESC",
"FORCE_REVEL_DESC",
"FORCE_FROSTCOFFIN_DESC",
"FORCE_NECRODRAIN_DESC",
"FORCE_ICE_DESC",
"FORCE_FROSTARMOR_DESC",
"FORCE_GRIP_DESC",
"FORCE_HATE_DESC",
"FORCE_CONTROLMIND_DESC",
"FORCE_HEALOTHER_DESC", 
"FORCE_WRACK_DESC",
"FORCE_GLYPH_DESC",
"EARTHREAVER_DESC",
"SOUND_GLYPH_DESC",
"ICEREAVER_DESC",
"FIREREAVER_DESC",
"LIGHTREAVER_DESC",
"DARKREAVER_DESC",
"SPECTRALREAVER_DESC",
};

const char *forcepowerDesc13[NUM_FORCE_POWERS] = 
{
"FORCE_ABSORB_DESC",
"FORCE_HEAL_DESC",
"FORCE_PROTECT_DESC",
"FORCE_MIND_TRICK_DESC",
"FORCE_STUN_DESC",
"AIRREAVER_DESC",
"MATERIALREAVER_DESC",
"FORCE_JUMP_DESC",
"FORCE_SPEED_DESC",
"FORCE_PUSH_DESC",
"FORCE_PULL_DESC",
"FORCE_SABER_THROW_DESC",
"FORCE_SABER_DEFENSE_DESC",
"FORCE_SABER_OFFENSE_DESC",
"FORCE_REVEL_DESC",
"FORCE_FROSTCOFFIN_DESC",
"FORCE_NECRODRAIN_DESC",
"FORCE_CRYSTAL_DESC",
"FORCE_CRYSTALARMOR_DESC",
"FORCE_GRIP_DESC",
"FORCE_HATE_DESC",
"FORCE_CONTROLMIND_DESC",
"FORCE_HEALOTHER_DESC", 
"FORCE_WRACK_DESC",
"FORCE_GLYPH_DESC",
"EARTHREAVER_DESC",
"SOUND_GLYPH_DESC",
"CRYSTALREAVER_DESC",
"FIREREAVER_DESC",
"LIGHTREAVER_DESC",
"DARKREAVER_DESC",
"SPECTRALREAVER_DESC",
};

const char *forcepowerDesc14[NUM_FORCE_POWERS] = 
{
"FORCE_ABSORB_DESC",
"FORCE_HEAL_DESC",
"FORCE_PROTECT_DESC",
"FORCE_MIND_TRICK_DESC",
"FORCE_STUN_DESC",
"AIRREAVER_DESC",
"MATERIALREAVER_DESC",
"FORCE_JUMP_DESC",
"FORCE_SPEED_DESC",
"FORCE_PUSH_DESC",
"FORCE_PULL_DESC",
"FORCE_SABER_THROW_DESC",
"FORCE_SABER_DEFENSE_DESC",
"FORCE_SABER_OFFENSE_DESC",
"FORCE_REVEL_DESC",
"FORCE_WEBBED_DESC",
"FORCE_SOULDRAIN_DESC",
"FORCE_WEB_DESC",
"FORCE_TRANCE_DESC",
"FORCE_GRIP_DESC",
"FORCE_HATE_DESC",
"FORCE_CONTROLMIND_DESC",
"FORCE_HEALOTHER_DESC", 
"FORCE_WRACK_DESC",
"FORCE_GLYPH_DESC",
"DUSTREAVER_DESC",
"SOUND_GLYPH_DESC",
"WATERREAVER_DESC",
"FIREREAVER_DESC",
"LIGHTREAVER_DESC",
"DARKREAVER_DESC",
"SPECTRALREAVER_DESC",
};

const char *forcepowerDesc15[NUM_FORCE_POWERS] = 
{
"FORCE_ABSORB_DESC",
"FORCE_HEAL_DESC",
"FORCE_PROTECT_DESC",
"FORCE_MIND_TRICK_DESC",
"FORCE_STUN_DESC",
"AIRREAVER_DESC",
"MATERIALREAVER_DESC",
"FORCE_JUMP_DESC",
"FORCE_SPEED_DESC",
"FORCE_PUSH_DESC",
"FORCE_PULL_DESC",
"FORCE_SABER_THROW_DESC",
"FORCE_SABER_DEFENSE_DESC",
"FORCE_SABER_OFFENSE_DESC",
"FORCE_REVEL_DESC",
"FORCE_WEBBED_DESC",
"FORCE_SOULDRAIN_DESC",
"FORCE_WEB_DESC",
"FORCE_TRANCE_DESC",
"FORCE_GRIP_DESC",
"FORCE_HATE_DESC",
"FORCE_CONTROLMIND_DESC",
"FORCE_HEALOTHER_DESC", 
"FORCE_WRACK_DESC",
"FORCE_GLYPH_DESC",
"DUSTREAVER_DESC",
"SOUND_GLYPH_DESC",
"WATERREAVER_DESC",
"FIREREAVER_DESC",
"LIGHTREAVER_DESC",
"DARKREAVER_DESC",
"MUDREAVER_DESC",
};

const char *forcepowerDesc16[NUM_FORCE_POWERS] = 
{
"FORCE_ABSORB_DESC",
"FORCE_HEAL_DESC",
"FORCE_PROTECT_DESC",
"FORCE_MIND_TRICK_DESC",
"FORCE_STUN_DESC",
"AIRREAVER_DESC",
"MATERIALREAVER_DESC",
"FORCE_JUMP_DESC",
"FORCE_SPEED_DESC",
"FORCE_PUSH_DESC",
"FORCE_PULL_DESC",
"FORCE_SABER_THROW_DESC",
"FORCE_SABER_DEFENSE_DESC",
"FORCE_SABER_OFFENSE_DESC",
"FORCE_REVEL_DESC",
"FORCE_STONED_DESC",
"FORCE_SOULDRAIN_DESC",
"FORCE_STONE_DESC",
"FORCE_RAGE_DESC",
"FORCE_GRIP_DESC",
"FORCE_HATE_DESC",
"FORCE_CONTROLMIND_DESC",
"FORCE_HEALOTHER_DESC", 
"FORCE_WRACK_DESC",
"FORCE_GLYPH_DESC",
"STONEREAVER_DESC",
"SOUND_GLYPH_DESC",
"WATERREAVER_DESC",
"FIREREAVER_DESC",
"LIGHTREAVER_DESC",
"DARKREAVER_DESC",
"SPECTRALREAVER_DESC",
};

const char *forcepowerDesc17[NUM_FORCE_POWERS] = 
{
"FORCE_ABSORB_DESC",
"FORCE_HEAL_DESC",
"FORCE_PROTECT_DESC",
"FORCE_MIND_TRICK_DESC",
"FORCE_STUN_DESC",
"AIRREAVER_DESC",
"MATERIALREAVER_DESC",
"FORCE_JUMP_DESC",
"FORCE_SPEED_DESC",
"FORCE_PUSH_DESC",
"FORCE_PULL_DESC",
"FORCE_SABER_THROW_DESC",
"FORCE_SABER_DEFENSE_DESC",
"FORCE_SABER_OFFENSE_DESC",
"FORCE_REVEL_DESC",
"FORCE_WEBBED_DESC",
"FORCE_SOULDRAIN_DESC",
"FORCE_WEB_DESC",
"FORCE_TRANCE_DESC",
"FORCE_GRIP_DESC",
"FORCE_HATE_DESC",
"FORCE_CONTROLMIND_DESC",
"FORCE_HEALOTHER_DESC", 
"FORCE_WRACK_DESC",
"FORCE_GLYPH_DESC",
"DUSTREAVER_DESC",
"SOUND_GLYPH_DESC",
"WATERREAVER_DESC",
"FIREREAVER_DESC",
"LIGHTREAVER_DESC",
"DARKREAVER_DESC",
"POISONREAVER_DESC",
};

const char *forcepowerDesc18[NUM_FORCE_POWERS] = 
{
"FORCE_ABSORB_DESC",
"FORCE_HEAL_DESC",
"FORCE_PROTECT_DESC",
"FORCE_MIND_TRICK_DESC",
"FORCE_STUN_DESC",
"AIRREAVER_DESC",
"MATERIALREAVER_DESC",
"FORCE_JUMP_DESC",
"FORCE_SPEED_DESC",
"FORCE_PUSH_DESC",
"FORCE_PULL_DESC",
"FORCE_SABER_THROW_DESC",
"FORCE_SABER_DEFENSE_DESC",
"FORCE_SABER_OFFENSE_DESC",
"FORCE_REVEL_DESC",
"FORCE_FREEZE_DESC",
"FORCE_SOULDRAIN_DESC",
"FORCE_REVERB_DESC",
"FORCE_NECROFEAR_DESC",
"FORCE_GRIP_DESC",
"FORCE_HATE_DESC",
"FORCE_CONTROLMIND_DESC",
"FORCE_HEALOTHER_DESC", 
"FORCE_WRACK_DESC",
"FORCE_GLYPH_DESC",
"EARTHREAVER_DESC",
"SOUND_GLYPH_DESC",
"WATERREAVER_DESC",
"FIREREAVER_DESC",
"LIGHTREAVER_DESC",
"SMOGREAVER_DESC",
"DEATHREAVER_DESC",
};

const char *forcepowerDesc19[NUM_FORCE_POWERS] = 
{
"FORCE_ABSORB_DESC",
"FORCE_HEAL_DESC",
"FORCE_PROTECT_DESC",
"FORCE_MIND_TRICK_DESC",
"FORCE_STUN_DESC",
"AIRREAVER_DESC",
"MATERIALREAVER_DESC",
"FORCE_JUMP_DESC",
"FORCE_SPEED_DESC",
"FORCE_PUSH_DESC",
"FORCE_PULL_DESC",
"FORCE_SABER_THROW_DESC",
"FORCE_SABER_DEFENSE_DESC",
"FORCE_SABER_OFFENSE_DESC",
"FORCE_REVEL_DESC",
"FORCE_FREEZE_DESC",
"FORCE_NECRODRAIN_DESC",
"FORCE_SKULL_DESC",
"FORCE_NECROFEAR_DESC",
"FORCE_GRIP_DESC",
"FORCE_HATE_DESC",
"FORCE_CONTROLMIND_DESC",
"FORCE_HEALOTHER_DESC", 
"FORCE_WRACK_DESC",
"FORCE_GLYPH_DESC",
"EARTHREAVER_DESC",
"SOUND_GLYPH_DESC",
"WATERREAVER_DESC",
"FIREREAVER_DESC",
"LIGHTREAVER_DESC",
"SMOGREAVER_DESC",
"DEATHREAVER_DESC",
};

const char *forcepowerDesc20[NUM_FORCE_POWERS] = 
{
"FORCE_ABSORB_DESC",
"FORCE_HEAL_DESC",
"FORCE_PROTECT_DESC",
"FORCE_MIND_TRICK_DESC",
"FORCE_STUN_DESC",
"AIRREAVER_DESC",
"SPIRITREAVER_DESC",
"FORCE_JUMP_DESC",
"FORCE_SPEED_DESC",
"FORCE_PUSH_DESC",
"FORCE_PULL_DESC",
"FORCE_SABER_THROW_DESC",
"FORCE_SABER_DEFENSE_DESC",
"FORCE_SABER_OFFENSE_DESC",
"FORCE_REVEL_DESC",
"FORCE_HOLLYSTASIS_DESC",
"FORCE_SOULDRAIN_DESC",
"FORCE_PSYCHIC_DESC",
"FORCE_TRANCE_DESC",
"FORCE_GRIP_DESC",
"FORCE_HATE_DESC",
"FORCE_CONTROLMIND_DESC",
"FORCE_HEALOTHER_DESC", 
"FORCE_WRACK_DESC",
"FORCE_GLYPH_DESC",
"STONE_GLYPH_DESC",
"SOUND_GLYPH_DESC",
"WATER_GLYPH_DESC",
"FIRE_GLYPH_DESC",
"SUN_GLYPH_DESC",
"DARKREAVER_DESC",
"SPECTRALREAVER_DESC",
};


const char *forcepowerDesc21[NUM_FORCE_POWERS] = 
{
"FORCE_ABSORB_DESC",
"FORCE_HEAL_DESC",
"FORCE_PROTECT_DESC",
"FORCE_MIND_TRICK_DESC",
"FORCE_STUN_DESC",
"AIRREAVER_DESC",
"HOLYREAVER_DESC",
"FORCE_JUMP_DESC",
"FORCE_SPEED_DESC",
"FORCE_PUSH_DESC",
"FORCE_PULL_DESC",
"FORCE_SABER_THROW_DESC",
"FORCE_SABER_DEFENSE_DESC",
"FORCE_SABER_OFFENSE_DESC",
"FORCE_REVEL_DESC",
"FORCE_HOLLYSTASIS_DESC",
"FORCE_SOULDRAIN_DESC",
"FORCE_HOLLY_DESC",
"FORCE_TRANCE_DESC",
"FORCE_GRIP_DESC",
"FORCE_HATE_DESC",
"FORCE_CONTROLMIND_DESC",
"FORCE_HEALOTHER_DESC", 
"FORCE_WRACK_DESC",
"FORCE_GLYPH_DESC",
"EARTHREAVER_DESC",
"SOUND_GLYPH_DESC",
"WATERREAVER_DESC",
"FIREREAVER_DESC",
"LIGHTREAVER_DESC",
"DARKREAVER_DESC",
"SPECTRALREAVER_DESC",
};

const char *forcepowerDesc22[NUM_FORCE_POWERS] = 
{
"FORCE_ABSORB_DESC",
"FORCE_HEALS_DESC",
"FORCE_PROTECT_DESC",
"FORCE_MIND_TRICK_DESC",
"FORCE_STUN_DESC",
"AIRREAVER_DESC",
"LIFEREAVER_DESC",
"FORCE_JUMP_DESC",
"FORCE_SPEED_DESC",
"FORCE_PUSH_DESC",
"FORCE_PULL_DESC",
"FORCE_SABER_THROW_DESC",
"FORCE_SABER_DEFENSE_DESC",
"FORCE_SABER_OFFENSE_DESC",
"FORCE_REVEL_DESC",
"FORCE_HOLLYSTASIS_DESC",
"FORCE_SOULDRAIN_DESC",
"FORCE_LIFE_DESC",
"FORCE_TRANCE_DESC",
"FORCE_GRIP_DESC",
"FORCE_HATE_DESC",
"FORCE_CONTROLMIND_DESC",
"FORCE_HEALOTHER_DESC", 
"FORCE_WRACK_DESC",
"FORCE_GLYPH_DESC",
"EARTHREAVER_DESC",
"SOUND_GLYPH_DESC",
"WATERREAVER_DESC",
"FIREREAVER_DESC",
"LIGHTREAVER_DESC",
"DARKREAVER_DESC",
"SPECTRALREAVER_DESC",
};

const char *forcepowerDesc23[NUM_FORCE_POWERS] = 
{
"FORCE_ABSORB_DESC",
"FORCE_HEAL_DESC",
"FORCE_PROTECT_DESC",
"FORCE_MIND_TRICK_DESC",
"FORCE_STUN_DESC",
"AIRREAVER_DESC",
"MATERIALREAVER_DESC",
"FORCE_JUMP_DESC",
"FORCE_SPEED_DESC",
"FORCE_PUSH_DESC",
"FORCE_PULL_DESC",
"FORCE_SABER_THROW_DESC",
"FORCE_SABER_DEFENSE_DESC",
"FORCE_SABER_OFFENSE_DESC",
"FORCE_REVEL_DESC",
"FORCE_IVYTRAP_DESC",
"FORCE_SOULDRAIN_DESC",
"FORCE_HOLLYFIRE_DESC",
"FORCE_RUNESHIELD_DESC",
"FORCE_GRIP_DESC",
"FORCE_HATE_DESC",
"FORCE_CONTROLMIND_DESC",
"FORCE_HEALOTHER_DESC", 
"FORCE_WRACK_DESC",
"FORCE_GLYPH_DESC",
"EARTHREAVER_DESC",
"SOUNDREAVER_DESC",
"WATERREAVER_DESC",
"FIREREAVER_DESC",
"LIGHTREAVER_DESC",
"DARKREAVER_DESC",
"SPECTRALREAVER_DESC",
};

const char *forcepowerDesc24[NUM_FORCE_POWERS] = 
{
"FORCE_ABSORB_DESC",
"FORCE_HEAL_DESC",
"FORCE_PROTECT_DESC",
"FORCE_MIND_TRICK_DESC",
"FORCE_STUN_DESC",
"AIRREAVER_DESC",
"MATERIALREAVER_DESC",
"FORCE_JUMP_DESC",
"FORCE_SPEED_DESC",
"FORCE_PUSH_DESC",
"FORCE_PULL_DESC",
"FORCE_SABER_THROW_DESC",
"FORCE_SABER_DEFENSE_DESC",
"FORCE_SABER_OFFENSE_DESC",
"FORCE_REVEL_DESC",
"FORCE_GRAVITYTRAP_DESC",
"FORCE_NECRODRAIN_DESC",
"FORCE_ASTRAL_DESC",
"FORCE_RUNESHIELD_DESC",
"FORCE_GRIP_DESC",
"FORCE_HATE_DESC",
"FORCE_CONTROLMIND_DESC",
"FORCE_HEALOTHER_DESC", 
"FORCE_WRACK_DESC",
"FORCE_GLYPH_DESC",
"EARTHREAVER_DESC",
"ASTRALREAVER_DESC",
"WATERREAVER_DESC",
"FIREREAVER_DESC",
"LIGHTREAVER_DESC",
"DARKREAVER_DESC",
"SPECTRALREAVER_DESC",
};

// Vampires 
const char *forcepowerDesc25[NUM_FORCE_POWERS] = 
{
"FORCE_ABSORB_DESC",
"FORCE_HEALV_DESC",
"FORCE_PROTECT_DESC",
"FORCE_WOLFFORM_DESC",
"FORCE_STUN_DESC",
"MINDREAVER_DESC",
"SOULREAVER_DESC",
"FORCE_JUMP_DESC",
"FORCE_SPEED2_DESC",
"FORCE_PUSH_DESC",
"FORCE_PULL_DESC",
"FORCE_SABER_THROW_DESC",
"FORCE_SABER_DEFENSE_DESC",
"FORCE_SABER_OFFENSE_DESC",
"FORCE_SEEING_DESC",
"FORCE_FREEZE_DESC",
"FORCE_DRAIN_DESC",
"FORCE_LIGHTNING_DESC",
"FORCE_RAGE_DESC",
"FORCE_GRIP_DESC",
"FORCE_HATE_DESC",
"FORCE_CONTROLMIND_DESC",
"FORCE_BATFORM_DESC", 
"FORCE_WRACK_DESC",
"FORCE_MISTFORM_DESC",
"TIMEREAVER_DESC",
"DIMENSIONREAVER_DESC",
"STATESREAVER_DESC",
"CONFLICTREAVER_DESC",
"ENERGYREAVER_DESC",
"NATUREREAVER_DESC",
"BLOODREAVER_DESC",
};

const char *forcepowerDesc26[NUM_FORCE_POWERS] = 
{
"FORCE_ABSORB_DESC",
"FORCE_HEALV_DESC",
"FORCE_PROTECT_DESC",
"FORCE_WOLFFORMDUAL_DESC",
"FORCE_FEARTRICK_DESC",
"FORCE_FOGOFFEAR_DESC",
"SOULREAVER_DESC",
"FORCE_JUMP_DESC",
"FORCE_SPEED2_DESC",
"FORCE_PUSH_DESC",
"FORCE_PULL_DESC",
"FORCE_SABER_THROW_DESC",
"FORCE_SABER_DEFENSE_DESC",
"FORCE_SABER_OFFENSE_DESC",
"FORCE_SEEING_DESC",
"FORCE_FREEZE_DESC",
"FORCE_DEMONDRAIN_DESC",
"FORCE_HELL_DESC",
"FORCE_INSPIREFEAR_DESC",
"FORCE_GRIP_DESC",
"FORCE_HATE_DESC",
"FORCE_CONTROLMIND_DESC",
"FORCE_BATFORM_DESC", 
"FORCE_WRACK_DESC",
"FORCE_MISTOFFEAR_DESC",
"TIMEREAVER_DESC",
"DIMENSIONREAVER_DESC",
"STATESREAVER_DESC",
"CONFLICTREAVER_DESC",
"ENERGYREAVER_DESC",
"NATUREREAVER_DESC",
"DEATHREAVER2_DESC",
};

const char *forcepowerDesc27[NUM_FORCE_POWERS] = 
{
"FORCE_ABSORB_DESC",
"FORCE_HEALV_DESC",
"FORCE_PROTECT_DESC",
"FORCE_WOLFFORM_DESC",
"FORCE_STUN_DESC",
"MINDREAVER_DESC",
"BALANCEREAVER_DESC",
"FORCE_JUMP_DESC",
"FORCE_SPEED2_DESC",
"FORCE_PUSH_DESC",
"FORCE_PULL_DESC",
"FORCE_SABER_THROW_DESC",
"FORCE_SABER_DEFENSE_DESC",
"FORCE_SABER_OFFENSE_DESC",
"FORCE_SEEING_DESC",
"FORCE_FREEZE_DESC",
"FORCE_DRAIN2_DESC",
"FORCE_LIGHTNING2_DESC",
"FORCE_RAGE_DESC",
"FORCE_GRIP_DESC",
"FORCE_HATE_DESC",
"FORCE_CONTROLMIND_DESC",
"FORCE_BATFORM_DESC", 
"FORCE_WRACK_DESC",
"FORCE_MISTFORM_DESC",
"TIMEREAVER_DESC",
"DIMENSIONREAVER_DESC",
"STATESREAVER_DESC",
"CONFLICTREAVER_DESC",
"ENERGYREAVER_DESC",
"NATUREREAVER_DESC",
"DEATHREAVER2_DESC",
};

const char *forcepowerDesc28[NUM_FORCE_POWERS] = 
{
"FORCE_ABSORB_DESC",
"FORCE_HEAL_DESC",
"FORCE_PROTECT_DESC",
"FORCE_MIND_TRICK_DESC",
"FORCE_STUN_DESC",
"AIRREAVER_DESC",
"MATERIALREAVER_DESC",
"FORCE_JUMP_DESC",
"FORCE_SPEED_DESC",
"FORCE_PUSH_DESC",
"FORCE_PULL_DESC",
"FORCE_SABER_THROW_DESC",
"FORCE_SABER_DEFENSE_DESC",
"FORCE_SABER_OFFENSE_DESC",
"FORCE_REVEL_DESC",
"FORCE_FREEZE_DESC",
"FORCE_SOULDRAIN_DESC",
"FORCE_WATER_DESC",
"FORCE_BERSERK_DESC",
"FORCE_GRIP_DESC",
"FORCE_HATE_DESC",
"FORCE_CONTROLMIND_DESC",
"FORCE_HEALOTHER_DESC", 
"FORCE_WRACK_DESC",
"FORCE_GLYPH_DESC",
"EARTHREAVER_DESC",
"SOUND_GLYPH_DESC",
"WATERREAVER_DESC",
"FIREREAVER_DESC",
"LIGHTREAVER_DESC",
"DARKREAVER_DESC",
"SPECTRALREAVER_DESC",
};

const char *forcepowerDesc29[NUM_FORCE_POWERS] = 
{
"FORCE_ABSORB_DESC",
"FORCE_HEALV_DESC",
"FORCE_PROTECT_DESC",
"FORCE_WOLFFORM_DESC",
"FORCE_STUN_DESC",
"MINDREAVER_DESC",
"SOULREAVER_DESC",
"FORCE_JUMP_DESC",
"FORCE_SPEED2_DESC",
"FORCE_PUSH_DESC",
"FORCE_PULL_DESC",
"FORCE_SABER_THROW_DESC",
"FORCE_SABER_DEFENSE_DESC",
"FORCE_SABER_OFFENSE_DESC",
"FORCE_SEEING_DESC",
"FORCE_FREEZE_DESC",
"FORCE_DRAIN_DESC",
"FORCE_NECRO_DESC",
"FORCE_RAGE_DESC",
"FORCE_GRIP_DESC",
"FORCE_HATE_DESC",
"FORCE_CONTROLMIND_DESC",
"FORCE_BATFORM_DESC", 
"FORCE_WRACK_DESC",
"FORCE_MISTFORM_DESC",
"TIMEREAVER_DESC",
"DIMENSIONREAVER_DESC",
"STATESREAVER_DESC",
"CONFLICTREAVER_DESC",
"ENERGYREAVER_DESC",
"NATUREREAVER_DESC",
"BLOODREAVER_DESC",
};

const char *forcepowerDesc30[NUM_FORCE_POWERS] = 
{
"FORCE_ABSORB_DESC",
"FORCE_HEALV_DESC",
"FORCE_PROTECT_DESC",
"FORCE_HELLWOLF_DESC",
"FORCE_STUN_DESC",
"MINDREAVER_DESC",
"SOULREAVER_DESC",
"FORCE_JUMP_DESC",
"FORCE_SPEED2_DESC",
"FORCE_PUSH_DESC",
"FORCE_PULL_DESC",
"FORCE_SABER_THROW_DESC",
"FORCE_SABER_DEFENSE_DESC",
"FORCE_SABER_OFFENSE_DESC",
"FORCE_SEEING_DESC",
"FORCE_FREEZE_DESC",
"FORCE_DEMONDRAIN_DESC",
"FORCE_HELL_DESC",
"FORCE_RAGE_DESC",
"FORCE_GRIP_DESC",
"FORCE_HATE_DESC",
"FORCE_CONTROLMIND_DESC",
"FORCE_BIRDFORM_DESC", 
"FORCE_WRACK_DESC",
"FORCE_HELLMIST_DESC",
"TIMEREAVER_DESC",
"DIMENSIONREAVER_DESC",
"STATESREAVER_DESC",
"CONFLICTREAVER_DESC",
"ENERGYREAVER_DESC",
"NATUREREAVER_DESC",
"DEATHREAVER2_DESC",
};

const char *forcepowerDesc31[NUM_FORCE_POWERS] = 
{
"FORCE_ABSORB_DESC",
"FORCE_HEALV_DESC",
"FORCE_PROTECT_DESC",
"FORCE_REVERTWOLF_DESC",
"FORCE_STUN_DESC",
"MINDREAVER_DESC",
"SOULREAVER_DESC",
"FORCE_JUMP_DESC",
"FORCE_SPEED2_DESC",
"FORCE_PUSH_DESC",
"FORCE_PULL_DESC",
"FORCE_SABER_THROW_DESC",
"FORCE_SABER_DEFENSE_DESC",
"FORCE_SABER_OFFENSE_DESC",
"FORCE_ECHOBAT_DESC",
"FORCE_FREEZE_DESC",
"FORCE_DRAIN_DESC",
"FORCE_LIGHTNING_DESC",
"FORCE_RAGE_DESC",
"FORCE_GRIP_DESC",
"FORCE_HATE_DESC",
"FORCE_CONTROLMIND_DESC",
"FORCE_REVERTBAT_DESC", 
"FORCE_WRACK_DESC",
"FORCE_REVERTMIST_DESC",
"TIMEREAVER_DESC",
"DIMENSIONREAVER_DESC",
"STATESREAVER_DESC",
"CONFLICTREAVER_DESC",
"ENERGYREAVER_DESC",
"NATUREREAVER_DESC",
"BLOODREAVER_DESC",
};


const char *forcepowerDesc32[NUM_FORCE_POWERS] = 
{
"FORCE_ABSORB_DESC",
"FORCE_HEALD_DESC",
"FORCE_PROTECT_DESC",
"FORCE_MIND_TRICK_DESC",
"FORCE_STUN_DESC",
"AIRREAVER_DESC",
"MATERIALREAVER_DESC",
"FORCE_JUMP_DESC",
"FORCE_SPEED2_DESC",
"FORCE_PUSH_DESC",
"FORCE_PULL_DESC",
"FORCE_SABER_THROW_DESC",
"FORCE_SABER_DEFENSE_DESC",
"FORCE_SABER_OFFENSE_DESC",
"FORCE_ECHOBAT_DESC",
"FORCE_FREEZE_DESC",
"FORCE_DEMONDRAIN_DESC",
"FORCE_POISON_DESC",
"FORCE_POISONSHIELD_DESC",
"FORCE_GRIP_DESC",
"FORCE_HATE_DESC",
"FORCE_CONTROLMIND_DESC",
"FORCE_FEARTRICK_DESC", 
"FORCE_WRACK_DESC",
"FORCE_GLYPH_DESC",
"STONE_GLYPH_DESC",
"SOUND_GLYPH_DESC",
"WATER_GLYPH_DESC",
"FIRE_GLYPH_DESC",
"SUN_GLYPH_DESC",
"DARKREAVER_DESC",
"POISONREAVER_DESC",
};

const char *forcepowerDesc33[NUM_FORCE_POWERS] = 
{
"FORCE_ABSORB_DESC",
"FORCE_HEALH_DESC",
"FORCE_PROTECT_DESC",
"FORCE_MIND_TRICK_DESC",
"FORCE_STUN_DESC",
"WIND_GLYPH_DESC",
"HOLY_GLYPH_DESC",
"FORCE_JUMP_DESC",
"FORCE_SPEED_DESC",
"FORCE_PUSH_DESC",
"FORCE_PULL_DESC",
"FORCE_SABER_THROW_DESC",
"FORCE_SABER_DEFENSE_DESC",
"FORCE_SABER_OFFENSE_DESC",
"FORCE_HYLREVEL_DESC",
"FORCE_FREEZE_DESC",
"FORCE_HYLDENDRAIN_DESC",
"FORCE_HYLDENFIRE_DESC",
"FORCE_TKSHIELD_DESC",
"FORCE_GRIP_DESC",
"FORCE_HATE_DESC",
"FORCE_CONTROLMIND_DESC",
"FORCE_HEALOTHER_DESC", 
"FORCE_WRACK_DESC",
"FORCE_GLYPH_DESC",
"STONE_GLYPH_DESC",
"SOUND_GLYPH_DESC",
"WATER_GLYPH_DESC",
"FIRE_GLYPH_DESC",
"SUN_GLYPH_DESC",
"DARK_GLYPH_DESC",
"NECRO_GLYPH_DESC",
};

const char *forcepowerDesc34[NUM_FORCE_POWERS] = 
{
"FORCE_ABSORB_DESC",
"FORCE_HEALV_DESC",
"FORCE_PROTECT_DESC",
"FORCE_WOLFFORM_DESC",
"FORCE_STUN_DESC",
"MINDREAVER_DESC",
"SOULREAVER_DESC",
"FORCE_JUMP_DESC",
"FORCE_SPEED2_DESC",
"FORCE_PUSH_DESC",
"FORCE_PULL_DESC",
"FORCE_SABER_THROW_DESC",
"FORCE_SABER_DEFENSE_DESC",
"FORCE_SABER_OFFENSE_DESC",
"FORCE_SEEING_DESC",
"FORCE_WEBBED_DESC",
"FORCE_DRAIN_DESC",
"FORCE_WEB_DESC",
"FORCE_RAGE_DESC",
"FORCE_GRIP_DESC",
"FORCE_HATE_DESC",
"FORCE_CONTROLMIND_DESC",
"BATFORM_DESC", 
"FORCE_WRACK_DESC",
"MISTFORM_DESC",
"TIMEREAVER_DESC",
"DIMENSIONREAVER_DESC",
"STATESREAVER_DESC",
"CONFLICTREAVER_DESC",
"ENERGYREAVER_DESC",
"NATUREREAVER_DESC",
"BLOODREAVER_DESC",
};

const char *forcepowerDesc35[NUM_FORCE_POWERS] = 
{
"FORCE_ABSORB_DESC",
"FORCE_HEALS_DESC",
"FORCE_PROTECT_DESC",
"FORCE_MIND_TRICK_DESC",
"FORCE_HOLYSTUN_DESC",
"WIND_GLYPH_DESC",
"HOLY_GLYPH_DESC",
"FORCE_JUMP_DESC",
"FORCE_SPEED_DESC",
"FORCE_PUSH_DESC",
"FORCE_PULL_DESC",
"FORCE_SABER_THROW_DESC",
"FORCE_SABER_DEFENSE_DESC",
"FORCE_SABER_OFFENSE_DESC",
"FORCE_SAREVEL_DESC",
"FORCE_HOLYFREEZE_DESC",
"FORCE_NECRODRAIN_DESC",
"FORCE_FIREBALL_DESC",
"FORCE_BERSERK_DESC",
"FORCE_GRIP_DESC",
"FORCE_HOLYHATE_DESC",
"FORCE_HOLYCONTROLMIND_DESC",
"FORCE_HOLYFEAR_DESC", 
"FORCE_HOLYWRACK_DESC",
"FORCE_GLYPH_DESC",
"STONE_GLYPH_DESC",
"SOUND_GLYPH_DESC",
"WATER_GLYPH_DESC",
"FIRE_GLYPH_DESC",
"SUN_GLYPH_DESC",
"DARK_GLYPH_DESC",
"NECRO_GLYPH_DESC",
};

const char *forcepowerDesc36[NUM_FORCE_POWERS] = 
{
"FORCE_ABSORB_DESC",
"FORCE_HEALV_DESC",
"FORCE_PROTECT_DESC",
"FORCE_LIONFORM_DESC",
"FORCE_STUN_DESC",
"MINDREAVER_DESC",
"SOULREAVER_DESC",
"FORCE_JUMP_DESC",
"FORCE_SPEED2_DESC",
"FORCE_PUSH_DESC",
"FORCE_PULL_DESC",
"FORCE_SABER_THROW_DESC",
"FORCE_SABER_DEFENSE_DESC",
"FORCE_SABER_OFFENSE_DESC",
"FORCE_SEEING_DESC",
"FORCE_FREEZE_DESC",
"FORCE_DRAIN_DESC",
"FORCE_LIGHTNING_DESC",
"FORCE_RAGE_DESC",
"FORCE_GRIP_DESC",
"FORCE_HATE_DESC",
"FORCE_CONTROLMIND_DESC",
"FORCE_CHYROPTERA_DESC", 
"FORCE_WRACK_DESC",
"FORCE_MISTFORM_DESC",
"TIMEREAVER_DESC",
"DIMENSIONREAVER_DESC",
"STATESREAVER_DESC",
"CONFLICTREAVER_DESC",
"ENERGYREAVER_DESC",
"NATUREREAVER_DESC",
"BLOODREAVER_DESC",
};

const char *forcepowerLvl1Desc[NUM_FORCE_POWERS] = 
{
"FORCE_ABSORB_LVL1_DESC",
"FORCE_HEAL_LVL1_DESC",
"FORCE_PROTECT_LVL1_DESC",
"FORCE_MIND_TRICK_LVL1_DESC",
"FORCE_STUN_LVL1_DESC",
"WIND_GLYPH_LVL1_DESC",
"HOLY_GLYPH_LVL1_DESC",

"FORCE_JUMP_LVL1_DESC",
"FORCE_SPEED_LVL1_DESC",
"FORCE_PUSH_LVL1_DESC",
"FORCE_PULL_LVL1_DESC",
"FORCE_SABER_THROW_LVL1_DESC",
"FORCE_SABER_DEFENSE_LVL1_DESC",
"FORCE_SABER_OFFENSE_LVL1_DESC",
"FORCE_SENSE_LVL1_DESC",

"FORCE_DRAIN_LVL1_DESC",
"FORCE_LIGHTNING_LVL1_DESC",
"FORCE_RAGE_LVL1_DESC",
"FORCE_GRIP_LVL1_DESC",

"FORCE_HATE_LVL1_DESC",
"FORCE_CONTROLMIND_LVL1_DESC",
"FORCE_FREEZE_LVL1_DESC",
"FORCE_HEALOTHER_LVL1_DESC",
"FORCE_WRACK_LVL1_DESC",
"FORCE_GLYPH_LVL1_DESC",
"STONE_GLYPH_LVL1_DESC",
"SOUND_GLYPH_LVL1_DESC",
"WATER_GLYPH_LVL1_DESC",
"FIRE_GLYPH_LVL1_DESC",
"SUN_GLYPH_LVL1_DESC",
"DARK_GLYPH_LVL1_DESC",
"NECRO_GLYPH_LVL1_DESC",
};

const char *forcepowerLvl2Desc[NUM_FORCE_POWERS] = 
{
"FORCE_ABSORB_LVL2_DESC",
"FORCE_HEAL_LVL2_DESC",
"FORCE_PROTECT_LVL2_DESC",
"FORCE_MIND_TRICK_LVL2_DESC",
"FORCE_STUN_LVL2_DESC",
"WIND_GLYPH_LVL2_DESC",
"HOLY_GLYPH_LVL2_DESC",

"FORCE_JUMP_LVL2_DESC",
"FORCE_SPEED_LVL2_DESC",
"FORCE_PUSH_LVL2_DESC",
"FORCE_PULL_LVL2_DESC",
"FORCE_SABER_THROW_LVL2_DESC",
"FORCE_SABER_DEFENSE_LVL2_DESC",
"FORCE_SABER_OFFENSE_LVL2_DESC",
"FORCE_SENSE_LVL2_DESC",

"FORCE_DRAIN_LVL2_DESC",
"FORCE_LIGHTNING_LVL2_DESC",
"FORCE_RAGE_LVL2_DESC",
"FORCE_GRIP_LVL2_DESC",

"FORCE_HATE_LVL2_DESC",
"FORCE_CONTROLMIND_LVL2_DESC",
"FORCE_FREEZE_LVL2_DESC",
"FORCE_HEALOTHER_LVL2_DESC",
"FORCE_WRACK_LVL2_DESC",
"FORCE_GLYPH_LVL2_DESC",
"STONE_GLYPH_LVL2_DESC",
"SOUND_GLYPH_LVL2_DESC",
"WATER_GLYPH_LVL2_DESC",
"FIRE_GLYPH_LVL2_DESC",
"SUN_GLYPH_LVL2_DESC",
"DARK_GLYPH_LVL2_DESC",
"NECRO_GLYPH_LVL2_DESC",
};

const char *forcepowerLvl3Desc[NUM_FORCE_POWERS] = 
{
"FORCE_ABSORB_LVL3_DESC",
"FORCE_HEAL_LVL3_DESC",
"FORCE_PROTECT_LVL3_DESC",
"FORCE_MIND_TRICK_LVL3_DESC",
"FORCE_STUN_LVL3_DESC",
"WIND_GLYPH_LVL3_DESC",
"HOLY_GLYPH_LVL3_DESC",

"FORCE_JUMP_LVL3_DESC",
"FORCE_SPEED_LVL3_DESC",
"FORCE_PUSH_LVL3_DESC",
"FORCE_PULL_LVL3_DESC",
"FORCE_SABER_THROW_LVL3_DESC",
"FORCE_SABER_DEFENSE_LVL3_DESC",
"FORCE_SABER_OFFENSE_LVL3_DESC",
"FORCE_SENSE_LVL3_DESC",

"FORCE_DRAIN_LVL3_DESC",
"FORCE_LIGHTNING_LVL3_DESC",
"FORCE_RAGE_LVL3_DESC",
"FORCE_GRIP_LVL3_DESC",


"FORCE_HATE_LVL3_DESC",
"FORCE_CONTROLMIND_LVL3_DESC",
"FORCE_FREEZE_LVL3_DESC",
"FORCE_HEALOTHER_LVL3_DESC",
"FORCE_WRACK_LVL3_DESC",
"FORCE_GLYPH_LVL3_DESC",
"STONE_GLYPH_LVL3_DESC",
"SOUND_GLYPH_LVL3_DESC",
"WATER_GLYPH_LVL3_DESC",
"FIRE_GLYPH_LVL3_DESC",
"SUN_GLYPH_LVL3_DESC",
"DARK_GLYPH_LVL3_DESC",
"NECRO_GLYPH_LVL3_DESC",

};

/*
===================
CG_DrawDataPadForceSelect
===================
*/
void CG_DrawDataPadForceSelect( void ) 
{
	int		i;
	int		count;
	int		holdX;
	int		sideLeftIconCnt,sideRightIconCnt;
	int		holdCount,iconCnt;
	char	text[1024]={0};
	char	text2[1024]={0};

	// count the number of powers known
	count = 0;
	for (i=0;i<MAX_DPSHOWPOWERS;i++)
	{
		if (ForcePowerDataPad_Valid(i))
		{
			count++;
		}
	}

	if (count < 1)	// If no force powers, don't display
	{
		return;
	}


	// Time to switch new icon colors
	// Faded side icon color
//	memcpy(fadeColor, colorTable[CT_WHITE], sizeof(vec4_t));

	cg.iconSelectTime = cg.forcepowerSelectTime;

	const int sideMax = 3;	// Max number of icons on the side

	// Calculate how many icons will appear to either side of the center one
	holdCount = count - 1;	// -1 for the center icon
	if (holdCount == 0)			// No icons to either side
	{
		sideLeftIconCnt = 0;
		sideRightIconCnt = 0;
	}
	else if (count > (2*sideMax))	// Go to the max on each side
	{
		sideLeftIconCnt = sideMax;
		sideRightIconCnt = sideMax;
	}
	else							// Less than max, so do the calc
	{
		sideLeftIconCnt = holdCount/2;
		sideRightIconCnt = holdCount - sideLeftIconCnt;
	}


	const int smallIconSize = 40;
	const int bigIconSize = 70;
	const int bigPad = 64;
	const int pad = 32;

	const int centerXPos = 320;
	const int graphicYPos = 340;

	i = cg.DataPadforcepowerSelect - 1;
	if (i < 0)
	{
		i = MAX_DPSHOWPOWERS-1;
	}

	// Print icons to the left of the center 

	cgi_R_SetColor(colorTable[CT_WHITE]);
	// Work backwards from current icon
	holdX = centerXPos - ((bigIconSize/2) + bigPad + smallIconSize);
	for (iconCnt=1;iconCnt<(sideLeftIconCnt+1);i--)
	{
		if (i < 0)
		{
			i = MAX_DPSHOWPOWERS-1;
		}

		if (!ForcePowerDataPad_Valid(i))	// Does he have this power?
		{
			continue;
		}

		++iconCnt;					// Good icon

	if ( player->client->NPC_class == CLASS_DARKREAVER ||
		player->client->NPC_class == CLASS_SABOTEUR ||
		player->client->NPC_class == CLASS_DARK ||
		player->client->NPC_class == CLASS_GOLEM_DARK )
	{
		if (force_icons[showDataPadPowers[i]])
		{
			CG_DrawPic( holdX, graphicYPos, smallIconSize, smallIconSize, force_icons1[showDataPadPowers[i]] );
		}
	}
	else if ( player->client->NPC_class == CLASS_TANKREAVER ||
	player->client->NPC_class == CLASS_BESPIN_COP ||
	player->client->NPC_class == CLASS_GOLEM_SMOKE )
	{
		if (force_icons[showDataPadPowers[i]])
		{
			CG_DrawPic( holdX, graphicYPos, smallIconSize, smallIconSize, force_icons2[showDataPadPowers[i]] );
		}
	}
	else if ( player->client->NPC_class == CLASS_SHADOWTROOPER ||
	player->client->NPC_class == CLASS_VOID || 
	player->client->NPC_class == CLASS_GOLEM_VOID )
	{
		if (force_icons[showDataPadPowers[i]])
		{
			CG_DrawPic( holdX, graphicYPos, smallIconSize, smallIconSize, force_icons3[showDataPadPowers[i]] );
		}
	}
	else if ( player->client->NPC_class == CLASS_LIGHTREAVER ||
		player->client->NPC_class == CLASS_SABER_DROID ||
		player->client->NPC_class == CLASS_ASSASSIN_DROID ||
		player->client->NPC_class == CLASS_LIGHT ||
		player->client->NPC_class == CLASS_GOLEM_LIGHT )
	{
		if (force_icons[showDataPadPowers[i]])
		{
			CG_DrawPic( holdX, graphicYPos, smallIconSize, smallIconSize, force_icons4[showDataPadPowers[i]] );
		}
	}
	else if ( player->client->NPC_class == CLASS_MORGANKATARN ||
		player->client->NPC_class == CLASS_GOLEM_GOLD ||
		player->client->NPC_class == CLASS_PROTOCOL ||
		player->client->NPC_class == CLASS_ATST )
	{
		if (force_icons[showDataPadPowers[i]])
		{
			CG_DrawPic( holdX, graphicYPos, smallIconSize, smallIconSize, force_icons5[showDataPadPowers[i]] );
		}
	}
	else if ( player->client->NPC_class == CLASS_FIREREAVER || 
		player->client->NPC_class == CLASS_GOLEM_FIRE || 
		player->client->NPC_class == CLASS_FIRE )
	{
		if (force_icons[showDataPadPowers[i]])
		{
			CG_DrawPic( holdX, graphicYPos, smallIconSize, smallIconSize, force_icons6[showDataPadPowers[i]] );
		}
	}
	else if ( player->client->NPC_class == CLASS_CLAW || player->client->NPC_class == CLASS_CLAW || player->client->NPC_class == CLASS_GOLEM_LAVA )
	{
		if (force_icons[showDataPadPowers[i]])
		{
			CG_DrawPic( holdX, graphicYPos, smallIconSize, smallIconSize, force_icons7[showDataPadPowers[i]] );
		}
	}
	else if ( player->client->NPC_class == CLASS_REBEL || 
		player->client->NPC_class == CLASS_NAERYANMIST ||
				player->client->NPC_class == CLASS_PROBE || 
				player->client->NPC_class == CLASS_AIR ||
				player->client->NPC_class == CLASS_GOLEM_AIR ||
				player->client->NPC_class == CLASS_BIRD 	)
	{
		if (force_icons[showDataPadPowers[i]])
		{
			CG_DrawPic( holdX, graphicYPos, smallIconSize, smallIconSize, force_icons8[showDataPadPowers[i]] );
		}
	}
	else if ( player->client->NPC_class == CLASS_AIRREAVER || player->client->NPC_class == CLASS_POLTER ||
		player->client->NPC_class == CLASS_NAERYANMIST2 )
	{
		if (force_icons[showDataPadPowers[i]])
		{
			CG_DrawPic( holdX, graphicYPos, smallIconSize, smallIconSize, force_icons9[showDataPadPowers[i]] );
		}
	}
	else if ( player->client->NPC_class == CLASS_ROCKETTROOPER ||
		player->client->NPC_class == CLASS_NAERYANMIST3 ||
			 player->client->NPC_class == CLASS_GOLEM_METAL ||
			  player->client->NPC_class == CLASS_DEMON_ELECTRO ||
			   player->client->NPC_class == CLASS_BOBAFETT )
	{
		if (force_icons[showDataPadPowers[i]])
		{
			CG_DrawPic( holdX, graphicYPos, smallIconSize, smallIconSize, force_icons10[showDataPadPowers[i]] );
		}
	}
	else if ( player->client->NPC_class == CLASS_VAMPIRE_SOUND ||
		player->client->NPC_class == CLASS_SENTRY || 
		player->client->NPC_class == CLASS_HOWLER ||
		player->client->NPC_class == CLASS_SONIC ||
		player->client->NPC_class == CLASS_GOLEM_SONIC )
	{
		if (force_icons[showDataPadPowers[i]])
		{
			CG_DrawPic( holdX, graphicYPos, smallIconSize, smallIconSize, force_icons11[showDataPadPowers[i]] );
		}
	}
	else if ( player->client->NPC_class == CLASS_BARTENDER || 
		player->client->NPC_class == CLASS_ICEREAVER ||
		player->client->NPC_class == CLASS_GOLEM_ICE )
	{
		if (force_icons[showDataPadPowers[i]])
		{
			CG_DrawPic( holdX, graphicYPos, smallIconSize, smallIconSize, force_icons12[showDataPadPowers[i]] );
		}
	}
	else if ( player->client->NPC_class == CLASS_CRYSTAL ||
		player->client->NPC_class == CLASS_GALAKMECH )
	{
		if (force_icons[showDataPadPowers[i]])
		{
			CG_DrawPic( holdX, graphicYPos, smallIconSize, smallIconSize, force_icons13[showDataPadPowers[i]] );
		}
	}
	else if ( player->client->NPC_class == CLASS_SPIDERREAVER )
	{
		if (force_icons[showDataPadPowers[i]])
		{
			CG_DrawPic( holdX, graphicYPos, smallIconSize, smallIconSize, force_icons14[showDataPadPowers[i]] );
		}
	}
	else if ( player->client->NPC_class == CLASS_SPIDERREAVER2 || player->client->NPC_class == CLASS_UGNAUGHT )
	{
		if (force_icons[showDataPadPowers[i]])
		{
			CG_DrawPic( holdX, graphicYPos, smallIconSize, smallIconSize, force_icons15[showDataPadPowers[i]] );
		}
	}
	else if ( player->client->NPC_class == CLASS_EARTHREAVER || 
				player->client->NPC_class == CLASS_LIZARD || 
				player->client->NPC_class == CLASS_EARTH || 
				player->client->NPC_class == CLASS_GONK ||
				player->client->NPC_class == CLASS_DEMON_BLADE || 
				player->client->NPC_class == CLASS_GOLEM )
	{
		if (force_icons[showDataPadPowers[i]])
		{
			CG_DrawPic( holdX, graphicYPos, smallIconSize, smallIconSize, force_icons16[showDataPadPowers[i]] );
		}
	}
	else if ( player->client->NPC_class == CLASS_SPIDERREAVER3 || player->client->NPC_class == CLASS_REELO )
	{
		if (force_icons[showDataPadPowers[i]])
		{
			CG_DrawPic( holdX, graphicYPos, smallIconSize, smallIconSize, force_icons17[showDataPadPowers[i]] );
		}
	}
	else if ( player->client->NPC_class == CLASS_NECROREAVER )
	{
		if (force_icons[showDataPadPowers[i]])
		{
			CG_DrawPic( holdX, graphicYPos, smallIconSize, smallIconSize, force_icons18[showDataPadPowers[i]] );
		}
	}
	else if ( player->client->NPC_class == CLASS_MURJJ ||
		player->client->NPC_class == CLASS_FISH || 
		player->client->NPC_class == CLASS_NECRO )
	{
		if (force_icons[showDataPadPowers[i]])
		{
			CG_DrawPic( holdX, graphicYPos, smallIconSize, smallIconSize, force_icons19[showDataPadPowers[i]] );
		}
	}
	else if ( player->client->NPC_class == CLASS_RAZIEL ||
		player->client->NPC_class == CLASS_MENTALIST )
	{
		if (force_icons[showDataPadPowers[i]])
		{
			CG_DrawPic( holdX, graphicYPos, smallIconSize, smallIconSize, force_icons20[showDataPadPowers[i]] );
		}
	}
	else if ( player->client->NPC_class == CLASS_HOLY ||
		player->client->NPC_class == CLASS_GOLEM_HOLY )
	{
		if (force_icons[showDataPadPowers[i]])
		{
			CG_DrawPic( holdX, graphicYPos, smallIconSize, smallIconSize, force_icons21[showDataPadPowers[i]] );
		}
	}
	else if ( player->client->NPC_class == CLASS_LUKE ) 
	{
		if (force_icons[showDataPadPowers[i]])
		{
			CG_DrawPic( holdX, graphicYPos, smallIconSize, smallIconSize, force_icons22[showDataPadPowers[i]] );
		}
	}
	else if ( player->client->NPC_class == CLASS_DRUIDREAVER ||
		player->client->NPC_class == CLASS_TRANDOSHAN ||
		player->client->NPC_class == CLASS_LYCAN ||
		player->client->NPC_class == CLASS_DRUID ) 
	{
		if (force_icons[showDataPadPowers[i]])
		{
			CG_DrawPic( holdX, graphicYPos, smallIconSize, smallIconSize, force_icons23[showDataPadPowers[i]] );
		}
	}
	else if ( player->client->NPC_class == CLASS_LANDO ) 
	{
		if (force_icons[showDataPadPowers[i]])
		{
			CG_DrawPic( holdX, graphicYPos, smallIconSize, smallIconSize, force_icons24[showDataPadPowers[i]] );
		}
	}
	else if ( player->client->NPC_class == CLASS_VAMPIRE ||
		player->client->NPC_class == CLASS_REBORN ||	
		player->client->NPC_class == CLASS_KAIN ||
		player->client->NPC_class == CLASS_KAIN2 ||
		player->client->NPC_class == CLASS_UMAH ||
		player->client->NPC_class == CLASS_VAMPIRE_DARK ||
		player->client->NPC_class == CLASS_VAMPIRE_NECRO ||
		player->client->NPC_class == CLASS_VAMPIRE_SARAFAN ||
		player->client->NPC_class == CLASS_HALFVAMPIRE ||
		player->client->NPC_class == CLASS_RAZIELHIM ||
		player->client->NPC_class == CLASS_MONMOTHA ||		
		player->client->NPC_class == CLASS_VAMPIRE_ELF ||
		player->client->NPC_class == CLASS_ALORA ||
		player->client->NPC_class == CLASS_VAMPIRE_WATER ||
		player->client->NPC_class == CLASS_VAMPIRE_LYCAN ||
		player->client->NPC_class == CLASS_VAMPIRE_ARCHER ||
		player->client->NPC_class == CLASS_VAMPIRE_BOUNTY ||
		player->client->NPC_class == CLASS_VAMPIRE_BLOOD ||
		player->client->NPC_class == CLASS_DUMAHIM ||
		player->client->NPC_class == CLASS_WEEQUAY ||
		player->client->NPC_class == CLASS_GRAN ||
		player->client->NPC_class == CLASS_TURELIM ) 
	{
		if (force_icons[showDataPadPowers[i]])
		{
			CG_DrawPic( holdX, graphicYPos, smallIconSize, smallIconSize, force_icons25[showDataPadPowers[i]] );
		}
	}
	else if ( player->client->NPC_class == CLASS_VAMPIRE_FEAR || player->client->NPC_class == CLASS_VAMPIRE_FEAR2 )
	{
		if (force_icons[showDataPadPowers[i]])
		{
			CG_DrawPic( holdX, graphicYPos, smallIconSize, smallIconSize, force_icons26[showDataPadPowers[i]] );
		}
	}
	else if ( player->client->NPC_class == CLASS_KAIN3 ||
		player->client->NPC_class == CLASS_KYLE ||
		player->client->NPC_class == CLASS_VAMPIRE_FROST ||
		player->client->NPC_class == CLASS_NECROMANCER ||
		player->client->NPC_class == CLASS_VAMPIRE_QUEEN ||
		player->client->NPC_class == CLASS_VORADOR ||
		player->client->NPC_class == CLASS_JANOS ||
		player->client->NPC_class == CLASS_VAMPIRE_NECRO2 )
	{
		if (force_icons[showDataPadPowers[i]])
		{
			CG_DrawPic( holdX, graphicYPos, smallIconSize, smallIconSize, force_icons27[showDataPadPowers[i]] );
		}
	}
	else if (  player->client->NPC_class == CLASS_SWAMP || player->client->NPC_class == CLASS_RAHABIM ||
						player->client->NPC_class == CLASS_SHARK || player->client->NPC_class == CLASS_SWAMPTROOPER || 
						player->client->NPC_class == CLASS_WATER || player->client->NPC_class == CLASS_GOLEM_WATER )
	{
		if (force_icons[showDataPadPowers[i]])
		{
			CG_DrawPic( holdX, graphicYPos, smallIconSize, smallIconSize, force_icons28[showDataPadPowers[i]] );
		}
	}
	else if ( player->client->NPC_class == CLASS_VAMPIRE_ZOMBIE || player->client->NPC_class == CLASS_TUSKEN || 
		player->client->NPC_class == CLASS_MELC || player->client->NPC_class == CLASS_SUMMONER || 
		player->client->NPC_class == CLASS_THRALL || 	player->client->NPC_class == CLASS_GLIDER || 
		player->client->NPC_class == CLASS_COMMANDO || player->client->NPC_class == CLASS_GOLEM_NECRO )
	{
		if (force_icons[showDataPadPowers[i]])
		{
			CG_DrawPic( holdX, graphicYPos, smallIconSize, smallIconSize, force_icons29[showDataPadPowers[i]] );
		}
	}
	else if ( player->client->NPC_class == CLASS_VAMPIRE_DEMON ||
		player->client->NPC_class == CLASS_WAMPA || 
		player->client->NPC_class == CLASS_DEMON_FIRE || 
		player->client->NPC_class == CLASS_CENOBITA || 
		player->client->NPC_class == CLASS_HELLGOLEM || 
		player->client->NPC_class == CLASS_HAZARD_TROOPER || 
		player->client->NPC_class == CLASS_IMPERIAL)
	{
		if (force_icons[showDataPadPowers[i]])
		{
			CG_DrawPic( holdX, graphicYPos, smallIconSize, smallIconSize, force_icons30[showDataPadPowers[i]] );
		}
	}
	else if ( player->client->NPC_class == CLASS_WOLFFORM ||
		 player->client->NPC_class == CLASS_WOLFKAIN ||
		 player->client->NPC_class == CLASS_WOLFKAIN2 ||
		 player->client->NPC_class == CLASS_WOLFKAIN3 ||
		 player->client->NPC_class == CLASS_WOLFVORADOR ||
		 player->client->NPC_class == CLASS_WOLFJANOS || 
		 player->client->NPC_class == CLASS_WOLFNECROMANCER ||
		 player->client->NPC_class == CLASS_WOLFVAMPIRE_QUEEN ||
		 player->client->NPC_class == CLASS_WOLFVAMPIRE_FEAR ||
		 player->client->NPC_class == CLASS_WOLFVAMPIRE_FEAR2 ||
		 player->client->NPC_class == CLASS_HELLWOLF ||
		 player->client->NPC_class == CLASS_WOLFVAMPIRE_LYCAN ||
		 player->client->NPC_class == CLASS_WOLFVAMPIRE_ELF ||
		 player->client->NPC_class == CLASS_LIONVAMPIRE_ELF ||
		 player->client->NPC_class == CLASS_LIONVAMPIRE_ELF2 || 
		 player->client->NPC_class == CLASS_WOLFHALFVAMPIRE ||
		 player->client->NPC_class == CLASS_ESKANDOR || 
		 // BAT FORM  
		 player->client->NPC_class == CLASS_BATFORM ||
		 player->client->NPC_class == CLASS_BATKAIN ||
		 player->client->NPC_class == CLASS_BATKAIN2 ||
		 player->client->NPC_class == CLASS_BATKAIN3 ||
		 player->client->NPC_class == CLASS_BATVORADOR ||
		 player->client->NPC_class == CLASS_BATVAMPIRE_FEAR ||
		 player->client->NPC_class == CLASS_BATVAMPIRE_FEAR2 || 
		 player->client->NPC_class == CLASS_BATVAMPIRE_QUEEN ||
		 player->client->NPC_class == CLASS_BATNECROMANCER ||
		 player->client->NPC_class == CLASS_BATVAMPIRE_FROST ||
		player->client->NPC_class == CLASS_HELLBAT || 
		 player->client->NPC_class == CLASS_BATVAMPIRE_WATER ||
		 player->client->NPC_class == CLASS_BATVAMPIRE_ELF ||
		 player->client->NPC_class == CLASS_BATVAMPIRE_ELF2 ||
		 player->client->NPC_class == CLASS_BATVAMPIRE_ELF3 ||
	// MIST FORM 
		player->client->NPC_class == CLASS_MISTFORM || 
		player->client->NPC_class == CLASS_MISTKAIN ||
		 player->client->NPC_class == CLASS_MISTKAIN2 ||
		 player->client->NPC_class == CLASS_MISTKAIN3 ||
		 player->client->NPC_class == CLASS_MISTVORADOR ||
		 player->client->NPC_class == CLASS_MISTJANOS || 
		 player->client->NPC_class == CLASS_MISTNECROMANCER ||
		 player->client->NPC_class == CLASS_MISTVAMPIRE_QUEEN ||
		 player->client->NPC_class == CLASS_MISTVAMPIRE_FEAR ||
		 player->client->NPC_class == CLASS_MISTVAMPIRE_FEAR2 ||
		 player->client->NPC_class == CLASS_HELLMIST ||
		 player->client->NPC_class == CLASS_MISTVAMPIRE_DARK ||
		 player->client->NPC_class == CLASS_MISTVAMPIRE_BLOOD ||
		 player->client->NPC_class == CLASS_MISTVAMPIRE_WATER ||
		  player->client->NPC_class == CLASS_MISTVAMPIRE_SARAFAN ||
		 player->client->NPC_class == CLASS_MISTVAMPIRE_ELF ||
		 player->client->NPC_class == CLASS_MISTVAMPIRE_ELF2 ||
		 player->client->NPC_class == CLASS_MISTVAMPIRE_ELF3 )
	{
		if (force_icons[showDataPadPowers[i]])
		{
			CG_DrawPic( holdX, graphicYPos, smallIconSize, smallIconSize, force_icons31[showDataPadPowers[i]] );
		}
	}
	// MINEMONSTER - Shade class?
	else if ( player->client->NPC_class == CLASS_MINEMONSTER || 
	  player->client->NPC_class == CLASS_POISON ||
	  player->client->NPC_class == CLASS_IVY || 
	  player->client->NPC_class == CLASS_DEMON_POISON ||
      player->client->NPC_class == CLASS_GOLEM_POISON || 
	  player->client->NPC_class == CLASS_RANCOR || 
	  player->client->NPC_class == CLASS_INTERROGATOR )
	 {
		if (force_icons[showDataPadPowers[i]])
		{
			CG_DrawPic( holdX, graphicYPos, smallIconSize, smallIconSize, force_icons32[showDataPadPowers[i]] );
		}
	}
	else if ( player->client->NPC_class == CLASS_TAVION ||
		player->client->NPC_class == CLASS_HYLDEN ||
		player->client->NPC_class == CLASS_DESANN ||
		player->client->NPC_class == CLASS_HYLDENLORD )
	{
		if (force_icons[showDataPadPowers[i]])
		{
			CG_DrawPic( holdX, graphicYPos, smallIconSize, smallIconSize, force_icons33[showDataPadPowers[i]] );
		}
	}
	else if ( player->client->NPC_class == CLASS_NOGHRI || player->client->NPC_class == CLASS_ZEPH ||
		player->client->NPC_class == CLASS_DECEIVER || player->client->NPC_class == CLASS_FLIER2 ) 
	
	{
		if (force_icons[showDataPadPowers[i]])
		{
			CG_DrawPic( holdX, graphicYPos, smallIconSize, smallIconSize, force_icons34[showDataPadPowers[i]] );
		}
	}
	else if ( player->client->NPC_class == CLASS_JAN || player->client->NPC_class == CLASS_WARRIOR ||
		player->client->NPC_class == CLASS_STORMTROOPER ||player->client->NPC_class == CLASS_SARAFAN ||
		player->client->NPC_class == CLASS_PROPHET || player->client->NPC_class == CLASS_GOLEM_HOLY ||
		
		// Unused but i place however for control mind and wrack 
		player->client->NPC_class == CLASS_PRISONER || player->client->NPC_class == CLASS_RODIAN ||
		player->client->NPC_class == CLASS_ARCHER ||player->client->NPC_class == CLASS_HUNTER ||
		player->client->NPC_class == CLASS_SCOUT || player->client->NPC_class == CLASS_ALCHEMIST ||
		player->client->NPC_class == CLASS_GALAK || player->client->NPC_class == CLASS_WIZARD ||
		player->client->NPC_class == CLASS_IMPWORKER ||player->client->NPC_class == CLASS_BEAST )
	{
		if (force_icons[showDataPadPowers[i]])
		{
			CG_DrawPic( holdX, graphicYPos, smallIconSize, smallIconSize, force_icons35[showDataPadPowers[i]] );
		}
	}
	else if ( player->client->NPC_class == CLASS_VAMPIRE_ELF2 ||
		player->client->NPC_class == CLASS_VAMPIRE_ELF3 )
 	{
		if (force_icons[showDataPadPowers[i]])
		{
			CG_DrawPic( holdX, graphicYPos, smallIconSize, smallIconSize, force_icons36[showDataPadPowers[i]] );
		}
	}
	// UNUSED
	// ARTIFACTS 
	else if ( player->client->NPC_class == CLASS_R2D2 ||
		player->client->NPC_class == CLASS_R5D2 ||
		player->client->NPC_class == CLASS_SEEKER ||
		player->client->NPC_class == CLASS_REMOTE )		
 	{
		if (force_icons[showDataPadPowers[i]])
		{
			CG_DrawPic( holdX, graphicYPos, smallIconSize, smallIconSize, force_icons[showDataPadPowers[i]] );
		}
	}
	else if ( player->client->NPC_class == CLASS_ELDERGOD ||
		player->client->NPC_class == CLASS_SAND_CREATURE || 
		player->client->NPC_class == CLASS_TENTACLE )
 	{
		if (force_icons[showDataPadPowers[i]])
		{
			CG_DrawPic( holdX, graphicYPos, smallIconSize, smallIconSize, force_icons[showDataPadPowers[i]] );
		}
	}
	else 
	{
		if (force_icons[showDataPadPowers[i]])
		{
			CG_DrawPic( holdX, graphicYPos, smallIconSize, smallIconSize, force_icons[showDataPadPowers[i]] );
		}
	}
		
		/*if (force_icons[showDataPadPowers[i]])
		{
			CG_DrawPic( holdX, graphicYPos, smallIconSize, smallIconSize, force_icons[showDataPadPowers[i]] ); 
		}*/

		// A new force power
		if (((cg_updatedDataPadForcePower1.integer - 1) == showDataPadPowers[i]) ||
			((cg_updatedDataPadForcePower2.integer - 1) == showDataPadPowers[i]) ||
			((cg_updatedDataPadForcePower3.integer - 1) == showDataPadPowers[i]))
		{
			CG_DrawPic( holdX, graphicYPos, smallIconSize, smallIconSize, cgs.media.DPForcePowerOverlay ); 
		}

		if (force_icons[showDataPadPowers[i]])
		{
			holdX -= (smallIconSize+pad);
		}
	}

	// Current Center Icon
	if (force_icons[showDataPadPowers[cg.DataPadforcepowerSelect]])
	{

		cgi_R_SetColor(colorTable[CT_WHITE]);
		
		//CG_DrawPic( centerXPos-(bigIconSize/2), (graphicYPos-((bigIconSize-smallIconSize)/2)), bigIconSize, bigIconSize, force_icons[showDataPadPowers[cg.DataPadforcepowerSelect]] );

	if ( player->client->NPC_class == CLASS_DARKREAVER ||
		player->client->NPC_class == CLASS_SABOTEUR ||
		player->client->NPC_class == CLASS_DARK ||
		player->client->NPC_class == CLASS_GOLEM_DARK )
	{
		CG_DrawPic( centerXPos-(bigIconSize/2), (graphicYPos-((bigIconSize-smallIconSize)/2)), bigIconSize, bigIconSize, force_icons1[showDataPadPowers[cg.DataPadforcepowerSelect]] );
	}
	else if ( player->client->NPC_class == CLASS_TANKREAVER ||
	player->client->NPC_class == CLASS_BESPIN_COP ||
	player->client->NPC_class == CLASS_GOLEM_SMOKE )
	{
		CG_DrawPic( centerXPos-(bigIconSize/2), (graphicYPos-((bigIconSize-smallIconSize)/2)), bigIconSize, bigIconSize, force_icons2[showDataPadPowers[cg.DataPadforcepowerSelect]] );
	}
	else if ( player->client->NPC_class == CLASS_SHADOWTROOPER ||
	player->client->NPC_class == CLASS_VOID || 
	player->client->NPC_class == CLASS_GOLEM_VOID )
	{
		CG_DrawPic( centerXPos-(bigIconSize/2), (graphicYPos-((bigIconSize-smallIconSize)/2)), bigIconSize, bigIconSize, force_icons3[showDataPadPowers[cg.DataPadforcepowerSelect]] );
	}
	else if ( player->client->NPC_class == CLASS_LIGHTREAVER ||
		player->client->NPC_class == CLASS_SABER_DROID ||
		player->client->NPC_class == CLASS_ASSASSIN_DROID ||
		player->client->NPC_class == CLASS_LIGHT ||
		player->client->NPC_class == CLASS_GOLEM_LIGHT )
	{
		CG_DrawPic( centerXPos-(bigIconSize/2), (graphicYPos-((bigIconSize-smallIconSize)/2)), bigIconSize, bigIconSize, force_icons4[showDataPadPowers[cg.DataPadforcepowerSelect]] );
	}
	else if ( player->client->NPC_class == CLASS_MORGANKATARN ||
		player->client->NPC_class == CLASS_GOLEM_GOLD ||
		player->client->NPC_class == CLASS_PROTOCOL ||
		player->client->NPC_class == CLASS_ATST )
	{
		CG_DrawPic( centerXPos-(bigIconSize/2), (graphicYPos-((bigIconSize-smallIconSize)/2)), bigIconSize, bigIconSize, force_icons5[showDataPadPowers[cg.DataPadforcepowerSelect]] );
	}
	else if ( player->client->NPC_class == CLASS_FIREREAVER || 
		player->client->NPC_class == CLASS_GOLEM_FIRE || 
		player->client->NPC_class == CLASS_FIRE )
	{
		CG_DrawPic( centerXPos-(bigIconSize/2), (graphicYPos-((bigIconSize-smallIconSize)/2)), bigIconSize, bigIconSize, force_icons6[showDataPadPowers[cg.DataPadforcepowerSelect]] );
	}
	else if ( player->client->NPC_class == CLASS_CLAW || player->client->NPC_class == CLASS_CLAW || player->client->NPC_class == CLASS_GOLEM_LAVA )
	{
		CG_DrawPic( centerXPos-(bigIconSize/2), (graphicYPos-((bigIconSize-smallIconSize)/2)), bigIconSize, bigIconSize, force_icons7[showDataPadPowers[cg.DataPadforcepowerSelect]] );
	}
	else if ( player->client->NPC_class == CLASS_REBEL || 
		player->client->NPC_class == CLASS_NAERYANMIST ||
				player->client->NPC_class == CLASS_PROBE || 
				player->client->NPC_class == CLASS_AIR ||
				player->client->NPC_class == CLASS_GOLEM_AIR ||
				player->client->NPC_class == CLASS_BIRD 	)
	{
		CG_DrawPic( centerXPos-(bigIconSize/2), (graphicYPos-((bigIconSize-smallIconSize)/2)), bigIconSize, bigIconSize, force_icons8[showDataPadPowers[cg.DataPadforcepowerSelect]] );
	}
	else if ( player->client->NPC_class == CLASS_AIRREAVER || player->client->NPC_class == CLASS_POLTER ||
		player->client->NPC_class == CLASS_NAERYANMIST2 )
	{
		CG_DrawPic( centerXPos-(bigIconSize/2), (graphicYPos-((bigIconSize-smallIconSize)/2)), bigIconSize, bigIconSize, force_icons9[showDataPadPowers[cg.DataPadforcepowerSelect]] );
	}
	else if ( player->client->NPC_class == CLASS_ROCKETTROOPER ||
		player->client->NPC_class == CLASS_NAERYANMIST3 ||
			 player->client->NPC_class == CLASS_GOLEM_METAL ||
			  player->client->NPC_class == CLASS_DEMON_ELECTRO ||
			   player->client->NPC_class == CLASS_BOBAFETT )
	{
		CG_DrawPic( centerXPos-(bigIconSize/2), (graphicYPos-((bigIconSize-smallIconSize)/2)), bigIconSize, bigIconSize, force_icons10[showDataPadPowers[cg.DataPadforcepowerSelect]] );
	}
	else if (  player->client->NPC_class == CLASS_VAMPIRE_SOUND ||
		player->client->NPC_class == CLASS_SENTRY || 
		player->client->NPC_class == CLASS_HOWLER ||
		player->client->NPC_class == CLASS_SONIC ||
		player->client->NPC_class == CLASS_GOLEM_SONIC )
	{
		CG_DrawPic( centerXPos-(bigIconSize/2), (graphicYPos-((bigIconSize-smallIconSize)/2)), bigIconSize, bigIconSize, force_icons11[showDataPadPowers[cg.DataPadforcepowerSelect]] );
	}
	else if ( player->client->NPC_class == CLASS_BARTENDER || 
		player->client->NPC_class == CLASS_ICEREAVER ||
		player->client->NPC_class == CLASS_GOLEM_ICE )
	{
		CG_DrawPic( centerXPos-(bigIconSize/2), (graphicYPos-((bigIconSize-smallIconSize)/2)), bigIconSize, bigIconSize, force_icons12[showDataPadPowers[cg.DataPadforcepowerSelect]] );
	}
	else if ( player->client->NPC_class == CLASS_CRYSTAL ||
		player->client->NPC_class == CLASS_GALAKMECH )
	{
		CG_DrawPic( centerXPos-(bigIconSize/2), (graphicYPos-((bigIconSize-smallIconSize)/2)), bigIconSize, bigIconSize, force_icons13[showDataPadPowers[cg.DataPadforcepowerSelect]] );
	}
	else if ( player->client->NPC_class == CLASS_SPIDERREAVER )
	{
		CG_DrawPic( centerXPos-(bigIconSize/2), (graphicYPos-((bigIconSize-smallIconSize)/2)), bigIconSize, bigIconSize, force_icons14[showDataPadPowers[cg.DataPadforcepowerSelect]] );
	}
	else if ( player->client->NPC_class == CLASS_SPIDERREAVER2 || player->client->NPC_class == CLASS_UGNAUGHT )
	{
		CG_DrawPic( centerXPos-(bigIconSize/2), (graphicYPos-((bigIconSize-smallIconSize)/2)), bigIconSize, bigIconSize, force_icons15[showDataPadPowers[cg.DataPadforcepowerSelect]] );
	}
	else if ( player->client->NPC_class == CLASS_EARTHREAVER || 
				player->client->NPC_class == CLASS_LIZARD || 
				player->client->NPC_class == CLASS_EARTH || 
				player->client->NPC_class == CLASS_GONK ||
				player->client->NPC_class == CLASS_DEMON_BLADE || 
				player->client->NPC_class == CLASS_GOLEM )
	{
		CG_DrawPic( centerXPos-(bigIconSize/2), (graphicYPos-((bigIconSize-smallIconSize)/2)), bigIconSize, bigIconSize, force_icons16[showDataPadPowers[cg.DataPadforcepowerSelect]] );
	}
	else if ( player->client->NPC_class == CLASS_SPIDERREAVER3 || player->client->NPC_class == CLASS_REELO )
	{
		CG_DrawPic( centerXPos-(bigIconSize/2), (graphicYPos-((bigIconSize-smallIconSize)/2)), bigIconSize, bigIconSize, force_icons17[showDataPadPowers[cg.DataPadforcepowerSelect]] );
	}
	else if ( player->client->NPC_class == CLASS_NECROREAVER )
	{
		CG_DrawPic( centerXPos-(bigIconSize/2), (graphicYPos-((bigIconSize-smallIconSize)/2)), bigIconSize, bigIconSize, force_icons18[showDataPadPowers[cg.DataPadforcepowerSelect]] );
	}
	else if ( player->client->NPC_class == CLASS_MURJJ ||
		player->client->NPC_class == CLASS_FISH || 
		player->client->NPC_class == CLASS_NECRO )
	{
		CG_DrawPic( centerXPos-(bigIconSize/2), (graphicYPos-((bigIconSize-smallIconSize)/2)), bigIconSize, bigIconSize, force_icons19[showDataPadPowers[cg.DataPadforcepowerSelect]] );
	}
	else if ( player->client->NPC_class == CLASS_RAZIEL ||
		player->client->NPC_class == CLASS_MENTALIST )
	{
		CG_DrawPic( centerXPos-(bigIconSize/2), (graphicYPos-((bigIconSize-smallIconSize)/2)), bigIconSize, bigIconSize, force_icons20[showDataPadPowers[cg.DataPadforcepowerSelect]] );
	}
	else if ( player->client->NPC_class == CLASS_HOLY ||
		player->client->NPC_class == CLASS_GOLEM_HOLY )
	{
		CG_DrawPic( centerXPos-(bigIconSize/2), (graphicYPos-((bigIconSize-smallIconSize)/2)), bigIconSize, bigIconSize, force_icons21[showDataPadPowers[cg.DataPadforcepowerSelect]] );
	}
	else if ( player->client->NPC_class == CLASS_LUKE ) 
	{
		CG_DrawPic( centerXPos-(bigIconSize/2), (graphicYPos-((bigIconSize-smallIconSize)/2)), bigIconSize, bigIconSize, force_icons22[showDataPadPowers[cg.DataPadforcepowerSelect]] );
	}
	else if ( player->client->NPC_class == CLASS_DRUIDREAVER ||
		player->client->NPC_class == CLASS_TRANDOSHAN ||
		player->client->NPC_class == CLASS_LYCAN ||
		player->client->NPC_class == CLASS_DRUID ) 
	{
		CG_DrawPic( centerXPos-(bigIconSize/2), (graphicYPos-((bigIconSize-smallIconSize)/2)), bigIconSize, bigIconSize, force_icons23[showDataPadPowers[cg.DataPadforcepowerSelect]] );
	}
	else if ( player->client->NPC_class == CLASS_LANDO ) 
	{
		CG_DrawPic( centerXPos-(bigIconSize/2), (graphicYPos-((bigIconSize-smallIconSize)/2)), bigIconSize, bigIconSize, force_icons24[showDataPadPowers[cg.DataPadforcepowerSelect]] );
	}
	else if ( player->client->NPC_class == CLASS_VAMPIRE ||
		player->client->NPC_class == CLASS_REBORN ||	
		player->client->NPC_class == CLASS_KAIN ||
		player->client->NPC_class == CLASS_KAIN2 ||
		player->client->NPC_class == CLASS_UMAH ||
		player->client->NPC_class == CLASS_VAMPIRE_DARK ||
		player->client->NPC_class == CLASS_VAMPIRE_NECRO ||
		player->client->NPC_class == CLASS_VAMPIRE_SARAFAN ||
		player->client->NPC_class == CLASS_HALFVAMPIRE ||
		player->client->NPC_class == CLASS_RAZIELHIM ||
		player->client->NPC_class == CLASS_MONMOTHA ||		
		player->client->NPC_class == CLASS_VAMPIRE_ELF ||
		player->client->NPC_class == CLASS_ALORA ||
		player->client->NPC_class == CLASS_VAMPIRE_WATER ||
		player->client->NPC_class == CLASS_VAMPIRE_LYCAN ||
		player->client->NPC_class == CLASS_VAMPIRE_ARCHER ||
		player->client->NPC_class == CLASS_VAMPIRE_BOUNTY ||
		player->client->NPC_class == CLASS_VAMPIRE_BLOOD ||
		player->client->NPC_class == CLASS_DUMAHIM ||
		player->client->NPC_class == CLASS_WEEQUAY ||
		player->client->NPC_class == CLASS_GRAN ||
		player->client->NPC_class == CLASS_TURELIM ) 
	{
		CG_DrawPic( centerXPos-(bigIconSize/2), (graphicYPos-((bigIconSize-smallIconSize)/2)), bigIconSize, bigIconSize, force_icons25[showDataPadPowers[cg.DataPadforcepowerSelect]] );
	}
	else if ( player->client->NPC_class == CLASS_VAMPIRE_FEAR || player->client->NPC_class == CLASS_VAMPIRE_FEAR2 )
	{
		CG_DrawPic( centerXPos-(bigIconSize/2), (graphicYPos-((bigIconSize-smallIconSize)/2)), bigIconSize, bigIconSize, force_icons26[showDataPadPowers[cg.DataPadforcepowerSelect]] );
	}
	else if ( player->client->NPC_class == CLASS_KAIN3 ||
		player->client->NPC_class == CLASS_KYLE ||
		player->client->NPC_class == CLASS_VAMPIRE_FROST ||
		player->client->NPC_class == CLASS_NECROMANCER ||
		player->client->NPC_class == CLASS_VAMPIRE_QUEEN ||
		player->client->NPC_class == CLASS_VORADOR ||
		player->client->NPC_class == CLASS_JANOS ||
		player->client->NPC_class == CLASS_VAMPIRE_NECRO2 )
	{
		CG_DrawPic( centerXPos-(bigIconSize/2), (graphicYPos-((bigIconSize-smallIconSize)/2)), bigIconSize, bigIconSize, force_icons27[showDataPadPowers[cg.DataPadforcepowerSelect]] );
	}
	else if (  player->client->NPC_class == CLASS_SWAMP || player->client->NPC_class == CLASS_RAHABIM ||
						player->client->NPC_class == CLASS_SHARK || player->client->NPC_class == CLASS_SWAMPTROOPER || 
						player->client->NPC_class == CLASS_WATER || player->client->NPC_class == CLASS_GOLEM_WATER )
	{
		CG_DrawPic( centerXPos-(bigIconSize/2), (graphicYPos-((bigIconSize-smallIconSize)/2)), bigIconSize, bigIconSize, force_icons28[showDataPadPowers[cg.DataPadforcepowerSelect]] );
	}
	else if ( player->client->NPC_class == CLASS_VAMPIRE_ZOMBIE || player->client->NPC_class == CLASS_TUSKEN || 
		player->client->NPC_class == CLASS_MELC || player->client->NPC_class == CLASS_SUMMONER || 
		player->client->NPC_class == CLASS_THRALL || 	player->client->NPC_class == CLASS_GLIDER || 
		player->client->NPC_class == CLASS_COMMANDO || player->client->NPC_class == CLASS_GOLEM_NECRO )
	{
		CG_DrawPic( centerXPos-(bigIconSize/2), (graphicYPos-((bigIconSize-smallIconSize)/2)), bigIconSize, bigIconSize, force_icons29[showDataPadPowers[cg.DataPadforcepowerSelect]] );
	}
	else if ( player->client->NPC_class == CLASS_VAMPIRE_DEMON ||
		player->client->NPC_class == CLASS_WAMPA || 
		player->client->NPC_class == CLASS_DEMON_FIRE || 
		player->client->NPC_class == CLASS_CENOBITA || 
		player->client->NPC_class == CLASS_HELLGOLEM || 
		player->client->NPC_class == CLASS_HAZARD_TROOPER || 
		player->client->NPC_class == CLASS_IMPERIAL)
	{
		CG_DrawPic( centerXPos-(bigIconSize/2), (graphicYPos-((bigIconSize-smallIconSize)/2)), bigIconSize, bigIconSize, force_icons30[showDataPadPowers[cg.DataPadforcepowerSelect]] );
	}
	else if ( player->client->NPC_class == CLASS_WOLFFORM ||
		 player->client->NPC_class == CLASS_WOLFKAIN ||
		 player->client->NPC_class == CLASS_WOLFKAIN2 ||
		 player->client->NPC_class == CLASS_WOLFKAIN3 ||
		 player->client->NPC_class == CLASS_WOLFVORADOR ||
		 player->client->NPC_class == CLASS_WOLFJANOS || 
		 player->client->NPC_class == CLASS_WOLFNECROMANCER ||
		 player->client->NPC_class == CLASS_WOLFVAMPIRE_QUEEN ||
		 player->client->NPC_class == CLASS_WOLFVAMPIRE_FEAR ||
		 player->client->NPC_class == CLASS_WOLFVAMPIRE_FEAR2 ||
		 player->client->NPC_class == CLASS_HELLWOLF ||
		 player->client->NPC_class == CLASS_WOLFVAMPIRE_LYCAN ||
		 player->client->NPC_class == CLASS_WOLFVAMPIRE_ELF ||
		 player->client->NPC_class == CLASS_LIONVAMPIRE_ELF ||
		 player->client->NPC_class == CLASS_LIONVAMPIRE_ELF2 || 
		 player->client->NPC_class == CLASS_WOLFHALFVAMPIRE ||
		 player->client->NPC_class == CLASS_ESKANDOR || 
		 // BAT FORM  
		 player->client->NPC_class == CLASS_BATFORM ||
		 player->client->NPC_class == CLASS_BATKAIN ||
		 player->client->NPC_class == CLASS_BATKAIN2 ||
		 player->client->NPC_class == CLASS_BATKAIN3 ||
		 player->client->NPC_class == CLASS_BATVORADOR ||
		 player->client->NPC_class == CLASS_BATVAMPIRE_FEAR ||
		 player->client->NPC_class == CLASS_BATVAMPIRE_FEAR2 || 
		 player->client->NPC_class == CLASS_BATVAMPIRE_QUEEN ||
		 player->client->NPC_class == CLASS_BATNECROMANCER ||
		 player->client->NPC_class == CLASS_BATVAMPIRE_FROST ||
		player->client->NPC_class == CLASS_HELLBAT || 
		 player->client->NPC_class == CLASS_BATVAMPIRE_WATER ||
		 player->client->NPC_class == CLASS_BATVAMPIRE_ELF ||
		 player->client->NPC_class == CLASS_BATVAMPIRE_ELF2 ||
		 player->client->NPC_class == CLASS_BATVAMPIRE_ELF3 ||
	// MIST FORM 
		player->client->NPC_class == CLASS_MISTFORM || 
		player->client->NPC_class == CLASS_MISTKAIN ||
		 player->client->NPC_class == CLASS_MISTKAIN2 ||
		 player->client->NPC_class == CLASS_MISTKAIN3 ||
		 player->client->NPC_class == CLASS_MISTVORADOR ||
		 player->client->NPC_class == CLASS_MISTJANOS || 
		 player->client->NPC_class == CLASS_MISTNECROMANCER ||
		 player->client->NPC_class == CLASS_MISTVAMPIRE_QUEEN ||
		 player->client->NPC_class == CLASS_MISTVAMPIRE_FEAR ||
		 player->client->NPC_class == CLASS_MISTVAMPIRE_FEAR2 ||
		 player->client->NPC_class == CLASS_HELLMIST ||
		 player->client->NPC_class == CLASS_MISTVAMPIRE_DARK ||
		 player->client->NPC_class == CLASS_MISTVAMPIRE_BLOOD ||
		 player->client->NPC_class == CLASS_MISTVAMPIRE_WATER ||
		  player->client->NPC_class == CLASS_MISTVAMPIRE_SARAFAN ||
		 player->client->NPC_class == CLASS_MISTVAMPIRE_ELF ||
		 player->client->NPC_class == CLASS_MISTVAMPIRE_ELF2 ||
		 player->client->NPC_class == CLASS_MISTVAMPIRE_ELF3 )
	{
		CG_DrawPic( centerXPos-(bigIconSize/2), (graphicYPos-((bigIconSize-smallIconSize)/2)), bigIconSize, bigIconSize, force_icons31[showDataPadPowers[cg.DataPadforcepowerSelect]] );
	}
	// MINEMONSTER - Shade class?
	else if ( player->client->NPC_class == CLASS_MINEMONSTER || 
	  player->client->NPC_class == CLASS_POISON ||
	  player->client->NPC_class == CLASS_IVY || 
	  player->client->NPC_class == CLASS_DEMON_POISON ||
      player->client->NPC_class == CLASS_GOLEM_POISON || 
	  player->client->NPC_class == CLASS_RANCOR || 
	  player->client->NPC_class == CLASS_INTERROGATOR )
	{
		CG_DrawPic( centerXPos-(bigIconSize/2), (graphicYPos-((bigIconSize-smallIconSize)/2)), bigIconSize, bigIconSize, force_icons32[showDataPadPowers[cg.DataPadforcepowerSelect]] );
	}
	else if ( player->client->NPC_class == CLASS_TAVION ||
		player->client->NPC_class == CLASS_HYLDEN ||
		player->client->NPC_class == CLASS_DESANN ||
		player->client->NPC_class == CLASS_HYLDENLORD )
	{
		CG_DrawPic( centerXPos-(bigIconSize/2), (graphicYPos-((bigIconSize-smallIconSize)/2)), bigIconSize, bigIconSize, force_icons33[showDataPadPowers[cg.DataPadforcepowerSelect]] );
	}
	else if ( player->client->NPC_class == CLASS_NOGHRI || player->client->NPC_class == CLASS_ZEPH ||
		player->client->NPC_class == CLASS_DECEIVER || player->client->NPC_class == CLASS_FLIER2 ) 
	{
		CG_DrawPic( centerXPos-(bigIconSize/2), (graphicYPos-((bigIconSize-smallIconSize)/2)), bigIconSize, bigIconSize, force_icons34[showDataPadPowers[cg.DataPadforcepowerSelect]] );
	}
	else if ( player->client->NPC_class == CLASS_JAN || player->client->NPC_class == CLASS_WARRIOR ||
		player->client->NPC_class == CLASS_STORMTROOPER ||player->client->NPC_class == CLASS_SARAFAN ||
		player->client->NPC_class == CLASS_PROPHET || player->client->NPC_class == CLASS_GOLEM_HOLY ||
		
		// Unused but i place however for control mind and wrack 
		player->client->NPC_class == CLASS_PRISONER || player->client->NPC_class == CLASS_RODIAN ||
		player->client->NPC_class == CLASS_ARCHER ||player->client->NPC_class == CLASS_HUNTER ||
		player->client->NPC_class == CLASS_SCOUT || player->client->NPC_class == CLASS_ALCHEMIST ||
		player->client->NPC_class == CLASS_GALAK || player->client->NPC_class == CLASS_WIZARD ||
		player->client->NPC_class == CLASS_IMPWORKER ||player->client->NPC_class == CLASS_BEAST )
	{
		CG_DrawPic( centerXPos-(bigIconSize/2), (graphicYPos-((bigIconSize-smallIconSize)/2)), bigIconSize, bigIconSize, force_icons35[showDataPadPowers[cg.DataPadforcepowerSelect]] );
	}
	else if ( player->client->NPC_class == CLASS_VAMPIRE_ELF2 ||
		player->client->NPC_class == CLASS_VAMPIRE_ELF3 )
 	{
		CG_DrawPic( centerXPos-(bigIconSize/2), (graphicYPos-((bigIconSize-smallIconSize)/2)), bigIconSize, bigIconSize, force_icons36[showDataPadPowers[cg.DataPadforcepowerSelect]] );
	}
	// UNUSED
	// ARTIFACTS 
	else if ( player->client->NPC_class == CLASS_R2D2 ||
		player->client->NPC_class == CLASS_R5D2 ||
		player->client->NPC_class == CLASS_SEEKER ||
		player->client->NPC_class == CLASS_REMOTE )		
 	{
		CG_DrawPic( centerXPos-(bigIconSize/2), (graphicYPos-((bigIconSize-smallIconSize)/2)), bigIconSize, bigIconSize, force_icons[showDataPadPowers[cg.DataPadforcepowerSelect]] );
	}
	else if ( player->client->NPC_class == CLASS_ELDERGOD ||
		player->client->NPC_class == CLASS_SAND_CREATURE || 
		player->client->NPC_class == CLASS_TENTACLE )
 	{
		CG_DrawPic( centerXPos-(bigIconSize/2), (graphicYPos-((bigIconSize-smallIconSize)/2)), bigIconSize, bigIconSize, force_icons[showDataPadPowers[cg.DataPadforcepowerSelect]] );
	}
	else 
	{
		CG_DrawPic( centerXPos-(bigIconSize/2), (graphicYPos-((bigIconSize-smallIconSize)/2)), bigIconSize, bigIconSize, force_icons[showDataPadPowers[cg.DataPadforcepowerSelect]] );
	}
		if (((cg_updatedDataPadForcePower1.integer - 1) == showDataPadPowers[cg.DataPadforcepowerSelect]) ||
			((cg_updatedDataPadForcePower2.integer - 1) == showDataPadPowers[cg.DataPadforcepowerSelect]) ||
			((cg_updatedDataPadForcePower3.integer - 1) == showDataPadPowers[cg.DataPadforcepowerSelect]))
		{			
			CG_DrawPic( centerXPos-(bigIconSize/2), (graphicYPos-((bigIconSize-smallIconSize)/2)), bigIconSize, bigIconSize, cgs.media.DPForcePowerOverlay ); 
		}
	}


	i = cg.DataPadforcepowerSelect + 1;
	if (i>=MAX_DPSHOWPOWERS)
	{
		i = 0;
	}

	cgi_R_SetColor(colorTable[CT_WHITE]);

	// Work forwards from current icon
	holdX = centerXPos + (bigIconSize/2) + bigPad;
	for (iconCnt=1;iconCnt<(sideRightIconCnt+1);i++)
	{
		if (i>=MAX_DPSHOWPOWERS)
		{
			i = 0;
		}

		if (!ForcePowerDataPad_Valid(i))	// Does he have this power?
		{
			continue;
		}

		++iconCnt;					// Good icon


		if ( player->client->NPC_class == CLASS_DARKREAVER ||
		player->client->NPC_class == CLASS_SABOTEUR ||
		player->client->NPC_class == CLASS_DARK ||
		player->client->NPC_class == CLASS_GOLEM_DARK )
	{
		if (force_icons[showDataPadPowers[i]])
		{
			CG_DrawPic( holdX, graphicYPos, smallIconSize, smallIconSize, force_icons1[showDataPadPowers[i]] );
		}
	}
	else if ( player->client->NPC_class == CLASS_TANKREAVER ||
	player->client->NPC_class == CLASS_BESPIN_COP ||
	player->client->NPC_class == CLASS_GOLEM_SMOKE )
	{
		if (force_icons[showDataPadPowers[i]])
		{
			CG_DrawPic( holdX, graphicYPos, smallIconSize, smallIconSize, force_icons2[showDataPadPowers[i]] );
		}
	}
	else if ( player->client->NPC_class == CLASS_SHADOWTROOPER ||
	player->client->NPC_class == CLASS_VOID || 
	player->client->NPC_class == CLASS_GOLEM_VOID )
	{
		if (force_icons[showDataPadPowers[i]])
		{
			CG_DrawPic( holdX, graphicYPos, smallIconSize, smallIconSize, force_icons3[showDataPadPowers[i]] );
		}
	}
	else if ( player->client->NPC_class == CLASS_LIGHTREAVER ||
		player->client->NPC_class == CLASS_SABER_DROID ||
		player->client->NPC_class == CLASS_ASSASSIN_DROID ||
		player->client->NPC_class == CLASS_LIGHT ||
		player->client->NPC_class == CLASS_GOLEM_LIGHT )
		
	{
		if (force_icons[showDataPadPowers[i]])
		{
			CG_DrawPic( holdX, graphicYPos, smallIconSize, smallIconSize, force_icons4[showDataPadPowers[i]] );
		}
	}
	else if ( player->client->NPC_class == CLASS_MORGANKATARN ||
		player->client->NPC_class == CLASS_GOLEM_GOLD ||
		player->client->NPC_class == CLASS_PROTOCOL ||
		player->client->NPC_class == CLASS_ATST )
	{
		if (force_icons[showDataPadPowers[i]])
		{
			CG_DrawPic( holdX, graphicYPos, smallIconSize, smallIconSize, force_icons5[showDataPadPowers[i]] );
		}
	}
	else if ( player->client->NPC_class == CLASS_FIREREAVER || 
		player->client->NPC_class == CLASS_GOLEM_FIRE || 
		player->client->NPC_class == CLASS_FIRE )
	{
		if (force_icons[showDataPadPowers[i]])
		{
			CG_DrawPic( holdX, graphicYPos, smallIconSize, smallIconSize, force_icons6[showDataPadPowers[i]] );
		}
	}
	else if ( player->client->NPC_class == CLASS_CLAW || player->client->NPC_class == CLASS_CLAW || player->client->NPC_class == CLASS_GOLEM_LAVA )
	{
		if (force_icons[showDataPadPowers[i]])
		{
			CG_DrawPic( holdX, graphicYPos, smallIconSize, smallIconSize, force_icons7[showDataPadPowers[i]] );
		}
	}
	else if ( player->client->NPC_class == CLASS_REBEL || 
		player->client->NPC_class == CLASS_NAERYANMIST ||
				player->client->NPC_class == CLASS_PROBE || 
				player->client->NPC_class == CLASS_AIR ||
				player->client->NPC_class == CLASS_GOLEM_AIR ||
				player->client->NPC_class == CLASS_BIRD 	)
	{
		if (force_icons[showDataPadPowers[i]])
		{
			CG_DrawPic( holdX, graphicYPos, smallIconSize, smallIconSize, force_icons8[showDataPadPowers[i]] );
		}
	}
	else if ( player->client->NPC_class == CLASS_AIRREAVER || player->client->NPC_class == CLASS_POLTER ||
		player->client->NPC_class == CLASS_NAERYANMIST2 	)
	{
		if (force_icons[showDataPadPowers[i]])
		{
			CG_DrawPic( holdX, graphicYPos, smallIconSize, smallIconSize, force_icons9[showDataPadPowers[i]] );
		}
	}
	else if ( player->client->NPC_class == CLASS_ROCKETTROOPER ||
		player->client->NPC_class == CLASS_NAERYANMIST3 ||
			 player->client->NPC_class == CLASS_GOLEM_METAL ||
			  player->client->NPC_class == CLASS_DEMON_ELECTRO ||
			   player->client->NPC_class == CLASS_BOBAFETT )
	{
		if (force_icons[showDataPadPowers[i]])
		{
			CG_DrawPic( holdX, graphicYPos, smallIconSize, smallIconSize, force_icons10[showDataPadPowers[i]] );
		}
	}
	else if (  player->client->NPC_class == CLASS_VAMPIRE_SOUND ||
		player->client->NPC_class == CLASS_SENTRY || 
		player->client->NPC_class == CLASS_HOWLER ||
		player->client->NPC_class == CLASS_SONIC ||
		player->client->NPC_class == CLASS_GOLEM_SONIC )
	{
		if (force_icons[showDataPadPowers[i]])
		{
			CG_DrawPic( holdX, graphicYPos, smallIconSize, smallIconSize, force_icons11[showDataPadPowers[i]] );
		}
	}
	else if ( player->client->NPC_class == CLASS_BARTENDER || 
		player->client->NPC_class == CLASS_ICEREAVER ||
		player->client->NPC_class == CLASS_GOLEM_ICE )
	{
		if (force_icons[showDataPadPowers[i]])
		{
			CG_DrawPic( holdX, graphicYPos, smallIconSize, smallIconSize, force_icons12[showDataPadPowers[i]] );
		}
	}
	else if ( player->client->NPC_class == CLASS_CRYSTAL ||
		player->client->NPC_class == CLASS_GALAKMECH )
	{
		if (force_icons[showDataPadPowers[i]])
		{
			CG_DrawPic( holdX, graphicYPos, smallIconSize, smallIconSize, force_icons13[showDataPadPowers[i]] );
		}
	}
	else if ( player->client->NPC_class == CLASS_SPIDERREAVER )
	{
		if (force_icons[showDataPadPowers[i]])
		{
			CG_DrawPic( holdX, graphicYPos, smallIconSize, smallIconSize, force_icons14[showDataPadPowers[i]] );
		}
	}
	else if ( player->client->NPC_class == CLASS_SPIDERREAVER2 || player->client->NPC_class == CLASS_UGNAUGHT )
	{
		if (force_icons[showDataPadPowers[i]])
		{
			CG_DrawPic( holdX, graphicYPos, smallIconSize, smallIconSize, force_icons15[showDataPadPowers[i]] );
		}
	}
	else if ( player->client->NPC_class == CLASS_EARTHREAVER || 
				player->client->NPC_class == CLASS_LIZARD || 
				player->client->NPC_class == CLASS_EARTH || 
				player->client->NPC_class == CLASS_GONK ||
				player->client->NPC_class == CLASS_DEMON_BLADE || 
				player->client->NPC_class == CLASS_GOLEM )
	{
		if (force_icons[showDataPadPowers[i]])
		{
			CG_DrawPic( holdX, graphicYPos, smallIconSize, smallIconSize, force_icons16[showDataPadPowers[i]] );
		}
	}
	else if ( player->client->NPC_class == CLASS_SPIDERREAVER3 || player->client->NPC_class == CLASS_REELO )
	{
		if (force_icons[showDataPadPowers[i]])
		{
			CG_DrawPic( holdX, graphicYPos, smallIconSize, smallIconSize, force_icons17[showDataPadPowers[i]] );
		}
	}
	else if ( player->client->NPC_class == CLASS_NECROREAVER )
	{
		if (force_icons[showDataPadPowers[i]])
		{
			CG_DrawPic( holdX, graphicYPos, smallIconSize, smallIconSize, force_icons18[showDataPadPowers[i]] );
		}
	}
	else if ( player->client->NPC_class == CLASS_MURJJ ||
		player->client->NPC_class == CLASS_FISH || 
		player->client->NPC_class == CLASS_NECRO )
	{
		if (force_icons[showDataPadPowers[i]])
		{
			CG_DrawPic( holdX, graphicYPos, smallIconSize, smallIconSize, force_icons19[showDataPadPowers[i]] );
		}
	}
	else if ( player->client->NPC_class == CLASS_RAZIEL ||
		player->client->NPC_class == CLASS_MENTALIST )
	{
		if (force_icons[showDataPadPowers[i]])
		{
			CG_DrawPic( holdX, graphicYPos, smallIconSize, smallIconSize, force_icons20[showDataPadPowers[i]] );
		}
	}
	else if ( player->client->NPC_class == CLASS_HOLY ) 
	{
		if (force_icons[showDataPadPowers[i]])
		{
			CG_DrawPic( holdX, graphicYPos, smallIconSize, smallIconSize, force_icons21[showDataPadPowers[i]] );
		}
	}
	else if ( player->client->NPC_class == CLASS_LUKE ) 
	{
		if (force_icons[showDataPadPowers[i]])
		{
			CG_DrawPic( holdX, graphicYPos, smallIconSize, smallIconSize, force_icons22[showDataPadPowers[i]] );
		}
	}
	else if ( player->client->NPC_class == CLASS_DRUIDREAVER ||
		player->client->NPC_class == CLASS_TRANDOSHAN ||
		player->client->NPC_class == CLASS_LYCAN ||
		player->client->NPC_class == CLASS_DRUID ) 
	{
		if (force_icons[showDataPadPowers[i]])
		{
			CG_DrawPic( holdX, graphicYPos, smallIconSize, smallIconSize, force_icons23[showDataPadPowers[i]] );
		}
	}
	else if ( player->client->NPC_class == CLASS_LANDO ) 
	{
		if (force_icons[showDataPadPowers[i]])
		{
			CG_DrawPic( holdX, graphicYPos, smallIconSize, smallIconSize, force_icons24[showDataPadPowers[i]] );
		}
	}
	else if ( player->client->NPC_class == CLASS_VAMPIRE ||
		player->client->NPC_class == CLASS_REBORN ||	
		player->client->NPC_class == CLASS_KAIN ||
		player->client->NPC_class == CLASS_KAIN2 ||
		player->client->NPC_class == CLASS_UMAH ||
		player->client->NPC_class == CLASS_VAMPIRE_DARK ||
		player->client->NPC_class == CLASS_VAMPIRE_NECRO ||
		player->client->NPC_class == CLASS_VAMPIRE_SARAFAN ||
		player->client->NPC_class == CLASS_HALFVAMPIRE ||
		player->client->NPC_class == CLASS_RAZIELHIM ||
		player->client->NPC_class == CLASS_MONMOTHA ||		
		player->client->NPC_class == CLASS_VAMPIRE_ELF ||
		player->client->NPC_class == CLASS_ALORA ||
		player->client->NPC_class == CLASS_VAMPIRE_WATER ||
		player->client->NPC_class == CLASS_VAMPIRE_LYCAN ||
		player->client->NPC_class == CLASS_VAMPIRE_ARCHER ||
		player->client->NPC_class == CLASS_VAMPIRE_BOUNTY ||
		player->client->NPC_class == CLASS_VAMPIRE_BLOOD ||
		player->client->NPC_class == CLASS_DUMAHIM ||
		player->client->NPC_class == CLASS_WEEQUAY ||
		player->client->NPC_class == CLASS_GRAN ||
		player->client->NPC_class == CLASS_TURELIM ) 
	{
		if (force_icons[showDataPadPowers[i]])
		{
			CG_DrawPic( holdX, graphicYPos, smallIconSize, smallIconSize, force_icons25[showDataPadPowers[i]] );
		}
	}
	else if ( player->client->NPC_class == CLASS_VAMPIRE_FEAR || player->client->NPC_class == CLASS_VAMPIRE_FEAR2 )
	{
		if (force_icons[showDataPadPowers[i]])
		{
			CG_DrawPic( holdX, graphicYPos, smallIconSize, smallIconSize, force_icons26[showDataPadPowers[i]] );
		}
	}
	else if ( player->client->NPC_class == CLASS_KAIN3 ||
		player->client->NPC_class == CLASS_KYLE ||
		player->client->NPC_class == CLASS_VAMPIRE_FROST ||
		player->client->NPC_class == CLASS_NECROMANCER ||
		player->client->NPC_class == CLASS_VAMPIRE_QUEEN ||
		player->client->NPC_class == CLASS_VORADOR ||
		player->client->NPC_class == CLASS_JANOS ||
		player->client->NPC_class == CLASS_VAMPIRE_NECRO2 )
	{
		if (force_icons[showDataPadPowers[i]])
		{
			CG_DrawPic( holdX, graphicYPos, smallIconSize, smallIconSize, force_icons27[showDataPadPowers[i]] );
		}
	}
	else if (  player->client->NPC_class == CLASS_SWAMP || player->client->NPC_class == CLASS_RAHABIM ||
						player->client->NPC_class == CLASS_SHARK || player->client->NPC_class == CLASS_SWAMPTROOPER || 
						player->client->NPC_class == CLASS_WATER || player->client->NPC_class == CLASS_GOLEM_WATER )
	{
		if (force_icons[showDataPadPowers[i]])
		{
			CG_DrawPic( holdX, graphicYPos, smallIconSize, smallIconSize, force_icons28[showDataPadPowers[i]] );
		}
	}
	else if ( player->client->NPC_class == CLASS_VAMPIRE_ZOMBIE || player->client->NPC_class == CLASS_TUSKEN || 
		player->client->NPC_class == CLASS_MELC || player->client->NPC_class == CLASS_SUMMONER || 
		player->client->NPC_class == CLASS_THRALL || 	player->client->NPC_class == CLASS_GLIDER || 
		player->client->NPC_class == CLASS_COMMANDO || player->client->NPC_class == CLASS_GOLEM_NECRO )
	{
		if (force_icons[showDataPadPowers[i]])
		{
			CG_DrawPic( holdX, graphicYPos, smallIconSize, smallIconSize, force_icons29[showDataPadPowers[i]] );
		}
	}
	else if ( player->client->NPC_class == CLASS_VAMPIRE_DEMON ||
		player->client->NPC_class == CLASS_WAMPA || 
		player->client->NPC_class == CLASS_DEMON_FIRE || 
		player->client->NPC_class == CLASS_CENOBITA || 
		player->client->NPC_class == CLASS_HELLGOLEM || 
		player->client->NPC_class == CLASS_HAZARD_TROOPER || 
		player->client->NPC_class == CLASS_IMPERIAL)
	{
		if (force_icons[showDataPadPowers[i]])
		{
			CG_DrawPic( holdX, graphicYPos, smallIconSize, smallIconSize, force_icons30[showDataPadPowers[i]] );
		}
	}
	else if ( player->client->NPC_class == CLASS_WOLFFORM ||
		 player->client->NPC_class == CLASS_WOLFKAIN ||
		 player->client->NPC_class == CLASS_WOLFKAIN2 ||
		 player->client->NPC_class == CLASS_WOLFKAIN3 ||
		 player->client->NPC_class == CLASS_WOLFVORADOR ||
		 player->client->NPC_class == CLASS_WOLFJANOS || 
		 player->client->NPC_class == CLASS_WOLFNECROMANCER ||
		 player->client->NPC_class == CLASS_WOLFVAMPIRE_QUEEN ||
		 player->client->NPC_class == CLASS_WOLFVAMPIRE_FEAR ||
		 player->client->NPC_class == CLASS_WOLFVAMPIRE_FEAR2 ||
		 player->client->NPC_class == CLASS_HELLWOLF ||
		 player->client->NPC_class == CLASS_WOLFVAMPIRE_LYCAN ||
		 player->client->NPC_class == CLASS_WOLFVAMPIRE_ELF ||
		 player->client->NPC_class == CLASS_LIONVAMPIRE_ELF ||
		 player->client->NPC_class == CLASS_LIONVAMPIRE_ELF2 || 
		 player->client->NPC_class == CLASS_WOLFHALFVAMPIRE ||
		 player->client->NPC_class == CLASS_ESKANDOR || 
		 // BAT FORM  
		 player->client->NPC_class == CLASS_BATFORM ||
		 player->client->NPC_class == CLASS_BATKAIN ||
		 player->client->NPC_class == CLASS_BATKAIN2 ||
		 player->client->NPC_class == CLASS_BATKAIN3 ||
		 player->client->NPC_class == CLASS_BATVORADOR ||
		 player->client->NPC_class == CLASS_BATVAMPIRE_FEAR ||
		 player->client->NPC_class == CLASS_BATVAMPIRE_FEAR2 || 
		 player->client->NPC_class == CLASS_BATVAMPIRE_QUEEN ||
		 player->client->NPC_class == CLASS_BATNECROMANCER ||
		 player->client->NPC_class == CLASS_BATVAMPIRE_FROST ||
		player->client->NPC_class == CLASS_HELLBAT || 
		 player->client->NPC_class == CLASS_BATVAMPIRE_WATER ||
		 player->client->NPC_class == CLASS_BATVAMPIRE_ELF ||
		 player->client->NPC_class == CLASS_BATVAMPIRE_ELF2 ||
		 player->client->NPC_class == CLASS_BATVAMPIRE_ELF3 ||
	// MIST FORM 
		player->client->NPC_class == CLASS_MISTFORM || 
		player->client->NPC_class == CLASS_MISTKAIN ||
		 player->client->NPC_class == CLASS_MISTKAIN2 ||
		 player->client->NPC_class == CLASS_MISTKAIN3 ||
		 player->client->NPC_class == CLASS_MISTVORADOR ||
		 player->client->NPC_class == CLASS_MISTJANOS || 
		 player->client->NPC_class == CLASS_MISTNECROMANCER ||
		 player->client->NPC_class == CLASS_MISTVAMPIRE_QUEEN ||
		 player->client->NPC_class == CLASS_MISTVAMPIRE_FEAR ||
		 player->client->NPC_class == CLASS_MISTVAMPIRE_FEAR2 ||
		 player->client->NPC_class == CLASS_HELLMIST ||
		 player->client->NPC_class == CLASS_MISTVAMPIRE_DARK ||
		 player->client->NPC_class == CLASS_MISTVAMPIRE_BLOOD ||
		 player->client->NPC_class == CLASS_MISTVAMPIRE_WATER ||
		  player->client->NPC_class == CLASS_MISTVAMPIRE_SARAFAN ||
		 player->client->NPC_class == CLASS_MISTVAMPIRE_ELF ||
		 player->client->NPC_class == CLASS_MISTVAMPIRE_ELF2 ||
		 player->client->NPC_class == CLASS_MISTVAMPIRE_ELF3 )
	{
		if (force_icons[showDataPadPowers[i]])
		{
			CG_DrawPic( holdX, graphicYPos, smallIconSize, smallIconSize, force_icons31[showDataPadPowers[i]] );
		}
	}
	// MINEMONSTER - Shade class?
	else if ( player->client->NPC_class == CLASS_MINEMONSTER || 
	  player->client->NPC_class == CLASS_POISON ||
	  player->client->NPC_class == CLASS_IVY || 
	  player->client->NPC_class == CLASS_DEMON_POISON ||
      player->client->NPC_class == CLASS_GOLEM_POISON || 
	  player->client->NPC_class == CLASS_RANCOR || 
	  player->client->NPC_class == CLASS_INTERROGATOR )
	 {
		if (force_icons[showDataPadPowers[i]])
		{
			CG_DrawPic( holdX, graphicYPos, smallIconSize, smallIconSize, force_icons32[showDataPadPowers[i]] );
		}
	}
	else if ( player->client->NPC_class == CLASS_TAVION ||
		player->client->NPC_class == CLASS_HYLDEN ||
		player->client->NPC_class == CLASS_DESANN ||
		player->client->NPC_class == CLASS_HYLDENLORD )
	{
		if (force_icons[showDataPadPowers[i]])
		{
			CG_DrawPic( holdX, graphicYPos, smallIconSize, smallIconSize, force_icons33[showDataPadPowers[i]] );
		}
	}
	else if ( player->client->NPC_class == CLASS_NOGHRI || player->client->NPC_class == CLASS_ZEPH ||
		player->client->NPC_class == CLASS_DECEIVER || player->client->NPC_class == CLASS_FLIER2 ) 
	
	{
		if (force_icons[showDataPadPowers[i]])
		{
			CG_DrawPic( holdX, graphicYPos, smallIconSize, smallIconSize, force_icons34[showDataPadPowers[i]] );
		}
	}
	else if ( player->client->NPC_class == CLASS_JAN || player->client->NPC_class == CLASS_WARRIOR ||
		player->client->NPC_class == CLASS_STORMTROOPER ||player->client->NPC_class == CLASS_SARAFAN ||
		player->client->NPC_class == CLASS_PROPHET || player->client->NPC_class == CLASS_GOLEM_HOLY ||
		
		// Unused but i place however for control mind and wrack 
		player->client->NPC_class == CLASS_PRISONER || player->client->NPC_class == CLASS_RODIAN ||
		player->client->NPC_class == CLASS_ARCHER ||player->client->NPC_class == CLASS_HUNTER ||
		player->client->NPC_class == CLASS_SCOUT || player->client->NPC_class == CLASS_ALCHEMIST ||
		player->client->NPC_class == CLASS_GALAK || player->client->NPC_class == CLASS_WIZARD ||
		player->client->NPC_class == CLASS_IMPWORKER ||player->client->NPC_class == CLASS_BEAST )
	{
		if (force_icons[showDataPadPowers[i]])
		{
			CG_DrawPic( holdX, graphicYPos, smallIconSize, smallIconSize, force_icons35[showDataPadPowers[i]] );
		}
	}
	else if ( player->client->NPC_class == CLASS_VAMPIRE_ELF2 ||
		player->client->NPC_class == CLASS_VAMPIRE_ELF3 )
 	{
		if (force_icons[showDataPadPowers[i]])
		{
			CG_DrawPic( holdX, graphicYPos, smallIconSize, smallIconSize, force_icons36[showDataPadPowers[i]] );
		}
	}
	// UNUSED
	// ARTIFACTS 
	else if ( player->client->NPC_class == CLASS_R2D2 ||
		player->client->NPC_class == CLASS_R5D2 ||
		player->client->NPC_class == CLASS_SEEKER ||
		player->client->NPC_class == CLASS_REMOTE )		
 	{
		if (force_icons[showDataPadPowers[i]])
		{
			CG_DrawPic( holdX, graphicYPos, smallIconSize, smallIconSize, force_icons[showDataPadPowers[i]] );
		}
	}
	else if ( player->client->NPC_class == CLASS_ELDERGOD ||
		player->client->NPC_class == CLASS_SAND_CREATURE || 
		player->client->NPC_class == CLASS_TENTACLE )
 	{
		if (force_icons[showDataPadPowers[i]])
		{
			CG_DrawPic( holdX, graphicYPos, smallIconSize, smallIconSize, force_icons[showDataPadPowers[i]] );
		}
	}
	else 
	{
		if (force_icons[showDataPadPowers[i]])
		{
			CG_DrawPic( holdX, graphicYPos, smallIconSize, smallIconSize, force_icons[showDataPadPowers[i]] );
		}
	}
		
		/*if (force_icons[showDataPadPowers[i]])
		{
			CG_DrawPic( holdX, graphicYPos, smallIconSize, smallIconSize, force_icons[showDataPadPowers[i]] );
		}*/

		// A new force power
		if (((cg_updatedDataPadForcePower1.integer - 1) == showDataPadPowers[i]) ||
			((cg_updatedDataPadForcePower2.integer - 1) == showDataPadPowers[i]) ||
			((cg_updatedDataPadForcePower3.integer - 1) == showDataPadPowers[i]))
		{
			CG_DrawPic( holdX, graphicYPos, smallIconSize, smallIconSize, cgs.media.DPForcePowerOverlay );
		}

		if (force_icons[showDataPadPowers[i]])
		{
			holdX += (smallIconSize+pad);
		}
	}

	// LE DESCRIZIONI DEI POTERI ARCANI 
	if (!cgi_SP_GetStringTextString( va("SP_INGAME_%s",forcepowerDesc[cg.DataPadforcepowerSelect]), text, sizeof(text) ))
	{
		cgi_SP_GetStringTextString( va("SPMOD_INGAME_%s",forcepowerDesc[cg.DataPadforcepowerSelect]), text, sizeof(text) );
	}
		if ( player->client->NPC_class == CLASS_DARKREAVER ||
		player->client->NPC_class == CLASS_SABOTEUR ||
		player->client->NPC_class == CLASS_DARK ||
		player->client->NPC_class == CLASS_GOLEM_DARK )
	{
		cgi_SP_GetStringTextString( va("SP_INGAME_%s",forcepowerDesc1[cg.DataPadforcepowerSelect]), text, sizeof(text) );
	}
	else if ( player->client->NPC_class == CLASS_TANKREAVER ||
	player->client->NPC_class == CLASS_BESPIN_COP ||
	player->client->NPC_class == CLASS_GOLEM_SMOKE )
	{
		cgi_SP_GetStringTextString( va("SP_INGAME_%s",forcepowerDesc2[cg.DataPadforcepowerSelect]), text, sizeof(text) );
	}
	else if ( player->client->NPC_class == CLASS_SHADOWTROOPER ||
	player->client->NPC_class == CLASS_VOID || 
	player->client->NPC_class == CLASS_GOLEM_VOID )
	{
		cgi_SP_GetStringTextString( va("SP_INGAME_%s",forcepowerDesc3[cg.DataPadforcepowerSelect]), text, sizeof(text) );
	}
	else if ( player->client->NPC_class == CLASS_LIGHTREAVER ||
		player->client->NPC_class == CLASS_SABER_DROID ||
		player->client->NPC_class == CLASS_ASSASSIN_DROID ||
		player->client->NPC_class == CLASS_LIGHT ||
		player->client->NPC_class == CLASS_GOLEM_LIGHT )
	{
		cgi_SP_GetStringTextString( va("SP_INGAME_%s",forcepowerDesc4[cg.DataPadforcepowerSelect]), text, sizeof(text) );
	}
	else if ( player->client->NPC_class == CLASS_MORGANKATARN ||
		player->client->NPC_class == CLASS_GOLEM_GOLD ||
		player->client->NPC_class == CLASS_PROTOCOL ||
		player->client->NPC_class == CLASS_ATST )
	{
		cgi_SP_GetStringTextString( va("SP_INGAME_%s",forcepowerDesc5[cg.DataPadforcepowerSelect]), text, sizeof(text) );
	}
	else if ( player->client->NPC_class == CLASS_FIREREAVER || 
		player->client->NPC_class == CLASS_GOLEM_FIRE || 
		player->client->NPC_class == CLASS_FIRE )
	{
		cgi_SP_GetStringTextString( va("SP_INGAME_%s",forcepowerDesc6[cg.DataPadforcepowerSelect]), text, sizeof(text) );
	}
	else if ( player->client->NPC_class == CLASS_CLAW || player->client->NPC_class == CLASS_CLAW || player->client->NPC_class == CLASS_GOLEM_LAVA )
	{
		cgi_SP_GetStringTextString( va("SP_INGAME_%s",forcepowerDesc7[cg.DataPadforcepowerSelect]), text, sizeof(text) );
	}
	else if ( player->client->NPC_class == CLASS_REBEL || 
		player->client->NPC_class == CLASS_NAERYANMIST ||
				player->client->NPC_class == CLASS_PROBE || 
				player->client->NPC_class == CLASS_AIR ||
				player->client->NPC_class == CLASS_GOLEM_AIR ||
				player->client->NPC_class == CLASS_BIRD 	)
	{
		cgi_SP_GetStringTextString( va("SP_INGAME_%s",forcepowerDesc8[cg.DataPadforcepowerSelect]), text, sizeof(text) );
	}
	else if ( player->client->NPC_class == CLASS_AIRREAVER || player->client->NPC_class == CLASS_POLTER ||
		player->client->NPC_class == CLASS_NAERYANMIST2 	)
	{
		cgi_SP_GetStringTextString( va("SP_INGAME_%s",forcepowerDesc9[cg.DataPadforcepowerSelect]), text, sizeof(text) );
	}
	else if ( player->client->NPC_class == CLASS_ROCKETTROOPER ||
			 player->client->NPC_class == CLASS_GOLEM_METAL ||
			  player->client->NPC_class == CLASS_DEMON_ELECTRO ||
			   player->client->NPC_class == CLASS_BOBAFETT ||
			   player->client->NPC_class == CLASS_NAERYANMIST3 )
	{
		cgi_SP_GetStringTextString( va("SP_INGAME_%s",forcepowerDesc10[cg.DataPadforcepowerSelect]), text, sizeof(text) );
	}
	else if ( player->client->NPC_class == CLASS_HOWLER ||
		player->client->NPC_class == CLASS_SONIC ||
		player->client->NPC_class == CLASS_GOLEM_SONIC  ||
		player->client->NPC_class == CLASS_VAMPIRE_SOUND ||
		player->client->NPC_class == CLASS_SENTRY )
	{
		cgi_SP_GetStringTextString( va("SP_INGAME_%s",forcepowerDesc11[cg.DataPadforcepowerSelect]), text, sizeof(text) );
	}
	else if ( player->client->NPC_class == CLASS_BARTENDER || 
		player->client->NPC_class == CLASS_ICEREAVER ||
		player->client->NPC_class == CLASS_GOLEM_ICE )
	{
		cgi_SP_GetStringTextString( va("SP_INGAME_%s",forcepowerDesc12[cg.DataPadforcepowerSelect]), text, sizeof(text) );
	}
	else if ( player->client->NPC_class == CLASS_CRYSTAL ||
		player->client->NPC_class == CLASS_GALAKMECH )
	{
		cgi_SP_GetStringTextString( va("SP_INGAME_%s",forcepowerDesc13[cg.DataPadforcepowerSelect]), text, sizeof(text) );
	}
	else if ( player->client->NPC_class == CLASS_SPIDERREAVER )
	{
		cgi_SP_GetStringTextString( va("SP_INGAME_%s",forcepowerDesc14[cg.DataPadforcepowerSelect]), text, sizeof(text) );
	}
	else if ( player->client->NPC_class == CLASS_SPIDERREAVER2 || player->client->NPC_class == CLASS_UGNAUGHT )
	{
		cgi_SP_GetStringTextString( va("SP_INGAME_%s",forcepowerDesc15[cg.DataPadforcepowerSelect]), text, sizeof(text) );
	}
	else if ( player->client->NPC_class == CLASS_EARTHREAVER || 
				player->client->NPC_class == CLASS_LIZARD || 
				player->client->NPC_class == CLASS_EARTH || 
				player->client->NPC_class == CLASS_GONK ||
				player->client->NPC_class == CLASS_DEMON_BLADE || 
				player->client->NPC_class == CLASS_GOLEM )
	{
		cgi_SP_GetStringTextString( va("SP_INGAME_%s",forcepowerDesc16[cg.DataPadforcepowerSelect]), text, sizeof(text) );
	}
	else if ( player->client->NPC_class == CLASS_SPIDERREAVER3 || player->client->NPC_class == CLASS_REELO )
	{
		cgi_SP_GetStringTextString( va("SP_INGAME_%s",forcepowerDesc17[cg.DataPadforcepowerSelect]), text, sizeof(text) );
	}
	else if ( player->client->NPC_class == CLASS_NECROREAVER )
	{
		cgi_SP_GetStringTextString( va("SP_INGAME_%s",forcepowerDesc18[cg.DataPadforcepowerSelect]), text, sizeof(text) );
	}
	else if ( player->client->NPC_class == CLASS_MURJJ ||
		player->client->NPC_class == CLASS_FISH || 
		player->client->NPC_class == CLASS_NECRO )
	{
		cgi_SP_GetStringTextString( va("SP_INGAME_%s",forcepowerDesc19[cg.DataPadforcepowerSelect]), text, sizeof(text) );
	}
	else if ( player->client->NPC_class == CLASS_RAZIEL ||
		player->client->NPC_class == CLASS_MENTALIST )
	{
		cgi_SP_GetStringTextString( va("SP_INGAME_%s",forcepowerDesc20[cg.DataPadforcepowerSelect]), text, sizeof(text) );
	}
	else if ( player->client->NPC_class == CLASS_HOLY ||
		player->client->NPC_class == CLASS_GOLEM_HOLY ) 
	{
		cgi_SP_GetStringTextString( va("SP_INGAME_%s",forcepowerDesc21[cg.DataPadforcepowerSelect]), text, sizeof(text) );
	}
	else if ( player->client->NPC_class == CLASS_LUKE ) 
	{
		cgi_SP_GetStringTextString( va("SP_INGAME_%s",forcepowerDesc22[cg.DataPadforcepowerSelect]), text, sizeof(text) );
	}
	else if ( player->client->NPC_class == CLASS_DRUIDREAVER ||
		player->client->NPC_class == CLASS_TRANDOSHAN ||
		player->client->NPC_class == CLASS_LYCAN ||
		player->client->NPC_class == CLASS_DRUID ) 
	{
		cgi_SP_GetStringTextString( va("SP_INGAME_%s",forcepowerDesc23[cg.DataPadforcepowerSelect]), text, sizeof(text) );
	}
	else if ( player->client->NPC_class == CLASS_LANDO ) 
	{
		cgi_SP_GetStringTextString( va("SP_INGAME_%s",forcepowerDesc24[cg.DataPadforcepowerSelect]), text, sizeof(text) );
	}
	else if ( player->client->NPC_class == CLASS_VAMPIRE ||
		player->client->NPC_class == CLASS_REBORN ||	
		player->client->NPC_class == CLASS_KAIN ||
		player->client->NPC_class == CLASS_KAIN2 ||
		player->client->NPC_class == CLASS_UMAH ||
		player->client->NPC_class == CLASS_VAMPIRE_DARK ||
		player->client->NPC_class == CLASS_VAMPIRE_NECRO ||
		player->client->NPC_class == CLASS_VAMPIRE_SARAFAN ||
		player->client->NPC_class == CLASS_HALFVAMPIRE ||
		player->client->NPC_class == CLASS_RAZIELHIM ||
		player->client->NPC_class == CLASS_MONMOTHA ||		
		player->client->NPC_class == CLASS_VAMPIRE_ELF ||
		player->client->NPC_class == CLASS_ALORA ||
		player->client->NPC_class == CLASS_VAMPIRE_WATER ||
		player->client->NPC_class == CLASS_VAMPIRE_LYCAN ||
		player->client->NPC_class == CLASS_VAMPIRE_ARCHER ||
		player->client->NPC_class == CLASS_VAMPIRE_BOUNTY ||
		player->client->NPC_class == CLASS_VAMPIRE_BLOOD ||
		player->client->NPC_class == CLASS_DUMAHIM ||
		player->client->NPC_class == CLASS_WEEQUAY ||
		player->client->NPC_class == CLASS_GRAN ||
		player->client->NPC_class == CLASS_TURELIM ) 
	{
		cgi_SP_GetStringTextString( va("SP_INGAME_%s",forcepowerDesc25[cg.DataPadforcepowerSelect]), text, sizeof(text) );
	}
	else if ( player->client->NPC_class == CLASS_VAMPIRE_FEAR || player->client->NPC_class == CLASS_VAMPIRE_FEAR2 )
	{
		cgi_SP_GetStringTextString( va("SP_INGAME_%s",forcepowerDesc26[cg.DataPadforcepowerSelect]), text, sizeof(text) );
	}
	else if ( player->client->NPC_class == CLASS_KAIN3 ||
		player->client->NPC_class == CLASS_KYLE ||
		player->client->NPC_class == CLASS_VAMPIRE_FROST ||
		player->client->NPC_class == CLASS_NECROMANCER ||
		player->client->NPC_class == CLASS_VAMPIRE_QUEEN ||
		player->client->NPC_class == CLASS_VORADOR ||
		player->client->NPC_class == CLASS_JANOS ||
		player->client->NPC_class == CLASS_VAMPIRE_NECRO2 )
	{
		cgi_SP_GetStringTextString( va("SP_INGAME_%s",forcepowerDesc27[cg.DataPadforcepowerSelect]), text, sizeof(text) );
	}
	else if (  player->client->NPC_class == CLASS_SWAMP || player->client->NPC_class == CLASS_RAHABIM ||
						player->client->NPC_class == CLASS_SHARK || player->client->NPC_class == CLASS_SWAMPTROOPER || 
						player->client->NPC_class == CLASS_WATER || player->client->NPC_class == CLASS_GOLEM_WATER )
	{
		cgi_SP_GetStringTextString( va("SP_INGAME_%s",forcepowerDesc28[cg.DataPadforcepowerSelect]), text, sizeof(text) );
	}
	else if ( player->client->NPC_class == CLASS_VAMPIRE_ZOMBIE || player->client->NPC_class == CLASS_TUSKEN || 
		player->client->NPC_class == CLASS_MELC || player->client->NPC_class == CLASS_SUMMONER || 
		player->client->NPC_class == CLASS_THRALL || 	player->client->NPC_class == CLASS_GLIDER || 
		player->client->NPC_class == CLASS_COMMANDO || player->client->NPC_class == CLASS_GOLEM_NECRO )
	{
		cgi_SP_GetStringTextString( va("SP_INGAME_%s",forcepowerDesc29[cg.DataPadforcepowerSelect]), text, sizeof(text) );
	}
	else if ( player->client->NPC_class == CLASS_VAMPIRE_DEMON ||
		player->client->NPC_class == CLASS_WAMPA || 
		player->client->NPC_class == CLASS_DEMON_FIRE || 
		player->client->NPC_class == CLASS_CENOBITA || 
		player->client->NPC_class == CLASS_HELLGOLEM || 
		player->client->NPC_class == CLASS_HAZARD_TROOPER || 
		player->client->NPC_class == CLASS_IMPERIAL)
	{
		cgi_SP_GetStringTextString( va("SP_INGAME_%s",forcepowerDesc30[cg.DataPadforcepowerSelect]), text, sizeof(text) );
	}
	else if ( player->client->NPC_class == CLASS_WOLFFORM ||
		 player->client->NPC_class == CLASS_WOLFKAIN ||
		 player->client->NPC_class == CLASS_WOLFKAIN2 ||
		 player->client->NPC_class == CLASS_WOLFKAIN3 ||
		 player->client->NPC_class == CLASS_WOLFVORADOR ||
		 player->client->NPC_class == CLASS_WOLFJANOS || 
		 player->client->NPC_class == CLASS_WOLFNECROMANCER ||
		 player->client->NPC_class == CLASS_WOLFVAMPIRE_QUEEN ||
		 player->client->NPC_class == CLASS_WOLFVAMPIRE_FEAR ||
		 player->client->NPC_class == CLASS_WOLFVAMPIRE_FEAR2 ||
		 player->client->NPC_class == CLASS_HELLWOLF ||
		 player->client->NPC_class == CLASS_WOLFVAMPIRE_LYCAN ||
		 player->client->NPC_class == CLASS_WOLFVAMPIRE_ELF ||
		 player->client->NPC_class == CLASS_LIONVAMPIRE_ELF ||
		 player->client->NPC_class == CLASS_LIONVAMPIRE_ELF2 || 
		 player->client->NPC_class == CLASS_WOLFHALFVAMPIRE ||
		 player->client->NPC_class == CLASS_ESKANDOR || 
		 // BAT FORM  
		 player->client->NPC_class == CLASS_BATFORM ||
		 player->client->NPC_class == CLASS_BATKAIN ||
		 player->client->NPC_class == CLASS_BATKAIN2 ||
		 player->client->NPC_class == CLASS_BATKAIN3 ||
		 player->client->NPC_class == CLASS_BATVORADOR ||
		 player->client->NPC_class == CLASS_BATVAMPIRE_FEAR ||
		 player->client->NPC_class == CLASS_BATVAMPIRE_FEAR2 || 
		 player->client->NPC_class == CLASS_BATVAMPIRE_QUEEN ||
		 player->client->NPC_class == CLASS_BATNECROMANCER ||
		 player->client->NPC_class == CLASS_BATVAMPIRE_FROST ||
		player->client->NPC_class == CLASS_HELLBAT || 
		 player->client->NPC_class == CLASS_BATVAMPIRE_WATER ||
		 player->client->NPC_class == CLASS_BATVAMPIRE_ELF ||
		 player->client->NPC_class == CLASS_BATVAMPIRE_ELF2 ||
		 player->client->NPC_class == CLASS_BATVAMPIRE_ELF3 ||
	// MIST FORM 
		player->client->NPC_class == CLASS_MISTFORM || 
		player->client->NPC_class == CLASS_MISTKAIN ||
		 player->client->NPC_class == CLASS_MISTKAIN2 ||
		 player->client->NPC_class == CLASS_MISTKAIN3 ||
		 player->client->NPC_class == CLASS_MISTVORADOR ||
		 player->client->NPC_class == CLASS_MISTJANOS || 
		 player->client->NPC_class == CLASS_MISTNECROMANCER ||
		 player->client->NPC_class == CLASS_MISTVAMPIRE_QUEEN ||
		 player->client->NPC_class == CLASS_MISTVAMPIRE_FEAR ||
		 player->client->NPC_class == CLASS_MISTVAMPIRE_FEAR2 ||
		 player->client->NPC_class == CLASS_HELLMIST ||
		 player->client->NPC_class == CLASS_MISTVAMPIRE_DARK ||
		 player->client->NPC_class == CLASS_MISTVAMPIRE_BLOOD ||
		 player->client->NPC_class == CLASS_MISTVAMPIRE_WATER ||
		 player->client->NPC_class == CLASS_MISTVAMPIRE_SARAFAN ||
		 player->client->NPC_class == CLASS_MISTVAMPIRE_ELF ||
		 player->client->NPC_class == CLASS_MISTVAMPIRE_ELF2 ||
		 player->client->NPC_class == CLASS_MISTVAMPIRE_ELF3 )
	{
		cgi_SP_GetStringTextString( va("SP_INGAME_%s",forcepowerDesc31[cg.DataPadforcepowerSelect]), text, sizeof(text) );
	}
	// MINEMONSTER - Shade class?
	else if ( player->client->NPC_class == CLASS_MINEMONSTER || 
	  player->client->NPC_class == CLASS_POISON ||
	  player->client->NPC_class == CLASS_IVY || 
	  player->client->NPC_class == CLASS_DEMON_POISON ||
      player->client->NPC_class == CLASS_GOLEM_POISON || 
	  player->client->NPC_class == CLASS_RANCOR || 
	  player->client->NPC_class == CLASS_INTERROGATOR )
	{
		cgi_SP_GetStringTextString( va("SP_INGAME_%s",forcepowerDesc32[cg.DataPadforcepowerSelect]), text, sizeof(text) );
	}
	else if ( player->client->NPC_class == CLASS_TAVION ||
		player->client->NPC_class == CLASS_HYLDEN ||
		player->client->NPC_class == CLASS_DESANN ||
		player->client->NPC_class == CLASS_HYLDENLORD )
	{
		cgi_SP_GetStringTextString( va("SP_INGAME_%s",forcepowerDesc33[cg.DataPadforcepowerSelect]), text, sizeof(text) );
	}
	else if ( player->client->NPC_class == CLASS_NOGHRI || player->client->NPC_class == CLASS_ZEPH ||
		player->client->NPC_class == CLASS_DECEIVER || player->client->NPC_class == CLASS_FLIER2 ) 
	
	{
		cgi_SP_GetStringTextString( va("SP_INGAME_%s",forcepowerDesc34[cg.DataPadforcepowerSelect]), text, sizeof(text) );
	}
	else if ( player->client->NPC_class == CLASS_JAN || player->client->NPC_class == CLASS_WARRIOR ||
		player->client->NPC_class == CLASS_STORMTROOPER ||player->client->NPC_class == CLASS_SARAFAN ||
		player->client->NPC_class == CLASS_PROPHET || player->client->NPC_class == CLASS_GOLEM_HOLY ||
		
		// Unused but i place however for control mind and wrack 
		player->client->NPC_class == CLASS_PRISONER || player->client->NPC_class == CLASS_RODIAN ||
		player->client->NPC_class == CLASS_ARCHER ||player->client->NPC_class == CLASS_HUNTER ||
		player->client->NPC_class == CLASS_SCOUT || player->client->NPC_class == CLASS_ALCHEMIST ||
		player->client->NPC_class == CLASS_GALAK || player->client->NPC_class == CLASS_WIZARD ||
		player->client->NPC_class == CLASS_IMPWORKER ||player->client->NPC_class == CLASS_BEAST )
	{
		cgi_SP_GetStringTextString( va("SP_INGAME_%s",forcepowerDesc35[cg.DataPadforcepowerSelect]), text, sizeof(text) );
	}
	else if ( player->client->NPC_class == CLASS_VAMPIRE_ELF2 ||
		player->client->NPC_class == CLASS_VAMPIRE_ELF3 )
 	{
		cgi_SP_GetStringTextString( va("SP_INGAME_%s",forcepowerDesc36[cg.DataPadforcepowerSelect]), text, sizeof(text) );
	}
	// UNUSED
	// ARTIFACTS 
	else if ( player->client->NPC_class == CLASS_R2D2 ||
		player->client->NPC_class == CLASS_R5D2 ||
		player->client->NPC_class == CLASS_SEEKER ||
		player->client->NPC_class == CLASS_REMOTE )		
 	{
		cgi_SP_GetStringTextString( va("SP_INGAME_%s",forcepowerDesc[cg.DataPadforcepowerSelect]), text, sizeof(text) );
	}
	else if ( player->client->NPC_class == CLASS_ELDERGOD ||
		player->client->NPC_class == CLASS_SAND_CREATURE || 
		player->client->NPC_class == CLASS_TENTACLE )
 	{
		cgi_SP_GetStringTextString( va("SP_INGAME_%s",forcepowerDesc[cg.DataPadforcepowerSelect]), text, sizeof(text) );
	}	
	else 
	{
		cgi_SP_GetStringTextString( va("SP_INGAME_%s",forcepowerDesc[cg.DataPadforcepowerSelect]), text, sizeof(text) );
	}
	
	//cgi_SP_GetStringTextString( va("SP_INGAME_%s",forcepowerDesc[cg.DataPadforcepowerSelect]), text, sizeof(text) );

	if (player->client->ps.forcePowerLevel[showDataPadPowers[cg.DataPadforcepowerSelect]]==1)
	{
		//cgi_SP_GetStringTextString( va("SP_INGAME_%s",forcepowerLvl1Desc[cg.DataPadforcepowerSelect]), text2, sizeof(text2) );
		if (!cgi_SP_GetStringTextString( va("SP_INGAME_%s",forcepowerLvl1Desc[cg.DataPadforcepowerSelect]), text2, sizeof(text2) ))
		{
			cgi_SP_GetStringTextString( va("SPMOD_INGAME_%s",forcepowerLvl1Desc[cg.DataPadforcepowerSelect]), text2, sizeof(text2) );
		}
	}
	else if (player->client->ps.forcePowerLevel[showDataPadPowers[cg.DataPadforcepowerSelect]]==2)
	{
		//cgi_SP_GetStringTextString( va("SP_INGAME_%s",forcepowerLvl2Desc[cg.DataPadforcepowerSelect]), text2, sizeof(text2) );
		if (!cgi_SP_GetStringTextString( va("SP_INGAME_%s",forcepowerLvl2Desc[cg.DataPadforcepowerSelect]), text2, sizeof(text2) ))
		{
			cgi_SP_GetStringTextString( va("SPMOD_INGAME_%s",forcepowerLvl2Desc[cg.DataPadforcepowerSelect]), text2, sizeof(text2) );
		}
	}
	else 
	{
		//cgi_SP_GetStringTextString( va("SP_INGAME_%s",forcepowerLvl3Desc[cg.DataPadforcepowerSelect]), text2, sizeof(text2) );
		if (!cgi_SP_GetStringTextString( va("SP_INGAME_%s",forcepowerLvl3Desc[cg.DataPadforcepowerSelect]), text2, sizeof(text2) ))
		{
			cgi_SP_GetStringTextString( va("SPMOD_INGAME_%s",forcepowerLvl3Desc[cg.DataPadforcepowerSelect]), text2, sizeof(text2) );
		}
	}

	if (text[0])
	{
		const short textboxXPos = 40;
		const short textboxYPos = 60;
		const int	textboxWidth = 560;
		const int	textboxHeight = 300;
		const float	textScale = 1.0f;

		CG_DisplayBoxedText(textboxXPos,textboxYPos,textboxWidth,textboxHeight,va("%s%s",text,text2),
													4,
													textScale,
													colorTable[CT_WHITE]	
													);
	}
}

// actually, these are pretty pointless so far in CHC, since in TA codebase they were used only so init some HUD
//	function ptrs to allow cinematics in onscreen displays. So far, we don't use those, but here they are anyway...
//
/*	These stupid pragmas don't work, they still give the warning. Forget it, REM the lot.

#pragma warning ( disable : 4505)		// unreferenced local function has been removed
static int CG_PlayCinematic(const char *name, float x, float y, float w, float h) {
  return trap_CIN_PlayCinematic(name, x, y, w, h, CIN_loop);
}

static void CG_StopCinematic(int handle) {
  trap_CIN_StopCinematic(handle);
}

static void CG_DrawCinematic(int handle, float x, float y, float w, float h) {
  trap_CIN_SetExtents(handle, x, y, w, h);
  trap_CIN_DrawCinematic(handle);
}

static void CG_RunCinematicFrame(int handle) {
  trap_CIN_RunCinematic(handle);
}
#pragma warning ( default : 4505)
*/




