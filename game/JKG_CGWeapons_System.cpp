#include "../cgame/cg_local.h"
#include "../game/bg_local.h"
#include "JKGCG_weapons.h"
#include "../game/anims.h"
#include "../cgame/cg_media.h"
#include "../cgame/FxScheduler.h"

GWInfo_t		GW_weapons[MAX_WEAPONS];
/// HERE WE SLAP ALL THE STUFF THAT THE FILE ASK SO I NOT NEED TO PUT INTO OTHER FILES. 
/// THESE FUNCTIONS MUST BE INDIPENDENT BY DEFAULT GAME! 
/*
#include "../cgame/animtable.h"
int ParseGenericAnimationFile ( animation_t *animset, size_t maxAnimations, const stringID_table_t *animTableOut, const char *filename, const char *fileText )
{
    int i;
    int animNum;
    float fps;
    const char *token;
    const char *text_p = fileText;

    //initialize anim array so that from 0 to MAX_ANIMATIONS, set default values of 0 1 0 100
	for ( i = 0; i < maxAnimations; i++ )
	{
		animset[i].firstFrame = 0;
		animset[i].numFrames = 0;
		animset[i].loopFrames = -1;
		animset[i].frameLerp = 100;
	}

    i = 0;
	// read information for each frame
	while ( 1 ) 
	{
		token = COM_Parse (&text_p);

		if ( !token || !token[0] ) 
		{
			break;
		}

		animNum = GetIDForString (const_cast<stringID_table_t *>(animTableOut), token);
		if ( animNum == -1 )
		{
//#ifndef FINAL_BUILD
#ifdef _DEBUG
			Com_Printf (S_COLOR_RED "WARNING: Unknown token %s in %s\n", token, filename);
			while ( token[0] )
			{
				token = COM_ParseExt (&text_p, qfalse);	//returns empty string when next token is EOL
			}
#endif
			continue;
		}
		
		if ( i >= maxAnimations )
		{
		    Com_Printf (S_COLOR_RED "WARNING: Max number of animations reached. Discontinuing animation file parsing.\n");
		    break;
		}

		token = COM_Parse (&text_p);
		if ( !token ) 
		{
			break;
		}
		animset[animNum].firstFrame = atoi( token );

		token = COM_Parse( &text_p );
		if ( !token ) 
		{
			break;
		}
		animset[animNum].numFrames = atoi( token );

		token = COM_Parse(&text_p);
		if ( !token ) 
		{
			break;
		}
		animset[animNum].loopFrames = atoi( token );

		token = COM_Parse(&text_p);
		if ( !token ) 
		{
			break;
		}
		fps = atof( token );
		if ( fps == 0 ) 
		{
			fps = 1;//Don't allow divide by zero error
		}
		if ( fps < 0 )
		{//backwards
			animset[animNum].frameLerp = floor (1000.0f / fps);
		}
		else
		{
			animset[animNum].frameLerp = ceil (1000.0f / fps);
		}
		
		i++;
	}
	
	return i;
}
*/





/////////////////// JKG WEAPON INIZIALITE!! ///////////////////////
//=========================================================
// Description:
// Returns true if modelPath is a GHOUL2 model file.
//=========================================================

static qboolean CG_IsGhoul2Model ( const char *modelPath )
{
    size_t len = strlen (modelPath);
    if ( len <= 4 )
    {
        return qfalse;
    }
    
    if ( Q_stricmp (modelPath + len - 4, ".glm") == 0 )
    {
        return qtrue;
    }
    
    return qfalse;
}

#define MAX_WEAPON_ANIMFILE_LENGTH (4096)

const stringID_table_t weaponAnimTable[MAX_WEAPON_ANIMATIONS + 1] =
{
    ENUM2STRING (ABASE),
    ENUM2STRING (E11_IDLE),
    ENUM2STRING (WPROOT),
    
    { NULL, -1 }
};

//=========================================================
// Description:
// Loads an animation config file for a view weapon.
//=========================================================
/*
static qboolean GW_LoadViewWeaponAnimations ( GWInfo_t *weapon, const char *animationPath )
{
    fileHandle_t f;
    char weaponFileData[MAX_WEAPON_ANIMFILE_LENGTH];
    int fileLength;
    int numAnims;
    
	fileLength = gi.FS_FOpenFile (animationPath, &f, FS_READ);
    if ( !f || fileLength == -1 )
    {
		gi.Printf (S_COLOR_YELLOW "Unable to load weapon animation config file %s\n", animationPath);
        return qfalse;
    }
    
    if ( (fileLength + 1) >= MAX_WEAPON_ANIMFILE_LENGTH )
    {
        gi.Printf (S_COLOR_YELLOW "Weapon animation config file %s too long (%d characters, max %d", animationPath, fileLength, MAX_WEAPON_ANIMFILE_LENGTH - 1);
		gi.FS_FCloseFile (f);
        
        return qfalse;
    }
    
	gi.FS_Read (weaponFileData, fileLength, f);
    weaponFileData[fileLength] = '\0';
    
	gi.FS_FCloseFile (f);
    
    //numAnims = ParseGenericAnimationFile (weapon->viewModelAnims, MAX_WEAPON_ANIMATIONS, weaponAnimTable, animationPath, weaponFileData);
    gi.Printf ("%s: %d animations loaded\n", animationPath, numAnims);

    return qtrue;
}
*/
//=========================================================
// Description:
// Loads a view weapon.
//=========================================================
/*
static void GW_LoadViewWeapon ( GWInfo_t *weapon, const char *modelPath )
{
	  char file[MAX_QPATH];
    char *slash;
//    int root;
//    int model;
    
    Q_strncpyz (file, modelPath, sizeof (file));
    slash = Q_strrchr (file, '/');
    
    Q_strncpyz (slash, "/model_default.skin", sizeof (file) - (slash - file));  
    weapon->viewModelSkin = cgi_R_RegisterSkin (file);
    

	gi.G2API_JKGInitGhoul2Model (&weapon->g2ViewModel, modelPath, 0, weapon->viewModelSkin, 0, 0, 0);
	if ( !gi.G2_HaveWeGhoul2Models (weapon->g2ViewModel) )
    {
        return;
    }
    
	/*root = gi.G2API_JKGAddBolt (weapon->g2ViewModel, 0, "model_root");
    
	model = gi.G2API_JKGInitGhoul2Model (&weapon->g2ViewModel, "models/weapons2/test3_r_hand/model.glm", 1, 0, gi.RE_RegisterSkin ("models/weapons2/test3_r_hand/model_default.skin"), 0, 0);
    if ( !gi.G2API_HasGhoul2ModelOnIndex (&weapon->g2ViewModel, 1) )
    {
		gi.Printf (S_COLOR_YELLOW "WARNING: Failed to load right hand model.\n");
    }
    
    gi.G2API_SetBoltInfo (weapon->g2ViewModel, 0, root);
    
    gi.G2API_JKGInitGhoul2Model (&weapon->g2ViewModel, "models/weapons2/test3_l_hand/model.glm", 2, 0, trap->R_RegisterSkin ("models/weapons2/test3_l_hand/model_default.skin"), 0, 0);
    if ( !gi.G2API_HasGhoul2ModelOnIndex (&weapon->g2ViewModel, 2) )
    {
		gi.Printf (S_COLOR_YELLOW "WARNING: Failed to load left hand model.\n");
    }
    
	gi.G2API_SetBoltInfo (weapon->g2ViewModel, 0, root);
   
    memset (weapon->viewModelAnims, 0, sizeof (weapon->viewModelAnims));
	gi.G2API_JKGGetGLAName (weapon->g2ViewModel, 0, file);
    if ( !file[0] )
    {
        return;
    }
    
    slash = Q_strrchr (file, '/');
    
    Q_strncpyz (slash, "/animation.cfg", sizeof (file) - (slash - file));
    GW_LoadViewWeaponAnimations (weapon, file);
}

static void CG_LoadG2ModelWithSkin ( const char *modelPath, void **g2ModelPtr )
{
    void *g2Model = NULL;
	gi.G2API_JKGInitGhoul2Model (g2ModelPtr, modelPath, 0, 0, 0, 0, 0);
    
    g2Model = *g2ModelPtr;
    
	if ( gi.G2_HaveWeGhoul2Models (g2Model) && gi.G2API_SkinlessModel (g2Model, 0) )
    {
        char skinName[MAX_QPATH];
        char *slash = NULL;
        int skinLen = 0;
        qhandle_t skin;
        
        Q_strncpyz (skinName, modelPath, sizeof (skinName));
        skinLen = strlen (skinName);
        
        slash = Q_strrchr (skinName, '/');
        Q_strncpyz (slash, "/model_default.skin", sizeof (skinName) - (slash - skinName));  
        
		skin = gi.RE_RegisterSkin (skinName);
		gi.G2API_JKGSetSkin (g2Model, 0, skin, skin);
    }
}
*/
static void ReadColor ( const char *vectorString, vec3_t out )
{
    int vector[3];
    sscanf (vectorString, "%d %d %d", &vector[0], &vector[1], &vector[2]);
    
    out[0] = vector[0] / 255.0f;
    out[1] = vector[1] / 255.0f;
    out[2] = vector[2] / 255.0f;
}

static void ReadVector ( const char *vectorString, vec3_t out )
{
    sscanf (vectorString, "%f %f %f", &out[0], &out[1], &out[2]);
}

static void LoadFireModeAssets ( weaponDrawData_t *drawData, const weaponFireModeStats_t* fireMode, const weaponVisualFireMode_t *fireModeVisuals )
{
    //qboolean isGrenade = (qboolean)(Q_stricmp (fireModeVisuals->type, "grenade") == 0);
    //qboolean isBlaster = (qboolean)(Q_stricmp (fireModeVisuals->type, "blaster") == 0);
    qboolean isTripmine = (qboolean)(Q_stricmp (fireModeVisuals->type, "tripmine") == 0);
    qboolean isDetpack = (qboolean)(Q_stricmp (fireModeVisuals->type, "detpack") == 0);
    
    int i;

    // Weapon render
    if ( fireModeVisuals->weaponRender.generic.chargingEffect[0] )
		drawData->weaponRender.generic.chargingEffect = theFxScheduler.RegisterEffect(fireModeVisuals->weaponRender.generic.chargingEffect);
	
	if ( fireModeVisuals->weaponRender.generic.muzzleEffect[0] )
        drawData->weaponRender.generic.muzzleEffect = theFxScheduler.RegisterEffect (fireModeVisuals->weaponRender.generic.muzzleEffect);
        
    ReadColor (fireModeVisuals->weaponRender.generic.muzzleLightColor, drawData->weaponRender.generic.muzzleLightColor);
    drawData->weaponRender.generic.muzzleLightIntensity = fireModeVisuals->weaponRender.generic.muzzleLightIntensity;
    
    // Weapon fire
    for ( i = 0; i < 8; i++ )
    {
        if ( fireModeVisuals->weaponFire.generic.fireSound[i][0] )
        {
            drawData->weaponFire.generic.fireSound[i] = cgi_S_RegisterSound (fireModeVisuals->weaponFire.generic.fireSound[i]);
        }
        else
        {
            break;
        }
    }

    drawData->weaponFire.generic.fireSoundCount = i;
    
    // Hitscan render
    drawData->tracelineRender.generic.lifeTime = fireModeVisuals->tracelineRender.generic.lifeTime;
    drawData->tracelineRender.generic.maxSize = fireModeVisuals->tracelineRender.generic.maxSize;
    drawData->tracelineRender.generic.minSize = fireModeVisuals->tracelineRender.generic.minSize;
    if ( fireModeVisuals->tracelineRender.generic.tracelineShader[0] )
        drawData->tracelineRender.generic.tracelineShader = cgi_R_RegisterShader (fireModeVisuals->tracelineRender.generic.tracelineShader);
    
    // Weapon charge
    if ( fireModeVisuals->weaponCharge.chargingSound[0] )
        drawData->weaponCharge.chargingSound = cgi_S_RegisterSound (fireModeVisuals->weaponCharge.chargingSound);
    
    // Projectile render
    ReadColor (fireModeVisuals->projectileRender.generic.lightColor, drawData->projectileRender.generic.lightColor);
    drawData->projectileRender.generic.lightIntensity = fireModeVisuals->projectileRender.generic.lightIntensity;
    
    if ( fireModeVisuals->projectileRender.generic.projectileEffect[0] )
        drawData->projectileRender.generic.projectileEffect = theFxScheduler.RegisterEffect (fireModeVisuals->projectileRender.generic.projectileEffect);
        
    if ( fireModeVisuals->projectileRender.generic.projectileModel[0] )
        drawData->projectileRender.generic.projectileModel = cgi_R_RegisterModel (fireModeVisuals->projectileRender.generic.projectileModel);
        




    if ( fireModeVisuals->projectileRender.generic.runSound[0] )
        drawData->projectileRender.generic.runSound = cgi_S_RegisterSound (fireModeVisuals->projectileRender.generic.runSound);
        
    if ( fireModeVisuals->projectileRender.generic.deathEffect[0] )
        drawData->projectileRender.generic.deathEffect = theFxScheduler.RegisterEffect (fireModeVisuals->projectileRender.generic.deathEffect);
    
    // Projectile miss event
    if ( isTripmine || isDetpack )
    {
        if ( fireModeVisuals->projectileMiss.explosive.stickSound[0] )
            drawData->projectileMiss.explosive.stickSound = cgi_S_RegisterSound (fireModeVisuals->projectileMiss.explosive.stickSound);
    }
    else
    {
        if ( fireModeVisuals->projectileMiss.generic.impactEffect[0] )
            drawData->projectileMiss.generic.impactEffect = theFxScheduler.RegisterEffect (fireModeVisuals->projectileMiss.generic.impactEffect);
            
        if ( fireModeVisuals->projectileMiss.grenade.shockwaveEffect[0] )
            drawData->projectileMiss.grenade.shockwaveEffect = theFxScheduler.RegisterEffect (fireModeVisuals->projectileMiss.grenade.shockwaveEffect);
    }
    
    // Projectile hit event
    if ( fireModeVisuals->projectileHitPlayer.generic.impactEffect[0] )
        drawData->projectileHitPlayer.generic.impactEffect = theFxScheduler.RegisterEffect (fireModeVisuals->projectileHitPlayer.generic.impactEffect);
        
    if ( fireModeVisuals->projectileHitPlayer.grenade.shockwaveEffect[0] )
        drawData->projectileHitPlayer.grenade.shockwaveEffect = theFxScheduler.RegisterEffect (fireModeVisuals->projectileHitPlayer.grenade.shockwaveEffect);
    
    // Projectile deflected event
    if ( fireModeVisuals->projectileDeflected.generic.deflectEffect[0] )
        drawData->projectileDeflected.generic.deflectEffect = theFxScheduler.RegisterEffect (fireModeVisuals->projectileDeflected.generic.deflectEffect);
    
    // Grenade bounce event
    if ( fireModeVisuals->grenadeBounce.grenade.bounceSound[0][0] )
        drawData->grenadeBounce.grenade.bounceSound[0] = cgi_S_RegisterSound (fireModeVisuals->grenadeBounce.grenade.bounceSound[0]);
        
    if ( fireModeVisuals->grenadeBounce.grenade.bounceSound[1][0] )
        drawData->grenadeBounce.grenade.bounceSound[1] = cgi_S_RegisterSound (fireModeVisuals->grenadeBounce.grenade.bounceSound[1]);
    
    // Explosive render
    if ( fireModeVisuals->explosiveRender.detpack.g2Model[0] )
//QQQOBJ TROUBLE        CG_LoadG2ModelWithSkin (fireModeVisuals->explosiveRender.detpack.g2Model, &drawData->explosiveRender.detpack.g2Model);
        
    drawData->explosiveRender.detpack.g2Radius = fireModeVisuals->explosiveRender.detpack.g2Radius;
    
    if ( fireModeVisuals->explosiveRender.tripmine.lineEffect[0] )
        drawData->explosiveRender.tripmine.lineEffect = theFxScheduler.RegisterEffect (fireModeVisuals->explosiveRender.tripmine.lineEffect);
    
    // Explosive blow event
    if ( fireModeVisuals->explosiveBlow.generic.explodeEffect[0] )
        drawData->explosiveBlow.generic.explodeEffect = theFxScheduler.RegisterEffect (fireModeVisuals->explosiveBlow.generic.explodeEffect);
    
    // Explosive armed event
    if ( fireModeVisuals->explosiveArm.armSound[0] )
        drawData->explosiveArm.armSound = cgi_S_RegisterSound (fireModeVisuals->explosiveArm.armSound);
}

void LoadWeaponAssets ( GWInfo_t *weaponInfo, const weaponData_t *weaponData )
{
    int i;
    char extensionlessModel[MAX_QPATH];
    const weaponVisual_t *weaponVisuals = &weaponData->visuals;
    
    weaponInfo->indicatorType = weaponVisuals->indicatorType;
	for ( i = 0; i < 3; i++ )
    {
        if ( weaponVisuals->groupedIndicatorShaders[i][0] )
        {
			weaponInfo->groupedIndicators[i] = cgi_R_RegisterShader (weaponVisuals->groupedIndicatorShaders[i]);
        }
    }
    
    if ( weaponVisuals->firemodeIndicatorShader[0] )
    {
        weaponInfo->fireModeIndicator = cgi_R_RegisterShader (weaponVisuals->firemodeIndicatorShader);
    }
    
    VectorClear (weaponInfo->gunPosition);
    if ( weaponVisuals->gunPosition[0] )
    {
        ReadVector (weaponVisuals->gunPosition, weaponInfo->gunPosition);
    }
    
    VectorClear (weaponInfo->ironsightsPosition);
    if ( weaponVisuals->ironsightsPosition[0] )
    {
        ReadVector (weaponVisuals->ironsightsPosition, weaponInfo->ironsightsPosition);
    }
    else
    {
        VectorCopy (weaponInfo->gunPosition, weaponInfo->ironsightsPosition);
    }
    weaponInfo->ironsightsFov = weaponVisuals->ironsightsFov;
    
    if ( CG_IsGhoul2Model (weaponVisuals->world_model) )
    {
		gi.G2API_JKGInitGhoul2Model (&weaponInfo->g2WorldModel, weaponVisuals->world_model, 0, 0, 0, 0, 0);
		if ( !gi.G2_HaveWeGhoul2Models (weaponInfo->g2WorldModel) )
        {
            weaponInfo->g2WorldModel = NULL;
        }
    }
    memset (weaponInfo->barrelModels, NULL_HANDLE, sizeof (weaponInfo->barrelModels));
    COM_StripExtension (weaponVisuals->view_model, extensionlessModel, sizeof(extensionlessModel));
    
    for ( i = 0; i < weaponVisuals->barrelCount; i++ )
    {
        const char *barrelModel;
        int len;
        qhandle_t barrel;

		if( i == 0 )
			barrelModel = va("%s_barrel.md3", extensionlessModel);
		else
			barrelModel = va("%s_barrel%i.md3", extensionlessModel, i+1);

		len = strlen( barrelModel );
        
        if ( (len + 1) > MAX_QPATH )
        {
            gi.Printf (S_COLOR_YELLOW "Warning: barrel model path %s is too long (%d chars). Max length is 63.\n", barrelModel, len);
            break;
        }
        
        barrel = cgi_R_RegisterModel (barrelModel);
        if ( barrel == NULL_HANDLE )
        {
            break;
        }
        
        weaponInfo->barrelModels[i] = barrel;
    }
    
    // Scope render
    if ( weaponVisuals->scopeShader[0] )
        weaponInfo->scopeShader = cgi_R_RegisterShader (weaponVisuals->scopeShader);
    
    // Scope toggle
    if ( weaponVisuals->scopeStartSound[0] )
        weaponInfo->scopeStartSound = cgi_S_RegisterSound (weaponVisuals->scopeStartSound);
        
    if ( weaponVisuals->scopeStopSound[0] )
        weaponInfo->scopeStopSound = cgi_S_RegisterSound (weaponVisuals->scopeStopSound);
    
    // Scope zoom
    if ( weaponVisuals->scopeLoopSound[0] )
        weaponInfo->scopeLoopSound =cgi_S_RegisterSound (weaponVisuals->scopeLoopSound);
        
    weaponInfo->scopeSoundLoopTime = weaponVisuals->scopeSoundLoopTime;

    //JKG_LoadFireModeAssets (&weaponInfo->primDrawData, &weaponData->firemodes[0], &weaponVisuals->primary);
    //JKG_LoadFireModeAssets (&weaponInfo->altDrawData, &weaponData->firemodes[1], &weaponVisuals->secondary);
	for( i = 0; i < weaponData->numFiringModes; i++ )
	{
		LoadFireModeAssets ( &weaponInfo->drawData[i], &weaponData->firemodes[i], &weaponVisuals->visualFireModes[i] );
	}
}


/*
-------------------------
TurretProjectileThink
-------------------------
*/
void TurretProjectileThink(  centity_t *cent, const struct weaponInfo_s *weapon )
{
	vec3_t forward;

	if ( VectorNormalize2( cent->currentState.pos.trDelta, forward ) == 0.0f )
	{
		forward[2] = 1.0f;
	}

	gi.FX_PlayEffectID( cgs.effects.turretShotEffect, cent->lerpOrigin, forward, -1, -1, false );
}

/*
=================
CG_RegisterWeapon

The server says this item is used on this level
=================
*/




/////////////////////////////////////// WEAPON JKG LIBRARY /////////
/*
// cg_weapons.c -- events and effects dealing with weapons
#include <math.h>
#include <float.h>

#include "cg_local.h"
#include "fx_local.h"

#include "ghoul2/G2.h"

extern vec4_t	bluehudtint;
extern vec4_t	redhudtint;
extern float	*hudTintColor;

static weaponInfo_t cg_weapons[MAX_WEAPON_TABLE_SIZE];

//=========================================================
// JKG_CalculateFiremodePhase
//---------------------------------------------------------
// Returns a value between 0 and 1, where 0 represents
// previous gun position, and 1 represents gun in
// correct position. All values in between are some
// position between normal position and firing mode
// position.
//=========================================================

float CalculateFireModePhase ( void )
{
	double phase;
	unsigned int time = cg.fireModeChangeTime;
	weaponData_t *wp = BG_GetWeaponDataByIndex( cg.predictedPlayerState.weaponId );

	phase = Q_min(cg.time - time, wp->ironsightsTime)/(double)wp->ironsightsTime;

	return Q_min (1.0f, Q_max (0.0f, phase));
}

qboolean FiringModeAnimsAreTheSame( int transition )
{
	switch(transition)
	{
		case FMTRANS_NONE_NONE:
		case FMTRANS_RAISED_RAISED:
		case FMTRANS_TILTED_TILTED:
			return qtrue;
		default:
			return qfalse;
	}
}

//=========================================================
// CalculateSprintPhase
//---------------------------------------------------------
// Returns a value between 0 and 1, where 0 represents
// normal gun position, and 1 represents sprinting in
// full. All values in between are some position between
// normal position and full-on sprinting.
//=========================================================
float CalculateSprintPhase( const playerState_t *ps )
{
	double phase;
	unsigned int time = ps->sprintTime & ~SPRINT_MSB;
	float newBlend;

	if ( ps->sprintTime & SPRINT_MSB )
	{
		phase = CubicBezierInterpolate (Q_min (cg.time - time, SPRINT_TIME) / (double)SPRINT_TIME, 0.0, 0.0, 1.0, 1.0);
		newBlend = Q_min(1.0f, Q_max(0.0f, phase));
		if (newBlend >= cg.sprintBlend) {
			cg.sprintBlend = newBlend;
		}
	}
	else
	{
		phase = cg.sprintBlend - CubicBezierInterpolate (Q_min (cg.time - time, SPRINT_TIME * cg.sprintBlend) / (double)(SPRINT_TIME * cg.sprintBlend), 0.0, 0.0, 1.0, 1.0);
	}

	return Q_min (1.0f, Q_max (0.0f, phase));
}

void CG_InitWeapons ( void )
{
	memset (cg_weapons, 0, sizeof (cg_weapons));
}

// set up the appropriate ghoul2 info to a refent
void SetGhoul2InfoRef( refEntity_t *ent, refEntity_t	*s1)
{
	ent->ghoul2 = s1->ghoul2;
	VectorCopy( s1->modelScale, ent->modelScale);
	ent->radius = s1->radius;
	VectorCopy( s1->angles, ent->angles);
}

/*
=================
CG_RegisterItemVisuals

The server says this item is used on this level
=================
*/
/*
void CG_RegisterItemVisuals( int itemNum ) {
	itemInfo_t		*itemInfo;
	gitem_t			*item;
	int				handle;

	if ( itemNum < 0 || itemNum >= bg_numItems ) {
		trap->Error( ERR_DROP, "CG_RegisterItemVisuals: itemNum %d out of range [0-%d]", itemNum, bg_numItems-1 );
	}

	itemInfo = &cg_items[ itemNum ];
	if ( itemInfo->registered ) {
		return;
	}

	item = &bg_itemlist[ itemNum ];

	memset( itemInfo, 0, sizeof( &itemInfo ) );
	itemInfo->registered = qtrue;

	if (item->giType == IT_WEAPON &&
		(item->giTag == WP_THERMAL || item->giTag == WP_TRIP_MINE || item->giTag == WP_DET_PACK))
	{
		itemInfo->models[0] = cgi_R_RegisterModel( item->world_model[1] );
	}
	else
	{
		itemInfo->models[0] = cgi_R_RegisterModel( item->world_model[0] );
	}

	if (!Q_stricmp(&item->world_model[0][strlen(item->world_model[0]) - 4], ".glm"))
	{
		handle = trap->G2API_InitGhoul2Model(&itemInfo->g2Models[0], item->world_model[0], 0 , 0, 0, 0, 0);
		if (handle<0)
		{
			itemInfo->g2Models[0] = NULL;
		}
		else
		{
			itemInfo->radius[0] = 60;
		}
	}

	if (item->icon)
	{
		if (item->giType == IT_HEALTH)
		{ //medpack gets nomip'd by the ui or something I guess.
			itemInfo->icon = cgi_R_RegisterShaderNoMip( item->icon );
		}
		else
		{
			itemInfo->icon = cgi_R_RegisterShader( item->icon );
		}
	}
	else
	{
		itemInfo->icon = 0;
	}

	if ( item->giType == IT_WEAPON ) {
		unsigned int numVariations = BG_NumberOfWeaponVariations (item->giTag);
		unsigned int i;
		for ( i = 0; i < numVariations; i++ )
		{
			CG_RegisterWeapon( item->giTag, i );
		}
	}

	//
	// powerups have an accompanying ring or sphere
	//
	if ( item->giType == IT_POWERUP || item->giType == IT_HEALTH || item->giType == IT_ARMOR ) {
		if ( item->world_model[1] ) {
			itemInfo->models[1] = cgi_R_RegisterModel( item->world_model[1] );
		}
	}
}


/*
========================================================================================

VIEW WEAPON

========================================================================================
*/
/*

#define WEAPON_FORCE_BUSY_HOLSTER

#ifdef WEAPON_FORCE_BUSY_HOLSTER
//rww - this was done as a last resort. Forgive me.
static int cgWeapFrame = 0;
static int cgWeapFrameTime = 0;
#endif

/*
=================
CG_MapTorsoToWeaponFrame

=================
*/
/*
static int CG_MapTorsoToWeaponFrame( const clientInfo_t *ci, int frame, int animNum ) {
	float animspeed = 1.0f;
	const weaponData_t *weaponData = GetWeaponData(cg.predictedPlayerState.weapon, cg.predictedPlayerState.weaponVariation);
	animation_t *animations = bgHumanoidAnimations;

#ifdef WEAPON_FORCE_BUSY_HOLSTER
	if (cg.snap->ps.forceHandExtend != HANDEXTEND_NONE || cgWeapFrameTime > cg.time)
	{ //the reason for the after delay is so that it doesn't snap the weapon frame to the "idle" (0) frame
		//for a very quick moment
		if (cgWeapFrame < 6)
		{
			cgWeapFrame = 6;
			cgWeapFrameTime = cg.time + 10;
		}

		if (cgWeapFrameTime < cg.time && cgWeapFrame < 10)
		{
			cgWeapFrame++;
			cgWeapFrameTime = cg.time + 10;
		}

		if (cg.snap->ps.forceHandExtend != HANDEXTEND_NONE &&
			cgWeapFrame == 10)
		{
			cgWeapFrameTime = cg.time + 100;
		}

		return cgWeapFrame;
	}
	else
	{
		cgWeapFrame = 0;
		cgWeapFrameTime = 0;
	}
#endif

	switch( animNum )
	{
	case TORSO_DROPWEAP1:
		if ( frame >= animations[animNum].firstFrame && frame < animations[animNum].firstFrame + 5 )
		{
			return frame - animations[animNum].firstFrame + 6;
		}
		break;

	case TORSO_RAISEWEAP1:
		if ( frame >= animations[animNum].firstFrame && frame < animations[animNum].firstFrame + 4 )
		{
			return frame - animations[animNum].firstFrame + 6 + 4;
		}
		break;
	case BOTH_ATTACK1:
	case BOTH_ATTACK2:
	case BOTH_ATTACK3:
	case BOTH_ATTACK4:
	case BOTH_ATTACK10:
	case BOTH_BASE_PISTOLSHOOT:
	case BOTH_BASE_RIFLESHOOT:
	case BOTH_THERMAL_THROW:
		if ( frame >= animations[animNum].firstFrame && frame < animations[animNum].firstFrame + 6 )
		{
			return 1 + ( frame - animations[animNum].firstFrame );
		}

		break;
	}
	if(animNum == weaponData->anims.sprint.torsoAnim)
	{
		if( weaponData->firstPersonSprintStyle == -1 )
		{
			// Jedi Knight Galaxies: Sprint animations (manually) via MD3 file. Uses 5 frames after the last (iirc?)
			if( frame >= animations[animNum].firstFrame && frame < animations[animNum].firstFrame + 13 )
			{
				return frame - animations[animNum].firstFrame + 6 + 4 + 5;
			}
		}
	}
	else if(animNum == weaponData->anims.reload.torsoAnim)
	{
		// TODO: reload animations (probably whenever we get a decent weapon skeleton for some of these guns?)
		if ( frame >= animations[animNum].firstFrame && frame < animations[animNum].firstFrame + animations[animNum].numFrames )
		{
			int theFrame = (frame - animations[animNum].firstFrame) + 6;
			int frameDif = (animations[animNum].numFrames - theFrame) + 6;
			if(frameDif <= 6 && cg.predictedPlayerState.weaponTime <= jkg_viewmodelPopup.integer)
			{
				// doesn't even matter what the frame is at this rate, just do it already
				return 15-frameDif;
			}
			else
			{
				CAP(theFrame, 10);
				return theFrame;
			}
		}
	}
	return -1;
}

//=========================================================
// Description:
// Updates the animations for the current view model.
//=========================================================
void CG_AnimateViewWeapon ( const playerState_t *ps )
{
	weaponInfo_t *weapon = NULL;

	weapon = CG_WeaponInfo (ps->weapon, ps->weaponVariation);

	if ( ps->persistant[PERS_TEAM] == TEAM_SPECTATOR )
	{
		return;
	}

	if ( ps->pm_type == PM_INTERMISSION )
	{
		return;
	}

	if ( !weapon->g2ViewModel )
	{
		return;
	}

	if ( cg.viewWeaponAnimation != NULL )
	{
		animation_t *anim = cg.viewWeaponAnimation;
		trap->G2API_SetBoneAnim (weapon->g2ViewModel, 0, "model_root", anim->firstFrame, anim->numFrames, BONE_ANIM_OVERRIDE | BONE_ANIM_BLEND, 50.0f / anim->frameLerp, cg.time, anim->firstFrame, 150);

		cg.viewWeaponAnimation = NULL;
	}
}

/*
==============
ViewPitchGunTilt

Tilt the gun based on our pitch
Won't occur during sprint or ironsights
==============
*/
/*
static void ViewPitchGunTilt(vec3_t origin, float ironSightsPhase, float sprintPhase) {
	float sprintInverse = 1.0f - sprintPhase;
	float sightsInverse = 1.0f - ironSightsPhase;

	float zamount;
	float xamount;

	xamount = Q_powf(cg.refdef.viewangles[PITCH], 1.02f);
	zamount = (cg.refdef.viewangles[PITCH] > 0) ? 0 : Q_powf(cg.refdef.viewangles[PITCH], 1.001f);

	xamount *= 1.0f / 45.0f;
	zamount *= 1.0f / 45.0f;

	xamount += 1.0f;
	zamount += 1.0f;

	xamount *= sprintInverse * sightsInverse;
	zamount *= sprintInverse * sightsInverse;

	if (xamount > 3.0f) {
		xamount = 3.0f;
	}
	else if (xamount < -2.0f) {
		xamount = -2.0f;
	}

	if (zamount > 2.0f) {
		zamount = 2.0f;
	}
	else if (zamount < -2.0f) {
		zamount = -2.0f;
	}

	VectorMA(origin, xamount, cg.refdef.viewaxis[0], origin);
	VectorMA(origin, zamount, cg.refdef.viewaxis[2], origin);
}

/*
==============
JKG_FiremodeTransitionAnimation

Performs the first person animation when switching firing modes
==============
*/
/*
extern int GetTransitionForFiringModeSet(int previous, int next);
static void FireModeTransitionAnimation(vec3_t origin, vec3_t angles, weaponData_t* prevWeaponData, weaponData_t* weaponData) {
	if (cg.lastFiringMode != cg.predictedPlayerState.firingMode && cg.lastFiringModeGun != cg.predictedPlayerState.weaponId)
	{
		cg.fireModeTransition = JKG_GetTransitionForFiringModeSet(prevWeaponData->visuals.visualFireModes[cg.lastFiringMode].animType, weaponData->visuals.visualFireModes[cg.predictedPlayerState.firingMode].animType);

		cg.lastFiringMode = cg.predictedPlayerState.firingMode;
		cg.lastFiringModeTime = cg.time + 200; // matches SP 1:1
		cg.lastFiringModeGun = cg.predictedPlayerState.weaponId;
	}
	else if (cg.lastFiringMode != cg.predictedPlayerState.firingMode)
	{
		cg.lastFiringMode = cg.predictedPlayerState.firingMode;
		cg.lastFiringModeTime = cg.time + 200; // matches SP 1:1
		cg.lastFiringModeGun = cg.predictedPlayerState.weaponId;
	}

	// Fire mode animations -- Jedi Knight Galaxies
	if (!JKG_FiringModeAnimsAreTheSame(cg.fireModeTransition))
	{
		int transition = cg.fireModeTransition;
		float firingModeAnimPhase = JKG_CalculateFireModePhase();
		vec3_t beginningOrigin = { 0, 0, 0 }, endOrigin = { 0, 0, 0 };				// Start of the firing mode transition
		vec3_t beginningDir = { 0, 0, 0 }, endDir = { 0, 0, 0 };					// End of the firing mode transition

		// Get the animation along its track there.
		switch (transition)
		{
		case FMTRANS_NONE_RAISED:
		case FMTRANS_TILTED_RAISED:
			// Final destination for these use the same as the pistol sprint
			// animation, for now. --eez
			endDir[PITCH] = -15.0f;
			endOrigin[2] = -1;
			break;
		case FMTRANS_RAISED_NONE:
			beginningDir[PITCH] = -15.0f;
			beginningOrigin[2] = -1;
			// Cancel out the beginning of the dir and origin
			endDir[PITCH] = 15.0f;
			endOrigin[2] = 1;
			break;
		}

		angles[PITCH] += (beginningDir[PITCH] + (endDir[PITCH] * firingModeAnimPhase));
		angles[YAW] += (beginningDir[YAW] + (endDir[YAW] * firingModeAnimPhase));
		angles[ROLL] += (beginningDir[ROLL] + (endDir[ROLL] * firingModeAnimPhase));

		VectorMA(origin, beginningOrigin[0] + (endOrigin[0] * firingModeAnimPhase), cg.refdef.viewaxis[0], origin);
		VectorMA(origin, beginningOrigin[1] + (endOrigin[1] * firingModeAnimPhase), cg.refdef.viewaxis[1], origin);
		VectorMA(origin, beginningOrigin[2] + (endOrigin[2] * firingModeAnimPhase), cg.refdef.viewaxis[2], origin);
	}
}

/*
==============
JKG_SprintViewmodelAnimation

Not a real animation, just faking angles
==============
*/

/*
static void JKG_SprintViewmodelAnimation(vec3_t origin, vec3_t angles, weaponData_t* weaponData, float sprintPhase, float scale) {
	// Now calculate where our gun will be angled
	float sprintXAdd = 0;
	//*jkg_debugSprintX.value
	float sprintYAdd = 0; //jkg_debugSprintY.value
	float sprintZAdd = 0; // jkg_debugSprintZ.value
	float sprintPitchAdd = 0; //jkg_debugSprintPitch.value
	float sprintYawAdd = 0; //jkg_debugSprintYaw.value/
	float sprintRollAdd = 0///*jkg_debugSprintRoll.value
	int sprintStyle = (debugSprintStyle.integer != -2) ? debugSprintStyle.integer : weaponData->firstPersonSprintStyle;		// the cvar lets us switch gun sprinting styles on the fly
	int sprintStyle = weaponData->firstPersonSprintStyle;
	bool bExtremeYaw = false;

	switch (sprintStyle)
	{
		// ANGLES/ORIGIN BEFORE BOBBING
	case SPRINTSTYLE_LOWERED:
	case SPRINTSTYLE_LOWERED_SLIGHT:
	case SPRINTSTYLE_LOWERED_HEAVY:
		sprintXAdd = 1;
		sprintYAdd = -1;
		sprintZAdd = -2;
		sprintPitchAdd = 20;
		break;

	case SPRINTSTYLE_SIDE:
	case SPRINTSTYLE_SIDE_SLIGHT:
	case SPRINTSTYLE_SIDE_HEAVY:
		sprintXAdd = 1;
		sprintZAdd = -1;
		sprintPitchAdd = 15;
		sprintYawAdd = 40;
		sprintRollAdd = -10;
		bExtremeYaw = true;
		break;

	case SPRINTSTYLE_RAISED:
	case SPRINTSTYLE_RAISED_SLIGHT:
	case SPRINTSTYLE_RAISED_HEAVY:
		sprintPitchAdd = -35;
		sprintZAdd = 1;
		break;

	case SPRINTSTYLE_ANGLED_DOWN:
	case SPRINTSTYLE_ANGLED_DOWN_SLIGHT:
	case SPRINTSTYLE_ANGLED_DOWN_HEAVY:
		sprintPitchAdd = 20;
		sprintZAdd = -1;
		break;

	case SPRINTSTYLE_SIDE_UP:
	case SPRINTSTYLE_SIDE_UP_SLIGHT:
	case SPRINTSTYLE_SIDE_UP_HEAVY:
		sprintXAdd = 1;
		sprintZAdd = 1;
		sprintPitchAdd = 15;
		sprintYawAdd = 40;
		sprintRollAdd = -10;
		bExtremeYaw = true;
		break;

	case SPRINTSTYLE_SIDE_MEDIUM:
	case SPRINTSTYLE_SIDE_MEDIUM_SLIGHT:
	case SPRINTSTYLE_SIDE_MEDIUM_HEAVY:
		sprintZAdd = -2;
		sprintYAdd = -1;
		sprintPitchAdd = 5;
		sprintYawAdd = 40;
		sprintRollAdd = -10;
		bExtremeYaw = true;
		break;
	}

	// Messy-ish code out the yin-yang here
	if (bExtremeYaw) 
	{
		float fPitchFactor = 1.0f - (abs(cg.predictedPlayerState.viewangles[PITCH]) / 90.0f);
		angles[PITCH] += (sprintPitchAdd * sprintPhase) * fPitchFactor;
		angles[YAW] += (sprintYawAdd * sprintPhase) * fPitchFactor;
		angles[ROLL] += (sprintRollAdd * sprintPhase) * fPitchFactor;
	}
	else 
	{
		angles[PITCH] += (sprintPitchAdd * sprintPhase);
		angles[YAW] += (sprintYawAdd * sprintPhase);
		angles[ROLL] += (sprintRollAdd * sprintPhase);
	}

	VectorMA(origin, sprintXAdd * sprintPhase, cg.refdef.viewaxis[0], origin);
	VectorMA(origin, sprintYAdd * sprintPhase, cg.refdef.viewaxis[1], origin);
	VectorMA(origin, sprintZAdd * sprintPhase, cg.refdef.viewaxis[2], origin);

	if (CalculateSprintPhase(&cg.predictedPlayerState) > 0.001)
	{
		float bobPitchAdd = 0; //jkg_debugSprintBobPitch.value
		float bobYawAdd = 0; //jkg_debugSprintBobYaw.value*
		float bobRollAdd = 0; //*jkg_debugSprintBobRoll.value
		float bobXAdd = 0; // jkg_debugSprintBobX.value //0;
		float bobYAdd = 0; // jkg_debugSprintBobY.value //0;
		float bobZAdd = 0; //jkg_debugSprintBobZ.value//0;
		// Calculate bobbing add
		switch (sprintStyle)
		{
			// BOBBING ANGLE/ORIGIN
		case SPRINTSTYLE_LOWERED_SLIGHT:
			bobYAdd = 0.001;
			bobYawAdd = 0.03;
			break;

		case SPRINTSTYLE_LOWERED_HEAVY:
			bobYawAdd = 0.04;
			bobPitchAdd = 0.01;
			bobRollAdd = 0.01;
			bobXAdd = 0.0005;
			bobYAdd = 0.002;
			break;

		case SPRINTSTYLE_SIDE_SLIGHT:
		case SPRINTSTYLE_SIDE_UP_SLIGHT:
		case SPRINTSTYLE_SIDE_MEDIUM_SLIGHT:
			bobYawAdd = 0.02;
			break;

		case SPRINTSTYLE_SIDE_HEAVY:
		case SPRINTSTYLE_SIDE_UP_HEAVY:
		case SPRINTSTYLE_SIDE_MEDIUM_HEAVY:
			bobYawAdd = 0.06;
			bobPitchAdd = 0.002;
			bobRollAdd = 0.002;
			break;

		case SPRINTSTYLE_RAISED_SLIGHT:
		case SPRINTSTYLE_ANGLED_DOWN_SLIGHT:
			bobZAdd = 0.0005;
			bobPitchAdd = 0.005;
			break;

		case SPRINTSTYLE_RAISED_HEAVY:
		case SPRINTSTYLE_ANGLED_DOWN_HEAVY:
			bobZAdd = 0.001;
			bobPitchAdd = 0.01;
			bobRollAdd = 0.004;
			break;
		}

		angles[ROLL] += scale * cg.bobfracsin * (0.005 + (bobPitchAdd*sprintPhase));
		angles[YAW] += scale * cg.bobfracsin * (0.01 + (bobYawAdd*sprintPhase));
		angles[PITCH] += cg.xyspeed * cg.bobfracsin * (0.005 + (bobRollAdd*sprintPhase));

		VectorMA(origin, scale * cg.bobfracsin * (bobXAdd*sprintPhase), cg.refdef.viewaxis[0], origin);
		VectorMA(origin, scale * cg.bobfracsin * (bobYAdd*sprintPhase), cg.refdef.viewaxis[1], origin);
		VectorMA(origin, scale * cg.bobfracsin * (bobZAdd*sprintPhase), cg.refdef.viewaxis[2], origin);
	}
}

/*
==============
ViewmodelMomentum
==============
*/
/*
static void ViewmodelMomentum(vec3_t angles) 
{
	static vec3_t prevAngles = { 0.0f };
	static vec3_t prevAngles2 = { 0.0f };

	vec3_t playAngles;
	
	VectorCopy(cg.predictedPlayerState.viewangles, playAngles);

	vec3_t delta = {
		AngleSubtract(playAngles[PITCH], prevAngles[PITCH]),
		0,
		AngleSubtract(playAngles[YAW], prevAngles[YAW])
	};

	// Average the difference over three frames to get the smoothest result
	delta[PITCH] += AngleSubtract(prevAngles[PITCH], prevAngles2[PITCH]);
	delta[ROLL] += AngleSubtract(prevAngles[YAW], prevAngles2[YAW]);
	VectorScale(delta, 0.5f, delta);

	// Damp the changes so it's not as jerky
	const float f = std::abs(1.0f / jkg_viewmodelMomentumDamp.value);
	const float dampRatio = 1.0f / std::pow(f, jkg_viewmodelMomentumInterval.value);
	VectorMA(playAngles, dampRatio, delta, angles);

	// Copy the new value over to the previous
	VectorCopy(prevAngles, prevAngles2);
	VectorCopy(playAngles, prevAngles);
}

/*
==============
CG_CalculateWeaponPosition
==============
*/
/*
static void CG_CalculateWeaponPosition( vec3_t origin, vec3_t angles ) 
{
	float	scale;
	float	swayscale;
	int		delta;
	float	fracsin;
	float	sprintPhase = JKG_CalculateSprintPhase(&cg.predictedPlayerState);
	float	ironSightsPhase = JKG_CalculateIronsightsPhase(&cg.predictedPlayerState, cg.time, &cg.ironsightsBlend);
	weaponData_t *weaponData = GetWeaponData(cg.predictedPlayerState.weapon, cg.predictedPlayerState.weaponVariation);
	weaponData_t *prevWeaponData = BG_GetWeaponDataByIndex(cg.lastFiringModeGun);

	VectorCopy( cg.refdef.vieworg, origin );
	VectorCopy( cg.refdef.viewangles, angles );

	// On odd legs, invert some angles
	if ( cg.bobcycle & 1 ) {
		scale = -cg.xyspeed;
	} else {
		scale = cg.xyspeed;
	}

	// Idle drift (not in ironsights)
	swayscale = cg.xyspeed + 40;
	fracsin = sin( cg.time * 0.001 );
	angles[ROLL] += swayscale * fracsin * 0.01 * (1.0f - ironSightsPhase);
	angles[YAW] += swayscale * fracsin * 0.01 * (1.0f - ironSightsPhase);
	angles[PITCH] += swayscale * fracsin * 0.01 * (1.0f - ironSightsPhase);

	// Drop the weapon when we have a hard landing
	delta = cg.time - cg.landTime;
	if ( delta < LAND_DEFLECT_TIME ) {
		origin[2] += cg.landChange*0.25 * delta / LAND_DEFLECT_TIME;
	} else if ( delta < LAND_DEFLECT_TIME + LAND_RETURN_TIME ) {
		origin[2] += cg.landChange*0.25 *
			(LAND_DEFLECT_TIME + LAND_RETURN_TIME - delta) / LAND_RETURN_TIME;
	}

	// Move the gun around if we've been changing our view around a lot
	if (jkg_viewmodelMomentum.integer) 
	{
		ViewmodelMomentum(angles);
	}

	// Tilt the gun at extreme low/high pitch values
	ViewPitchGunTilt(origin, ironSightsPhase, sprintPhase);

	// Alter angles based on our firing mode
	FireModeTransitionAnimation(origin, angles, prevWeaponData, weaponData);

	// Weapon Bobbing
	// In sprinting, we use special sprinting animations instead of bobbing
	// In iron sights, we don't bob at all
	// In normal situations, we use the JKA weapon bobbing system
	if(sprintPhase > 0)
	{
		JKG_SprintViewmodelAnimation(origin, angles, weaponData, sprintPhase, scale);
	}
	else
	{
		angles[ROLL] += scale * cg.bobfracsin * 0.005 * (1.0f - ironSightsPhase);
		angles[YAW] += scale * cg.bobfracsin * 0.01 * (1.0f - ironSightsPhase);
		angles[PITCH] += cg.xyspeed * cg.bobfracsin * 0.005 * (1.0f - ironSightsPhase);

		VectorMA(origin, jkg_sightsBobX.value * scale * cg.bobfracsin * ironSightsPhase, cg.refdef.viewaxis[0], origin);
		VectorMA(origin, jkg_sightsBobY.value * scale * cg.bobfracsin * ironSightsPhase, cg.refdef.viewaxis[1], origin);
		VectorMA(origin, jkg_sightsBobZ.value * scale * cg.bobfracsin * ironSightsPhase, cg.refdef.viewaxis[2], origin);
	}

}

/*
========================
CG_AddWeaponWithPowerups
========================
*/
/*
void CG_AddWeaponWithPowerups( refEntity_t *gun, int powerups ) 
{
	// add powerup effects
	trap->R_AddRefEntityToScene( gun );

	if (cg.predictedPlayerState.electrifyTime > cg.time)
	{ //add electrocution shell
		int preShader = gun->customShader;
		if ( rand() & 1 )
		{
			gun->customShader = cgs.media.electricBodyShader;
		}
		else
		{
			gun->customShader = cgs.media.electricBody2Shader;
		}
		trap->R_AddRefEntityToScene( gun );
		gun->customShader = preShader; //set back just to be safe
	}
}


/*
=============
CG_AddPlayerWeapon

Used for both the view weapon (ps is valid) and the world modelother character models (ps is NULL)
The main player will have this called for BOTH cases, so effects like light and
sound should only be done on the world model case.
=============
*/

/*
// JKG - Weapon indicators
void WeaponIndicators_Update(const centity_t *cent, const playerState_t *ps);

void CG_AddPlayerWeapon( refEntity_t *parent, playerState_t *ps, centity_t *cent, int team, vec3_t newAngles, qboolean thirdPerson) 
{
	refEntity_t	gun;
	refEntity_t	barrel;
	vec3_t		angles;
	weapon_t	weaponNum;
	weaponInfo_t	*weapon;
	centity_t	*nonPredictedCent;
	refEntity_t	flash;

	weaponNum = (weapon_t)cent->currentState.weapon;

	if (cent->currentState.weapon == WP_EMPLACED_GUN)
	{
		return;
	}

	if (cg.predictedPlayerState.pm_type == PM_SPECTATOR &&
		cent->currentState.number == cg.predictedPlayerState.clientNum)
	{ //spectator mode, don't draw it...
		return;
	}

	// JKG - Update weapon indicators
	WeaponIndicators_Update(cent, ps);

	weapon = CG_WeaponInfo (weaponNum, cent->currentState.weaponVariation);

	memset( &gun, 0, sizeof( gun ) );

	// only do this if we are in first person, since world weapons are now handled on the server by Ghoul2
	if (!thirdPerson)
	{

		// add the weapon
		VectorCopy( parent->lightingOrigin, gun.lightingOrigin );
		gun.shadowPlane = parent->shadowPlane;
		gun.renderfx = parent->renderfx;

		if (ps)
		{	// this player, in first person view
			if ( weapon->g2ViewModel )
			{
				gun.ghoul2 = weapon->g2ViewModel;
				gun.radius = 32.0f;
				if ( !gun.ghoul2 )
				{
					return;
				}
			}
			else
			{
				gun.hModel = weapon->viewModel;
				if ( !gun.hModel )
				{
					return;
				}
			}
		}
		else
		{
			gun.hModel = weapon->weaponModel;

			if (!gun.hModel) {
				return;
			}
		}

		if ( !ps ) {
			// add weapon ready sound
			cent->pe.lightningFiring = qfalse;
			if ( ( cent->currentState.eFlags & EF_FIRING ) && weapon->firingSound ) {
				// lightning gun and guantlet make a different sound when fire is held down
				trap->S_AddLoopingSound( cent->currentState.number, cent->lerpOrigin, vec3_origin, weapon->firingSound );
				cent->pe.lightningFiring = qtrue;
			} else if ( weapon->readySound ) {
				trap->S_AddLoopingSound( cent->currentState.number, cent->lerpOrigin, vec3_origin, weapon->readySound );
			}
		}

		CG_PositionEntityOnTag( &gun, parent, parent->hModel, "tag_weapon");

		if (!CG_IsMindTricked(cent->currentState.trickedentindex,
			cent->currentState.trickedentindex2,
			cent->currentState.trickedentindex3,
			cent->currentState.trickedentindex4,
			cg.snap->ps.clientNum))
		{
			CG_AddWeaponWithPowerups( &gun, cent->currentState.powerups ); //don't draw the weapon if the player is invisible
		}

		if (weaponNum == WP_STUN_BATON)
		{
			int i = 0;

			while (i < 3)
			{
				memset( &barrel, 0, sizeof( barrel ) );
				VectorCopy( parent->lightingOrigin, barrel.lightingOrigin );
				barrel.shadowPlane = parent->shadowPlane;
				barrel.renderfx = parent->renderfx;

				if (i == 0)
				{
					barrel.hModel = cgi_R_RegisterModel("models/weapons2/stun_baton/baton_barrel.md3");
				}
				else if (i == 1)
				{
					barrel.hModel = cgi_R_RegisterModel("models/weapons2/stun_baton/baton_barrel2.md3");
				}
				else
				{
					barrel.hModel = cgi_R_RegisterModel("models/weapons2/stun_baton/baton_barrel3.md3");
				}



				angles[YAW] = 0;
				angles[PITCH] = 0;
				angles[ROLL] = 0;

				AnglesToAxis( angles, barrel.axis );

				if (i == 0)
				{
					CG_PositionRotatedEntityOnTag( &barrel, parent/*&gun*///, /*weapon->weaponModel*/weapon->handsModel, "tag_barrel" );
/*				
				}
				else if (i == 1)
				{
					CG_PositionRotatedEntityOnTag( &barrel, parent/*&gun*///, /*weapon->weaponModel*/weapon->handsModel, "tag_barrel2" );
/*					
				}
				else
				{
					CG_PositionRotatedEntityOnTag( &barrel, parent/*&gun*///, /*weapon->weaponModel*/weapon->handsModel, "tag_barrel3" );
	/*			}
				CG_AddWeaponWithPowerups( &barrel, cent->currentState.powerups );

				i++;
			}
		}
	}

	memset (&flash, 0, sizeof(flash));
	CG_PositionEntityOnTag( &flash, &gun, gun.hModel, "tag_flash");

	VectorCopy(flash.origin, cg.lastFPFlashPoint);

	// Do special charge bits
	//-----------------------
	//Make the guns do their charging visual in True View.
	if ( (ps || cg.renderingThirdPerson || cg.predictedPlayerState.clientNum != cent->currentState.number || cg_trueguns.integer) && 
		cent->currentState.modelindex2 == WEAPON_CHARGING )
	{
		int		shader = 0;
		float	val = 0.0f;
		float	scale = 1.0f;
		addspriteArgStruct_t fxSArgs;
		vec3_t flashorigin, flashdir;

		if (!thirdPerson)
		{
			VectorCopy(flash.origin, flashorigin);
			VectorCopy(flash.axis[0], flashdir);
		}
		else
		{
			mdxaBone_t 		boltMatrix;

			if (!trap->G2API_HasGhoul2ModelOnIndex(&(cent->ghoul2), 1))
			{ //it's quite possible that we may have have no weapon model and be in a valid state, so return here if this is the case
				return;
			}

			// go away and get me the bolt position for this frame please
			if (!(trap->G2API_GetBoltMatrix(cent->ghoul2, 1, 0, &boltMatrix, newAngles, cent->lerpOrigin, cg.time, cgs.gameModels, cent->modelScale)))
			{	// Couldn't find bolt point.
				return;
			}

			BG_GiveMeVectorFromMatrix(&boltMatrix, ORIGIN, flashorigin);
			BG_GiveMeVectorFromMatrix(&boltMatrix, POSITIVE_X, flashdir);
		}

		if ( val < 0.0f )
		{
			val = 0.0f;
		}
		else if ( val > 1.0f )
		{
			val = 1.0f;
			if (ps && cent->currentState.number == ps->clientNum)
			{
				CGCam_Shake( 0.2f, 100 );
			}
		}
		else
		{
			if (ps && cent->currentState.number == ps->clientNum)
			{
				CGCam_Shake( val * val * 0.6f, 100 );
			}
		}

		val += random() * 0.5f;

		VectorCopy(flashorigin, fxSArgs.origin);
		VectorClear(fxSArgs.vel);
		VectorClear(fxSArgs.accel);
		fxSArgs.scale = 3.0f*val*scale;
		fxSArgs.dscale = 0.0f;
		fxSArgs.sAlpha = 0.7f;
		fxSArgs.eAlpha = 0.7f;
		fxSArgs.rotation = random()*360;
		fxSArgs.bounce = 0.0f;
		fxSArgs.life = 1.0f;
		fxSArgs.shader = shader;
		fxSArgs.flags = 0x08000000;

		//FX_AddSprite( flash.origin, NULL, NULL, 3.0f * val, 0.0f, 0.7f, 0.7f, WHITE, WHITE, random() * 360, 0.0f, 1.0f, shader, FX_USE_ALPHA );
		trap->FX_AddSprite(&fxSArgs);
	}

	// make sure we aren't looking at cg.predictedPlayerEntity for LG
	nonPredictedCent = &cg_entities[cent->currentState.clientNum];

	// if the index of the nonPredictedCent is not the same as the clientNum
	// then this is a fake player (like on teh single player podiums), so
	// go ahead and use the cent
	if( ( nonPredictedCent - cg_entities ) != cent->currentState.clientNum ) {
		nonPredictedCent = cent;
	}

	// add the flash
	if ( ( weaponNum == WP_DEMP2)
		&& ( nonPredictedCent->currentState.eFlags & EF_FIRING ) )
	{
		// continuous flash
	} else {
		// impulse flash
		if ( cg.time - cent->muzzleFlashTime > MUZZLE_FLASH_TIME) {
			return;
		}
	}

	if ( ps || cg.renderingThirdPerson || cg_trueguns.integer
		|| cent->currentState.number != cg.predictedPlayerState.clientNum )
	{	// Make sure we don't do the thirdperson model effects for the local player if we're in first person
		vec3_t flashorigin, flashdir;
		refEntity_t	flash;

		memset (&flash, 0, sizeof(flash));

		if (!thirdPerson)
		{
			CG_PositionEntityOnTag( &flash, &gun, gun.hModel, "tag_flash");
			VectorCopy(flash.origin, flashorigin);
			VectorCopy(flash.axis[0], flashdir);
		}
		else
		{
			mdxaBone_t 		boltMatrix;

			if (!trap->G2API_HasGhoul2ModelOnIndex(&(cent->ghoul2), 1))
			{ //it's quite possible that we may have have no weapon model and be in a valid state, so return here if this is the case
				return;
			}

			// go away and get me the bolt position for this frame please
			if (!(trap->G2API_GetBoltMatrix(cent->ghoul2, 1, 0, &boltMatrix, newAngles, cent->lerpOrigin, cg.time, cgs.gameModels, cent->modelScale)))
			{	// Couldn't find bolt point.
				return;
			}

			BG_GiveMeVectorFromMatrix(&boltMatrix, ORIGIN, flashorigin);
			BG_GiveMeVectorFromMatrix(&boltMatrix, POSITIVE_X, flashdir);
		}

		if ( cg.time - cent->muzzleFlashTime <= MUZZLE_FLASH_TIME + 10 )
		{	// Handle muzzle flashes
			if ( cent->currentState.eFlags & EF_ALT_FIRING )
			{
				// Check the alt firing first.
				if (weapon->altMuzzleEffect)
				{
					if (!thirdPerson)
					{
						trap->FX_PlayEntityEffectID(weapon->altMuzzleEffect, flashorigin, flash.axis, -1, -1, -1, -1  );
					}
					else
					{
						trap->FX_PlayEffectID(weapon->altMuzzleEffect, flashorigin, flashdir, -1, -1, false);
					}
				}
			}
			else
			{
				// Regular firing
				if (weapon->muzzleEffect)
				{
					if (!thirdPerson)
					{
						trap->FX_PlayEntityEffectID(weapon->muzzleEffect, flashorigin, flash.axis, -1, -1, -1, -1);
					}
					else
					{
						trap->FX_PlayEffectID(weapon->muzzleEffect, flashorigin, flashdir, -1, -1, false);
					}
				}
			}
		}

		if ( weapon->flashDlightColor[0] || weapon->flashDlightColor[1] || weapon->flashDlightColor[2] ) {
			trap->R_AddLightToScene( flashorigin, 300 + (rand()&31), weapon->flashDlightColor[0],
				weapon->flashDlightColor[1], weapon->flashDlightColor[2] );
		}
	}
}

/*
==============
CG_AddViewWeapon

Add the weapon, and flash for the player's view
==============
*/
/*

void CG_AddViewWeapon( playerState_t *ps ) 
{
	refEntity_t	hand;
	centity_t	*cent;
	clientInfo_t	*ci;
	float		fovOffset;
	vec3_t		angles;
	weaponInfo_t	*weapon;
	float	cgFov;

	if(!cg.renderingThirdPerson && (cg_trueguns.integer || cg.predictedPlayerState.weapon == WP_SABER
	|| cg.predictedPlayerState.weapon == WP_MELEE) && cg_truefov.value
		&& (cg.predictedPlayerState.pm_type != PM_SPECTATOR)
		&& (cg.predictedPlayerState.pm_type != PM_INTERMISSION))
	{
		cgFov = cg_truefov.value;
	}
	else
	{
		cgFov = cg_fov.value;
	}

	if (cgFov < 1)
	{
		cgFov = 1;
	}

	if (cgFov > 97)
	{
		cgFov = 97;
	}

	if ( ps->persistant[PERS_TEAM] == TEAM_SPECTATOR ) {
		return;
	}

	if ( ps->pm_type == PM_INTERMISSION ) {
		return;
	}

	// no gun if in third person view or a camera is active
	if ( cg.renderingThirdPerson ) {
		return;
	}

	// allow the gun to be completely removed
	if ( !cg_drawGun.integer || cg.predictedPlayerState.zoomMode || cg_trueguns.integer
		|| cg.predictedPlayerState.weapon == WP_SABER || cg.predictedPlayerState.weapon == WP_MELEE) {
		vec3_t		origin;

		if ( cg.predictedPlayerState.eFlags & EF_FIRING ) {
			// special hack for lightning gun...
			VectorCopy( cg.refdef.vieworg, origin );
			VectorMA( origin, -8, cg.refdef.viewaxis[2], origin );
		}
		return;
	}

	// don't draw if testing a gun model
	if ( cg.testGun ) {
		return;
	}

	// drop gun lower at higher fov
	if ( cgFov > 90 ) {
		fovOffset = -0.2 * ( cgFov - 90 );
	} else {
		fovOffset = 0;
	}

	cent = &cg_entities[cg.predictedPlayerState.clientNum];
	weapon = CG_WeaponInfo (ps->weapon, ps->weaponVariation);

	memset (&hand, 0, sizeof(hand));

	// set up gun position
	CG_CalculateWeaponPosition( hand.origin, angles );
	{
		float xoffs = cg_gunX.value, yoffs = cg_gunX.value, zoffs = cg_gunZ.value;
		VectorMA( hand.origin, xoffs, cg.refdef.viewaxis[0], hand.origin );
		VectorMA( hand.origin, yoffs, cg.refdef.viewaxis[1], hand.origin );
		VectorMA( hand.origin, (zoffs+fovOffset), cg.refdef.viewaxis[2], hand.origin );
	}
	AnglesToAxis( angles, hand.axis );

	// map torso animations to weapon animations
	if ( cg_debugGun.integer ) {
		// development tool
		hand.frame = hand.oldframe = cg_debugGun.integer;
		hand.backlerp = 0;
	} else {
		float currentFrame;
		// get clientinfo for animation map
		if (cent->currentState.eType == ET_NPC)
		{
			if (!cent->npcClient)
			{
				return;
			}

			ci = cent->npcClient;
		}
		else
		{
			ci = &cgs.clientinfo[ cent->currentState.clientNum ];
		}
		trap->G2API_GetBoneFrame(cent->ghoul2, "lower_lumbar", cg.time, &currentFrame, cgs.gameModels, 0);
		hand.frame = CG_MapTorsoToWeaponFrame( ci, ceil( currentFrame ), ps->torsoAnim );
		hand.oldframe = CG_MapTorsoToWeaponFrame( ci, floor( currentFrame ), ps->torsoAnim );
		hand.backlerp = 1.0f - (currentFrame-floor(currentFrame));

		// Handle the fringe situation where oldframe is invalid
		if ( hand.frame == -1 )
		{
			hand.frame = 0;
			hand.oldframe = 0;
			hand.backlerp = 0;
		}
		else if ( hand.oldframe == -1 )
		{
			hand.oldframe = hand.frame;
			hand.backlerp = 0;
		}
	}

	hand.hModel = weapon->handsModel;
	hand.renderfx = RF_DEPTHHACK | RF_FIRST_PERSON;// | RF_MINLIGHT;

	// add everything onto the hand
	CG_AddPlayerWeapon( &hand, ps, &cg_entities[cg.predictedPlayerState.clientNum], ps->persistant[PERS_TEAM], angles, qfalse );
}

/*
==============================================================================

WEAPON SELECTION

==============================================================================
*/

/*
===============
CG_WeaponSelectable
===============
*/
/*

static qboolean CG_WeaponSelectable( int i )
{
	if (i < 0)
	{
		return qfalse;
	}

	if(cg.playerACI[i] < 0)
		return qfalse;

	if( (*cg.playerInventory)[cg.playerACI[i]].id->weaponData.weapon == WP_NONE)
		return qfalse;

	return qtrue;
}

/*
===============
JKG_NextWeapon_f
===============
*//*
void JKG_NextWeapon_f( void )
{
	qboolean doWeaponNotify = qtrue;

	int desiredWeaponSelect = cg.weaponSelect + 1;
	int numIterations = 0;

	if(!cg.snap)
	{
		return;
	}

	if ( cg.snap->ps.pm_flags & PMF_FOLLOW ) {
		return;
	}

	if (cg.predictedPlayerState.pm_type == PM_SPECTATOR)
	{
		return;
	}

	// Don't let us switch weapons while we're sprinting.
	int current = trap->GetCurrentCmdNumber();
	usercmd_t ucmd;
	trap->GetUserCmd(current, &ucmd);
	if (BG_IsSprinting(&cg.predictedPlayerState, &ucmd, false))
	{
		return;
	}

	while(numIterations < MAX_ACI_SLOTS)
	{
		if(desiredWeaponSelect >= MAX_ACI_SLOTS)
		{
			desiredWeaponSelect = 0;
		}
		if(cg.playerACI[desiredWeaponSelect] < 0)
		{
			desiredWeaponSelect++;
			numIterations++;
			continue;
		}
		if(!(*cg.playerInventory)[cg.playerACI[desiredWeaponSelect]].id)
		{
			desiredWeaponSelect++;
		}
		else if((*cg.playerInventory)[cg.playerACI[desiredWeaponSelect]].id->itemType != ITEM_WEAPON)
		{
			desiredWeaponSelect++;
		}
		else
		{
			break;
		}
		numIterations++;
	}
	if(numIterations >= MAX_ACI_SLOTS)
	{
		return;	// FIXME: lame solution
	}

	if( desiredWeaponSelect == cg.weaponSelect )
	{
		doWeaponNotify = qfalse;
	}

	if(CG_WeaponSelectable(desiredWeaponSelect))
	{
		cg.weaponSelect = desiredWeaponSelect;

		if( doWeaponNotify )
		{
			CG_Notifications_Add((*cg.playerInventory)[cg.playerACI[cg.weaponSelect]].id->displayName, qtrue);
		}
	}
	else
	{
		return;
	}
	cg.weaponSelectTime = cg.time;
}

/*
===============
JKG_PrevWeapon_f
===============
*//*
void JKG_PrevWeapon_f( void )
{
	qboolean doWeaponNotify = qtrue;

	int desiredWeaponSelect = cg.weaponSelect - 1;
	int numIterations = 0;

	if(!cg.snap)
	{
		return;
	}

	if ( cg.snap->ps.pm_flags & PMF_FOLLOW ) {
		return;
	}

	if (cg.predictedPlayerState.pm_type == PM_SPECTATOR)
	{
		return;
	}

	// Don't allow us to switch weapons while we're sprinting.
	int current = trap->GetCurrentCmdNumber();
	usercmd_t ucmd;
	trap->GetUserCmd(current, &ucmd);
	if (BG_IsSprinting(&cg.predictedPlayerState, &ucmd, qfalse))
	{
		return;
	}

	while(numIterations < MAX_ACI_SLOTS)
	{
		if(desiredWeaponSelect < 0)
		{
			desiredWeaponSelect = MAX_ACI_SLOTS-1;
		}
		if(cg.playerACI[desiredWeaponSelect] < 0)
		{
			desiredWeaponSelect--;
			numIterations++;
			continue;
		}
		if(!(*cg.playerInventory)[cg.playerACI[desiredWeaponSelect]].id)
		{
			desiredWeaponSelect--;
		}
		else if((*cg.playerInventory)[cg.playerACI[desiredWeaponSelect]].id->itemType != ITEM_WEAPON)
		{
			desiredWeaponSelect--;
		}
		else
		{
			break;
		}
		numIterations++;
	}

	if( desiredWeaponSelect == cg.weaponSelect )
	{
		doWeaponNotify = qfalse;
	}

	if(CG_WeaponSelectable(desiredWeaponSelect))
	{
		cg.weaponSelect = desiredWeaponSelect;
		if( doWeaponNotify )
		{
			CG_Notifications_Add((*cg.playerInventory)[cg.playerACI[cg.weaponSelect]].id->displayName, qtrue);
		}
	}
	else
	{
		return;
	}
	cg.weaponSelectTime = cg.time;
}


/*
===============
JKG_FindNewACISlot
Automatically fills an ACI slot with an item with the same ID number (if one exists)
===============
*//*
int JKG_FindNewACISlot( int slotNum )
{
	int i;
	int slotID;
	if(slotNum < 0 || slotNum > MAX_ACI_SLOTS)
	{
		return -1;
	}
	slotID = (*cg.playerInventory)[cg.playerACI[slotNum]].id->itemID;
	for(i = 0; (*cg.playerInventory)[i].id && (*cg.playerInventory)[i].id->itemID > 0; i++)
	{
		if((*cg.playerInventory)[i].id->itemID == slotID && i != cg.playerACI[slotNum])
		{
			return i;
		}
	}
	return -1;
}
/*
===============
JKG_RemoveACIItemsOfType
Removes any ACI items that match a specific type
===============
*/

/*
void JKG_RemoveACIItemsOfType(jkgItemType_t itemType) {
	for (int i = 0; i < MAX_ACI_SLOTS; i++) {
		itemData_t* itemInThisSlot;
		if (cg.playerACI[i] >= cg.playerInventory->size() || cg.playerACI[i] < 0) {
			cg.playerACI[i] = -1;
			continue;
		}

		itemInThisSlot = (*cg.playerInventory)[cg.playerACI[i]].id;
		if (itemInThisSlot->itemType == itemType) {
			cg.playerACI[i] = -1;
			break;
		}
	}
}

/*
===============
JKG_CG_FillACISlot
Assign an item to an ACI slot
===============
*/
/*
void JKG_CG_FillACISlot(int itemNum, int slot)
{
	itemData_t *item;
	qboolean alreadyInACI = qfalse;

	if (itemNum < 0 || itemNum >= cg.playerInventory->size())
	{
		return;
	}

	if (slot >= MAX_ACI_SLOTS)
	{
		return;
	}

	// Find out if we have the item already in our ACI
	for (int i = 0; i < MAX_ACI_SLOTS; i++) {
		if (cg.playerACI[i] == itemNum) {
			cg.playerACI[i] = -1;
			alreadyInACI = qtrue;
		}
	}

	item = (*cg.playerInventory)[itemNum].id;

	// If the item is a shield, then we need to remove all other shields in our ACI and inform the server that we equipped shield
	// Unless of course we already had this item in the ACI, in which case we do nothing
	if (item->itemType == ITEM_SHIELD && !alreadyInACI) {
		JKG_RemoveACIItemsOfType(ITEM_SHIELD);
		trap->SendClientCommand(va("equipShield %i", itemNum));
	}
	else if (item->itemType == ITEM_JETPACK && !alreadyInACI) {
		JKG_RemoveACIItemsOfType(ITEM_JETPACK);
		trap->SendClientCommand(va("equipJetpack %i", itemNum));
	}
	else if (cg.playerACI[slot] > 0 && cg.playerACI[slot] < cg.playerInventory->size()) {
		// Check to see if we're overwriting a shield/jetpack (if so, unequip it)
		itemData_t* itemInThisSlot;
		itemInThisSlot = (*cg.playerInventory)[cg.playerACI[slot]].id;
		if (itemInThisSlot->itemType == ITEM_SHIELD) {
			trap->SendClientCommand("unequipShield");
		}
		else if (itemInThisSlot->itemType == ITEM_JETPACK) {
			trap->SendClientCommand("unequipJetpack");
		}
	}

	// A slot of -1 means that we pick the first available one
	if (slot == -1) {
		for (int i = 0; i < MAX_ACI_SLOTS; i++) {
			if (cg.playerACI[i] == -1) {
				slot = i;
				break;
			}
		}
	}

	if (slot == -1) {
		trap->Print("Couldn't assign %i to ACI, ACI is full\n", itemNum);
		return;
	}


	cg.playerACI[slot] = itemNum;
}

/*
 *	JKG_CG_ACIPostFix
 *	Adjusts the ACI after an item stack has been removed
 *//*
void JKG_CG_ACIPostFix(int itemSlot)
{
	for (int i = 0; i < MAX_ACI_SLOTS; i++)
	{
		if (cg.playerACI[i] == itemSlot)
		{
			cg.playerACI[i] = -1;
		}
		else if (cg.playerACI[i] > itemSlot)
		{
			cg.playerACI[i]--;
		}
	}
}

void JKG_CG_ClearACISlot(int slot)
{
	if (slot < 0 || slot >= MAX_ACI_SLOTS)
	{
		return;
	}

	if (cg.playerACI[slot] > 0 && cg.playerACI[slot] < cg.playerInventory->size()) {
		itemInstance_t* item = &(*cg.playerInventory)[cg.playerACI[slot]];
		if (item->id->itemType == ITEM_SHIELD) {
			// inform the server that we've unequipped our shield
			trap->SendClientCommand("unequipShield");
		}
	}

	cg.playerACI[slot] = -1;
}

void JKG_CG_ACICheckRemoval(int itemNumber)
{
	int i;
	for (i = 0; i < MAX_ACI_SLOTS; i++)
	{
		if (cg.playerACI[i] < 0)
		{
			continue;
		}
		else if (cg.playerACI[i] > itemNumber)
		{
			cg.playerACI[i]--;
		}
	}
}
*/
/*
void CG_EquipItem(int newItem, int oldItem)
{
	(*cg.playerInventory)[newItem].equipped = qtrue;
	if (oldItem != -1)
	{

(*cg.playerInventory)[oldItem].equipped = qfalse;
	}
}

void CG_UnequipItem(int inventorySlot)
{
	(*cg.playerInventory)[inventorySlot].equipped = qfalse;
}

/*
===============
CG_Weapon_f
===============
*/
/*
void CG_Weapon_f( void ) 
{
	int		num;
	int itemType;
	qboolean doWeaponNotify = qtrue;

	if ( !cg.snap ) {
		return;
	}
	if ( cg.snap->ps.pm_flags & PMF_FOLLOW ) {
		return;
	}

	if (cg.snap->ps.emplacedIndex)
	{
		return;
	}

	// sprint check --eez
	int current = trap->GetCurrentCmdNumber();
	usercmd_t ucmd;
	trap->GetUserCmd(current, &ucmd);
	if (BG_IsSprinting(&cg.predictedPlayerState, &ucmd, qfalse))
	{
		return;
	}

	num = atoi( CG_Argv( 1 ) );
	if(num < 0 || num > 9)
	{
		return;
	}

	if(cg.playerACI[num] < 0)
	{
		return;
	}
	if(!(*cg.playerInventory)[cg.playerACI[num]].id)
	{
		return;
	}
	itemType = (*cg.playerInventory)[cg.playerACI[num]].id->itemType;

	if (itemType != ITEM_WEAPON)
	{
		// Not a weapon. Check the type!
		switch(itemType)
		{
			case ITEM_CONSUMABLE:
				{
					trap->SendConsoleCommand(va("inventoryUse %i", cg.playerACI[num]));
				}
				break;
		}
		return;
	}

	// JKG - Manual saber detection
	if(num == cg.weaponSelect)
	{
		if((*cg.playerInventory)[cg.playerACI[num]].id->weaponData.varID ==
			BG_GetWeaponIndex(WP_SABER, (*cg.playerInventory)[cg.playerACI[num]].id->weaponData.variation))
		{
			trap->SendClientCommand("togglesaber");
		}
		// Set our holster state
		cg.holsterState = (cg.holsterState) ? qfalse : qtrue;
		doWeaponNotify = qfalse;
	}

	// FIXME: TEMPORARY
	if(CG_WeaponSelectable(num))
	{
		cg.weaponSelect = num;
		if(doWeaponNotify)
		{
			CG_Notifications_Add((*cg.playerInventory)[cg.playerACI[num]].id->displayName, qtrue);
		}
	}
	else
	{
		return;
	}
	cg.weaponSelectTime = cg.time;
}

/*
===================================================================================================

WEAPON EVENTS

===================================================================================================
*/
/*

void CG_GetClientWeaponMuzzleBoltPoint(int clIndex, vec3_t to)
{
	centity_t *cent;
	mdxaBone_t	boltMatrix;

	if (clIndex < 0 || clIndex >= MAX_CLIENTS)
	{
		return;
	}

	cent = &cg_entities[clIndex];

	if (!cent || !cent->ghoul2 || !trap->G2_HaveWeGhoul2Models(cent->ghoul2) ||
		!trap->G2API_HasGhoul2ModelOnIndex(&(cent->ghoul2), 1))
	{
		return;
	}

	trap->G2API_GetBoltMatrix(cent->ghoul2, 1, 0, &boltMatrix, cent->turAngles, cent->lerpOrigin, cg.time, cgs.gameModels, cent->modelScale);
	BG_GiveMeVectorFromMatrix(&boltMatrix, ORIGIN, to);
}

/*
============================================================================

BULLETS

============================================================================
*/


/*
======================
CG_CalcMuzzlePoint
======================
*/

/*
qboolean CG_CalcMuzzlePoint( int entityNum, vec3_t muzzle ) {
	vec3_t		forward, right;
	vec3_t		gunpoint;
	centity_t	*cent;
	int			anim;

	if ( entityNum == cg.snap->ps.clientNum )
	{ //I'm not exactly sure why we'd be rendering someone else's crosshair, but hey.
		int weapontype = cg.snap->ps.weapon;
		vec3_t weaponMuzzle;
		centity_t *pEnt = &cg_entities[cg.predictedPlayerState.clientNum];

		VectorClear( weaponMuzzle );

		if (cg.renderingThirdPerson)
		{
			VectorCopy( pEnt->lerpOrigin, gunpoint );
			AngleVectors( pEnt->lerpAngles, forward, right, NULL );
		}
		else
		{
			VectorCopy( cg.refdef.vieworg, gunpoint );
			AngleVectors( cg.refdef.viewangles, forward, right, NULL );
		}

		if (weapontype == WP_EMPLACED_GUN && cg.snap->ps.emplacedIndex)
		{
			centity_t *gunEnt = &cg_entities[cg.snap->ps.emplacedIndex];

			if (gunEnt)
			{
				vec3_t pitchConstraint;

				VectorCopy(gunEnt->lerpOrigin, gunpoint);
				gunpoint[2] += 46;

				if (cg.renderingThirdPerson)
				{
					VectorCopy(pEnt->lerpAngles, pitchConstraint);
				}
				else
				{
					VectorCopy(cg.refdef.viewangles, pitchConstraint);
				}

				if (pitchConstraint[PITCH] > 40)
				{
					pitchConstraint[PITCH] = 40;
				}
				AngleVectors( pitchConstraint, forward, right, NULL );
			}
		}

		VectorCopy(gunpoint, muzzle);

		VectorMA(muzzle, weaponMuzzle[0], forward, muzzle);
		VectorMA(muzzle, weaponMuzzle[1], right, muzzle);

		if (weapontype == WP_EMPLACED_GUN && cg.snap->ps.emplacedIndex)
		{
			//Do nothing
		}
		else if (cg.renderingThirdPerson)
		{
			muzzle[2] += cg.snap->ps.viewheight + weaponMuzzle[2];
		}
		else
		{
			muzzle[2] += weaponMuzzle[2];
		}

		return qtrue;
	}

	cent = &cg_entities[entityNum];
	if ( !cent->currentValid ) {
		return qfalse;
	}

	VectorCopy( cent->currentState.pos.trBase, muzzle );

	AngleVectors( cent->currentState.apos.trBase, forward, NULL, NULL );
	anim = cent->currentState.legsAnim;
	if ( anim == BOTH_CROUCH1WALK || anim == BOTH_CROUCH1IDLE ) {
		muzzle[2] += CROUCH_VIEWHEIGHT;
	} else {
		muzzle[2] += DEFAULT_VIEWHEIGHT;
	}

	VectorMA( muzzle, 14, forward, muzzle );

	return qtrue;

}

// clean out any g2 models we instanciated for copying purposes
void CG_ShutDownG2Weapons(void)
{
	int i, j;
	weaponInfo_t *weapon;
	weaponData_t *weaponData;

	for ( i = 0; i < MAX_WEAPON_TABLE_SIZE; i++ )
	{
		trap->G2API_CleanGhoul2Models (&g2WeaponInstances[i].ghoul2);

		weapon = CG_WeaponInfoUnsafe (g2WeaponInstances[i].weaponNum, g2WeaponInstances[i].weaponVariation);
		weaponData = GetWeaponData( g2WeaponInstances[i].weaponNum, g2WeaponInstances[i].weaponVariation );
		if ( weapon != NULL )
		{
			trap->G2API_CleanGhoul2Models (&weapon->g2WorldModel);
			trap->G2API_CleanGhoul2Models (&weapon->g2ViewModel);
			for(j = 0; j < weaponData->numFiringModes; j++)
			{
				trap->G2API_CleanGhoul2Models (&weapon->drawData[j].explosiveRender.tripmine.g2Model);
				trap->G2API_CleanGhoul2Models (&weapon->drawData[j].explosiveRender.detpack.g2Model);
			}
		}
	}
	BG_ShutdownWeaponG2Instances();
}

void *CG_G2WeaponInstance(centity_t *cent, int weapon, int variation)
{
	clientInfo_t *ci = NULL;

	if (weapon != WP_SABER)
	{
		return BG_GetWeaponGhoul2 (weapon, variation);
	}

	if (cent->currentState.eType != ET_PLAYER &&
		cent->currentState.eType != ET_NPC)
	{
		return BG_GetWeaponGhoul2 (weapon, variation);
	}

	if (cent->currentState.eType == ET_NPC)
	{
		ci = cent->npcClient;
	}
	else
	{
		ci = &cgs.clientinfo[cent->currentState.number];
	}

	if (!ci)
	{
		return BG_GetWeaponGhoul2 (weapon, variation);
	}

	//If no custom then just use the default.
	return BG_GetWeaponGhoul2 (weapon, variation);
}

// what ghoul2 model do we want to copy ?
void CG_CopyG2WeaponInstance(centity_t *cent, int weaponNum, int weaponVariation, void *toGhoul2)
{
	//rww - the -1 is because there is no "weapon" for WP_NONE
	assert(weaponNum < MAX_WEAPONS);
	if (CG_G2WeaponInstance(cent, weaponNum,//-1
     weaponVariation))
	{
		if (weaponNum == WP_SABER)
		{
			clientInfo_t *ci = NULL;

			if (cent->currentState.eType == ET_NPC)
			{
				ci = cent->npcClient;
			}
			else
			{
				ci = &cgs.clientinfo[cent->currentState.number];
			}

			if (!ci)
			{
				trap->G2API_CopySpecificGhoul2Model(CG_G2WeaponInstance(cent, weaponNum/*-1*///, weaponVariation), 0, toGhoul2, 1);
	/*		}
			else
			{ //Try both the left hand saber and the right hand saber
				int i = 0;

				while (i < MAX_SABERS)
				{
					if (ci->saber[i].model[0] &&
						ci->ghoul2Weapons[i])
					{
						trap->G2API_CopySpecificGhoul2Model(ci->ghoul2Weapons[i], 0, toGhoul2, i+1);
					}
					else if (ci->ghoul2Weapons[i])
					{ //if the second saber has been removed, then be sure to remove it and free the instance.
						qboolean g2HasSecondSaber = trap->G2API_HasGhoul2ModelOnIndex(&(toGhoul2), 2);

						if (g2HasSecondSaber)
						{ //remove it now since we're switching away from sabers
							trap->G2API_RemoveGhoul2Model(&(toGhoul2), 2);
						}
						trap->G2API_CleanGhoul2Models(&ci->ghoul2Weapons[i]);
					}

					i++;
				}
			}
		}
		else
		{
			qboolean g2HasSecondSaber = trap->G2API_HasGhoul2ModelOnIndex(&(toGhoul2), 2);

			if (g2HasSecondSaber)
			{ //remove it now since we're switching away from sabers
				trap->G2API_RemoveGhoul2Model(&(toGhoul2), 2);
			}

			if (weaponNum == WP_EMPLACED_GUN)
			{ //a bit of a hack to remove gun model when using an emplaced weap
				if (trap->G2API_HasGhoul2ModelOnIndex(&(toGhoul2), 1))
				{
					trap->G2API_RemoveGhoul2Model(&(toGhoul2), 1);
				}
			}
			else if (weaponNum == WP_MELEE)
			{ //don't want a weapon on the model for this one
				if (trap->G2API_HasGhoul2ModelOnIndex(&(toGhoul2), 1))
				{
					trap->G2API_RemoveGhoul2Model(&(toGhoul2), 1);
				}
			}
			else
			{
				trap->G2API_CopySpecificGhoul2Model(CG_G2WeaponInstance(cent, weaponNum/*-1*///, weaponVariation), 0, toGhoul2, 1);
	/*	    }
		}
	}
	else
	{
		// Remove any weapons if nothing is returned.
		int i;
		for ( i = 0; i < MAX_SABERS; i++ )
		{
			if ( trap->G2API_HasGhoul2ModelOnIndex(&toGhoul2, i + 1) )
			{
				trap->G2API_RemoveGhoul2Model (&toGhoul2, i + 1);
			}
		}
	}
}

void CG_CheckPlayerG2Weapons(playerState_t *ps, centity_t *cent)
{
	if (!ps)
	{
		assert(0);
		return;
	}

	if (ps->pm_flags & PMF_FOLLOW)
	{
		return;
	}

	if (cent->currentState.eType == ET_NPC)
	{
		assert(0);
		return;
	}

	// should we change the gun model on this player?
	if (cent->currentState.saberInFlight)
	{
		cent->ghoul2weapon = CG_G2WeaponInstance(cent, WP_SABER, 0);
	}

	if (cent->currentState.eFlags & EF_DEAD)
	{ //no updating weapons when dead
		cent->ghoul2weapon = NULL;
		return;
	}

	if (cent->torsoBolt)
	{ //got our limb cut off, no updating weapons until it's restored
		cent->ghoul2weapon = NULL;
		return;
	}

	if (cgs.clientinfo[ps->clientNum].team == TEAM_SPECTATOR ||
		ps->persistant[PERS_TEAM] == TEAM_SPECTATOR)
	{
		cent->ghoul2weapon = cg_entities[ps->clientNum].ghoul2weapon = NULL;
		cent->weapon = cg_entities[ps->clientNum].weapon = 0;
		return;
	}

	if (cent->ghoul2 && cent->ghoul2weapon != CG_G2WeaponInstance(cent, ps->weapon, ps->weaponVariation) &&
		ps->clientNum == cent->currentState.number) //don't want spectator mode forcing one client's weapon instance over another's
	{
		CG_CopyG2WeaponInstance(cent, ps->weapon, ps->weaponVariation, cent->ghoul2);
		cent->ghoul2weapon = CG_G2WeaponInstance(cent, ps->weapon, ps->weaponVariation);
		if( cent->weapon == WP_SABER && ps->weapon == WP_SABER &&
			cent->currentState.weaponVariation != ps->weaponVariation &&
			ps->saberHolstered != 2)
		{
			// We are switching to another saber from our current one
			if (cgs.clientinfo[ps->clientNum].saber[0].soundOn)
			{
				trap->S_StartSound(cent->lerpOrigin, cent->currentState.number, CHAN_AUTO, cgs.clientinfo[ps->clientNum].saber[0].soundOn);
			}

			if (cgs.clientinfo[ps->clientNum].saber[1].soundOn)
			{
				trap->S_StartSound(cent->lerpOrigin, cent->currentState.number, CHAN_AUTO, cgs.clientinfo[ps->clientNum].saber[1].soundOn);
			}
		}
		else if (ps->weapon == WP_SABER && cent->weapon != ps->weapon && !cent->saberWasInFlight && ps->saberHolstered != 2)
		{ //switching to the saber
			//trap->S_StartSound(cent->lerpOrigin, cent->currentState.number, CHAN_AUTO, cgi_S_RegisterSound( "sound/weapons/saber/saberon.wav" ));
			if (cgs.clientinfo[ps->clientNum].saber[0].soundOn)
			{
				trap->S_StartSound(cent->lerpOrigin, cent->currentState.number, CHAN_AUTO, cgs.clientinfo[ps->clientNum].saber[0].soundOn);
			}

			if (cgs.clientinfo[ps->clientNum].saber[1].soundOn)
			{
				trap->S_StartSound(cent->lerpOrigin, cent->currentState.number, CHAN_AUTO, cgs.clientinfo[ps->clientNum].saber[1].soundOn);
			}

			cgs.clientinfo[ps->clientNum].saber[0].SetDesiredLength(0, -1);
			cgs.clientinfo[ps->clientNum].saber[0].SetDesiredLength(0, -1);
		}
		cent->weapon = ps->weapon;
	}
}
*/
GWInfo_t *CG_WeaponInfo ( unsigned int weaponNum, unsigned int variation )
{
	static GWInfo_t *lastInfo = NULL;

	if ( lastInfo != NULL && lastInfo->weaponNum == weaponNum && lastInfo->variation == variation )
	{
		return lastInfo;
	}
	else
	{
		GWInfo_t *info = &GW_weapons[0];
		unsigned int i;

	/*	RegisterWeapon (weaponNum, variation);

		for ( i = 0; i < MAX_WEAPON_TABLE_SIZE; i++, info++ )
		{
			if ( info->weaponNum == weaponNum && info->variation == variation )
			{
				lastInfo = info;
				return info;
			}
		}*/
	}

	return CG_WeaponInfo (0, 0);
}

GWInfo_t *CG_WeaponInfoUnsafe ( unsigned int weaponNum, unsigned int variation )
{
	static GWInfo_t *lastInfo = NULL;

	if ( lastInfo != NULL && lastInfo->weaponNum == weaponNum && lastInfo->variation == variation )
	{
		return lastInfo;
	}
	else
	{
		GWInfo_t *info = &GW_weapons[0];
		unsigned int i;
		for ( i = 0; i < MAX_WEAPON_TABLE_SIZE; i++, info++ )
		{
			if ( info->weaponNum == weaponNum && info->variation == variation )
			{
				lastInfo = info;
				return lastInfo;
			}
		}
	}

	return NULL;
}

GWInfo_t *NextFreeWeaponInfo ( void )
{
	GWInfo_t *info = &GW_weapons[0];
	unsigned int i;
	for ( i = 0; i < MAX_WEAPON_TABLE_SIZE; i++, info++ )
	{
		if ( !info->registered )
		{
			return info;
		}
	}

	return NULL;
}



/*

//=========================================================
// Weapon event handling functions
//=========================================================
static void JKG_FireBlaster ( centity_t *cent, const weaponDrawData_t *weaponData, unsigned char firingMode )
{
	const entityState_t *s = &cent->currentState;
	const weaponData_t *thisWeaponData = GetWeaponData (cg.snap->ps.weapon, cg.snap->ps.weaponVariation);
	vec3_t viewangles;
	int current = trap->GetCurrentCmdNumber();
	usercmd_t ucmd;
	const ammo_t* thisAmmo = &ammoTable[cent->currentState.ammoType];
	trap->GetUserCmd(current, &ucmd);

	trap->JKG_GetViewAngles(viewangles);

	int channel = CHAN_AUTO;
	if (thisWeaponData->firemodes[firingMode].chargeTime)
	{
		channel = CHAN_WEAPON;
	}

	// Update the muzzle flash time, so we know to draw it in the render function.
	if ( (cent->shotCount + 1) == UINT_MAX )
	{
		cent->shotCount = 0;
		cent->shotCountOverflowed = qtrue;
	}
	else
	{
		cent->shotCount++;
		cent->shotCountOverflowed = qfalse;
	}

	// Do recoil
	if ( s->number == cg.snap->ps.clientNum )
	{
		double pitchRecoil = thisWeaponData->firemodes[cg.snap->ps.firingMode].recoil;
		if (ucmd.upmove < 0) {
			pitchRecoil /= 2.0f;	// Half recoil when we are crouched
		}

		if ( pitchRecoil )
		{
			double yawRecoil = flrand (0.15 * pitchRecoil, 0.25 * pitchRecoil);

			if ( Q_irand (0, 1) )
			{
				yawRecoil = -yawRecoil;
			}

			JKG_ApplyAmmoOverride(pitchRecoil, thisAmmo->overrides.recoil);
			JKG_ApplyAmmoOverride(yawRecoil, thisAmmo->overrides.recoil);

			CGCam_Shake (flrand (0.85f * pitchRecoil, 0.15f * pitchRecoil), 100);

			viewangles[YAW] += yawRecoil;
			viewangles[PITCH] -= pitchRecoil;
		}
	}

	if ( JKG_SimpleAmmoOverridePresent(thisAmmo->visualOverrides.fireSound) )
	{
		int max = thisAmmo->visualOverrides.fireSound.second.size() - 1;
		int index = Q_irand(0, max);

		trap->S_StartSound(NULL, s->number, channel,
			cgi_S_RegisterSound(thisAmmo->visualOverrides.fireSound.second[index].c_str()));
	}
	else if ( weaponData->weaponFire.generic.fireSoundCount > 0 )
	{
		int index = Q_irand (0, weaponData->weaponFire.generic.fireSoundCount - 1);

		trap->S_StartSound (NULL, s->number, channel, weaponData->weaponFire.generic.fireSound[index]);
	}

	trap->JKG_SetViewAngles(viewangles);
}

static void JKG_RenderGenericProjectile ( const centity_t *cent, const weaponDrawData_t *weaponData, const ammo_t* ammoData )
{
	const entityState_t *s = &cent->currentState;
	vec3_t forward, velocity;

	if (VectorNormalize2(s->pos.trDelta, forward) == 0.0f)
	{
		forward[2] = 1.0f;
	}

	if (cent->currentState.weapon == WP_THERMAL ||
		cent->currentState.weapon == WP_TRIP_MINE ||
		cent->currentState.weapon == WP_DET_PACK) {
		// Weapons which don't have much velocity (ie grenades) don't need the doppler effect.
		// Using the doppler effect on certain variations (lemons) can cause the game to crash
		VectorCopy(vec3_origin, velocity);
	}
	else {
		BG_EvaluateTrajectory(&s->pos, cg.time, velocity);
	}

	if ( JKG_SimpleAmmoOverridePresent(ammoData->visualOverrides.projectile.projectileEffect) )
	{
		trap->FX_PlayEffectID(theFxScheduler.RegisterEffect(ammoData->visualOverrides.projectile.projectileEffect.second.c_str()),
			(float*)cent->lerpOrigin,
			forward,
			-1, -1,
			false);
	}
	else if ( weaponData->projectileRender.generic.projectileEffect )
	{

		trap->FX_PlayEffectID (
			weaponData->projectileRender.generic.projectileEffect,
			(float *)cent->lerpOrigin,
			forward,
			-1, -1,
			false
		);
	}

	double lightIntensity = weaponData->projectileRender.generic.lightIntensity;
	JKG_ApplyAmmoOverride(lightIntensity, ammoData->visualOverrides.projectile.lightIntensity);

	if ( lightIntensity > 0.0f )
	{
		if (JKG_SimpleAmmoOverridePresent(ammoData->visualOverrides.projectile.lightColor))
		{
			trap->R_AddLightToScene(
				cent->lerpOrigin,
				lightIntensity,
				ammoData->visualOverrides.projectile.lightColor.second[0],
				ammoData->visualOverrides.projectile.lightColor.second[1],
				ammoData->visualOverrides.projectile.lightColor.second[2]
			);
		}
		else
		{
			trap->R_AddLightToScene(
				cent->lerpOrigin,
				lightIntensity,
				weaponData->projectileRender.generic.lightColor[0],
				weaponData->projectileRender.generic.lightColor[1],
				weaponData->projectileRender.generic.lightColor[2]
			);
		}
		
	}

	if ( JKG_SimpleAmmoOverridePresent(ammoData->visualOverrides.projectile.runSound) )
	{
		trap->S_AddLoopingSound(s->number, cent->lerpOrigin, velocity,
			cgi_S_RegisterSound(ammoData->visualOverrides.projectile.runSound.second.c_str()));
	}
	else if ( weaponData->projectileRender.generic.runSound )
	{
		trap->S_AddLoopingSound (s->number, cent->lerpOrigin, velocity, weaponData->projectileRender.generic.runSound);
	}

	qhandle_t model = weaponData->projectileRender.generic.projectileModel;

	if ( JKG_SimpleAmmoOverridePresent(ammoData->visualOverrides.projectile.projectileModel) )
	{
		model = cgi_R_RegisterModel(ammoData->visualOverrides.projectile.projectileModel.second.c_str());
	}

	if ( model )
	{
		refEntity_t ent;

		memset (&ent, 0, sizeof (ent));
		VectorCopy (cent->lerpOrigin, ent.origin);
		VectorCopy (cent->lerpOrigin, ent.oldorigin);

		CG_SetGhoul2Info (&ent, cent);

		ent.skinNum = cg.clientFrame & 1;
		ent.renderfx = RF_NOSHADOW;

		ent.hModel = weaponData->projectileRender.generic.projectileModel;

		if ( s->apos.trType != TR_INTERPOLATE )
		{
			if ( VectorNormalize2 (s->pos.trDelta, ent.axis[0]) == 0.0f )
			{
				ent.axis[0][2] = 1.0f;
			}

			if ( s->pos.trType != TR_STATIONARY )
			{
				RotateAroundDirection (ent.axis, cg.time * ((s->eFlags & EF_MISSILE_STICK) ? 0.5f : 0.25f));
			}
			else
			{
				RotateAroundDirection (ent.axis, ((s->eFlags & EF_MISSILE_STICK) ? s->pos.trTime * 0.5f : s->time * 0.25f));
			}
		}
		else
		{
			AnglesToAxis (cent->lerpAngles, ent.axis);
		}

		CG_AddRefEntityWithPowerups ((const refEntity_t *)&ent, s, TEAM_FREE);
	}
}

static void JKG_BounceSpecialGrenade ( const centity_t *cent, const weaponDrawData_t *weaponData )
{
	if ( weaponData->grenadeBounce.grenade.bounceSound[0] )
	{
		trap->S_StartSound (NULL, cent->currentState.number, CHAN_BODY, weaponData->grenadeBounce.grenade.bounceSound[Q_irand (0, 1)]);
	}
}

static void JKG_RenderGenericProjectileMiss ( const centity_t* cent, const weaponDrawData_t *weaponData, const ammo_t* ammoData, const vec3_t origin, const vec3_t normal )
{
	if (JKG_SimpleAmmoOverridePresent(ammoData->visualOverrides.projectile.impactEffect))
	{
		trap->FX_PlayEffectID(theFxScheduler.RegisterEffect(ammoData->visualOverrides.projectile.impactEffect.second.c_str()),
			(float*) origin, (float*) normal, -1, -1, false);
	}
	else if ( weaponData->projectileMiss.generic.impactEffect )
	{
		trap->FX_PlayEffectID (weaponData->projectileMiss.generic.impactEffect, (float *)origin, (float *)normal, -1, -1, false);
	}
}

static void JKG_RenderGenericProjectileDeath (const centity_t* cent, const weaponDrawData_t *weaponData, const ammo_t* ammoData, const vec3_t origin, const vec3_t normal )
{
	if (JKG_SimpleAmmoOverridePresent(ammoData->visualOverrides.projectile.deathEffect))
	{
		trap->FX_PlayEffectID(theFxScheduler.RegisterEffect(ammoData->visualOverrides.projectile.deathEffect.second.c_str()), 
			(float*)origin, (float*)normal, -1, -1, false);
	}
	else if ( weaponData->projectileRender.generic.deathEffect )
	{
		trap->FX_PlayEffectID (weaponData->projectileRender.generic.deathEffect, (float *)origin, (float *)normal, -1, -1, false);
	}
}

static void JKG_RenderGenericProjectileHitPlayer ( const weaponDrawData_t *weaponData, const ammo_t* ammoData, const vec3_t origin, const vec3_t normal )
{
	if (JKG_SimpleAmmoOverridePresent(ammoData->visualOverrides.projectile.impactEffect))
	{
		trap->FX_PlayEffectID(theFxScheduler.RegisterEffect(ammoData->visualOverrides.projectile.impactEffect.second.c_str()),
			(float*)origin, (float*)normal, -1, -1, false);
	}
	else if ( weaponData->projectileHitPlayer.generic.impactEffect )
	{
		trap->FX_PlayEffectID (weaponData->projectileHitPlayer.generic.impactEffect, (float *)origin, (float *)normal, -1, -1, false);
	}
}

static void JKG_RenderExplosiveProjectileMiss ( const centity_t *cent, const weaponDrawData_t *weaponData, const ammo_t* ammo, const vec3_t origin, const vec3_t normal )
{
	if ( weaponData->projectileMiss.explosive.stickSound )
	{
		trap->S_StartSound (NULL, cent->currentState.number, CHAN_WEAPON, weaponData->projectileMiss.explosive.stickSound);
	}
}

static void JKG_RenderGrenadeProjectileMiss ( const centity_t *cent, const weaponDrawData_t *weaponData, const ammo_t* ammoData, const vec3_t origin, const vec3_t normal )
{
	if ( weaponData->projectileMiss.grenade.impactEffect )
	{
		trap->FX_PlayEffectID (weaponData->projectileMiss.grenade.impactEffect, (float *)origin, (float *)normal, -1, -1, false);
	}

	if ( weaponData->projectileMiss.grenade.shockwaveEffect )
	{
		trap->FX_PlayEffectID (weaponData->projectileMiss.grenade.shockwaveEffect, (float *)origin, (float *)normal, -1, -1, false);
	}
}

static void JKG_RenderGrenadeProjectileHitPlayer ( const weaponDrawData_t *weaponData, const ammo_t* ammoData, const vec3_t origin, const vec3_t normal )
{
	if ( weaponData->projectileHitPlayer.grenade.impactEffect )
	{
		trap->FX_PlayEffectID (weaponData->projectileHitPlayer.grenade.impactEffect, (float *)origin, (float *)normal, -1, -1, false);
	}

	if ( weaponData->projectileMiss.grenade.shockwaveEffect )
	{
		trap->FX_PlayEffectID (weaponData->projectileMiss.grenade.shockwaveEffect, (float *)origin, (float *)normal, -1, -1, false);
	}
}

static void JKG_RenderGenericProjectileDeflected (const weaponDrawData_t *weaponData, const ammo_t* ammoData, const vec3_t origin, const vec3_t normal )
{
	if (JKG_SimpleAmmoOverridePresent(ammoData->visualOverrides.projectile.deflectEffect))
	{
		trap->FX_PlayEffectID( theFxScheduler.RegisterEffect(ammoData->visualOverrides.projectile.deflectEffect.second.c_str()),
			(float*)origin, (float*)normal, -1, -1, qfalse);
	}
	else if ( weaponData->projectileDeflected.generic.deflectEffect )
	{
		trap->FX_PlayEffectID (weaponData->projectileDeflected.generic.deflectEffect, (float *)origin, (float *)normal, -1, -1, false);
	}
}

static void JKG_ChargeGenericWeapon ( const centity_t *cent, const weaponDrawData_t *weaponData )
{
	if ( weaponData->weaponCharge.chargingSound )
	{
		trap->S_StartSound (NULL, cent->currentState.number, CHAN_WEAPON, weaponData->weaponCharge.chargingSound);
	}
}

static void JKG_GetMuzzleLocation ( centity_t *cent, const vec3_t angles, vec3_t origin, vec3_t direction )
{
	mdxaBone_t boltMatrix;

	if ( !trap->G2API_HasGhoul2ModelOnIndex (&cent->ghoul2, 1) )
	{
		// No weapon model on this player
		return;
	}

	if ( !trap->G2API_GetBoltMatrix (cent->ghoul2, 1, 0, &boltMatrix, angles, cent->lerpOrigin, cg.time, cgs.gameModels, cent->modelScale) )
	{
		// Couldn't find the muzzle bolt
		return;
	}

	if ( origin != NULL )
	{
		BG_GiveMeVectorFromMatrix (&boltMatrix, ORIGIN, origin);
	}

	if ( direction != NULL )
	{
		BG_GiveMeVectorFromMatrix (&boltMatrix, POSITIVE_X, direction);
	}
}

#define FX_USE_ALPHA (0x08000000)
static __inline void JKG_RenderChargingEffect ( centity_t *cent, const vec3_t muzzlePosition, vec3_t *axis, fxHandle_t chargingEffect, qboolean isLocalPlayer, qboolean isFirstPerson, int startedChargingTime )
{
	float time = (cg.time - startedChargingTime) * 0.01f;

	if ( time < 0.0f )
	{
		time = 0.0f;
	}
	else if ( time > 1.0f )
	{
		time = 1.0f;
		if ( isLocalPlayer )
		{
			CGCam_Shake (0.2f, 100);
		}
	}
	else
	{
		if ( isLocalPlayer )
		{
			CGCam_Shake (time * time * 0.6f, 100);
		}
	}

	if ( isFirstPerson )
	{
		trap->FX_PlayEntityEffectID (chargingEffect, const_cast<float *>(muzzlePosition), axis, -1, -1, -1, -1);
	}
	else
	{
		trap->FX_PlayEntityEffectID(chargingEffect, const_cast<float *>(muzzlePosition), axis, cent->boltInfo, cent->currentState.number, -1, -1);
	}
}

void JKG_RenderGenericWeaponWorld ( centity_t *cent, const weaponDrawData_t *weaponData, const ammo_t* ammoData, unsigned char firingMode, const vec3_t angles )
{
	const entityState_t *s = &cent->currentState;
	qboolean isLocalPlayer = (qboolean)(s->number == cg.predictedPlayerState.clientNum);
	fxHandle_t chargeEffect = weaponData->weaponRender.generic.chargingEffect;
	fxHandle_t muzzleEffect = weaponData->weaponRender.generic.muzzleEffect;
	double muzzleLightIntensity = weaponData->weaponRender.generic.muzzleLightIntensity;

	qboolean hasMuzzleLocation = qfalse;
	vec3_t flashOrigin, flashDirection;

	if (JKG_SimpleAmmoOverridePresent(ammoData->visualOverrides.chargingEffect))
	{
		chargeEffect = theFxScheduler.RegisterEffect(ammoData->visualOverrides.chargingEffect.second.c_str());
	}

	if (JKG_SimpleAmmoOverridePresent(ammoData->visualOverrides.muzzleEffect))
	{
		muzzleEffect = theFxScheduler.RegisterEffect(ammoData->visualOverrides.muzzleEffect.second.c_str());
	}

	JKG_ApplyAmmoOverride(muzzleLightIntensity, ammoData->visualOverrides.muzzleLightIntensity);

	// Do muzzle charge effects
	if ( !isLocalPlayer || cg.renderingThirdPerson || cg_trueguns.integer )
	{
		if ( s->modelindex2 == WEAPON_CHARGING && weaponData->weaponRender.generic.chargingEffect )
		{
			hasMuzzleLocation = qtrue;
			JKG_GetMuzzleLocation (cent, angles, flashOrigin, flashDirection);

			matrix3_t flashAxis;
			AnglesToAxis(cent->lerpAngles, flashAxis);

			JKG_RenderChargingEffect (
				cent,
				flashOrigin,
				flashAxis,
				chargeEffect,
				isLocalPlayer,
				qfalse,
				s->constantLight
			);
		}
	}

	VectorClear (cg.lastFPFlashPoint);

	if ( !cent->shotCountOverflowed && cent->shotCount <= cent->muzzleFlashCount )
	{
		return;
	}

	cent->muzzleFlashCount = cent->shotCount;
	cent->shotCountOverflowed = qfalse;

	// Do muzzle flash
	if ( !isLocalPlayer || cg.renderingThirdPerson || cg_trueguns.integer )
	{
		if ( !hasMuzzleLocation )
		{
			JKG_GetMuzzleLocation (cent, angles, flashOrigin, flashDirection);
		}

		if ( weaponData->weaponRender.generic.muzzleEffect )
		{
			matrix3_t axis;
			int boltNum = trap->G2API_AddBolt(cent->ghoul2, 1, "*flash");
			AnglesToAxis(cent->lerpAngles, axis);
			trap->FX_PlayEntityEffectID(muzzleEffect,
				flashOrigin, axis, cent->boltInfo, cent->currentState.number, -1, -1);
		}

		if ( muzzleLightIntensity > 0.0f )
		{
			if (JKG_SimpleAmmoOverridePresent(ammoData->visualOverrides.muzzleLightColor))
			{
				trap->R_AddLightToScene(
					flashOrigin,
					muzzleLightIntensity + (rand() & 31),
					ammoData->visualOverrides.muzzleLightColor.second[0],
					ammoData->visualOverrides.muzzleLightColor.second[1],
					ammoData->visualOverrides.muzzleLightColor.second[2]
				);
			}
			else
			{
				trap->R_AddLightToScene(
					flashOrigin,
					muzzleLightIntensity + (rand() & 31),
					weaponData->weaponRender.generic.muzzleLightColor[0],
					weaponData->weaponRender.generic.muzzleLightColor[1],
					weaponData->weaponRender.generic.muzzleLightColor[2]
				);
			}
		}
	}
}

static void JKG_RenderGenericTraceline ( const weaponDrawData_t *weaponData, const ammo_t* ammoData, const vec3_t start, const vec3_t end )
{
	qhandle_t shader = weaponData->tracelineRender.generic.tracelineShader;
	double minSize = weaponData->tracelineRender.generic.minSize;
	double maxSize = weaponData->tracelineRender.generic.maxSize;
	int lifeTime = weaponData->tracelineRender.generic.lifeTime;

	if (JKG_SimpleAmmoOverridePresent(ammoData->visualOverrides.traceline.shader))
	{
		shader = cgi_R_RegisterShader(ammoData->visualOverrides.traceline.shader.second.c_str());
	}

	JKG_ApplyAmmoOverride(minSize, ammoData->visualOverrides.traceline.minSize);
	JKG_ApplyAmmoOverride(maxSize, ammoData->visualOverrides.traceline.maxSize);
	JKG_ApplyAmmoOverride(lifeTime, ammoData->visualOverrides.traceline.lifeTime);

	if ( shader )
	{
		static const vec3_t WHITE = { 1.0f, 1.0f, 1.0f };

		trap->FX_AddLine (
			start, end,
			minSize,
			maxSize,
			0.0f,
			1.0f, 0.0f, 0.0f,
			WHITE, WHITE, 0.0f,
			lifeTime,
			shader,
			FX_SIZE_LINEAR | FX_ALPHA_LINEAR
		);
	}
}

static void JKG_PrepareExplosiveModel ( refEntity_t *ent, const centity_t *cent, void *g2Model, float radius )
{
	const entityState_t *s = &cent->currentState;

	memset (ent, 0, sizeof (*ent));

	memcpy (ent->shaderRGBA, s->customRGBA, sizeof (ent->shaderRGBA));

	ent->oldframe = ent->frame = s->frame;

	ent->ghoul2 = g2Model;
	ent->radius = radius;

	VectorCopy (cent->lerpOrigin, ent->origin);
	VectorCopy (cent->lerpOrigin, ent->oldorigin);

	VectorCopy (cent->lerpAngles, ent->angles);

	AnglesToAxis (cent->lerpAngles, ent->axis);
}

static __inline qboolean JKG_IsTripMineArmed ( const centity_t *cent )
{
	const entityState_t *s = &cent->currentState;

	return (s->eFlags & EF_FIRING) && s->time == -1 && s->bolt2 == 1;
}

static __inline qboolean JKG_TripMineHasBeam ( const centity_t *cent )
{
	return cent->currentState.time == -1;
}

static void JKG_RenderTripmineExplosive ( const centity_t *cent, const weaponDrawData_t *weaponData, unsigned char firingMode )
{
	if ( weaponData->explosiveRender.tripmine.g2Model )
	{
		refEntity_t ent;

		JKG_PrepareExplosiveModel (
			&ent,
			cent,
			weaponData->explosiveRender.tripmine.g2Model,
			weaponData->explosiveRender.tripmine.g2Radius
		);

		if ( firingMode )
		{
			if ( !JKG_IsTripMineArmed (cent) )
			{
				trap->R_AddRefEntityToScene (&ent);
			}

			ent.renderfx &= ~RF_FORCE_ENT_ALPHA;
			ent.renderfx |= RF_RGB_TINT;
			ent.shaderRGBA[0] = ent.shaderRGBA[1] = ent.shaderRGBA[2] = 255;
			ent.shaderRGBA[3] = 0;
			ent.customShader = cgs.media.cloakedShader;
		}

		trap->R_AddRefEntityToScene (&ent);

		if ( cent->currentState.owner != cg.snap->ps.clientNum && !TeamFriendly (cent->currentState.owner) && !(cg.snap->ps.fd.forcePowersActive & (1 << FP_SEE)) )
		{
			return;
		}

		if ( weaponData->explosiveRender.tripmine.lineEffect )
		{
			if ( JKG_TripMineHasBeam (cent) )
			{
				if ( !(((cg.time >> 10) & 1) &&
					TeamFriendly (cent->currentState.owner) &&
					cent->currentState.owner != cg.snap->ps.clientNum) )
				{
					vec3_t beamOrigin;
					int i;
					int max = 1;

					

					VectorMA (ent.origin, 6.6f, ent.axis[0], beamOrigin);

					if ( JKG_IsTripMineArmed (cent) && cg.snap->ps.fd.forcePowersActive & (1 << FP_SEE) )
					{
						max += cg.snap->ps.fd.forcePowerLevel[FP_SEE] * 2;
					}

					for ( i = 0; i < max; i++ )
					{
						trap->FX_PlayEffectID (
							weaponData->explosiveRender.tripmine.lineEffect,
							beamOrigin,
							(float *)cent->currentState.pos.trDelta,
							-1, -1,
							false
						);
					}
				}
			}
		}
	}
}

static void JKG_RenderDetpackExplosive ( const centity_t *cent, const weaponDrawData_t *weaponData, unsigned char firingMode )
{
	if ( weaponData->explosiveRender.detpack.g2Model )
	{
		refEntity_t ent;

		JKG_PrepareExplosiveModel (
			&ent,
			cent,
			weaponData->explosiveRender.detpack.g2Model,
			weaponData->explosiveRender.detpack.g2Radius
		);

		trap->R_AddRefEntityToScene (&ent);
	}
}

static void JKG_BlowGenericExplosive ( const centity_t *cent, const weaponDrawData_t *weaponData )
{
	if ( weaponData->explosiveBlow.generic.explodeEffect )
	{
		const entityState_t *s = &cent->currentState;
		vec3_t forward;

		VectorCopy (s->angles, forward);

		if ( VectorNormalize2 (forward, forward) == 0.0f )
		{
			forward[1] = 1.0f;
		}

		trap->FX_PlayEffectID (weaponData->explosiveBlow.generic.explodeEffect, (float *)s->origin, forward, -1, -1, false);
	}
}

static void JKG_ArmGenericExplosive ( const centity_t *cent, const weaponDrawData_t *weaponData )
{
	if ( weaponData->explosiveArm.armSound )
	{
		trap->S_StartSound (NULL, cent->currentState.number, CHAN_WEAPON, weaponData->explosiveArm.armSound);
	}
}

qboolean JKG_ShouldRenderWeaponViewModel ( const centity_t *cent, const playerState_t *ps )
{
	if ( ps->persistant[PERS_TEAM] == TEAM_SPECTATOR )
	{
		return qfalse;
	}

	if ( ps->pm_type == PM_INTERMISSION || ps->pm_type == PM_SPECTATOR )
	{
		return qfalse;
	}

	if ( cg.renderingThirdPerson )
	{
		return qfalse;
	}

	if ( !cg.renderingThirdPerson && cg_trueguns.integer && ps->weapon != WP_SABER && ps->weapon != WP_MELEE )
	{
		return qfalse;
	}

	if ( cg.testGun )
	{
		return qfalse;
	}

	if ( ps->zoomMode )
	{
		return qfalse;
	}

	return qtrue;
}

static void JKG_RenderGenericWeaponView ( const weaponDrawData_t *weaponData )
{
	static const char *barrelTags[] = {
		"tag_barrel",
		"tag_barrel2",
		"tag_barrel3",
		"tag_barrel4"
	};

	const playerState_t *ps = &cg.predictedPlayerState;
	centity_t *cent = &cg_entities[ps->clientNum];
	const entityState_t *s = &cent->currentState;

	const weaponInfo_t *weapon = NULL;

	float fov = cg_fov.value;
	float fovOffset = 0.0f;

	vec3_t gunPosition;

	refEntity_t hand;
	refEntity_t gun;
	refEntity_t muzzle;

	int i;
	refEntity_t barrel;

	if ( !JKG_ShouldRenderWeaponViewModel (cent, ps) )
	{
		return;
	}

	if ( !cg_drawGun.integer )
	{
		return;
	}

	// Calculate fov
	fov = CG_ClampFov (fov);
	if ( fov > 90.0f )
	{
		fovOffset = -0.2f * (fov - 90.0f);
	}

	// Position hand correctly on-screen
	weapon = CG_WeaponInfo (ps->weapon, ps->weaponVariation);

	if ( !weapon->viewModel )
	{
		return;
	}

	// Set up basic stuff for the muzzle, so that we can use the muzzle position in our calculations for bob/sway
	memset (&gun, 0, sizeof (gun));


	// Calculate the position of the hand
	memset (&hand, 0, sizeof (hand));
	CG_CalculateWeaponPosition (hand.origin, hand.angles);

	// Offset the gun if necessary
	gunPosition[0] = abs (cg_gunX.value) > FLT_EPSILON ? cg_gunX.value : weapon->gunPosition[0];
	gunPosition[1] = abs (cg_gunY.value) > FLT_EPSILON ? cg_gunY.value : weapon->gunPosition[1];
	gunPosition[2] = abs (cg_gunZ.value) > FLT_EPSILON ? cg_gunZ.value : weapon->gunPosition[2];

	float phase = JKG_CalculateIronsightsPhase (ps, cg.time, &cg.ironsightsBlend);
	vec3_t s2;
	VectorSubtract (weapon->ironsightsPosition, gunPosition, s2);

	VectorMA (gunPosition, phase, s2, gunPosition);

	VectorMA (hand.origin, gunPosition[0], cg.refdef.viewaxis[0], hand.origin);
	VectorMA (hand.origin, gunPosition[1], cg.refdef.viewaxis[1], hand.origin);
	VectorMA (hand.origin, gunPosition[2] + (fovOffset * (1 - phase)), cg.refdef.viewaxis[2], hand.origin);

	AnglesToAxis (hand.angles, hand.axis);

	// Set the correct animation frame
	if ( cg_gun_frame.integer )
	{
		hand.frame = hand.oldframe = cg_gun_frame.integer;
		hand.backlerp = 0.0f;
	}
	else
	{
		const clientInfo_t *ci;

		// Xy: The original code checked if the current player is an NPC. I'm
		// thinking this was just left over from the SP code, so I've removed it
		// but added an assertion here just in case it's still needed.
		assert (cent->currentState.eType != ET_NPC);

		ci = &cgs.clientinfo[s->clientNum];

		if( cent->ghoul2 )
		{
			float currentFrame, animSpeed;
			int startFrame,endFrame,flags; //Filler data to make the trap call not go kaboom
			trap->G2API_GetBoneAnim(cent->ghoul2, "lower_lumbar", cg.time, &currentFrame, &startFrame, &endFrame, &flags, &animSpeed, 0, 0);
			hand.frame = CG_MapTorsoToWeaponFrame( ci, ceil(currentFrame), ps->torsoAnim );
			hand.oldframe = CG_MapTorsoToWeaponFrame( ci, floor(currentFrame), ps->torsoAnim );
			hand.backlerp = 1.0f - (currentFrame-floor(currentFrame));
		}

		if ( hand.frame == -1 )
		{
			hand.frame = hand.oldframe = 0;
			hand.backlerp = 0.0f;
		}
		else if ( hand.oldframe == -1 )
		{
			hand.oldframe = hand.frame;
			hand.backlerp = 0.0f;
		}
	}

	// Render hands with gun
	hand.hModel = weapon->handsModel;
	hand.renderfx = RF_DEPTHHACK | RF_FIRST_PERSON;

	gun.renderfx = hand.renderfx;
	gun.hModel = weapon->viewModel;

	CG_PositionEntityOnTag (&gun, &hand, hand.hModel, "tag_weapon");
	if ( !CG_IsMindTricked (s->trickedentindex,
		s->trickedentindex2, s->trickedentindex3,
		s->trickedentindex4, ps->clientNum) )
	{
		JKG_WeaponDebuffVisuals(cent, &gun);
		CG_AddWeaponWithPowerups (&gun, s->powerups);
	}

	// Draw barrel models if any
	for ( i = 0; i < 4; i++ )
	{
		if ( weapon->barrelModels[i] == NULL_HANDLE ) break;

		memset (&barrel, 0, sizeof (barrel));
		barrel.renderfx = hand.renderfx;
		barrel.hModel = weapon->barrelModels[i];

		AnglesToAxis (vec3_origin, barrel.axis);
		CG_PositionRotatedEntityOnTag (&barrel, &hand, hand.hModel, barrelTags[i]);

		JKG_WeaponDebuffVisuals(cent, &barrel);
		CG_AddWeaponWithPowerups (&barrel, s->powerups);
	}

	memset (&muzzle, 0, sizeof (muzzle));
	CG_PositionEntityOnTag (&muzzle, &gun, gun.hModel, "tag_flash");

	VectorCopy (muzzle.origin, cg.lastFPFlashPoint);

	if ( s->modelindex2 == WEAPON_CHARGING && weaponData->weaponRender.generic.chargingEffect )
	{
		JKG_RenderChargingEffect (
			cent,
			muzzle.origin,
			muzzle.axis,
			weaponData->weaponRender.generic.chargingEffect,
			qtrue,
			qtrue,
			s->constantLight);
	}

	// TODO: At some point, I want to put this into a common function which the
	// world model and view model can call. For now it's just copy/pasted.
	// Do muzzle flash
	if ( !cent->shotCountOverflowed && cent->shotCount <= cent->muzzleFlashCount )
	{
		return;
	}

	cent->muzzleFlashCount = cent->shotCount;
	cent->shotCountOverflowed = qfalse;

	if ( weaponData->weaponRender.generic.muzzleEffect )
	{
		trap->FX_PlayEntityEffectID (
			weaponData->weaponRender.generic.muzzleEffect,
			muzzle.origin, muzzle.axis,
			-1, -1, -1, -1
		);
	}

	if ( weaponData->weaponRender.generic.muzzleLightIntensity > 0.0f )
	{
		trap->R_AddLightToScene (
			muzzle.origin,
			weaponData->weaponRender.generic.muzzleLightIntensity + (rand() & 31),
			weaponData->weaponRender.generic.muzzleLightColor[0],
			weaponData->weaponRender.generic.muzzleLightColor[1],
			weaponData->weaponRender.generic.muzzleLightColor[2]
		);
	}
}
*/
//=========================================================
// Event handlers
//=========================================================
static const weaponEventsHandler_t wpEventsTable[] =
{
	// Does anyone mind me listing these vertically? I don't think we'll have that many of them here
	// so it shouldn't take up too much screen space.
	
	/*{
		"blaster",
		JKG_RenderGenericWeaponWorld,
		JKG_RenderGenericWeaponView,
		JKG_FireBlaster,
		JKG_ChargeGenericWeapon,
		JKG_RenderGenericTraceline,
		NULL,
		NULL,
		NULL,
		NULL,
		JKG_RenderGenericProjectile,
		JKG_RenderGenericProjectileMiss,
		JKG_RenderGenericProjectileDeath,
		JKG_RenderGenericProjectileHitPlayer,
		JKG_RenderGenericProjectileDeflected
	},

	{
		"grenade",
		JKG_RenderGenericWeaponWorld,
		JKG_RenderGenericWeaponView,
		JKG_FireBlaster,
		JKG_ChargeGenericWeapon,
		NULL,
		JKG_BounceSpecialGrenade,
		NULL,
		NULL,
		NULL,
		JKG_RenderGenericProjectile,
		JKG_RenderGrenadeProjectileMiss,
		NULL,
		JKG_RenderGrenadeProjectileHitPlayer,
		NULL
	},

	{
		"tripmine",
		JKG_RenderGenericWeaponWorld,
		JKG_RenderGenericWeaponView,
		NULL,
		NULL,
		NULL,
		NULL,
		JKG_RenderTripmineExplosive,
		JKG_BlowGenericExplosive,
		JKG_ArmGenericExplosive,
		NULL,
		JKG_RenderExplosiveProjectileMiss,
		NULL,
		JKG_RenderGenericProjectileHitPlayer,
		NULL
	},

	{
		"detpack",
		JKG_RenderGenericWeaponWorld,
		JKG_RenderGenericWeaponView,
		NULL,
		NULL,
		NULL,
		NULL,
		JKG_RenderDetpackExplosive,
		JKG_BlowGenericExplosive,
		NULL,
		NULL,
		JKG_RenderExplosiveProjectileMiss,
		NULL,
		JKG_RenderGenericProjectileHitPlayer,
		NULL
	},

	{
		"laserproj",
		JKG_RenderGenericWeaponWorld,
		JKG_RenderGenericWeaponView,
		NULL,
		NULL,
		NULL,
		NULL,
		JKG_RenderDetpackExplosive,
		NULL,
		JKG_ArmGenericExplosive,
		NULL,
		JKG_RenderExplosiveProjectileMiss,
		NULL,
		JKG_RenderGenericProjectileHitPlayer,
		NULL
	},
	*/
	// End sentinel
	{ NULL }
};

void GWSetWeaponEventsHandler ( GWInfo_t *weaponInfo, const char *eventHandlerName, unsigned char firingMode )
{
	const weaponEventsHandler_t *wpEventHandler = wpEventsTable;
	int found = 0;


	weaponInfo->eventsHandler[firingMode] = NULL;

	while( !found && wpEventHandler->handlerName != NULL )
	{
		if ( !weaponInfo->eventsHandler[firingMode] &&
			Q_stricmp (eventHandlerName, wpEventHandler->handlerName) == 0 )
		{
			weaponInfo->eventsHandler[firingMode] = wpEventHandler;
			found++;
		}
		wpEventHandler++;
	}

	if( !found )
	{
		gi.Printf ("Weapon %d: invalid firing mode %i event handler \"%s\".\n", weaponInfo->weaponNum, firingMode, eventHandlerName);
	}
}

void CG_RegisterWeapon( int weaponNum, int variation ) 
{
	GWInfo_t	*GWInfo;
	const weaponData_t *weaponData;
	gitem_t			*item/*, *ammo*/;
	
	char			path[MAX_QPATH];
	vec3_t			mins, maxs;
	int				i;

	if ( weaponNum == 0 ) {
		return;
	}
	
	GWInfo = 0; // QQQ IDENTIFICATOR NOT FOUND... WeaponInfoUnsafe (weaponNum, variation);
	if ( GWInfo )
	{
	    // Non-null means the weapon exists.
	    return;
	}

    GWInfo = NextFreeWeaponInfo();
	if ( GWInfo == NULL ) 
	{
	    gi.Printf ( "ERR_DROP, Max weapon info slots exceeded.\n" );
		return;
	}
	
	#ifdef _DEBUG
	//gi.Printf ("Registering weapon %d variation %d\n", weaponNum, variation);
	#endif

	memset( GWInfo, 0, sizeof( *GWInfo ) );
	GWInfo->registered = qtrue;
	GWInfo->weaponNum = weaponNum;
	GWInfo->variation = variation;

	for ( item = bg_itemlist + 1 ; item->classname ; item++ ) {
		if ( item->giType == IT_WEAPON && item->giTag == weaponNum ) {
			GWInfo->item = item;
			break;
		}
	}
	if ( !item->classname ) 
	{
		gi.Printf( "ERR_DROP ,  Couldn't find weapon %i", weaponNum );
	}
	CG_RegisterItemVisuals( item - bg_itemlist );
	
	weaponData = GetWeaponData (weaponNum, variation);

	// load cmodel before model so filecache works
	if (weaponData->visuals.world_model[0] != '\0') { // Don't spam the console.
		GWInfo->weaponModel = cgi_R_RegisterModel(weaponData->visuals.world_model);
	}
	// load in-view model also
	GWInfo->viewModel = NULL_HANDLE;
	if ( GWInfo->g2ViewModel )
	{
		gi.G2API_JKGCleanGhoul2Models (&GWInfo->g2ViewModel);
	    GWInfo->g2ViewModel = NULL;
	}
		
	if ( CG_IsGhoul2Model (weaponData->visuals.view_model) )
	{
	   // GW_LoadViewWeapon (GWInfo, weaponData->visuals.view_model);
	}
	else if (weaponData->visuals.view_model[0] != '\0')
	{
		GWInfo->viewModel = cgi_R_RegisterModel(weaponData->visuals.view_model);
	}

	// calc midpoint for rotation
	if (GWInfo->weaponModel != NULL_HANDLE) {
		cgi_R_ModelBounds(GWInfo->weaponModel, mins, maxs);
		for (i = 0; i < 3; i++) {
			GWInfo->weaponMidpoint[i] = mins[i] + 0.5 * (maxs[i] - mins[i]);
		}
	}

	GWInfo->hudIcon = cgi_R_RegisterShaderNoMip (weaponData->visuals.icon);
	GWInfo->hudNAIcon = cgi_R_RegisterShaderNoMip (weaponData->visuals.icon_na);

	if (weaponNum != WP_SABER)
	{
		strcpy( path, weaponData->visuals.view_model );
		COM_StripExtension( path, path, sizeof( path ) );
		strcat( path, "_hand.md3" );
		GWInfo->handsModel = cgi_R_RegisterModel( path );
	}
	else
	{
		GWInfo->handsModel = 0;
	}

	for(i = 0; i < weaponData->numFiringModes; i++)
	{
		// this only deals with them one at a time from now on --eez
		GWSetWeaponEventsHandler (GWInfo, weaponData->visuals.visualFireModes[i].type, i);
	}
    LoadWeaponAssets (GWInfo, weaponData);

	switch ( weaponNum ) {
	case WP_STUN_BATON:
	case WP_MELEE:
		theFxScheduler.RegisterEffect( "stunBaton/flesh_impact" );

		if (weaponNum == WP_STUN_BATON)
		{
			cgi_S_RegisterSound( "sound/weapons/baton/idle.wav" );
			GWInfo->flashSound[0] = cgi_S_RegisterSound( "sound/weapons/baton/fire.mp3" );
			GWInfo->altFlashSound[0] = cgi_S_RegisterSound( "sound/weapons/baton/fire.mp3" );
		}
		break;
	case WP_SABER:
		MAKERGB( GWInfo->flashDlightColor, 0.6f, 0.6f, 1.0f );
		GWInfo->firingSound = cgi_S_RegisterSound( "sound/weapons/saber/saberhum1.wav" );
		GWInfo->missileModel		= cgi_R_RegisterModel( "models/weapons2/saber/saber_w.glm" );
		break;

	case WP_TURRET:
		GWInfo->flashSound[0]		= NULL_SOUND;
		GWInfo->firingSound			= NULL_SOUND;
		GWInfo->chargeSound			= NULL_SOUND;
		GWInfo->muzzleEffect		= NULL_HANDLE;
		GWInfo->missileModel		= NULL_HANDLE;
		GWInfo->missileSound		= NULL_SOUND;
		GWInfo->missileDlight		= 0;
		GWInfo->missileHitSound		= NULL_SOUND;
		GWInfo->missileTrailFunc	 = TurretProjectileThink;

		theFxScheduler.RegisterEffect("effects/blaster/wall_impact.efx");
		theFxScheduler.RegisterEffect("effects/blaster/flesh_impact.efx");
		break;

	 default:
		MAKERGB( GWInfo->flashDlightColor, 1, 1, 1 );
		GWInfo->flashSound[0] = cgi_S_RegisterSound( "sound/weapons/rocket/rocklf1a.wav" );
		break;
	}
}



/*
void JKG_RenderWeaponWorldModel ( centity_t *cent, const vec3_t angles )
{
	const weaponInfo_t *weapon;
	const entityState_t *s = &cent->currentState;
	qboolean isLocalPlayer = (qboolean)(s->number == cg.predictedPlayerState.clientNum);
	playerState_t *ps = isLocalPlayer ? &cg.predictedPlayerState : NULL;

	if ( s->weapon == WP_EMPLACED_GUN )
	{
		return;
	}

	if ( isLocalPlayer && ps->pm_type == PM_SPECTATOR )
	{
		// Don't draw our own weapon if we're spectating.
		return;
	}

	JKG_WeaponIndicators_Update (cent, NULL);

	weapon = CG_WeaponInfo (s->weapon, s->weaponVariation);

	if ( weapon->eventsHandler[cent->currentState.firingMode] && weapon->eventsHandler[cent->currentState.firingMode]->WeaponRenderWorld )
	{
		weapon->eventsHandler[cent->currentState.firingMode]->WeaponRenderWorld( cent, &weapon->drawData[cent->currentState.firingMode], &ammoTable[cent->currentState.ammoType], cent->currentState.firingMode, angles );
	}
}

void JKG_RenderWeaponViewModel ( void )
{
	const weaponInfo_t *weapon;
	playerState_t *ps = &cg.predictedPlayerState;
	const centity_t *cent = &cg_entities[ps->clientNum];
	const entityState_t *s = &cent->currentState;

	if ( s->weapon == WP_EMPLACED_GUN )
	{
		return;
	}

	if ( !JKG_ShouldRenderWeaponViewModel (cent, ps) )
	{
		return;
	}

	JKG_WeaponIndicators_Update (cent, ps);

	weapon = CG_WeaponInfo (s->weapon, s->weaponVariation);

	if ( weapon->eventsHandler[ps->firingMode] && weapon->eventsHandler[ps->firingMode]->WeaponRenderView )
	{
		weapon->eventsHandler[ps->firingMode]->WeaponRenderView(&weapon->drawData[ps->firingMode]);
	}
}

void JKG_RenderProjectileHitPlayer ( const centity_t *cent, const vec3_t origin, const vec3_t direction, qboolean altFire )
{
	const entityState_t *s = &cent->currentState;
	const weaponInfo_t *weapon = CG_WeaponInfo (s->weapon, s->weaponVariation);

	if( weapon->eventsHandler[s->firingMode] && weapon->eventsHandler[s->firingMode]->ProjectileHitPlayer )
	{
		weapon->eventsHandler[s->firingMode]->ProjectileHitPlayer ( &weapon->drawData[s->firingMode], &ammoTable[cent->currentState.ammoType], origin, direction );
	}
}

void JKG_RenderProjectileMiss ( const centity_t *cent, const vec3_t origin, const vec3_t direction, qboolean altFire )
{
	const entityState_t *s = &cent->currentState;
	const weaponInfo_t *weapon = CG_WeaponInfo (s->weapon, s->weaponVariation);

	if( weapon->eventsHandler[s->firingMode] && weapon->eventsHandler[s->firingMode]->ProjectileMiss)
	{
		weapon->eventsHandler[s->firingMode]->ProjectileMiss ( cent, &weapon->drawData[s->firingMode], &ammoTable[cent->currentState.ammoType], origin, direction );
	}
}

void JKG_RenderProjectileDeath ( const centity_t *cent, const vec3_t origin, const vec3_t direction, unsigned char firingMode )
{
	const entityState_t *s = &cent->currentState;
	const weaponInfo_t *weapon = CG_WeaponInfo (s->weapon, s->weaponVariation);

	if( weapon->eventsHandler[firingMode] && weapon->eventsHandler[firingMode]->ProjectileDeath )
	{
		weapon->eventsHandler[firingMode]->ProjectileDeath ( cent, &weapon->drawData[firingMode], &ammoTable[cent->currentState.ammoType], origin, direction );
	}
}

void JKG_RenderProjectile ( const centity_t *cent, unsigned char firingMode )
{
	const entityState_t *s = &cent->currentState;
	const weaponInfo_t *weapon = CG_WeaponInfo (s->weapon, s->weaponVariation);

	if( weapon->eventsHandler[firingMode] && weapon->eventsHandler[firingMode]->ProjectileRender )
	{
		weapon->eventsHandler[firingMode]->ProjectileRender ( cent, &weapon->drawData[firingMode], &ammoTable[cent->currentState.ammoType]);
	}
}

void JKG_RenderTraceline ( const centity_t *cent, const vec3_t start, const vec3_t end, unsigned char firingMode )
{
	const entityState_t *s = &cent->currentState;
	const weaponInfo_t *weapon = CG_WeaponInfo (s->weapon, s->weaponVariation);

	if( weapon->eventsHandler[firingMode] && weapon->eventsHandler[firingMode]->TracelineRender )
	{
		weapon->eventsHandler[firingMode]->TracelineRender ( &weapon->drawData[firingMode], &ammoTable[cent->currentState.ammoType], start, end );
	}
}

void JKG_BounceGrenade ( const centity_t *cent, unsigned char firingMode )
{
	const entityState_t *s = &cent->currentState;
	const weaponInfo_t *weapon = CG_WeaponInfo (s->weapon, s->weaponVariation);

	if( weapon->eventsHandler[firingMode] && weapon->eventsHandler[firingMode]->GrenadeBounce )
	{
		weapon->eventsHandler[firingMode]->GrenadeBounce ( cent, &weapon->drawData[firingMode] );
	}
}

void JKG_RenderExplosive ( const centity_t *cent, unsigned char firingMode )
{
	const entityState_t *s = &cent->currentState;
	const weaponInfo_t *weapon = CG_WeaponInfo (s->weapon, s->weaponVariation);

	if( weapon->eventsHandler[firingMode] && weapon->eventsHandler[firingMode]->ExplosiveRender )
	{
		weapon->eventsHandler[firingMode]->ExplosiveRender ( cent, &weapon->drawData[firingMode], firingMode );
	}
}

void JKG_ArmExplosive ( const centity_t *cent, unsigned char firingMode )
{
	const entityState_t *s = &cent->currentState;
	const weaponInfo_t *weapon = CG_WeaponInfo (s->weapon, s->weaponVariation);

	if( weapon->eventsHandler[firingMode] && weapon->eventsHandler[firingMode]->ExplosiveArm )
	{
		weapon->eventsHandler[firingMode]->ExplosiveArm ( cent, &weapon->drawData[firingMode] );
	}
}

void JKG_ToggleScope ( const centity_t *cent )
{
	const entityState_t *s = &cent->currentState;
	const weaponInfo_t *weapon = CG_WeaponInfo (s->weapon, s->weaponVariation);

	if ( cg.predictedPlayerState.zoomMode )
	{
		if ( weapon->scopeStartSound )
		{
			trap->S_StartLocalSound (weapon->scopeStartSound, CHAN_AUTO);
		}
	}
	else
	{
		if ( weapon->scopeStopSound )
		{
			trap->S_StartLocalSound (weapon->scopeStopSound, CHAN_AUTO);
		}
	}
}

void JKG_RenderScope ( const centity_t *cent )
{
	const entityState_t *s = &cent->currentState;
	const weaponInfo_t *weapon = CG_WeaponInfo (s->weapon, s->weaponVariation);

	trap->R_SetColor (colorTable[CT_WHITE]);
	if ( weapon->scopeShader )
	{
		CG_DrawPic (0.0f, 0.0f, 640.0f, 480.0f, weapon->scopeShader);
	}
}

void JKG_ZoomScope ( const centity_t *cent )
{
	const entityState_t *s = &cent->currentState;
	const weaponInfo_t *weapon = CG_WeaponInfo (s->weapon, s->weaponVariation);

	static int zoomSoundTime = 0;

	if ( weapon->scopeLoopSound && weapon->scopeSoundLoopTime > 0 )
	{
		if ( zoomSoundTime < cg.time || zoomSoundTime > (cg.time + 10000) )
		{
			trap->S_StartSound (cg.refdef.vieworg, ENTITYNUM_WORLD, CHAN_LOCAL, weapon->scopeLoopSound);
			zoomSoundTime = cg.time + weapon->scopeSoundLoopTime;
		}
	}
}

void JKG_ChargeWeapon ( const centity_t *cent, qboolean altFire )
{
	const entityState_t *s = &cent->currentState;
	int weaponNum, variation;
	const weaponInfo_t *weapon;

	weaponNum = s->eventParm >> 8;
	variation = s->eventParm & 0xFF;

	weapon = CG_WeaponInfo (weaponNum, variation);

	if ( weapon->eventsHandler[cent->currentState.firingMode] && weapon->eventsHandler[cent->currentState.firingMode]->WeaponCharge )
	{
		weapon->eventsHandler[cent->currentState.firingMode]->WeaponCharge(cent, &weapon->drawData[cent->currentState.firingMode]);
	}
}

void JKG_BlowExplosive ( const centity_t *cent, qboolean altFire )
{
	const entityState_t *s = &cent->currentState;
	const weaponInfo_t *weapon = CG_WeaponInfo (s->weapon, s->weaponVariation);

	if ( weapon->eventsHandler[s->firingMode] && weapon->eventsHandler[s->firingMode] )
	{
		weapon->eventsHandler[s->firingMode]->ExplosiveBlow (cent, &weapon->drawData[s->firingMode]);
	}
}

void JKG_FireWeapon ( centity_t *cent, qboolean altFire )
{
	const weaponInfo_t *weapon;
	const entityState_t *s = &cent->currentState;

	if ( s->weapon == WP_NONE )
	{
		return;
	}

	if ( s->weapon >= WP_NUM_WEAPONS )
	{
		// FIXME: should check this in all weapons --eez
		trap->Error (ERR_DROP, "JKG_FireWeapon: entityState_t::weapon >= WP_NUM_WEAPONS");
		return;
	}

	if( s->number == cg.clientNum )
	{
		// It's our gun. do the vibration stuff.
		weaponData_t	*weaponData = GetWeaponData( s->weapon, s->weaponVariation );
		float damageModifier = (weaponData->firemodes[s->firingMode].baseDamage)/100.0f;

		CLAMP(damageModifier, 0.3f, 1.0f);
	}

	weapon = CG_WeaponInfo (s->weapon, s->weaponVariation);

	if ( weapon->eventsHandler[s->firingMode] && weapon->eventsHandler[s->firingMode]->WeaponFire )
	{
		weapon->eventsHandler[s->firingMode]->WeaponFire (cent, &weapon->drawData[s->firingMode], s->firingMode);
	}
}*/


